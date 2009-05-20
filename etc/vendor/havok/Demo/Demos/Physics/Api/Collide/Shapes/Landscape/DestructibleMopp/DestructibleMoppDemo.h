/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

/// This demo illustrates construction/destruction of a mopp shape (for triangle mesh landscapes)
/// Also demonstrates collision between boxes and the landscape

#ifndef HK_DestructibleMoppDemo_H
#define HK_DestructibleMoppDemo_H

class hkpMoppBvTreeShape;

#include <Demos/DemoCommon/DemoFramework/hkDefaultPhysicsDemo.h>
#include <Physics/Collide/Shape/Compound/Collection/Mesh/hkpMeshMaterial.h>
#include <Physics/Dynamics/Collide/hkpCollisionListener.h>

class hkpExtendedMeshShape;
class hkpMoppCode;
class hkpMoppBvTreeShape;
class DestroyAllMopps;


class DestructibleMoppDemo: public hkDefaultPhysicsDemo
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO);

		DestructibleMoppDemo(hkDemoEnvironment* env);

		~DestructibleMoppDemo();
		
	private:

		hkpMoppBvTreeShape* createMoppShape();

		//
		//	This is our landscape data (normally provided within the graphics engine)
		// 
		enum { NUM_VERTICES = 20 };
		enum { NUM_TRIANGLES = NUM_VERTICES - 2 };

		float		m_vertices[NUM_VERTICES*3];
		hkUint16	m_indices[NUM_TRIANGLES+2];
		hkUchar		m_materialIndizes[NUM_TRIANGLES];
		hkpMeshMaterial	m_materials[2];
};

#endif // HK_DestructibleMoppDemo_H


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
