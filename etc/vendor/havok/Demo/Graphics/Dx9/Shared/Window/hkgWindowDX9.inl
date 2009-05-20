/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

hkgWindowDX9::hkgWindowDX9()
: hkgWindow(), m_pd3dDevice(HK_NULL), m_pD3D(NULL), m_dwCreateFlags(0), m_fsaa(false), m_shadowMapSize(0), m_shadowMapSupport(HKG_SHADOWMAP_NOTKNOWN),
m_pShadowColorMap(HK_NULL), m_pShadowZMap(HK_NULL), m_pShadowColorSurface(HK_NULL), m_pShadowZSurface(HK_NULL),
m_pOrigRT(HK_NULL), m_pOrigDS(HK_NULL), m_origDepthBias(0), m_origDepthBiasSlope(0), m_debugShadowMaps(false),
m_debugShadowMapVShader(HK_NULL),m_debugShadowMapPShader(HK_NULL),
m_framebuffer(HK_NULL), m_framebufferSize(0)
{

}

inline const LPDIRECT3DDEVICE9 hkgWindowDX9::getDevice() const
{
	return m_pd3dDevice;
}

inline LPDIRECT3DDEVICE9 hkgWindowDX9::getDevice()
{
	return m_pd3dDevice;
}

inline const D3DCAPS9& hkgWindowDX9::getCaps() const
{ 
	return m_d3dCaps; 
}

inline DWORD hkgWindowDX9::getCreateFlags() const
{
	return m_dwCreateFlags;
}

inline LPDIRECT3DTEXTURE9 hkgWindowDX9::getShadowMap()
{
	return m_pShadowZMap;
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
