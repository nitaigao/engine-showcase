/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


#ifndef HK_DestructibleShackDEMO_H
#define HK_DestructibleShackDEMO_H


#include <Demos/DemoCommon/DemoFramework/hkDefaultPhysicsDemo.h>


// Faults of this demo:
//  - it estimates impulse based on a single cp, and bodie's state; it does it for every new contact point, so total impulse accumulated might be unrealistically large
//  - when objects are removed from mopp, it does not disable contact points that were added in previous frames


class DestructibleShackDemo : public hkDefaultPhysicsDemo
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO);

		 DestructibleShackDemo(hkDemoEnvironment* env);
		~DestructibleShackDemo();

		hkDemo::Result stepDemo();

		hkArray<class DestructibleHierarchy*> m_hierarchies;
		hkArray<class DestructibleHierarchyListener*> m_destructionListeners;

		static DestructibleHierarchy* buildShack( const hkVector4& halfSize );
};



#endif // HK_DestructibleShackDEMO_H


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
