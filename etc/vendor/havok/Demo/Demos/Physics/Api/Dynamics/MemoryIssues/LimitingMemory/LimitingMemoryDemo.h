/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

// This is a very simple object manager, that will remove "debris" and "ragdolls" from a simulation.
// The debris and ragdoll objects must be explicitly added to the manager. It will add a reference
// to these objects, and will try to remove them from the world in a "first in / first out" order.
// NOTE: This is intended as sample code, and is designed to be simple and easy to understand.
// It is recommended you create your own watch dog class to suit your own needs.

#ifndef HK_LimitingMemoryDemo_H
#define HK_LimitingMemoryDemo_H

#include <Demos/DemoCommon/DemoFramework/hkDefaultPhysicsDemo.h>
#include <Physics/Dynamics/World/Memory/hkpWorldMemoryAvailableWatchDog.h>
#include <Physics/Dynamics/World/hkpPhysicsSystem.h>


class MySimpleObjectWatchDog : public hkWorldMemoryAvailableWatchDog
{
	public:
        MySimpleObjectWatchDog( hkUint32 minAvailable ) : hkWorldMemoryAvailableWatchDog( minAvailable ) {}
		~MySimpleObjectWatchDog();

		virtual void freeMemory( hkpWorld* world );

		void addRagdoll( hkpPhysicsSystem* system )
		{
			system->addReference();
			m_ragdolls.pushBack(system);
		}

		void addDebris( hkpRigidBody* debris )
		{
			debris->addReference();
			m_debris.pushBack(debris);
		}

		void removeRagdoll( hkpPhysicsSystem* system )
		{
			system->removeReference();
			m_ragdolls.removeAt(m_ragdolls.indexOf(system));
		}

		void removeDebris( hkpRigidBody* debris )
		{
			debris->addReference();
			m_debris.removeAt(m_debris.indexOf(debris));
		}

	protected:

		hkArray<hkpPhysicsSystem*> m_ragdolls;
		hkArray<hkpRigidBody*> m_debris;
};


class LimitingMemoryDemo : public hkDefaultPhysicsDemo
{
	public:
		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO);

		LimitingMemoryDemo(hkDemoEnvironment* environment);
		~LimitingMemoryDemo();

		Result stepDemo();

	private:
		void createHollowSphereMopp(hkpMotion::MotionType motionType);

		void createAndAddRagdoll();
		void createAndAddDebris();

		MySimpleObjectWatchDog* m_watchDog;

		hkReal	m_timeToSpawnNewRagdoll;
		int m_numCurrentRagdolls;

		hkReal m_timeToSpawnDebris;

		hkpGroupFilter* m_filter;
		hkGeometry* m_geom;

		/// This is used to track how many 'stepDemo' calls are made. We only set 
		/// the memory limit in this demo after a couple of frames, so we can take into account
		/// the thread memory overhead.
		int m_frameCount;
};

#endif

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
