/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
inline hkgMaterialFaceSet::hkgMaterialFaceSet( )
: m_material( HK_NULL )
{
}

inline hkgMaterialFaceSet* hkgMaterialFaceSet::defaultCreateInternal()
{
	return new hkgMaterialFaceSet();
}

inline void hkgMaterialFaceSet::addFaceSet(hkgFaceSet* faceSet)
{
	faceSet->reference();
	m_faceSets.pushBack(faceSet);
}

inline int hkgMaterialFaceSet::getNumFaceSets() const
{
	return m_faceSets.getSize();
}

inline const hkgFaceSet* hkgMaterialFaceSet::getFaceSet(int i) const
{
	return m_faceSets[i];
}

inline hkgFaceSet* hkgMaterialFaceSet::getFaceSet(int i)
{
	return m_faceSets[i];
}

inline const hkgMaterial* hkgMaterialFaceSet::getMaterial() const
{
	return m_material;
}

inline hkgMaterial* hkgMaterialFaceSet::getMaterial()
{
	return m_material;
}

inline void hkgMaterialFaceSet::setMaterial( hkgMaterial* m )
{
	if (m)
	{
		m->reference();
	}
	if (m_material)
	{
		m_material->release();
	}
	m_material = m;
}


inline void hkgMaterialFaceSet::reserve( int estimateNum )
{
	if (estimateNum > m_faceSets.getCapacity())
		m_faceSets.reserve(estimateNum);
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
