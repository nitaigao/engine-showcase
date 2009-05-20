/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_GRAPHICS_RAY_H
#define HK_GRAPHICS_RAY_H

#include <Graphics/Common/Math/hkgMath.h>

/// A simple class to store an infinite ray from a give point in a given direction.
class hkgRay
{
	public:

		HKG_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( hkgRay );

			/// Default constructor. Note that a ray is not reference counted.
			/// Will default to be from (0,0,0) along Z axis.
		inline hkgRay();

			/// Constructor. Note that a ray is not reference counted.
			/// The given direction must be normalized before this call
			/// as the data will just be copied directly.
		inline hkgRay(const float from[3], const float dir[3]);

			/// Get the 'from' point as a pointer to the internal structure.
		inline float* getFromPtr();

			/// Get the 'from' point as a const pointer to the internal structure.
		inline const float* getFromPtr() const;

			/// Get the 'from' point, returned in the vector 'f'.
		inline void getFrom(float f[3]) const;

			/// Get the 'dir' direction as a pointer to the internal structure.
		inline float* getDirPtr();

			/// Get the 'dir' direction as a const pointer to the internal structure.
		inline const float* getDirPtr() const;

			/// Get the 'dir' direction, returned in the vector 'd'.
		inline void getDir(float d[3]) const;

			/// This will transform the ray by 't', and it will only
			/// rotate 'dir' and transform 'from'. The 4x4 transform t should
			/// not contain shew etc.
		inline void transform( float t[16] );

			/// Given current ray (from pos in direction dir)
			/// returns (t,u,v) where t is distance to the plane and u,v are 
			/// the coords within the triangle
			/// so the point of intersection will be v0 + u*(v1-v0) + v*(v2-v0)
			///
			/// (from:	Fast,Minimum Storage Ray-Triangle Intersection
			///			Journal of Graphics Tools, 2(1):21-28, 1997 )
		bool triangleIntersect( const float v0[3], const float v1[3], const float v2[3],
								float &t, float &u, float &v) const;

			// Does the ray intersect with any part of the given sphere
		inline bool hitsSphere(const float* pos, float radius) const;

		inline void getPosAtDist( float pos[3], float dist ) const;

	protected:

		float m_from[3]; // in the space that you are tring to cast the ray in.
		float m_dir[3]; // has to be normalized.
};

#include <Graphics/Common/Math/hkgRay.inl>

#endif // HK_GRAPHICS_RAY_H

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
