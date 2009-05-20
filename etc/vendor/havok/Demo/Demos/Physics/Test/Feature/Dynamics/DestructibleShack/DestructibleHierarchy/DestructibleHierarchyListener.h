/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


#ifndef HK_DestructibleHierarchyListener_H
#define HK_DestructibleHierarchyListener_H



#include <Demos/DemoCommon/DemoFramework/hkDefaultPhysicsDemo.h>

#include <Physics/Dynamics/Collide/hkpCollisionListener.h>
#include <Physics/Dynamics/Entity/hkpEntityListener.h>


struct DestructibleHierarchyCollisionEvent
{
	hkContactPoint m_contactPoint;
	hkReal m_impulse;
	class DestructibleHierarchy* m_hierarchy;
	unsigned int m_nodeIdx;

	hkpRigidBody* m_body;
	hkpShapeKey m_shapeKey;
	class hkpContactMgr* m_contactMgr;
	hkContactPointId m_contactPointId;
};

class DestructibleHierarchyListener : public hkpCollisionListener, public hkpEntityListener
{
	public:

		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO, DestructibleHierarchyListener);
		//DestructibleHierarchyListener(hkpRigidBody* body)
		//{
		//		// weak reference (not counted)
		//	m_body = body;
		//	m_body->addCollisionListener(this);
		//	m_body->addEntityListener(this);
		//}

		//~DestructibleHierarchyListener()
		//{
		//	if (m_body)
		//	{
		//		m_body->removeCollisionListener(this);
		//		m_body->removeEntityListener(this);
		//	}
		//}

		virtual void contactPointAddedCallback(	hkpContactPointAddedEvent& event);

		virtual void contactPointConfirmedCallback( hkpContactPointConfirmedEvent& event) {}

		virtual void contactPointRemovedCallback( hkpContactPointRemovedEvent& event );

		//virtual void entityDeletedCallback( hkpEntity* entity )
		//{
		//	HK_ASSERT2(0xad678aba, entity == m_body, "Unexpected entity.");
		//	m_body = HK_NULL;
		//}

		inline void reset()
		{
			m_collisionEvents.clear();
		}

		void processCollisionEvents();

//		hkpRigidBody* m_body;
		hkArray<DestructibleHierarchyCollisionEvent> m_collisionEvents;
};



#endif // HK_DestructibleHierarchyListener_H


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
