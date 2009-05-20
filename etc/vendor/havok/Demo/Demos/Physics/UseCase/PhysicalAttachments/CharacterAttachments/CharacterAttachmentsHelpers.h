/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef CHARACTER_ATTACHMENTS_HELPERS_H
#define CHARACTER_ATTACHMENTS_HELPERS_H

#include <Demos/DemoCommon/DemoFramework/hkDefaultPhysicsDemo.h>
#include <Common/Serialize/Packfile/hkPackfileReader.h>

#include <Animation/Animation/Rig/hkaPose.h>

#include <Demos/DemoCommon/Utilities/GameUtils/CollisionFilterViewerUtil/CollisionFilterViewerUtil.h>

class hkaRagdollInstance;
class hkaAnimatedSkeleton;
class hkaSkeletonMapper;


class CharacterAttachmentsHelpers
{
	public:

		struct ConstraintStabilityTricks
		{
			ConstraintStabilityTricks()
			{
				m_useLimits = false;
				m_useDamping = false;
				m_useChain = false;
			}

			hkBool m_useLimits;
			hkBool m_useDamping;
			hkBool m_useChain;
		};

	public:

		// Create a ragdoll, including skeleton, aligning constraints, and stabilizing inertia tensors.
		static void HK_CALL createRagdollFromBodiesAndConstraints(const hkpPhysicsSystem* physicsSystem, const hkpGroupFilter* filter,
																	hkaSkeleton*& ragdollSkeletonOut,
																	hkaRagdollInstance*& ragdollInstanceOut);

		// Setup the high (aniamtion) to low (ragdoll) mapping based on the skeltons and a (hard-coded for this demo) naming convention.
		static hkaSkeletonMapper* HK_CALL setupRagdollMapping(const hkaSkeleton* animationSkeleton, const hkaSkeleton* ragdollSkeleton);

		// Set all rigid bodies to a given motion type.
		static void HK_CALL setRagdollMotionType(hkaRagdollInstance* ragdollInstance, hkpMotion::MotionType motionType);

		// Helpers for each of the three example constraint systems we add.
		static void HK_CALL addPowerline(hkpWorld* world, const hkaRagdollInstance* ragdollInstance, const hkQsTransform& currentTransform, const char* startBoneName, const char* endBoneName, hkpGroupFilter* filter, const ConstraintStabilityTricks& tricks  );
		static void HK_CALL addScabbard(hkpWorld* world, const hkaRagdollInstance* ragdollInstance, const hkQsTransform& currentTransform,const char* startBoneName, hkpGroupFilter* filter, const ConstraintStabilityTricks& tricks  );
		static void HK_CALL addBackpack(hkpWorld* world, const hkaRagdollInstance* ragdollInstance, const hkQsTransform& currentTransform,const char* startBoneName, hkpGroupFilter* filter, const ConstraintStabilityTricks& tricks );

	protected:

		// Helpers for string compare for bone matching.
		static void HK_CALL stripPrefix( hkString& strInOut );
		static int HK_CALL compareWithoutPrefix( const char* str1, const char* str2 );

};

#endif // CHARACTER_ATTACHMENTS_HELPERS_H

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
