/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


inline void HoverExampleAction::init( const hkVector4& gravity , hkReal desiredHeight )
{
	m_gravity = gravity;
	m_desiredHeight = desiredHeight;

		// zap surrent linear velocity as soon as we start applying to the body
	hkVector4 startVel(1.0f, 0, 0);
	static_cast<hkpRigidBody*>(m_entity)->setLinearVelocity(startVel);
}

inline HoverExampleAction::HoverExampleAction( hkpRigidBody* body, const hkVector4& gravity , hkReal desiredHeight )
	: hkpUnaryAction( body )
{
	init(gravity, desiredHeight);	
}

inline void HoverExampleAction::applyAction( const hkStepInfo& stepInfo )
{
	hkpRigidBody* body = static_cast<hkpRigidBody*>( m_entity );

	// Apply the hover force to the body. 
	hkReal height = body->getPosition()(1);

	if(height > m_desiredHeight)
		return;

	hkReal scale = (m_desiredHeight - height) * body->getMass();
	hkVector4 force;
	force.setMul4(-scale, m_gravity);
	
	body->applyForce( stepInfo.m_deltaTime, force );

	scale *= 0.1f;
	force.setMul4(-scale, m_gravity);
		// Show force applied
	hkVector4 start = body->getPosition();
	hkVector4 end = start;
	end.addMul4(-.5f, force);
	HK_DISPLAY_LINE(start, end, hkColor::RED);

	end = start;
	end.addMul4(-.2f, force);
	end(0) += 0.4f * scale;
	HK_DISPLAY_LINE(start, end, hkColor::RED);
	end(0) -= 0.8f * scale;
	HK_DISPLAY_LINE(start, end, hkColor::RED);

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
