/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2008 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_COLLIDE2_LIST_SHAPE_H
#define HK_COLLIDE2_LIST_SHAPE_H

#include <Physics/Collide/Shape/Compound/Collection/hkpShapeCollection.h>
#include <Physics/Collide/Util/hkpAabbUtil.h>

extern const hkClass hkpListShapeClass;

struct hkAabbUint32;
struct hkpCollisionInput;

/// A simple static list of hkShapes. You can use this shape class to create compound bodies.
/// A list shape can hold a mix of different shape types e.g. an ice cream cone could be made
/// from a list shape containing a sphere for the ice cream and a convex vertices shape
/// for the wafer cone.
/// If your list shape contains many subshapes, consider using a hkpBvTreeShape for faster access
class hkpListShape : public hkpShapeCollection
{
	public:

		//+version(2)
		HK_DECLARE_REFLECTION();

		enum { MAX_CHILDREN_FOR_SPU_MIDPHASE = 252 };

		struct ChildInfo
		{
			//+version(1)
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_COLLIDE, hkpListShape::ChildInfo );
			HK_DECLARE_REFLECTION();
			HK_ALIGN16(const hkpShape* m_shape);	// we need this because all the child infos might not fit into a cacheline
			hkUint32 m_collisionFilterInfo;
			mutable int m_shapeSize; // +nosave
			mutable int m_numChildShapes; // +nosave	
			
		};

			/// Flags for controlling list shape algorithms
		enum ListShapeFlags
		{
			ALL_FLAGS_CLEAR = 0,							///
			DISABLE_SPU_CACHE_FOR_LIST_CHILD_INFO = 1<<0	///
		};

	public:

			/// Constructs a list shape with an array of pointers to shapes.
			/// Note: The hkpShapeContainer::ReferencePolicy is only used for the constructor, the destructor will still always decrement the childs reference
		hkpListShape( const hkpShape*const* shapeArray, int numShapes, hkpShapeContainer::ReferencePolicy ref = hkpShapeContainer::REFERENCE_POLICY_INCREMENT );

		hkpListShape( class hkFinishLoadedObjectFlag flag );

			/// The destructor removes references to child shapes.
		~hkpListShape();


			/// Returns the ith child shape.
		inline const hkpShape* getChildShapeInl(int i) const { return m_childInfo[i].m_shape; }


			/// Call this whenever your underlying child shapes moves or increases its AABB
		void recalcAabbExtents();

			/// Call this whenever your underlying child shapes moves or increases its AABB
			/// Also returns the aabb
		void recalcAabbExtents( hkAabb& aabbOut );

		//
		// hkpShapeCollection interface
		//


			// hkpShapeCollection interface implementation.
		inline virtual int getNumChildShapes() const { return m_childInfo.getSize(); }

			/// Get the first child shape key.
		inline virtual hkpShapeKey getFirstKey() const { return 0; }

			/// Get the next child shape key.
		inline virtual hkpShapeKey getNextKey( hkpShapeKey key ) const { return int(++key) < m_childInfo.getSize() ? key : HK_INVALID_SHAPE_KEY; }

			// hkpShapeContainer::getCollisionFilterInfo() interface implementation.
		HKP_SHAPE_VIRTUAL hkUint32 getCollisionFilterInfoImpl( HKP_SHAPE_VIRTUAL_THIS hkpShapeKey key ) HKP_SHAPE_VIRTUAL_CONST;

			/// Sets the collisionFilterInfo for a given index
		void setCollisionFilterInfo( hkpShapeKey index, hkUint32 filterInfo );

		/// Allows for quickly disabling a child shape.
		/// Warning: you also have to invalidate the corresponding cached aabb in the hkpCollidable::m_boundingVolumeData::m_childShapeAabbs.
		/// You can use the hkpBreakOffPartsUtil::removeKeysFromListShape() to do this for you
		HK_FORCE_INLINE void disableChild( hkpShapeKey index )
		{
			HK_ASSERT2( 0xf0f34fe5, index < MAX_DISABLED_CHILDREN, "You can only disable the first 256 children" );
			m_enabledChildren[ index>>5 ] &= ~(1<<(index&0x1f));
		}

		/// Allows for quickly enabling a child shape.
		HK_FORCE_INLINE void enableChild( hkpShapeKey index )
		{
			m_enabledChildren[ index>>5 ] |= (1<<(index&0x1f));
		}

		HK_FORCE_INLINE hkBool32 isChildEnabled( hkpShapeKey index ) const
		{
			if ( index < MAX_DISABLED_CHILDREN )
			{
				return m_enabledChildren[ index>>5 ] & (1<<(index&0x1f));
			}
			return true;
		}

			/// hkpShapeContainer::getChildShape() interface implementation.
		HKP_SHAPE_VIRTUAL const hkpShape* getChildShapeImpl( HKP_SHAPE_VIRTUAL_THIS hkpShapeKey key, ShapeBuffer& buffer ) HKP_SHAPE_VIRTUAL_CONST;


		//
		// hkpShape implementation
		//
			// hkpShape interface implementation.
		HKP_SHAPE_VIRTUAL void getAabbImpl( HKP_SHAPE_VIRTUAL_THIS const hkTransform& localToWorld, hkReal tolerance, hkAabb& out ) HKP_SHAPE_VIRTUAL_CONST;

			//	hkpShape interface implementation.
		HKP_SHAPE_VIRTUAL hkBool castRayImpl( HKP_SHAPE_VIRTUAL_THIS const hkpShapeRayCastInput& input, hkpShapeRayCastOutput& results) HKP_SHAPE_VIRTUAL_CONST;

			/// Returns a struct of function pointers needed by the SPU
		static void HK_CALL registerSimulationFunctions( ShapeFuncs& sf );

			/// Returns a struct of function pointers needed by the SPU
		static void HK_CALL registerCollideQueryFunctions( ShapeFuncs& sf );

			/// Returns a struct of function pointers needed by the SPU
		static void HK_CALL registerGetAabbFunction( ShapeFuncs& sf );

        virtual void calcContentStatistics( hkStatisticsCollector* collector, const hkClass* cls) const;

		virtual int calcSizeForSpu(const CalcSizeForSpuInput& input, int spuBufferSizeLeft) const;

		void getAabbWithChildShapes(const hkpCollisionInput& collisionInput, const hkpAabbUtil::OffsetAabbInput& input, const hkTransform& localToWorld, const hkVector4& massCenter, hkReal tolerance, hkAabbUint32& rootAabbUint32, hkAabbUint32* childShapesAabbUint32, int numAabbs) const;

			// Returns the number of enabled child shapes.
		int getAabbWithChildShapesForAgent(const hkpCollisionInput& collisionInput, const hkpAabbUtil::OffsetAabbInput& input, hkBool32 useContinuousPhysics, const hkTransform& localToWorld, hkReal tolerance, hkAabb& rootAabb, hkAabbUint32* aabbsOut, int numAabbs) const;


	public:

		friend class hkpListAgent;
		// hkInplaceArray<struct ChildInfo,4> m_childInfo;
		hkArray<struct ChildInfo>	m_childInfo;

		hkUint32 m_flags; ///

		hkVector4					m_aabbHalfExtents;
		hkVector4					m_aabbCenter;

			// if one of the bits is set, the child is disabled
		enum { MAX_DISABLED_CHILDREN = 256 };

		hkUint32 m_enabledChildren[ 8 ]; // [MAX_DISABLED_CHILDREN/32]

	protected:

		void setShapes( const hkpShape*const* shapeArray, int numShapes, const hkUint32* filterInfoArray = HK_NULL, hkpShapeContainer::ReferencePolicy ref = hkpShapeContainer::REFERENCE_POLICY_INCREMENT );

};

#	define HK_ACCESS_COLLECTION_METHOD(obj, func) obj->func


#endif // HK_COLLIDE2_LIST_SHAPE_H

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
