/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

inline GravityAction::GravityAction(hkpRigidBody* satellite,
									const hkVector4 &gravityCenter,
									const hkReal gravityConstant)
:	hkpUnaryAction(satellite),
	m_gravityCenter(gravityCenter),
	m_gravityConstant(gravityConstant)
{
}

inline void GravityAction::applyAction( const hkStepInfo& stepInfo )
{
	hkpRigidBody* rb = getRigidBody();

	hkVector4 dir;

	// Sets dir to be the difference of m_gravityCenter and rb->getCenterOfMassInWorld().
	dir.setSub4( m_gravityCenter, rb->getCenterOfMassInWorld() );
	
	hkReal distance = dir.normalizeWithLength3();
	
	// Use a version of the gravitational formula:
	//
	// F = (GMm)/d^2
	//
	// If G and M are fixed (rolled into m_gravityConstant), then F = m_gravityConstant * m / d^2
	// i.e. the force is proportional both to the mass, and the inverse distance squared.
	hkReal magnitude = (m_gravityConstant * rb->getMass()) / (distance * distance);

	hkVector4 force;

	// Set force to be dir multiplied by magnitude.
	force.setMul4(magnitude, dir);

	// Apply the gravity force.
	rb->applyForce( stepInfo.m_deltaTime, force );

	if ( distance < 8.0f )
	{
		rb->setLinearDamping( -0.1f );
	}
	else
	{
		rb->setLinearDamping( 0.0f );
	}
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
