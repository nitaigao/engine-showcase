/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


#ifndef HK_RAGDOLL_ON_STAIRS_H
#define HK_RAGDOLL_ON_STAIRS_H


#include <Demos/DemoCommon/DemoFramework/hkDefaultPhysicsDemo.h>

class hkRagdoll;

///[introduction_text]
/// This demo shows a number of 11 bone ragdolls falling onto a stair case. 
/// [IMAGE Resources/ragdollOnStairs.png]
///
///
/// The ragdoll can be constructed from a number shapes types including:
/// - Box
/// - Multi-sphere
/// - Sphere
/// - Convex
/// - Capsule
/// Use the left and right arrows on the dpad to change between these types.
///
/// For a more specific example of how to set up constraints, and what each limit means,
/// please check out the constraint api demos.

class RagdollOnStairsDemo : public hkDefaultPhysicsDemo
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO);

		RagdollOnStairsDemo(hkDemoEnvironment* env);
		~RagdollOnStairsDemo();

		Result stepDemo();

	protected:

		hkArray<class hkpPhysicsSystem*>	m_ragdolls;
		int					m_ragdollType;
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
