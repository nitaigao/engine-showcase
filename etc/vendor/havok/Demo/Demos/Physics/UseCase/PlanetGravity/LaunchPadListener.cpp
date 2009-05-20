/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>

#include <Demos/Physics/UseCase/PlanetGravity/LaunchPadListener.h>
#include <Demos/Physics/UseCase/PlanetGravity/PlanetGravityDemo.h>

#include <Physics/Dynamics/Entity/hkpRigidBody.h>

void LaunchPadListener::contactPointAddedCallback(hkpContactPointAddedEvent& event)
{
	// Grab the body to launch
	hkpRigidBody* collidedRb = hkGetRigidBody( event.m_bodyA->getRootCollidable() );

	// If m_forceMagnitude is zero then launch toward a target
	if( m_forceMagnitude == 0.0f )
	{
		collidedRb->setMaxLinearVelocity( 300.0f );

		hkVector4 impulse;
		impulse.setSub4( m_targetPosition, collidedRb->getPosition() );
		impulse.mul4( 225.0f );

		PlanetGravityDemo::applyScaledLinearImpulse( collidedRb, impulse );
	}
	// Just apply an impulse along the contact normal
	else
	{
		hkVector4 impulse( event.m_contactPoint->getNormal() );
		impulse.mul4( m_forceMagnitude * 225.0f );
		collidedRb->setMaxLinearVelocity( 300.0f );

		PlanetGravityDemo::applyScaledLinearImpulse( collidedRb, impulse );
	}

	event.m_status = HK_CONTACT_POINT_REJECT;
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
