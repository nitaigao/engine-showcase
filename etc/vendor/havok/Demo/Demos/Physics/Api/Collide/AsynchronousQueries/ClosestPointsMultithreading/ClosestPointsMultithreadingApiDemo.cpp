/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


#include <Demos/demos.h>

#include <Physics/Collide/Query/Collector/PointCollector/hkpFixedBufferCdPointCollector.h>
#include <Physics/Collide/Query/Multithreaded/CollisionQuery/hkpCollisionQueryJobs.h>


#include <Common/Visualize/hkDebugDisplay.h>

#include <Demos/DemoCommon/Utilities/GameUtils/GameUtils.h>

#include <Demos/Physics/Api/Collide/AsynchronousQueries/ClosestPointsMultithreading/ClosestPointsMultithreadingApiDemo.h>

#include <Physics/Collide/Query/Multithreaded/CollisionQuery/hkpCollisionQueryJobQueueUtils.h>

#include <Common/Base/Thread/Job/ThreadPool/Cpu/hkCpuJobThreadPool.h>
#include <Common/Base/Thread/Job/ThreadPool/Spu/hkSpuJobThreadPool.h>

#include <Physics/Collide/Shape/Compound/Collection/List/hkpListShape.h>


#if defined(HK_PLATFORM_PS3_PPU)
//	# of real SPUs
#	define NUM_SPUS 6
#else
//	# of simulated SPUs
#	define NUM_SPUS 1
#endif


ClosestPointsMultithreadingApiDemo::ClosestPointsMultithreadingApiDemo(hkDemoEnvironment* env)
	: hkDefaultPhysicsDemo(env),
	m_semaphore(0,1000)
{
	//
	// Setup the camera.
	//
	{
		hkVector4 from( 0.0f, 4.0f, 12.0f);
		hkVector4 to  ( 0.0f,-2.0f,  0.0f);
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

	{
		hkpGroupFilter* filter = new hkpGroupFilter();
		m_world->setCollisionFilter(filter);
		filter->removeReference();
	}

	//
	// Create some bodies (reuse the ShapeRayCastApi demo)
	//
	createBodies();

	m_world->unlock();


	/// Special case for this demo: we do not allow the # of active SPUs to drop to zero as this can cause a deadlock.
	m_allowZeroActiveSpus = false;

	// Register the collision query functions
	hkpCollisionQueryJobQueueUtils::registerWithJobQueue(m_jobQueue);

	// register the default addCdPoint() function; you are free to register your own implementation here though
	hkpFixedBufferCdPointCollector::registerDefaultAddCdPointFunction();

}


ClosestPointsMultithreadingApiDemo::~ClosestPointsMultithreadingApiDemo()
{	
}


void ClosestPointsMultithreadingApiDemo::createBodies()
{
	//
	// Create a simple list shape
	//
	hkpListShape* listShape;
	{
		hkVector4 boxSize1(1,2,1);
		hkpBoxShape* box1 = new hkpBoxShape(boxSize1);

		hkVector4 boxSize2(1,1,3);
		hkpBoxShape* box2 = new hkpBoxShape(boxSize2);

		hkpShape* list[2] = {box1, box2};
		listShape = new hkpListShape(list, 2);

		box1->removeReference();
		box2->removeReference();
	}

	hkVector4 positions[3];
	positions[0].set(0 , 0, 0);
	positions[1].set(-3,-3, 0);
	positions[2].set( 3,-3, 0);

	//
	// Create some bodies
	//

	for (int i=0; i<3; i++)
	{
		m_bodies[i] = GameUtils::createRigidBody(listShape, 1.0f, positions[i]);
		m_world->addEntity(m_bodies[i]);
		m_bodies[i]->removeReference();
	}

	listShape->removeReference();

}


hkDemo::Result ClosestPointsMultithreadingApiDemo::stepDemo()
{
	if ( m_env->m_gamePad->isButtonPressed(HKG_PAD_BUTTON_1) )
	{
		return stepDemoWorldGetClosestPoints();
	}
	else
	{
		return stepDemoPairGetClosestPoints();
	}
}

void ClosestPointsMultithreadingApiDemo::makeFakeInput()
{
	if ( (m_physicsStepCounter % 2) == 0)
	{
		m_env->m_gamePad->forceButtonPressed(HKG_PAD_BUTTON_1);
	}
}

hkDemo::Result ClosestPointsMultithreadingApiDemo::stepDemoPairGetClosestPoints()
{
	//
	// Setup the output array where the resulting closest point/shortest distance (in this case only one) will be returned.
	//
	hkpRootCdPoint* closestPoint = hkAllocateChunk<hkpRootCdPoint>(1, HK_MEMORY_CLASS_DEMO);

	//
	// Setup the pairGetClosestPointraycast command.
	//
	hkpPairGetClosestPointsCommand* command;
	{
		command = hkAllocateChunk<hkpPairGetClosestPointsCommand>(1, HK_MEMORY_CLASS_DEMO);

		// Init input data.
		{
			// hkpWorldObject::getCollidable() needs a read-lock on the object
			m_bodies[0]->markForRead();
			m_bodies[2]->markForRead();

			command->m_collidableA = m_bodies[0]->getCollidable();
			command->m_collidableB = m_bodies[2]->getCollidable();

			m_bodies[2]->unmarkForRead();
			m_bodies[0]->unmarkForRead();
		}

		// Init output struct.
		{
			command->m_results			= closestPoint;
			command->m_resultsCapacity	= 1;
			command->m_numResultsOut	= 0;
		}
	}

	//
	// create the job header
	//
	hkpCollisionQueryJobHeader* jobHeader;
	{
		jobHeader = hkAllocateChunk<hkpCollisionQueryJobHeader>(1, HK_MEMORY_CLASS_DEMO);
	}

	//
	// setup getClosestPointsJob
	//
	hkReal queryTolerance = 10.0f;
	m_world->markForRead();
	hkpPairGetClosestPointsJob pairGetClosestPointsJob(m_world->getCollisionInput(), jobHeader, command, 1, queryTolerance, &m_semaphore, 5);
	m_world->unmarkForRead();

	{
		pairGetClosestPointsJob.setRunsOnSpuOrPpu();
		m_jobQueue->addJob( *reinterpret_cast<hkJobQueue::JobQueueEntry*>(&pairGetClosestPointsJob), hkJobQueue::JOB_HIGH_PRIORITY );
		m_jobThreadPool->processAllJobs( m_jobQueue );
		m_jobThreadPool->waitForCompletion();


		//
		// Wait for the one single job we started to finish.
		//
		m_semaphore.acquire();
	}

	//
	// process return values
	//
	{
		//
		// Draw the shortest distance returned by our own command.
		//
		if ( command->m_numResultsOut > 0 )
		{
			const hkContactPoint& pt = command->m_results[0].m_contact;
			drawContactPoint(pt);
		}
	}

	//
	// Free temporarily allocated memory.
	//
	hkDeallocateChunk(jobHeader,    1, HK_MEMORY_CLASS_DEMO);
	hkDeallocateChunk(command,      1, HK_MEMORY_CLASS_DEMO);
	hkDeallocateChunk(closestPoint, 1, HK_MEMORY_CLASS_DEMO);

	return hkDefaultPhysicsDemo::stepDemo();
}


hkDemo::Result ClosestPointsMultithreadingApiDemo::stepDemoWorldGetClosestPoints()
{
	//
	// Setup the output array where the resulting closest points/shortest distances (in this case two) will be returned.
	//
	hkpRootCdPoint* closestPoint = hkAllocateChunk<hkpRootCdPoint>(2, HK_MEMORY_CLASS_DEMO);

	//
	// Setup the hkWorldGetClosestPoints command.
	//
	hkpWorldGetClosestPointsCommand* command;
	{
		command = hkAllocateChunk<hkpWorldGetClosestPointsCommand>(1, HK_MEMORY_CLASS_DEMO);

		// Init input data.
		{
			// hkpWorldObject::getCollidable() needs a read-lock on the object
			m_bodies[1]->markForRead();
			command->m_collidable = m_bodies[1]->getCollidable();
			m_bodies[1]->unmarkForRead();
		}

		// Init output struct.
		{
			command->m_results			= closestPoint;
			command->m_resultsCapacity	= 2;
			command->m_numResultsOut	= 0;
		}
	}

	// Reasonably large subCommandBuffer (for this situation).
	const int subCommandBufferCapacity = 5;
	hkpPairGetClosestPointsCommand* subCommandBuffer = hkAllocateChunk<hkpPairGetClosestPointsCommand>(subCommandBufferCapacity, HK_MEMORY_CLASS_DEMO);

	//
	// create the job header
	//
	hkpCollisionQueryJobHeader* jobHeader;
	{
		jobHeader = hkAllocateChunk<hkpCollisionQueryJobHeader>(1, HK_MEMORY_CLASS_DEMO);
	}

	//
	// setup hkpWorldGetClosestPointsJob
	// Put the job on the queue, kick-off the PPU/SPU threads and wait for everything to finish.
	//
	{
		m_world->lockReadOnly();

		{
			hkReal queryTolerance = 10.0f;
			hkpWorldGetClosestPointsJob worldGetClosestPointsJob(m_world->getCollisionInput(), jobHeader, command, 1, subCommandBuffer, subCommandBufferCapacity, m_world->m_broadPhase, queryTolerance, &m_semaphore);
			worldGetClosestPointsJob.setRunsOnSpuOrPpu();
			m_jobQueue->addJob( *reinterpret_cast<hkJobQueue::JobQueueEntry*>(&worldGetClosestPointsJob), hkJobQueue::JOB_HIGH_PRIORITY );
		}

		m_jobThreadPool->processAllJobs( m_jobQueue );
		m_jobThreadPool->waitForCompletion();


		//
		// Wait for the one single job we started to finish.
		//
		m_semaphore.acquire();

		m_world->unlockReadOnly();
	}

	//
	// Draw closest distances as returned by the job.
	//
	{
		for (hkUint32 r = 0; r < command->m_numResultsOut; r++)
		{
			const hkContactPoint& pt = command->m_results[r].m_contact;
			drawContactPoint(pt);
		}
	}

	//
	// Free temporarily allocated memory.
	//
	hkDeallocateChunk(jobHeader,    1, HK_MEMORY_CLASS_DEMO);
	hkDeallocateChunk(subCommandBuffer, subCommandBufferCapacity, HK_MEMORY_CLASS_DEMO);
	hkDeallocateChunk(command,      1, HK_MEMORY_CLASS_DEMO);
	hkDeallocateChunk(closestPoint, 2, HK_MEMORY_CLASS_DEMO);



	return hkDefaultPhysicsDemo::stepDemo();
}


void ClosestPointsMultithreadingApiDemo::drawContactPoint(const hkContactPoint& contactPoint)
{
	// getPosition() returns a point on B by convention
	const hkVector4 pointOnBInWorld   = contactPoint.getPosition();

	// normal goes from B to A by convention
	const hkVector4 normalBtoAInWorld = contactPoint.getNormal();

	// pointOnA = pointOnB + dist * normalBToA
	hkVector4 pointOnAInWorld;
	pointOnAInWorld.setAddMul4(pointOnBInWorld, normalBtoAInWorld, contactPoint.getDistance());

	// Draw the line from A to B and the normal on B
	HK_DISPLAY_LINE(  pointOnAInWorld, pointOnBInWorld, hkColor::GREEN );
}


#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
"This demonstrates how to find the closest point(s) between a given pair of objects using multithreading either on PPU or on SPU (if available).\n" \
"Hold down \221 to find the closest points between the box and ALL its surrounding objects (using an hkpWorldGetClosestPointsJob).\n" \
"Release \221 to find the closest points between the box and ONE of the surrounding objects (using an hkpPairGetClosestPointsJob).";


HK_DECLARE_DEMO( ClosestPointsMultithreadingApiDemo, HK_DEMO_TYPE_OTHER, "Multithreading collision queries.", helpString );

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
