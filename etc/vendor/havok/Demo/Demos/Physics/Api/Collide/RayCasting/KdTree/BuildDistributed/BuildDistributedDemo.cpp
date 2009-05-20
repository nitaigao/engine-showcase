/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>

#include <Demos/Physics/Api/Collide/RayCasting/KdTree/BuildDistributed/BuildDistributedDemo.h>
#include <Demos/DemoCommon/Utilities/KdTree/KdTreeDemoUtils.h>
#include <Demos/DemoCommon/Utilities/KdTree/KdTreeDebugUtils.h>

#include <Common/Internal/KdTree/hkKdTree.h>
#include <Common/Internal/KdTree/Build/hkKdTreeBuilder.h>
#include <Common/Internal/KdTree/Build/hkKdTreeBuildingUtils.h>
#include <Common/Internal/KdTree/QueryUtils/hkKdTreeUtils.h>

#include <Common/Base/Algorithm/PseudoRandom/hkPseudoRandomGenerator.h>
#include <Common/Base/Monitor/MonitorStreamAnalyzer/hkMonitorStreamAnalyzer.h>

#include <Physics/Internal/Collide/BroadPhase/hkpBroadPhase.h>

#include <Physics/Collide/Query/Multithreaded/CollisionQuery/hkpCollisionQueryJobs.h>
#include <Physics/Collide/Query/Multithreaded/CollisionQuery/hkpCollisionQueryJobQueueUtils.h>
#include <Physics/Collide/Query/Multithreaded/RayCastQuery/hkpRayCastQueryJobQueueUtils.h>
#include <Physics/Collide/Query/Multithreaded/RayCastQuery/hkpRayCastQueryJobs.h>
#include <Physics/Collide/Util/KdTree/Mediator/hkpCollidableMediator.h>


#if defined HK_PLATFORM_PS3_PPU
#define NUM_SPUS 4
#define	SPURS_THREAD_GROUP_PRIORITY 250
#define SPURS_HANDLER_THREAD_PRIORITY 1
#else
#define NUM_SPUS 1
#endif

BuildKdTreeDistributedDemo::BuildKdTreeDistributedDemo( hkDemoEnvironment* env )
:	hkDefaultPhysicsDemo(env),
	m_semaphore(0, 1000 )
{

	hkVector4 levelCenter;
	hkVector4 levelExtents;

	{
		m_worldSizeX = 2.0f * hkMath::sqrt(hkReal(m_env->m_cpuMhz));
		m_worldSizeY = 3;
		m_worldSizeZ = m_worldSizeX;
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
	// Add a lot of rigid bodies to the world
	//
	{
		hkAabb worldAabb; 
		worldAabb.m_max.set( m_worldSizeX,  10.0f*m_worldSizeY,  m_worldSizeZ);
		worldAabb.m_min.setNeg4( worldAabb.m_max );	

		hkpMotion::MotionType motionType = hkpMotion::MOTION_FIXED;
		hkPseudoRandomGenerator rand(1337);
		
		KdTreeDemoUtils::createRandomBodies(m_world, worldAabb, 2300, motionType, &rand, m_collidables);

	}

	// Setup the camera
	{

		hkVector4 up(0, 0, 2);
		hkVector4 from(0.0f, m_worldSizeZ*5.f, 0);
		hkVector4 to  (0.0f, 0.0f,  0.0f);
		setupDefaultCameras( env, from, to, up, 1.0f, 10000.0f );
	}

	setupGraphics( );

	m_tree = HK_NULL;

	m_world->unlock();

	hkpRayCastQueryJobQueueUtils::registerWithJobQueue(m_jobQueue);

	// Special case for this demo variant: we do not allow the # of active SPUs to drop to zero as this can cause a deadlock.
	{
		m_allowZeroActiveSpus = false;
	}
}

BuildKdTreeDistributedDemo::~BuildKdTreeDistributedDemo()
{
	if (m_tree)
		delete m_tree;
}

//
// Builds a kd-tree, distributing the work over several threads or SPUs
//
void BuildKdTreeDistributedDemo::buildTreeMTDistributed()
{
	m_world->markForRead();

	const int numCollidables = m_collidables.getSize();
	hkKdTreeCinfo cinfo;
	cinfo.m_numPrimitives = numCollidables;
	cinfo.m_isDistributedBuild = true;
	( (const hkpWorld*) m_world )->getBroadPhase()->getExtents(cinfo.m_aabb.m_min, cinfo.m_aabb.m_max);
	hkKdTree* spuTree = new hkKdTree(cinfo);

	
	hkKdTreeBuildInput::WorkingSet set;
	hkKdTreeBuildingUtils::allocateBuffers( set, numCollidables );

	hkpRayCastQueryJobHeader* jobHeader = hkAllocateChunk<hkpRayCastQueryJobHeader>(1, HK_MEMORY_CLASS_DEMO);

	hkArray<int> branchDepths(4);
	hkArray<int> subJobSizes(4);

	//
	// hkpKdTreeBuildCoordinatorJob handles schedules multiple hkpKdTreeBuildSetupJobs, a single hkpKdTreeBuildDistributedJob, and 4 hkpKdTreeBuildDistributedSubJobs
	// This helps avoid an extra synchronization step in the main thread between the hkpKdTreeBuildSetupJob(s) and hkpKdTreeBuildDistributedJob
	//
	hkpKdTreeBuildCoordinatorJob buildJob(jobHeader, m_world);
	m_world->unmarkForRead();

	buildJob.m_semaphore = &m_semaphore;
	buildJob.m_collidables = m_collidables.begin();

	// hkpKdTreeBuildDistributedJob info
	buildJob.m_primitivesIn				= set.m_entriesIn;
	buildJob.m_primitivesOut			= set.m_entriesOut;
	buildJob.m_branchDepthsOut			= branchDepths.begin();
	buildJob.m_subJobSizes				= subJobSizes.begin();
	buildJob.m_numPrimitives			= spuTree->getNumPrimitives();
	buildJob.m_treeOut					= spuTree->getRoot();
	buildJob.m_nodeArraySize			= spuTree->getTotalNumNodes();
	buildJob.m_projEntOut				= spuTree->getProjectedEntries();
	buildJob.m_numAvailableEmptyNodes	= spuTree->getNumReservedEmptyNodes();

	{
		HK_TIME_CODE_BLOCK("buildTreeMTDistributed", 0);
		m_jobQueue->addJob( *reinterpret_cast<hkJobQueue::JobQueueEntry*>(&buildJob), hkJobQueue::JOB_HIGH_PRIORITY );
		m_jobThreadPool->processAllJobs( m_jobQueue );
		m_jobThreadPool->waitForCompletion();

		//
		// Wait for the one single job we started to finish.
		//
		m_semaphore.acquire();

		hkKdTreeBuildingUtils::completeDistributedBuild( spuTree, numCollidables, buildJob.m_branchDepthsOut, buildJob.m_subJobSizes );
	}

	hkKdTreeBuildingUtils::deallocateBuffers( set, numCollidables );
	hkDeallocateChunk(jobHeader, 1, HK_MEMORY_CLASS_DEMO);

	{
		m_world->markForRead();
		hkpCollidableMediator mediator( m_collidables, m_world );
		m_world->unmarkForRead();
		hkKdTreeDebugUtils::validateTree(spuTree, mediator);
		hkKdTreeDebugUtils::verifyTreeRaycasts(spuTree, m_world, 10);
	}

	delete spuTree;
}
 
//
// Builds a kd-tree, with all of the work on a single separate thread or SPU
//
void BuildKdTreeDistributedDemo::buildTreeMT()
{
	m_world->markForWrite();
	const int numCollidables = m_collidables.getSize();
	hkKdTreeCinfo cinfo;
	cinfo.m_numPrimitives = numCollidables;
	cinfo.m_isDistributedBuild = false;
	( (const hkpWorld*) m_world )->getBroadPhase()->getExtents(cinfo.m_aabb.m_min, cinfo.m_aabb.m_max);
	hkKdTree* spuTree = new hkKdTree(cinfo);
	

	hkKdTreeBuildInput::WorkingSet set;
	hkKdTreeBuildingUtils::allocateBuffers(set, numCollidables);

	hkpCollidableMediator mediator(m_collidables, m_world);
	m_world->unmarkForWrite();

	hkpKdTreeBuildSetupJob setupJob;
	setupJob.m_semaphore	= &m_semaphore;
	setupJob.m_collidables	= m_collidables.begin();
	setupJob.m_entriesOut	= set.m_entriesIn; //entriesIn.begin();
	setupJob.m_startIdx		= 0;
	setupJob.m_endIdx		= numCollidables;

	{
		HK_TIME_CODE_BLOCK("buildSetup", 0);
		m_jobQueue->addJob( *reinterpret_cast<hkJobQueue::JobQueueEntry*>(&setupJob), hkJobQueue::JOB_HIGH_PRIORITY );
		m_jobThreadPool->processAllJobs( m_jobQueue );
		m_jobThreadPool->waitForCompletion();

		//
		// Wait for the one single job we started to finish.
		//
		m_semaphore.acquire();
	}

	hkpKdTreeBuildJobFast buildJob(spuTree);
	buildJob.m_semaphore		= &m_semaphore;
	buildJob.m_entriesIn		= set.m_entriesIn; 
	buildJob.m_entriesOut		= set.m_entriesOut;

	{
		HK_TIME_CODE_BLOCK("buildTreeMT", 0);
		m_jobQueue->addJob( *reinterpret_cast<hkJobQueue::JobQueueEntry*>(&buildJob), hkJobQueue::JOB_HIGH_PRIORITY);
		m_jobThreadPool->processAllJobs( m_jobQueue );
		m_jobThreadPool->waitForCompletion();

		//
		// Wait for the one single job we started to finish.
		//
		m_semaphore.acquire();	}

	hkKdTreeDebugUtils::validateTree(spuTree, mediator);
	hkKdTreeDebugUtils::verifyTreeRaycasts(spuTree, m_world, 10);

	hkKdTreeBuildingUtils::deallocateBuffers(set, numCollidables);

	delete spuTree;
}

//
// Builds a kd-tree in a single thread
//
void BuildKdTreeDistributedDemo::buildTreeST()
{
	m_world->markForRead();
	hkpCollidableMediator mediator( m_collidables, m_world );

	if (m_tree)
		delete m_tree;

	const int numPrims = mediator.getNumPrimitives();
	hkKdTreeCinfo cinfo;
	cinfo.m_numPrimitives = numPrims;
	cinfo.m_isDistributedBuild = false;
	( (const hkpWorld*) m_world )->getBroadPhase()->getExtents(cinfo.m_aabb.m_min, cinfo.m_aabb.m_max);
	m_tree = new hkKdTree(cinfo);
	m_world->unmarkForRead();


	HK_TIMER_BEGIN("buildTreeST", HK_NULL);
	hkKdTreeBuildInput::WorkingSet set;
	hkKdTreeBuildingUtils::allocateBuffers( set, numPrims );
	hkKdTreeBuildingUtils::projectPrimitives(&mediator, 0, numPrims, set.m_entriesIn);
	hkKdTreeBuildingUtils::buildTree( set, m_tree);
	hkKdTreeBuildingUtils::deallocateBuffers( set, numPrims );
	HK_TIMER_END();

	hkKdTreeDebugUtils::validateTree(m_tree, mediator);
	hkKdTreeDebugUtils::verifyTreeRaycasts(m_tree, m_world, 10);	
}

//
// Builds a kd-tree in a single thread, but using the distributed building methods
// There is no real reason to use this technique over the one in buildTreeST(); it's only included here for testing
//
void BuildKdTreeDistributedDemo::buildTreeSTDistributed ()
{
	m_world->markForRead();
	hkpCollidableMediator mediator( m_collidables, m_world );

	const int numPrims = m_collidables.getSize();

	if (m_tree)
		delete m_tree;

	hkKdTreeCinfo cinfo;
	cinfo.m_numPrimitives = numPrims;
	cinfo.m_isDistributedBuild = true;
	( (const hkpWorld*) m_world )->getBroadPhase()->getExtents(cinfo.m_aabb.m_min, cinfo.m_aabb.m_max);
	m_tree = new hkKdTree(cinfo);

	m_world->unmarkForRead();

	HK_TIMER_BEGIN("buildTreeSTDistributed", HK_NULL);
	hkKdTreeBuildingUtils::buildTreeDistributed( mediator, m_tree );
	HK_TIMER_END();

	hkKdTreeDebugUtils::validateTree( m_tree, mediator );
	hkKdTreeDebugUtils::verifyTreeRaycasts(m_tree, m_world, 10);
}


hkDemo::Result BuildKdTreeDistributedDemo::stepDemo()
{
	if (m_jobThreadPool->getNumThreads() == 0)
	{
		 HK_WARN(0x34561f23, "This demo does not run with only one thread");
		 return DEMO_STOP;
	}

#if defined(HK_PLATFORM_MULTI_THREAD) && (HK_CONFIG_THREAD == HK_CONFIG_MULTI_THREADED)
	buildTreeMT();
	buildTreeMTDistributed();
#endif
	buildTreeST();
	buildTreeSTDistributed();

	return hkDemo::DEMO_OK;
}


#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char* helpString = "This demo demonstrates the various ways that a kd-tree can be built, along with the timings for each."
" It maintains its own list of bodies, instead of using the built-in hkpKdTreeWorldManager.";

HK_DECLARE_DEMO(BuildKdTreeDistributedDemo, HK_DEMO_TYPE_PHYSICS, "KdTree building", helpString);

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
