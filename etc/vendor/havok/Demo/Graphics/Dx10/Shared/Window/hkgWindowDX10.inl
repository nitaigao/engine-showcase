/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

hkgWindowDX10::hkgWindowDX10()
: hkgWindow(), m_device(HK_NULL), m_swapChain(HK_NULL), m_renderTargetView(HK_NULL), m_depthStencil(HK_NULL), m_depthStencilView(HK_NULL)
{

}

inline const ID3D10Device* hkgWindowDX10::getDevice() const
{
	return m_device;
}

inline ID3D10Device* hkgWindowDX10::getDevice()
{
	return m_device;
}

inline const IDXGISwapChain* hkgWindowDX10::getSwapChain() const
{
	return m_swapChain;
}

inline IDXGISwapChain* hkgWindowDX10::getSwapChain()
{
	return m_swapChain;
}

inline const ID3D10RenderTargetView* hkgWindowDX10::getRenderTargetView() const
{
	return m_renderTargetView;
}

inline ID3D10RenderTargetView* hkgWindowDX10::getRenderTargetView()
{
	return m_renderTargetView;
}

inline const ID3D10Texture2D* hkgWindowDX10::getDepthStencil() const
{
	return m_depthStencil;
}

inline ID3D10Texture2D* hkgWindowDX10::getDepthStencil()
{
	return m_depthStencil;
}

inline const ID3D10DepthStencilView* hkgWindowDX10::getDepthStencilView() const
{
	return m_depthStencilView;
}

inline ID3D10DepthStencilView* hkgWindowDX10::getDepthStencilView()
{
	return m_depthStencilView;
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
