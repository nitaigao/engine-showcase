/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef MARBLE_ACTION_H
#define MARBLE_ACTION_H

#include <Physics/Dynamics/Action/hkpUnaryAction.h>
#include <Common/Base/hkBase.h>

class hkpRigidBody;

/// This is a really basic Action which applies impulses to a sphere to roll it around a world.
/// When he corresponding keys are pressed, the internal state is set to "push" or "rotate" the
/// given body. If this body is a sphere, it will roll (due to the friction) when in contact
/// with the ground. When "rotated", ie. turned left/right, the body itself not actually rotated,
/// only the "forward" direction.
/// The setXPressed() accessors allow you to change the internal state. The body maintains its state
/// internally, so it is not necessary (for example) to continuously tell the action to go forward once
/// setForwardPressed(true) is called. It will go forward until setForwardPressed(false) is called.
///
/// You can also make the body "jump" by applying an upward impulse, and "brake" by killing all velocity
class MarbleAction: public hkpUnaryAction
{
	public:
			// The forward direction here is the original direction (in world space) which the 
			// body is said to be "facing".
		MarbleAction(hkpRigidBody* r, const hkVector4& forward, const hkVector4& resetPosition, hkReal impulseScale = 0.1f );

		
			// Set internal states
		inline void setForwardPressed(hkBool f);
		inline void setBackwardPressed(hkBool b);
		inline void setLeftPressed(hkBool l);
		inline void setRightPressed(hkBool r);
		inline void setJumpPressed(hkBool j);
		inline void setBrakePressed(hkBool b);

			// Move body back to reset postion (set on construction)
		void reset();

			// This is the call that performs the action, that gets called from the physics engine
		void applyAction( const hkStepInfo& stepInfo );

		/// no need to clone as we don't use clone func in this demo
		virtual hkpAction* clone( const hkArray<hkpEntity*>& newEntities, const hkArray<hkpPhantom*>& newPhantoms ) const { return HK_NULL; }

	private:		

		hkBool				m_forwardPressed;
		hkBool				m_backwardPressed;
		hkBool				m_leftPressed;
		hkBool				m_rightPressed;				
		hkBool				m_jumpPressed;
		hkBool				m_resetPressed;
		hkBool				m_brakePressed;

		hkReal				m_lastJump;

		hkReal				m_impulseScale;

		hkReal				m_lasttimeCalled;

		const float			m_rotationIncrement;
		float				m_currentAngle;

		const hkVector4		m_forward;
		const hkVector4		m_resetPosition;
};

inline void MarbleAction::setForwardPressed(hkBool f)
{
	m_forwardPressed = f;
}

inline void MarbleAction::setJumpPressed(hkBool j)
{
	m_jumpPressed = j;
}

inline void MarbleAction::setBackwardPressed(hkBool b)
{
	m_backwardPressed = b;
}

inline void MarbleAction::setLeftPressed(hkBool l)
{
	m_leftPressed = l;
}

inline void MarbleAction::setRightPressed(hkBool r)
{
	m_rightPressed = r;
}

inline void MarbleAction::setBrakePressed(hkBool b)
{
	m_brakePressed = b;
}


#endif // MARBLE_ACTION_H

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
