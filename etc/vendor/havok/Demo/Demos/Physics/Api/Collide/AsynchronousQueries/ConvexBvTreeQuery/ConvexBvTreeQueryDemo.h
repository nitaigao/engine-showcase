/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

// This demo shows a bounding volume tree (hkpMoppBvTreeShape) being 
// created around a convex geometry. This allows OBB or sphere queries 
// against the convex geometry. For comparison a brute force test is also 
// performed which iterates through each triangle and tests whether it 
// intersects with the sphere being queried. 
//
// The bv-tree approach
// does not test against the actual geometry, but rather against a bounding 
// structure which envelopes each triangle.
// Since the intersection tests are not made directly against the triangle
// surface there is a minute inaccuracy when compared with the brute force
// method. However, the tightness for the triangle bounding
// volumes can be set by the user (see hkpMoppCompilerInput). 
// The timings show that for the simplest convex shape the bv-tree query 
// is quicker.
//
// This is particularly useful when you get a contact point from a raycast
// or collision against a convex vertices shape. Since this shape does 
// not store the triangles, it cannot return triangle indices. This query
// will determine which triangles (of your original display geometry if 
// you wish) intersect with a small sphere around the contact point.
// 
// Each test is done on the same shape. Results of the brute force test
// are shown in blue for 50 frames, then results of the BV tree test are 
// shown in green for 50 frames. The shape is then rotated and the 
// tests re-run.

#ifndef HK_ConvexBvTreeQueryDemo_H
#define HK_ConvexBvTreeQueryDemo_H

#include <Demos/DemoCommon/DemoFramework/hkDefaultPhysicsDemo.h>

class hkPseudoRandomGenerator;


class ConvexBvTreeQueryDemo : public hkDefaultPhysicsDemo
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO);

		// Construction
		ConvexBvTreeQueryDemo(hkDemoEnvironment* env);

		// Destruction
		~ConvexBvTreeQueryDemo();

		// Do queries
		Result stepDemo();

		// Utility function to create a triangle shape from vertices and indices
		void buildTriangle( unsigned int tri, hkpTriangleShape& shape );

		void displayTriangle( unsigned int tri, const hkTransform& tr, unsigned int c ) const;



private:

		hkpMoppBvTreeShape* m_moppShape;

		hkpRigidBody* m_rigidBody;

		hkReal m_radiusOfQuerySphere;

		int m_frameCount;

		hkPseudoRandomGenerator* m_generator;
};

#endif // HK_ConvexBvTreeQueryDemo_H

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
