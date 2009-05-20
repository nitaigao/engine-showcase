/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

// A demo which shows use of the hkpWorld raycast which searches all Shapes of all the RigidBodies
// in the World for intersection, returning the minimum if an intersection occurs. It is optimized 
// to use the broad-phase to avoid expensive low-level intersection tests, and thus will usually be
// considerably faster to use than simply iterating through all RigidBodies in the World.
// The ray is moved over time in this demo to illustrate correctness of the raycast even as the bodies
// move around the world.

#ifndef HK_WorldRaycastDemo_H
#define HK_WorldRaycastDemo_H

#include <Demos/DemoCommon/DemoFramework/hkDefaultPhysicsDemo.h>

class hkpRigidBody;
class hkpMoppBvTreeShape;


class WorldRaycastDemo : public hkDefaultPhysicsDemo
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO);

		WorldRaycastDemo(hkDemoEnvironment* env);
		Result stepDemo(); 

		// Helper methods
		void createBodies();

		hkReal m_time;
};

#endif	// HK_WorldRaycastDemo_H

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
