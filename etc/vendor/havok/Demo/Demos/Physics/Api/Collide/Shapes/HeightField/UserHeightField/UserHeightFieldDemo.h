/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

/// This demo shows how to implement your own procedural heightfield shape.
/// Top keep things simple, our implementation is a simple hollow
/// sphere filled with boxes

#ifndef HK_UserHeightFieldDemo_H
#define HK_UserHeightFieldDemo_H

#include <Demos/DemoCommon/DemoFramework/hkDefaultPhysicsDemo.h>
#include <Common/Visualize/Shape/hkDisplaySphere.h>
#include <Physics/Collide/Shape/HeightField/hkpHeightFieldShape.h>

	// This is our user heightField
class HollowSphereShape: public hkpHeightFieldShape
{
	public:
		HollowSphereShape( hkReal radius );

	protected:
		// This is the main method that we must implement. It will take in a list of 
		// spheres and report back the distance to the surface for each of the spheres.
		void collideSpheres( const CollideSpheresInput& input, SphereCollisionOutput* outputArray) const;

		// The the AABB around this sphere
 		void getAabbImpl( const hkTransform& localToWorld, hkReal tolerance, hkAabb& out  ) const;

		hkpShapeType getType() const;

		// The rest of the virtual methods we will not implement
		// for this demo as they are not used, and they are the 
		// cast methods used by the ray and linear cast algorithms.

		// We do not care about implementing this function in this demo
		hkBool castRayImpl( const hkpShapeRayCastInput& input, hkpShapeRayCastOutput& results) const;

		// We do not care about implementing this function in this demo
		void castRayWithCollectorImpl( const hkpShapeRayCastInput& input, const hkpCdBody& cdBody, hkpRayHitCollector& collector ) const;

		// We do not care about implementing this function in this demo
		void castSphere( const hkpSphereCastInput& input, const hkpCdBody& cdBody, hkpRayHitCollector& collector ) const;

		// Finally we need some local object storage for our heightfield
		// which is just the radius of our hollow sphere. The center is
		// assumed to be the local space origin.
		hkReal m_radius;
};


class UserHeightFieldDemo : public hkDefaultPhysicsDemo
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO);

		UserHeightFieldDemo(hkDemoEnvironment* env);

		~UserHeightFieldDemo();

		Result stepDemo(); 

			// A display representation for the heightfield
		hkDisplaySphere* m_sphereDisp;

			// Our heightfield
		hkpRigidBody*	m_rigidBody;


};

#endif	// HK_UserHeightFieldDemo_H

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
