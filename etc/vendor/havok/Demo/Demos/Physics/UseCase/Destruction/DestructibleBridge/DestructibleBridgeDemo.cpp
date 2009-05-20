/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>

#include <Demos/Physics/UseCase/Destruction/DestructibleBridge/DestructibleBridgeDemo.h>
#include <Demos/Physics/UseCase/Destruction/DestructibleBridge/DestructibleBridgeUtil.h>
#include <Demos/Physics/UseCase/Destruction/DestructibleBridge/DestructibleMoppUtility.h>

#include <Physics/Collide/Agent/hkpProcessCollisionInput.h>
#include <Physics/Collide/Query/Collector/PointCollector/hkpClosestCdPointCollector.h>
#include <Physics/Collide/Dispatch/hkpCollisionDispatcher.h>
#include <Physics/Collide/Shape/Compound/Tree/Mopp/Modifiers/hkpRemoveTerminalsMoppModifier.h>
#include <Physics/Collide/Shape/Convex/Sphere/hkpSphereShape.h>
#include <Physics/Collide/Shape/Convex/ConvexTranslate/hkpConvexTranslateShape.h>
#include <Physics/Collide/Shape/Convex/ConvexTransform/hkpConvexTransformShape.h>
#include <Physics/Collide/Shape/Misc/Transform/hkpTransformShape.h>
#include <Physics/Collide/Shape/Compound/Tree/Mopp/hkpMoppUtility.h>
#include <Physics/Collide/Shape/Compound/Collection/ExtendedMeshShape/hkpExtendedMeshShape.h>

#include <Physics/Dynamics/World/hkpPhysicsSystem.h>
#include <Physics/Dynamics/Collide/hkpCollisionListener.h>
#include <Physics/Dynamics/Entity/hkpEntityListener.h>

#include <Graphics/Common/DisplayObject/hkgDisplayObject.h>
#include <Graphics/Common/Geometry/VertexSet/hkgVertexSet.h>

#include <Graphics/Bridge/DisplayHandler/hkgDisplayHandler.h>



#define PLANE_ORIGIN_RADIUS 80.0f



struct DestructibleBridgeVariant
{
	const char*					m_name;
	DestructibleBridgeDemoType	m_type;
	int							m_numPlanes;
	const char*					m_details;
};



static const DestructibleBridgeVariant g_variants[] =
{
	{ "2 Planes",					DESTRUCTIBLE_BRIDGE_DEMO_TYPE_CRASH,	 2, "2 airplanes crashing into a bridge." },
	{ "10 Planes",					DESTRUCTIBLE_BRIDGE_DEMO_TYPE_CRASH,	 10, "10 airplanes crashing into a bridge." },
	{ "4 in a Row",					DESTRUCTIBLE_BRIDGE_DEMO_TYPE_FLYOVER,	 4, "4 airplanes in a row" },
};




DestructibleBridgeDemo::DestructibleBridgeDemo(hkDemoEnvironment* env)
	:	hkDefaultPhysicsDemo(env)
{

	const DestructibleBridgeVariant& variant = g_variants[m_variantId];

	//
	// Setup the camera
	//
	{
		hkVector4 from( 0.0f, 20.0f, -160.0f);
		hkVector4 to  ( 0.0f,  0.0f,    0.0f);
		hkVector4 up  ( 0.0f,  1.0f,    0.0f);
		setupDefaultCameras(env, from, to, up);
	}

	//
	// Create the world
	//
	{
		hkpWorldCinfo worldInfo;
		{
			worldInfo.m_gravity.set(0.0f, -9.81f, 0.0f);
			worldInfo.setBroadPhaseWorldSize(1000.0f);
		}
		m_world = new hkpWorld(worldInfo);
		m_world->lock();

		// Register ALL agents (though some may not be necessary)
		hkpAgentRegisterUtil::registerAllAgents(m_world->getCollisionDispatcher());
		setupGraphics();
	}

	//
	//	Create the break off utility
	//
	{
		m_breakUtil = new hkpBreakOffPartsUtil( m_world, this );
	}

	// create the terrain and the bridge(s)
	setupTerrain();

	// create the airplanes
	setupAirplanes(variant.m_type, variant.m_numPlanes);

	m_world->unlock();
}

DestructibleBridgeDemo::~DestructibleBridgeDemo()
{
	m_breakUtil->removeReference();
	m_terrainData->removeReference();
}


void DestructibleBridgeDemo::setupTerrain()
{
	//
	// create the terrain
	//
	{
		DestructibleBridgeUtil::TerrainData data;
		{
			data.m_maxHeight		= 20.0f;
			data.m_numTiles			= 50;
			data.m_canyonWidthRatio	= 0.5f;
			data.m_smoothBreak		= false;
			data.m_scale			. set(3.0f, 2.0f, 3.0f);
			data.m_position			. set(0.0f, 0.0f, 0.0f);
			data.m_numBridges		= 6;
			data.m_maxBreakOffImpulse = 100.0f;
		}

		hkpRigidBody* terrain = DestructibleBridgeUtil::createTerrainAndBridges(m_breakUtil, data, m_terrainData );
		m_world->addEntity(terrain);
		terrain->removeReference();
	}
}

void DestructibleBridgeDemo::setupAirplanes(DestructibleBridgeDemoType type, int numPlanes)
{
	if ( type == DESTRUCTIBLE_BRIDGE_DEMO_TYPE_FLYOVER )
	{
		hkReal wingSpan = 10.0f;
		hkReal deltaPos = wingSpan * 1.2f;
		hkReal xPos = -60.0f;

		for (int i=0; i<numPlanes; i++)
		{
			DestructibleBridgeUtil::AirplaneData data;
			{
				data.m_wingSpan			= wingSpan;
				data.m_position			. set( xPos, 1.0f, 0.0f);
				data.m_destination		. set(-xPos, 1.0f, 0.0f);
				data.m_velocity			= 70.0f;
				data.m_mass				= 100.0f;
			}

			hkpRigidBody* airplane = DestructibleBridgeUtil::createAirplane(data);
			m_world->addEntity(airplane);
			airplane->removeReference();
			xPos -= deltaPos;
		}
	}
	else
	{
		hkReal deltaAngle = ((360.0f / numPlanes) / 180.0f)*HK_REAL_PI;
		hkReal angle = 0.0f;

		for (int i=0; i<numPlanes; i++)
		{
			hkReal xPos = hkMath::cos(angle) * PLANE_ORIGIN_RADIUS;
			hkReal zPos = hkMath::sin(angle) * PLANE_ORIGIN_RADIUS;

			DestructibleBridgeUtil::AirplaneData data;
			{
				data.m_wingSpan			= 10.0f;
				data.m_position			. set(xPos,  10.0f, zPos);
				data.m_destination		. set(0.0f, 0.0f, 0.0f);
				data.m_velocity			= 70.0f;
				data.m_mass				= 100.0f;
			}

			hkpRigidBody* airplane = DestructibleBridgeUtil::createAirplane(data);
			m_world->addEntity(airplane);
			airplane->removeReference();
			angle += deltaAngle;
		}
	}
}


void DestructibleBridgeDemo::removeSubShapeFromDisplay(hkpRigidBody* moppRigidBody, hkpMoppBvTreeShape* moppShape, int subShapeKey)
{
	//
	// calculate the shape's index in the hkgDisplayObject's geometry array
	//
	int geometryIndex;
	{
		const hkpExtendedMeshShape* meshShape = static_cast<const hkpExtendedMeshShape*>( moppShape->getShapeCollection() );

		hkpExtendedMeshShape::SubpartType type = meshShape->getSubpartType(subShapeKey);
		int subpartIndex = meshShape->getSubPartIndex(subShapeKey);

		geometryIndex = 0;

		if ( type == hkpExtendedMeshShape::SUBPART_TRIANGLES )
		{
			geometryIndex = subpartIndex;
		}
		else // type == hkpExtendedMeshShape::SUBPART_SHAPE
		{
			// skip all triangle subparts
			geometryIndex += meshShape->getNumTrianglesSubparts();

			// loop over all previous shape subparts
			{
				for (int i=0; i<subpartIndex; i++)
				{
					geometryIndex += meshShape->getShapesSubpartAt(i).m_numChildShapes;
				}
			}
			geometryIndex += meshShape->getTerminalIndexInSubPart(subShapeKey);
		}
	}


	//
	// get the terrain's vertex set
	//
	hkgVertexSet* vertexSet;
	{
		const hkpCollidable* collidable = moppRigidBody->getCollidable();
		hkgDisplayObject* displayObject = m_env->m_displayHandler->findDisplayObject(hkUlong(collidable));
		hkgGeometry* geometry = displayObject->getGeometry(geometryIndex);
		hkgFaceSet* faceSet = geometry->getMaterialFaceSet(0)->getFaceSet(0);
		vertexSet = faceSet->getVertexSet();
	}

	m_env->m_window->getContext()->lock();

	// vertex set has to be locked before any vertices can be manipulated
	vertexSet->lock(HKG_LOCK_WRITEDISCARD);

	{
		//
		// Simply zero-out all vertices. This will degenerate the triangles but that should be ok.
		//
		hkVector4 newPos(0,0,0);
		for (int i = 0; i < vertexSet->getNumVerts(); i++)
		{
			vertexSet->setVertexComponentData(HKG_VERTEX_COMPONENT_POS, i, &newPos(0));
		}
	}

	// unlock the vertex set again
	vertexSet->unlock();

	m_env->m_window->getContext()->unlock();

	return;
}


hkResult DestructibleBridgeDemo::breakOffSubPart( const ContactImpulseLimitBreachedEvent& event, hkArray<hkpShapeKey>& keysBrokenOffOut, hkpPhysicsSystem& systemOut ) 
{
		// get the MOPP shape
	hkpRigidBody* body = event.m_breakingBody;
	hkpMoppBvTreeShape* moppShape;
	{
		const hkpShape* shape = body->getCollidable()->getShape();
		HK_ASSERT2(0xafef4321, shape->getType() == HK_SHAPE_MOPP, "DestructibleMoppUtility can only be attached to MOPPs.");
		moppShape = const_cast<hkpMoppBvTreeShape*>(static_cast<const hkpMoppBvTreeShape*>(shape));
	}

	//	for (int p = 0; p < event.m_points.getSize(); p++)
    for (int p = 0; p < 1; p++)     // use only one point ignore the others
	{
		const ContactImpulseLimitBreachedEvent::PointInfo& pointInfo = event.m_points[p];

			// calc impact position in mopp space
		hkVector4 impactPosMoppSpace; impactPosMoppSpace.setTransformedInversePos( body->getTransform(), pointInfo.m_contactPoint->getPosition() );
		hkReal impactRadius= 5.0f;


		hkBool keyAlreadyBrokenOff = false;
		for ( int keyIt = 0; keyIt < keysBrokenOffOut.getSize(); keyIt++ )
		{
			if ( keysBrokenOffOut[keyIt] == pointInfo.m_brokenShapeKey )
			{
				keyAlreadyBrokenOff = true;
				break;
			}
		}
		if ( keyAlreadyBrokenOff == true )
		{		
			//TODO: apply impulse from collision to the rigid body
			continue;
		}
			//
			// grep objects within a given impact sphere
			//
		hkArray<hkpShapeKey>& keys = keysBrokenOffOut;
		hkInplaceArray<DestructibleMoppUtility::ConvexShapeData,64> shapes;
		{
			DestructibleMoppUtility::collectAllConvexChildShapesWithinRadius( moppShape, impactPosMoppSpace, impactRadius, *m_world->getCollisionInput(), shapes, keys );
		}

			//
			// remove those objects from mopp and display
			//
		{
			for (int i =0; i < keys.getSize();i++){ removeSubShapeFromDisplay( body, moppShape, keys[i] ); }
			DestructibleMoppUtility::removeKeysFromMopp( moppShape, impactPosMoppSpace, impactRadius, keys );
		}


			// merge shapes which are further away to make things more interesting
		DestructibleMoppUtility::ShapeData mergedShape;
		{
			hkInplaceArray<DestructibleMoppUtility::ConvexShapeData,32> mergedShapes;
				// decide which shapes to merge
			for (int i = shapes.getSize()-1; i>=0; i-- )
			{
				DestructibleMoppUtility::ConvexShapeData& data = shapes[i];
				if ( data.m_contactDetails.getDistance() > 0.8f )
				{
					mergedShapes.pushBack( data );
					shapes.removeAt( i );
				}
			}
			DestructibleMoppUtility::mergeShapes( mergedShapes, mergedShape );
		}


			//
			// create new pieces from single and merged shape
			//
		for (int i = shapes.getSize()-1; i>=-1; i-- )
		{
			const hkpShape* shape;
			hkReal massFactor = 1.0f;
			const hkTransform* transform;
			{
				if ( i >=0 )
				{
					DestructibleMoppUtility::ConvexShapeData& data = shapes[i];
					transform = &data.m_transform;
					shape     = data.m_shape;
				}
				else
				{
					transform = &mergedShape.m_transform;
					shape     = mergedShape.m_shape;
					if (!shape)	{	continue;		}
					massFactor = 5.0f;
				}
			}

			hkTransform transformWorldSpace;	transformWorldSpace.setMul(body->getTransform(), *transform);

			hkpRigidBodyCinfo info;
			{
				info.m_shape				= shape;
				info.m_position				= transformWorldSpace.getTranslation();
				info.m_rotation				. set(transformWorldSpace.getRotation());
				info.m_motionType			= hkpMotion::MOTION_DYNAMIC;
				info.m_qualityType			= HK_COLLIDABLE_QUALITY_MOVING;
				info.m_restitution			= 0.0f;
				info.m_angularDamping		= 0.7f;
				info.m_maxAngularVelocity	= 3.0f;
				hkpInertiaTensorComputer::setShapeVolumeMassProperties(info.m_shape, massFactor * 5.0f, info);
				hkpInertiaTensorComputer::clipInertia( 20.0f, info );
			}

			hkReferencedObject::lockAll();		
			hkpRigidBody* newBody = new hkpRigidBody(info);
			hkpPropertyValue shapeKeyProperty;
			shapeKeyProperty.setInt( pointInfo.m_brokenShapeKey );
			newBody->addProperty( HK_BROKEN_OFF_SHAPEKEY_PROP, shapeKeyProperty );
			systemOut.addRigidBody(newBody);
			newBody->removeReference();
			if ( i < 0 )
			{
				// only remove reference to our newly created shape
				info.m_shape->removeReference();
			}
			hkReferencedObject::unlockAll();
		}
	}

	return HK_SUCCESS;
}





hkDemo::Result DestructibleBridgeDemo::stepDemo()
{
	return hkDefaultPhysicsDemo::stepDemo();
}



#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
	"This demo showcases a utility to break off shapes from a MOPP shape. " \
	"On impact the shapes within a certain area around the contact point will be collected and depending" \
	"on their distance either be broken off as separate new rigid bodies or as a merged new MOPP.";

HK_DECLARE_DEMO_VARIANT_USING_STRUCT( DestructibleBridgeDemo, HK_DEMO_TYPE_OTHER, DestructibleBridgeVariant, g_variants, helpString );


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
