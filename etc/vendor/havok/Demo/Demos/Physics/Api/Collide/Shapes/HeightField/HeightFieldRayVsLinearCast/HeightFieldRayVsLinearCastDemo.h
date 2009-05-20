/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

// This demo shows the efficiency of the 3dda algorithm
// for casting rays along heightfields while doing raycasts.
// It also shows demonstrates the inaccurate linear sphere cast implementation.

#ifndef HK_HeightFieldRayVsLinearCastDemo_H
#define HK_HeightFieldRayVsLinearCastDemo_H

#include <Demos/DemoCommon/DemoFramework/hkDefaultPhysicsDemo.h>

class hkpRigidBody;
class hkpShape;
class hkpSimpleShapePhantom;


class HeightFieldRayVsLinearCastDemo : public hkDefaultPhysicsDemo
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO);

		HeightFieldRayVsLinearCastDemo(hkDemoEnvironment* env);

		~HeightFieldRayVsLinearCastDemo();

		Result stepDemo(); 
		
	protected:

		// The heightfield shape
		hkpShape* m_shape;

		// Keep track of time and use to move the casts around
		hkReal m_time;

		// The position of the heightfield rigidbody
		hkVector4 m_position;

		// The rotation of the heightfield rigidbody
		hkQuaternion m_rotation;

		// A simple phantom to use for the ray cast
		hkpSimpleShapePhantom* m_simpleShapePhantom;
};



#endif	// HK_HeightFieldRayVsLinearCastDemo_H

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
