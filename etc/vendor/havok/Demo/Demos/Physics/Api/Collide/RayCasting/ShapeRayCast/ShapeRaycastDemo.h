/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

// A demo which shows use of the hkpShape raycast, which determines the closest point of intersection 
// between a ray (finite, given start and end points) and a hkpShape, if it exists.
// In addition, the normal at the point of intersection is calculated.
// In this demo the ray is swept over time to illustrate the correctness of the raycast.

#ifndef HK_ShapeRaycastDemo_H
#define HK_ShapeRaycastDemo_H

#include <Demos/DemoCommon/DemoFramework/hkDefaultPhysicsDemo.h>
#include <Common/Visualize/Shape/hkDisplayPlane.h>


class hkpRigidBody;
class hkpMoppBvTreeShape;
class hkpHeightFieldShape;


class ShapeRaycastDemo : public hkDefaultPhysicsDemo
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO);

		ShapeRaycastDemo(hkDemoEnvironment* env);

		~ShapeRaycastDemo();

		Result stepDemo(); 

		// Helper methods
		static void HK_CALL createBodies(hkArray<hkpRigidBody*> & bodiesOut, const hkpProcessCollisionInput* collisionInput = HK_NULL);

		static hkpMoppBvTreeShape* createMoppShape();

		// Toggle shapes during bootstrapping
		void makeFakeInput();

	private:

		hkArray<hkpRigidBody*>	m_bodies;

		// A display representation for the plane shape
		hkDisplayPlane*	m_planeDisp;



		hkReal m_time;
		enum CastMode
		{
			CAST_RAY_SINGLE = 0,
			CAST_RAY_COLLECTOR = 1
		};
		hkEnum<CastMode,hkInt8> m_castMode;

		hkBool m_simulationPaused;
};

#endif	// HK_ShapeRaycastDemo_H

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
