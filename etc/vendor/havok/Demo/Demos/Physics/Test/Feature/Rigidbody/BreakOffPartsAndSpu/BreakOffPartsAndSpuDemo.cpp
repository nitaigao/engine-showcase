/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/Physics/Test/Feature/Rigidbody/BreakOffPartsAndSpu/BreakOffPartsAndSpuDemo.h>

#include <Physics/Collide/Shape/Compound/Collection/List/hkpListShape.h>
#include <Physics/Collide/Shape/Convex/ConvexTranslate/hkpConvexTranslateShape.h>
#include <Physics/Collide/Shape/Convex/ConvexTransform/hkpConvexTransformShape.h>
#include <Physics/Collide/Shape/Compound/Collection/ExtendedMeshShape/hkpExtendedMeshShape.h>
#include <Physics/Collide/Shape/Compound/Tree/Mopp/hkpMoppBvTreeShape.h>
#include <Physics/Collide/Shape/Compound/Tree/Mopp/hkpMoppUtility.h>

#include <Physics/Dynamics/World/hkpPhysicsSystem.h>

#include <Graphics/Common/DisplayObject/hkgDisplayObject.h>
#include <Graphics/Common/Geometry/VertexSet/hkgVertexSet.h>
#include <Graphics/Bridge/DisplayHandler/hkgDisplayHandler.h>

#include <Physics/Collide/Shape/Compound/Tree/Mopp/hkpMoppUtility.h>

#include <Physics/Utilities/VisualDebugger/Viewer/Collide/hkpMidphaseViewer.h>

#include <Demos/DemoCommon/Utilities/GameUtils/GameUtils.h>
#include <Physics/Dynamics/Entity/Util/hkpEntityAabbUtil.h>
#include <Physics/Collide/Agent/hkpProcessCollisionInput.h>

// options:
// list, meshShape, 
// with/without mopp

enum BreakOffPartsAndSpuDemoType
{
	BREAK_OFF_PARTS_DEMO_SIMPLE,
	BREAK_OFF_PARTS_DEMO_COLLISION_RESPONSE,
	BREAK_OFF_PARTS_DEMO_TOI,
};

struct BreakOffPartsAndSpuVariant
{
	const char*					m_name;
	BreakOffPartsAndSpuDemoType		m_type;
	hkBool32					m_useMeshCollection;
	hkBool32					m_useMopp;
	hkBool32					m_extraListInHierarchy;
	hkBool32					m_fixOneBody;
	const char*					m_details;
};



static const BreakOffPartsAndSpuVariant g_variants[] =
{
	{ "List - Breaking Off",			BREAK_OFF_PARTS_DEMO_TOI,				 0, 0, 0, 0, "Note that we don't update the physics properties (inertia, center of mass) hence the reduced shapes look unnaturally." },
	{ "List - Breaking Off a Fixed Body",BREAK_OFF_PARTS_DEMO_TOI,				 0, 0, 0, 1, "" },
	{ "List x 2 levels - Breaking Off",	BREAK_OFF_PARTS_DEMO_TOI,				 0, 0, 1, 0, "" },
	{ "List+Mopp - Breaking Off",		BREAK_OFF_PARTS_DEMO_TOI,				 0, 1, 0, 0, "(This looks bad, because we don't remove shape keys from the wrapping mopp.)" },
	{ "Mesh - No breaking",				BREAK_OFF_PARTS_DEMO_TOI,				 1, 0, 0, 0, "" },
	{ "Mesh+Mopp - No breaking",		BREAK_OFF_PARTS_DEMO_TOI,				 1, 1, 0, 0, "" },
};


BreakOffPartsAndSpuDemo::BreakOffPartsAndSpuDemo(hkDemoEnvironment* env)
:	hkDefaultPhysicsDemo(env)
{
	m_list0 = m_list1 = HK_NULL;

	m_debugViewerNames.pushBack( hkpMidphaseViewer::getName() );

	m_bootstrapIterations = 150;

	const BreakOffPartsAndSpuVariant& variant = g_variants[m_variantId];

	//
	// Setup the camera
	//
	{
		hkVector4 from(0.0f, 35.0f, -50.0f);
		hkVector4 to  (0.0f, 0.0f,  0.0f);
		hkVector4 up  (0.0f, 1.0f,  0.0f);
		setupDefaultCameras( env, from, to, up );
	}


	//
	// Create the world
	//
	{
		hkpWorldCinfo info;

		info.m_gravity.set(0.0f, -9.8f, 0.0f);

		if ( variant.m_type == BREAK_OFF_PARTS_DEMO_COLLISION_RESPONSE)
		{
			info.m_contactRestingVelocity = 0.5f;
		}

		m_world = new hkpWorld( info );
		m_world->lock();

		hkpAgentRegisterUtil::registerAllAgents(m_world->getCollisionDispatcher());
		setupGraphics();
	}

	//
	//	Create the break off utility
	//
	{
		m_breakUtil = new hkpBreakOffPartsUtil( m_world, this );
	}


	// Create ground
	//
	{
		hkVector4 size (100.0f, 1.0f, 100.0f);
		hkReal mass = 0.0f;
		hkVector4 position (0.0f, -0.5f, 0.0f);
		hkpRigidBody* ground = GameUtils::createBox(size, mass, position);
		m_world->addEntity(ground);
		ground->removeReference();
	}

	// Create two big shapes
	//
	{
		hkVector4 size(0.5f, 0.5f, 0.5f);
		hkReal radius = 0.02f;
		hkReal dist = size(0)*2.0f + radius + 0.1f;
		
		hkArray<hkpConvexShape*> shapes; shapes.reserve(400);
		{
			hkpBoxShape* terminal = new hkpBoxShape(size, radius);
			int dim = 20;
			for (int i = 0; i < dim; i++)
			{
				for (int j = (i+1)%2; j < dim; j+=2)
				{
					hkTransform transform = hkTransform::getIdentity();
					hkReal offset = (hkReal(dim)-1.0f)*-0.5f;
					transform.setTranslation(hkVector4( (offset+hkReal(i))*dist, (offset+hkReal(j))*dist, 0.0f));
					hkpConvexTransformShape* cts = new hkpConvexTransformShape(terminal, transform);
					shapes.pushBack(cts);
				}
			}
			terminal->removeReference();
		}

		// Create new list/mesh shape
		//
		hkpShapeCollection* collectionShape0;
		hkpShapeCollection* collectionShape1;
		if (!variant.m_useMeshCollection)
		{
			collectionShape0 = m_list0 = new hkpListShape(reinterpret_cast<hkpShape**>(static_cast<hkpConvexShape**>(shapes.begin())), shapes.getSize());
			collectionShape1 = m_list1 = new hkpListShape(reinterpret_cast<hkpShape**>(static_cast<hkpConvexShape**>(shapes.begin())), shapes.getSize());
			if (variant.m_extraListInHierarchy)
			{
				collectionShape0 = new hkpListShape((hkpShape**)&collectionShape0, 1);
				collectionShape1 = new hkpListShape((hkpShape**)&collectionShape1, 1);
				m_list0->removeReference();
				m_list1->removeReference();
			}
		}
		else
		{
			// This is a share shape because it's not destructible
			hkpExtendedMeshShape* meshShape = new hkpExtendedMeshShape(radius);
			collectionShape0 = meshShape;
			collectionShape1 = meshShape;
			hkpExtendedMeshShape::ShapesSubpart part(shapes.begin(), shapes.getSize());
			meshShape->addShapesSubpart(part);
			// Add a reference to meshShape.
			// collectionShape0 and collectionShape1 both reference it, which means there has to be a second reference to remove later.
			meshShape->addReference();
		}
		hkReferencedObject::removeReferences(shapes.begin(), shapes.getSize());

		// Wrap it with a mopp
		//
		hkpShape* finalShape0 = collectionShape0;
		hkpShape* finalShape1 = collectionShape1;
		if (variant.m_useMopp)
		{
			hkpMoppCompilerInput mci;
			mci.m_enableChunkSubdivision = true;
			hkpMoppCode* code0 = hkpMoppUtility::buildCode( collectionShape0->getContainer(),mci);
			hkpMoppCode* code1 = hkpMoppUtility::buildCode( collectionShape1->getContainer(),mci);

			finalShape0 = new hkpMoppBvTreeShape( collectionShape0, code0 );
			finalShape1 = new hkpMoppBvTreeShape( collectionShape1, code1 );

			code0->removeReference();
			code1->removeReference();
			collectionShape0->removeReference();
			collectionShape1->removeReference();
		}

		// finally add create two bodies with the shape
		//
		{
			hkpRigidBodyCinfo info;
			info.m_shape = finalShape0;
			hkpInertiaTensorComputer::setShapeVolumeMassProperties(finalShape0, 10.0f, info);
			info.m_motionType = hkpMotion::MOTION_DYNAMIC;
			info.m_numUserDatasInContactPointProperties = 1;
			info.m_position.set(0.0f, 15.0f, 0.0f);

			m_body0 = new hkpRigidBody(info);

			info.m_rotation.setAxisAngle(hkVector4(0.0f,1.0f, 0.0f), 90.0f * HK_REAL_DEG_TO_RAD);
			info.m_position(0) += dist;
			info.m_shape = finalShape1;

			if (variant.m_fixOneBody)
			{
				info.m_motionType = hkpMotion::MOTION_FIXED;
			}

			m_body1 = new hkpRigidBody(info);

			finalShape0->removeReference();
			finalShape1->removeReference();

			m_world->addEntity(m_body0);
			m_world->addEntity(m_body1); 

			
		}

	}

	// Mark bodies breakable
	//
	if (variant.m_useMeshCollection != 1 && variant.m_extraListInHierarchy != 1)
	{
		m_breakUtil->markEntityBreakable(m_body0, 1.0f);
		m_breakUtil->markEntityBreakable(m_body1, 1.0f);
	}


	m_world->unlock();
}


void BreakOffPartsAndSpuDemo::removeSubShapeFromDisplay(hkpRigidBody* body, hkpListShape* listShape, int shapeKey)
{
		// get the vertex set
	hkgVertexSet* vertexSet;
	{
		int geometryIndex = shapeKey;
		const hkpCollidable* collidable = body->getCollidable();
		hkgDisplayObject* displayObject = m_env->m_displayHandler->findDisplayObject(hkUlong(collidable));
		hkgGeometry* geometry = displayObject->getGeometry(geometryIndex);
		hkgFaceSet* faceSet = geometry->getMaterialFaceSet(0)->getFaceSet(0);
		vertexSet = faceSet->getVertexSet();
	}

		// Simply zero-out all vertices. This will degenerate the triangles but that should be ok.
	{
		m_env->m_window->getContext()->lock();
		vertexSet->lock(HKG_LOCK_WRITEDISCARD);
		hkVector4 newPos(0,0,0);
		for (int i = 0; i < vertexSet->getNumVerts(); i++)
		{
			vertexSet->setVertexComponentData(HKG_VERTEX_COMPONENT_POS, i, &newPos(0));
		}
		vertexSet->unlock();
		m_env->m_window->getContext()->unlock();
	}
}

hkResult BreakOffPartsAndSpuDemo::breakOffSubPart( const ContactImpulseLimitBreachedEvent& event, hkArray<hkpShapeKey>& keysBrokenOffOut, hkpPhysicsSystem& systemOut ) 
{
	const BreakOffPartsAndSpuVariant& variant = g_variants[m_variantId];

		//	
		//	Remove the broken pieces from the car or landscape
		//
	hkpRigidBody* breakingBody = event.m_breakingBody;

	// This disables this listener for variants, with an additional toplevel list shape.
	//
	{	
		hkpShape* shape = const_cast<hkpShape*>(breakingBody->getCollidable()->getShape());
		{
			hkpListShape* list = static_cast<hkpListShape*>(shape);
			if (list->getType() == HK_SHAPE_LIST && list->m_childInfo.getSize() == 1)
			{
				return HK_FAILURE;
			}
		}
		hkpMoppBvTreeShape* moppTree = static_cast<hkpMoppBvTreeShape*>(shape);
		if (moppTree->getType() == HK_SHAPE_MOPP)
		{
			hkpShapeCollection* collection = const_cast<hkpShapeCollection*>(moppTree->getShapeCollection());

			HK_ASSERT2(0xad875a22, collection->getType() == HK_SHAPE_LIST, "The container must be a list shape..");
			hkpListShape* list = static_cast<hkpListShape*>(collection);

			if (list->m_childInfo.getSize() == 1 )
			{
				return HK_FAILURE;
			}

		}

	}

	for (int p = 0; p < event.m_points.getSize(); p++)
	{
		const ContactImpulseLimitBreachedEvent::PointInfo& pointInfo = event.m_points[p];

		hkpShapeKey brokenPieceKey = pointInfo.m_brokenShapeKey;

		if (brokenPieceKey >= hkpListShape::MAX_DISABLED_CHILDREN)
		{
			continue;
		}

		const hkpShape* newShape = HK_NULL;
		{
			hkpShape* shape = const_cast<hkpShape*>(breakingBody->getCollidable()->getShape());
			switch (shape->m_type )
			{
			case HK_SHAPE_LIST:
				{
					hkpListShape* list = static_cast<hkpListShape*>(shape);

					newShape = list->m_childInfo[brokenPieceKey].m_shape;

					hkpBreakOffPartsUtil::removeKeysFromListShape( breakingBody, &brokenPieceKey, 1 );

					removeSubShapeFromDisplay( event.m_breakingBody, list, brokenPieceKey );
					keysBrokenOffOut.pushBack( brokenPieceKey );

					break;
				}
			case HK_SHAPE_MOPP:
				{
					hkpMoppBvTreeShape* moppTree = static_cast<hkpMoppBvTreeShape*>(shape);
					hkpShapeCollection* collection = const_cast<hkpShapeCollection*>(moppTree->getShapeCollection());

					HK_ASSERT2(0xad875a22, collection->getType() == HK_SHAPE_LIST, "The container must be a list shape..");
					hkpListShape* list = static_cast<hkpListShape*>(collection);
					newShape = list->m_childInfo[brokenPieceKey].m_shape;

					hkpBreakOffPartsUtil::removeKeysFromListShape( breakingBody, &brokenPieceKey, 1 );

					removeSubShapeFromDisplay( event.m_breakingBody, list, brokenPieceKey );
					keysBrokenOffOut.pushBack( brokenPieceKey );

					break;
				}
			default:
				HK_ASSERT2( 0xf03df569, 0, "This shape is not implemented yet" );
				return HK_FAILURE;
			}
		}

			//
			//	Create the new broken off piece
			//
		hkpRigidBodyCinfo rigidBodyCinfo;
		{
			rigidBodyCinfo.m_shape = newShape;
			rigidBodyCinfo.m_position        = breakingBody->getPosition();
			rigidBodyCinfo.m_rotation        = breakingBody->getRotation();
			rigidBodyCinfo.m_linearVelocity  = breakingBody->getLinearVelocity(); 
			rigidBodyCinfo.m_angularVelocity = breakingBody->getAngularVelocity();	
			rigidBodyCinfo.m_mass = 10.0f;
			rigidBodyCinfo.m_qualityType = (variant.m_type == BREAK_OFF_PARTS_DEMO_TOI) ? HK_COLLIDABLE_QUALITY_MOVING : HK_COLLIDABLE_QUALITY_DEBRIS;
			hkpInertiaTensorComputer::setShapeVolumeMassProperties( newShape, rigidBodyCinfo.m_mass, rigidBodyCinfo );
		}

		hkReferencedObject::lockAll();
		hkpRigidBody* newBody = new hkpRigidBody( rigidBodyCinfo );

		if (newShape->getContainer() && newShape->getContainer()->getNumChildShapes() > 1)
		{
			m_breakUtil->markEntityBreakable(newBody, 1.0f);
			newBody->m_numUserDatasInContactPointProperties = 1;
		}

		systemOut.addRigidBody( newBody );
		newBody->removeReference();
		hkReferencedObject::unlockAll();


			// if the original unbroken body was fixed, the colliding impulse is lost, simply apply the impulse to the new piece
		if ( breakingBody->isFixedOrKeyframed() )
		{
			hkReal maxImpulse = pointInfo.m_properties->m_maxImpulse;
			hkVector4 impulse; impulse.setMul4( -maxImpulse, pointInfo.m_contactPoint->getNormal() );
			newBody->applyPointImpulse( impulse, pointInfo.m_contactPoint->getPosition() );
		}		
	}

	return HK_SUCCESS;
}


hkDemo::Result BreakOffPartsAndSpuDemo::stepDemo()
{
	hkpListShape* lists[] = {m_list0, m_list1};
	hkpRigidBody* bodies[] = {m_body0, m_body1};
	for (int i = 0; i < 2; i++)
	{
		hkpListShape* list = lists[i];
		if (list)// && list->getNumChildShapes() > 1)
		{
			hkpShapeKey key = list->getFirstKey();
			if (i)
			{
				key = list->getNextKey(key); // leave the first key in. on one shape -- for testing.
			}
			if (key < hkpListShape::MAX_DISABLED_CHILDREN)
			{
				list->disableChild(key);
				if (bodies[i]->isFixed())
				{
					
					// update aabbs this way:
					//
					   bodies[i]->markForWrite();
					   hkpBreakOffPartsUtil::removeKeysFromListShape( bodies[i], &key, 1 );
					   bodies[i]->unmarkForWrite();
					//
					// or this way:
					//
					//   m_world->markForWrite();
					//   hkpEntityAabbUtil::entityBatchRecalcAabb(m_world->getCollisionInput(), (hkpEntity**)bodies+i, 1);
					//   m_world->unmarkForWrite();
				}

				// remove child from graphics
				bodies[i]->markForWrite();
				removeSubShapeFromDisplay( bodies[i], list, key );
				bodies[i]->unmarkForWrite();
			}
		}
	}

	return hkDefaultPhysicsDemo::stepDemo();
}

BreakOffPartsAndSpuDemo::~BreakOffPartsAndSpuDemo()
{
	m_world->lock();

	m_body0->removeReference();
	m_body1->removeReference();

	m_breakUtil->removeReference();

	m_world->unlock();
}


#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
"A demo which shows how to brake of single parts from a compound body, e.g. a mirror from a car and a street lamppost";

//HK_DECLARE_DEMO_VARIANT_USING_STRUCT( BreakOffPartsAndSpuDemo, HK_DEMO_TYPE_PHYSICS | HK_DEMO_TYPE_CRITICAL, BreakOffPartsVariant, g_variants, helpString );
HK_DECLARE_DEMO_VARIANT_USING_STRUCT( BreakOffPartsAndSpuDemo, HK_DEMO_TYPE_PHYSICS, BreakOffPartsAndSpuVariant, g_variants, helpString );

/*
* Havok SDK - NO SOURCE PC DOWNLOAD, BUILD(#20090216)
* 
* Confidential Information of Havok.  (C) Copyright 1999-2009
* Telekinesys Research Limited t/a Havok. All Rights Reserved. The Havok
* Logo, and the Havok buzzsaw logo are trademarks of Havok.  Title, ownership
* rights, and intellectual property rights in the Havok software remain in
* Havok and/or its suppliers.
* 
* Use of this software for evaluation purposes is subject to and indicates
* acceptance of the End User licence Agreement for this product. A copy of
* the license is included with this software and is also available at www.havok.com/tryhavok.
* 
*/
