/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


inline hkgPad::hkgPad()
: m_curButtonState(0),
  m_prevButtonState(0),
  m_valid(false)
{
}

inline bool hkgPad::isValid() const
{
	return m_valid;
}

inline void hkgPad::cycleState()
{
	m_prevButtonState = m_curButtonState;
}

inline void hkgPad::setButtonState(HKG_PAD_BUTTON newState)
{
	m_curButtonState = newState;
}

inline HKG_PAD_BUTTON hkgPad::getButtonState() const
{
	return m_curButtonState;
}

inline HKG_PAD_BUTTON hkgPad::getPrevButtonState() const
{
	return m_prevButtonState;
}

inline float hkgPad::getStickPosX(int s) const
{
	return m_stick[s].m_cur[0];
}

inline float hkgPad::getStickPosY(int s) const
{
	return m_stick[s].m_cur[1];
}

inline float hkgPad::getPrevStickPosX(int s) const
{
	return m_stick[s].m_prev[0];
}

inline float hkgPad::getPrevStickPosY(int s) const
{
	return m_stick[s].m_prev[1];
}

inline bool hkgPad::isButtonPressed(HKG_PAD_BUTTON b) const
{
	return (m_curButtonState & b) != 0;
}

inline bool hkgPad::wasButtonPressed(HKG_PAD_BUTTON b) const
{
	return (m_curButtonState & b) && ((m_prevButtonState & b) == 0);
}

inline bool hkgPad::wasButtonReleased(HKG_PAD_BUTTON b) const
{
	return (m_prevButtonState & b) && ((m_curButtonState & b) == 0);
}

inline void hkgPad::forceButtonPressed(HKG_PAD_BUTTON b)
{
	m_curButtonState |= b;
	m_prevButtonState &= (~b);
}

inline bool hkgPad::hasStickXChanged(int stick, float tolerance ) const
{
	return hkg_fabs( m_stick[stick].m_cur[0] - m_stick[stick].m_prev[0] ) >= tolerance;
}

inline bool hkgPad::hasStickYChanged(int stick, float tolerance ) const
{
	return hkg_fabs( m_stick[stick].m_cur[1] - m_stick[stick].m_prev[1] ) >= tolerance;
}

inline const hkgPad::Stick& hkgPad::getStickState(int stick) const
{
	return m_stick[stick];
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
