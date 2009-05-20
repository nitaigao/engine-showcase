/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


#ifndef HK_OneFixedmoppUtil_H
#define HK_OneFixedmoppUtil_H


#include <Physics/Collide/Shape/Compound/Collection/ExtendedMeshShape/hkpExtendedMeshShape.h>

class hkpWorld;
class hkpRigidBody;

class hkpShape;
class hkpMoppBvTreeShape;



class hkOneFixedMoppUtil
{
	public:
			// rename: this creates a new mopp made of all fixed object, removes all the old objects.
		static hkpRigidBody* HK_CALL addAllFixedObjectToMopp(hkpWorld* world, class hkpCollisionListener* listener = HK_NULL, hkBool useDestructibleHierarchy = true, hkBool putConvexShapesInSeparateSubparts = false);

			/// This gets leaf shapes form a shape. Each leaf shape is wrapped by a new instance of a hkpTransformShape or hkpConvexTransformShape
			/// and must be deallocated by the calling function later.
		static hkResult HK_CALL extractLeafShapesFromShape(const hkpShape* shape, const hkTransform& transform, hkArray<hkpExtendedMeshShape::TrianglesSubpart>& trianglePartsOut, hkArray<hkpConvexShape*>& convexShapes, hkArray<hkpShape*>& otherShapesOut);

		static hkResult HK_CALL updateRigidBodyOfShape(const hkpShape* shape, hkpRigidBody* newOwningBody);	

		//
		// Helper functions for creating hkpExtendedMeshShape
		//

		static hkpExtendedMeshShape* HK_CALL createMeshShape(hkArray<hkpExtendedMeshShape::TrianglesSubpart>& triangleParts, hkArray<hkpConvexShape*>& convexShapes, hkBool putConvexShapesInSeparateSubparts = false);
		static hkpMoppBvTreeShape* HK_CALL createMoppShape(hkpExtendedMeshShape* meshShape);

		//
		// This is missing -- we don't extract mesh subparts yet.
		//

			/// Note! This function allocates memory for the TriangleSubparts, that has to be deallocated by the calling function.
		static void HK_CALL getMeshSubparts(const hkpShape* shape, const hkTransform& transform, hkArray<hkpExtendedMeshShape::TrianglesSubpart*>& triangleSubpartsOut);


		//
		// Helper functions for processing of hierarchies
		//

		static hkBool HK_CALL isConvexOrListShape(const hkpShape* shape);

		static hkBool HK_CALL isTerminalConvexShape(const hkpShape* shape);

		static hkBool HK_CALL isMeshShape(const hkpShape* shape);
};



#endif // HK_OneFixedmoppUtil_H


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
