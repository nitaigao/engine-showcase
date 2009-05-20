/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

// A demo which shows use of a very basic filter to disallow collisions between certain bodies.
// The scene consists of 3 boxes; two of which are fixed and the third dynamic. Initially the
// dynamic box is placed above the other two and dropped. However, due to the nature of the
// collision filter we have created, the box will pass through the first fixed box and only
// collide with the second lower one.

#ifndef HK_USER_COLLISION_FILTER_H
#define HK_USER_COLLISION_FILTER_H

#include <Demos/DemoCommon/DemoFramework/hkDefaultPhysicsDemo.h>
#include <Physics/Collide/Filter/hkpCollidableCollidableFilter.h>


class UserCollisionFilterDemo : public hkDefaultPhysicsDemo
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DEMO );

		UserCollisionFilterDemo( hkDemoEnvironment* env);
};


// A simple filter which allows collidables to collide only if their owners have identical 
// collisionfilterinfo details.
class MyCollisionFilter : public hkpCollisionFilter
{
	public:

		// Checks two collidables 
		virtual hkBool isCollisionEnabled( const hkpCollidable& a, const hkpCollidable& b ) const;

		// hkpShapeCollectionFilter interface forwarding
		virtual hkBool isCollisionEnabled( const hkpCollisionInput& input, const hkpCdBody& a, const hkpCdBody& b, const hkpShapeContainer& bContainer, hkpShapeKey keyB  ) const;

		// hkpShapeCollectionFilter interface forwarding
		virtual hkBool isCollisionEnabled( const hkpCollisionInput& input, const hkpCdBody& collectionBodyA, const hkpCdBody& collectionBodyB, const HK_SHAPE_CONTAINER& containerShapeA, const HK_SHAPE_CONTAINER& containerShapeB, hkpShapeKey keyA, hkpShapeKey keyB ) const;

		// hkpRayShapeCollectionFilter interface forwarding
		virtual hkBool isCollisionEnabled( const hkpShapeRayCastInput& aInput, const hkpShape& shape, const hkpShapeContainer& bContainer, hkpShapeKey bKey ) const;

		// hkpRayCollidableFilter interface forwarding
		virtual hkBool isCollisionEnabled( const hkpWorldRayCastInput& aInput, const hkpCollidable& collidableB ) const;

	protected:
		// The base collision filtering logic - all the other function calls will forward to this one
		hkBool isCollisionEnabled(hkUint32 infoA, hkUint32 infoB) const;

};

#endif // HK_USER_COLLISION_FILTER_H


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
