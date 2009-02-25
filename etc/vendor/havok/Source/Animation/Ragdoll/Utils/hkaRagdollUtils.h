/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2008 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef INC_HKRAGDOLL_UTILS_H
#define INC_HKRAGDOLL_UTILS_H

#include <Common/Base/hkBase.h>

class hkpRigidBody;
class hkpConstraintInstance;
class hkaSkeleton;
class hkaRagdollInstance;

/// The static methods in this utility class provide functionality in order to create and manipulate ragdoll instances
class hkaRagdollUtils
{
public:

		/// Reorders the rigid bodies (parent first) and the constaints
	static hkResult HK_CALL reorderForRagdoll (hkArray<hkpRigidBody*> &rigidBodiesInOut, hkArray<hkpConstraintInstance*> &constraintsInOut);
	
		/// Reorders the rigid bodies (parent first) and aligns their pivots to the constraints spaces
		/// The requirements for this to succeed are:.
		/// a - There is n rigid bodies and n-1 constraints
		/// b - Constraints are properly parented (rigid body a -> child bone, rigid body b -> parent bone).
		/// c - Constraints are properly placed at the joints.
		/// If you give a skeleton it will make sure that the bodies are in the same order as the skeleton bones
		/// If warnOnAlignment is set to true, a warning will be raised whenever a rigid body had to be re-aligned
	static hkResult HK_CALL reorderAndAlignForRagdoll (hkArray<hkpRigidBody*>& rigidBodiesInOut, hkArray<hkpConstraintInstance*>& constraintsInOut, hkBool warnOnAlignment = false);

		/// Given a physics system representing a ragdoll, constructs an hkaSkeleton describing its structure.
		/// It assumes the physics system has been properly set up using "reorderAndAlignForRagdoll".
		/// Note that in the creation of a skeleton some memory gets allocated and needs to be deallocated properly by calling the
		/// utility method "destroySkeleton".
	static hkaSkeleton* HK_CALL constructSkeletonForRagdoll (const hkArray<hkpRigidBody*> &rigidBodies, const hkArray<hkpConstraintInstance*> &constraints);

		/// Deallocates any memory allocated by constructSkeletonForagdoll().
	static void HK_CALL destroySkeleton (hkaSkeleton* skeleton);

		/// Constructs a ragdoll instance based on the given skeleton.
		/// It takes the rigid bodies and constraints from the "candidate" lists passed as a parameter.
		/// Returns HK_NULL on failure.
	static class hkaRagdollInstance* createRagdollInstanceFromSkeleton (const hkaSkeleton* skeleton, const hkArray<hkpRigidBody*>& candidateRBs, const hkArray<hkpConstraintInstance*>& candidateConstraints);

};

#endif //INC_HKRAGDOLL_UTILS_H

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
