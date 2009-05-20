/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/Physics/Api/Collide/RayCasting/KdTree/AabbQuery/AabbQueryDemo.h>
#include <Demos/DemoCommon/Utilities/KdTree/KdTreeDemoUtils.h>

#include <Common/Internal/KdTree/hkKdTree.h>
#include <Common/Internal/KdTree/QueryUtils/hkKdTreeAabbQueryUtils.h>
#include <Physics/Dynamics/World/Util/KdTree/hkpKdTreeWorldManager.h>
#include <Physics/Internal/Collide/BroadPhase/hkpBroadPhase.h>

// Used for graphics, I/O, and profiling
#include <Common/Base/Monitor/hkMonitorStream.h>
#include <Common/Base/Container/LocalArray/hkLocalArray.h>
#include <Common/Visualize/hkDebugDisplay.h>
#include <Common/Base/Algorithm/PseudoRandom/hkPseudoRandomGenerator.h>
#include <Demos/DemoCommon/Utilities/MonitorHelper/MonitorHelperMultithreaded.h>

#include <Physics/Collide/Agent/hkpProcessCollisionInput.h>
#include <Physics/Collide/Dispatch/BroadPhase/hkpTypedBroadPhaseHandlePair.h>

// Multithreading
#include <Physics/Collide/Query/Multithreaded/RayCastQuery/hkpRayCastQueryJobs.h>
#include <Physics/Collide/Query/Multithreaded/RayCastQuery/hkpRayCastQueryJobQueueUtils.h>


#ifdef HK_DEBUG
static const int numQueries = 2;
#else
static const int numQueries = 100;
#endif


AabbQueryDemo::AabbQueryDemo(hkDemoEnvironment* env)
:	hkDefaultPhysicsDemo(env), 
	m_semaphore(0, 1000 ), 
	m_rand(1337)
{

	{
		m_worldSizeX = 2.0f * hkMath::sqrt(hkReal(m_env->m_cpuMhz));
		m_worldSizeY = 3;
		m_worldSizeZ = m_worldSizeX;
	}

	// Setup the camera and graphics
	{
		// setup the camera
		hkVector4 from(0.0f, m_worldSizeZ*2.f, -m_worldSizeZ);
		hkVector4 to  (0.0f, 0.0f,  0.0f);
		hkVector4 up  (0.0f, 1.0f,  0.0f);
		setupDefaultCameras( env, from, to, up, 1.0f, 5000.0f );
	}


	{
		hkpWorldCinfo cinfo;
		cinfo.m_gravity.setAll(0);
		cinfo.m_broadPhaseWorldAabb.m_max.set( m_worldSizeX,  10.0f*m_worldSizeY,  m_worldSizeZ);
		cinfo.m_broadPhaseWorldAabb.m_min.setNeg4(	cinfo.m_broadPhaseWorldAabb.m_max );	
		cinfo.m_useKdTree = true;
		m_world = new hkpWorld(cinfo);
		m_world->lock();
	}

	{
		hkpAgentRegisterUtil::registerAllAgents(m_world->getCollisionDispatcher());
	}

	//
	// Create a random batch of boxes in the world
	//
	{
		hkAabb worldAabb; 
		worldAabb.m_max.set( m_worldSizeX,  m_worldSizeY, m_worldSizeZ );
		worldAabb.m_min.setNeg4( worldAabb.m_max );	

		hkpMotion::MotionType motionType = hkpMotion::MOTION_SPHERE_INERTIA;
		hkPseudoRandomGenerator rand(100);

		KdTreeDemoUtils::createRandomBodies(m_world, worldAabb, 500, motionType, &rand, m_collidables);
	}

	setupGraphics();

	m_world->unlock();

	hkpRayCastQueryJobQueueUtils::registerWithJobQueue(m_jobQueue);
	m_collIdx = 0;

	// timer tracking
	m_monitorHelper = new MultithreadedMonitorHelper(this);
	m_monitorHelper->trackTimer("KdAabbQueryJob", MultithreadedMonitorHelper::TRACK_FRAME_MAX);
	m_monitorHelper->trackTimer("KdQueryRecursiveST", MultithreadedMonitorHelper::TRACK_FRAME_MAX);
	m_monitorHelper->trackTimer("KdQueryIterativeST", MultithreadedMonitorHelper::TRACK_FRAME_MAX);
	m_monitorHelper->trackTimer("BroadphaseQueryAabb", MultithreadedMonitorHelper::TRACK_FRAME_MAX);
}

AabbQueryDemo::~AabbQueryDemo()
{
	delete m_monitorHelper;
}


//
// Test function to make sure that the hits from the kd-tree are identical to the ones from the broadphase.
//
static hkBool compareHitArrays(const hkAabb& aabb, const hkPrimitiveId* kdHits, int numKdHits, const hkArray<hkpBroadPhaseHandlePair>& sapHits)
{
	hkLocalArray<const hkpCollidable*> sapCollidables(sapHits.getSize());

	for (int i=0; i<sapHits.getSize(); i++)
	{
		const hkpTypedBroadPhaseHandle* tp = static_cast<const hkpTypedBroadPhaseHandle*>( sapHits[i].m_b );
		const hkpCollidable* collB = static_cast<hkpCollidable*>(tp->getOwner());

		// Make sure no spurious hits from the broadphase
		hkAabb colAabb;
		hkpRigidBody* rb = hkGetRigidBody(collB);

		if (rb)
		{
			collB->getShape()->getAabb(rb->getTransform(), 0.0f, colAabb);
			hkBool aabbOverlap = colAabb.overlaps(aabb);

			if (aabbOverlap)
				sapCollidables.pushBack(collB);
		}
	}

	// Make sure that the contents of the arrays at the same
	// It would be faster to sort and do a string compare, but this is just as easy.
	
	hkBool isOk = true;
	for (int i=0; i<numKdHits; i++)
	{
		const hkpCollidable* kdCollidable = reinterpret_cast<const hkpCollidable*> (kdHits[i]);
		int idx = sapCollidables.indexOf(kdCollidable);
		
		if(idx < 0)
		{
			// There's a hit in the kd-tree's list but not the 3AxisSweep's list
			// It's possible that 3AxisSweep skipped something. So get the AABB of the body in the kd-tree list.
			hkAabb colAabb;
			hkpRigidBody* rb = hkGetRigidBody(kdCollidable);
			kdCollidable->getShape()->getAabb(rb->getTransform(), rb->getWorld()->getCollisionInput()->getTolerance(), colAabb);
			
			hkBool aabbOverlap = colAabb.overlaps(aabb);
			if (!aabbOverlap)
			{
				// Something in the list doesn't overlap with the query's aabb
				isOk = false;
			}

			continue;
		}

		sapCollidables.removeAt(idx);
	}

	// If we made it this far, they must agree.
	return isOk && sapCollidables.isEmpty();
}

void AabbQueryDemo::queryAabbST()
{
	HK_TIME_CODE_BLOCK("queryAabbST", HK_NULL);
	hkAabb aabb;
	// Grab a body from the world, and compute it's AABB + some padding
	const hkpCollidable* queryCollidable;
	{
		queryCollidable = m_collidables[m_collIdx];
		hkpRigidBody* rb = hkGetRigidBody(queryCollidable);
		queryCollidable->getShape()->getAabb(rb->getTransform(), 20.0f, aabb);
	}

	//
	// For performance timings, we query the same AABB multiple times and overwrite the results.
	// When using this, make sure to use different output arrays!
	//
	hkArray<hkPrimitiveId> kdhitsRecurs, kdhitsIter;		
	hkArray<hkpBroadPhaseHandlePair> sapHits;

	{
		HK_TIME_CODE_BLOCK("KdQueryRecursiveST", HK_NULL);
		for (int i=0; i<numQueries; i++)
		{
			kdhitsRecurs.clear();
			hkKdTreeAabbQueryUtils::queryAabbRecursive(m_world->m_kdTreeManager->getTree(), aabb, kdhitsRecurs);
		}
	}

	{
		HK_TIME_CODE_BLOCK("KdQueryIterativeST", HK_NULL);
		for (int i=0; i<numQueries; i++)
		{
			kdhitsIter.clear();
			hkKdTreeAabbQueryUtils::queryAabbIterative(m_world->m_kdTreeManager->getTree(), aabb, kdhitsIter);
		}
	}

	// Visualize the results
	HK_DISPLAY_BOUNDING_BOX(aabb, hkColor::YELLOW);

	for (int i=0; i<kdhitsIter.getSize(); i++)
	{
		HK_SET_OBJECT_COLOR(kdhitsIter[i], hkColor::YELLOW);		
	}

	HK_SET_OBJECT_COLOR((hkUlong)queryCollidable, hkColor::RED);


	// Call the corresponding hkp3AxisSweep method for comparison
	{
		HK_TIME_CODE_BLOCK("BroadphaseQueryAabb", HK_NULL);
		for (int i=0; i<numQueries; i++)
		{
			sapHits.clear();
			m_world->getBroadPhase()->querySingleAabb( aabb, sapHits );
		}

	}

	{
		hkBool iterOk = compareHitArrays(aabb, kdhitsIter.begin(), kdhitsIter.getSize(), sapHits); 
		hkBool recursOk = compareHitArrays(aabb, kdhitsRecurs.begin(), kdhitsRecurs.getSize(), sapHits);

		if( !(iterOk && recursOk) )
		{
			m_env->m_textDisplay->outputText("ST Hit lits differed!", 20, 150, (hkUint32) hkColor::RED);
		}
	}
}

void AabbQueryDemo::queryAabbMT()
{
	HK_TIMER_BEGIN_LIST("queryAabbMT", "setup");

	hkAabb aabb;
	// Grab a body from the world, and compute it's AABB + some padding
	const hkpCollidable* queryCollidable;
	{
		queryCollidable = m_collidables[m_collIdx];
		hkpRigidBody* rb = hkGetRigidBody(queryCollidable);
		queryCollidable->getShape()->getAabb(rb->getTransform(), 20.0f, aabb);
	}

	hkArray<hkpKdTreeAabbCommand> commands;
	commands.setSize(numQueries);
	
	//
	// For performance timings, we query the same AABB multiple times and overwrite the results.
	// When using this, make sure to use different output arrays!
	//
	hkArray<hkPrimitiveId> output;
	output.setSize(50);

	//
	// Set up the commands for the job
	//
	for (int i=0; i<commands.getSize(); i++)
	{
		hkpKdTreeAabbCommand& command = commands[i];
		command.m_aabb = aabb;
		command.m_results = output.begin();
		command.m_resultsCapacity = output.getSize();
		command.m_numResultsOut = 0;
	}


	// 
	// Setup the job
	//
	hkArray<hkpRayCastQueryJobHeader> header(1);
	hkpKdTreeAabbJob aabbJob(header.begin(), commands.begin(), commands.getSize(), &m_semaphore);
	aabbJob.m_numTrees = 1;
	aabbJob.m_trees[0] = m_world->m_kdTreeManager->getTree();

	m_jobQueue->addJob( *reinterpret_cast<hkJobQueue::JobQueueEntry*>(&aabbJob), hkJobQueue::JOB_HIGH_PRIORITY );

	HK_TIMER_SPLIT_LIST("process");
	m_jobThreadPool->processAllJobs( m_jobQueue );
	m_jobThreadPool->waitForCompletion();
	m_semaphore.acquire();

	HK_TIMER_END_LIST();

	//
	// Run the same query on the broadphase for comparison purposes
	//
	hkArray<hkpBroadPhaseHandlePair> sapHits;
	{
		HK_TIME_CODE_BLOCK("BroadphaseQueryAabb", HK_NULL);
		for (int i=0; i<numQueries; i++)
		{
			sapHits.clear();
			m_world->getBroadPhase()->querySingleAabb( aabb, sapHits );
		}

	}

	//
	// Check results and draw 
	//
	for (int i=0; i<commands.getSize(); i++)
	{
		hkpKdTreeAabbCommand& command = commands[i];
		hkBool jobOk = compareHitArrays(command.m_aabb, command.m_results, command.m_numResultsOut, sapHits); 

		for (int j=0; j<command.m_numResultsOut; j++)
		{
			HK_SET_OBJECT_COLOR(command.m_results[j], hkColor::YELLOW);
		}
		HK_SET_OBJECT_COLOR((hkUlong)queryCollidable, hkColor::LIME);

		if( !jobOk )
		{
			m_env->m_textDisplay->outputText("MT Hit lits differed!", 20, 250, (hkUint32) hkColor::RED);
		}
	}


}

hkDemo::Result AabbQueryDemo::stepDemo()
{
	if (m_jobThreadPool->getNumThreads() == 0)
	{
		 HK_WARN(0x34561f23, "This demo does not run with only one thread");
		 return DEMO_STOP;
	}

	hkDemo::Result result = hkDefaultPhysicsDemo::stepDemo();

	m_world->markForWrite();

	{
		queryAabbST();
		queryAabbMT();
	}
	
	m_world->unmarkForWrite();

	{
		m_monitorHelper->updateTree();
		hkString timers;
		hkReal recursive, iterative, spu = 0.0f, bp;

		recursive = m_monitorHelper->findTrackedTimer("KdQueryRecursiveST", MultithreadedMonitorHelper::TRACK_FRAME_MAX)->getAvgFrameValue();
		iterative = m_monitorHelper->findTrackedTimer("KdQueryIterativeST", MultithreadedMonitorHelper::TRACK_FRAME_MAX)->getAvgFrameValue();
		spu = m_monitorHelper->findTrackedTimer("KdAabbQueryJob", MultithreadedMonitorHelper::TRACK_FRAME_MAX)->getAvgFrameValue();
		
		// This gets called twice, but the helper only picks up once instance of it.
		bp = m_monitorHelper->findTrackedTimer("BroadphaseQueryAabb", MultithreadedMonitorHelper::TRACK_FRAME_MAX)->getAvgFrameValue();

		timers.printf("CPU recursive: %f\nCPU iterative: %f\nSPU query: %f\nbroadphase: %f", recursive, iterative, spu, bp);

		m_env->m_textDisplay->outputText(timers.cString(), 20, 400);
	}

	return result;

}


#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
"The world's kd-tree can also be used for AABB queries. Depending on the size of the AABB and the number of bodies in the world," \
" it may be faster to use hkp3AxisSweep::querySingleAabb instead.";

HK_DECLARE_DEMO(AabbQueryDemo, HK_DEMO_TYPE_PHYSICS, "AABB queries", helpString);

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
