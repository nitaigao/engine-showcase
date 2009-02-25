/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2008 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_UTILITIES2_PAIRWISE_COLLISION_FILTER_H
#define HK_UTILITIES2_PAIRWISE_COLLISION_FILTER_H

#include <Physics/Collide/Filter/hkpCollisionFilter.h>
#include <Physics/Dynamics/Entity/hkpEntityListener.h>
#include <Physics/Dynamics/Entity/hkpEntity.h>

extern const hkClass hkpPairwiseCollisionFilterClass;

/// A simple filter to disable collisions on a pairwise basis. 
/// Note that this is currently VERY INEFFICIENT with large numbers of pairs, as it is implemented using a linear list.
/// If you want to use this style of collision filter in a production game you should definitely use your own optimized filter.
class hkpPairwiseCollisionFilter : public hkpCollisionFilter, private hkpEntityListener
{
	public:
 
		//+version(1)
		HK_DECLARE_REFLECTION();

			///
		hkpPairwiseCollisionFilter();

			///
		~hkpPairwiseCollisionFilter();

			/// Disables collisions between this pair of collidables.
		void disableCollisionPair(hkpEntity* a, hkpEntity* b);

			/// Enables collisions between this pair of collidables.
		void enableCollisionPair(hkpEntity* a, hkpEntity* b);

			/// hkpCollisionFilter interface implementation.
			/// This implementation checks if the pair is in the disabled list.
		virtual hkBool isCollisionEnabled(const hkpCollidable& a, const hkpCollidable& b ) const;

			// hkpShapeCollectionFilter interface forwarding
		virtual	hkBool isCollisionEnabled( const hkpCollisionInput& input, const hkpCdBody& a, const hkpCdBody& b, const hkpShapeContainer& bContainer, hkpShapeKey bKey  ) const;

			// hkpShapeCollectionFilter interface forwarding
		virtual hkBool isCollisionEnabled( const hkpCollisionInput& input, const hkpCdBody& collectionBodyA, const hkpCdBody& collectionBodyB, const HK_SHAPE_CONTAINER& containerShapeA, const HK_SHAPE_CONTAINER& containerShapeB, hkpShapeKey keyA, hkpShapeKey keyB ) const;

			// hkpRayShapeCollectionFilter interface forwarding
		virtual hkBool isCollisionEnabled( const hkpShapeRayCastInput& aInput, const hkpShape& shape, const hkpShapeContainer& bContainer, hkpShapeKey bKey ) const;

			// hkpRayCollidableFilter interface forwarding
		virtual hkBool isCollisionEnabled( const hkpWorldRayCastInput& a, const hkpCollidable& collidableB ) const;

			/// hkpEntityListener implementation - removes any references to removed entities.
		virtual void entityRemovedCallback(hkpEntity* entity);

			/// hkpEntityListener implementation.
		virtual void entityDeletedCallback(hkpEntity* entity);
		
			/// gives the number of disabled pairs in the filter
		virtual int  getNumDisabledPairs() const;

	public:

		struct CollisionPair
		{
			//+version(1)
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_UTILITIES, hkpPairwiseCollisionFilter::CollisionPair );
			HK_DECLARE_REFLECTION();

			inline CollisionPair(hkpEntity* a, hkpEntity* b);
			
			hkpEntity* m_a;
			hkpEntity* m_b;
		};

	public:

		hkArray<struct CollisionPair> m_disabledPairs; // changed back from hkMap is it is just an array too.

	public:

		hkpPairwiseCollisionFilter(hkFinishLoadedObjectFlag f) : hkpCollisionFilter(f), m_disabledPairs(f) {}
};

inline hkBool operator == (const hkpPairwiseCollisionFilter::CollisionPair& a, const hkpPairwiseCollisionFilter::CollisionPair& b);

#include <Physics/Utilities/Collide/Filter/Pairwise/hkpPairwiseCollisionFilter.inl>

#endif // HK_PAIRWISE_COLLISION_FILTER_H

/*
* Havok SDK - NO SOURCE PC DOWNLOAD, BUILD(#20080925)
* 
* Confidential Information of Havok.  (C) Copyright 1999-2008
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
