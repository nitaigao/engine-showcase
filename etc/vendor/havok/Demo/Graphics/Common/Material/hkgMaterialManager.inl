/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


inline bool hkgMaterialManager::materialIsTextured(int i) const
{
	HK_ASSERT(0x3fa40ec4,  i < m_materials.getSize() );
	return ( m_materials[i]->getTexture(0) != HK_NULL );
}

inline hkgMaterial* hkgMaterialManager::getMaterial(int i)
{
	HK_ASSERT(0x30112203,  i < m_materials.getSize() );
	return m_materials[i];
}

inline const hkgMaterial* hkgMaterialManager::getMaterial(int i) const
{
	HK_ASSERT(0x6f480eb0,  i < m_materials.getSize() );
	return m_materials[i];
}

inline const hkgMaterial* hkgMaterialManager::getDefaultMaterial(int i) const
{
	HK_ASSERT(0x2e1b1c89,  i < m_defMaterials.getSize() );
	return m_defMaterials[i];
}

inline hkgMaterial* hkgMaterialManager::getDefaultMaterial(int i)
{
	HK_ASSERT(0x7fc7e1ee,  i < m_defMaterials.getSize() );
	return m_defMaterials[i];
}

inline void hkgMaterialManager::setDefaultMaterial(hkgMaterial* mat, int i)
{
	HK_ASSERT(0x7fc7e1ee,  i < m_defMaterials.getSize() );
	mat->reference();
	m_defMaterials[i]->release();
	m_defMaterials[i] = mat;
}

inline void hkgMaterialManager::clearMaterials()
{
	// release all materials
	int nm = m_materials.getSize();
	for (int i =0; i < nm; ++i)
		m_materials[i]->release();

	m_materials.clear();
}

inline void hkgMaterialManager::clearDefaultMaterials()
{
	// release all default materials
	int nm = m_defMaterials.getSize();
	for (int i =0; i < nm; ++i)
		m_defMaterials[i]->release();

	m_defMaterials.clear();
}

inline int hkgMaterialManager::addMaterial(hkgMaterial* m)
{
	m_materials.pushBack(m);
	m->reference();
	return ( m_materials.getSize() - 1 );
}

inline int hkgMaterialManager::getNumMaterials() const
{
	return m_materials.getSize();
}

inline int hkgMaterialManager::getNumDefaultMaterials() const
{
	return m_defMaterials.getSize();
}

inline hkgMaterialManager* hkgMaterialManager::defaultCreateInternal(hkgDisplayContext* context)
{
	return new hkgMaterialManager(context);
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
