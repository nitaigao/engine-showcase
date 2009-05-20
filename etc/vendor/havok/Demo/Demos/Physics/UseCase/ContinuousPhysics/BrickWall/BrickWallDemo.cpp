/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>

#include <Physics/Collide/Query/CastUtil/hkpWorldRayCastInput.h>
#include <Physics/Collide/Query/CastUtil/hkpWorldRayCastOutput.h>

#include <Demos/Physics/UseCase/ContinuousPhysics/BrickWall/BrickWallDemo.h>
#include <Common/Visualize/hkDebugDisplay.h>
#include <Physics/Internal/Collide/BroadPhase/hkpBroadPhase.h>

#include <Common/Base/Thread/Job/ThreadPool/Cpu/hkCpuJobThreadPool.h>
#include <Common/Base/Config/hkConfigThread.h>
//#include <hkcollide/shape/packedconvexvertices/hkpPackedConvexVerticesShape.h>

// To enable instancing in shape viewer
#include <Physics/Utilities/VisualDebugger/Viewer/Collide/hkpShapeDisplayViewer.h>

 
struct BrickWallVariant
{
	const char*	 m_name;
	int    m_width;
	int	   m_height;
	int    m_numWalls;
	bool   m_usePredictive;
	const char* m_details;
};

static const char helpString[] = "A wrecking ball demolishes a brick wall";

static const BrickWallVariant g_variants[] =
{
	{ "4x8x4 Discrete  ", 4,4,4, false, helpString },
	{ "4x8x4 Continuous", 4,4,4, true,  helpString },
	{ "8x16x1 Discrete  ", 8,8,1, false, helpString },
	//{ "20x20x1 Discrete  ", 20,20,1, false, helpString },
	{ "8x16x1 Continuous", 8,8,1, true,  helpString },
	{ "8x16x3 Discrete  ", 8,8,3, false, helpString },
	{ "8x16x3 Continuous", 8,8,3, true,  helpString },
	{ "16x32x3 Continuous", 16,16,3, true,  helpString },
	{ "50x40x1 Continuous", 50,20,1, true,  helpString },
	{ "50x14x3 Continuous", 50, 7,3, true,  helpString },
};

void BrickWallDemo::createBrickWall( hkpWorld* world, int height, int length, const hkVector4& position, hkReal gapWidth, hkpConvexShape* box, hkVector4Parameter halfExtents )
{
	hkVector4 posx = position; 
	// do a raycast to place the ragdoll
	{
		hkpWorldRayCastInput ray;
		ray.m_from = posx;
		ray.m_to = posx;

		ray.m_from(1) += 20.0f;
		ray.m_to(1)   -= 20.0f;
		
		hkpWorldRayCastOutput result;
		world->castRay( ray, result );
		posx.setInterpolate4( ray.m_from, ray.m_to, result.m_hitFraction );
	}
	// move the start point
	posx(0) -= ( gapWidth + 2.0f * halfExtents(0) ) * length * 0.5f;
	posx(1) -= halfExtents(1) + box->getRadius();

	hkArray<hkpEntity*> entitiesToAdd;

	for ( int x = 0; x < length; x ++ )		// along the ground
	{
		hkVector4 pos = posx;
		for( int ii = 0; ii < height; ii++ )
		{
			pos(1) += (halfExtents(1) + box->getRadius())* 2.0f;
			
			hkpRigidBodyCinfo boxInfo;
			boxInfo.m_mass = 10.0f;
			hkpMassProperties massProperties;
			hkpInertiaTensorComputer::computeBoxVolumeMassProperties(halfExtents, boxInfo.m_mass, massProperties);
			
			boxInfo.m_mass = massProperties.m_mass;
			boxInfo.m_centerOfMass = massProperties.m_centerOfMass;
			boxInfo.m_inertiaTensor = massProperties.m_inertiaTensor;
			boxInfo.m_solverDeactivation = boxInfo.SOLVER_DEACTIVATION_MEDIUM;
			boxInfo.m_shape = box;
			//boxInfo.m_qualityType = HK_COLLIDABLE_QUALITY_DEBRIS;
			boxInfo.m_restitution = 0.0f;
			
			boxInfo.m_motionType = hkpMotion::MOTION_BOX_INERTIA;
			
			{
				boxInfo.m_position = pos;
				hkpRigidBody* boxRigidBody = new hkpRigidBody(boxInfo);
				world->addEntity( boxRigidBody );
				boxRigidBody->removeReference();
			}
			
			pos(1) += (halfExtents(1) + box->getRadius())* 2.0f;
			pos(0) += halfExtents(0) * 0.6f;
			{
				boxInfo.m_position = pos;
				hkpRigidBody* boxRigidBody = new hkpRigidBody(boxInfo);
				entitiesToAdd.pushBack(boxRigidBody);
			}
			pos(0) -= halfExtents(0) * 0.6f;
		}
		posx(0) += halfExtents(0)* 2.0f + gapWidth;
	}
	world->addEntityBatch( entitiesToAdd.begin(), entitiesToAdd.getSize());

	for (int i=0; i < entitiesToAdd.getSize(); i++){ entitiesToAdd[i]->removeReference(); }
}

#include <Physics/Collide/Dispatch/hkpCollisionDispatcher.h>
#include <Physics/Internal/Collide/Agent3/PredGskAgent3/hkpPredGskAgent3.h>

BrickWallDemo::BrickWallDemo(hkDemoEnvironment* env)
:	hkDefaultPhysicsDemo(env)
{

	// Disable warnings:									
	hkError::getInstance().setEnabled(0xaf55adde, false); //'No runtime block of size 637136 currently available. Allocating new block from unmanaged memory.'


	hkDefaultPhysicsDemo::enableDisplayingToiInformation( true );
	m_frameToSimulationFrequencyRatio = 1.0f;
	m_timestep = 1.0f / 60.0f;

	const BrickWallVariant& variant =  g_variants[m_variantId];

	int wallHeight = variant.m_height;
	int wallWidth  = variant.m_width;
	int numWalls = variant.m_numWalls;
	hkpWorldCinfo::SimulationType simulationType;
	simulationType = hkpWorldCinfo::SIMULATION_TYPE_CONTINUOUS;
	
	//
	// Setup the camera so that we can see the ball hit the (first) wall.
	//
	{
		hkVector4 from(40.0f, 20.0f, 40.0f);
		hkVector4 to  ( 0.0f, 10.0f, 0.0f);
		hkVector4 up  ( 0.0f,  1.0f, 0.0f);
		setupDefaultCameras( env, from, to, up );
	}


	{

		hkpWorldCinfo info;
		info.setBroadPhaseWorldSize( 5000.0f );
		info.setupSolverInfo(hkpWorldCinfo::SOLVER_TYPE_4ITERS_HARD);
		info.m_collisionTolerance = 0.01f;
		info.m_gravity = hkVector4( 0.0f,-9.8f,0.0f);
		info.m_simulationType = simulationType;
		info.m_broadPhaseBorderBehaviour = info.BROADPHASE_BORDER_FIX_ENTITY;
		//info.m_enableDeactivation = false;
		//info.m_enableSimulationIslands = false;
		info.m_enableDeactivation = false;
		m_world = new hkpWorld( info );
		m_world->lock();
	}
	{
		// Register ALL agents (though some may not be necessary)
		hkpAgentRegisterUtil::registerAllAgents(m_world->getCollisionDispatcher());

		setupGraphics();

		// enable instancing (if present on platform)
		hkpShapeDisplayViewer* shapeViewer = (hkpShapeDisplayViewer*)getLocalViewerByName( hkpShapeDisplayViewer::getName() );
		if (shapeViewer)
		{
			shapeViewer->setInstancingEnabled( true );
		}
	}




	//
	//  Create the ground box
	//
	{
		hkVector4 groundRadii( 70.0f, 2.0f, 70.0f );
		hkpConvexShape* shape = new hkpBoxShape( groundRadii , 0 );
		
		hkpRigidBodyCinfo ci;
		
		ci.m_shape = shape;
		ci.m_motionType = hkpMotion::MOTION_FIXED;
		ci.m_position = hkVector4( 0.0f, -2.0f, 0.0f );
		ci.m_qualityType = HK_COLLIDABLE_QUALITY_FIXED;
		
		m_world->addEntity( new hkpRigidBody( ci ) )->removeReference();
		shape->removeReference();
	}

	hkVector4 groundPos( 0.0f, 0.0f, 0.0f );
	hkVector4 posy = groundPos;
	
	//
	// Create the walls
	//
	if(1)
	{
		hkVector4 boxSize( 1.0f, 0.5f, 0.5f);
		hkpBoxShape* box = new hkpBoxShape( boxSize , 0 );
		box->setRadius( 0.0f );
		hkReal deltaZ = 25.0f;
		posy(2) = -deltaZ * numWalls * 0.5f; 

		for ( int y = 0; y < numWalls; y ++ )			// first wall
		{
			createBrickWall( m_world, wallHeight, wallWidth, posy, 0.2f, box, boxSize );
			posy(2) += deltaZ;
		}
		box->removeReference();
	}


	//
	// Create the ball
	//
	if (1)
	{
		const hkReal radius = 1.5f;
		const hkReal sphereMass = 150.0f;

		hkVector4 relPos( 0.0f,radius + 0.0f, 20.0f );

		hkpRigidBodyCinfo info;
		hkpMassProperties massProperties;
		hkpInertiaTensorComputer::computeSphereVolumeMassProperties(radius, sphereMass, massProperties);

		info.m_mass = massProperties.m_mass;
		info.m_centerOfMass  = massProperties.m_centerOfMass;
		info.m_inertiaTensor = massProperties.m_inertiaTensor;
		info.m_shape = new hkpSphereShape( radius );
		info.m_position.setAdd4(posy, relPos );
		info.m_motionType  = hkpMotion::MOTION_BOX_INERTIA;

		if ( variant.m_usePredictive)
		{
			info.m_qualityType = HK_COLLIDABLE_QUALITY_BULLET;
		}
		else
		{
			info.m_qualityType = HK_COLLIDABLE_QUALITY_DEBRIS;
		}

		hkpRigidBody* sphereRigidBody = new hkpRigidBody( info );

		m_world->addEntity( sphereRigidBody );
		sphereRigidBody->removeReference();
		info.m_shape->removeReference();

		hkVector4 vel(  0.0f,4.9f, -200.0f );
		sphereRigidBody->setLinearVelocity( vel );
	}

	m_world->unlock();
}

hkDemo::Result BrickWallDemo::stepDemo()
{
	return hkDefaultPhysicsDemo::stepDemo();
}

BrickWallDemo::~BrickWallDemo()
{
	// Re-enable warnings:									
	hkError::getInstance().setEnabled(0xaf55adde, true);
}

#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

HK_DECLARE_DEMO_VARIANT_USING_STRUCT( BrickWallDemo, HK_DEMO_TYPE_PRIME, BrickWallVariant, g_variants, helpString ); 

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
