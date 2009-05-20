/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_BRICKFILTER_H
#define HK_BRICKFILTER_H

#include <Physics/Collide/Filter/hkpCollisionFilter.h>
#include <Physics/Dynamics/Entity/hkpRigidBody.h>

// Sample collision filter for brick walls
class BrickFilter : public hkpCollisionFilter {
public:

	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO);

	BrickFilter() {}

	// hkpCollisionFilter interface
	/// Checks two collidables 
	inline virtual hkBool isCollisionEnabled( const hkpCollidable& a, const hkpCollidable& b ) const;

	/// hkpShapeCollectionFilter interface forwarding
	virtual	hkBool isCollisionEnabled( const hkpCollisionInput& input, const hkpCdBody& a, const hkpCdBody& b, const hkpShapeContainer& bContainer, hkpShapeKey bKey  ) const { return true; }

	/// hkpRayShapeCollectionFilter interface forwarding
	virtual hkBool isCollisionEnabled( const hkpShapeRayCastInput& aInput, const hkpShape& bShape, const hkpShapeContainer& bContainer, hkpShapeKey bKey ) const { return true; }

	// hkpShapeCollectionFilter interface forwarding
	virtual hkBool isCollisionEnabled( const hkpCollisionInput& input, const hkpCdBody& collectionBodyA, const hkpCdBody& collectionBodyB, const HK_SHAPE_CONTAINER& containerShapeA, const HK_SHAPE_CONTAINER& containerShapeB, hkpShapeKey keyA, hkpShapeKey keyB ) const { return true; }

	/// hkpRayCollidableFilter interface forwarding
	virtual hkBool isCollisionEnabled( const hkpWorldRayCastInput& a, const hkpCollidable& collidableB ) const { return true; }

	hkUint32 GetInfo() { return m_Info; }

	// helper functions to compute filter info
	static inline hkUint32 HK_CALL calcFilterInfo(hkUint32 i, hkUint32 j, hkUint32 wallID, bool leftconstr=false, bool bottconstr=false);
	static inline hkUint32 HK_CALL calcFilterInfo(hkUint32 i, hkUint32 j, hkUint32 wallID, hkUint32 constrMask);
	static inline hkUint32 HK_CALL calcConstraintMask(bool leftconstr=false, bool bottconstr=false);

	// remove the constraint from the 2 elements from the mask. returns 1 if the first is modifieed, 2 if the second,
	//  0 if none of the two is modified
	static int removeConstaintFromMask(hkpRigidBody* a, hkpRigidBody* b);

	// Extract position (in terms of rows and columns in the originary wall) from the brick
	static inline void extractPositionFromInfo(const hkpRigidBody& brick, int& i, int& j);
	// returns true if the brick has left constraint
	static inline hkBool hasLeftConstraint(const hkpRigidBody& brick);
	// returns true if the brick has bottom constraint
	static inline hkBool hasBottomConstraint(const hkpRigidBody& brick);
	// extracts the wall id from filter info
	static inline int getWallID(const hkpRigidBody& brick);
	// set the wall id 
	static inline void HK_CALL setWallID(hkUint32& info, int wallID);

private:

	// actual collision fitering function, the member functions of the interface call this one
	hkBool isCollisionEnabled( hkUint32 infoA, hkUint32 infoB) const;

	static inline void HK_CALL setMask(hkUint32& info, hkInt32 mask);
	static inline void removeConstraintAndUpdateInfo(hkUint32 mask, hkUint32 direction, hkUint32 info, hkpRigidBody* b);
private:
	// internal data 
	//    14bits |  16 bits   |  2 bits
	// [---pos---|----wall----|--neigh--]
	// Pos   : position [i,j] of the brick in the wall grid
	// Wall  : wall ID
	// Neigh : constraints with neighbourhood [Left|Bottom], 1 if the constraint is enabled, 0 otherwise
	// Two bricks can collide only if 
	// a) they are in different walls
	// b) they are in the same wall and they're not neighbours, or they're neighbours but not constrained to each other in the same direction
	hkUint32 m_Info;
};



class BrickFilterNoCollisions : public hkpCollisionFilter {
public:

	BrickFilterNoCollisions() {}

	// hkpCollisionFilter interface
	/// Checks two collidables 
	virtual hkBool isCollisionEnabled( const hkpCollidable& a, const hkpCollidable& b ) const { return false; }

	/// hkpShapeCollectionFilter interface forwarding
	virtual	hkBool isCollisionEnabled( const hkpCollisionInput& input, const hkpCdBody& a, const hkpCdBody& b, const hkpShapeContainer& bContainer, hkpShapeKey bKey  ) const { return false; }

	// hkpShapeCollectionFilter interface forwarding
	virtual hkBool isCollisionEnabled( const hkpCollisionInput& input, const hkpCdBody& collectionBodyA, const hkpCdBody& collectionBodyB, const HK_SHAPE_CONTAINER& containerShapeA, const HK_SHAPE_CONTAINER& containerShapeB, hkpShapeKey keyA, hkpShapeKey keyB ) const 
	{
		return false;
	}

	/// hkpRayShapeCollectionFilter interface forwarding
	virtual hkBool isCollisionEnabled( const hkpShapeRayCastInput& aInput, const hkpShape& bShape, const hkpShapeContainer& bContainer, hkpShapeKey bKey ) const { return false; }

	/// hkpRayCollidableFilter interface forwarding
	virtual hkBool isCollisionEnabled( const hkpWorldRayCastInput& a, const hkpCollidable& collidableB ) const { return false; }

};


#include <Demos/Physics/Test/Feature/Dynamics/DestructibleWalls/BrickFilter.inl>

#endif

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
