/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

// This demo demonstrates the use of Collision Events (for a specific Entity) on PLAYSTATION(R)3 SPUs.

#ifndef HK_COLLISION_EVENTS_FRm_SPU_H
#define HK_COLLISION_EVENTS_FRm_SPU_H


#include <Demos/DemoCommon/DemoFramework/hkDefaultPhysicsDemo.h>
#include <Physics/Dynamics/Collide/Callback/Dispatch/hkpSpuCollisionCallbackUtil.h>


class CollisionEventsFromSpuDemo : public hkDefaultPhysicsDemo, hkSpuCollisionCallbackUtil::Callbacks
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO);

		CollisionEventsFromSpuDemo(hkDemoEnvironment* env);
		~CollisionEventsFromSpuDemo();

		Result stepDemo();

	protected:

		void contactPointAddedCallbackFromSpu  (hkSpuCollisionCallbackUtil::ContactPointAddedEvent*   event);
		void contactPointProcessCallbackFromSpu(hkSpuCollisionCallbackUtil::ContactPointProcessEvent* event);

	public:

		hkSpuCollisionCallbackUtil*			m_collisionCallbackUtil;

};


#endif // HK_COLLISION_EVENTS_FRm_SPU_H


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
