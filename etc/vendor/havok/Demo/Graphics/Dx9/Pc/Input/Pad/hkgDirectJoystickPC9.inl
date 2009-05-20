/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

inline bool hkgDirectJoystickPC9::isValid()
{
	return (m_pJoystick != NULL);
}

inline float hkgDirectJoystickPC9::getXAxis() const
{
	return (float)m_curJoystickState.lX / 1000.0f;
}

inline float hkgDirectJoystickPC9::getYAxis() const
{
	return (float)m_curJoystickState.lY / 1000.0f;
}

inline float hkgDirectJoystickPC9::getZAxis() const
{
	if ( m_caps & HKG_DIRECT_JOYSTICK_ZAXIS )
	{
		return (float)m_curJoystickState.lZ / 1000.0f;
	}
	else if ( m_caps & HKG_DIRECT_JOYSTICK_SLIDER0 )
	{
		return (float)m_curJoystickState.rglSlider[0] / 1000.0f;
	}
	else
	{
		return 0.0f;
	}
}

inline float hkgDirectJoystickPC9::getXRotation() const
{
	return (float)m_curJoystickState.lRx / 1000.0f;
}

inline float hkgDirectJoystickPC9::getYRotation() const
{
	return (float)m_curJoystickState.lRy / 1000.0f;
}

inline float hkgDirectJoystickPC9::getZRotation() const
{
	return (float)m_curJoystickState.lRz / 1000.0f;
}

inline float hkgDirectJoystickPC9::getSlider(unsigned int s) const
{
	if ( s > 1)
		return 0.0f;
	
	return (float)m_curJoystickState.rglSlider[s] / 1000.0f;
}

inline float hkgDirectJoystickPC9::getPov(unsigned int s) const
{
	if ( s > 3)
		return 0.0f;
	
	DWORD dwPOV = m_curJoystickState.rgdwPOV[s];
	BOOL centered = (LOWORD(dwPOV) == 0xFFFF);

	if (centered || ((int)dwPOV) < 0)
		return -1.0f;

	return ( (float)dwPOV ) / 100.0f; // returns degrees from north (away from user), clockwize
}

inline bool hkgDirectJoystickPC9::getButton(unsigned int b) const
{
	if ( b > 127 ) // 128 buttons
		return 0.0f;

	return (m_curJoystickState.rgbButtons[b] & 0x80) != 0;
}




inline float hkgDirectJoystickPC9::getPrevXAxis() const
{
	return (float)m_prevJoystickState.lX / 1000.0f;
}

inline float hkgDirectJoystickPC9::getPrevYAxis() const
{
	return (float)m_prevJoystickState.lY / 1000.0f;
}

inline float hkgDirectJoystickPC9::getPrevZAxis() const
{
	if ( m_caps & HKG_DIRECT_JOYSTICK_ZAXIS )
	{
		return (float)m_prevJoystickState.lZ / 1000.0f;
	}
	else if ( m_caps & HKG_DIRECT_JOYSTICK_SLIDER0 )
	{
		return (float)m_prevJoystickState.rglSlider[0] / 1000.0f;
	}
	else
	{
		return 0.0f;
	}
}

inline float hkgDirectJoystickPC9::getPrevXRotation() const
{
	return (float)m_prevJoystickState.lRx / 1000.0f;
}

inline float hkgDirectJoystickPC9::getPrevYRotation() const
{
	return (float)m_prevJoystickState.lRy / 1000.0f;
}

inline float hkgDirectJoystickPC9::getPrevZRotation() const
{
	return (float)m_prevJoystickState.lRz / 1000.0f;
}

inline float hkgDirectJoystickPC9::getPrevSlider(unsigned int s) const
{
	if ( s > 1)
		return 0.0f;
	
	return (float)m_prevJoystickState.rglSlider[s] / 1000.0f;
}

inline float hkgDirectJoystickPC9::getPrevPov(unsigned int s) const
{
	if ( s > 3)
		return 0.0f;
	
	DWORD dwPOV = m_prevJoystickState.rgdwPOV[s];
	BOOL centered = (LOWORD(dwPOV) == 0xFFFF);

	if (centered || ((int)dwPOV) < 0)
		return -1.0f;

	return ( (float)dwPOV ) / 100.0f; // returns degrees from north (away from user), clockwize
}

inline bool hkgDirectJoystickPC9::getPrevButton(unsigned int b) const
{
	if ( b > 127 ) // 128 buttons
		return 0.0f;

	return (m_prevJoystickState.rgbButtons[b] & 0x80) != 0;
}



inline unsigned int hkgDirectJoystickPC9::getNumSupportedButtons() const
{
	return m_numButtons;
}

inline HKG_DIRECT_JOYSTICK_CAPS hkgDirectJoystickPC9::getCaps() const
{
	return m_caps;
}

inline void hkgDirectJoystickPC9::addCaps( HKG_DIRECT_JOYSTICK_CAPS caps )
{
	m_caps |= caps;
}

inline HKG_DIRECT_JOYSTICK_TYPE hkgDirectJoystickPC9::getType() const
{
	return m_type;
}

inline LPDIRECTINPUTDEVICE8& hkgDirectJoystickPC9::getDevice()
{
	return m_pJoystick;
}

inline bool hkgDirectJoystickPC9::hasPS2Caps() const
{
	const HKG_DIRECT_JOYSTICK_CAPS ps2Caps = 
				HKG_DIRECT_JOYSTICK_XAXIS |
				HKG_DIRECT_JOYSTICK_YAXIS	| 
				HKG_DIRECT_JOYSTICK_ZAXIS	|
				HKG_DIRECT_JOYSTICK_ZROT	|
				HKG_DIRECT_JOYSTICK_POV0;

	// this is a variant of the ps2 controller that uses slider0 instead of Zaxis

	const HKG_DIRECT_JOYSTICK_CAPS ps2Caps2 =
				HKG_DIRECT_JOYSTICK_XAXIS |
				HKG_DIRECT_JOYSTICK_YAXIS	| 
				HKG_DIRECT_JOYSTICK_SLIDER0	|
				HKG_DIRECT_JOYSTICK_ZROT	|
				HKG_DIRECT_JOYSTICK_POV0;

	return (((ps2Caps & m_caps) == ps2Caps) || ((ps2Caps2 & m_caps) == ps2Caps2)) && (getNumSupportedButtons() >= 12);
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
