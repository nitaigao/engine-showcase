/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

// This demo shows how to find the closest point(s) between a given pair of objects using multithreading.
// On PLAYSTATION(R)3 we are free to also use an available SPU for processing the job.
// Note that in the demo's current implementation only one single job is created and thus only one
// additional thread (be it on PPU/CPU or on SPU) is actually utilized. This was done to keep the
// demo code as simple as possible.

#ifndef HK_ClosestPointsMultithreadingApi_H
#define HK_ClosestPointsMultithreadingApi_H


#include <Demos/DemoCommon/DemoFramework/hkDefaultPhysicsDemo.h>


class ClosestPointsMultithreadingApiDemo : public hkDefaultPhysicsDemo
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DEMO);

		 ClosestPointsMultithreadingApiDemo(hkDemoEnvironment* env);
		~ClosestPointsMultithreadingApiDemo();

		Result stepDemo();
		void makeFakeInput();

	private:

		void createBodies();

		Result stepDemoPairGetClosestPoints();
		Result stepDemoWorldGetClosestPoints();

		void drawContactPoint(const hkContactPoint& contactPoint);

	private:

 		HK_ALIGN128( hkSemaphoreBusyWait	m_semaphore );

		hkpRigidBody*						m_bodies[3];
};


#endif // HK_ClosestPointsMultithreadingApi_H

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
