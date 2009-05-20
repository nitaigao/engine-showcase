/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

// A demo which shows creation of a MoppBVTree shape with Box primitives (and use in a hkpRigidBody).
// The BVTree interface adds bounding volume tree information to a collection of shapes (hkpShapeCollection)
// for fast overlap/ray queries.
// The MoppBvTreeShape implements this by using a MOPP (a compressed BV-Tree) to provide these queries.

#ifndef HK_SerializeMoppDemo_H
#define HK_SerializeMoppDemo_H

#include <Demos/DemoCommon/DemoFramework/hkDefaultPhysicsDemo.h>
#include <Physics/Collide/Shape/Compound/Collection/ExtendedMeshShape/hkpExtendedMeshShape.h>


class SerializeMoppDemo : public hkDefaultPhysicsDemo
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO);

		SerializeMoppDemo(hkDemoEnvironment* env);
		~SerializeMoppDemo();

		void setupMeshPart( hkpExtendedMeshShape::TrianglesSubpart& part );

		Result stepDemo();

		hkpRigidBody* m_body;
		hkArray<char> m_buffer;

		enum { NUM_VERTS = 288, NUM_TRIS=576 };

        static const hkReal meshVertices[NUM_VERTS*3];
		static const int meshIndices[NUM_TRIS*3];
};

#endif // HK_SerializeMoppDemo_H

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
