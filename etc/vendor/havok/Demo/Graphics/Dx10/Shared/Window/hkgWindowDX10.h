/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_GRAPHICS_WINDOW_DX10_H
#define HK_GRAPHICS_WINDOW_DX10_H

#include <Graphics/Common/Window/hkgWindow.h>

// we make all explicit DLL calls as function ptrs so 
// that we cvan dynamically load the dll (if present)
// and handle when it is missing and also simplifiy
// the includes required for the exe.

typedef HRESULT (WINAPI *_D3D10CreateDeviceAndSwapChainFUNC)(
    IDXGIAdapter *pAdapter,
    D3D10_DRIVER_TYPE DriverType,
    HMODULE Software,
    UINT Flags,
    UINT SDKVersion,
    DXGI_SWAP_CHAIN_DESC *pSwapChainDesc,
    IDXGISwapChain **ppSwapChain,    
	ID3D10Device **ppDevice);

typedef HRESULT (WINAPI *_D3D10CompileShaderFUNC)(	LPCSTR pSrcData, SIZE_T SrcDataLen, LPCSTR pFileName, CONST D3D10_SHADER_MACRO* pDefines, LPD3D10INCLUDE pInclude, 
													LPCSTR pFunctionName, LPCSTR pProfile, UINT Flags, ID3D10Blob** ppShader, ID3D10Blob** ppErrorMsgs);

typedef HRESULT (WINAPI *_D3D10GetInputSignatureBlobFUNC)(void *pShaderBytecode, SIZE_T BytecodeLength, ID3D10Blob **ppSignatureBlob);

typedef HRESULT (WINAPI *_D3D10ReflectShaderFUNC)(void *pShaderBytecode, SIZE_T BytecodeLength, ID3D10ShaderReflection **ppReflector);

extern _D3D10CreateDeviceAndSwapChainFUNC _D3D10CreateDeviceAndSwapChain;
extern _D3D10CompileShaderFUNC _D3D10CompileShader;
extern _D3D10GetInputSignatureBlobFUNC _D3D10GetInputSignatureBlob;
extern _D3D10ReflectShaderFUNC _D3D10ReflectShader;


class hkgWindowDX10 : public hkgWindow
{
public:

	inline const ID3D10Device* getDevice() const;
	inline ID3D10Device* getDevice();
	inline const IDXGISwapChain* getSwapChain() const;
	inline IDXGISwapChain* getSwapChain();
	inline const ID3D10RenderTargetView* getRenderTargetView() const;
	inline ID3D10RenderTargetView* getRenderTargetView();
	inline const ID3D10Texture2D* getDepthStencil() const;
	inline ID3D10Texture2D* getDepthStencil();
	inline const ID3D10DepthStencilView* getDepthStencilView() const;
	inline ID3D10DepthStencilView* getDepthStencilView();

	inline hkUint32 vertexShaderMinorVersion() const { return 0; } //XX
	inline hkUint32 vertexShaderMajorVersion() const { return 4; }		
	inline hkUint32 pixelShaderMinorVersion () const { return 0; }
	inline hkUint32 pixelShaderMajorVersion () const { return 4; }		

	virtual HKG_MESHBLENDING_SUPPORT getMeshBlendingSupport() { return HKG_MESHBLENDING_NOSUPPORT; }//return HKG_MESHBLENDING_HARDWARE; 
	virtual int getMaxBlendMatrices() { return 0; } //20
	virtual int getMaxTextureBlendStages() { return 2; } // usually 8 ort so, but our texture based shaders only use one texture so far

	virtual bool supportsShaderCompilation() const { return true; } // in core dlls 

	// debug utils
	void displayShadowMap();

protected:

	inline hkgWindowDX10();
	virtual ~hkgWindowDX10() {}

	ID3D10Device*           m_device;
	IDXGISwapChain*         m_swapChain;
	DXGI_SWAP_CHAIN_DESC	m_swapChainDesc;
	ID3D10RenderTargetView* m_renderTargetView;
	ID3D10Texture2D*		m_depthStencil;
	ID3D10DepthStencilView*	m_depthStencilView;

	// shadow map support
	HKG_SHADOWMAP_SUPPORT   m_shadowMapSupport;

};

#include <Graphics/Dx10/Shared/Window/hkgWindowDX10.inl>

#endif //HK_GRAPHICS_WINDOW_DX10_H
	

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
