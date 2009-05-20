/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_GRAPHICS_PICK_DATA
#define HK_GRAPHICS_PICK_DATA

#include <Graphics/Common/hkgObject.h>

// forward decl.
class hkgDisplayWorld;

/// hkgFaceSetPickData, returned by hkgFaceSet::intersect().
struct hkgFaceSetPickData
{
		HKG_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( hkgFaceSetPickData );

		/// The distance along ray of triangle hit point. 
		/// Use  (dir*m_dist + from)  for world point
		float m_dist; 

		/// The index of the primitive in the hkgFaceSet in which triangle presides that was hit.
		int m_primitiveIndex; 
		
		/// The index in the primitive of the triangle (i-th triangle, not i-th connectivity value)
		int m_primitiveTriIndex; 

		/// The 3 indices that make up the triangle (so that you don't have to walk the above information
		/// if you don't want to). Index into the vertex set with these values to get the vertex positions.
		int m_verts[3]; 
};


/// hkgGeometryPickData, returned by hkgGeometry::intersect().
struct hkgGeometryPickData
{
		HKG_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( hkgGeometryPickData );

		/// The hkgFaceSetPickData	that came from the successfull hkgFaceSet::intersect call.
		hkgFaceSetPickData	m_facePickData;

		/// The index of the material face set that held the hkgFaceSet that returned the closest hit.
		int		m_matFaceSetIndex;		
	
		/// The index in the material facet set of the actual hkgFaceSet, used in conjunction with the above index.
		int		m_matFaceSetSubIndex;	
};


/// hkgDisplayObjectPickData, returned by hkgDisplayObject::intersect().
struct hkgDisplayObjectPickData
{
		HKG_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( hkgDisplayObjectPickData );

		/// The hkgGeometryPickData  that came from the successful hkgGeometry::intersect call.
		hkgGeometryPickData	m_geomPickData;	
		
		float	m_localPos[3];

		/// The index of the geometry that returned the closest hit.
		int		m_geomIndex;
};

// hkgViewportPickData, returned from hkgViewport::pick().
struct hkgViewportPickData
{
		HKG_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( hkgViewportPickData );

		/// The hkgDisplayObjectPickData  that came from the hkgDisplayObject::intersect() for the 
		/// hkgDisplayObject that was closest hit by the ray in the hkgDisplayWorld given to the pick()
		/// method. The display world is stored below for reference too.
		hkgDisplayObjectPickData	m_objectPickData;	

		/// The index into the hkgDisplayWorld of the successful hit object.
		int							m_objectIndex;	

		/// The world position of the hit object. This is hit point from the hkgFaceSet transformed
		/// by the hkgDisplayObject.m_transform if one.
		float						m_worldPos[3];

		/// The pointer to the world that was used to query for intersection. Stored here for
		/// convenience.
		hkgDisplayWorld*			m_world;

		/// The total number of triangles that where successfully hit during the test for the closest hit with the ray.
		int							m_trianglesHit; 

		/// As the distance along the vect to the hit is the preserved through the display obj transform
		/// the distance to the hot along the ray is just the fact set dist metric.
		inline float getHitDistance() { return m_objectPickData.m_geomPickData.m_facePickData.m_dist; }
};

#endif // HK_GRAPHICS_PICK_DATA

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
