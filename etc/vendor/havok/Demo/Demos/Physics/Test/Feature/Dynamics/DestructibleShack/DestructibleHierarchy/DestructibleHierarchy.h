/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


#ifndef HK_DestructibleHierarchy_H
#define HK_DestructibleHierarchy_H



#include <Demos/DemoCommon/DemoFramework/hkDefaultPhysicsDemo.h>

#include <Common/Base/Container/Array/hkObjectArray.h>

#include <Demos/Physics/Test/Feature/Dynamics/DestructibleShack/DestructibleHierarchy/DestructibleHierarchyListener.h>


class DestructibleHierarchy
{
	public:
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO, DestructibleHierarchy);

		enum NodeIdx
		{
			INVALID_NODE_INDEX = -1
		};

		class Node
		{
			public:

				HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO, Node);

				Node() : m_body(HK_NULL), m_initialTransformOfHierarchy(HK_NULL), m_shape(HK_NULL), m_transform(hkTransform::getIdentity()), m_shapeKey(HK_INVALID_SHAPE_KEY), m_parentIdx(INVALID_NODE_INDEX), m_health(1000)
				{
				}

				~Node()
				{
					if (m_body) {m_body->removeReference();}
					if (m_initialTransformOfHierarchy) {delete m_initialTransformOfHierarchy;}
					if (m_shape) {m_shape->removeReference();}
				}

					/// Set to non-null value only for the top element of the hierarchy, 
					/// or the top element of a disconnected hierarchy subtree.
				hkpRigidBody* m_body;

					/// This is used when the hierarchy's body is inserted into mopp.
				hkTransform* m_initialTransformOfHierarchy;

					/// Shape represented by this node.
				hkpShape* m_shape;

					/// Transform relative to parent node.
				hkTransform m_transform;

					/// Shape key used to reference the shape when it is in destructible mopp.
				hkpShapeKey m_shapeKey;

					/// All child nodes of the current one.
					/// When a hierarchy is disjoint, the disjoint child is removed from the list.
				hkArray<int> m_childrenIdx;

					/// Parent of this node. This is not set to null, if this node is disconnected from 
					/// the parent. That is to allow to calculate the transform from the root
				int m_parentIdx;

					/// Health of the node. When the value drops below 0, the hierarchy subtree is broken off;
				hkReal m_health;

		};

		hkArray<Node> m_nodes;

		int m_rootNodeIdx;

		
		hkpShape* buildShape(int rootNodeIdx = INVALID_NODE_INDEX); 

		static hkpRigidBody* buildRigidBody(const hkpShape* shape, DestructibleHierarchy* hierarchy, int nodeIdx = INVALID_NODE_INDEX);

		static void updateShapeOfRigidBody(const hkpShape* shape, hkpRigidBody* body);

		//
		// applying damage
		//
		
		void applyDamage(int nodeIdx, hkReal impulse, hkArray<DestructibleHierarchyCollisionEvent>& collisionEvents);

		void breakOffNode(int nodeIdx, hkArray<DestructibleHierarchyCollisionEvent>& collisionEvents);

		void collectShapeKeys(int nodeIdx, hkArray<hkpShapeKey>& shapeKeys);




		//
		//
		//
		struct NodeInformation
		{
			DestructibleHierarchy* m_destructibleHierarchy;
			unsigned int m_nodeIdx;
		};

		// There must be a dummy info in here (as zero denotes a non-destructible object)
		static hkArray<NodeInformation>* s_nodeInfos;

	public:
		static inline void initStaticNodeLookupArray();

		static inline void destroyStaticNodeLookupArray();

		static inline unsigned int getNodeIndex(hkUint16 lookupTableIndex);

		static inline DestructibleHierarchy* getDestructibleHierarchy(hkUint16 lookupTableIndex);

			// This gets the index of NodeInformation in s_nodeInfos for a newly created node.
			// This value is to be stored in shapeUserData's upper 16 bits.
		static hkUint16 getLookupIndexOfNewDestructibleNode(DestructibleHierarchy* hierarchy, int nodeIdxInDestructibleHierarchy);


		//
		//
		//


		static void insertShapeKeysIntoDestructibleHierarchy(hkpRigidBody* fixedBody);

		static hkResult updateRigidBodyOfDestructibleHierarchy(DestructibleHierarchy* hierarchy, unsigned int nodeIdx, hkpRigidBody* newOwningBody);


		class hkDemoEnvironment* m_env;
		void removeSubShapeFromDisplay(hkpRigidBody* moppRigidBody, class hkpMoppBvTreeShape* moppShape, int subShapeKey);

};

#include <Demos/Physics/Test/Feature/Dynamics/DestructibleShack/DestructibleHierarchy/DestructibleHierarchy.inl>


#endif // HK_DestructibleHierarchy_H


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
