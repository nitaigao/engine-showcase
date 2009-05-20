/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/DemoCommon/DemoFramework/PhysicsStepper.h>
#include <Common/Base/Thread/JobQueue/hkJobQueue.h>
#include <Common/Base/Thread/Job/ThreadPool/hkJobThreadPool.h>
#include <Physics/Dynamics/World/hkpWorld.h>

void HK_CALL PhysicsDemoStepper::init( hkDefaultDemo* demo, hkpWorld* world )
{
	PhysicsDemoStepper* stepper = new PhysicsDemoStepper();
	stepper->m_world = world;
	demo->addStepper( stepper );
}

hkDemo::Result PhysicsDemoStepper::stepDemo( hkDefaultDemo* demo )
{
	if ( m_world->m_simulationType == hkpWorldCinfo::SIMULATION_TYPE_MULTITHREADED )
	{
		m_world->initMtStep( demo->m_jobQueue, demo->m_timestep );

		demo->m_jobThreadPool->processAllJobs( demo->m_jobQueue );
		demo->m_jobQueue->processAllJobs( );
		demo->m_jobThreadPool->waitForCompletion();

		m_world->finishMtStep( demo->m_jobQueue, demo->m_jobThreadPool );
	}
	else
	{
		m_world->stepDeltaTime(demo->m_timestep);
	}

	return hkDemo::DEMO_OK;
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
