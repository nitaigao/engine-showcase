/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

// A demo which shows use of a Phantom. A phantom is an unintegrated object that maintains
// a list of other objects whose AABBs overlap with it (including other phantoms).

// We illustrate which other objects (in this case rigid bodies) are overlapping with the Phantom by
// applying an upward force to them as the phantom volume sweeps over.
// The phantom itself is just an AABB and has no physical presence in the simulation.

#ifndef HK_PHANTOMOBJECTDEMO_H
#define HK_PHANTOMOBJECTDEMO_H

#include <Demos/DemoCommon/DemoFramework/hkDefaultPhysicsDemo.h>
#include <Physics/Dynamics/Phantom/hkpAabbPhantom.h>


class PhantomObjectDemo : public hkDefaultPhysicsDemo
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO);

		PhantomObjectDemo(hkDemoEnvironment* env);

		~PhantomObjectDemo();

		Result stepDemo();

	public:

		struct Options
		{
			HK_DECLARE_REFLECTION();

			Options()
				:	m_numberOfBoxes(70),
					m_aabbWidth(2)
		   	{
			}

			int m_numberOfBoxes; //+default(70) +absmin(50) +absmax(200)
			int m_aabbWidth; //+default(2) +absmin(.1f) +absmax(20)
		};

		DEMO_OPTIONS_DECLARE(Options);
		
		hkpAabbPhantom*	m_phantom;
		hkReal m_time;
};

#endif	// !HK_PHANTOMOBJECTDEMO_H

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
