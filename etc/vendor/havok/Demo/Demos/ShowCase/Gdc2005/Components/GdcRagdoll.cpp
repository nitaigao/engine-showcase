/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/ShowCase/Gdc2005/Gdc2005Demo.h>

// Serialization
#include <Common/Base/Reflection/hkClass.h>
#include <Common/Serialize/Util/hkLoader.h>
#include <Common/Serialize/Util/hkRootLevelContainer.h>
#include <Common/Base/Reflection/Registry/hkTypeInfoRegistry.h>
#include <Demos/DemoCommon/Utilities/Asset/hkAssetManagementUtil.h>
#include <Common/Base/Container/LocalArray/hkLocalBuffer.h>

// Rig
#include <Animation/Animation/Rig/hkaSkeleton.h>
#include <Animation/Animation/Rig/hkaSkeletonUtils.h>
#include <Animation/Animation/Rig/hkaPose.h>
#include <Animation/Animation/Mapper/hkaSkeletonMapper.h>

// Animation
#include <Animation/Animation/hkaAnimationContainer.h>
#include <Animation/Animation/Playback/hkaAnimatedSkeleton.h>
#include <Animation/Animation/Playback/Control/Default/hkaDefaultAnimationControl.h>
#include <Animation/Animation/Animation/hkaAnimationBinding.h>

// Constraints and motors
#include <Physics/Dynamics/Constraint/Bilateral/LimitedHinge/hkpLimitedHingeConstraintData.h>
#include <Physics/Dynamics/Constraint/Bilateral/Ragdoll/hkpRagdollConstraintData.h>
#include <Physics/Dynamics/Constraint/Motor/Position/hkpPositionConstraintMotor.h>
#include <Physics/Utilities/Constraint/Bilateral/hkpConstraintUtils.h>

// Ragdoll
#include <Animation/Ragdoll/Instance/hkaRagdollInstance.h>
#include <Animation/Ragdoll/Controller/PoweredConstraint/hkaRagdollPoweredConstraintController.h>
#include <Animation/Ragdoll/Controller/RigidBody/hkaRagdollRigidBodyController.h>
#include <Animation/Ragdoll/Utils/hkaRagdollUtils.h>

// State Machine
#include <Demos/ShowCase/Gdc2005/StateMachine/GdcState.h>
#include <Demos/ShowCase/Gdc2005/StateMachine/GdcStateTypes.h>

// Pose matching
#include <Animation/Ragdoll/PoseMatching/hkaPoseMatchingUtility.h>

// Ray casting
#include <Physics/Collide/Query/CastUtil/hkpWorldRayCaster.h>
#include <Physics/Collide/Query/CastUtil/hkpWorldRayCastInput.h>
#include <Physics/Collide/Query/CastUtil/hkpWorldRayCastOutput.h>
#include <Physics/Collide/Query/Collector/RayCollector/hkpClosestRayHitCollector.h>

// Proxy
#include <Physics/Utilities/CharacterControl/CharacterProxy/hkpCharacterProxy.h>

// Graphics
#include <Graphics/Common/DisplayWorld/hkgDisplayWorld.h>
#include <Graphics/Common/DisplayObject/hkgDisplayObject.h>
#include <Graphics/Bridge/DisplayHandler/hkgDisplayHandler.h>

#include <Common/Visualize/hkDebugDisplay.h>

// Set quality and filter info for a ragdoll body part
static void setDynamicWithCollisions(hkpRigidBody* rb, int boneId, int parentId);
static void setDynamicWithoutCollisions(hkpRigidBody* rb);
static void setKeyframedWithoutCollisions(hkpRigidBody* rb);

// display related
void _removeRagdollDisplayBodies( hkaRagdollInstance* ragdollInstance, hkDemoEnvironment* env, hkArray<class hkgDisplayObject*>& ragdollDisplayBodies );

void Gdc2005Demo::initRagdoll()
{
	// Rig container should be loaded already
	HK_ASSERT(0, m_rigContainer);

	// Extract the ragdoll instance created with the CreateRagdoll filter in the tool chain
	m_ragdollInstance = reinterpret_cast<hkaRagdollInstance*>( m_rigContainer->findObjectByType( hkaRagdollInstanceClass.getName() ));
	HK_ASSERT2(0, m_ragdollInstance, "Couldn't load ragdoll setup");

	// Initialize collision/motion settings for the rigidbody ragdoll parts
	for ( int b =0; b < m_ragdollInstance->getNumBones(); b++)
	{
		hkpRigidBody* rb = m_ragdollInstance->getRigidBodyOfBone(b);

		// Initialize with quality type and collision filter
		setDynamicWithCollisions( rb, b, m_ragdollInstance->getParentOfBone( b ) );
	}

	// We turn off friction for all constraints - this helps the controllers match the animation pose better
	for ( int c =1; c < m_ragdollInstance->getNumBones(); c++)
	{
		hkpConstraintData* constraintData = m_ragdollInstance->getConstraintOfBone(c)->getDataRw();
		switch (constraintData->getType())
		{
			case hkpConstraintData::CONSTRAINT_TYPE_LIMITEDHINGE:
			{
				hkpLimitedHingeConstraintData* hinge = static_cast<hkpLimitedHingeConstraintData*> (constraintData);
				hinge->setMaxFrictionTorque( 0.0f );
				break;
			}
			case hkpConstraintData::CONSTRAINT_TYPE_RAGDOLL:
			{
				hkpRagdollConstraintData* ragdoll = static_cast<hkpRagdollConstraintData*> (constraintData);
				ragdoll->setMaxFrictionTorque( 0.0f );
				break;
			}
		}
	}

	// This routine iterates through the bodies pointed to by the constraints and stabilizes their inertias.
	// This makes both ragdoll controllers lees sensitive to angular effects and hence more effective
	const hkArray<hkpConstraintInstance*>& constraints = m_ragdollInstance->getConstraintArray();
	hkpInertiaTensorComputer::optimizeInertiasOfConstraintTree( constraints.begin(), constraints.getSize(), m_ragdollInstance->getRigidBodyOfBone(0) );

	// Construct the ragdoll rigidbody controller
	// This controller handles the hit reaction
	m_rigidBodyController = new hkaRagdollRigidBodyController( m_ragdollInstance );
	m_initFeedback = false;

	// Add the ragdoll to the world
	m_ragdollInstance->addToWorld(m_world, true);

	// Find the two mappings
	const hkaSkeleton* ragdollSkeleton = m_ragdollInstance->getSkeleton();
	void *objectFound = m_rigContainer->findObjectByType(hkaSkeletonMapperClass.getName());
	while (objectFound)
	{
		hkaSkeletonMapper* mapperFound = reinterpret_cast<hkaSkeletonMapper*> (objectFound);

		// Use the skeleton to dtermine which mapper is which
		if (mapperFound->m_mapping.m_skeletonA == ragdollSkeleton)
		{
			m_ragdollToHighResMapper = mapperFound;
		}
		else
		{
			HK_ASSERT(0,mapperFound->m_mapping.m_skeletonB == ragdollSkeleton);
			m_highResToRagdollMapper = mapperFound;
		}

		objectFound = m_rigContainer->findObjectByType(hkaSkeletonMapperClass.getName(), objectFound);
	}
	HK_ASSERT2(0, m_highResToRagdollMapper, "Couldn't load high-to-ragdoll mapping");
	HK_ASSERT2(0, m_ragdollToHighResMapper, "Couldn't load ragdoll-to-high mapping");

	// Create a list of bones in the lower body
	// We use this array to set the leg bones / lower body to keyframed and upper body to dynamic later.
	{
		const hkaSkeleton* skeleton = m_ragdollInstance->getSkeleton();

		// Pelvis
		m_lowerBodyBones.pushBack(0);

		// Right Leg
		const hkInt16 startRight = hkaSkeletonUtils::findBoneWithName(*skeleton, "Biped_RightHip");
		m_lowerBodyBones.pushBack(startRight);
		hkaSkeletonUtils::getDescendants(*skeleton, startRight, m_lowerBodyBones);

		// Left leg
		const hkInt16 startLeft = hkaSkeletonUtils::findBoneWithName(*skeleton, "Biped_LeftHip");
		m_lowerBodyBones.pushBack(startLeft);
		hkaSkeletonUtils::getDescendants(*skeleton, startLeft, m_lowerBodyBones);
	}

	// Remove the display bodies from the display world
	_removeRagdollDisplayBodies( m_ragdollInstance, m_env, m_ragdollDisplayBodies );
	m_ragdollDisplayBodiesVisible = false;

	m_blendControl = HK_NULL;
}

// doRagdoll() : drives the ragdoll to the incoming pose and modifies that pose to reflect the ragdoll
void Gdc2005Demo::doRagdoll( hkaPose& thePose )
{
	// Get the ragdoll's reference pose in model space
	// We use this pose as the in+out pose to the mapper below
	const hkaSkeleton* rSkel = m_ragdollInstance->getSkeleton();
	hkLocalBuffer<hkQsTransform> ragdollModelSpace( rSkel->m_numBones );
	hkaSkeletonUtils::transformLocalPoseToModelPose( rSkel->m_numBones, rSkel->m_parentIndices, rSkel->m_referencePose, ragdollModelSpace.begin() );

	// Map the pose from the animation (highres) to ragdoll (lowres)
	m_highResToRagdollMapper->mapPose( thePose.getSyncedPoseModelSpace().begin(), rSkel->m_referencePose, ragdollModelSpace.begin(), hkaSkeletonMapper::CURRENT_POSE );

	// Drive the powered constraints to this ragdoll pose using a controller
	{
		hkLocalBuffer<hkQsTransform> ragdollLocalSpace( rSkel->m_numBones );
		hkaSkeletonUtils::transformModelPoseToLocalPose( rSkel->m_numBones, rSkel->m_parentIndices, ragdollModelSpace.begin(), ragdollLocalSpace.begin() );

		hkaRagdollPoweredConstraintController::startMotors(m_ragdollInstance);
		hkaRagdollPoweredConstraintController::driveToPose( m_ragdollInstance, ragdollLocalSpace.begin() ) ;
	}

	// Get the current pose from the ragdoll, map it back to the character
	{
		m_ragdollInstance->getPoseModelSpace(ragdollModelSpace.begin(), m_currentTransform);

		const hkQsTransform* localBIn  = thePose.getSyncedPoseLocalSpace().begin();
		hkQsTransform*       modelBOut = thePose.accessSyncedPoseModelSpace().begin();
		m_ragdollToHighResMapper->mapPose( ragdollModelSpace.begin(), localBIn, modelBOut, hkaSkeletonMapper::CURRENT_POSE );
	}

	// During the first half second, we check that no constraints are violated: this may happen if the ragdoll
	// started in a position where some bones where penetrating the landscape. We use the checkAndFixConstraint() utility method
	// to push the bones out.
	const hkReal timeInState = m_animationStateMachine->getCurrentTime()- m_animationStateMachine->getLastStateChange();
	if (timeInState < 0.5f)
	{
		const hkArray<hkpConstraintInstance*>& constraints = m_ragdollInstance->getConstraintArray();
		for (int c=constraints.getSize()-1; c>=0; --c)
		{
			// Do only one at a time
			if (hkpConstraintUtils::checkAndFixConstraint(constraints[c], 0.1f))
				break;
		}
	}

	// For this demo, we force the proxy to follow the root - we just do this to simplify shadow calcs which are relative to the proxy
	// In general, you don't need to do this
	m_characterProxy->setPosition( m_ragdollInstance->getRigidBodyOfBone(0)->getPosition() );

}

// doGetup() is called during the GETUP state, it handles the blending from the get up animation and the current pose of the ragdoll
// When getting up we need to blend between the local pose of the current ragdoll and the local pose of the get up animation.
// The ragdoll is in world space, so we must compute a reference frame to convert it to a local space pose (m_ragdollFrame)
// Similarly we must find an appropiate frame of reference to play the animation in (m_animFrame)
// As we get up we blend the local poses and the reference frames to produce our output pose

void Gdc2005Demo::doGetup (hkaPose &thePose)
{
	const hkaSkeleton* rSkel = m_ragdollInstance->getSkeleton();

	// When we start getiing up we compute the required reference frames
	if (!m_initGetup)
	{
		hkLocalBuffer<hkQsTransform> referenceModelSpace( rSkel->m_numBones );
		hkaSkeletonUtils::transformLocalPoseToModelPose( rSkel->m_numBones, rSkel->m_parentIndices, rSkel->m_referencePose, referenceModelSpace.begin() );

		// Map from animation to reduced num of bones
		m_highResToRagdollMapper->mapPose( thePose.getSyncedPoseModelSpace().begin(), rSkel->m_referencePose, referenceModelSpace.begin(), hkaSkeletonMapper::CURRENT_POSE );

		// Grab the pose from the ragdoll
		hkLocalBuffer<hkQsTransform> ragdollWorldSpace( rSkel->m_numBones );
		m_ragdollInstance->getPoseModelSpace( ragdollWorldSpace.begin() , hkQsTransform::getIdentity() );

		// Work out reference frames for both the ragdoll and the animation
		m_poseMatchUtils->computeReferenceFrame(referenceModelSpace.begin(), ragdollWorldSpace.begin(), m_animFrame, m_ragdollFrame);

		// If the ragdoll does not exactly match the first pose of the get up animation then the position
		// of the animation reference frame will not be exact. We compensate for this by projecting the frame using a ray cast
		// Our animations are authored with a reference frame that is the position of the pelvis projected onto the ground plane
		// If you animations use a different reference frame position then you must compensate here by translating
		// the result you get from the ray cast appropriately.
		{
			hkpWorldRayCastInput raycastIn;
			hkpClosestRayHitCollector rayCollector;

			// Set up a ray cast straight down from the pelvis
			raycastIn.m_from = m_ragdollInstance->getRigidBodyOfBone(0)->getPosition();
			raycastIn.m_to.setAdd4( hkVector4(0,-5,0), raycastIn.m_from);
			raycastIn.m_filterInfo=hkpGroupFilter::calcFilterInfo(LAYER_RAYCAST,0);

			// cast the ray into the landscape
			m_world->castRay( raycastIn, rayCollector );

			const hkBool didHit = rayCollector.hasHit();
			if (didHit)
			{
				const hkpWorldRayCastOutput& raycastOut = rayCollector.getHit();
				{
					// Adjust the reference frame
					hkVector4 hitPointWS; hitPointWS.setInterpolate4(raycastIn.m_from, raycastIn.m_to, raycastOut.m_hitFraction);
					m_animFrame.m_translation = hitPointWS;
				}
			}
		}

		// We position the proxy at the point we're getting up
		m_characterProxy->setPosition( m_animFrame.m_translation );
		m_verticalDisplacement = 0.0f;

		// Get up has been initialized
		m_initGetup = true;

		// We use the ease curve support in a control for our blend weight
		m_blendControl = new hkaDefaultAnimationControl( HK_NULL );
		m_blendControl->setMasterWeight(1.0f);
		m_blendControl->easeOut(m_options.m_GetUp.m_getupBlendSpeed);

	}

	// Update the control - adjusting the weight
	m_blendControl->update(m_timestep);

	// We change the shape of the ease curve to slow down the blend
	hkReal blendWeight =  hkMath::sqrt(hkMath::sqrt(m_blendControl->getWeight()));

	// We have set up the proxy with the initially calculated animation frame
	// We now retrieve this from the proxy. The proxy may be moved during get up
	// because it is interacting, or falling or resolving a penetration
	m_animFrame.m_translation = m_characterProxy->getPosition();
	m_verticalDisplacement = 0.0f;

	// Interpolate our reference frame from the ragdoll frame and the animation frame
	m_currentTransform.setInterpolate4(m_animFrame, m_ragdollFrame, blendWeight);

	// Blend the poses in local space
	{
		// We map the ragdoll pose in model space to a higher resolution pose in model space
		hkLocalBuffer<hkQsTransform> ragdollHighResModelSpace( thePose.getSkeleton()->m_numBones );
		hkString::memCpy( ragdollHighResModelSpace.begin(), thePose.getSyncedPoseModelSpace().begin(), thePose.getSkeleton()->m_numBones * sizeof(hkQsTransform) );

		// Get the current pose form the ragdoll
		hkLocalBuffer<hkQsTransform> ragdollModelSpace( rSkel->m_numBones );
		m_ragdollInstance->getPoseModelSpace( ragdollModelSpace.begin() , m_ragdollFrame );

		// Map ragdoll pose in model space from low res to high res
		m_ragdollToHighResMapper->mapPose( ragdollModelSpace.begin(), thePose.getSyncedPoseLocalSpace().begin(), ragdollHighResModelSpace.begin(), hkaSkeletonMapper::CURRENT_POSE );

		// Compute the local space pose for the ragdoll
		hkLocalBuffer<hkQsTransform> ragdollHighResLocalSpace( thePose.getSkeleton()->m_numBones );
		hkaSkeletonUtils::transformModelPoseToLocalPose( thePose.getSkeleton()->m_numBones, thePose.getSkeleton()->m_parentIndices, ragdollHighResModelSpace.begin(), ragdollHighResLocalSpace.begin() );

		const hkQsTransform* originalLocal = thePose.getSyncedPoseLocalSpace().begin();
		hkaSkeletonUtils::blendPoses(  m_animatedSkeleton->getSkeleton()->m_numBones,
			originalLocal,
			ragdollHighResLocalSpace.begin(),
			blendWeight,
			thePose.accessUnsyncedPoseLocalSpace().begin() );
	}

	// Here we bypass the state machine logic and explicitly force ourselves out of the GETTING_UP state once
	// If the animation has nearly finished then we stop playing get up and switch state causing the get up animation to be blended out.
	hkaDefaultAnimationControl* getUpControl = (hkaDefaultAnimationControl*)m_animatedSkeleton->getAnimationControl( GDC_GET_UP_CONTROL );

	if (getUpControl->getAnimationBinding()->m_animation->m_duration - getUpControl->getLocalTime() < 0.3f )
	{
		// Destroy the control we were using for blending
		m_blendControl->removeReference();
		m_blendControl = HK_NULL;
		// Hold the animation (so it doesn't loop)
		getUpControl->setPlaybackSpeed(0.0f);
		// Force the state machine
		m_animationStateMachine->setCurrentState(GDC_STAND_STATE, m_animationMachine);
	}

}

// doRagdollFeedback() drives the rigid bodies that make up the ragdoll during non-death states. This is used for get hit effects, and
// for bone collisions.
// By default, the lower body is keyframed - it doesn't react, while the upper body is dynamic - it reacts.
void Gdc2005Demo::doRagdollFeedback(hkaPose &thePose)
{
	// We start by mapping the animation pose to the low-res space

	// Start with the reference pose (in+out parameter to mapper)
	const hkaSkeleton* rSkel = m_ragdollInstance->getSkeleton();
	hkLocalBuffer<hkQsTransform> lowResModelSpace( rSkel->m_numBones );
	hkaSkeletonUtils::transformLocalPoseToModelPose( rSkel->m_numBones, rSkel->m_parentIndices, rSkel->m_referencePose, lowResModelSpace.begin() );

	// Map from animation to ragdoll
	m_highResToRagdollMapper->mapPose( thePose.getSyncedPoseModelSpace().begin(), rSkel->m_referencePose, lowResModelSpace.begin(), hkaSkeletonMapper::CURRENT_POSE );

	// Convert the output to local space (required by the controller
	hkLocalBuffer<hkQsTransform> lowResLocalSpace( rSkel->m_numBones );
	hkaSkeletonUtils::transformModelPoseToLocalPose( rSkel->m_numBones, rSkel->m_parentIndices, lowResModelSpace.begin(), lowResLocalSpace.begin() );

	// If we weren't doing this before...
	if (!m_initFeedback)
	{
		// Switch off any motors that we may have on (if we were doing ragdolls in death state)
		hkaRagdollPoweredConstraintController::stopMotors( m_ragdollInstance );

		m_ragdollInstance->setPoseModelSpace(lowResModelSpace.begin(), m_currentTransform);

		// Set the properties (motion type and collision filter) of the rigid bodies representing the bones
		for (int i=0; i < m_ragdollInstance->getNumBones(); i++)
		{
			hkpRigidBody* rb = m_ragdollInstance->getRigidBodyOfBone(i);

			// Bones in the legs are usually keyframed (unless the user has told us otherwise)
			// Rest of bones (upper body) are dynamic (for get-hit effects).
			// We put all of them in the same system to ignore collisions between bones
			bool isLowerBodyBone = (m_lowerBodyBones.indexOf( (hkInt16)i ) != -1);
			if (m_options.m_GetHit.m_fixLegs && isLowerBodyBone)
			{
				setKeyframedWithoutCollisions( rb );
			}
			else
			{
				setDynamicWithoutCollisions( rb );
			}

			rb->setLinearVelocity( hkVector4::getZero() );
			rb->setAngularVelocity( hkVector4::getZero() );
		}

		// Let the ragdoll rigid body controller know that we are starting to drive rigid bodies again
		m_rigidBodyController->reinitialize();

		m_initFeedback = true;
	}

	const int numBodies = m_ragdollInstance->getRigidBodyArray().getSize();

	HK_TIMER_BEGIN("KFHUtil", HK_NULL);

	// We now drive the rigid bodies so they follow the animation, using a ragdoll rigid body controller
	hkaKeyFrameHierarchyUtility::ControlData& controlData = m_rigidBodyController->m_controlDataPalette[0];

	controlData.m_hierarchyGain =	m_options.m_GetHit.m_hierarchyGain;
	controlData.m_velocityDamping = m_options.m_GetHit.m_dampVelocity;
	controlData.m_accelerationGain = m_options.m_GetHit.m_accelerationGain;
	controlData.m_velocityGain = m_options.m_GetHit.m_velocity.m_velocityGain;
	controlData.m_positionGain = m_options.m_GetHit.m_position.m_positionGain;
	controlData.m_positionMaxLinearVelocity = m_options.m_GetHit.m_position.m_positionMaxLinearVelocity;
	controlData.m_positionMaxAngularVelocity = m_options.m_GetHit.m_position.m_positionMaxAngularVelocity;
	controlData.m_snapGain = m_options.m_GetHit.m_snap.m_snapGain;
	controlData.m_snapMaxLinearVelocity = m_options.m_GetHit.m_snap.m_snapMaxLinearVelocity;
	controlData.m_snapMaxAngularVelocity = m_options.m_GetHit.m_snap.m_snapMaxAngularVelocity;
	controlData.m_snapMaxLinearDistance = m_options.m_GetHit.m_snap.m_snapMaxLinearDistance;
	controlData.m_snapMaxAngularDistance = m_options.m_GetHit.m_snap.m_snapMaxAngularDistance;
	hkLocalBuffer<hkaKeyFrameHierarchyUtility::Output> output(numBodies);
	m_rigidBodyController->driveToPose( m_timestep, lowResLocalSpace.begin(), m_currentTransform, output.begin());

	HK_TIMER_END();

	// Get the current pose, and blend based on stress
	{
		hkLocalBuffer<hkQsTransform> ragdollModelSpace( rSkel->m_numBones );
		m_ragdollInstance->getPoseModelSpace(ragdollModelSpace.begin(), m_currentTransform);

		hkLocalBuffer<hkQsTransform> ragdollLocalSpace( rSkel->m_numBones );
		hkaSkeletonUtils::transformModelPoseToLocalPose( rSkel->m_numBones, rSkel->m_parentIndices, ragdollModelSpace.begin(), ragdollLocalSpace.begin() );

		for (int i =0; i < numBodies; i++ )
		{
			// Show stress : we set the color of rigid bodies based on the output of the controller (the stress)
			if(m_ragdollDisplayBodiesVisible && m_options.m_GetHit.m_showStress)
			{
				hkReal stress = hkMath::sqrt( output[i].m_stressSquared );
				stress *= 0.05f; // scale the value for display
				stress = hkMath::max2( stress, 0.0f );
				stress = hkMath::min2( stress, 1.0f );

				HK_TIMER_BEGIN("DisplayObject", HK_NULL);
				int color = 0xff808080;
				int colorOffset = int( stress * 127 );
				color -= colorOffset << 16;
				color += colorOffset << 8;
				color -= colorOffset << 0;
				HK_SET_OBJECT_COLOR( hkUlong( m_ragdollInstance->getRigidBodyArray()[i]->getCollidable()), color );
				HK_TIMER_END();
			}

			// Here we do the interpolation  between the animation-based pose and the ragdoll-based pose, based on the stress
			lowResLocalSpace[i].setInterpolate4( lowResLocalSpace[i], ragdollLocalSpace[i], m_options.m_GetHit.m_ragdollFeedback);
		}
	}

	// We now map the blended pose to the high res skeleton
	{
		// Need to recalculate the model space version of the blended pose
		hkaSkeletonUtils::transformLocalPoseToModelPose( rSkel->m_numBones, rSkel->m_parentIndices, lowResLocalSpace.begin(), lowResModelSpace.begin() );

		// Do the mapping
		const hkQsTransform* localBIn  = thePose.getSyncedPoseLocalSpace().begin();
		hkQsTransform*       modelBOut = thePose.accessSyncedPoseModelSpace().begin();
		m_ragdollToHighResMapper->mapPose( lowResModelSpace.begin(), localBIn, modelBOut, hkaSkeletonMapper::CURRENT_POSE );
	}
}

// stopRagdollFeedback() just turns the rigid bodies to fully dynamics, with collisions between them, except parent vs. child.
void Gdc2005Demo::stopRagdollFeedback()
{
	m_initFeedback = false;

	for (int b=0; b<m_ragdollInstance->getNumBones(); b++ )
	{
		const int parentId = m_ragdollInstance->getParentOfBone(b);
		setDynamicWithCollisions( m_ragdollInstance->getRigidBodyOfBone(b), b, parentId);
	}
}


// quitRagdoll() it's only call on destruction of the demo; it deallocates resources
void Gdc2005Demo::quitRagdoll ()
{
	if (m_ragdollInstance->getWorld())
	{
		m_ragdollInstance->removeFromWorld();
	}

	if (m_rigidBodyController)
	{
		delete m_rigidBodyController;
	}

	m_env->m_window->getContext()->lock();

	for (int rbi=0; rbi < m_ragdollDisplayBodies.getSize(); ++rbi)
	{
		m_ragdollDisplayBodies[rbi]->removeReference(); // remove our reference, but it should have been safe removed by the handler so will not be deleted just yet
	}
	m_ragdollDisplayBodies.clear();
	m_env->m_window->getContext()->unlock();


	if (m_blendControl)
	{
		m_blendControl->removeReference();
		m_blendControl = HK_NULL;
	}

	if (m_poseMatchUtils)
	{
		delete m_poseMatchUtils;
		m_poseMatchUtils= HK_NULL;
	}

	// The ragdoll bodies are loaded from a packfile will not have their destructors called (the memory is just dumped)
	// Because we change from keyframed to dynamic we need to make sure the dtor is called to destroy the motion
	for ( int b =0; b < m_ragdollInstance->getNumBones(); b++)
	{
		// Explictly call to the destructor
		m_ragdollInstance->getRigidBodyOfBone(b)->~hkpRigidBody();
	}

}

// warpCharacter() just resets the character position to one of the predefined "warp points"
void Gdc2005Demo::warpCharacter(int warpPointIndex)
{
	int numWarpPoints = m_warpPoints.getSize();
	if (numWarpPoints < 1) return;

	// Switch to the stand up state
	m_animationStateMachine->setCurrentState(GDC_STAND_STATE, m_animationMachine);
	// Ensure that the animation is fully eased in
	m_animationMachine->update(1.0f);
	m_animatedSkeleton->stepDeltaTime(1.0f);

	// Set the current transform to this new warp point
	const hkTransform& warpPoint = m_warpPoints[warpPointIndex];
	m_currentTransform.m_translation = warpPoint.getTranslation();
	m_currentTransform.m_rotation = hkQuaternion(warpPoint.getRotation());

	// Reset foot ik stuff
	m_footOnAir[LEFT_LEG] = false;
	m_footOnAir[RIGHT_LEG] = false;

	// Set the pose for the ragdoll bones
	{
		// Get the current animation pose, convert it to model space.
		// We reuse the same array: transformLocalPoseToModelPose is alias safe (but transformModelPoseToLocalPose is not!)
		const hkaSkeleton* skel = m_animatedSkeleton->getSkeleton();
		hkLocalBuffer< hkQsTransform > animPose( skel->m_numBones );
		m_animatedSkeleton->sampleAndCombineAnimations( animPose.begin(), HK_NULL );
		hkaSkeletonUtils::transformLocalPoseToModelPose( skel->m_numBones, skel->m_parentIndices, animPose.begin(), animPose.begin() );

		// Now, map the animation pose into a low-res pose we can use for the ragdoll
		// We start with the reference pose (in model space)
		const hkaSkeleton* rSkel = m_ragdollInstance->getSkeleton();
		hkLocalBuffer<hkQsTransform> ragdollModelSpace( rSkel->m_numBones );
		hkaSkeletonUtils::transformLocalPoseToModelPose( rSkel->m_numBones, rSkel->m_parentIndices, rSkel->m_referencePose, ragdollModelSpace.begin() );
		m_highResToRagdollMapper->mapPose( animPose.begin(), rSkel->m_referencePose, ragdollModelSpace.begin(), hkaSkeletonMapper::CURRENT_POSE );

		// Set the resulting pose to the ragdoll
		m_ragdollInstance->setPoseModelSpace( ragdollModelSpace.begin(), m_currentTransform );
	}

	// Move the character proxy
	m_characterProxy->setPosition(warpPoint.getTranslation());

	// Tell the ragdoll rigid body controller that the position of the rigid bodies have changes, and we need to
	// start driving based on the new positions.
	m_rigidBodyController->reinitialize();
}


// Given a rigid body (a ragdoll bone), make it dynamic, enable collisions with all bones except its parent,
// and set the layer and quality properties accordingly
static void setDynamicWithCollisions(hkpRigidBody* rb, int boneId, int parentId)
{
	const hkUint32 fi = hkpGroupFilter::calcFilterInfo(Gdc2005Demo::LAYER_RAGDOLL_DYNAMIC, 1, boneId+1, parentId+1);

	if (( rb->getMotionType() == hkpMotion::MOTION_KEYFRAMED ) ||
		( rb->getQualityType() != HK_COLLIDABLE_QUALITY_MOVING) ||
		( rb->getCollisionFilterInfo() != fi))
	{
		rb->setMotionType(hkpMotion::MOTION_DYNAMIC);
		rb->setQualityType( HK_COLLIDABLE_QUALITY_MOVING );
		rb->setCollisionFilterInfo(fi);

		if (rb->getWorld())
		{
			rb->getWorld()->updateCollisionFilterOnEntity( rb, HK_UPDATE_FILTER_ON_ENTITY_FULL_CHECK, HK_UPDATE_COLLECTION_FILTER_PROCESS_SHAPE_COLLECTIONS );
		}
	}
}

// Given a rigid body (a ragdoll bone), make it dynamic, disable collisions with all bones in the ragdoll (assign the same system with
// no subsystems), and set the layer and quality properties accordingly
static void setDynamicWithoutCollisions(hkpRigidBody* rb)
{
	const hkUint32 newFi = hkpGroupFilter::calcFilterInfo(Gdc2005Demo::LAYER_RAGDOLL_DYNAMIC,1, 0, 0);

	if (( rb->getMotionType() == hkpMotion::MOTION_KEYFRAMED ) ||
		( rb->getQualityType() != HK_COLLIDABLE_QUALITY_MOVING) ||
		( rb->getCollisionFilterInfo() != newFi))
	{
		rb->setMotionType(hkpMotion::MOTION_DYNAMIC);
		rb->setQualityType( HK_COLLIDABLE_QUALITY_MOVING );
		rb->setCollisionFilterInfo(newFi);

		if (rb->getWorld())
		{
			rb->getWorld()->updateCollisionFilterOnEntity( rb, HK_UPDATE_FILTER_ON_ENTITY_FULL_CHECK, HK_UPDATE_COLLECTION_FILTER_PROCESS_SHAPE_COLLECTIONS );
		}
	}
}

// Given a rigid body (a ragdoll bone), make it keyframed, disable collisions with all bones in the ragdoll (assign the same system with
// no subsystems), and set the layer and quality properties accordingly
static void setKeyframedWithoutCollisions(hkpRigidBody* rb)
{
	const hkUint32 fi = hkpGroupFilter::calcFilterInfo(Gdc2005Demo::LAYER_RAGDOLL_KEYFRAMED,1, 0, 0);

	if (( rb->getMotionType() != hkpMotion::MOTION_KEYFRAMED ) ||
		( rb->getQualityType() != HK_COLLIDABLE_QUALITY_KEYFRAMED) ||
		( rb->getCollisionFilterInfo() != fi))
	{
		rb->setMotionType(hkpMotion::MOTION_KEYFRAMED);
		rb->setQualityType( HK_COLLIDABLE_QUALITY_KEYFRAMED );
		rb->setCollisionFilterInfo(fi);

		if (rb->getWorld())
		{
			rb->getWorld()->updateCollisionFilterOnEntity( rb, HK_UPDATE_FILTER_ON_ENTITY_FULL_CHECK, HK_UPDATE_COLLECTION_FILTER_PROCESS_SHAPE_COLLECTIONS );
		}
	}
}

// Currently in the toolchain all constraints for a single ragdoll share the same motor.
// We could simply grab the first motor and set its params but instead
// we iterate through all the constraints and build a proper list of motors to set.
void Gdc2005Demo::setMotors(hkaRagdollInstance* ragdoll, hkReal force, hkReal tau, hkReal propRecoveryVel, hkReal conRecoveryVel)
{
	hkLocalArray<hkpPositionConstraintMotor*> motors( m_ragdollInstance->getNumBones() );

	for ( int c =1; c < m_ragdollInstance->getNumBones(); c++)
	{
		hkpConstraintData* constraintData = m_ragdollInstance->getConstraintOfBone(c)->getDataRw();

		switch (constraintData->getType())
		{
			case hkpConstraintData::CONSTRAINT_TYPE_LIMITEDHINGE:
			{
				hkpLimitedHingeConstraintData* powered = static_cast<hkpLimitedHingeConstraintData*> (constraintData);
				hkpPositionConstraintMotor* motor = (hkpPositionConstraintMotor*)powered->getMotor();
				if (motor && (motors.indexOf(motor) == -1) )
				{
					motors.pushBack( motor );
				}
				break;
			}
			case hkpConstraintData::CONSTRAINT_TYPE_RAGDOLL:
			{
				hkpRagdollConstraintData* powered = static_cast<hkpRagdollConstraintData*> (constraintData);
				hkpPositionConstraintMotor* motor = (hkpPositionConstraintMotor*)powered->getPlaneMotor();
				if (motor && (motors.indexOf(motor) == -1) )
				{
					motors.pushBack( motor );
				}
				motor = (hkpPositionConstraintMotor*)powered->getConeMotor();
				if (motor && (motors.indexOf(motor) == -1) )
				{
					motors.pushBack( motor );
				}
				motor = (hkpPositionConstraintMotor*)powered->getTwistMotor();
				if (motor && (motors.indexOf(motor) == -1) )
				{
					motors.pushBack( motor );
				}
				break;
			}
		}
	}

	for (int m=0 ;m < motors.getSize(); m++)
	{
		motors[m]->m_maxForce = force;
		motors[m]->m_tau = tau;
		motors[m]->m_proportionalRecoveryVelocity = propRecoveryVel;
		motors[m]->m_constantRecoveryVelocity = conRecoveryVel;
	}
}

void Gdc2005Demo::toggleRagdollVisibility()
{

	if (m_ragdollDisplayBodies.getSize() < 1) return;
	m_ragdollDisplayBodiesVisible = !m_ragdollDisplayBodiesVisible;

	int numBodies = m_ragdollDisplayBodies.getSize();
	if (m_ragdollDisplayBodiesVisible)
	{
		// Add
		for (int b=0; b< numBodies; ++b)
		{
			m_env->m_displayWorld->addDisplayObject(m_ragdollDisplayBodies[b]);
		}
	}
	else
	{
		// Remove
		for (int b=0; b< numBodies; ++b)
		{
			int idx = m_env->m_displayWorld->findDisplayObject(m_ragdollDisplayBodies[b]);
			if (idx >=0 )
			{
				m_env->m_displayWorld->removeDisplayObject(idx);
				m_ragdollDisplayBodies[b]->release(); // the world ref
			}
		}
	}
}

void _removeRagdollDisplayBodies( hkaRagdollInstance* ragdollInstance, hkDemoEnvironment* env, hkArray<class hkgDisplayObject*>& ragdollDisplayBodies )
{
	for (hkInt16 i=0; i<ragdollInstance->getNumBones(); i++)
	{
		// Make all debug (auto) rigid bodies transparent
		hkpRigidBody* rb = ragdollInstance->getRigidBodyOfBone(i);
		hkUlong id = (hkUlong)(rb->getCollidable());
		HK_SET_OBJECT_COLOR(id, hkColor::rgbFromChars( 100, 255, 100, 255));

		// keep a list of the bodies so that they can be added or removed from view
		// start off removed from view
		hkgDisplayObject* dispObj = env->m_displayHandler->findDisplayObject( id );
		int idx = env->m_displayWorld->findDisplayObject( dispObj );
		if (idx)
		{
			ragdollDisplayBodies.pushBack( dispObj );
			env->m_displayWorld->removeDisplayObject( idx ); // we keep the ref it gives back
		}
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
