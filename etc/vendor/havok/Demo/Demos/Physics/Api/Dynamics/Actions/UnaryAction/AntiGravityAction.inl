/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

///[TUTORIAL 2]

inline AntiGravityAction::AntiGravityAction( hkpRigidBody* body, const hkVector4& gravity )
	: hkpUnaryAction( body ),
	  m_gravity( gravity ),
	  m_modulateStrength(0.0f)
{
}

///[CODE antigravitycode]<
inline void AntiGravityAction::applyAction( const hkStepInfo& stepInfo )
{
	// Apply the antigravity force to the body. 
	// The force ranges from [-2 * gravity to 0], modulated by a sine wave as a function of time
	// so sometimes it is stronger than gravity, sometimes less strong.
	m_modulateStrength += stepInfo.m_deltaTime;

	// Clamp m_modulateStrength so that it will never overrun float range (although the demo
	// will need to run for quite some time for that to happen!)
	if( m_modulateStrength > HK_REAL_PI )
	{
		m_modulateStrength = 0;
	}

	// Scaling factor that varies over time.
	hkReal scale = 1.0f + hkMath::cos(2 * m_modulateStrength);

	// Vector representing force to be applied.
	hkVector4 force;

	// Apply the antigravity force in the opposite direction to gravity. Its strength is
	// determined by multiplying m_gravity (also a vector) by scale.
	force.setMul4( -1 * scale, m_gravity);

	// This function call actually applies the force to the rigidBody.
	static_cast<hkpRigidBody*>(m_entity)->applyForce( stepInfo.m_deltaTime, force );
}
///[CODE antigravitycode]>

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
