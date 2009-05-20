/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


#ifndef HK_DESTRUCTIBLEMOPPUTIL_H
#define HK_DESTRUCTIBLEMOPPUTIL_H

#include <Physics/Dynamics/Collide/hkpCollisionListener.h>
#include <Physics/Dynamics/Entity/hkpEntityListener.h>

class hkpBvTreeShape;
class hkpMoppBvTreeShape;

class DestructibleMoppUtility 
{
	public:
		struct ConvexShapeData
		{
			const hkpConvexShape* m_shape;
			hkContactPoint m_contactDetails;	// in shape space
			hkTransform	   m_transform;	
		};

			// this collects all child shapes with top level convex transform/translate shapes (which might have been added by the hkpExtendedMeshShape)
		static void collectAllConvexChildShapesWithinRadius( const hkpBvTreeShape* treeShape, hkVector4Parameter sphereCenterInShapeSpace, hkReal radius, const hkpCollisionInput& collisionInput,
																hkArray<ConvexShapeData>& shapesOut, hkArray<hkpShapeKey>& keysOut );


			// removes some keys from a mopp
		static void removeKeysFromMopp( const hkpMoppBvTreeShape* treeShape, hkVector4Parameter sphereCenterInShapeSpace, hkReal radius, const hkArray<hkpShapeKey>& keys );

		struct ShapeData
		{
			hkpShape*       m_shape;
			hkTransform	   m_transform;	
		};

			// merge multiple shapes into one
		static void mergeShapes( hkArray<ConvexShapeData>& shapeDataArray, ShapeData& shapeOut );

};



#endif // HK_DESTRUCTIBLEMOPPUTIL_H


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
