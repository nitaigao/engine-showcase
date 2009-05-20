/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#include <Demos/demos.h>




#include <Physics/Collide/hkpCollide.h>

#include <Demos/Physics/Test/Feature/Dynamics/DestructibleShack/ShapeHierarchyUtils/hkFlattenShapeHierarchyUtil.h>
#include <Demos/Physics/Test/Feature/Dynamics/DestructibleShack/ShapeHierarchyUtils/hkOneFixedMoppUtil.h>

#include <Physics/Dynamics/World/hkpPhysicsSystem.h>
#include <Physics/Dynamics/World/hkpWorld.h>

#include <Physics/Dynamics/Entity/hkpRigidBody.h>

#include <Physics/Collide/Shape/Misc/Transform/hkpTransformShape.h>
#include <Physics/Collide/Shape/Convex/ConvexTransform/hkpConvexTransformShape.h>
#include <Physics/Collide/Shape/Convex/ConvexTranslate/hkpConvexTranslateShape.h>
#include <Physics/Collide/Shape/Compound/Collection/List/hkpListShape.h>

#include <Physics/Collide/Shape/Compound/Tree/Mopp/hkpMoppBvTreeShape.h>

#include <Common/Serialize/Util/hkBuiltinTypeRegistry.h>
#include <Common/Base/Reflection/Registry/hkVtableClassRegistry.h>


#include <Physics/Collide/Shape/Deprecated/Mesh/hkpMeshShape.h>


#include <Common/Base/Reflection/hkClass.h>

hkBool hkFlattenShapeHierarchyUtil::isLeafShape(const hkpShape* shape)
{
	switch(shape->getType())
	{
	case HK_SHAPE_LIST:
	case HK_SHAPE_CONVEX_LIST:
	case HK_SHAPE_TRANSFORM:
	case HK_SHAPE_CONVEX_TRANSFORM:
	case HK_SHAPE_CONVEX_TRANSLATE:
		return false;
	default:
		return true;
	}
}

void hkFlattenShapeHierarchyUtil::getLeafShapesFromShape(const hkpShape* shape, const hkTransform& transform, const hkBool isFixedBody, hkArray<hkpExtendedMeshShape::TrianglesSubpart>& trianglePartsOut, hkArray<hkpConvexShape*>& convexShapesOut, hkArray<hkpShape*>& otherShapesOut)
{
	const hkpShapeType type = shape->getType();

	hkTransform newTransform;

	const hkpShape* childShape = HK_NULL;
	hkUlong userData = HK_NULL;

	switch(type)
	{
	case HK_SHAPE_LIST:
	case HK_SHAPE_CONVEX_LIST:
		{
			const hkpShapeContainer* container = shape->getContainer();
			hkpShapeContainer::ShapeBuffer buffer;

			HK_ASSERT2(0xad67da22, 0 == (0xffff0000 & hkUlong(shape->getUserData())), "We're dropping a non-zero lookupIndex from user data.");
			hkUint16 materialId = hkUint16(0xffff & hkUlong(shape->getUserData()));

			for (hkpShapeKey key = container->getFirstKey(); key != HK_INVALID_SHAPE_KEY; key = container->getNextKey(key))
			{
				const hkpShape* child = container->getChildShape(key, buffer);

				if (materialId && 0 == (0xffff & hkUlong(child->getUserData())) )
				{
					// no material id for the child -- copy it
					hkUlong childData = hkUlong(child->getUserData()) | materialId;
					// Warning: modifying the const input hkpShape*
					const_cast<hkpShape*>(child)->setUserData(childData);
				}
				//HK_ASSERT2(0xad6777dd, isFixedBody || !isLeafShape(child), "A child of a list shape cannot be a terminal node. You must use a transform shape in between." );
				getLeafShapesFromShape(child, transform, isFixedBody, trianglePartsOut, convexShapesOut, otherShapesOut);
			}
		}
		break;
	case HK_SHAPE_TRANSFORM:
		{
			const hkpTransformShape* transformShape = static_cast<const hkpTransformShape*>(shape);
			newTransform.setMul(transform, transformShape->getTransform());
			childShape = transformShape->getChildShape();
			userData = hkUlong(transformShape->getUserData());
		}
		break;
	case HK_SHAPE_CONVEX_TRANSFORM:
		{
			const hkpConvexTransformShape* convexTransformShape = static_cast<const hkpConvexTransformShape*>(shape);
			newTransform.setMul(transform, convexTransformShape->getTransform());
			childShape = convexTransformShape->getChildShape();
			userData = hkUlong(convexTransformShape->getUserData());
		}
		break;
	case HK_SHAPE_CONVEX_TRANSLATE:
		{
			const hkpConvexTranslateShape* convexTranslateShape = static_cast<const hkpConvexTranslateShape*>(shape);
			hkTransform localTransform( static_cast<const hkRotation&>(hkRotation::getIdentity()), convexTranslateShape->getTranslation() );
			newTransform.setMul(transform, localTransform);
			childShape = convexTranslateShape->getChildShape();
			userData = hkUlong(convexTranslateShape->getUserData());
		}
		break;

	case HK_SHAPE_BV_TREE:
		{
			const hkpBvTreeShape* bvTreeshape = static_cast<const hkpBvTreeShape*>(shape);	
			HK_ASSERT2(0xad67da22, 0 == (0xffff0000 & hkUlong(bvTreeshape->getUserData())), "We're dropping a non-zero lookupIndex from user data.");

			const hkpShapeContainer* container = bvTreeshape->getContainer();
			for (hkpShapeKey key = container->getFirstKey(); key!= HK_INVALID_SHAPE_KEY; key = container->getNextKey(key))
			{
				hkpShapeContainer::ShapeBuffer buffer;
				const hkpShape* child = container->getChildShape(key, buffer);
				const hkpShapeType childType = child->getType();
				if ((childType == HK_SHAPE_LIST) || (childType == HK_SHAPE_CONVEX_LIST))
				{
					getLeafShapesFromShape(child, transform, isFixedBody, trianglePartsOut, convexShapesOut, otherShapesOut);
				}
			}

			break;
		}
	case HK_SHAPE_MOPP:
		{
			const hkpMoppBvTreeShape* bvTreeshape = static_cast<const hkpMoppBvTreeShape*>(shape);	
		//	HK_ASSERT2(0xad67da22, 0 == (0xffff0000 & hkUlong(bvTreeshape->getUserData())), "We're dropping a non-zero lookupIndex from user data.");
			getLeafShapesFromShape(bvTreeshape->getShapeCollection(), transform, isFixedBody, trianglePartsOut, convexShapesOut, otherShapesOut);

			break;
		}

	default:
		{
			//HK_ASSERT2(0xad67ddaa, isFixedBody, "A child of a list was attached without an intermediate transform shape. This is not handled.");
			
			// We can get simple shapes without transforms when processing fixed bodies. We do add a hkpConvexTransformShape as usual then ...
			childShape = shape;
			newTransform = transform;
			userData = hkUlong(shape->getUserData());
			//HK_ASSERT2(0XAD678D8D, 0 == (userData & 0xffff0000), "Userdata of a fixed body (other than the one fixed uber body) has a non-zero destructible info index.");
		}
		break;
	}

	if (HK_NULL != childShape)
	{
		hkBool leafDone = false;
		if (hkOneFixedMoppUtil::isTerminalConvexShape(childShape))
		{
			// Create new transform shape to wrap the child terminal shape
			hkpConvexTransformShape* newConvexTransformShape = new hkpConvexTransformShape(static_cast<const hkpConvexShape*>(childShape), newTransform);
			newConvexTransformShape->setUserData( userData );

			HK_ASSERT2(0xad67da23, 0 == (0xffff0000 & hkUlong(childShape->getUserData())) || (0xffff0000 & userData) == (0xffff0000 & hkUlong(childShape->getUserData())), "We're dropping a non-zero user data.");
			HK_ASSERT2(0xad67da24, 0 == (0xffff & hkUlong(childShape->getUserData())) || (0xffff & userData) == (0xffff & hkUlong(childShape->getUserData())), "Materials differ in the terminal shape and its wrapping hkpTransformShape.");

			// put this transform on the shapesOut list
			convexShapesOut.pushBack(newConvexTransformShape);
			leafDone = true;
		}
		else if (isLeafShape(childShape))
		{
				// It's not a terminal(leaf?) convex shape, but it might be a mesh, or indeed a simplemesh.
				// It's most likely to be a storagemesh, but we can't assume that, so check vtable:
			const hkClass* childShapeClass = hkBuiltinTypeRegistry::getInstance().getVtableClassRegistry()->getClassFromVirtualInstance(childShape);
			if( hkpMeshShapeClass.isSuperClass(*childShapeClass) )
			{
				const hkpMeshShape* mesh = static_cast<const hkpMeshShape*>(childShape);

				// Confirm vertex data not shared, see below
#if defined(HK_DEBUG)
				{
					for(int i = 0; i < mesh->getNumSubparts(); i++)
					{
						const hkpMeshShape::Subpart& meshSubPartI = mesh->getSubpartAt(i);
						for(int j = i+1; j < mesh->getNumSubparts(); j++)
						{
							const hkpMeshShape::Subpart& meshSubPartJ = mesh->getSubpartAt(j);
							HK_ASSERT2(0x0, meshSubPartI.m_vertexBase != meshSubPartJ.m_vertexBase, "This method can't (currently) collapse chared meshs data as it collpases the transform into the verts\n");
						}
						
					}
				}
#endif

				for(int i = 0; i < mesh->getNumSubparts(); i++)
				{
					const hkpMeshShape::Subpart& meshSubPart = mesh->getSubpartAt(i);
					// Now we have the subpart. We can't know if the data pointed to is 'owned' by the mesh (eg. subclass hkpStorageMeshShape)
					// or 'pointed to' (base class hkpMeshShape)
					//
					// We'll just assume here we can 'share' the data by grabbing the pointers...

					hkpExtendedMeshShape::TrianglesSubpart extendedMeshSubPart;
					
					extendedMeshSubPart.m_vertexBase = meshSubPart.m_vertexBase;
					extendedMeshSubPart.m_vertexStriding = meshSubPart.m_vertexStriding;
					extendedMeshSubPart.m_numVertices = meshSubPart.m_numVertices;
					extendedMeshSubPart.m_triangleOffset = meshSubPart.m_triangleOffset;

					// .. but we'll have to multiply in the transform! This assumes the vertex data is not shared!
					{
						for(int j = 0; j < extendedMeshSubPart.m_numVertices ; j++)
						{
							hkVector4 v;
							v(0) = extendedMeshSubPart.m_vertexBase[0 + extendedMeshSubPart.m_vertexStriding/sizeof(float) * j];
							v(1) = extendedMeshSubPart.m_vertexBase[1 + extendedMeshSubPart.m_vertexStriding/sizeof(float) * j];
							v(2) = extendedMeshSubPart.m_vertexBase[2 + extendedMeshSubPart.m_vertexStriding/sizeof(float) * j];
							v.setTransformedPos(transform, v);
							const_cast<float*>(extendedMeshSubPart.m_vertexBase)[0 + extendedMeshSubPart.m_vertexStriding/sizeof(float) * j] = v(0);
							const_cast<float*>(extendedMeshSubPart.m_vertexBase)[1 + extendedMeshSubPart.m_vertexStriding/sizeof(float) * j] = v(1);
							const_cast<float*>(extendedMeshSubPart.m_vertexBase)[2 + extendedMeshSubPart.m_vertexStriding/sizeof(float) * j] = v(2);
						}
					}

					extendedMeshSubPart.m_indexBase = meshSubPart.m_indexBase;
					extendedMeshSubPart.m_indexStriding = meshSubPart.m_indexStriding;
					extendedMeshSubPart.m_numTriangleShapes = meshSubPart.m_numTriangles;
					extendedMeshSubPart.m_stridingType = (meshSubPart.m_stridingType == hkpMeshShape::INDICES_INT16) ? hkpExtendedMeshShape::INDICES_INT16 : hkpExtendedMeshShape::INDICES_INT32;

					trianglePartsOut.pushBack(extendedMeshSubPart);

					leafDone = true;
				}
			}
		}

		if(!leafDone)
		{
			HK_WARN(0xad678dda, "An extra hkTransform shape has been inserted into the hierarchy. This might be suboptimal.");
			// Create new transform shape to wrap the child terminal shape
			hkpTransformShape* newTransformShape = new hkpTransformShape(childShape, newTransform);
			newTransformShape->setUserData( userData );

			HK_ASSERT2(0xad67da23, 0 == (0xffff0000 & hkUlong(childShape->getUserData())) || (0xffff0000 & userData) == (0xffff0000 & hkUlong(childShape->getUserData())), "We're dropping a non-zero user data.");
			HK_ASSERT2(0xad67da24, 0 == (0xffff & hkUlong(childShape->getUserData())) || (0xffff & userData) == (0xffff & hkUlong(childShape->getUserData())), "Materials differ in the terminal shape and its wrapping hkpTransformShape.");

			// put this transform on the shapesOut list
			otherShapesOut.pushBack(newTransformShape);
			leafDone = true;
		}

		if(!leafDone)
		{
			//HK_ASSERT2(0xad67da23, 0 == (0xffff0000 & shape->getUserData()), "We're dropping a non-zero user data.");
			if ( 0xffff0000 & hkUlong(shape->getUserData()) )
			{
				// copy the destruction info index downwards..
				HK_ASSERT2(0xad67da23, 0 == (0xffff0000 & hkUlong(childShape->getUserData())) || (0xffff0000 & userData) == (0xffff0000 & hkUlong(childShape->getUserData())), "We're dropping a non-zero user data.");
				HK_ASSERT2(0xad67da24, 0 == (0xffff & hkUlong(childShape->getUserData())) || (0xffff & hkUlong(shape->getUserData())) == (0xffff & hkUlong(childShape->getUserData())), "Materials differ in the terminal shape and its wrapping hkpTransformShape.");

				//HK_WORLD_ACCESS_CHECK(parentBody->getWorld(), HK_ACCESS_RW );
				// Warning: we're actually modifying the const hkpShape* passed as an input parameter
				const_cast<hkpShape*>(childShape)->setUserData( shape->getUserData() );
			}
			getLeafShapesFromShape(childShape, newTransform, isFixedBody, trianglePartsOut, convexShapesOut, otherShapesOut);
		}
	}
}


// the original shapes should be intact..
hkpShape* hkFlattenShapeHierarchyUtil::flattenHierarchy(const hkpShape* shape)
{
	hkArray<hkpExtendedMeshShape::TrianglesSubpart> triangleParts;
	hkArray<hkpConvexShape*> convexShapes;
	hkArray<hkpShape*> otherShapes;

	// Get all leaf shapes with their 'wrapping' transform shapes onto the list
	getLeafShapesFromShape(shape, hkTransform::getIdentity(), false, triangleParts, convexShapes, otherShapes);

	hkpShape* list = HK_NULL;

	hkArray<hkpShape*> allShapes;
	for (int s = 0; s < convexShapes.getSize(); s++)
	{
		allShapes.pushBack(static_cast<hkpShape*>(convexShapes[s]));
	}
	for (int s = 0; s < otherShapes.getSize(); s++)
	{
		allShapes.pushBack(otherShapes[s]);
	}

	HK_ASSERT2(0xad6788c6, allShapes.getSize(), "No shapes to process!.");

	if (allShapes.getSize() > 1)
	{
		list = new hkpListShape(allShapes.begin(), allShapes.getSize());
		for (int s = 0; s < allShapes.getSize(); s++)
		{
			allShapes[s]->removeReference();
		}
	}
	else
	{
		return allShapes[0];
	}


	HK_ASSERT2(0xad789aaa, 0 == list->getUserData(), "Overwriting user data ?");

	// WE'RE MARKING THIS BODY FOR INCLUSION IN THE FIXED UBER SHAPE LATER ..
	//  -- only if the number of child shapes is greater than 20
//	if (list->getNumChildShapes() > 20)
	{
		list->setUserData(0x0000beef);
	}

	return list;
}

void hkFlattenShapeHierarchyUtil::flattenAllDynamicShapes(hkpWorld* world)
{
	hkpPhysicsSystem* system = world->getWorldAsOneSystem();
	for (int r = 0; r < system->getRigidBodies().getSize(); r++)
	{
		hkpRigidBody* body = system->getRigidBodies()[r];

		if (!body->isFixedOrKeyframed())
		{
			// flatten the dynamic shape
			const hkpShape* originalShape = body->getCollidable()->getShape();
			if (!isLeafShape(originalShape))
			{
				hkpShape* flatShape = flattenHierarchy(originalShape);
				body->setShape(flatShape);
				flatShape->removeReference();
			}

		}
	}
}

static void verifyIsTerminalOrTransform(const hkpShape* shape)
{
	if (hkOneFixedMoppUtil::isTerminalConvexShape(shape))
	{
		return;
	}

	hkpShapeType type = shape->getType();

	if (type == HK_SHAPE_TRANSFORM || type == HK_SHAPE_CONVEX_TRANSFORM || type == HK_SHAPE_CONVEX_TRANSLATE )
	{
		const hkpShape* childShape = HK_NULL;
		switch(type)
		{
			case HK_SHAPE_TRANSFORM: childShape = static_cast<const hkpTransformShape*>(shape)->getChildShape(); break;
			case HK_SHAPE_CONVEX_TRANSFORM: childShape = static_cast<const hkpConvexTransformShape*>(shape)->getChildShape(); break;
			case HK_SHAPE_CONVEX_TRANSLATE: childShape = static_cast<const hkpConvexTranslateShape*>(shape)->getChildShape(); break;
			default: break;
		}
		HK_ASSERT2(0xad7788dd, hkOneFixedMoppUtil::isTerminalConvexShape(childShape), "bad" );

		return;
	}

	HK_ASSERT2(0xad6888dd, false, "bad");
}

static void verifyDynamicShape(const hkpShape* shape)
{
	hkpShapeType type = shape->getType();

	// may be list
	if (type == HK_SHAPE_LIST || type == HK_SHAPE_CONVEX_LIST)
	{
		const hkpListShape* list = static_cast<const hkpListShape*>(shape);
		for (int i = 0; i < list->getNumChildShapes(); i++)
		{
			verifyIsTerminalOrTransform(list->getChildShapeInl(i));
		}
	}
	else
	{
		verifyIsTerminalOrTransform(shape);
	}


	//HK_ASSERT2(0xad6778dd, shape->getType(), "bad");
}

void hkFlattenShapeHierarchyUtil::verifyShapeOfBody(const hkpRigidBody* body)
{
	if (!body->isFixed())
	{
		verifyDynamicShape(body->getCollidable()->getShape());
	}
	else 
	{
		//verifyFixedShape();
	}
}




void hkFlattenShapeHierarchyUtil::verifyAllShapes(const hkpWorld* m_pWorld)
{
	hkpPhysicsSystem* system = m_pWorld->getWorldAsOneSystem();

	for (int i = 0; i < system->getRigidBodies().getSize(); i++)
	{
		hkpRigidBody* body = system->getRigidBodies()[i];
		
		verifyShapeOfBody(body);
	}

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
