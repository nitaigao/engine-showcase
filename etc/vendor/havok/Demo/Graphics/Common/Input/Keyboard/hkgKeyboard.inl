/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
inline hkgKeyboard::hkgKeyboard()
: m_valid(false)
{
	hkString::memSet( m_curState, 0, sizeof(m_curState) );
	hkString::memSet( m_prevState, 0, sizeof(m_prevState) );
}

inline bool hkgKeyboard::getKeyState( HKG_KEYBOARD_VKEY key ) const
{
	HK_ASSERT(0x36d3484b,  key < HKG_KEYBOARD_NUM_VKEYS );
	return m_curState[key];
}

inline void hkgKeyboard::setCurrentKeyState ( HKG_KEYBOARD_VKEY key, bool state)
{
	HK_ASSERT(0x36d3484b,  key < HKG_KEYBOARD_NUM_VKEYS );
	m_curState[key] = state;
}

inline bool hkgKeyboard::getPrevKeyState( HKG_KEYBOARD_VKEY key ) const
{
	HK_ASSERT(0x5b1b621e,  key < HKG_KEYBOARD_NUM_VKEYS );
	return m_prevState[key];
}

inline bool hkgKeyboard::isValid() const
{
	return m_valid;
}

inline bool hkgKeyboard::wasKeyPressed(HKG_KEYBOARD_VKEY key ) const
{
	return m_curState[key] && (!m_prevState[key]);
}	

inline bool hkgKeyboard::wasKeyReleased(HKG_KEYBOARD_VKEY key ) const
{
	return (!m_curState[key]) && m_prevState[key];
}

inline void hkgKeyboard::cycleState()
{
	hkString::memCpy( m_prevState, m_curState, HKG_KEYBOARD_NUM_VKEYS * sizeof(bool));
}

inline int hkgKeyboard::numKeysPressed() const
{
	hkUint32 c = 0;
	for (int cc=0; cc < HKG_KEYBOARD_NUM_VKEYS; ++cc)
		c += (hkUint32)( m_curState[cc] );
	return c;
}

inline void hkgKeyboard::clear()
{
	cycleState();
	hkString::memSet( m_curState, 0, sizeof(m_curState) );
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
