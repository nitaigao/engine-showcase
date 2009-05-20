/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_CrashTestDummies_H
#define HK_CrashTestDummies_H

#include <Demos/DemoCommon/DemoFramework/hkDefaultPhysicsDemo.h>

class hkRagdoll;
class hkpWorld;
class hkpGroupFilter;


class CrashTestDummiesDemo : public hkDefaultPhysicsDemo
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO);

		CrashTestDummiesDemo(hkDemoEnvironment* environment);
		~CrashTestDummiesDemo();

		Result stepDemo(); 
		
		void displayTimePoint( hkTime time, hkReal physicsDeltaTime, int color);

	private:

		static hkpRigidBody* HK_CALL createSimpleCarHull();
		void fitRagdollsIn(const hkVector4& velocity);

		void putBoxesIn(const hkVector4& pos);
		void createGroundBox();
		void createFastObject();

        void addRagdoll();
        void removeRagdoll();

		hkpPhysicsSystem*		m_ragdoll;
		hkpGroupFilter*	m_filter;

		hkpRigidBody* m_car;

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
