


/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef PINIONMOTOR_ACTION_H
#define PINIONMOTOR_ACTION_H

#include <Physics/Dynamics/Action/hkpUnaryAction.h>
#include <Physics/Dynamics/Entity/hkpRigidBody.h>

// We'll need to display a debug line
#include <Common/Visualize/hkDebugDisplay.h>

class hkpRigidBody;

class PinionMotorAction: public hkpUnaryAction
{
	public:
		inline PinionMotorAction(hkpRigidBody* r, const hkVector4& axis, hkReal spinRate, hkReal gain = 2.0f);


			/// Gets the spin rate (angular velocity) the action is trying to reach
		inline hkReal getSpinRate();

			/// Sets the new desired spin rate the motor action will try to reach
		inline void setSpin(hkReal new_rate);

			/// Gets the current gain
		inline hkReal getGain();

			/// Sets the a new gain rate for the action
		inline void setGain(hkReal new_rate);

			/// Gets the axis around which the object should spin
		inline const hkVector4& getAxis();

			/// Sets the axis around which the object should spin
		inline void setAxis(hkVector4& axis);

			/// Activate/deactivate
		inline void setActivation(hkBool b);

			/// no need to clone as we don't use clone func in this demo
		virtual hkpAction* clone( const hkArray<hkpEntity*>& newEntities, const hkArray<hkpPhantom*>& newPhantoms ) const { return HK_NULL; }

	private:		
		inline void applyAction( const hkStepInfo& stepInfo );

		hkVector4	m_axis;			//[The desired angular axis of spin for the motor.//]
		hkReal		m_spinRate;		//[The rate of spin about this axis, specified in radians per second.//]
		hkReal		m_gain;			//[The speed with which the desired velocity is attained.//]

		hkBool		m_active;


};

inline PinionMotorAction::PinionMotorAction(hkpRigidBody* r, const hkVector4& axis, hkReal spinRate, hkReal gain) :
	hkpUnaryAction(r), m_axis(axis), m_spinRate(spinRate), m_gain(gain), m_active(true)
{
	m_axis.normalize3();
}



/// This function sets a new spin rate (angular velocity, in rad/s) that the motor
/// action will try to reach.
/// \param new_rate The new desired spin rate
inline void PinionMotorAction::setSpin(hkReal new_rate)
{
	m_spinRate = new_rate;
}

/// This function changes the gain (the rate at which the action will try to reach
/// the desired angular velocity).
///
/// \param new_rate The new gain. Range [>0].
///
inline void PinionMotorAction::setGain(hkReal new_rate)
{
	m_gain = new_rate;
}

/// This function retrieves the axis around which the object should spin (in local
/// space). This axis is specified on construction.
inline const hkVector4& PinionMotorAction::getAxis()
{
	return m_axis;
}

/// This function sets the axis around which the object should spin (in local
/// space). 
inline void PinionMotorAction::setAxis(hkVector4& axis)
{
	m_axis = axis;
}

/// This function retrieves the angular velocity the action will try to reach during
/// simulation. This value is set on construction and can be changed by calling
/// setSpinRate()
inline hkReal PinionMotorAction::getSpinRate()
{
	return m_spinRate;
}

/// This member function retrieves the gain that the action is currently using to
/// reach the desired spin speed. The gain is set on construction and can be
/// modified by the function setGain()
inline hkReal PinionMotorAction::getGain()
{
	return m_gain;
}



inline void PinionMotorAction::setActivation(hkBool b)
{
	m_active = b;
}




inline void PinionMotorAction::applyAction( const hkStepInfo& stepInfo )
{
	if(!m_active)
		return;

	hkpRigidBody* rb = getRigidBody();

	// Work out the current angular velocity in body space
	const hkTransform& tr = rb->getTransform();
	hkVector4 curr;
	curr.setRotatedInverseDir(tr.getRotation(), rb->getAngularVelocity());
		
	// Calculate the difference between the desired spin rate and the current rate of spin
	// about the desired axis 'm_axis'
	hkReal diff((hkSimdReal)m_spinRate - (hkSimdReal)m_axis.dot3(curr));

	// Calculate the newTorque to apply based on the difference and the gain
	// The newTorque should be proportional to each of difference, gain, and inertia 
	// (to make the Action mass-independent)
	hkVector4 newTorque;
	newTorque.setMul4(diff * m_gain, m_axis);
	hkMatrix3 m;
	rb->getInertiaLocal(m);
	newTorque.setMul3(m, newTorque);

	newTorque.setRotatedDir(tr.getRotation(), newTorque);

	// Apply the new torque;
	rb->applyTorque( stepInfo.m_deltaTime, newTorque);
}

#endif // PINIONMOTOR_ACTION_H

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
