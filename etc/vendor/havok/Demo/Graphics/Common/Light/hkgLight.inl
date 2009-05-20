/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


hkgLight::hkgLight()
: m_lightNum(-1), m_desiredEnableState(true),
  m_spotInnerAngle(0.0f), m_spotOuterAngle(0.0f), m_spotExponent(0.0f),
  m_type(HKG_LIGHT_OMNI)
{
	hkgVec3Zero(m_pos);
	hkgVec3Zero(m_direction);
	hkgVec3Zero(m_diffuse);
	hkgVec3Zero(m_specular);
	hkgVec3Zero(m_ambient);
}

inline bool hkgLight::getIsEnabled() const
{
	return m_lightNum >= 0;
}

inline HKG_LIGHT_TYPE hkgLight::getType() const
{
	return m_type;
}

inline void hkgLight::setType(HKG_LIGHT_TYPE t)
{
	m_type = t;
}

inline void hkgLight::getPosition(float p[3]) const
{
	hkgVec3Copy( p, m_pos );
}

inline float* hkgLight::getPositionPtr()
{
	return m_pos;
}

inline void hkgLight::setPosition(const float p[3])
{
	hkgVec3Copy( m_pos, p );
}

inline void hkgLight::getDirection(float d[3]) const
{
	hkgVec3Copy(d, m_direction);
}

inline float* hkgLight::getDirectionPtr()
{
	return m_direction;
}

inline void hkgLight::setDirection(const float p[3])
{
	hkgVec3Copy( m_direction, p );
	hkgVec3Normalize(m_direction);
}

inline void hkgLight::getDiffuse(float d[3]) const
{
	hkgVec3Copy( d, m_diffuse );
}

inline float* hkgLight::getDiffusePtr()
{
	return m_diffuse;
}

inline void hkgLight::setDiffuse(const float p[3])
{
	hkgVec3Copy( m_diffuse, p);
}

inline void hkgLight::getSpecular(float s[3]) const
{
	hkgVec3Copy( s, m_specular);
}

inline float* hkgLight::getSpecularPtr()
{
	return m_specular;
}

inline void hkgLight::setSpecular(const float p[3])
{
	hkgVec3Copy(m_specular, p);
}

inline void hkgLight::getAmbient(float s[3]) const
{
	hkgVec3Copy(s, m_ambient);
}

inline float* hkgLight::getAmbientPtr()
{
	return m_ambient;
}

inline void hkgLight::setAmbient(const float p[3])
{
	hkgVec3Copy( m_ambient, p );
}

inline float hkgLight::getSpotInnerAngle() const
{
	return m_spotInnerAngle;
}

inline float hkgLight::getSpotOuterAngle() const
{
	return m_spotOuterAngle;
}

inline float hkgLight::getSpotFalloffExponent() const
{
	return m_spotExponent;
}

inline void hkgLight::setSpotInnerAngle(float a)
{
	m_spotInnerAngle = a;
}

inline void hkgLight::setSpotOuterAngle(float a)
{
	m_spotOuterAngle = a;
}

inline void hkgLight::setSpotFalloffExponent(float a)
{
	m_spotExponent = a;
}

inline void hkgLight::setLightNumber(int l)
{
	m_lightNum = l;
}

inline int hkgLight::getLightNumber() const
{
	return m_lightNum;
}

inline bool hkgLight::getDesiredEnabledState() const
{
	return m_desiredEnableState;
}

inline void hkgLight::setDesiredEnabledState(bool on)
{
	m_desiredEnableState = on;
}

inline void hkgLight::addShadowPlane( float plane[4] )
{
	ShadowPlane& sp = m_planes.expandOne();
	hkgVec4Copy(sp.m_plane, plane);
	float posDir[4];
	if (m_type==HKG_LIGHT_DIRECTIONAL)
	{
		hkgVec3Mult(posDir, m_direction, -1.0f);
		posDir[HKG_W] = 0.0f;
	}
	else
	{
		hkgVec3Copy(posDir, m_pos);
		posDir[HKG_W] = 1.0f;
	}
	sp.compute(posDir);
}

inline const float* hkgLight::getShadowMatrix(int i) const 
{
	return m_planes[i].m_shadowMatrix;
}

inline int hkgLight::getNumShadowPlanes() const
{
	return m_planes.getSize();
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
