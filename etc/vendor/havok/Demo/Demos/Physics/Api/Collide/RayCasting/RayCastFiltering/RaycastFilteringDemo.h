/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

// This demo illustrates the use of raycast filtering. 
// On one side of the demo, the ray cast hits only objects of the same
// collision layer (indicated by the same color as the ray).
// On the other side, the ray is shot against a hkpMeshShape, 
// where it only hits selected triangles.
// This selection of triangles is achieved by adding material
// information to the mesh, which includes also collision filter
// data.

#ifndef HK_RaycastFilteringDemo_H
#define HK_RaycastFilteringDemo_H

#include <Demos/DemoCommon/DemoFramework/hkDefaultPhysicsDemo.h>
#include <Physics/Internal/Collide/BroadPhase/hkpBroadPhaseCastCollector.h>
#include <Physics/Collide/Query/CastUtil/hkpWorldRayCastInput.h>
#include <Physics/Collide/Shape/Compound/Collection/Mesh/hkpMeshMaterial.h>
class hkpRigidBody;


class RaycastFilteringDemo : public hkDefaultPhysicsDemo
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO);

		RaycastFilteringDemo(hkDemoEnvironment* env);

		~RaycastFilteringDemo() {}

		Result stepDemo(); 
		
	protected:

		hkReal m_time;

		struct MyVector4
		{
			hkReal x,y,z,w;
		};

		//
		//	This is our landscape data (normally provided within the graphics engine
		// 
		enum { NUM_VERTICES = 32 };
		enum { NUM_TRIANGLES = NUM_VERTICES - 2 };

		HK_ALIGN16( MyVector4   m_vertices[NUM_VERTICES] );
		hkUint16	m_indices[NUM_TRIANGLES+2];
		hkUint8		m_materialIndices[NUM_TRIANGLES+1];

		hkpMeshMaterial m_materials[4];

		HK_ALIGN128( hkSemaphoreBusyWait	m_semaphore );
};



#endif	// HK_RaycastFilteringDemo_H

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
