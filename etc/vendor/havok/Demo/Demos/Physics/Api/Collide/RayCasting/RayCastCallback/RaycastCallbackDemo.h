/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

// This demo illustrates the use of the raycast callback mechanism
// to filter ray casts between two shape types. 
//
// When the ray, drawn in GREY, intersects the broadphase AABB of a given shape the display colour of 
// the shape is changed from GREY to YELLOW. The callback then checks to see if the AABB intersected belongs
// to a shape of type HK_SHAPE_SPHERE and if it does an additional shape raycast is performed to see
// if the sphere itself is intersected. If it is, then both the ray and the sphere are displayed in RED
// and the ray is clipped to the intersection point.
// 
// Shape ray casts are not performed on broadphase entries that contain any shape other than sphere.

#ifndef HK_RaycastCallbackDemo_H
#define HK_RaycastCallbackDemo_H

#include <Demos/DemoCommon/DemoFramework/hkDefaultPhysicsDemo.h>
#include <Physics/Internal/Collide/BroadPhase/hkpBroadPhaseCastCollector.h>
#include <Physics/Collide/Query/CastUtil/hkpWorldRayCastInput.h>

class hkpRigidBody;


class RaycastCallbackDemo : public hkDefaultPhysicsDemo
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO);

		RaycastCallbackDemo(hkDemoEnvironment* env);
		~RaycastCallbackDemo();
		Result stepDemo(); 
		
		// Create random spheres and box rigid bodies
		void createBodies();

	private:

		hkReal m_time;
		hkArray<hkpRigidBody*> m_bodies;



	// This class links the output of the broadphase raycaster to the narrowphase raycaster.
	class MyFilteredRayCastCallback : public hkpBroadPhaseCastCollector
	{
		public:
		
			MyFilteredRayCastCallback( const hkpWorldRayCastInput& input, hkpWorldRayCastOutput* output );

		protected:
			// the function which is called every time the broadphase raycaster hits the aabb of an
			// object. This implementation checks the type of object and calls object->raycast if
			// necessary
			virtual	hkReal addBroadPhaseHandle( const hkpBroadPhaseHandle* broadPhaseHandle, int castIndex );

		private:
			// The information about the ray start and end point
			hkpWorldRayCastInput   m_input;

			// A pointer to the result data structure
			hkpWorldRayCastOutput* m_output;
	};

};


#endif	// HK_RaycastCallbackDemo_H

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
