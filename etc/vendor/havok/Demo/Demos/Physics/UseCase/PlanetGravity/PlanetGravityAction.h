/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_PLANET_GRAVITY_ACTION_H
#define HK_PLANET_GRAVITY_ACTION_H

#include <Physics/Dynamics/Action/hkpUnaryAction.h>

// Bodies with this action will have a force applied to them toward the surface of the planet
class PlanetGravityAction: public hkpUnaryAction
{
	public:
		// Constructor takes fixed point, mobile satellite body and a scaling factor
		// for the force to be applied. 

		static const hkUlong HK_SPHERE_ACTION_ID = 0x28021978;

		// Pass 0 for no phantomID
		PlanetGravityAction( hkpRigidBody* planetBody, hkpRigidBody* satellite, const hkpCollidable* hullCollidable, hkUlong phantomId, hkReal maxAcceleration = 45.0f ):
			hkpUnaryAction(satellite), m_planetBody(planetBody), m_hullCollidable(hullCollidable), m_phantomId(phantomId), 
			m_gravityForce(maxAcceleration)
		{
			setUserData( HK_SPHERE_ACTION_ID );
		}

		hkUlong& accessPhantomId()
		{
			return m_phantomId;
		}

	private:

		// applyAction Called by the simulation every timestep.
		virtual void applyAction( const hkStepInfo& stepInfo );
		virtual hkpAction* clone( const hkArray<hkpEntity*>& newEntities, const hkArray<hkpPhantom*>& newPhantoms ) const { return HK_NULL; }

		// Planet to which we are being attracted
		hkpRigidBody* m_planetBody;

		// The collidable representing the gravitational field
		const hkpCollidable* m_hullCollidable;

		// Phantom ID, if a phantom has been associated with this action then we have this ID so that
		// we can search through a body's actions and update values as necessary
		hkUlong m_phantomId;

		// Max Acceleration used for objects inside innerRadius
		hkReal m_gravityForce;
};

#endif  // HK_PLANET_GRAVITY_ACTION_H

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
