/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#include <Demos/demos.h>
#include <Demos/DemoCommon/Utilities/MarbleAction/MarbleAction.h>
#include <Physics/Dynamics/Entity/hkpRigidBody.h>

//#define HK_DISABLE_DEBUG_DISPLAY
#include <Common/Visualize/hkDebugDisplay.h>


MarbleAction::MarbleAction(hkpRigidBody* r, const hkVector4& forward, const hkVector4& resetPosition, hkReal impulseScale) :
	hkpUnaryAction(r),
	m_forwardPressed(false),
	m_backwardPressed(false),
	m_leftPressed(false),
	m_rightPressed(false),
	m_jumpPressed(false),
	m_resetPressed(false),
	m_brakePressed(false),
	m_lastJump(0.0f),
	m_impulseScale(impulseScale),
	m_lasttimeCalled(0.0f),
	m_rotationIncrement(0.01f),
	m_currentAngle(0.0f),
	m_forward(forward),
	m_resetPosition(resetPosition)
{

}

void MarbleAction::applyAction( const hkStepInfo& stepInfo )
{
	hkpRigidBody* rb = getRigidBody();

		// We need the delta time to record how long it's been since we last jumped
		// (to avoid jumping while in the air!)
	hkReal dt = stepInfo.m_deltaTime;

		// Get a "scale" to change the force of the impulse by,
		// depending on both the mass of the body, an an arbitrary "gain", eg 0.1
	hkReal scale  = rb->getMass() * m_impulseScale;
	
	hkVector4 axis(0,1,0);
	hkQuaternion q(axis, m_currentAngle);
	hkVector4 f;
	f.setRotatedDir(q, m_forward);

	if (m_forwardPressed)
	{	
		hkVector4 imp;
		imp.setMul4(scale, f);
		rb->applyLinearImpulse(imp);
	}
	
	if (m_backwardPressed)
	{
		hkVector4 imp;
		imp.setMul4(-scale, f);
		rb->applyLinearImpulse(imp);
	}

	if (m_leftPressed)
	{
		m_currentAngle += 3.141592653f * 2 * m_rotationIncrement;
	}

	if (m_rightPressed)
	{
		m_currentAngle -= 3.141592653f * 2 * m_rotationIncrement;
	}

	m_lasttimeCalled += dt;
	
		// Jump (only if haven't jumped for at least 1 second)
	if (m_jumpPressed && ((m_lasttimeCalled - m_lastJump) > 1.0f))
	{
		m_lastJump = m_lasttimeCalled;
		hkVector4 imp(0, rb->getMass() * 6, 0);
		rb->applyLinearImpulse(imp);
		setJumpPressed(false);
	}
	setJumpPressed(false);

		// If brake pressed, zero all velocities
	
	if (m_brakePressed)
	{	
		hkVector4 zero;
		zero.setZero4();
		rb->setLinearVelocity(zero);
		rb->setAngularVelocity(zero);
		setBrakePressed(false);
	}
	

		// Draw current "facing" direction, using s "Debug" line. This gets pushed onto a global
		// list, and gets dealt with by (perhaps) a drawDebugPointsAndLines() method from the mainline.
	hkVector4 start = rb->getPosition();
		// end = start + 1.5 * "forward"
	hkVector4 end(start);
	f.mul4( 1.5f );
	end.add4(f);
	HK_DISPLAY_LINE(start, end, 0xff0000ff);

}


void MarbleAction::reset()
{
	hkpRigidBody* rb = getRigidBody();

		// Put marble back to the reset position defined on construction, and zero velocities.
	hkVector4 zero;
	zero.setZero4();
	rb->setPosition(m_resetPosition);
	rb->setLinearVelocity(zero);
	rb->setAngularVelocity(zero);
	
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
