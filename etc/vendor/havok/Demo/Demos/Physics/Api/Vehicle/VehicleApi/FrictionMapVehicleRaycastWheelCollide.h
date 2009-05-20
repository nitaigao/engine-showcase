/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef FRICTION_MAP_VEHICLE_RAYCAST_COLLIDE_H
#define FRICTION_MAP_VEHICLE_RAYCAST_COLLIDE_H

#include <Physics/Vehicle/WheelCollide/RayCast/hkpVehicleRaycastWheelCollide.h>
#include <Physics/Vehicle/hkpVehicleInstance.h>
#include <Physics/Collide/Query/CastUtil/hkpWorldRayCastOutput.h>

#define VEHICLE_RAYCAST_ICY_FRICTION_PROPERTY 67341

	// This is the class which implements the calcSingleWheelGroundFriction, allowing you to override the
	// ground friction calculation for raycast vehicles. Here, as a very simple example, we overwrite the friction to be
	// 0.01 for all bodies (upon which we are driving) which have been tagged with a VEHICLE_RAYCAST_FRICTION_PROPERTY
	// user property, otherwise we leave the friction as it is.
	// In general of course, the friction value need not be constant over the body (or time), and any function can be used 
	// here, for example a lookup into your own full 2D 'friction map'.
	// Other ways to store/retrieve friction values would be via:
	//	hkpShape user data
	//	A 'float' hkpProperty stored with a the rigid body
	//	hkMeshMaterials for hkMeshShapes
class FrictionMapVehicleRaycastWheelCollide : public hkpVehicleRaycastWheelCollide
{
	virtual void calcSingleWheelGroundFriction(hkpVehicleInstance* vehicle, hkInt8 wheelInfoNum, const hkpWorldRayCastOutput& worldRayCastOutput, hkReal& frictionOut) const
	{
		hkpRigidBody* rb = hkGetRigidBody(worldRayCastOutput.m_rootCollidable);
		// Collisions with phantoms are possible in general, depending on how the user sets up the 'wheel filtering'
		// so we cannot assume that the collidable corresponds to a rigid body. Hence we check the result of hkGetRigidBody()
		// here.
		if( rb )
		{
			if( rb->hasProperty(VEHICLE_RAYCAST_ICY_FRICTION_PROPERTY) )
			{
				frictionOut = 0.01f;
			}
		}
	}
};

#endif // FRICTION_MAP_VEHICLE_RAYCAST_COLLIDE_H


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
