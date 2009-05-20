/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

// This demo uses the SPUs to perform a number of linear casts

#ifndef HK_PairLinearCastMultithreadedDemo_H
#define HK_PairLinearCastMultithreadedDemo_H


#include <Demos/DemoCommon/DemoFramework/hkDefaultPhysicsDemo.h>


class PairLinearCastMultithreadedDemo  : public hkDefaultPhysicsDemo
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DEMO);

		 PairLinearCastMultithreadedDemo (hkDemoEnvironment* env);
		~PairLinearCastMultithreadedDemo ();

		Result stepDemo();

	protected:

		void setupWorld(hkDemoEnvironment* env, bool spuSupportedShapesOnly);
		hkpRigidBody* createTriangle(const hkpRigidBodyCinfo& tmplt, const hkVector4& pos, hkReal mass);
		void drawContactPoint(const hkContactPoint& contactPoint, hkBool isGjk);

	private:

 		HK_ALIGN128( hkSemaphoreBusyWait	m_semaphore );

		hkArray<hkpRigidBody*>				m_bodies;
		hkReal								m_time;
};


#endif // HK_PairLinearCastMultithreadedDemo_H

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
