/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


#include <Demos/demos.h>
#include <Physics/Collide/Query/Collector/RayCollector/hkpClosestRayHitCollector.h>
#include <Demos/Physics/Test/Stress/Feature/Collide/AabbCast/AabbCastStressDemo.h>

#include <Common/Base/Monitor/hkMonitorStream.h>

#include <Physics/Internal/Collide/BroadPhase/hkpBroadPhase.h>
#include <Physics/Collide/Query/CastUtil/hkpWorldRayCastInput.h>
#include <Physics/Collide/Query/CastUtil/hkpWorldRayCastOutput.h>
#include <Physics/Collide/Query/CastUtil/hkpWorldRayCaster.h>

#include <Physics/Internal/Collide/BroadPhase/hkpBroadPhase.h>
#include <Physics/Collide/Dispatch/BroadPhase/hkpTypedBroadPhaseHandle.h>

// We need to create a phantom
#include <Physics/Dynamics/Phantom/hkpAabbPhantom.h>

// We need to display the results of the raycast to confirm correctness.
#include <Common/Visualize/hkDebugDisplay.h>
//#include <hkdemoframework/hkDemoFramework.h>

#include <Common/Base/Algorithm/PseudoRandom/hkPseudoRandomGenerator.h>

AabbCastStressDemo::AabbCastStressDemo(hkDemoEnvironment* env)
:	hkDefaultPhysicsDemo(env)
{
	{
		m_numRigidBodies = int(m_env->m_cpuMhz);
		m_worldSizeX = 2.0f * hkMath::sqrt(hkReal(m_env->m_cpuMhz));
		m_worldSizeY = 3;
		m_worldSizeZ = m_worldSizeX;
	}
	//
	// Setup the camera.
	//
	{
		hkVector4 from(30.0f, 8.0f, 25.0f);
		hkVector4 to  ( 4.0f, 0.0f, -3.0f);
		hkVector4 up  ( 0.0f, 1.0f,  0.0f);
		setupDefaultCameras(env, from, to, up);

		// Demo is slow graphicaly as it without shadows
		forceShadowState(false);

	}

	//
	// Create the world.
	//
	{
		hkpWorldCinfo info;
		
		// Set gravity to zero so body floats.
		info.m_gravity.setZero4();

		// make the world big enough to hold all our objects
		// make y larger so that our raycasts stay within the world aabb
		info.m_broadPhaseWorldAabb.m_max.set( m_worldSizeX, 10*m_worldSizeY, m_worldSizeZ );
		info.m_broadPhaseWorldAabb.m_min.setNeg4( info.m_broadPhaseWorldAabb.m_max );

		// Subdivide the broadphase space into equal sections along the x-axis
		// NOTE: Disabling this until the marker crash issue is fixed.
		//info.m_broadPhaseNumMarkers = m_numBroadphaseMarkers;

		
		m_world = new hkpWorld(info);
		m_world->lock();

		setupGraphics();
	}

	// register all agents(however, we put all objects into the some group,
	// so no collision will be detected
	hkpAgentRegisterUtil::registerAllAgents( m_world->getCollisionDispatcher() );

	// Add a collision filter to the world to allow the bodies interpenetrate
	{
		hkpGroupFilter* filter = new hkpGroupFilter();
		filter->disableCollisionsBetween( hkpGroupFilterSetup::LAYER_DEBRIS, hkpGroupFilterSetup::LAYER_DEBRIS );
		m_world->setCollisionFilter( filter );
		filter->removeReference();
	}


	//
	// Create some bodies (reuse the ShapeRaycastApi demo)
	//
	createBodies();

	m_world->unlock();
}

AabbCastStressDemo::~AabbCastStressDemo()
{

}


class MySimpleCollector: public hkpBroadPhaseCastCollector
{
public:

	MySimpleCollector() { m_hits = 0; }

	virtual	hkReal addBroadPhaseHandle( const hkpBroadPhaseHandle* broadphaseHandle, int castIndex )
	{
		HK_SET_OBJECT_COLOR( (hkUlong)static_cast<const hkpTypedBroadPhaseHandle*>(broadphaseHandle)->getOwner(), 0xffff0000 );
		m_hits++;
		return 1.0f;
	}

	int m_hits;
};

hkDemo::Result AabbCastStressDemo::stepDemo()
{
	m_world->lock();


	hkVector4 worldSize(m_worldSizeX, m_worldSizeY, m_worldSizeZ);
	worldSize.mul4(0.9f);

	int numCasts = 50;
	hkStopwatch stopwatch;
	stopwatch.start();

	MySimpleCollector collector;

	hkPseudoRandomGenerator rand(123);
	{
		hkpBroadPhase::hkpCastAabbInput input;
		for (int t=0; t < numCasts; t++)
		{
			rand.getRandomVector11( input.m_from );
			rand.getRandomVector11( input.m_to );

			hkVector4 dir;
			dir.setSub4(input.m_to, input.m_from);
			input.m_from.mul4( worldSize );
			input.m_to.setAddMul4( input.m_from, dir, 3.0f );

			input.m_halfExtents.setAll3( 1.0f );

			HK_DISPLAY_LINE( input.m_to, input.m_from, 0xffffffff);

			m_world->getBroadPhase()->castAabb(input, collector);	
		}			
	}
	stopwatch.stop();

	//
	// Output statistics
	//
	{
		hkReal totalTimeUsecs = stopwatch.getSplitSeconds() * 1e6f;
		hkReal timePerCastuSecs = totalTimeUsecs / numCasts;

		char buf[512];
		hkString::snprintf(buf, 512, "Average time per cast:%f microsecs total time:%f (%d hits)", timePerCastuSecs, totalTimeUsecs, collector.m_hits);
		m_env->m_textDisplay->outputText(buf, 20, 350);
	}

	m_world->unlock();

	return hkDefaultPhysicsDemo::stepDemo();
}



void AabbCastStressDemo::createBodies()
{
	hkpRigidBodyCinfo rigidBodyInfo;
	rigidBodyInfo.m_collisionFilterInfo = hkpGroupFilter::calcFilterInfo(1, 1);
	hkPseudoRandomGenerator rand(100);

	hkArray<hkpEntity*> bodyArray;
	bodyArray.reserve(m_numRigidBodies);

	hkpShape* shape;
	{
		hkVector4 halfExtents(0.5f, 0.5f, 0.5f);
		shape = new hkpBoxShape( halfExtents, 0.0f );
	}

	for( int i = 0; i < m_numRigidBodies; i++)
	{
		// All bodies created below are movable
		rigidBodyInfo.m_motionType = hkpMotion::MOTION_SPHERE_INERTIA;

		// A collection of many rigid bodies is randomly created using a hkpBoxShape
		rigidBodyInfo.m_shape = shape;

		// As usual we fill out the hkpRigidBodyCinfo 'blueprint' for the rigidbody, with the code above specifying
		// the necessary information for the 'm_shape' member. To create a hkpConvexVerticesShape we need a set of vertices and
		// we must generate a set of plane equations from these points. As you can see from the code this is all performed 
		// prior to instantiating the shape.

		// Fake Inertia tensor for simplicity, assume it's a unit cube
		{
			hkReal mass = 10.0f;
			hkReal d = mass * 0.5f;
			rigidBodyInfo.m_inertiaTensor.setDiagonal( d,d,d );
			rigidBodyInfo.m_mass = mass;			
		}	

		// The object is then assigned a random position, orientation and angular velocity and added to the world:

		rigidBodyInfo.m_position.set(	rand.getRandRange(-1.f*m_worldSizeX, 1.f*m_worldSizeX),
										rand.getRandRange(-1.f*m_worldSizeY, 1.f*m_worldSizeY),
										rand.getRandRange(-1.f*m_worldSizeZ, 1.f*m_worldSizeZ));
		rand.getRandomRotation( rigidBodyInfo.m_rotation );

		rigidBodyInfo.m_collisionFilterInfo = hkpGroupFilterSetup::LAYER_DEBRIS;

		hkpRigidBody* rigidBody = new hkpRigidBody(rigidBodyInfo);

		// Give them an intial velocity
		hkVector4 angularVel(rand.getRandRange(-1.0f, 1.0f), rand.getRandRange(-1.0f, 1.0f), rand.getRandRange(-1.0f, 1.0f));

		bodyArray.pushBack( rigidBody );

		// There is no gravity vector for this world and so the bodies will appear to float in space.
	}
	shape->removeReference();

	// Batch add all bodies to the system and defragment the broadphase
	m_world->addEntityBatch( bodyArray.begin(), bodyArray.getSize() );
	m_world->getBroadPhase()->defragment();
		
	//
	//	Remove all references to bodies. They are now referenced by m_world
	//
	{
		for ( int i = 0; i < bodyArray.getSize(); i++ )
		{
			bodyArray[i]->removeReference();
		}
	}
}


#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

HK_DECLARE_DEMO(AabbCastStressDemo, HK_DEMO_TYPE_PRIME, "AAbb cast stress test", "");

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
