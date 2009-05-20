/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

/// A demo which creates an array of box piles.
/// Each pile is used to show how the hkpGroupFilter works.

#ifndef HK_LIMITING_MEMORY_2_H
#define HK_LIMITING_MEMORY_2_H

#include <Demos/DemoCommon/DemoFramework/hkDefaultPhysicsDemo.h>

class hkpMoppBvTreeShape;


class LimitingMemory2Demo :public hkDefaultPhysicsDemo
{
	public:

		~LimitingMemory2Demo();

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO);

		LimitingMemory2Demo(hkDemoEnvironment* env);
		Result stepDemo();

		hkpMoppBvTreeShape* createMoppShape();

	private:
		int m_numFramesToWaitBeforeAddingConstraintBatch;


	// mopp stuff

		enum { NUM_VERTICES = 20 };
		enum { NUM_TRIANGLES = NUM_VERTICES - 2 };

		float		m_vertices[NUM_VERTICES*3];
		hkUint16	m_indices[NUM_TRIANGLES+1];

	//
		hkReferencedObject* m_landscape;

		class hkpDefaultWorldMemoryWatchDog* m_watchDog;

};


#endif // HK_LIMITING_MEMORY_2_H


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
