/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#include <Demos/demos.h>



#include <Physics/Collide/hkpCollide.h>

#include <Demos/Physics/Test/Feature/Dynamics/DestructibleShack/DestructibleHierarchy/DestructibleHierarchy.h>
#include <Demos/Physics/Test/Feature/Dynamics/DestructibleShack/ShapeHierarchyUtils/hkFlattenShapeHierarchyUtil.h>
#include <Demos/Physics/Test/Feature/Dynamics/DestructibleShack/ShapeHierarchyUtils/hkOneFixedMoppUtil.h>

#include <Physics/Collide/Shape/Misc/Transform/hkpTransformShape.h>
#include <Physics/Collide/Shape/Convex/ConvexTransform/hkpConvexTransformShape.h>
#include <Physics/Collide/Shape/Convex/ConvexTranslate/hkpConvexTranslateShape.h>
#include <Physics/Collide/Shape/Compound/Collection/List/hkpListShape.h>
//#include <Physics/Collide/Shape/Misc/ConvexList/hkpConvexListShape.h>
#include <Physics/Collide/Shape/Compound/Collection/ExtendedMeshShape/hkpExtendedMeshShape.h>

#include <Physics/Dynamics/World/hkpWorld.h>
#include <Physics/Dynamics/Entity/hkpRigidBody.h>
#include <Physics/Dynamics/World/hkpPhysicsSystem.h>

#include <Physics/Internal/Collide/Mopp/Code/hkpMoppCode.h>
#include <Physics/Collide/Shape/Compound/Tree/Mopp/hkpMoppCompilerInput.h>
#include <Physics/Collide/Shape/Compound/Tree/Mopp/hkpMoppUtility.h>

#include <Physics/Collide/Shape/Compound/Tree/Mopp/hkpMoppBvTreeShape.h>
#include <Physics/Collide/Shape/Compound/Collection/SimpleMesh/hkpSimpleMeshShape.h>

#include <Common/Base/Container/PointerMap/hkPointerMapBase.h>
#include <Common/Base/Container/PointerMap/hkPointerMap.h>


hkpRigidBody* HK_CALL hkOneFixedMoppUtil::addAllFixedObjectToMopp(hkpWorld* world, hkpCollisionListener* listener, hkBool useDestructibleHierarchy, hkBool putConvexShapesInSeparateSubparts)
{
	// create one fixed hkpRigidBody with zero position ..
	hkpRigidBodyCinfo info;
	info.m_motionType = hkpMotion::MOTION_FIXED;
	hkpRigidBody* fixedBody = new hkpRigidBody(info);

	hkBool scalingSet = false;

	hkArray<hkpRigidBody*> rigidBodiesToRemove;

	hkArray<hkpConvexShape*> convexShapes;
	hkArray<hkpShape*> otherShapes;
	hkArray<hkpExtendedMeshShape::TrianglesSubpart> triangleSubparts;

		// This is to make sure that all listeners form the bodies being set to fixed are copied to the one joint fixed object.
	hkPointerMap<hkpCollisionListener*, hkUint32> collisionListenersFromDynamicBodies;

	hkpPhysicsSystem* system = world->getWorldAsOneSystem();
	for (int r = 0; r < system->getRigidBodies().getSize(); r++)
	{
		hkpRigidBody* body = system->getRigidBodies()[r];

			// Process all fixed objects, and objects marked in hkFlattenShapeHierarchyUtil::flattenHierarchy().
		if (body->isFixed() || (body->getCollidable()->getShape() && hkUlong(body->getCollidable()->getShape()->getUserData()) == 0x0000beef))
		{
			// for all the complex shapes -- get their final convex pieces & the transforms & add them to subparts.
			const hkpShape* shape = body->getCollidable()->getShape();
			hkTransform transform = body->getTransform();
			if ( shape && HK_SUCCESS == extractLeafShapesFromShape(shape, transform, triangleSubparts, convexShapes, otherShapes) )
			{
				if (!body->isFixed())
				{
					// This is only done for destructible bodies, i.e. not fixed ones.
					updateRigidBodyOfShape(shape, fixedBody);
				}
				rigidBodiesToRemove.pushBack(body);
			}
		}
	}
	system->removeReference();

	for (int r = 0; r < rigidBodiesToRemove.getSize(); r++)
	{
		world->removeEntity(rigidBodiesToRemove[r]);
	}
	rigidBodiesToRemove.clear();

	// build the mesh shape
	hkpExtendedMeshShape* meshShape = hkOneFixedMoppUtil::createMeshShape(triangleSubparts, convexShapes, putConvexShapesInSeparateSubparts);
	{
		for (int i=0; i<convexShapes.getSize(); i++)
		{
			convexShapes[i]->removeReference();
		}
		convexShapes.clear();

		HK_ASSERT2(0XAD78999D, otherShapes.isEmpty(), "Non convex shapes were extracted -- they cannot be inserted into the mopp, and will be removed.");

		for (int i=0; i<otherShapes.getSize(); i++)
		{
			otherShapes[i]->removeReference();
		}
		otherShapes.clear();

		triangleSubparts.clear();
	}

	// build the mopp
	hkpMoppBvTreeShape* moppShape = hkOneFixedMoppUtil::createMoppShape(meshShape);
	meshShape->removeReference();

	fixedBody->setShape(moppShape);
	moppShape->removeReference();

	world->addEntity(fixedBody);

	if(useDestructibleHierarchy)
	{
		// This inserts ShapeKey information for all shapes of the mopp which are connected to a destructible hierarchy
		DestructibleHierarchy::insertShapeKeysIntoDestructibleHierarchy(fixedBody);
	}

	if(listener)
	{
		fixedBody->addCollisionListener(listener);
	}
	fixedBody->removeReference();

	return fixedBody;
}


// Convex, list or transform shape !
hkBool hkOneFixedMoppUtil::isConvexOrListShape(const hkpShape* shape)
{
	static const hkpShapeType acceptedTypes[] = {
		HK_SHAPE_CONVEX,
		HK_SHAPE_SPHERE,
		HK_SHAPE_CYLINDER,
		HK_SHAPE_TRIANGLE,
		HK_SHAPE_BOX,
		HK_SHAPE_CAPSULE,
		HK_SHAPE_CONVEX_VERTICES,
		HK_SHAPE_CONVEX_PIECE,
		HK_SHAPE_CONVEX_TRANSLATE,
		HK_SHAPE_CONVEX_TRANSFORM,
		HK_SHAPE_TRANSFORM,
		HK_SHAPE_LIST,
		HK_SHAPE_CONVEX_LIST };
		static const int numTypes = sizeof(acceptedTypes) / sizeof(hkpShapeType);

		for (int t = 0; t < numTypes; t++)
		{
			if (shape->getType() == acceptedTypes[t])
			{
				return true;
			}
		}

		return false;
}

//get rid of hkBool hkFlattenShapeHierarchyUtil::isLeafShape(const hkpShape* shape)
hkBool hkOneFixedMoppUtil::isTerminalConvexShape(const hkpShape* shape)
{
	static const hkpShapeType terminalTypes[] = {
		HK_SHAPE_CONVEX,
		HK_SHAPE_SPHERE,
		HK_SHAPE_CYLINDER,
		HK_SHAPE_TRIANGLE,
		HK_SHAPE_BOX,
		HK_SHAPE_CAPSULE,
		HK_SHAPE_CONVEX_VERTICES,
		HK_SHAPE_CONVEX_PIECE };
		static const int numTypes = sizeof(terminalTypes) / sizeof(hkpShapeType);

		for (int t = 0; t < numTypes; t++)
		{
			if (shape->getType() == terminalTypes[t])
			{
				return true;
			}
		}

		return false;
}

hkBool hkOneFixedMoppUtil::isMeshShape(const hkpShape* shape)
{
	static const hkpShapeType meshTypes[] = {
		/// hkpMoppBvTreeShape type.
		HK_SHAPE_MOPP,
		/// A shape collection which only returns triangles as child shapes, e.g. hkpMeshShape.
		HK_SHAPE_TRIANGLE_COLLECTION
	};
	static const int numTypes = sizeof(meshTypes) / sizeof(hkpShapeType);

	for (int t = 0; t < numTypes; t++)
	{
		if (shape->getType() == meshTypes[t])
		{
			return true;
		}
	}

	return false;
}

hkResult hkOneFixedMoppUtil::extractLeafShapesFromShape(const hkpShape* shape, const hkTransform& transform, hkArray<hkpExtendedMeshShape::TrianglesSubpart>& trianglePartsOut, hkArray<hkpConvexShape*>& convexShapesOut, hkArray<hkpShape*>& otherShapesOut)
{
	if (isConvexOrListShape(shape))
	{
		hkFlattenShapeHierarchyUtil::getLeafShapesFromShape(shape, transform, true, trianglePartsOut, convexShapesOut, otherShapesOut);
		return HK_SUCCESS;
	}
	else if (isMeshShape(shape))
	{
		//getMeshSubparts(shape, transform, trianglePartsOut);
		//return HK_SUCCESS;
		return HK_FAILURE;
	}

	return HK_FAILURE;
}

void hkOneFixedMoppUtil::getMeshSubparts(const hkpShape* shape, const hkTransform& transform, hkArray<hkpExtendedMeshShape::TrianglesSubpart*>& triangleSubpartsOut)
{
	//const hkpShapeCollection* collection = HK_NULL;
	//if (shape->getType() == HK_SHAPE_TRIANGLE_COLLECTION)
	//{
	//	collection = static_cast<const hkpShapeCollection*>(shape);
	//}
	//else if (shape->getType() == HK_SHAPE_MOPP)
	//{
	//	collection = static_cast<const hkpMoppBvTreeShape*>(shape)->getShapeCollection();
	//	HK_ASSERT2(0xad678d8d, collection->getType() == HK_SHAPE_TRIANGLE_COLLECTION, "unexpected collection type");
	//}
	//else
	//{
	//	HK_ASSERT2(0xad678ddd, false, "unhandled shape");
	//}

	//HK_WARN(0xad678aaa, "WARNING!!! only hkSimpleMeshShapes are handled by hkOneFixedMoppUtil::getMeshSubparts. Other shapes will cause a crash. ");

	//const hkpSimpleMeshShape* simpleMesh = static_cast<const hkpSimpleMeshShape*>(collection);

	////allocate subpart
	//hkpExtendedMeshShape::TrianglesSubpart* newPart = new hkpExtendedMeshShape::TrianglesSubpart(simpleMesh, transform);
	//triangleSubpartsOut.pushBack(newPart);
}


hkpExtendedMeshShape* hkOneFixedMoppUtil::createMeshShape(hkArray<hkpExtendedMeshShape::TrianglesSubpart>& triangleParts, hkArray<hkpConvexShape*>& convexShapes, hkBool putConvexShapesInSeparateSubparts)
{
	const hkReal defaultRadius = hkConvexShapeDefaultRadius;
	hkpExtendedMeshShape* extendeMeshShape = new hkpExtendedMeshShape(defaultRadius);

	//
	// add the convex shapes
	//
	if(convexShapes.getSize() > 0)
	{
		if(!putConvexShapesInSeparateSubparts)
		{
			// Important: don't use any transform when creating subpart -- otherwise the shape extracted with a hkpShapeKey gets wrapped in
			// another shape and we cannot access the userData directly
			hkpExtendedMeshShape::ShapesSubpart part(convexShapes.begin(), convexShapes.getSize()); // check num shapes
			extendeMeshShape->addShapesSubpart( part );
		}
		else
		{
			for (int c = 0; c < convexShapes.getSize(); c++)
			{
				hkpExtendedMeshShape::ShapesSubpart part(&convexShapes[c], 1);
				extendeMeshShape->addShapesSubpart( part );
			}

		}
	}

	for (int t = 0; t < triangleParts.getSize(); t++)
	{
		extendeMeshShape->addTrianglesSubpart(triangleParts[t]);
	}


	return extendeMeshShape;
}


hkpMoppBvTreeShape* hkOneFixedMoppUtil::createMoppShape(hkpExtendedMeshShape* meshShape)
{
	hkpMoppCompilerInput mci;
	hkpMoppCode* code = hkpMoppUtility::buildCode( meshShape , mci);
	hkpMoppBvTreeShape* moppShape = new hkpMoppBvTreeShape(meshShape, code);
	code->removeReference();
	return moppShape;
}




hkResult hkOneFixedMoppUtil::updateRigidBodyOfShape(const hkpShape* shape, hkpRigidBody* newOwningBody)
{
	hkUint16 lookupIdx = hkUint16((hkUlong(shape->getUserData()) >> 16) & 0xffff);
	if (lookupIdx && lookupIdx != 0xffff)
	{
		DestructibleHierarchy* hierarchy = DestructibleHierarchy::getDestructibleHierarchy(lookupIdx);
		unsigned int nodeIdx = DestructibleHierarchy::getNodeIndex(lookupIdx);

		hkResult result = DestructibleHierarchy::updateRigidBodyOfDestructibleHierarchy(hierarchy, nodeIdx, newOwningBody);

		if (HK_SUCCESS == result)
		{
			return HK_SUCCESS;
		}
		HK_ASSERT2(0xad6088dd, HK_SUCCESS == result, "Could not find a rigid body of a hieararchy");
	}


	const hkpShape* childShape = HK_NULL;;
	switch(shape->getType())
	{
	case HK_SHAPE_LIST:
	case HK_SHAPE_CONVEX_LIST:
		{
			const hkpShapeContainer* container = shape->getContainer();
			hkpShapeContainer::ShapeBuffer buffer;

			// todo: handle the situation where the assert below is triggered
			//HK_ASSERT2(0xad67da22, 0 == list->getUserData(), "We're dropping a non-zero user data.");

			for (hkpShapeKey key = container->getFirstKey(); key != HK_INVALID_SHAPE_KEY; key = container->getNextKey(key))
			{
				const hkpShape* child = container->getChildShape(key, buffer);
				if (HK_SUCCESS == updateRigidBodyOfShape( child, newOwningBody) )
				{
					return HK_SUCCESS;
				}
			}
			return HK_FAILURE;
		}
		break;
	case HK_SHAPE_TRANSFORM:
		{
			const hkpTransformShape* transformShape = static_cast<const hkpTransformShape*>(shape);
			childShape = transformShape->getChildShape();
		}
		break;
	case HK_SHAPE_CONVEX_TRANSFORM:
		{
			const hkpConvexTransformShape* convexTransformShape = static_cast<const hkpConvexTransformShape*>(shape);
			childShape = convexTransformShape->getChildShape();
		}
		break;
	case HK_SHAPE_CONVEX_TRANSLATE:
		{
			const hkpConvexTranslateShape* convexTranslateShape = static_cast<const hkpConvexTranslateShape*>(shape);
			childShape = convexTranslateShape->getChildShape();
		}
		break;
	default:
		break;
	}

	if (childShape)
	{
		if (HK_SUCCESS == updateRigidBodyOfShape(childShape, newOwningBody) )
		{
			return HK_SUCCESS;
		}
	}

	return HK_FAILURE;
}

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
