/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

inline hkgCamera* hkgViewport::getCamera() const
{
	return m_camera;
}

inline hkgSkyBox* hkgViewport::getSkyBox() const
{
	return m_skyBox;
}

inline HKG_ENABLED_STATE hkgViewport::getDesiredState() const
{
	return m_desiredState;
}

inline void hkgViewport::setDesiredState( HKG_ENABLED_STATE s)
{
	m_desiredState = s;
}

inline HKG_CULLFACE_MODE hkgViewport::getDesiredCullFaceMode() const
{
	return m_desiredCullFaceMode;
}

inline void hkgViewport::setDesiredCullFaceMode( HKG_CULLFACE_MODE m)
{
	m_desiredCullFaceMode = m;
}

inline HKG_BLEND_MODE hkgViewport::getDesiredBlendMode() const
{
	return m_desiredBlendMode;
}

inline void hkgViewport::setDesiredBlendMode( HKG_BLEND_MODE  m)
{
	m_desiredBlendMode = m;
}

inline void hkgViewport::setExtentsAbsolute( int ll_x, int ll_y, int ur_x, int ur_y )
{
	m_winLL[0] = ll_x;
	m_winLL[1] = ll_y;
	m_winUR[0] = ur_x;
	m_winUR[1] = ur_y;
	m_absolute = true;
	updateExtents();
}

inline void hkgViewport::setExtentsRelative( float ll_x, float ll_y,
							 float ur_x, float ur_y )
{
	m_winLLRelative[0] = ll_x;
	m_winLLRelative[1] = ll_y;
	m_winURRelative[0] = ur_x;
	m_winURRelative[1] = ur_y;
	m_absolute = false;
	updateExtents();
}

inline HKG_CAMERA_NAVIGATION_MODE hkgViewport::getNavigationMode() const
{
	return m_navMode;
}

inline void hkgViewport::setNavigationMode( HKG_CAMERA_NAVIGATION_MODE m )
{
	m_navMode = m;
}

inline void hkgViewport::setFlyModeUp( const float* d )
{
	hkgVec3Copy(m_flyModeUp, d);
}

inline float* hkgViewport::getFlyModeUpPtr( )
{
	return m_flyModeUp;
}

inline const float* hkgViewport::getFlyModeUpPtr( ) const
{
	return m_flyModeUp;
}

inline void hkgViewport::setInvertLook( bool invert )
{
	m_flyInvertLook = invert;
}

inline bool hkgViewport::getInvertLook() const
{
	return m_flyInvertLook;
}

inline bool hkgViewport::getAcceptsMouseEvents() const
{
	return m_wantMouseEvents;
}

inline void hkgViewport::setAcceptsMouseEvents(bool a)
{
	m_wantMouseEvents = a;
}

inline bool hkgViewport::getAcceptsPadEvents() const
{
	return m_wantPadEvents;
}

inline void hkgViewport::setAcceptsPadEvents(bool a)
{
	m_wantPadEvents = a;
}

inline void hkgViewport::getLowerLeftCoord(int &x, int &y) const
{
	x = m_winLL[0];
	y = m_winLL[1];
}

inline void hkgViewport::getUpperRightCoord( int &x,  int &y) const
{
	x = m_winUR[0];
	y = m_winUR[1];
}

inline void hkgViewport::toggleState(HKG_ENABLED_STATE s)
{
	if (m_desiredState & s) // already enabled
		m_desiredState &= static_cast<unsigned short>(~s);
	else // disabled
		m_desiredState |= s;
}

inline int hkgViewport::getWidth() const
{
	return m_winUR[0] - m_winLL[0];
}

inline int hkgViewport::getHeight() const
{
	return m_winUR[1] - m_winLL[1];
}


inline bool hkgViewport::containsWindowCoord( int wx, int wy) const
{
	return  (wx >= (int)m_winLL[0]) && (wy >= (int)m_winLL[1]) && 
			(wx <= (int)m_winUR[0]) && (wy <= (int)m_winUR[1]);
}

inline HKG_VIEWPORT_SIDE hkgViewport::isWindowCoordOnBorder( int wx, int wy, unsigned int width) const
{
	HKG_VIEWPORT_SIDE ret = HKG_VIEWPORT_SIDE_NONE;

	if (( hkg_abs( wx - (int)m_winLL[0] ) < (int)width ) && (wy > ((int)m_winLL[1] - (int)width)) && ( wy < ((int)m_winUR[1] + (int)width)) )
		ret |= HKG_VIEWPORT_SIDE_LEFT;
	if (( hkg_abs( wy - (int)m_winLL[1] ) < (int)width ) && (wx > ((int)m_winLL[0] - (int)width)) && ( wx < ((int)m_winUR[0] + (int)width)) )
		ret |= HKG_VIEWPORT_SIDE_BOTTOM;

	if (( hkg_abs( wx - (int)m_winUR[0] ) < (int)width ) && (wy > ((int)m_winLL[1] - (int)width)) && ( wy < ((int)m_winUR[1] + (int)width)) )
		ret |= HKG_VIEWPORT_SIDE_RIGHT;
	if (( hkg_abs( wy - (int)m_winUR[1] ) < (int)width ) && (wx > ((int)m_winLL[0] - (int)width)) && ( wx < ((int)m_winUR[0] + (int)width)) )
		ret |= HKG_VIEWPORT_SIDE_TOP;

	return ret;
}


inline const hkgWindow* hkgViewport::getOwnerWindow() const
{
	return m_owner;
}

inline hkgWindow* hkgViewport::getOwnerWindow()
{
	return m_owner;
}

inline void hkgViewport::setMouseSpeed(int pan, int wheelZoom, int buttonZoom, bool absoluteSpeed)
{
	m_speedMousePan = pan;
	m_speedMouseWheelZoom = wheelZoom;
	m_speedMouseButtonZoom = buttonZoom;
	m_mouseAbsoluteSpeed = absoluteSpeed;
}

inline void hkgViewport::getMouseSpeed(int& pan, int& wheelZoom, int& buttonZoom, bool& absoluteSpeed) const
{
	pan = m_speedMousePan;
	wheelZoom = m_speedMouseWheelZoom;
	buttonZoom = m_speedMouseButtonZoom;
	absoluteSpeed = m_mouseAbsoluteSpeed;
}

inline void hkgViewport::setFlySpeed(int strafe, int move)
{
	m_speedFlyStrafe = strafe;
	m_speedFlyMove = move;
}

inline void hkgViewport::getFlySpeed(int& strafe, int& move) const
{
	strafe = m_speedFlyStrafe;
	move = m_speedFlyMove;
}


inline void hkgViewport::setPadSpeed(int pan, int zoom, bool absoluteSpeed)
{
	m_speedPadPan = pan;
	m_speedPadZoom = zoom;
	m_padAbsoluteSpeed = absoluteSpeed;
}

inline void hkgViewport::getPadSpeed(int& pan, int& zoom, bool& absoluteSpeed) const
{
	pan = m_speedPadPan;
	zoom = m_speedPadZoom;
	absoluteSpeed = m_padAbsoluteSpeed;
}
		
inline void hkgViewport::setMouseConvention (HKG_MOUSE_CONVENTION convention)
{
	m_mouseConvention = convention;
}

inline HKG_MOUSE_CONVENTION hkgViewport::getMouseConvention() const
{
	return m_mouseConvention;
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
