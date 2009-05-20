/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


#include <Demos/demos.h>

#include <Common/Base/Thread/Job/ThreadPool/Cpu/hkCpuJobThreadPool.h>
#include <Common/Base/Algorithm/PseudoRandom/hkPseudoRandomGenerator.h>
#include <Common/Visualize/hkDebugDisplay.h>

#include <Physics/Collide/Query/CastUtil/hkpWorldRayCastOutput.h>
#include <Physics/Collide/Query/Multithreaded/RayCastQuery/hkpRayCastQueryJobs.h>
#include <Physics/Collide/Query/Multithreaded/RayCastQuery/hkpRayCastQueryJobQueueUtils.h>

#include <Demos/Physics/Api/Collide/RayCasting/WorldRayCastMultithreading/WorldRaycastMultithreadingApiDemo.h>


#if defined(HK_PLATFORM_PS3_PPU)
//	# of real SPUs
#	define NUM_SPUS 6
#else
//	# of simulated SPUs
#	define NUM_SPUS 1
#endif


struct WorldRayCastMultithreadingApiDemoVariant
{
	enum DemoType
	{
		MULTITHREADED_ON_PPU,
		MULTITHREADED_ON_SPU,
		MULTITHREADED_ON_PPU_AND_SPU,
	};

	const char*	m_name;
	DemoType	m_demoType;
	const char*	m_details;
};


static const char helpString[] = \
	"This demo demonstrates how to emulate the hkpWorld::castRay() functionality in a multithreaded "	\
	"environment through the use of a dedicated hkpWorldRayCastJob. By putting such a job on "			\
	"the job queue it can be processed by other threads or SPUs (on PS3). "								\
	"One command of this job will cast exactly one ray into the world, using the broadphase to avoid "	\
	"expensive low-level intersection tests. You can place an arbitrary number of commands into one "	\
	"job, as the job will automatically split itself into parallel sub-jobs. "							\
	"For more information on the original hkpWorld::castRay() functionality, see WorldRayCastDemo. ";


static const WorldRayCastMultithreadingApiDemoVariant g_WorldRayCastMultithreadingApiDemoVariants[] =
{
	{ "Multithreaded",				WorldRayCastMultithreadingApiDemoVariant::MULTITHREADED_ON_PPU,			helpString },
};


WorldRayCastMultithreadingApiDemo::WorldRayCastMultithreadingApiDemo(hkDemoEnvironment* env)
	: hkDefaultPhysicsDemo(env),
	m_semaphore(0,1000)
{
	const WorldRayCastMultithreadingApiDemoVariant& variant = g_WorldRayCastMultithreadingApiDemoVariants[m_variantId];

	//
	// Setup the camera.
	//
	{
		hkVector4 from(30.0f, 8.0f, 25.0f);
		hkVector4 to  ( 4.0f, 0.0f, -3.0f);
		hkVector4 up  ( 0.0f, 1.0f,  0.0f);
		setupDefaultCameras(env, from, to, up);
	}

	//
	// Create the world.
	//
	{
		hkpWorldCinfo info;
		info.setupSolverInfo(hkpWorldCinfo::SOLVER_TYPE_4ITERS_MEDIUM); 
		
		// Set gravity to zero so body floats.
		info.m_gravity.set(0.0f, 0.0f, 0.0f);	
		info.setBroadPhaseWorldSize( 100.0f );
		m_world = new hkpWorld(info);
		m_world->lock();

		// Disable backface culling, since we have mopp's etc.
		setGraphicsState(HKG_ENABLED_CULLFACE, false);

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
	// Set the simulation time to 0
	//
	m_time = 0.0f;

	//
	// Create some bodies (reuse the ShapeRayCastApi demo)
	//
	createBodies();

	m_world->unlock();

	//
	// Setup multithreading.
	//
	hkpRayCastQueryJobQueueUtils::registerWithJobQueue(m_jobQueue);

	// Special case for this demo variant: we do not allow the # of active SPUs to drop to zero as this can cause a deadlock.
	if ( variant.m_demoType == WorldRayCastMultithreadingApiDemoVariant::MULTITHREADED_ON_SPU ) m_allowZeroActiveSpus = false;

}



void WorldRayCastMultithreadingApiDemo::createBodies()
{
	hkpRigidBodyCinfo rigidBodyInfo;
	rigidBodyInfo.m_collisionFilterInfo	= hkpGroupFilterSetup::LAYER_DEBRIS;
	rigidBodyInfo.m_motionType			= hkpMotion::MOTION_SPHERE_INERTIA;

	hkPseudoRandomGenerator rand(100);

	int numObjects = 100;

	//
	// A collection of rigid bodies is randomly created by generating an integer in the range(0,1) and choosing
	// either a box or a sphere shape.
	//
	{
		for( int i = 0; i < numObjects; i++)
		{
			int shapeType = (int)(rand.getRandRange(0,1) * 2);
			switch(shapeType)
			{
			case 0:
				{
					hkVector4 boxHalfExtents = hkVector4( rand.getRandRange(0.5f, 3.0f), rand.getRandRange(0.5f, 3.0f), rand.getRandRange(0.5f, 3.0f) );
					rigidBodyInfo.m_shape = new hkpBoxShape(boxHalfExtents);
					break;
				}

			case 1:
				{
					hkReal sphereRadius = rand.getRandRange(0.5f, 2.0f);
					rigidBodyInfo.m_shape = new hkpSphereShape(sphereRadius);
					break;
				}

			}

			// Fake Inertia tensor for simplicity, assume it's a unit cube.
			{
				hkVector4 halfExtents(0.5f, 0.5f, 0.5f);
				hkReal mass = 10.0f;
				hkpMassProperties massProperties;
				hkpInertiaTensorComputer::computeBoxVolumeMassProperties(halfExtents, mass, massProperties);

				rigidBodyInfo.m_inertiaTensor = massProperties.m_inertiaTensor;
				rigidBodyInfo.m_centerOfMass  = massProperties.m_centerOfMass;
				rigidBodyInfo.m_mass		  = massProperties.m_mass;			
			}	

			// The object is then assigned a random position, orientation and angular velocity and added to the world:
			rigidBodyInfo.m_position.set( rand.getRandRange(-10.0f, 10.0f), rand.getRandRange(-10.0f, 10.0f), rand.getRandRange(0.0f, -40.0f) );
			rand.getRandomRotation( rigidBodyInfo.m_rotation );

			hkpRigidBody* rigidBody = new hkpRigidBody(rigidBodyInfo);
			rigidBodyInfo.m_shape->removeReference();

			// Give the object an initial velocity.
			hkVector4 angularVel(rand.getRandRange(-1.0f, 1.0f), rand.getRandRange(-1.0f, 1.0f), rand.getRandRange(-1.0f, 1.0f));
			rigidBody->setAngularVelocity(angularVel);
			rigidBody->setAngularDamping(0.0f);

			m_world->addEntity(rigidBody);
			rigidBody->removeReference();
		}
	}
}


hkDemo::Result WorldRayCastMultithreadingApiDemo::stepDemo()
{
	//const WorldRayCastMultithreadingApiDemoVariant& variant = g_WorldRayCastMultithreadingApiDemoVariants[m_variantId];

	m_time += m_timestep;

	// The start point of the ray remains fixed in world space with the destination point of the
	// ray being varied in both the X and Y directions. This is achieved with simple
	// sine and cosine functions calls using the current time as the varying parameter:

	hkReal xDir = 12.0f * hkMath::sin(m_time * 0.3f);
	hkReal yDir = 12.0f * hkMath::cos(m_time * 0.3f);

	//
	// Setup the output array where the ray's results (in this case only one) will be returned.
	//
	hkpWorldRayCastOutput* rayOutput = hkAllocateChunk<hkpWorldRayCastOutput>(1, HK_MEMORY_CLASS_DEMO);

	//
	// Setup the raycast command.
	//
	hkpWorldRayCastCommand* command;
	{
		command = hkAllocateChunk<hkpWorldRayCastCommand>(1, HK_MEMORY_CLASS_DEMO);

		// Init ray data.
		{
			command->m_rayInput.m_from							. set(0.0f, 0.0f, 15.0f);
			command->m_rayInput.m_to							. set(xDir, yDir, -15.0f);
			command->m_rayInput.m_enableShapeCollectionFilter	= false;
			command->m_rayInput.m_filterInfo					= 0;
		}

		// Init output struct.
		{
			command->m_results									= rayOutput;
			command->m_resultsCapacity							= 1;
			command->m_numResultsOut							= 0;
		}
	}

	//
	// create the job header
	//
	hkpCollisionQueryJobHeader* jobHeader = hkAllocateChunk<hkpCollisionQueryJobHeader>(1, HK_MEMORY_CLASS_DEMO);

	//
	// setup hkpWorldRayCastJob
	//
	m_world->markForRead();
	hkpWorldRayCastJob worldRayCastJob(m_world->getCollisionInput(), jobHeader, command, 1, m_world->m_broadPhase, &m_semaphore);
	m_world->unmarkForRead();

	//
	// Put the job on the queue, kick-off the PPU/SPU threads and wait for everything to finish.
	//
	{
		m_world->lockReadOnly();


		//
		// Put the raycast job on the job queue.
		//
		worldRayCastJob.setRunsOnSpuOrPpu();
		m_jobQueue->addJob( worldRayCastJob, hkJobQueue::JOB_LOW_PRIORITY );

		m_jobThreadPool->processAllJobs( m_jobQueue );

		m_jobThreadPool->waitForCompletion();


		//
		// Wait for the one single job we started to finish.
		//
		m_semaphore.acquire();

		m_world->unlockReadOnly();
	}

	//
	// Display results.
	//
	if( command->m_numResultsOut > 0 )
	{
		hkVector4 intersectionPointWorld;
		intersectionPointWorld.setInterpolate4( command->m_rayInput.m_from, command->m_rayInput.m_to, command->m_results->m_hitFraction );
		HK_DISPLAY_LINE( command->m_rayInput.m_from, intersectionPointWorld, hkColor::RED);
		HK_DISPLAY_ARROW( intersectionPointWorld, command->m_results->m_normal, hkColor::CYAN);
	}
	else
	{
		// Otherwise draw as GREY
		HK_DISPLAY_LINE(command->m_rayInput.m_from, command->m_rayInput.m_to, hkColor::rgbFromChars(200, 200, 200));
	}

	//
	// Free temporarily allocated memory.
	//
	hkDeallocateChunk(jobHeader, 1, HK_MEMORY_CLASS_DEMO);
	hkDeallocateChunk(command,   1, HK_MEMORY_CLASS_DEMO);
	hkDeallocateChunk(rayOutput, 1, HK_MEMORY_CLASS_DEMO);

	return hkDefaultPhysicsDemo::stepDemo();
}


#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif


HK_DECLARE_DEMO_VARIANT_USING_STRUCT( WorldRayCastMultithreadingApiDemo, HK_DEMO_TYPE_OTHER, WorldRayCastMultithreadingApiDemoVariant, g_WorldRayCastMultithreadingApiDemoVariants, HK_NULL );

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
