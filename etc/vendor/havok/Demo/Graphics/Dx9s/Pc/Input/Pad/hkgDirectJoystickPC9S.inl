/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

inline bool hkgDirectJoystickPC9S::isValid()
{
	return (m_pDI != NULL) && (m_pJoystick != NULL);
}

inline float hkgDirectJoystickPC9S::getXAxis() const
{
	return (float)m_curJoystickState.lX / 1000.0f;
}

inline float hkgDirectJoystickPC9S::getYAxis() const
{
	return (float)m_curJoystickState.lY / 1000.0f;
}

inline float hkgDirectJoystickPC9S::getZAxis() const
{
	return (float)m_curJoystickState.lZ / 1000.0f;
}

inline float hkgDirectJoystickPC9S::getXRotation() const
{
	return (float)m_curJoystickState.lRx / 1000.0f;
}

inline float hkgDirectJoystickPC9S::getYRotation() const
{
	return (float)m_curJoystickState.lRy / 1000.0f;
}

inline float hkgDirectJoystickPC9S::getZRotation() const
{
	return (float)m_curJoystickState.lRz / 1000.0f;
}

inline float hkgDirectJoystickPC9S::getSlider(unsigned int s) const
{
	if ( s > 1)
		return 0.0f;
	
	return (float)m_curJoystickState.rglSlider[s] / 1000.0f;
}

inline float hkgDirectJoystickPC9S::getPov(unsigned int s) const
{
	if ( s > 3)
		return 0.0f;
	
	DWORD dwPOV = m_curJoystickState.rgdwPOV[s];
	BOOL centered = (LOWORD(dwPOV) == 0xFFFF);

	if (centered || ((int)dwPOV) < 0)
		return -1.0f;

	return ( (float)dwPOV ) / 100.0f; // returns degrees from north (away from user), clockwize
}

inline bool hkgDirectJoystickPC9S::getButton(unsigned int b) const
{
	if ( b > 127 ) // 128 buttons
		return 0.0f;

	return (m_curJoystickState.rgbButtons[b] & 0x80) != 0;
}




inline float hkgDirectJoystickPC9S::getPrevXAxis() const
{
	return (float)m_prevJoystickState.lX / 1000.0f;
}

inline float hkgDirectJoystickPC9S::getPrevYAxis() const
{
	return (float)m_prevJoystickState.lY / 1000.0f;
}

inline float hkgDirectJoystickPC9S::getPrevZAxis() const
{
	return (float)m_prevJoystickState.lZ / 1000.0f;
}

inline float hkgDirectJoystickPC9S::getPrevXRotation() const
{
	return (float)m_prevJoystickState.lRx / 1000.0f;
}

inline float hkgDirectJoystickPC9S::getPrevYRotation() const
{
	return (float)m_prevJoystickState.lRy / 1000.0f;
}

inline float hkgDirectJoystickPC9S::getPrevZRotation() const
{
	return (float)m_prevJoystickState.lRz / 1000.0f;
}

inline float hkgDirectJoystickPC9S::getPrevSlider(unsigned int s) const
{
	if ( s > 1)
		return 0.0f;
	
	return (float)m_prevJoystickState.rglSlider[s] / 1000.0f;
}

inline float hkgDirectJoystickPC9S::getPrevPov(unsigned int s) const
{
	if ( s > 3)
		return 0.0f;
	
	DWORD dwPOV = m_prevJoystickState.rgdwPOV[s];
	BOOL centered = (LOWORD(dwPOV) == 0xFFFF);

	if (centered || ((int)dwPOV) < 0)
		return -1.0f;

	return ( (float)dwPOV ) / 100.0f; // returns degrees from north (away from user), clockwize
}

inline bool hkgDirectJoystickPC9S::getPrevButton(unsigned int b) const
{
	if ( b > 127 ) // 128 buttons
		return 0.0f;

	return (m_prevJoystickState.rgbButtons[b] & 0x80) != 0;
}



inline unsigned int hkgDirectJoystickPC9S::getNumSupportedButtons() const
{
	return m_numButtons;
}

inline HKG_DIRECT_JOYSTICK_CAPS hkgDirectJoystickPC9S::getCaps() const
{
	return m_caps;
}

inline void hkgDirectJoystickPC9S::addCaps( HKG_DIRECT_JOYSTICK_CAPS caps )
{
	m_caps |= caps;
}

inline HKG_DIRECT_JOYSTICK_TYPE hkgDirectJoystickPC9S::getType() const
{
	return m_type;
}

inline LPDIRECTINPUTDEVICE8& hkgDirectJoystickPC9S::getDevice()
{
	return m_pJoystick;
}

inline bool hkgDirectJoystickPC9S::hasPS2Caps() const
{
	const HKG_DIRECT_JOYSTICK_CAPS ps2Caps = 
		HKG_DIRECT_JOYSTICK_XAXIS |
		HKG_DIRECT_JOYSTICK_YAXIS	| 
		HKG_DIRECT_JOYSTICK_ZAXIS	|
		HKG_DIRECT_JOYSTICK_ZROT	|
		HKG_DIRECT_JOYSTICK_POV0;

	return ((ps2Caps & m_caps) == m_caps) && (getNumSupportedButtons() >= 12);
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
