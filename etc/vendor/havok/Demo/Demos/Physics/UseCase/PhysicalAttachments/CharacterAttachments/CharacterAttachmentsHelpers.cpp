/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#include <Demos/demos.h>

#include <Demos/Physics/UseCase/PhysicalAttachments/CharacterAttachments/CharacterAttachmentsHelpers.h>

// Ragdoll stuff
#include <Animation/Ragdoll/Utils/hkaRagdollUtils.h>
#include <Animation/Ragdoll/Instance/hkaRagdollInstance.h>

// Graphics Stuff
#include <Graphics/Common/DisplayObject/hkgDisplayObject.h>
#include <Common/Visualize/hkDebugDisplay.h>

// Skeletal Animation
#include <Animation/Animation/Rig/hkaPose.h>
#include <Animation/Animation/Playback/hkaAnimatedSkeleton.h>
#include <Animation/Animation/Mapper/hkaSkeletonMapperUtils.h>
#include <Animation/Animation/Mapper/hkaSkeletonMapper.h>

// Physics stuff
#include <Physics/Utilities/Constraint/Keyframe/hkpKeyFrameUtility.h>
#include <Physics/Utilities/Collide/Filter/GroupFilter/hkpGroupFilterUtil.h>

// Constraint Includes
#include <Physics/Dynamics/Constraint/Chain/BallSocket/hkpBallSocketChainData.h>
#include <Physics/Dynamics/Constraint/Chain/hkpConstraintChainInstance.h>
#include <Physics/Dynamics/Constraint/Chain/RagdollLimits/hkpRagdollLimitsData.h>
#include <Physics/Dynamics/Constraint/Bilateral/LimitedHinge/hkpLimitedHingeConstraintData.h>
#include <Physics/Dynamics/Constraint/Bilateral/Ragdoll/hkpRagdollConstraintData.h>
#include <Physics/Dynamics/Constraint/Bilateral/BallAndSocket/hkpBallAndSocketConstraintData.h>


void CharacterAttachmentsHelpers::createRagdollFromBodiesAndConstraints(const hkpPhysicsSystem* physicsSystem,
																 const hkpGroupFilter* filter,
																 hkaSkeleton*& ragdollSkeletonOut, 
																 hkaRagdollInstance*& ragdollInstanceOut)
{
	// Reordering constraints and bodies - need non-const arrays of bodies which will initially fill with the loaded-in bodies
	hkArray<hkpRigidBody*> rigidBodies;
	rigidBodies = physicsSystem->getRigidBodies();

	hkArray<hkpConstraintInstance*> newConstraints;
	newConstraints = physicsSystem->getConstraints();
	
	{
		//
		// Reorder and align
		//
		hkaRagdollUtils::reorderAndAlignForRagdoll(rigidBodies, newConstraints);
	
		//
		// Make a ragdoll (animation) skeleton/rig hierarchy from the bodies and constraints
		//
		ragdollSkeletonOut = hkaRagdollUtils::constructSkeletonForRagdoll(rigidBodies, newConstraints);
	
		//
		// Set up the system groups for the filtering to ensure adjacent bodies don't collide
		//
		hkpRigidBody* testRigidBody = rigidBodies[0];
		const int systemGroup = hkpGroupFilter::getSystemGroupFromFilterInfo(testRigidBody->getCollidable()->getCollisionFilterInfo());
		hkpGroupFilterUtil::disableCollisionsBetweenConstraintBodies( newConstraints.begin(),newConstraints.getSize(), systemGroup);
	}

	ragdollInstanceOut = new hkaRagdollInstance(rigidBodies, newConstraints, ragdollSkeletonOut);

	// Further filter setup: Move all bodies to layer 1 so we can later enable collisions with the attachments
	for (hkInt16 i=0; i< ragdollInstanceOut->getNumBones(); i++)
	{
		hkpRigidBody* rb = ragdollInstanceOut->getRigidBodyOfBone( i );
		hkUint32 filterInfo = rb->getCollidable()->getCollisionFilterInfo();

		filterInfo = hkpGroupFilter::calcFilterInfo(1, 
			hkpGroupFilter::getSystemGroupFromFilterInfo(filterInfo),
			hkpGroupFilter::getSubSystemIdFromFilterInfo(filterInfo),
			hkpGroupFilter::getSubSystemDontCollideWithFromFilterInfo(filterInfo)
			);
		rb->setCollisionFilterInfo(filterInfo);
	}
	

	// This routine iterates through the bodies pointed to by the constraints and stabilizes their inertias.
	// This makes both ragdoll controllers less sensitive to angular effects and hence more effective
	const hkArray<hkpConstraintInstance*>& constraints = ragdollInstanceOut->getConstraintArray();
	hkpInertiaTensorComputer::optimizeInertiasOfConstraintTree( constraints.begin(), constraints.getSize(), ragdollInstanceOut->getRigidBodyOfBone(0) );

}

// Helper to loop over all bodies in ragdoll
void CharacterAttachmentsHelpers::setRagdollMotionType(hkaRagdollInstance* ragdollInstance, hkpMotion::MotionType motionType)
{
	for( int b=0; b < ragdollInstance->getNumBones(); b++)
	{
		hkpRigidBody* rb = ragdollInstance->getRigidBodyOfBone( b );
		rb->setMotionType( motionType );
	}
}

hkaSkeletonMapper* CharacterAttachmentsHelpers::setupRagdollMapping(const hkaSkeleton* animationSkeleton, const hkaSkeleton* ragdollSkeleton)
{
	// Mapping between both skeletons.

	// Lock the translation of most bones of the animations, except pelvis and thigh - these may have animated translation.
	{
		// Locks all translations, except root.
		hkaSkeletonUtils::lockTranslations(*animationSkeleton, true);

		// Unlocks translation for pelvis and both thighs (triangle pelvis).
		const hkInt16 pelvis = hkaSkeletonUtils::findBoneWithName( *animationSkeleton, "Ragdoll Pelvis", compareWithoutPrefix );
		animationSkeleton->m_bones[pelvis]->m_lockTranslation = false;

		const hkInt16 lthigh = hkaSkeletonUtils::findBoneWithName( *animationSkeleton, "Ragdoll L_Thigh", compareWithoutPrefix );
		animationSkeleton->m_bones[lthigh]->m_lockTranslation = false;

		const hkInt16 rthigh = hkaSkeletonUtils::findBoneWithName( *animationSkeleton, "Ragdoll R_Thigh", compareWithoutPrefix );
		animationSkeleton->m_bones[rthigh]->m_lockTranslation = false;
	}

	// Create mapper.
	{
		hkaSkeletonMapperData high_low_data;
		hkaSkeletonMapperData low_high_data;
		{
			hkaSkeletonMapperUtils::Params params;

			params.m_skeletonA = animationSkeleton;
			params.m_skeletonB = ragdollSkeleton;
			params.m_compareNames = compareWithoutPrefix;

			// Explicit mappings.
			// Re-parent the forearm's twists to the forearm.
			{
				hkaSkeletonMapperUtils::UserMapping mapping;
				mapping.m_boneIn = "HavokBipedRig L_ForeArm";
				mapping.m_boneOut = "Ragdoll L_ForeTwist";
				params.m_userMappingsBtoA.pushBack(mapping);
				mapping.m_boneIn = "HavokBipedRig R_ForeArm";
				mapping.m_boneOut = "Ragdoll R_ForeTwist";
				params.m_userMappingsBtoA.pushBack(mapping);
			}

			params.m_autodetectChains = false;

			hkaSkeletonMapperUtils::createMapping( params, high_low_data, low_high_data );
		}

		hkaSkeletonMapper* skelmapper = new hkaSkeletonMapper(high_low_data);
		return skelmapper;
	}
}


// The powerline is a *chain* of constraints and will suffer from the following  artifacts if implemented using a
// series of ball-and-socket constraints:
// 1. Stretching when large accelerations are present.
// 2. Collision artifacts or a perforamcne hit) if we allow the entire chain to collide with the ragdoll, or if we
// allow inter-link collisions.
// To address this we wrap the ball-and-socket constraints in a "Chain" hkpConstraintChainInstance constraint, and
// also increase the inertia tensor and angular damping to further hinding excess motion.
// We also disable inter-link collisions and collisions between the end links and the ragdoll - see details below.
void HK_CALL CharacterAttachmentsHelpers::addPowerline(hkpWorld* world, 
												const hkaRagdollInstance* ragdollInstance,
												const hkQsTransform& currentTransform,
												const char* startBoneName, const char* endBoneName, 
												hkpGroupFilter* filter, 
												const ConstraintStabilityTricks& tricks  )
{
	// We will use two tricks to increase stability:
	const hkReal inertiaToMassRatio = tricks.m_useDamping ? 40.0f : 1.0f;
	const hkReal angularDamping = tricks.m_useDamping ? 4.0f: 0.05f;


	// We are going to add a belt of constrained bodies where the 
	// ends of the chain do not collide with anything in the ragdoll layer (L1), 
	// and the chain does not collide with itself.
	// We do the rb-chain collisions with layers (forcing all chain bodies into a different group than the ragdoll bodies).
	// All chain = G3 => collisions with ragdoll (L1) done by layer
	// Ends == Layer 2
	// All other bodies in chain == Layer 3
	// Disable(Layer 1, Layer 2)
	// Enable(Layer 1, Layer 3)
	// and we do the chain-chain collisions with groups:
	// All chain = Group 3, SGroup 0, SGroup No-Collide 0 => collisions disabled
	filter->disableCollisionsBetween(1, 2);
	filter->enableCollisionsBetween(1, 3);

	const hkReal linkSize = 0.055f;
	const int numLinks = 10;

	
	// Get the relevant RBs to which we attach this belt.
	hkpRigidBody* torso = HK_NULL;
	hkpRigidBody* hand = HK_NULL;
	{
		int torsoIndex = hkaSkeletonUtils::findBoneWithName( *ragdollInstance->m_skeleton, startBoneName);
		HK_ASSERT2( 0, torsoIndex >= 0, "Couldn't find " << startBoneName << " in ragdoll" );
		torso = ragdollInstance->getRigidBodyOfBone( torsoIndex );

		int handIndex = hkaSkeletonUtils::findBoneWithName( *ragdollInstance->m_skeleton, endBoneName );
		HK_ASSERT2( 0, handIndex >= 0, "Couldn't find " << endBoneName << " in ragdoll" );		
		hand = ragdollInstance->getRigidBodyOfBone( handIndex );
	}

	// Get a list of bodies for the chain
	hkArray<hkpRigidBody*> bodies;
	{
		// Start with the torso
		bodies.pushBack( torso );

		// Create a bunch of new bodies for the links in the belt
		hkpRigidBodyCinfo info;
		info.m_shape = new hkpSphereShape( linkSize );

		// Note that we use a scaled mass here to artificially damp the movement
		const hkReal mass = 1.0f;
		hkpInertiaTensorComputer::setShapeVolumeMassProperties( info.m_shape, mass * inertiaToMassRatio, info );
		info.m_mass = mass;

		// Note that we use an increased angular damping to further artificially damp the movement
		info.m_angularDamping = angularDamping;

		for( int i = 0; i < numLinks; i++ )
		{
			// Place roughly behind the character model space - we'll let a short simulation run settle the constraint preoperly.
			hkReal iReal = i / (hkReal) numLinks;
			info.m_position.set( -iReal* 1.0f + 0.2f, 0.2f, 0.2f - iReal* 0.2f );	

			// Move into world space behind character
			info.m_position.setTransformedPos(currentTransform, info.m_position);

			// Filter info, see above. End links have layer 2, othwerwise have layer 3.
			// All links have group 3, and subgroup 0, subgroup-nocollide 0
			if(i ==0 || i == numLinks-1)
			{
				info.m_collisionFilterInfo =  hkpGroupFilter::calcFilterInfo( 2, 3, 0, 0 );
			}
			else
			{
				info.m_collisionFilterInfo =  hkpGroupFilter::calcFilterInfo( 3, 3, 0, 0 );
			}

			hkpRigidBody* body = new hkpRigidBody( info );
			bodies.pushBack( body );
			world->addEntity( body );
			body->removeReference();
		}

		info.m_shape->removeReference();

		// End with the hand
		bodies.pushBack( hand );
	}

	
	// Create the chain constraint. This is the 'core' constraint that will link the bodies together in 
	// a chain. Using a single 'chain' constraint rather than n (unrelated) individual constraints will
	// reduce possible stretching dramatically.
	if( tricks.m_useChain )
	{
		hkpBallSocketChainData* chainData = new hkpBallSocketChainData();
		for( int i=0; i<bodies.getSize()-1; ++i )
		{
			hkVector4 pivotA( 0.0f, 0.0f, linkSize*0.75f );
			hkVector4 pivotB( 0.0f, 0.0f, -linkSize*0.75f );

			if( i == 0 ) // torso
			{
				pivotA.set( 0.0f, -0.1f, 0.0f );
			}
			else if( i == bodies.getSize()-2 ) // hand
			{
				pivotB.set( 0.0f, -0.05f, -0.1f );
			}

			chainData->addConstraintInfoInBodySpace( pivotA, pivotB );
		}

		hkpConstraintChainInstance* chain = new hkpConstraintChainInstance( chainData );
		chainData->removeReference();

		for( int i=0; i<bodies.getSize(); ++i )
		{
			chain->addEntity( bodies[i] );
		}

		world->addConstraint( chain );
		chain->removeReference();
	}
	else
	{
		for( int i=0; i<bodies.getSize()-1; ++i )
		{
			hkVector4 pivotA( 0.0f, 0.0f, linkSize*0.75f );
			hkVector4 pivotB( 0.0f, 0.0f, -linkSize*0.75f );

			if( i == 0 ) // torso
			{
				pivotA.set( 0.0f, -0.1f, 0.0f );
			}
			else if( i == bodies.getSize()-2 ) // hand
			{
				pivotB.set( 0.0f, -0.05f, -0.1f );
			}

			hkpBallAndSocketConstraintData* bscd = new hkpBallAndSocketConstraintData();
			bscd->setInBodySpace(pivotA, pivotB);
			hkpConstraintInstance* constraint = new hkpConstraintInstance(bodies[i], bodies[i+1], bscd );
			world->addConstraint(constraint);
			constraint->removeReference();
			bscd->removeReference();
		}
	}
}

// The scabbard is a single constraint which should have multiple degrees of freedom, but will suffer from the following 
// artifacts if implemented using a single ball-and-socket constraint:
// 1. Undesired motion (excessive, potentially implausible, or just not artisitically desired) due to the simplification 
//    to a single point of attachment.
// 2. Excessive energy, or undersired enrgy preservation when the character comes to rest.
// 3. A potential performance impact if collisions are used to prevent interpenetration with the character.
// To address these we use a ragdoll constraint instead which allows full control to restict angular degrees of freedom of the
// underlying ball-and-socket, and we use angular damping to make sure the scabbard comes to rest quickly.
// We can then disable all collision between the scabbard and the hip, leaving all other collisions valid.
void HK_CALL CharacterAttachmentsHelpers::addScabbard(hkpWorld* world, 
												const hkaRagdollInstance* ragdollInstance,
												const hkQsTransform& currentTransform,
												const char* startBoneName, 
												hkpGroupFilter* filter,
												const ConstraintStabilityTricks& tricks )
{
	// We will use a tricks to increase stability:
	const hkReal angularDamping = 4.0f;


	// Get the relevant RBs to which we attach this belt.
	hkpRigidBody* torso = HK_NULL;
	{
		int torsoIndex = hkaSkeletonUtils::findBoneWithName( *ragdollInstance->m_skeleton, startBoneName);
		HK_ASSERT2( 0, torsoIndex >= 0, "Couldn't find " << startBoneName << " in ragdoll" );
		torso = ragdollInstance->getRigidBodyOfBone( torsoIndex );
	}

	hkpRigidBody* scabbard;
	{
		hkVector4 boxHalfExtents( 0.02f, 0.05f, 0.3f);

		// Create body 
		hkpRigidBodyCinfo info;
		info.m_shape = new hkpBoxShape( boxHalfExtents, 0.01f );

		// We are going to add a single body which does not collide with the bone to which it is attached in the ragdoll so we 
		// will put in the same group and disable collision using subgroups. 
		hkUint32 torsoFilterInfo = torso->getCollidable()->getCollisionFilterInfo();
		info.m_collisionFilterInfo = hkpGroupFilter::calcFilterInfo(
			1, 
			2,
			31, // New subgroup - larger than any subgroup in the ragdoll
			hkpGroupFilter::getSubSystemIdFromFilterInfo(torsoFilterInfo) // Don't collide with torso subgroup
			);

		// Note that we use a scaled mass here to artificially damp the movement
		const hkReal mass = 1.0f;
		hkpInertiaTensorComputer::setShapeVolumeMassProperties( info.m_shape, mass, info );
		info.m_mass = mass;

		// Note that we use an increased angular damping to further artificially damp the movement
		if( tricks.m_useDamping ) 
		{
			info.m_angularDamping = angularDamping;
		}

		// Place roughly beside the character model space - we'll let a short simulation run settle the constraint properly.
		info.m_position.set( 0.2f, 0.0f, -0.2f );	

		// Move into world space behind character
		info.m_position.setTransformedPos(currentTransform, info.m_position);

		scabbard = new hkpRigidBody( info );
		info.m_shape->removeReference();

		world->addEntity( scabbard );
		scabbard->removeReference();
	}

	//
	// Create constraint (to attach and also restrict motion)
	// 
	hkVector4 pivotA, pivotB;
	{
		// Scabbard
		pivotA.set(-0.02f, 0.0f, 0.3f);

		// Torso
		pivotB.set(0.0f, 0.0f, 0.2f);
	}

	hkpConstraintData* cd = HK_NULL;

	if( tricks.m_useLimits ) 
	{
		hkReal planeMin =  -HK_REAL_PI * 0.5f;
		hkReal planeMax =  HK_REAL_PI * 0.05f;
		hkReal twistMin =  HK_REAL_PI * -0.04f;
		hkReal twistMax =  HK_REAL_PI *  0.04f;
		hkReal coneMin  =  HK_REAL_PI * 0.3f;

		hkpRagdollConstraintData* rdcd = new hkpRagdollConstraintData();

		rdcd->setConeAngularLimit(coneMin);
		rdcd->setPlaneMinAngularLimit(planeMin);
		rdcd->setPlaneMaxAngularLimit(planeMax);
		rdcd->setTwistMinAngularLimit(twistMin);
		rdcd->setTwistMaxAngularLimit(twistMax);

		// Scabbard
		hkVector4 twistAxisA(0.0f, 0.0f, 1.0f);
		hkVector4 planeAxisA(1.0f, 0.0f, 0.0f);

		// Torso
		hkVector4 twistAxisB(1.0f, 0.0f, 0.0f);
		hkVector4 planeAxisB(0.0f, 0.0f, 1.0f);

		rdcd->setInBodySpace(pivotA, pivotB, planeAxisA, planeAxisB, twistAxisA, twistAxisB);

		cd = rdcd;
	}
	else
	{
		// Just use a ball-and-socket (no angular limits)
		hkpBallAndSocketConstraintData* bscd = new hkpBallAndSocketConstraintData();

		bscd->setInBodySpace(pivotA, pivotB);

		cd = bscd;
	}

	//
	//	Create and add the constraint
	//
	{
		hkpConstraintInstance* constraint = new hkpConstraintInstance(scabbard, torso, cd );
		world->addConstraint(constraint);
		constraint->removeReference();
	}	
	cd->removeReference();
}

// The backpack is a single constraint which should have only one degree of freedom, but will suffer from the following 
// artifacts if implemented using a single hinge constraint:
// 1. Undesired motion (excessive, potentially implausible, or just not artisitically desired) due to the large
//    accelerations of the character.
// 2. A potential performance impact if collisions are used to prevent interpenetration with the character.
// To address this we use a limited hinge constraint which keeps the backpack from penetrating the spline and
// from moving to far upwards.
// The alternative approach of ensuring collisions between the character and the backpack keep it in place has the following flaws:
// (a) Constant collision (such as when the character is standing idle) is expensive, especially if colliding with several bodies
// such as are in the spine - a single constraint with fixed limits avoids this completely with minimal expense.
// (b) Again, without limits, even if the backpack rests against the base of the spine when moving down, there is nothing to prevent
// the large accelerations of the character forcing the backpack to swing up and forward over the shoulders until it collides with
// the back of the head, which is probably not desirable from an artistic standpoint.
void HK_CALL CharacterAttachmentsHelpers::addBackpack(hkpWorld* world, 
												const hkaRagdollInstance* ragdollInstance,
												const hkQsTransform& currentTransform,
												const char* startBoneName, 
												hkpGroupFilter* filter,
												const ConstraintStabilityTricks& tricks  )
{

	// Get the relevant RBs to which we attach this backpack.
	hkpRigidBody* torso = HK_NULL;
	{
		int torsoIndex = hkaSkeletonUtils::findBoneWithName( *ragdollInstance->m_skeleton, startBoneName);
		HK_ASSERT2( 0, torsoIndex >= 0, "Couldn't find " << startBoneName << " in ragdoll" );
		torso = ragdollInstance->getRigidBodyOfBone( torsoIndex );
	}

	hkpRigidBody* backpack;
	{
		hkVector4 boxHalfExtents( 0.1f, 0.05f, 0.2f);

		// Create a body
		hkpRigidBodyCinfo info;
		info.m_shape = new hkpBoxShape( boxHalfExtents, 0.01f );

		// We are going to add a single body which does not collide with the ragdoll *at all* so we 
		// will put in a different group and different layer and disable collision using layers. 
		info.m_collisionFilterInfo = hkpGroupFilter::calcFilterInfo( 2, 3 );
		filter->disableCollisionsBetween(1, 2);
	
		const hkReal mass = 1.0f;
		hkpInertiaTensorComputer::setShapeVolumeMassProperties( info.m_shape, mass, info );
		info.m_mass = mass;

		// Place roughly behind the character model space - we'll let a short simulation run settle the constraint properly.
		info.m_position.set( 0.2f, 0.0f, -0.2f );	

		// Move into world space behind character
		info.m_position.setTransformedPos(currentTransform, info.m_position);

		backpack = new hkpRigidBody( info );
		info.m_shape->removeReference();

		world->addEntity( backpack );
		backpack->removeReference();
	}

	//
	// Create constraint (to attach and also restrict motion)
	// 
	{
		hkpLimitedHingeConstraintData* lhc = new hkpLimitedHingeConstraintData();

		hkVector4 pivotA, pivotB;

		// Backpack
		pivotA.set(0.0f, -0.05f, 0.2f);
		hkVector4 axisA(1.0f, 0.0f, 0.0f);
		hkVector4 axisAPerp(0.0f, 0.0f, 1.0f);

		// Torso
		pivotB.set(0.1f, -0.07f, 0.0f);
		hkVector4 axisB(0.0f, 0.0f, 1.0f);
		hkVector4 axisBPerp(1.0f, 0.0f, 0.0f);

		lhc->setInBodySpace(pivotA, pivotB, axisA, axisB, axisAPerp, axisBPerp);
		lhc->setMinAngularLimit(HK_REAL_PI/40.0f);
		lhc->setMaxAngularLimit(HK_REAL_PI/4.0f);

		//
		//	Create and add the constraint
		//
		{
			hkpConstraintInstance* constraint = new hkpConstraintInstance(backpack, torso, lhc );
			world->addConstraint(constraint);
			constraint->removeReference();
		}	

		if( !tricks.m_useLimits ) 
		{
			lhc->disableLimits();
		}


		lhc->removeReference();
	}

}



// Demo helper function for matching bones from their names.
void HK_CALL CharacterAttachmentsHelpers::stripPrefix( hkString& strInOut )
{
	// Replace spaces by underscores.
	strInOut = strInOut.replace(' ','_');

	// Remove skeleton prefix.
	// This the text before the first underscore.
	const int endPrefix = strInOut.indexOf('_',0);

	if (endPrefix!=-1)
	{
		strInOut = strInOut.substr(endPrefix+1);
	}
}

// This is the string comparison we use to match names from the animation skeleton
// to names in the ragdoll skeleton. The filter just extracts the bone name,
// from either, so we will use the same filter for both skeletons.
// For example, it will match "Rig Pelvis" to "Ragdoll_Pelvis".
int HK_CALL CharacterAttachmentsHelpers::compareWithoutPrefix( const char* str1, const char* str2 )
{
	hkString string1(str1);
	stripPrefix(string1);

	hkString string2(str2);
	stripPrefix(string2);

	return string1.compareToIgnoreCase( string2 );
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
