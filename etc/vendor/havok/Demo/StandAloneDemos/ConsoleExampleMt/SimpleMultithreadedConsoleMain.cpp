/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

// Math and base include
#include <Common/Base/hkBase.h>
#include <Common/Base/hkBase.h>
#include <Common/Base/System/hkBaseSystem.h>
#include <Common/Base/Memory/hkThreadMemory.h>
#include <Common/Base/Memory/Memory/Pool/hkPoolMemory.h>
#include <Common/Base/System/Error/hkDefaultError.h>
#include <Common/Base/Monitor/hkMonitorStream.h>

// Dynamics includes
#include <Physics/Collide/hkpCollide.h>										
#include <Physics/Collide/Agent/ConvexAgent/SphereBox/hkpSphereBoxAgent.h>	
#include <Physics/Collide/Shape/Convex/Box/hkpBoxShape.h>					
#include <Physics/Collide/Shape/Convex/Sphere/hkpSphereShape.h>				
#include <Physics/Collide/Dispatch/hkpAgentRegisterUtil.h>					

#include <Physics/Collide/Query/CastUtil/hkpWorldRayCastInput.h>			
#include <Physics/Collide/Query/CastUtil/hkpWorldRayCastOutput.h>			

#include <Physics/Dynamics/World/hkpWorld.h>								
#include <Physics/Dynamics/Entity/hkpRigidBody.h>							
#include <Physics/Utilities/Dynamics/Inertia/hkpInertiaTensorComputer.h>	

#include <Common/Base/Thread/Job/ThreadPool/Cpu/hkCpuJobThreadPool.h>
#include <Common/Base/Thread/Job/ThreadPool/Spu/hkSpuJobThreadPool.h>
#include <Common/Base/Thread/JobQueue/hkJobQueue.h>

// Visual Debugger includes
#include <Common/Visualize/hkVisualDebugger.h>
#include <Physics/Utilities/VisualDebugger/hkpPhysicsContext.h>				

// Keycode
#include <Common/Base/keycode.cxx>

#if !defined USING_HAVOK_PHYSICS
#error Physics is needed to build this demo. It is included in the common package for reference only.
#endif



// Classlists
#define INCLUDE_HAVOK_PHYSICS_CLASSES
#define HK_CLASSES_FILE <Common/Serialize/Classlist/hkClasses.h>
#include <Common/Serialize/Util/hkBuiltinTypeRegistry.cxx>

// Generate a custom list to trim memory requirements
#define HK_COMPAT_FILE <Common/Compat/hkCompatVersions.h>
#include <Common/Compat/hkCompat_None.cxx>

#include <stdio.h>
static void HK_CALL errorReport(const char* msg, void*)
{
	printf("%s", msg);
}

#if defined(HK_PLATFORM_PS3_PPU)
#include <Common/Base/Spu/Util/hkSpuUtil.h>
#include <cell/spurs.h>
#endif


//
// Forward declarations
//

void setupPhysics(hkpWorld* physicsWorld);
hkpRigidBody* g_ball;


int HK_CALL main(int argc, const char** argv)
{
	//
	// Do platform specific initialization
	//
#if !defined(HK_PLATFORM_WIN32)
	extern void initPlatform();
	initPlatform();
#endif



	//
	// Initialize the base system including our memory system
	//

	hkPoolMemory* memoryManager = new hkPoolMemory();
	hkThreadMemory* threadMemory = new hkThreadMemory(memoryManager);
	hkBaseSystem::init( memoryManager, threadMemory, errorReport );
	memoryManager->removeReference();

	// We now initialize the stack area to 100k (fast temporary memory to be used by the engine).
	char* stackBuffer;
	{
		int stackSize = 0x100000;
		stackBuffer = hkAllocate<char>( stackSize, HK_MEMORY_CLASS_BASE);
		hkThreadMemory::getInstance().setStackArea( stackBuffer, stackSize);
	}

	{

		//
		// Initialize the multi-threading classes, hkJobQueue, and hkJobThreadPool
		//

		// They can be used for all Havok multithreading tasks. In this exmaple we only show how to use
		// them for physics, but you can reference other multithreading demos in the demo framework
		// to see how to multithread other products. The model of usage is the same as for physics.
		// The hkThreadpool has a specified number of threads that can run Havok jobs.  These can work
		// alongside the main thread to perform any Havok multi-threadable computations.
		// The model for running Havok tasks in Spus and in auxilary threads is identical.  It is encapsulated in the
		// class hkJobThreadPool.  On PLAYSTATION(R)3 we initialize the SPU version of this class, which is simply a SPURS taskset.
		// On other multi-threaded platforms we initialize the CPU version of this class, hkCpuJobThreadPool, which creates a pool of threads
		// that run in exactly the same way.  On the PLAYSTATION(R)3 we could also create a hkCpuJobThreadPool.  However, it is only
		// necessary (and advisable) to use one Havok PPU thread for maximum efficiency. In this case we simply use this main thread
		// for this purpose, and so do not create a hkCpuJobThreadPool.
		hkJobThreadPool* threadPool;

		// We can cap the number of threads used - here we use the maximum for whatever multithreaded platform we are running on. This variable is
		// set in the following code sections.
		int totalNumThreadsUsed;

	#if defined HK_PLATFORM_PS3_PPU

		hkSpuJobThreadPoolCinfo threadPoolCinfo;

		extern CellSpurs* initSpurs();
		HK_CELL_SPURS* spurs = initSpurs();

		hkSpuUtil* spuUtil = new hkSpuUtil( spurs );

		spuUtil->attachHelperThreadToSpurs();
		threadPoolCinfo.m_spuUtil = spuUtil;
		threadPoolCinfo.m_numSpus = 6; // Use all 6 SPUs for this example

		totalNumThreadsUsed = 1; // only use one CPU thread for PS3.

		// This line enables timers collection, by allocating 200 Kb per thread.  If you leave this at its default (0),
		// timer collection will not be enabled.
		threadPoolCinfo.m_perSpuMontiorBufferSize = 200000;
		threadPool = new hkSpuJobThreadPool( threadPoolCinfo );
		spuUtil->removeReference();

	#else

		// Get the number of physical threads available on the system
		hkHardwareInfo hwInfo;
		hkGetHardwareInfo(hwInfo);
		totalNumThreadsUsed = hwInfo.m_numThreads;

		// We use one less than this for our thread pool, because we must also use this thread for our simulation
		hkCpuJobThreadPoolCinfo threadPoolCinfo;
		threadPoolCinfo.m_numThreads = totalNumThreadsUsed - 1;

		// This line enables timers collection, by allocating 200 Kb per thread.  If you leave this at its default (0),
		// timer collection will not be enabled.
		threadPoolCinfo.m_timerBufferPerThreadAllocation = 200000;
		threadPool = new hkCpuJobThreadPool( threadPoolCinfo );

	#endif

		// We also need to create a Job queue. This job queue will be used by all Havok modules to run multithreaded work.
		// Here we only use it for physics.
		hkJobQueueCinfo info;
		info.m_jobQueueHwSetup.m_numCpuThreads = totalNumThreadsUsed;
		hkJobQueue* jobQueue = new hkJobQueue(info);

		//
		// Enable monitors for this thread.
		//

		// Monitors have been enabled for thread pool threads already (see above comment).
		hkMonitorStream::getInstance().resize(200000);



		//
		// <PHYSICS-ONLY>: Create the physics world.
		// At this point you would initialize any other Havok modules you are using.
		//
		hkpWorld* physicsWorld;
		{
			// The world cinfo contains global simulation parameters, including gravity, solver settings etc.
			hkpWorldCinfo worldInfo;

			// Set the simulation type of the world to multi-threaded.
			worldInfo.m_simulationType = hkpWorldCinfo::SIMULATION_TYPE_MULTITHREADED;

			// Flag objects that fall "out of the world" to be automatically removed - just necessary for this physics scene
			worldInfo.m_broadPhaseBorderBehaviour = hkpWorldCinfo::BROADPHASE_BORDER_REMOVE_ENTITY;

			physicsWorld = new hkpWorld(worldInfo);

			// Disable deactivation, so that you can view timers in the VDB. This should not be done in your game.
			physicsWorld->m_wantDeactivation = false;


			// When the simulation type is SIMULATION_TYPE_MULTITHREADED, in the debug build, the sdk performs checks
			// to make sure only one thread is modifying the world at once to prevent multithreaded bugs. Each thread
			// must call markForRead / markForWrite before it modifies the world to enable these checks.
			physicsWorld->markForWrite();


			// Register all collision agents, even though only box - box will be used in this particular example.
			// It's important to register collision agents before adding any entities to the world.
			hkpAgentRegisterUtil::registerAllAgents( physicsWorld->getCollisionDispatcher() );

			// We need to register all modules we will be running multi-threaded with the job queue
			physicsWorld->registerWithJobQueue( jobQueue );

			// Create all the physics rigid bodies
			setupPhysics( physicsWorld );
		}


		//
		// Initialize the VDB
		//
		hkArray<hkProcessContext*> contexts;


		// <PHYSICS-ONLY>: Register physics specific visual debugger processes
		// By default the VDB will show debug points and lines, however some products such as physics and cloth have additional viewers
		// that can show geometries etc and can be enabled and disabled by the VDB app.
		hkpPhysicsContext* context;
		{
			// The visual debugger so we can connect remotely to the simulation
			// The context must exist beyond the use of the VDB instance, and you can make
			// whatever contexts you like for your own viewer types.
			context = new hkpPhysicsContext();
			hkpPhysicsContext::registerAllPhysicsProcesses(); // all the physics viewers
			context->addWorld(physicsWorld); // add the physics world so the viewers can see it
			contexts.pushBack(context);

			// Now we have finished modifying the world, release our write marker.
			physicsWorld->unmarkForWrite();
		}

		hkVisualDebugger* vdb = new hkVisualDebugger(contexts);
		vdb->serve();


		//
		// Simulate the world for 1 minute.
		//


		// Take fixed time steps of 1/60th of a second.
		// This works well if your game runs solidly at 60Hz. If your game runs at 30Hz
		// you can take either 2 60Hz steps or 1 30Hz step. Note that at lower frequencies (i.e. 30 Hz)
		// more bullet through paper issues appear, and constraints will not be as stiff.
		// If you run at variable frame rate, or are likely to drop frames, you can consider
		// running your physics for a variable number of steps based on the system clock (i.e. last frame time).
		// Please refer to the user guide section on time stepping for a full treatment of this issue.

		// A stopwatch for waiting until the real time has passed
		hkStopwatch stopWatch;
		stopWatch.start();
		hkReal lastTime = stopWatch.getElapsedSeconds();

		hkReal timestep = 1.f / 60.f;
		int numSteps = int(60.f / timestep);

		for ( int i = 0; i < numSteps; ++i )
		{
			// <PHYSICS-ONLY>:
			// Step the physics world. This single call steps using this thread and all threads
			// in the threadPool. For other products you add jobs, call process all jobs and wait for completion.
			// See the multithreading chapter in the user guide for details
			{
				physicsWorld->stepMultithreaded( jobQueue, threadPool, timestep );
			}

			// Step the visual debugger. We first synchronize the timer data
			context->syncTimers( threadPool );
			vdb->step();

			// Clear accumulated timer data in this thread and all slave threads
			hkMonitorStream::getInstance().reset();
			threadPool->clearTimerData();


			// <PHYSICS-ONLY>:  Display the sphereRigidBody position to the console every second
			if (i % 60 == 0)
			{
				hkVector4 pos = g_ball->getPosition();
				printf("[%f,%f,%f]\n", pos(0), pos(1), pos(2));
			}

			// Pause until the actual time has passed
			while (stopWatch.getElapsedSeconds() < lastTime + timestep);
			lastTime += timestep;

			// Step the graphics display (none in this demo).
		}



		//
		// Clean up physics and graphics
		//

		// <PHYSICS-ONLY>: cleanup physics
		{
			physicsWorld->markForWrite();
			physicsWorld->removeReference();
		}
		vdb->removeReference();

		// Contexts are not reference counted at the base class level by the VDB as
		// they are just interfaces really. So only delete the context after you have
		// finished using the VDB.
		context->removeReference();

		delete jobQueue;

		//
		// Clean up the thread pool
		//

		threadPool->removeReference();


	#if defined HK_PLATFORM_PS3_PPU
		extern void quitSpurs( CellSpurs* spurs );
		quitSpurs( spurs );
	#endif
	}

	// Deallocate stack area
	threadMemory->setStackArea(0, 0);
	hkDeallocate(stackBuffer);

	threadMemory->removeReference();

	// Quit base system
	hkBaseSystem::quit();

	return 0;
}



void createBrickWall( hkpWorld* world, int height, int length, const hkVector4& position, hkReal gapWidth, hkpConvexShape* box, hkVector4Parameter halfExtents )
{
	hkVector4 posx = position;
	// do a raycast to place the wall
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

void setupPhysics(hkpWorld* physicsWorld)
{
	//
	//  Create the ground box
	//
	{
		hkVector4 groundRadii( 70.0f, 2.0f, 140.0f );
		hkpConvexShape* shape = new hkpBoxShape( groundRadii , 0 );

		hkpRigidBodyCinfo ci;

		ci.m_shape = shape;
		ci.m_motionType = hkpMotion::MOTION_FIXED;
		ci.m_position = hkVector4( 0.0f, -2.0f, 0.0f );
		ci.m_qualityType = HK_COLLIDABLE_QUALITY_FIXED;

		physicsWorld->addEntity( new hkpRigidBody( ci ) )->removeReference();
		shape->removeReference();
	}

	hkVector4 groundPos( 0.0f, 0.0f, 0.0f );
	hkVector4 posy = groundPos;

	//
	// Create the walls
	//

	int wallHeight = 8;
	int wallWidth  = 8;
	int numWalls = 6;
	hkVector4 boxSize( 1.0f, 0.5f, 0.5f);
	hkpBoxShape* box = new hkpBoxShape( boxSize , 0 );
	box->setRadius( 0.0f );

	hkReal deltaZ = 25.0f;
	posy(2) = -deltaZ * numWalls * 0.5f;

	for ( int y = 0; y < numWalls; y ++ )			// first wall
	{
		createBrickWall( physicsWorld, wallHeight, wallWidth, posy, 0.2f, box, boxSize );
		posy(2) += deltaZ;
	}
	box->removeReference();

	//
	// Create a ball moving towards the walls
	//

	const hkReal radius = 1.5f;
	const hkReal sphereMass = 150.0f;

	hkVector4 relPos( 0.0f,radius + 0.0f, 50.0f );

	hkpRigidBodyCinfo info;
	hkpMassProperties massProperties;
	hkpInertiaTensorComputer::computeSphereVolumeMassProperties(radius, sphereMass, massProperties);

	info.m_mass = massProperties.m_mass;
	info.m_centerOfMass  = massProperties.m_centerOfMass;
	info.m_inertiaTensor = massProperties.m_inertiaTensor;
	info.m_shape = new hkpSphereShape( radius );
	info.m_position.setAdd4(posy, relPos );
	info.m_motionType  = hkpMotion::MOTION_BOX_INERTIA;

	info.m_qualityType = HK_COLLIDABLE_QUALITY_BULLET;


	hkpRigidBody* sphereRigidBody = new hkpRigidBody( info );
	g_ball = sphereRigidBody;

	physicsWorld->addEntity( sphereRigidBody );
	sphereRigidBody->removeReference();
	info.m_shape->removeReference();

	hkVector4 vel(  0.0f,4.9f, -100.0f );
	sphereRigidBody->setLinearVelocity( vel );


}

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
