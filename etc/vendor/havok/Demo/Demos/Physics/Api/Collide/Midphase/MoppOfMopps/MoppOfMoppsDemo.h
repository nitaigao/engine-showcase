/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_MOPP_OF_MOPPS_DEMO_H
#define HK_MOPP_OF_MOPPS_DEMO_H

class hkpWorld;
class hkpListShape;

#include <Demos/DemoCommon/DemoFramework/hkDefaultPhysicsDemo.h>


/// A demo, showing useage of mopp of transformed mopps and how such shapes can be collided on SPUs.
///
/// This demo presents a simple landscape with a number trees. Where each tree is an instance of the same
/// hkpExtendedMeshShape wrapped with the same hkpMoppBvTreeShape. This is then referenced by a hkpTransformShape, 
/// grouped under a hkpListShape and wrapped with another mopp.
class MoppOfMoppsDemo : public hkDefaultPhysicsDemo
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO);

		MoppOfMoppsDemo(hkDemoEnvironment* env);

		~MoppOfMoppsDemo();

		Result stepDemo();

	protected:

		class hkLoader* m_loader;
};
#endif // HK_MOPP_OF_MOPPS_DEMO_H

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
