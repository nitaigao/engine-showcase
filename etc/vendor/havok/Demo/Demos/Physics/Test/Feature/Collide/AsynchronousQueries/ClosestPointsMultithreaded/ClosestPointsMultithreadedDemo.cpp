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


#include <Demos/Physics/Test/Feature/Collide/AsynchronousQueries/ClosestPointsMultithreaded/ClosestPointsMultithreadedDemo.h>

#include <Physics/Collide/Query/Multithreaded/CollisionQuery/hkpCollisionQueryJobQueueUtils.h>
#include <Common/Base/Thread/Job/ThreadPool/Cpu/hkCpuJobThreadPool.h>


#if defined(HK_PLATFORM_PS3_PPU)
//	# of real SPUs
#	define NUM_SPUS 6
#else
//	# of simulated SPUs
#	define NUM_SPUS 1
#endif


struct ClosestPointsMultithreadedDemoVariant
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
	"This demonstrates how to find the closest point(s) between a given pair of objects using multithreading either on PPU or on SPU (if available).\n" \
	"Hold down \221 to find the closest points to 2 selected objects (using hkWorldGetClosestPointsJobs), release \221 to find the closest points between all objects." \
	"Hold down \222 to display 'closest distances between all shapes of all bodies', release \222 for 'closest distance between rigid bodies only'.";


static const ClosestPointsMultithreadedDemoVariant g_ClosestPointsMultithreadedDemoVariants[] =
{
	{ "Multithreaded",				ClosestPointsMultithreadedDemoVariant::MULTITHREADED_ON_PPU,			helpString },
};


ClosestPointsMultithreadedDemo::ClosestPointsMultithreadedDemo(hkDemoEnvironment* env)
	: ClosestPointsDemo(env, false),
	m_semaphore(0,1000)
{
	const ClosestPointsMultithreadedDemoVariant& variant =  g_ClosestPointsMultithreadedDemoVariants[m_variantId];

	//
	// setup world (using the ClosestPointsDemo setup function)
	//
	{
		int variantIdBackup = m_variantId;
		m_variantId = 0; // temporarily override variant id to force 'getClosestPoints' variant

		switch ( variant.m_demoType )
		{
			case ClosestPointsMultithreadedDemoVariant::MULTITHREADED_ON_PPU:
				{
					// create world with "all" shapes
					setupWorld(env, false);
					break;
				}

		}

		m_variantId = variantIdBackup;
	}


	hkpCollisionQueryJobQueueUtils::registerWithJobQueue(m_jobQueue);

	// Special case for this demo variant: we do not allow the # of active SPUs to drop to zero as this can cause a deadlock.
	if ( variant.m_demoType == ClosestPointsMultithreadedDemoVariant::MULTITHREADED_ON_SPU ) m_allowZeroActiveSpus = false;

	// register the default addCdPoint() function; you are free to register your own implementation here though
	hkpFixedBufferCdPointCollector::registerDefaultAddCdPointFunction();

}


ClosestPointsMultithreadedDemo::~ClosestPointsMultithreadedDemo()
{
	m_variantId = 0; // override variant id to force 'getClosestPoints' variant on base class'es destructor
}


hkDemo::Result ClosestPointsMultithreadedDemo::stepDemo()
{
	if (m_jobThreadPool->getNumThreads() == 0)
	{
		 HK_WARN(0x34561f23, "This demo does not run with only one thread");
		 return DEMO_STOP;
	}

	if ( m_env->m_gamePad->isButtonPressed(HKG_PAD_BUTTON_1) )
	{
		return stepDemoWorldGetClosestPoints();
	}
	else
	{
		return stepDemoPairGetClosestPoints();
	}
}


hkDemo::Result ClosestPointsMultithreadedDemo::stepDemoPairGetClosestPoints()
{
//	const ClosestPointsMultithreadedDemoVariant& variant = g_ClosestPointsMultithreadedDemoVariants[m_variantId];

	// the maximum number of 'closest points' that will be returned
	const int maxNumResultsPerPair = 15;

	int numBodies = m_bodies.getSize();
	int numPairs = (numBodies*(numBodies-1))/2;
	hkArray<hkpRootCdPoint> resultsArray(numPairs*maxNumResultsPerPair);

	//
	// setup commands: one command for each object pair
	//
	hkArray<hkpPairGetClosestPointsCommand> commands;
	commands.reserve(2); // this forces the actual array to be 16byte aligned!
	int resultIdx = 0;
	{
		for (int i=0; i < m_bodies.getSize(); i++)
		{
			for (int j=i+1; j < m_bodies.getSize(); j++)
			{
				hkpPairGetClosestPointsCommand& command = commands.expandOne();
				{
					// hkpWorldObject::getCollidable() needs a read-lock on the object
					m_bodies[i]->markForRead();
					m_bodies[j]->markForRead();

					command.m_collidableA = m_bodies[i]->getCollidable();
					command.m_collidableB = m_bodies[j]->getCollidable();

					m_bodies[j]->unmarkForRead();
					m_bodies[i]->unmarkForRead();

					// init output struct
					{
						command.m_results			= &resultsArray[resultIdx];
						command.m_resultsCapacity	= maxNumResultsPerPair;
						command.m_numResultsOut		= 0;
					}

				}

				resultIdx += maxNumResultsPerPair;
			}
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
	hkpPairGetClosestPointsJob pairGetClosestPointsJob(m_world->getCollisionInput(), jobHeader, commands.begin(), commands.getSize(), queryTolerance, &m_semaphore, 5);
	m_world->unmarkForRead();

	//
	// Put the job on the queue, kick-off the PPU/SPU threads and wait for everything to finish.
	//
	{
		//
		// Put the job on the job queue.
		//
		pairGetClosestPointsJob.setRunsOnSpuOrPpu();
		m_jobQueue->addJob( pairGetClosestPointsJob, hkJobQueue::JOB_LOW_PRIORITY );

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
	if ( m_env->m_gamePad->isButtonPressed(HKG_PAD_BUTTON_2) )
	{
		//
		// draw all distances between all shapes as returned by the job
		//
		for ( int commandIdx = 0; commandIdx < commands.getSize(); commandIdx++)
		{
			hkpPairGetClosestPointsCommand& command = commands[commandIdx];

			for ( int r = 0; r < command.m_numResultsOut; r++)
			{
				const hkContactPoint& pt = command.m_results[r].m_contact;
				drawContactPoint(pt, false);
			}
		}
	}
	else
	{
		//
		// only draw closest distance between each pair of rigid bodies
		//
		for ( int commandIdx = 0; commandIdx < commands.getSize(); commandIdx++)
		{
			hkpPairGetClosestPointsCommand& command = commands[commandIdx];

			//
			// search for the shortest distance among all returned distances for this pair of objects
			//
			const hkContactPoint* closestContactPointForPair = HK_NULL;
			{
				for ( int r = 0; r < command.m_numResultsOut; r++)
				{
					const hkContactPoint* contactPoint = &command.m_results[r].m_contact;
					if ( closestContactPointForPair == HK_NULL || contactPoint->getDistance() < closestContactPointForPair->getDistance() )
					{
						closestContactPointForPair = contactPoint;
					}
				}
			}

			if ( closestContactPointForPair )
			{
				drawContactPoint(*closestContactPointForPair, false);
			}
		}

	}

	hkDeallocateChunk(jobHeader, 1, HK_MEMORY_CLASS_DEMO);

	return hkDefaultPhysicsDemo::stepDemo();
}


hkDemo::Result ClosestPointsMultithreadedDemo::stepDemoWorldGetClosestPoints()
{
//	const ClosestPointsMultithreadedDemoVariant& variant = g_ClosestPointsMultithreadedDemoVariants[m_variantId];

	// we simply take 2 of the available bodies, in this case the two single spheres
	m_queryBodyA = m_bodies[1];
	m_queryBodyB = m_bodies[2];

	// The maximum number of results we are interested in for each command (i.e. each querying object).
	const int maxNumResultsPerCommand = 15;

	// We will create 2 commands, so we'll need a large enough array to hold both commands' results.
	hkArray<hkpRootCdPoint> resultArray(2*maxNumResultsPerCommand);

	// hkpWorldObject::getCollidable() needs a read-lock on the object
	m_queryBodyA->markForRead();
	m_queryBodyB->markForRead();

	//
	// Create 2 commands with each one querying the surroundings of one particular object in our world.
	//
	hkArray<hkpWorldGetClosestPointsCommand> commands;
	{
		hkpWorldGetClosestPointsCommand& command = commands.expandOne();

		command.m_collidable		= m_queryBodyA->getCollidable();
		command.m_resultsCapacity	= maxNumResultsPerCommand;
		command.m_results			= &resultArray[0];
	}
	{
		hkpWorldGetClosestPointsCommand& command = commands.expandOne();

		command.m_collidable		= m_queryBodyB->getCollidable();
		command.m_resultsCapacity	= maxNumResultsPerCommand;
		command.m_results			= &resultArray[maxNumResultsPerCommand];
	}

	m_queryBodyB->unmarkForRead();
	m_queryBodyA->unmarkForRead();

	// reasonably large buffer
	const int subCommandBufferCapacity = 20;
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
	//
	hkReal queryTolerance = 10.0f;
	m_world->markForRead();
	hkpWorldGetClosestPointsJob worldGetClosestPointsJob(m_world->getCollisionInput(), jobHeader, commands.begin(), commands.getSize(), subCommandBuffer, subCommandBufferCapacity, m_world->m_broadPhase, queryTolerance, &m_semaphore);
	m_world->unmarkForRead();

	//
	// Put the job on the queue, kick-off the PPU/SPU threads and wait for everything to finish.
	//
	{
		m_world->lockReadOnly();


		//
		// Put the job on the job queue.
		//
		worldGetClosestPointsJob.setRunsOnSpuOrPpu();
		m_jobQueue->addJob( worldGetClosestPointsJob, hkJobQueue::JOB_LOW_PRIORITY );

		m_jobThreadPool->processAllJobs( m_jobQueue );

		m_jobThreadPool->waitForCompletion();
		
		//
		// Wait for the one single job we started to finish.
		//
		m_semaphore.acquire();

		m_world->unlockReadOnly();
	}

	//
	// draw closest distances as returned by the job
	//
	{
		for (int commandIdx = 0; commandIdx < commands.getSize(); commandIdx++)
		{
			hkpWorldGetClosestPointsCommand& command = commands[commandIdx];

			for (hkUint32 r = 0; r < command.m_numResultsOut; r++)
			{
				const hkContactPoint& pt = command.m_results[r].m_contact;
				drawContactPoint(pt, false);
			}
		}
	}

	// free memory
	hkDeallocateChunk(jobHeader,		1,						  HK_MEMORY_CLASS_DEMO);
	hkDeallocateChunk(subCommandBuffer, subCommandBufferCapacity, HK_MEMORY_CLASS_DEMO);

	return hkDefaultPhysicsDemo::stepDemo();
}


#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif


HK_DECLARE_DEMO_VARIANT_USING_STRUCT( ClosestPointsMultithreadedDemo, HK_DEMO_TYPE_OTHER, ClosestPointsMultithreadedDemoVariant, g_ClosestPointsMultithreadedDemoVariants, HK_NULL );

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
