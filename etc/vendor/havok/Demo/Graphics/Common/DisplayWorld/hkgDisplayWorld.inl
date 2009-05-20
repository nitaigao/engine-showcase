/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

inline hkgDisplayWorld* hkgDisplayWorld::defaultCreateInternal()
{
	return new hkgDisplayWorld();
}

inline int hkgDisplayWorld::getNumDisplayObjects() const
{
	return ( m_solidObjects.getSize() + m_alphaObjects.getSize() + m_mixedObjects.getSize() );
}

inline hkgDisplayObject* hkgDisplayWorld::getDisplayObject( int i)
{
	HK_ASSERT(0x125e53ed,   i < getNumDisplayObjects() );
	int ss = m_solidObjects.getSize();
	if (i < ss)
		return m_solidObjects[i];
	else if ( i < (ss + m_alphaObjects.getSize()) )
		return m_alphaObjects[i-ss];
	else
		return m_mixedObjects[i-(ss+m_alphaObjects.getSize())];
}

inline const hkgDisplayObject* hkgDisplayWorld::getDisplayObject( int i) const
{
	return const_cast<hkgDisplayWorld*>(this)->getDisplayObject(i);
}


inline const hkgMaterialManager* hkgDisplayWorld::getMaterialManager() const
{
	return m_materialManager;
}

inline hkgMaterialManager* hkgDisplayWorld::getMaterialManager()
{
	return m_materialManager;
}



inline const hkgLightManager* hkgDisplayWorld::getLightManager() const
{
	return m_lightManager;
}

inline hkgLightManager* hkgDisplayWorld::getLightManager()
{
	return m_lightManager;
}

inline int hkgDisplayWorld::getNumAlphaObjects() const
{
	return m_alphaObjects.getSize();
}

inline int hkgDisplayWorld::getNumSolidObjects() const
{
	return m_solidObjects.getSize();
}

inline int hkgDisplayWorld::getNumMixedObjects() const
{
	return m_mixedObjects.getSize();
}

inline void hkgDisplayWorld::setMutableObjectsUpdatedPerFrame(int i) const
{
	m_iMutableUpdatesPerFrame = i;
}

inline void hkgDisplayWorld::setAlphaDepthSortEnabled(bool on)
{
	if (on && !m_alphaDepthSort)
	{
		hkgVec3Zero( m_lastSortedCamFrom );
		hkgVec3Zero( m_lastSortedCamTo );
	}
	m_alphaDepthSort = on;
}

inline bool hkgDisplayWorld::getAlphaDepthSortEnabled() const
{
	return m_alphaDepthSort;
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
