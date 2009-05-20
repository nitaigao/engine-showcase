/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_SLIDING_RAGDOLLS_DEMO_H
#define HK_SLIDING_RAGDOLLS_DEMO_H


#include <Demos/DemoCommon/DemoFramework/hkDefaultPhysicsDemo.h>

///[Introduction]
/// A demo that shows many ragdolls simulated on multiple threads
class SlidingRagdollsDemo : public hkDefaultPhysicsDemo
{
	public:
		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO);

		SlidingRagdollsDemo(hkDemoEnvironment* env);

		virtual ~SlidingRagdollsDemo();

		virtual Result stepDemo(); 



	protected:

			// Build the landscape to drive on and add it to m_world.
		void buildLandscape();

		static void createRagdollGrid( hkpWorld* world, int x_size, int y_size, hkReal xStep, hkReal yStep, hkArray<hkpPhysicsSystem*>&	ragdollsOut);
			
		void respawnRagdollsIfNecessary();
	public:

		hkArray<hkpPhysicsSystem*>	m_ragdolls;

		
};

#endif // HK_SLIDING_RAGDOLLS_DEMO_H

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
