/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

inline void hkgSceneDataConverter::setAllowMeshSharing( hkBool on )
{
	m_bAllowMeshSharing = on;
}

inline void hkgSceneDataConverter::setAllowMaterialSharing( hkBool on )
{
	m_bAllowMaterialSharing = on;
}

inline void hkgSceneDataConverter::setAllowTextureSharing( hkBool on )
{
	m_bAllowTextureSharing = on;
}

inline void hkgSceneDataConverter::setAllowExtraTextures( hkBool on )
{
	m_materialCache.m_supportsShaders = on;
}

inline void hkgSceneDataConverter::setAllowTextureMipmap( hkBool on )
{
	m_bAllowTextureMipMaps = on;
}

inline void hkgSceneDataConverter::setAllowTextureCompression( hkBool on )
{
	m_bAllowTextureCompression = on;
}

inline void hkgSceneDataConverter::setAllowTextureAnisotropicFilter( hkBool on )
{
	m_bAllowTextureAnisotropicFilter = on;
}

inline void hkgSceneDataConverter::setMeshMaterialCreation( hkBool on )
{
	m_bMeshMaterialCreation = on;
}

inline void hkgSceneDataConverter::setAllowHardwareSkinning( hkBool on )
{
	m_bAllowHardwareSkinning = on;
}

inline void hkgSceneDataConverter::setShaderLibraryEnabled( hkBool on )
{
	m_bEnableShaderLibLookup = on;
}

inline void hkgSceneDataConverter::revertToDefaultSettings()
{
	m_bAllowHardwareSkinning = false;
	m_bAllowMaterialSharing = true;
	m_bAllowTextureSharing = true;
	m_bAllowTextureMipMaps = true;
	m_materialCache.m_supportsShaders = true;
	m_materialCache.m_searchPaths.clear();
	m_bAllowTextureCompression = false;
	m_bInvertNormalMapGreenChannel = false;
	m_bAllowMeshSharing = true;
	m_bMeshMaterialCreation = true;
	m_bEnableShaderLibLookup = false;
}

inline void hkgSceneDataConverter::addTextureSearchPath(const char* path)
{
	m_materialCache.m_searchPaths.pushBack(path);
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
