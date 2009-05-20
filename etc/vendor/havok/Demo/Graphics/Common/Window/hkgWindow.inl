/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

inline hkgWindow::hkgWindow()
	:
	m_width(0),
	m_height(0),
	m_widescreen(false),
	m_fullscreen(false),
	m_active(true),
	m_tvDeadZoneH(0), m_tvDeadZoneV(0),
	m_platformHandle(HK_NULL), m_platformParentHandle(HK_NULL), 
	m_context(HK_NULL), m_currentViewport(0), 
	m_viewportResizeByMouse( true ), m_viewportBorders(true), m_viewportThickGrayBorders(false),
	m_drawMousePointer(false),
	m_mousePointerTexture( HK_NULL ),
	m_virtualMouseEnabled(false),
	m_handleFlyKeys(false),
	m_drawHavokLogo(true),
	m_havokLogoTexture( HK_NULL ), 
	m_windowOrthoView( HK_NULL),
	m_keyFunc( HK_NULL ),
	m_keyFuncUserContext( HK_NULL ),
	m_mouseMoveFunc( HK_NULL ),
	m_mouseMoveFuncUserContext( HK_NULL ),
	m_mouseWheelFunc( HK_NULL ),
	m_mouseWheelFuncUserContext( HK_NULL ),
	m_mouseButtonFunc( HK_NULL ),
	m_mouseButtonFuncUserContext( HK_NULL ),
	m_padMoveFunc( HK_NULL ),
	m_padMoveFuncUserContext( HK_NULL ),
	m_padButtonFunc( HK_NULL ),
	m_padButtonFuncUserContext( HK_NULL ),
	m_windowResizeFunc( HK_NULL ),
	m_windowResizeFuncUserContext( HK_NULL ),
	m_windowCommandFunc( HK_NULL ),
	m_windowCommandFuncUserContext( HK_NULL )
{
	m_clearColor[0] = 0.3f;
	m_clearColor[1] = 0.3f;
	m_clearColor[2] = 0.4f;
	m_virtualMousePos[0] = 0;
	m_virtualMousePos[1] = 0;
}

inline void* hkgWindow::getPlatformHandle() const
{
	return m_platformHandle;
}


inline bool hkgWindow::isFullscreen() const
{
	return m_fullscreen;
}	

inline bool hkgWindow::isWidescreen() const
{
	return m_widescreen;
}	

inline unsigned int hkgWindow::getWidth() const
{
	return m_width;
}

inline unsigned int hkgWindow::getHeight() const
{
	return m_height;
}

inline bool hkgWindow::hasWidescreenAspect() const
{
	//16/9 == 1.7777..   4/3 = 1.3333..
	float aspect = getWidth() / (float)getHeight();
	return (aspect > 1.7f);
}

inline int hkgWindow::addViewport(hkgViewport* v)
{
	HK_ASSERT(0x7153b240,  v != HK_NULL );
	m_viewports.pushBack( v );
	v->reference();
	return m_viewports.getSize() - 1 ;
}

inline int hkgWindow::getNumViewports() const
{
	return m_viewports.getSize();
}

inline hkgViewport* hkgWindow::getViewport(int i) const
{
	HK_ASSERT(0x1d879a2f,  i < m_viewports.getSize() );
	return m_viewports[i];
}

inline hkgViewport* hkgWindow::getCurrentViewport() const
{
	return m_viewports[m_currentViewport];
}

inline int hkgWindow::getCurrentViewportIndex() const
{
	return m_currentViewport;
}

inline void hkgWindow::setCurrentViewport(int i)
{
	HK_ASSERT(0x375b50ef,  i < m_viewports.getSize() );
	if (m_currentViewport != i)
	{
		m_viewports[i]->acceptMouseFocus();
		m_currentViewport = i;
	}
}

inline int hkgWindow::getViewportID(hkgViewport* v) const
{
	int i = 0;
	while ( (i< m_viewports.getSize()) && (m_viewports[i] != v) )
		++i;

	if (i != m_viewports.getSize())
		return i;

	return -1;
}

inline hkgViewport* hkgWindow::removeViewport(int i)
{
	HK_ASSERT(0x647bf8f0,  i < m_viewports.getSize() );
	hkgViewport* v = m_viewports[i];
	m_viewports.removeAtAndCopy(i);

	if (m_currentViewport > i)
		--m_currentViewport;
	else if (m_currentViewport == i)
		m_currentViewport = 0;

	return v;
}

inline int hkgWindow::getTVDeadZoneH() const
{
	return m_tvDeadZoneH;
}

inline int hkgWindow::getTVDeadZoneV() const
{
	return m_tvDeadZoneV;
}

inline hkgDisplayContext* hkgWindow::getContext() const
{
	return m_context;
}

inline const hkgViewport* hkgWindow::getWindowOrthoView() const
{
	return m_windowOrthoView;
}

inline hkgViewport* hkgWindow::getWindowOrthoView()
{
	return m_windowOrthoView;
}

inline const hkgMouse& hkgWindow::getMouse() const
{
	return m_mouse;
}

inline const hkgPad& hkgWindow::getGamePad(int i) const
{
	HK_ASSERT(0x5af001e9,  i < 2 );
	return m_pad[i];
}

inline const hkgKeyboard& hkgWindow::getKeyboard() const
{
	return m_keyboard;
}

inline void hkgWindow::stepInput()
{
	m_keyboard.cycleState();
	m_mouse.cycleState();
	m_pad[0].cycleState();
	m_pad[1].cycleState();
}

inline void hkgWindow::setKeyFunction( HKG_USER_FUNC_KEY fn, void* userContext )
{
	m_keyFunc = fn;
	m_keyFuncUserContext = userContext;
}

inline void hkgWindow::setMouseMoveFunction( HKG_USER_FUNC_MOUSE_MOVE fn, void* userContext )
{
	m_mouseMoveFunc = fn;
	m_mouseMoveFuncUserContext = userContext;
}

inline void hkgWindow::setMouseWheelFunction( HKG_USER_FUNC_MOUSE_WHEEL fn, void* userContext )
{
	m_mouseWheelFunc = fn;
	m_mouseWheelFuncUserContext = userContext;
}

inline void hkgWindow::setMouseButtonFunction( HKG_USER_FUNC_MOUSE_BUTTON fn, void* userContext )
{
	m_mouseButtonFunc = fn;
	m_mouseButtonFuncUserContext = userContext;
}

inline void hkgWindow::setPadMoveFunction( HKG_USER_FUNC_PAD_MOVE fn, void* userContext )
{
	m_padMoveFunc = fn;
	m_padMoveFuncUserContext = userContext;
}

inline void hkgWindow::setPadButtonFunction( HKG_USER_FUNC_PAD_BUTTON fn, void* userContext )
{
	m_padButtonFunc = fn;
	m_padButtonFuncUserContext = userContext;
}

inline void hkgWindow::setWindowResizeFunction( HKG_USER_FUNC_RESIZE fn, void* userContext )
{
	m_windowResizeFunc = fn;
	m_windowResizeFuncUserContext = userContext;
}

inline void hkgWindow::setWindowCommandFunction( HKG_USER_FUNC_COMMAND fn, void* userContext )
{
	m_windowCommandFunc = fn;
	m_windowCommandFuncUserContext = userContext;
}

inline void hkgWindow::setWantViewportBorders(bool on)
{
	m_viewportBorders = on;
}

inline void hkgWindow::setWantViewportThickGrayBorders(bool on)
{
	m_viewportThickGrayBorders = on;
}

inline void hkgWindow::setWantViewportResizeByMouse(bool on)
{
	m_viewportResizeByMouse = on;
}


inline void hkgWindow::setClearColor( const float c[3] )
{
	hkgVec3Copy( m_clearColor, c );
}

inline void hkgWindow::getClearColor( float c[3]) const
{
	hkgVec3Copy( c, m_clearColor);
}

inline float* hkgWindow::getClearColorPtr()
{
	return m_clearColor;
}

inline void hkgWindow::setWantDrawMousePointer(bool on)
{
	m_drawMousePointer = on;
}

inline bool hkgWindow::getWantDrawMousePointer() const
{
	return m_drawMousePointer;
}

inline void hkgWindow::setWantDrawHavokLogo(bool on)
{
	m_drawHavokLogo = on;
}

inline bool hkgWindow::getWantDrawHavokLogo() const
{
	return m_drawHavokLogo;
}


inline void hkgWindow::setWantVirtualMouse(bool on)
{
	m_virtualMouseEnabled = on;
}

inline bool hkgWindow::getWantVirtualMouse() const
{
	return m_virtualMouseEnabled;
}

inline void hkgWindow::setVirtualMousePos(int x, int y)
{
	m_virtualMousePos[0] = x;
	m_virtualMousePos[1] = y;
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
