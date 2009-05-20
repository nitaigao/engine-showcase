/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_GRAPHICS_WINDOW_DX9_H
#define HK_GRAPHICS_WINDOW_DX9_H

#include <Graphics/Common/Window/hkgWindow.h>
class hkgPostEffectDX9;

#include <d3dx9.h>

typedef int HKG_WINDOW_DX9_OPERATING_SYSTEM;
#define HKG_WINDOW_DX9_OS_UNKNOWN				  0x0
#define HKG_WINDOW_DX9_OS_CONSOLE				  0x001

#define HKG_WINDOW_DX9_OS_WINDOWS_9598ME          0x100

#define HKG_WINDOW_DX9_OS_WINDOWS_VISTA           0x600 // NT 6.0
#define HKG_WINDOW_DX9_OS_WINDOWS_SERVER_LONGHORN 0x601 // NT 6.0 Server
#define HKG_WINDOW_DX9_OS_WINDOWS_SERVER_2003_R2  0x522 // NT 5.2 Server2
#define HKG_WINDOW_DX9_OS_WINDOWS_XP_64           0x520 // NT 5.2
#define HKG_WINDOW_DX9_OS_WINDOWS_SERVER_2003     0x521 // NT 5.2 Server
#define HKG_WINDOW_DX9_OS_WINDOWS_XP              0x510 // NT 5.1
#define HKG_WINDOW_DX9_OS_WINDOWS_2000            0x500 // NT 5.0
#define HKG_WINDOW_DX9_OS_WINDOWS_NT              0x400 // <= NT 4.0

#define HKG_DIRECTX_SUPPORT_ANISOTROPY 1


// D3DX function poointers (As may not be present in the correct form)
typedef LPCSTR (WINAPI *_D3DXGetVertexShaderProfileFUNC)( LPDIRECT3DDEVICE9 pDevice );
typedef LPCSTR (WINAPI *_D3DXGetPixelShaderProfileFUNC)( LPDIRECT3DDEVICE9 pDevice );

typedef HRESULT (WINAPI *_D3DXCompileShaderFUNC )(
				LPCSTR                          pSrcData,
				UINT                            SrcDataLen,
				CONST D3DXMACRO*                pDefines,
				LPD3DXINCLUDE                   pInclude,
				LPCSTR                          pFunctionName,
				LPCSTR                          pProfile,
				DWORD                           Flags,
				LPD3DXBUFFER*                   ppShader,
				LPD3DXBUFFER*                   ppErrorMsgs,
				LPD3DXCONSTANTTABLE*            ppConstantTable);

typedef HRESULT (WINAPI *_D3DXCompileShaderFromFileAFUNC )( 
				LPCSTR pSrcFile,
				CONST D3DXMACRO*                pDefines,
				LPD3DXINCLUDE                   pInclude,
				LPCSTR                          pFunctionName,
				LPCSTR                          pProfile,
				DWORD                           Flags,
				LPD3DXBUFFER*                   ppShader,
				LPD3DXBUFFER*                   ppErrorMsgs,
				LPD3DXCONSTANTTABLE*            ppConstantTable);


typedef HRESULT (WINAPI *_D3DXCreateEffectFUNC )(
				LPDIRECT3DDEVICE9               pDevice,
				LPCVOID                         pSrcData,
				UINT                            SrcDataLen,
				CONST D3DXMACRO*                pDefines,
				LPD3DXINCLUDE                   pInclude,
				DWORD                           Flags,
				LPD3DXEFFECTPOOL                pPool,
				LPD3DXEFFECT*                   ppEffect,
				LPD3DXBUFFER*                   ppCompilationErrors);

typedef HRESULT (WINAPI *_D3DXCreateCubeTextureFromFileInMemoryFUNC )(
				LPDIRECT3DDEVICE9         pDevice,
				LPCVOID                   pSrcData,
				UINT                      SrcDataSize,
				LPDIRECT3DCUBETEXTURE9*   ppCubeTexture);

typedef HRESULT (WINAPI * _D3DXCreateTextureFromFileInMemoryExFUNC)(
				LPDIRECT3DDEVICE9 pDevice,
				LPCVOID pSrcData,
				UINT SrcDataSize,
				UINT Width,
				UINT Height,
				UINT MipLevels,
				DWORD Usage,
				D3DFORMAT Format,
				D3DPOOL Pool,
				DWORD Filter,
				DWORD MipFilter,
				D3DCOLOR ColorKey,
				D3DXIMAGE_INFO * pSrcInfo,
				PALETTEENTRY * pPalette,
				LPDIRECT3DTEXTURE9 * ppTexture);

typedef HRESULT (WINAPI * _D3DXCreateVolumeTextureFromFileInMemoryExFUNC)(
				LPDIRECT3DDEVICE9 pDevice,
				LPCVOID pSrcData,
				UINT SrcDataSize,
				UINT Width,
				UINT Height,
				UINT Depth,
				UINT MipLevels,
				DWORD Usage,
				D3DFORMAT Format,
				D3DPOOL Pool,
				DWORD Filter,
				DWORD MipFilter,
				D3DCOLOR ColorKey,
				D3DXIMAGE_INFO * pSrcInfo,
				PALETTEENTRY * pPalette,
				LPDIRECT3DVOLUMETEXTURE9 * ppVolumeTexture);

typedef HRESULT (WINAPI * _D3DXLoadSurfaceFromSurfaceFUNC)(
				LPDIRECT3DSURFACE9 pDestSurface,
				CONST PALETTEENTRY * pDestPalette,
				CONST RECT * pDestRect,
				LPDIRECT3DSURFACE9 pSrcSurface,
				CONST PALETTEENTRY * pSrcPalette,
				CONST RECT * pSrcRect,
				DWORD Filter,
				D3DCOLOR ColorKey);

extern _D3DXCompileShaderFUNC			_D3DXCompileShader;
extern _D3DXCompileShaderFromFileAFUNC	_D3DXCompileShaderFromFile;
extern _D3DXCreateEffectFUNC			_D3DXCreateEffect;
extern _D3DXCreateCubeTextureFromFileInMemoryFUNC	_D3DXCreateCubeTextureFromFileInMemory;
extern _D3DXCreateTextureFromFileInMemoryExFUNC		_D3DXCreateTextureFromFileInMemoryEx; 
extern _D3DXCreateVolumeTextureFromFileInMemoryExFUNC		_D3DXCreateVolumeTextureFromFileInMemoryEx; 
extern _D3DXGetPixelShaderProfileFUNC  _D3DXGetPixelShaderProfile;
extern _D3DXGetVertexShaderProfileFUNC _D3DXGetVertexShaderProfile;
extern _D3DXLoadSurfaceFromSurfaceFUNC _D3DXLoadSurfaceFromSurface;

class hkgWindowDX9ResetEventHandler
{
public:

	virtual void releaseD3D9() = 0;
	virtual void resetD3D9() = 0;
};

class hkgWindowDX9 : public hkgWindow
{
public:

	inline const LPDIRECT3DDEVICE9 getDevice() const;
	inline LPDIRECT3DDEVICE9 getDevice();

	inline const D3DCAPS9& getCaps() const;
	inline DWORD getCreateFlags() const; 

	virtual HKG_WINDOW_DX9_OPERATING_SYSTEM getOperatingSystem() const { return HKG_WINDOW_DX9_OS_UNKNOWN; }

	virtual hkUint32 vertexShaderMinorVersion() const { return (m_d3dCaps.VertexShaderVersion    ) & 0xFF; }
	virtual hkUint32 vertexShaderMajorVersion() const { return ((m_d3dCaps.VertexShaderVersion)>>8) & 0xFF; }		
	virtual hkUint32 pixelShaderMinorVersion() const { return ( m_d3dCaps.PixelShaderVersion     ) & 0xFF; }
	virtual hkUint32 pixelShaderMajorVersion() const { return ((m_d3dCaps.PixelShaderVersion )>>8) & 0xFF; }	

	virtual void buildCommonShaderDefines( hkArray<const char*>& defines );
	virtual bool supportsShaderCompilation() const { return _D3DXCompileShaderFromFile != HK_NULL; }

	// some DX9 specific window commands (for shadowing etc)
	virtual HKG_SHADOWMAP_SUPPORT getShadowMapSupport();
	virtual void prepareForShadowMap(); // creates depth and render target
	virtual void beginShadowMapRender(class hkgDisplayWorld* world, class hkgCamera* camera, class hkgLight* light, bool clearMap = true ); // sets the depth and render target to be the current
	virtual int endShadowMapRender(int textureStageForMap, bool shaderDriven); // resets to the normal depth and render target
	virtual void revertShadowMapState(); // resets to the normal texture modes etc
	virtual void cleanupShadowMap(); // releases held targets
	virtual void setShadowMapSize(int size); 
	virtual int getShadowMapSize() const;
	virtual void enableDebugShadowMapView(bool on) const { m_debugShadowMaps = on; } 

	inline LPDIRECT3DTEXTURE9 getShadowMap(); // an F32 texture
	virtual const float* getComputedShadowMatrix(bool includeCameraViewMatrix) const; // get the perspective shadow matrix
	virtual const float* getComputedShadowTextureMatrix(bool includeCameraViewMatrix) const; // get the perspective shadow matrix with the texture lookup and bias baked in

	virtual HKG_MESHBLENDING_SUPPORT getMeshBlendingSupport();
	virtual int getMaxBlendMatrices();
	virtual int getMaxTextureBlendStages();

	virtual bool saveFrontFrameBufferToBMP(const char* filename);
	virtual bool saveFrontFrameBufferToStream(unsigned char * str, int stridebytes, int pixelsize);

	// debug utils
	void displayShadowMap();

	void cleanupPostEffects();
	void addPostEffect(hkgPostEffectDX9* e);
	void removePostEffect(hkgPostEffectDX9* e);
	void removePostEffect(const char* name);
	
	virtual void applyPostEffects();
	virtual bool applyPreClearEffectCommands();

	const D3DPRESENT_PARAMETERS& getPresentParams() const { return m_d3dpp;  }

	void addD3DEventHandler( hkgWindowDX9ResetEventHandler* handler );
	void removeD3DEventHandler( hkgWindowDX9ResetEventHandler* handler );

protected:

	inline hkgWindowDX9();
	virtual ~hkgWindowDX9();
	
	LPDIRECT3DDEVICE9		m_pd3dDevice;
	LPDIRECT3D9				m_pD3D;              
	D3DPRESENT_PARAMETERS	m_d3dpp;   
	D3DCAPS9				m_d3dCaps;           // Caps for the device
    DWORD					m_dwCreateFlags;     // Indicate sw or hw vertex processing

	bool					m_fsaa; 

	// Post process effect (FX file etc) support, ordered.
	hkgArray<hkgPostEffectDX9*>	m_postEffects;

	// shadow map support
	int						m_shadowMapSize;
	HKG_SHADOWMAP_SUPPORT	m_shadowMapSupport;
	bool					m_shadowNoClearPass;

	LPDIRECT3DTEXTURE9      m_pShadowColorMap; 
	LPDIRECT3DTEXTURE9      m_pShadowZMap; 
	LPDIRECT3DSURFACE9      m_pShadowColorSurface;  
	LPDIRECT3DSURFACE9      m_pShadowZSurface;  

	LPDIRECT3DSURFACE9		m_pOrigRT;
	LPDIRECT3DSURFACE9		m_pOrigDS;
	
	float					m_origDepthBias;
	float					m_origDepthBiasSlope;

	HKG_ENABLED_STATE		m_nonShadowState;
	// to help cull and create the optimal shadow map projection matrices
	int						m_currentShadowTextureStage;
	mutable float			m_shadowTexMatrix[16];
	mutable bool			m_debugShadowMaps;
	class hkgPixelShaderDX9* m_debugShadowMapPShader;
	class hkgVertexShaderDX9* m_debugShadowMapVShader;

	hkArray< hkgWindowDX9ResetEventHandler* > m_eventHandlers;

	unsigned char* m_framebuffer; ///
	int m_framebufferSize;

};

#include <Graphics/Dx9/Shared/Window/hkgWindowDX9.inl>

#endif //HK_GRAPHICS_WINDOW_DX9_H
	

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
