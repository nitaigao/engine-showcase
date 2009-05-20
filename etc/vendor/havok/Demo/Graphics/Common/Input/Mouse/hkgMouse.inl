/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


inline hkgMouse::hkgMouse()
:m_curButtonState(0), m_prevButtonState(0), m_curX(0), m_curY(0), m_prevX(0), m_prevY(0), m_valid(false)
{
}

inline bool hkgMouse::isValid() const
{
	return m_valid;
}

inline void hkgMouse::cycleState()
{
	m_prevButtonState = m_curButtonState;
	m_prevX = m_curX;
	m_prevY = m_curY;
}

inline HKG_MOUSE_BUTTON hkgMouse::getButtonState() const
{
	return m_curButtonState;
}

inline void hkgMouse::setButtonState(HKG_MOUSE_BUTTON state)
{
	m_curButtonState = state;
}

inline HKG_MOUSE_BUTTON hkgMouse::getPrevButtonState() const
{
	return m_prevButtonState;
}

inline int hkgMouse::getPosX() const
{
	return m_curX;
}

inline int hkgMouse::getPosY() const
{
	return m_curY;
}

inline int hkgMouse::getPrevPosX() const
{
	return m_prevX;
}

inline int hkgMouse::getPrevPosY() const
{
	return m_prevY;
}

inline bool hkgMouse::wasButtonPressed(HKG_MOUSE_BUTTON b) const
{
	return (m_curButtonState & b) && ((m_prevButtonState & b) == 0);
}

inline bool hkgMouse::wasButtonReleased(HKG_MOUSE_BUTTON b) const
{
	return (m_prevButtonState & b) && ((m_curButtonState & b) == 0);
}

inline bool hkgMouse::hasXChanged(int tolerance) const
{
	return hkg_fabs( m_curX - m_prevX ) >= tolerance;
}

inline bool hkgMouse::hasYChanged(int tolerance) const
{
	return hkg_fabs( m_curY - m_prevY ) >= tolerance;
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
