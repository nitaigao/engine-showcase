/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_SIMPLE_GRAVITY_PHANTOM_H
#define HK_SIMPLE_GRAVITY_PHANTOM_H

#include <Physics/Dynamics/Phantom/hkpAabbPhantom.h>

class hkpCollidable;
class hkpCharacterRigidBody;
class hkpRigidBody;

// This phantom surrounds a planet and adds the simple gravity action to overlapping bodies
// It sets the action userdata to be this phantom so that it knows which action to remove. 
class SimpleGravityPhantom : public hkpAabbPhantom
{
	public:
		SimpleGravityPhantom( hkpRigidBody* body, const hkAabb& aabb, const hkpCollidable* hullCollidable, hkUint32 collisionFilterInfo = 0 )
		  : hkpAabbPhantom( aabb, collisionFilterInfo), m_body(body), m_hullCollidable(hullCollidable) {}

		  // Adds the action to overlapping rigid bodies
		  virtual void addOverlappingCollidable( hkpCollidable* handle );

		  // Removes the action from rigid bodies as they leave
		  virtual void removeOverlappingCollidable( hkpCollidable* handle );

	private:
		// Rigid body representing the planet
		hkpRigidBody* m_body;

		// Collidable representing the gravitational field
		const hkpCollidable* m_hullCollidable;
};

#endif  // HK_SIMPLE_GRAVITY_PHANTOM_H

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
