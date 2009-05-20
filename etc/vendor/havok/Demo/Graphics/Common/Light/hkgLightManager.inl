/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
inline hkgLightManager* hkgLightManager::defaultCreateInternal()
{
	return new hkgLightManager();
}


inline int hkgLightManager::getNumLights() const
{
	return m_lights.getSize();
}

inline hkgLight* hkgLightManager::getLight(int i)
{
	HK_ASSERT(0x7666a5ad,  i < m_lights.getSize() );
	return m_lights[i];
}

inline const hkgLight* hkgLightManager::getLight(int i) const
{
	HK_ASSERT(0x4e0f374e,  i < m_lights.getSize() );
	return m_lights[i];
}

inline const hkgLight* hkgLightManager::getActiveSetLight(int i) const
{
	HK_ASSERT(0x2e8021a6,  i < m_activeSet.getSize() );
	return m_activeSet[i];
}

inline hkgLight* hkgLightManager::getActiveSetLight(int i)
{
	HK_ASSERT(0x10abaa3d,  i < m_activeSet.getSize() );
	return m_activeSet[i];
}

inline void hkgLightManager::setMaxActiveSetSize(int asl)
{
	int maxlen = hkgDisplayContext::getMaxNumLights();
	int rasl = HKG_MIN(asl, maxlen);
	m_maxActiveSetSize = rasl;
}


inline int hkgLightManager::addLight(hkgLight* l)
{
	m_lights.pushBack( l );
	l->reference();
	return m_lights.getSize() -1;
}


inline int hkgLightManager::findLight(hkgLight* l) const
{
	int nl = m_lights.getSize();
	for (int i =0; i < nl; ++i)
		if (m_lights[i] == l)
			return i;

	return -1;
}

inline hkgLight* hkgLightManager::removeLight(int i)
{
	HK_ASSERT(0x70c9c531,  i < m_lights.getSize() );
	hkgLight* l = m_lights[i];
	m_lights.removeAt(i);

	// it may be in the active set too:

	int nl = m_activeSet.getSize();
	for (int j =0; j < nl; ++j)
	{
		if (m_activeSet[j] == l)
		{
			m_activeSet.removeAt(j);
			break;
		}
	}

	return l;
}

inline int hkgLightManager::getActiveSetSize() const
{
	return m_activeSet.getSize();
}

inline void hkgLightManager::addSceneAmbient( const float a[3] )
{
	hkgVec3Add( m_sceneAmbient, m_sceneAmbient, a );
	hkgVec3Clamp( m_sceneAmbient, 0.0f, 1.0f );
}

inline void hkgLightManager::setSceneAmbient( const float a[3] )
{
	hkgVec3Copy( m_sceneAmbient, a );
}

inline void hkgLightManager::getSceneAmbient( float a[3] ) const
{
	hkgVec3Copy( a, m_sceneAmbient );
}

inline float* hkgLightManager::getSceneAmbientPtr()
{
	return m_sceneAmbient;
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
