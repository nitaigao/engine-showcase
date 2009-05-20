/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/Physics/UseCase/PlanetGravity/PlanetGravityDemo.h>
#include <Demos/Physics/UseCase/PlanetGravity/PlanetGravityAction.h>
#include <Physics/Collide/Query/Collector/PointCollector/hkpClosestCdPointCollector.h>
#include <Physics/Collide/Agent/hkpProcessCollisionInput.h>
#include <Physics/Collide/Dispatch/hkpCollisionDispatcher.h>
#include <Physics/Utilities/CharacterControl/CharacterRigidBody/hkpCharacterRigidBody.h>
#include <Common/Visualize/hkDebugDisplay.h>

// applyAction Called by the simulation every frame.
void PlanetGravityAction::applyAction( const hkStepInfo& stepInfo )
{
	hkpRigidBody* rb = getRigidBody();

	// Sets forceDir to be the difference of m_center and rb->getCenterOfMassInWorld().
	hkVector4 forceDir;

	hkpCollisionInput input = *getWorld()->getCollisionInput();
	hkpClosestCdPointCollector collector;
	hkpShapeType a = rb->getCollidable()->getShape()->getType();
	input.m_tolerance = 1.0f;

	do {
		// Use m_hullCollidable for gravity calculations if available.
		// GetClosestPoints() should provide a gravity vector
		if( m_hullCollidable == HK_NULL)
		{
			hkpShapeType b = m_planetBody->getCollidable()->getShape()->getType();
			hkpCollisionDispatcher::GetClosestPointsFunc getClosestPoints = getWorld()->getCollisionDispatcher()->getGetClosestPointsFunc( a, b );
			getClosestPoints( *rb->getCollidable(), *m_planetBody->getCollidable(), input, collector );
		}
		else
		{
			hkpShapeType b = m_hullCollidable->getShape()->getType();
			hkpCollisionDispatcher::GetClosestPointsFunc getClosestPoints = getWorld()->getCollisionDispatcher()->getGetClosestPointsFunc( a, b );
			getClosestPoints( *rb->getCollidable(), *m_hullCollidable, input, collector );	
		}

		// Grow the tolerance until we get a hit.
		input.m_tolerance = input.m_tolerance * 2.0f;
	} while( !collector.hasHit() );

	// Flip the normal of the getClosestPoints() result
	forceDir.setMul4( -1.0f, collector.getHitContact().getNormal() );
	forceDir.normalize3();
	forceDir.zeroElement( 3 );

	// Set force to be direction multiplied by magnitude multiplied by mass.
	hkVector4 force;
	force.setMul4( rb->getMass() * m_gravityForce, forceDir );

	// Apply the gravity force.
	if( rb == PlanetGravityDemo::m_characterRigidBody->getRigidBody() )
	{
		hkVector4 newUp(forceDir);
		newUp.setNeg4(forceDir);
		newUp.normalize3();

		// Only change gravity if the change isn't negligible
		if( PlanetGravityDemo::m_worldUp.dot3(newUp) < 1e-6f)
		{
			// don't reorient character when it has an invalid rotational axis
			//  (this may happen the first few frames)
			if( PlanetGravityDemo::m_characterRigidBody->getRigidBody()->getRotation().hasValidAxis())
			{
				hkRotation rbRotation; rbRotation.set(PlanetGravityDemo::m_characterRigidBody->getRigidBody()->getRotation());
				hkVector4& oldForward = rbRotation.getColumn(0);
				hkVector4 newRot; newRot.setCross(oldForward, newUp);
 				PlanetGravityDemo::m_characterForward.setCross(newUp, newRot);
 				PlanetGravityDemo::m_characterForward.normalize3();
			}
		}

		PlanetGravityDemo::m_worldUp = newUp;
	}
	else
	{
		rb->applyForce( stepInfo.m_deltaTime, force );
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
