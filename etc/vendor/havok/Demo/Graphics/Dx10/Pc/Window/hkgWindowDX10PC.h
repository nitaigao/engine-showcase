/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_GRAPHICS_WINDOW_DX10_PC_H
#define HK_GRAPHICS_WINDOW_DX10_PC_H

#include <Graphics/Common/Window/hkgWindow.h>
#include <Graphics/Dx10/Shared/Window/hkgWindowDX10.h>

class hkgDirectJoystickPC9S;

class hkgWindowDX10PC : public hkgWindowDX10
{
public:

	static hkgWindow* createWindowDX10PC();
	
	virtual bool initialize( HKG_WINDOW_CREATE_FLAG flags, HKG_WINDOW_BUFFERS buffers, int w, int h, const char* name, void* windowHandle = HK_NULL, void* parentHandle = HK_NULL ); // returns success
	virtual bool initializeContext();

	virtual	bool setAsCurrent(); // only need to call this if you have > 1 window
	virtual bool clearBuffers();
	virtual bool swapBuffers();

	virtual bool updateSize( unsigned int w, unsigned int h);
	virtual bool updatePos( unsigned int x, unsigned int y);

	virtual void setMousePosition(int x,int y);
	virtual void showMouseCursor(bool visible);

	virtual HKG_WINDOW_MSG_STATUS peekMessages(bool inViewportResize = false); // will loop on windows messages if any. Will return if idle.
	virtual void waitMessages(); // block until there is messages

	inline HINSTANCE getInstanceHandle() const;

	// XX fixme (Add MS Input support)
	virtual bool hasGamePads() const { return false; }

	inline bool getActiveState() const;
	inline void setActiveState(bool a);
	
	// Internal
	void initShaderCollection();

protected:

	hkgWindowDX10PC( HINSTANCE instance, WORD menuID = 0);
	virtual ~hkgWindowDX10PC();

	void createRenderTargets(int w, int h);
	void releaseRenderTargets();

	void					cleanupD3D();
	HRESULT					resizeD3D();
	bool					updateSizeD3D();

	DWORD					m_dwWindowStyle;     // Saved window style for mode switches
    RECT					m_rcWindowBounds;    // Saved window bounds for mode switches
    RECT					m_rcWindowClient;    // Saved client area size for mode switches
	
	BOOL					m_bUseDepthBuffer;   // Whether to autocreate depthbuffer
    DWORD					m_dwMinDepthBits;    // Minimum number of bits needed in depth buffer
    DWORD					m_dwMinStencilBits;  // Minimum number of bits needed in stencil buffer
 


	HRESULT					toggleFullscreen();
	HRESULT					forceWindowed();
	HRESULT					adjustWindowForChange();

//	HRESULT 				displayErrorMsg( HRESULT hr, DWORD dwType );

    HRESULT					RestoreDeviceObjects();                    
    HRESULT					InvalidateDeviceObjects();

	HINSTANCE				m_hinstance;
	bool					m_ownHWND; // did it create the hwnd?					
};

// WndProc prototype
LRESULT CALLBACK hkgWindowProcDX10PC(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

#include <Graphics/Dx10/Pc/Window/hkgWindowDX10PC.inl>

#endif //HK_GRAPHICS_WINDOW_DX10_PC_H

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
