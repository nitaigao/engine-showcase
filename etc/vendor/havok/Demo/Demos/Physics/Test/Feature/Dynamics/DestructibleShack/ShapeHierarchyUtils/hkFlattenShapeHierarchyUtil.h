/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


#ifndef HK_hkFlattenShapeHierarchyUtil_H
#define HK_hkFlattenShapeHierarchyUtil_H


class hkpShape;
class hkpConvexShape;
class hkpWorld;
class hkTransform;
template <typename T>
class Array;


#include <Physics/Collide/Shape/Compound/Collection/ExtendedMeshShape/hkpExtendedMeshShape.h>


class hkFlattenShapeHierarchyUtil
{
	public:

		//
		// 
		//


			/// This returns a new shape consisting of a hkpListShape (or optionally a hkConexListShape)
			///	where every child is a hkpConvexTransformShape, hkpConvexTranslateShape, or hkpTransformShape
			/// wrapping a termininal shape, e.g. a hkpBoxShape.
			///
			/// Note: current implementeation does not support hkpConvexListShape, as it hides the userData
			/// of the enclosed shapes upon collision.
		static hkpShape* HK_CALL flattenHierarchy(const hkpShape* shape);

			/// This scans through all non-fixed bodies in the world and if they hava a compound shape 
			/// It processes them with the flattenHierarchy() function.
		static void HK_CALL flattenAllDynamicShapes(hkpWorld* world);

		//
		// Helper functions
		//

		static void HK_CALL getLeafShapesFromShape(const hkpShape* shape, const hkTransform& transform, const hkBool isFixedBody, hkArray<hkpExtendedMeshShape::TrianglesSubpart>& trianglePartsOut, hkArray<hkpConvexShape*>& convexShapesOut, hkArray<hkpShape*>& otherShapesOut);
		static hkBool HK_CALL isLeafShape(const hkpShape* shape);


		static void HK_CALL verifyAllShapes(const hkpWorld* m_pWorld);

		static void HK_CALL verifyShapeOfBody(const class hkpRigidBody* body);
};


#endif // HK_hkFlattenShapeHierarchyUtil_H


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
