/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/Physics/Api/Dynamics/RigidBodies/BreakOffParts/BreakOffPartsDemo.h>

#include <Physics/Collide/Shape/Compound/Collection/List/hkpListShape.h>
#include <Physics/Collide/Shape/Convex/ConvexTranslate/hkpConvexTranslateShape.h>

#include <Physics/Dynamics/World/hkpPhysicsSystem.h>

#include <Graphics/Common/DisplayObject/hkgDisplayObject.h>
#include <Graphics/Common/Geometry/VertexSet/hkgVertexSet.h>
#include <Graphics/Bridge/DisplayHandler/hkgDisplayHandler.h>

#include <Physics/Collide/Shape/Compound/Tree/Mopp/hkpMoppUtility.h>


enum BreakOffPartsDemoType
{
	BREAK_OFF_PARTS_DEMO_SIMPLE,
	BREAK_OFF_PARTS_DEMO_COLLISION_RESPONSE,
	BREAK_OFF_PARTS_DEMO_TOI,
};

struct BreakOffPartsVariant
{
	const char*					m_name;
	BreakOffPartsDemoType		m_type;
	const char*					m_details;
};



static const BreakOffPartsVariant g_variants[] =
{
	{ "Break Off Using TOI Collisions",		BREAK_OFF_PARTS_DEMO_TOI,				"A car mirror is hitting a wall in a TOI collisions" },
	{ "Break Off Using CollisionResponse",	BREAK_OFF_PARTS_DEMO_COLLISION_RESPONSE,"A car mirror is hitting a wall using the extended collision response" },
	{ "Break Off Using Default Collisions",	BREAK_OFF_PARTS_DEMO_SIMPLE,			"A car mirror is hitting a wall using the default no TOI collisions" },
};


BreakOffPartsDemo::BreakOffPartsDemo(hkDemoEnvironment* env)
:	hkDefaultPhysicsDemo(env)
{
	m_bootstrapIterations = 150;

	const BreakOffPartsVariant& variant = g_variants[m_variantId];

	//
	// Setup the camera
	//
	{
		hkVector4 from(0.0f, 7.0f, -10.0f);
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


	//
	// Create the ground base with 2 breakable pillars and 2 unbreakable walls
	//
	{
		hkpShape* shape;
		{
			hkpConvexShape* ground = new hkpBoxShape( hkVector4( 15.0f, 0.3f, 15.0f) , 0 );
			hkpConvexShape* pilar  = new hkpBoxShape( hkVector4(   .1f, 0.3f,   .1f) , 0 );
			hkpConvexShape* box    = new hkpBoxShape( hkVector4(   .2f, 0.3f,   .5f) , 0 );


			const hkpShape* shapes[5] = { 
				new hkpConvexTranslateShape( ground,hkVector4( 0.0f, -0.3f, 0.0f )),
				new hkpConvexTranslateShape( pilar, hkVector4(-1.5f,  0.3f, 0.95f )),
				new hkpConvexTranslateShape( pilar, hkVector4(-3.0f,  0.3f,- .95f )),
				new hkpConvexTranslateShape( box,   hkVector4( 1.5f,   .5f, 1.2f )),
				new hkpConvexTranslateShape( box,   hkVector4( 3.0f,   .5f,-1.2f )),
			};

			int numShapes = sizeof(shapes)/sizeof(shapes[0]);
			hkpListShape* list = new hkpListShape(shapes, numShapes);

			{ for (int i =0; i < numShapes; i++) { shapes[i]->removeReference(); } }
			ground->removeReference();
			pilar ->removeReference();
			box   ->removeReference();

			// Create mopp around the list shape

			hkpMoppCompilerInput mci;
			mci.m_enableChunkSubdivision = true;
			hkpMoppCode* code = hkpMoppUtility::buildCode( list,mci);

			shape = new hkpMoppBvTreeShape( list, code );

			code->removeReference();
			list->removeReference();
		}

		hkpRigidBodyCinfo ci;
		ci.m_shape = shape;
		ci.m_motionType = hkpMotion::MOTION_FIXED;
		ci.m_numUserDatasInContactPointProperties = 1;

		hkpEntity* ground = m_world->addEntity( new hkpRigidBody( ci ) );
		ground->removeReference();
		shape->removeReference();

		// make the two pillars breakable
		m_breakUtil->markPieceBreakable( ground, 1, 100.0f );
		m_breakUtil->markPieceBreakable( ground, 2, 100.0f );
	}	


	//
	//	Create our car with 2 mirrors
	//
	hkpRigidBody* car;
	{
		hkpShape* shape;
		{
			hkpConvexShape* mirror = new hkpBoxShape( hkVector4(.1f, .1f, .2f), 0 );

			const hkpShape* shapes[3] =	{
				new hkpConvexTranslateShape( mirror, hkVector4 ( .8f, .1f,  .7f )),
				new hkpConvexTranslateShape( mirror, hkVector4 ( .8f, .1f, -.7f )),
				new hkpBoxShape( hkVector4( 2.0f, .2f, .5f), 0 )
			};

			int numShapes = sizeof(shapes)/sizeof(shapes[0]);
			hkpListShape* list = new hkpListShape(shapes, numShapes);
			
			{ for (int i =0; i < numShapes; i++) { shapes[i]->removeReference(); } }
			mirror->removeReference();

			// Create mopp around the list shape

			hkpMoppCompilerInput mci;
			mci.m_enableChunkSubdivision = true;
			hkpMoppCode* code = hkpMoppUtility::buildCode( list ,mci);

			shape = new hkpMoppBvTreeShape( list, code );

			code->removeReference();
			list->removeReference();
		}

	
		hkpRigidBodyCinfo ci;
		{
			ci.m_shape = shape;
			ci.m_qualityType = HK_COLLIDABLE_QUALITY_DEBRIS;
			if ( variant.m_type == BREAK_OFF_PARTS_DEMO_TOI)
			{
				ci.m_qualityType = HK_COLLIDABLE_QUALITY_MOVING;
			}
			ci.m_motionType = hkpMotion::MOTION_BOX_INERTIA;
			ci.m_restitution = 1.0f;
			ci.m_friction = 0.0f;
			ci.m_position.set( -15.0f, .2f, 0.0f );
			ci.m_linearVelocity.set( 10.0f, 0.0f, 0.0f );
			ci.m_numUserDatasInContactPointProperties = 1;
			hkpInertiaTensorComputer::setShapeVolumeMassProperties( shape, 1000.0f, ci );
		}

		car = new hkpRigidBody(ci);
		m_world->addEntity( car );
		car->removeReference();		// Remove reference, since we no longer want to remember this
		shape->removeReference();
	}

	// make the two mirrors breakable
	{
		m_breakUtil->markPieceBreakable(car, 0, 1000.0f ); // left mirror
		m_breakUtil->markPieceBreakable(car, 1, 1000.0f ); // right mirror
	}

	m_world->unlock();
}


void BreakOffPartsDemo::removeSubShapeFromDisplay(hkpRigidBody* body, hkpListShape* listShape, int shapeKey)
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

hkResult BreakOffPartsDemo::breakOffSubPart( const ContactImpulseLimitBreachedEvent& event, hkArray<hkpShapeKey>& keysBrokenOffOut, hkpPhysicsSystem& systemOut ) 
{
	const BreakOffPartsVariant& variant = g_variants[m_variantId];

		//	
		//	Remove the broken pieces from the car or landscape
		//
	hkpRigidBody* breakingBody = event.m_breakingBody;

	for (int p = 0; p < event.m_points.getSize(); p++)
	{
		const ContactImpulseLimitBreachedEvent::PointInfo& pointInfo = event.m_points[p];

		hkpShapeKey brokenPieceKey = pointInfo.m_brokenShapeKey;

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


hkDemo::Result BreakOffPartsDemo::stepDemo()
{
	return hkDefaultPhysicsDemo::stepDemo();
}

BreakOffPartsDemo::~BreakOffPartsDemo()
{
	m_world->lock();

	m_breakUtil->removeReference();

	m_world->unlock();
}


#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
"A demo which shows how to brake of single parts from a compound body, e.g. a mirror from a car and a street lamppost";

//HK_DECLARE_DEMO_VARIANT_USING_STRUCT( BreakOffPartsDemo, HK_DEMO_TYPE_PHYSICS | HK_DEMO_TYPE_CRITICAL, BreakOffPartsVariant, g_variants, helpString );
HK_DECLARE_DEMO_VARIANT_USING_STRUCT( BreakOffPartsDemo, HK_DEMO_TYPE_PHYSICS, BreakOffPartsVariant, g_variants, helpString );

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
