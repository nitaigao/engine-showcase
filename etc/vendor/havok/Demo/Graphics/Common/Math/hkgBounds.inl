/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
inline hkgAabb::hkgAabb() 
{
	hkgVec3Set(m_min,  1e33f, 1e33f, 1e33f);
	hkgVec3Set(m_max, -1e33f,-1e33f,-1e33f);
}

inline hkgAabb::hkgAabb(const float pmin[3], const float pmax[3])
{
	hkgVec3Copy( m_min, pmin );
	hkgVec3Copy( m_max, pmax );
}

inline void hkgAabb::addPoint( const float p[3] )
{
	m_min[HKG_X] = HKG_MIN(m_min[HKG_X], p[HKG_X]);
	m_min[HKG_Y] = HKG_MIN(m_min[HKG_Y], p[HKG_Y]);
	m_min[HKG_Z] = HKG_MIN(m_min[HKG_Z], p[HKG_Z]);

	m_max[HKG_X] = HKG_MAX(m_max[HKG_X], p[HKG_X]);
	m_max[HKG_Y] = HKG_MAX(m_max[HKG_Y], p[HKG_Y]);
	m_max[HKG_Z] = HKG_MAX(m_max[HKG_Z], p[HKG_Z]);
}

inline void hkgAabb::getPoint(int i, float p[3]) const 
{
	hkgVec3Set(p, (i&1) ? m_min[HKG_X] : m_max[HKG_X], 
		(i&2) ? m_min[HKG_Y] : m_max[HKG_Y], 
		(i&4) ? m_min[HKG_Z] : m_max[HKG_Z] );  
}

inline void hkgAabb::getCentroid(float c[3]) const
{
	hkgVec3Add(c, m_min, m_max);
	hkgVec3Scale(c, 0.5f);
}

inline void hkgAabb::transform( const float t[16] )
{
	const float currentExtents[8][3] = {
		{ m_min[HKG_X], m_min[HKG_Y], m_min[HKG_Z] },
		{ m_min[HKG_X], m_min[HKG_Y], m_max[HKG_Z] },
		{ m_min[HKG_X], m_max[HKG_Y], m_min[HKG_Z] },
		{ m_min[HKG_X], m_max[HKG_Y], m_max[HKG_Z] },
		{ m_max[HKG_X], m_min[HKG_Y], m_min[HKG_Z] },
		{ m_max[HKG_X], m_min[HKG_Y], m_max[HKG_Z] },
		{ m_max[HKG_X], m_max[HKG_Y], m_min[HKG_Z] },
		{ m_max[HKG_X], m_max[HKG_Y], m_max[HKG_Z] } };

	hkgAabb newAabb;
	float tp[3];
	for (int i=0; i<8; i++)
	{
		hkgVec3Transform(tp, currentExtents[i], t);
		newAabb.addPoint(tp);
	}

	*this = newAabb;
}

inline hkgAabb::hkgAabb(const float* points, int numPoints)
{
	for (int i=0; i < numPoints; ++i)
		addPoint( &(points[i*3]) );
}

inline hkgSphere::hkgSphere()
{
	hkgVec3Zero( m_center);
	m_radius = 0;
}	

inline hkgSphere::hkgSphere( const hkgAabb& aabb )
{
	float ext[3];
	hkgVec3Add( m_center, aabb.m_min, aabb.m_max);
	hkgVec3Scale( m_center, 0.5f );
	hkgVec3Sub( ext, aabb.m_max, m_center);
	m_radius = hkgVec3Length(ext);
}

inline hkgViewCone::hkgViewCone()
: m_fovy(0),
  m_fovx(0),
  m_fNear(0.001f),
  m_fFar(1.f)
{
	hkgVec3Copy(m_dir, HKG_VEC3_Z);
	hkgVec3Zero(m_from);
	hkgMat4Identity(m_mLookAt);
}

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
