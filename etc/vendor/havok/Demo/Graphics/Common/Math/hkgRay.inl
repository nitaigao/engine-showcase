/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

inline hkgRay::hkgRay()
{
	hkgVec3Zero( m_from );
	m_dir[0] = 0.0f;
	m_dir[1] = 0.0f;
	m_dir[2] = 1.0f;
}


inline hkgRay::hkgRay(const float from[3], const float dir[3])
{
	hkgVec3Copy( m_dir, dir );
	hkgVec3Copy( m_from, from );
}

inline float* hkgRay::getFromPtr()
{
	return m_from;
}

inline const float* hkgRay::getFromPtr() const
{
	return m_from;
}

inline void hkgRay::getFrom(float f[3]) const
{
	hkgVec3Copy( f, m_from );
}

inline float* hkgRay::getDirPtr()
{
	return m_dir;
}

inline const float* hkgRay::getDirPtr() const
{
	return m_dir;
}

inline void hkgRay::getDir(float d[3]) const
{
	hkgVec3Copy( d, m_dir );
}

inline void hkgRay::transform( float t[16] )
{
	hkgVec3Rotate( m_dir, t );
	float temp[3];
	hkgVec3Copy( temp, m_from );
	hkgVec3Transform( m_from, temp, t );
}

inline void hkgRay::getPosAtDist( float pos[3], float dist ) const
{
	hkgVec3Mult( pos, m_dir, dist  );
	hkgVec3Add( pos, m_from ); 
}

inline bool hkgRay::hitsSphere(const float* pos, float radius) const
{
	float dst[3]; hkgVec3Sub(dst, m_from, pos );
	const float B = hkgVec3Dot(dst, m_dir );
	const float C = hkgVec3Dot(dst, dst) - (radius * radius);
	const float D = B*B - C;
	return D > 0; /* -B - sqrt(D) */
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
