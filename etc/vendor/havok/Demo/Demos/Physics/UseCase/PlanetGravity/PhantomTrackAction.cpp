/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/Physics/UseCase/PlanetGravity/PhantomTrackAction.h>
#include <Demos/Physics/UseCase/PlanetGravity/PlanetGravityAction.h>
#include <Demos/Physics/UseCase/PlanetGravity/SimpleGravityPhantom.h>

PhantomTrackAction::PhantomTrackAction( hkpRigidBody* trackedBody, hkpAabbPhantom* phantomToUpdate ) :
	hkpUnaryAction( trackedBody ), m_phantom(phantomToUpdate)
{
}

void PhantomTrackAction::applyAction( const hkStepInfo& stepInfo )
{
	// Find the new center to move to
	const hkVector4& newCenter = getRigidBody()->getPosition();

	// Grab the halfextents of the phantom's AABB
	hkVector4 halfExtents;
	halfExtents.setSub4( m_phantom->getAabb().m_max, m_phantom->getAabb().m_min );
	halfExtents.mul4( 0.5f );

	// Create a new AABB for the phantom, of the same size
	hkAabb newAabb;
	newAabb.m_min.setSub4( newCenter, halfExtents );
	newAabb.m_max.setAdd4( newCenter, halfExtents );

	// Update the phantom's AABB so it encloses the rigid body
	m_phantom->setAabb( newAabb );
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
