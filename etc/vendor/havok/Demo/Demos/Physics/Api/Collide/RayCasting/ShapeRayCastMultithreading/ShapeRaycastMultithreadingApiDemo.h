/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

// This demo shows how to use the hkpShape raycast to determine the closest point of intersection (should one exist)
// between a ray (finite, given start and end points) and an hkpShape using multithreading.
// On PLAYSTATION(R)3 we are free to also use any available SPUs for processing the jobs.
// In addition, the normal at the point of intersection is calculated.

#ifndef HK_ShapeRayCastMultithreadingApiDemo_H
#define HK_ShapeRayCastMultithreadingApiDemo_H


#include <Demos/DemoCommon/DemoFramework/hkDefaultPhysicsDemo.h>


class hkpRigidBody;


class ShapeRayCastMultithreadingApiDemo : public hkDefaultPhysicsDemo
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO);

		 ShapeRayCastMultithreadingApiDemo(hkDemoEnvironment* env);

		Result stepDemo(); 

	private:

		HK_ALIGN16 ( hkSemaphoreBusyWait	m_semaphore );

		hkpRigidBody*						m_box;
};


#endif	// HK_ShapeRayCastMultithreadingApiDemo_H

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
