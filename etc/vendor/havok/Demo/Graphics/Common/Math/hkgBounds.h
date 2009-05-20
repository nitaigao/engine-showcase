/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK_GRAPHICS_BOUNDS_H
#define HK_GRAPHICS_BOUNDS_H

// An axis aligned bounding box
class hkgAabb
{
public:
	HKG_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( hkgAabb );

	inline hkgAabb();

	inline hkgAabb(const float pmin[3], const float pmax[3]);

		/// Each point is assumed to be a float[3], no stride.
	inline hkgAabb(const float* points, int numPoints); 

	inline void addPoint( const float p[3] );
	inline void transform( const float t[16] );
	inline void getPoint(int i, float p[3]) const ;
	inline void getCentroid(float c[3]) const;

	bool rayIntersect(const float start[3], const float dir[3], float& t ) const;
	
	float m_min[3];
	float m_max[3];
};

// A bounding sphere
class hkgSphere
{
public:
	HKG_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( hkgSphere );

	inline hkgSphere();
	explicit inline hkgSphere( const hkgAabb& aabb );
	hkgSphere( const float* points, int numPoints );

	float	m_center[3];
	float   m_radius;
};

// A cone that is commonly used in bounding a scene for use in determining camera or shadow projection
struct hkgViewCone
{
public:
	HKG_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( hkgViewCone );

	inline hkgViewCone();

		// given from and dir, just compute fov bound
	void compute( const hkgArray<hkgAabb>& aabbs, const float projMatrix[16], const float from[3], const float dir[3] );

		// given from, compute a tight enough direction and bound.
	void compute(const hkgArray<hkgAabb>& boxes, const float projection[16], const float from[3]);

	float m_mLookAt[16];
	float m_dir[3];
	float m_from[3];
	
	float m_fovy; // in radians, half angle
	float m_fovx; // in radians, half angle
	float m_fNear; // distance along dir to closest 
	float m_fFar;// distance along dir to furthest
};

// A six sided view frustum
class hkgFrustum
{
public:
	HKG_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( hkgFrustum );

	explicit hkgFrustum(const float* viewProj);

	enum TestResult 
	{
		TEST_OUTSIDE = 0,
		TEST_INSIDE = 1,
		TEST_INTERSECTS = 2
	};

	TestResult testAabb( const hkgAabb& aabb ) const;
	TestResult testSphere( const hkgSphere& sphere ) const;
	TestResult testSweptSphere( const hkgSphere& sphere, const float sweepDir[3]) const;

	// the 6 planes in world space that mark the view frustum 
	// 4 sides (l,r,b,t) and near (n) and far(f). Stored in that order.
	float	m_planes[6][4]; 
	int     m_aabbMinMaxTable[6]; 
};

#include <Graphics/Common/Math/hkgBounds.inl>

#endif // HK_GRAPHICS_BOUNDS_H


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
