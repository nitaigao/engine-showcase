/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

// This demo demonstrates how to emulate the hkpWorld::castRay() functionality in a multithreaded
// environment through the use of a dedicated hkpWorldRayCastJob. By putting such a job on
// the job queue it can be processed by other threads or SPUs (on PLAYSTATION(R)3).
// One command of this job will cast exactly one ray into the world, using the broadphase to avoid
// expensive low-level intersection tests. You can place an arbitrary number of commands into one
// job, as the job will automatically split itself into parallel sub-jobs.
// For more information on the original hkpWorld::castRay() functionality, see WorldRayCastDemo.

#ifndef HK_WorldRayCastMultithreadedDemo_H
#define HK_WorldRayCastMultithreadedDemo_H


#include <Demos/DemoCommon/DemoFramework/hkDefaultPhysicsDemo.h>


class WorldRayCastMultithreadedDemo : public hkDefaultPhysicsDemo
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO);

	public:

		 WorldRayCastMultithreadedDemo(hkDemoEnvironment* env);

		Result stepDemo(); 

	protected:

		void createBodies();

	private:

 		HK_ALIGN128( hkSemaphoreBusyWait	m_semaphore );

		hkReal								m_time;
};

#endif	// HK_WorldRayCastMultithreadedDemo_H

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
