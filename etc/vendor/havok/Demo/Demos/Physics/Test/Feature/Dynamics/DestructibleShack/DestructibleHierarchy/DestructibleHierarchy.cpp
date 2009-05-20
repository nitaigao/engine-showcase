/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#include <Demos/demos.h>




#include <Demos/Physics/Test/Feature/Dynamics/DestructibleShack/DestructibleHierarchy/DestructibleHierarchy.h>
#include <Demos/Physics/Test/Feature/Dynamics/DestructibleShack/ShapeHierarchyUtils/hkFlattenShapeHierarchyUtil.h>

#include <Graphics/Common/DisplayObject/hkgDisplayObject.h>
#include <Graphics/Common/Geometry/VertexSet/hkgVertexSet.h>

#include <Graphics/Bridge/DisplayHandler/hkgDisplayHandler.h>

// Shapes used for building a shack
#include <Physics/Collide/Shape/Convex/Box/hkpBoxShape.h>
#include <Physics/Collide/Shape/Compound/Collection/List/hkpListShape.h>
#include <Physics/Collide/Shape/Misc/Transform/hkpTransformShape.h>
#include <Physics/Collide/Shape/Compound/Collection/ExtendedMeshShape/hkpExtendedMeshShape.h>

#include <Physics/Internal/Collide/Mopp/Machine/hkpMoppMachine.h>
#include <Physics/Collide/Shape/Compound/Tree/Mopp/Modifiers/hkpRemoveTerminalsMoppModifier.h>

hkArray<DestructibleHierarchy::NodeInformation>* DestructibleHierarchy::s_nodeInfos = HK_NULL;





hkpShape* DestructibleHierarchy::buildShape(int rootNodeIdx)
{
	if (rootNodeIdx == INVALID_NODE_INDEX)
	{
		rootNodeIdx = m_rootNodeIdx;
	}

	const DestructibleHierarchy::Node& node = m_nodes[rootNodeIdx];

	hkArray<hkpShape*> shapes;
	if (node.m_shape)
	{
		shapes.pushBack(node.m_shape);
		node.m_shape->addReference();
	}
	for (int c = 0; c < node.m_childrenIdx.getSize(); c++)
	{
		hkpShape* childShape = buildShape(node.m_childrenIdx[c]);
		if (childShape)
		{
			shapes.pushBack( childShape );
		}
	}

	if (shapes.getSize())
	{

		hkpShape* combinedShape = new hkpListShape(shapes.begin(), shapes.getSize());
		for (int s = 0; s < shapes.getSize(); s++)
		{
			shapes[s]->removeReference();
		}

		// all this under transform
		hkpShape* transformed = new hkpTransformShape(combinedShape, node.m_transform);
		combinedShape->removeReference();
		return transformed;
	}

	return HK_NULL;
}

hkpRigidBody* DestructibleHierarchy::buildRigidBody(const hkpShape* shape, DestructibleHierarchy* hierarchy, int nodeIdx)
{
	hkpRigidBodyCinfo info;
	info.m_shape = shape;
	
	hkpMassProperties massProperties;
	hkpInertiaTensorComputer::computeShapeVolumeMassProperties(shape, 1000.0f, massProperties);
	
	info.m_centerOfMass = massProperties.m_centerOfMass;
	info.m_inertiaTensor.setMul(massProperties.m_volume, massProperties.m_inertiaTensor);
	info.m_mass = massProperties.m_volume * massProperties.m_mass;

	info.m_motionType = hkpMotion::MOTION_BOX_INERTIA;
	info.m_qualityType = HK_COLLIDABLE_QUALITY_DEBRIS;

		// Many broken off elements will have thin box inertia, so we limit their angular vel.
	//info.m_maxAngularVelocity = 5.0f;

	hkpRigidBody* body = new hkpRigidBody(info);

	if (nodeIdx == INVALID_NODE_INDEX)
	{
		nodeIdx = hierarchy->m_rootNodeIdx;
	}
	hkpRigidBody*& bodyPtr = hierarchy->m_nodes[nodeIdx].m_body;

	HK_ASSERT2(0xad7899dd, bodyPtr == HK_NULL, "Overwriting body pointer");
	bodyPtr = body;
	body->addReference();

	return body;
}

void DestructibleHierarchy::updateShapeOfRigidBody(const hkpShape* shape, hkpRigidBody* body)
{
	hkpMassProperties massProperties;
	hkpInertiaTensorComputer::computeShapeVolumeMassProperties(shape, 1000.0f, massProperties);

	body->setCenterOfMassLocal(massProperties.m_centerOfMass);
	body->setMass(massProperties.m_volume * massProperties.m_mass);
	hkMatrix3 tmp; tmp.setMul(massProperties.m_volume, massProperties.m_inertiaTensor);
	body->setInertiaLocal(tmp);

	body->setShape(const_cast<hkpShape*>(shape));
}

//////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////



void DestructibleHierarchy::applyDamage(int nodeIdx, hkReal impulse, hkArray<DestructibleHierarchyCollisionEvent>& collisionEvents)
{
	// decrease health of nodes and build list of nodes to break off

	DestructibleHierarchy::Node* node = &m_nodes[nodeIdx];

	//while (node->m_parentIdx != INVALID_NODE_INDEX)
	//while (or health is zero) or
	while (node->m_body == HK_NULL)
	{
		node->m_health = hkMath::max2(0.0f, node->m_health - impulse);
		if (node->m_health  == 0.0f)
		{
			// we don't reset parent to null..

			breakOffNode(nodeIdx, collisionEvents);

			return;
		}
		nodeIdx = node->m_parentIdx;
		node = &m_nodes[nodeIdx];
		//impulse *= 0.5f;
	}

}


void DestructibleHierarchy::breakOffNode(int nodeIdx, hkArray<DestructibleHierarchyCollisionEvent>& collisionEvents)
{
	Node* node = &m_nodes[nodeIdx];

	int immediateParent = node->m_parentIdx;
	HK_ASSERT2(0xad78d9dd, immediateParent != INVALID_NODE_INDEX, "where's the parent?");

	// detach from parent
	Node* immediateParentNode = &m_nodes[immediateParent];
	HK_ON_DEBUG( int initChildCount = immediateParentNode->m_childrenIdx.getSize() );
	for (int c = 0; c < immediateParentNode->m_childrenIdx.getSize(); c++)
	{
		if (immediateParentNode->m_childrenIdx[c] == nodeIdx)
		{
			immediateParentNode->m_childrenIdx.removeAtAndCopy(c);
			break;
		}
	}
	HK_ASSERT2(0xad78ddaa, initChildCount-1 == immediateParentNode->m_childrenIdx.getSize(), "child not detached!" );

	hkTransform accumulatedTransform = immediateParentNode->m_transform;

	// Find parent rigid body..
	int parentIdx = immediateParent;
	Node* parentNode = immediateParentNode;
	while(parentNode->m_body == HK_NULL)
	{
		parentIdx = parentNode->m_parentIdx;
		parentNode = &m_nodes[parentIdx];
		hkTransform tmp; tmp.setMul(parentNode->m_transform, accumulatedTransform);
		accumulatedTransform = tmp;
		HK_ASSERT2(0xad678daa, parentNode, "No parent rigid body found!");
	}

	// Create new body
	hkpRigidBody* newBody;
	{
		hkpShape* shape = buildShape(nodeIdx);
		hkpShape* flatShape = hkFlattenShapeHierarchyUtil::flattenHierarchy(shape);
		shape->removeReference();
		newBody = DestructibleHierarchy::buildRigidBody(flatShape, this, nodeIdx);
		flatShape->removeReference();

		const hkSmallArray< hkpCollisionListener* >& listeners = parentNode->m_body->getCollisionListeners();
		for (int i = 0; i < listeners.getSize(); i++)
		{
			newBody->addCollisionListener(listeners[i]);
		}

	}
	// init velocites for new body
	{
		// reposition the body
		hkTransform parentTransform;
		if (parentNode->m_initialTransformOfHierarchy)
		{
			parentTransform = *parentNode->m_initialTransformOfHierarchy;
		}
		else
		{
			parentTransform = parentNode->m_body->getTransform();
		}
		hkTransform newBodyTransform; newBodyTransform.setMul( parentTransform, accumulatedTransform );
		newBody->setTransform(newBodyTransform);

		// compute velocities
		hkVector4 linVel = parentNode->m_body->getLinearVelocity();
		hkVector4 angVel = parentNode->m_body->getAngularVelocity();
		hkVector4 relCm; relCm.setSub4(newBody->getCenterOfMassInWorld(), parentNode->m_body->getCenterOfMassInWorld());
		hkVector4 extraLin; extraLin.setCross(angVel, relCm);
		linVel.add4(extraLin);
		newBody->setLinearVelocity( linVel );
		newBody->setAngularVelocity( angVel );

	}

	// set newBody  position
	parentNode->m_body->getWorld()->addEntity(newBody);
	newBody->removeReference();
	newBody = HK_NULL;

	// Update shape of parent body
	if (!parentNode->m_body->isFixed())
	{
		hkpShape* shape = buildShape(parentIdx);
		if (shape)
		{
			hkVector4 oldCm = parentNode->m_body->getCenterOfMassInWorld();
			hkpShape* flatShape = hkFlattenShapeHierarchyUtil::flattenHierarchy(shape);
			updateShapeOfRigidBody(flatShape, parentNode->m_body);
			shape->removeReference();
			flatShape->removeReference();

			hkVector4 relCm; relCm.setSub4(parentNode->m_body->getCenterOfMassInWorld(), oldCm);
			hkVector4 extraLin; extraLin.setCross(parentNode->m_body->getAngularVelocity(), relCm);

			hkVector4 linVel; linVel.setAdd4(parentNode->m_body->getLinearVelocity(), extraLin);
			parentNode->m_body->setLinearVelocity(linVel);
		}
		else 
		{
			parentNode->m_body->getWorld()->removeEntity(parentNode->m_body);
			parentNode->m_body->removeReference();
			parentNode->m_body = HK_NULL;
		}
	}
	else // if (!parentNode->m_body->isFixed())
	{

		// if we're breaking off of a fixed shape -- this must be the one fixed mopp shape

		const hkpShape* shape = parentNode->m_body->getCollidable()->getShape();
		HK_ASSERT2(0xad1788dd, shape->getType() == HK_SHAPE_MOPP, "The fixed body must have a mopp.");
		const hkpMoppBvTreeShape* mopp = static_cast<const hkpMoppBvTreeShape*>(shape);

		// Remove shapeKeys from mopp

		HK_ACCESS_CHECK_OBJECT(parentNode->m_body->getWorld(), HK_ACCESS_RW );

		hkArray<hkpShapeKey> brokenOffShapeKeys;
		collectShapeKeys(nodeIdx, brokenOffShapeKeys);

		for (int i = brokenOffShapeKeys.getSize()-1; i >=0; i--)
		{
			if(brokenOffShapeKeys[i] == HK_INVALID_SHAPE_KEY)
			{
				brokenOffShapeKeys.removeAt(i);
			}
			else
			{
				const hkpMoppBvTreeShape* moppShape = static_cast<const hkpMoppBvTreeShape*>( parentNode->m_body->getCollidable()->getShape() );
				removeSubShapeFromDisplay(parentNode->m_body, const_cast<hkpMoppBvTreeShape*>(moppShape), brokenOffShapeKeys[i]);
			}
		}
		hkpRemoveTerminalsMoppModifier modifier(mopp->getMoppCode(), mopp->getShapeCollection(), brokenOffShapeKeys);
		modifier.applyRemoveTerminals( const_cast<hkpMoppCode*>(mopp->getMoppCode()) );

		// disable contact points for the removed shapes..
		hkPointerMap<hkpShapeKey, int> shapeKeyMap;
		shapeKeyMap.reserve(brokenOffShapeKeys.getSize());
		for (int k = 0; k < brokenOffShapeKeys.getSize(); k++)
		{
			shapeKeyMap.insert(brokenOffShapeKeys[k], 0);
		}

		for (int e = 0; e < collisionEvents.getSize(); e++)
		{
			if (collisionEvents[e].m_contactMgr)
			{
				hkpShapeKey key = collisionEvents[e].m_shapeKey;

				hkPointerMap<hkpShapeKey, int>::Iterator it = shapeKeyMap.findKey(key);
				if (shapeKeyMap.isValid(it) && collisionEvents[e].m_body == parentNode->m_body)
				{
					static_cast<hkpDynamicsContactMgr*>(collisionEvents[e].m_contactMgr)->getContactPoint(collisionEvents[e].m_contactPointId)->setDistance(100000.0f);
					collisionEvents.removeAt(e);
					e--;
				}
			}
			else
			{
				collisionEvents.removeAt(e);
				e--;
			}
		}

	}



}


void DestructibleHierarchy::collectShapeKeys(int nodeIdx, hkArray<hkpShapeKey>& shapeKeys)
{
	Node* node = &m_nodes[nodeIdx];

	if (node->m_shapeKey)
	{
		shapeKeys.pushBack(node->m_shapeKey);
	}

	for (int c = 0; c < node->m_childrenIdx.getSize(); c++)
	{
		collectShapeKeys(node->m_childrenIdx[c], shapeKeys);
	}
}



//////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////



hkUint16 DestructibleHierarchy::getLookupIndexOfNewDestructibleNode(DestructibleHierarchy* hierarchy, int nodeIdxInDestructibleHierarchy)
{
	HK_ASSERT2(0xad678bad, s_nodeInfos, "Array not created.");
	if (s_nodeInfos->isEmpty())
	{
		// dummy node, so that index of valid nodes in non-zero
		s_nodeInfos->expandOne();
	}

	unsigned int lookupIdx = s_nodeInfos->getSize();

	NodeInformation& info = s_nodeInfos->expandOne();
	info.m_destructibleHierarchy = hierarchy;
	info.m_nodeIdx = nodeIdxInDestructibleHierarchy;
	HK_ASSERT2(0xad6288dd, s_nodeInfos->getSize() <= (1<<16), "Too many destructible nodes in the lookup table -- we only have 16 bits for the index into the table" );

	return hkUint16(lookupIdx);
}

hkResult DestructibleHierarchy::updateRigidBodyOfDestructibleHierarchy(DestructibleHierarchy* hierarchy, unsigned int nodeIdx, hkpRigidBody* newOwningBody)
{
	Node* node = &hierarchy->m_nodes[nodeIdx];

	while(node->m_body == HK_NULL)
	{
		HK_ASSERT2(0xad789aae, node->m_parentIdx != INVALID_NODE_INDEX, "could not find the parent hkpRigidBody.");
		node = &hierarchy->m_nodes[node->m_parentIdx];
	}

//!! update for Evos
	if (node->m_body)
	{
		HK_ASSERT2(0xad789aee, node->m_initialTransformOfHierarchy == HK_NULL, "this body was alredy reassigned to a different hkpRigidBody once..");
		node->m_initialTransformOfHierarchy = new hkTransform(node->m_body->getTransform());

		node->m_body->removeReference();
		node->m_body = newOwningBody;
		node->m_body->addReference();
		return HK_SUCCESS;
	}

	HK_ASSERT2(0xad768dda, false, "Could not find the rigid body a hierarchy");
	return HK_FAILURE;
}



void DestructibleHierarchy::insertShapeKeysIntoDestructibleHierarchy(hkpRigidBody* fixedBody)
{
	const hkpShape* shape = fixedBody->getCollidable()->getShape();
	HK_ASSERT2(0xad6738dd, shape->getType() == HK_SHAPE_MOPP, "The fixed body must have a mopp.");
	const hkpMoppBvTreeShape* mopp = static_cast<const hkpMoppBvTreeShape*>(shape);

	hkArray<hkpShapeKey> shapeKeys;
	hkMoppFindAllVirtualMachine_getAllKeys(mopp->getMoppCode(), &shapeKeys );

	// ultimately we want to skip looking through triangles here..

	hkpShapeCollection::ShapeBuffer shapeBuffer;
	//hkpShape* childShape = reinterpret_cast<hkpShape*>(&shapeBuffer);
	for (int k = 0; k < shapeKeys.getSize(); k++)
	{
		hkpShapeKey shapeKey = shapeKeys[k];
		const hkpShape* childShape = mopp->getShapeCollection()->getChildShape(shapeKey, shapeBuffer);
		hkUint16 lookupIdx = hkUint16((hkUlong(childShape->getUserData())>>16) & 0xffff);

		if (childShape->getUserData())
		{
			int oyo  = 0;
			oyo++;
		}

		if (lookupIdx)
		{
			DestructibleHierarchy* hierarchy = DestructibleHierarchy::getDestructibleHierarchy(lookupIdx);
			unsigned int nodeIdx = DestructibleHierarchy::getNodeIndex(lookupIdx);

			DestructibleHierarchy::Node* node = &hierarchy->m_nodes[nodeIdx];
			node->m_shapeKey = shapeKey;
		}
	}
}




//
//
// Display
//
//

void DestructibleHierarchy::removeSubShapeFromDisplay(hkpRigidBody* moppRigidBody, hkpMoppBvTreeShape* moppShape, int subShapeKey)
{
	if (!m_env)
	{
		return;
	}

	//
	// calculate the shape's index in the hkgDisplayObject's geometry array
	//
	int geometryIndex;
	{
		hkpShapeCollection::ShapeBuffer buffer;
		const hkpExtendedMeshShape* meshShape = static_cast<const hkpExtendedMeshShape*>( moppShape->getContainer()->getChildShape(0, buffer) );

		hkpExtendedMeshShape::SubpartType type = meshShape->getSubpartType(subShapeKey);
		int subpartIndex = meshShape->getSubPartIndex(subShapeKey);

		geometryIndex = 0;

		if ( type == hkpExtendedMeshShape::SUBPART_TRIANGLES )
		{
			geometryIndex = subpartIndex;
		}
		else // type == hkpExtendedMeshShape::SUBPART_SHAPE
		{
			// skip all triangle subparts
			geometryIndex += meshShape->getNumTrianglesSubparts();

			// loop over all previous shape subparts
			{
				for (int i=0; i<subpartIndex; i++)
				{
					geometryIndex += meshShape->getShapesSubpartAt(i).m_numChildShapes;
				}
			}
			geometryIndex += meshShape->getTerminalIndexInSubPart(subShapeKey);
		}
	}

	//
	// get the terrain's vertex set
	//
	hkgVertexSet* vertexSet;
	{
		const hkpCollidable* collidable = moppRigidBody->getCollidable();
		hkgDisplayObject* displayObject = m_env->m_displayHandler->findDisplayObject(hkUlong(collidable));
		hkgGeometry* geometry = displayObject->getGeometry(geometryIndex);
		hkgFaceSet* faceSet = geometry->getMaterialFaceSet(0)->getFaceSet(0);
		vertexSet = faceSet->getVertexSet();
	}

	m_env->m_window->getContext()->lock();

	// vertex set has to be locked before any vertices can be manipulated
	vertexSet->lock(HKG_LOCK_WRITEDISCARD);

	{
		//
		// Simply zero-out all vertices. This will degenerate the triangles but that should be ok.
		//
		hkVector4 newPos(0,0,0);
		for (int i = 0; i < vertexSet->getNumVerts(); i++)
		{
			vertexSet->setVertexComponentData(HKG_VERTEX_COMPONENT_POS, i, &newPos(0));
		}
	}

	// unlock the vertex set again
	vertexSet->unlock();

	m_env->m_window->getContext()->unlock();

	return;
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
