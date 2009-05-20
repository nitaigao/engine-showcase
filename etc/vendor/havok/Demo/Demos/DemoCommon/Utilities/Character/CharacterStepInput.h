/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_CHARACTER_STEP_INPUT_H
#define HK_CHARACTER_STEP_INPUT_H

struct CharacterStepInput
{
	hkReal	m_forwardVelocity;			// in meters per second
	hkReal	m_turnVelocity;				// in radians per second
	hkReal	m_straffeLeftRightVelocity; // in meters per second
	hkReal	m_jumpVelocity;

	CharacterStepInput()
	{
		m_forwardVelocity = 0.0f;
		m_turnVelocity = 0.0f;
		m_straffeLeftRightVelocity = 0.0f;
		m_jumpVelocity = 0.0f;
	}
};

struct CharacterActionInfo
{
	hkBool m_wasDiePressed;
	hkBool m_wasGetUpPressed;
	hkBool m_wasJumpPressed;
	hkBool m_wasDivePressed;

	CharacterActionInfo()
	{
		m_wasDiePressed = false;
		m_wasGetUpPressed = false;
		m_wasJumpPressed = false;
		m_wasDivePressed = false;
	}
};

enum CharacterAction
{
	ACTION_DIE,
	ACTION_GETUP,
	ACTION_JUMP,
	ACTION_DIVE,

	MAX_ACTIONS,
};

#endif // HK_BIPED_CHARACTER_H

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
