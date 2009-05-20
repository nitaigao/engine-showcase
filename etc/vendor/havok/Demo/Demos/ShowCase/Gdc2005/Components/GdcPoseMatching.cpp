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

// Ragdoll
#include <Animation/Ragdoll/Instance/hkaRagdollInstance.h>

// State Machine
#include <Demos/ShowCase/Gdc2005/StateMachine/GdcState.h>
#include <Demos/ShowCase/Gdc2005/StateMachine/GdcStateTypes.h>

// Pose matching
#include <Animation/Ragdoll/PoseMatching/hkaPoseMatchingUtility.h>

// Proxy
#include <Physics/Utilities/CharacterControl/CharacterProxy/hkpCharacterProxy.h>

#include <Common/Visualize/hkDebugDisplay.h>

const hkVector4 UP(0,1,0);

// When we are choosing which pose to get up from we want a smooth rather than discrete transition from one
// candidate pose to the next. To acheive this we use the AnimatedSkeleton as a mixer. For each candidate
// pose we create a new control. Each control points to the right get up animation and has its local time
// set to the point in the animation to get up from. (This get up point is extracted from annotations that
// we're marked up in the original asset). The playback speed of the control is set to 0 so the control holds
// the animation at the correct pose. When we use these controls in an animated skeleton adjusting their weights
// blends in and out between the static poses.
//
// We would like this blend of static poses to become an input into the high level animation mixing.
// We need this result to be blended in and out as we go from state to state. To acheive this we wrap the
// AnimatedSkeleton in a hkaAnimation interface. We also create a new master control which scales the
// values of each of the controls mentioned above.
// This new input called AnimationMixerSkeletalAnimation and the new control AnimationMixerControl are added
// to the high level mixer m_animatedSkeleton
class AnimationMixerSkeletalAnimation : public hkaAnimation
{
	public:

	AnimationMixerSkeletalAnimation( class hkaAnimatedSkeleton* mixer )
		: m_mixer( mixer )
	{
		m_mixer->addReference();

		m_numberOfTransformTracks = (hkInt16)m_mixer->getSkeleton()->m_numBones;

		// Init a fake binding
		m_dummyBinding.m_animation = this;
		m_dummyBinding.m_numTransformTrackToBoneIndices = m_numberOfTransformTracks;
		m_dummyBinding.m_transformTrackToBoneIndices = hkAllocateChunk<hkInt16>( m_dummyBinding.m_numTransformTrackToBoneIndices, HK_MEMORY_CLASS_ANIM_RUNTIME);

		for (int i=0 ; i< m_dummyBinding.m_numTransformTrackToBoneIndices; i++)
		{
			m_dummyBinding.m_transformTrackToBoneIndices[i] = (hkInt16)i;
		}


		// The source of this animation is the result of a mix of poses so it has infinite length
		m_duration = HK_REAL_MAX;
	}

	~AnimationMixerSkeletalAnimation()
	{
		m_mixer->removeReference();
		hkDeallocateChunk( m_dummyBinding.m_transformTrackToBoneIndices, m_dummyBinding.m_numTransformTrackToBoneIndices, HK_MEMORY_CLASS_ANIM_RUNTIME );
	}

	virtual void sampleTracks(hkReal time, hkQsTransform* transformTracksOut, hkReal* floatTracksOut,  hkaChunkCache* cache) const
	{
		// When asked for a pose we query the mixer
		m_mixer->sampleAndCombineAnimations( transformTracksOut, floatTracksOut, cache );
	}

		/// Sample individual animation tracks
	virtual void sampleIndividualTransformTracks( hkReal time, const hkInt16* tracks, hkUint32 numTracks, hkQsTransform* transformOut ) const {}

		/// Sample a individual floating tracks
	virtual void sampleIndividualFloatTracks( hkReal time, const hkInt16* tracks, hkUint32 numTracks, hkReal* out ) const {}


	virtual int getNumOriginalFrames() const
	{
		return 0;
	}

	class hkaAnimationBinding m_dummyBinding;
	hkaAnimatedSkeleton* m_mixer;
};

class AnimationMixerControl : public hkaDefaultAnimationControl
{
	public:

	AnimationMixerControl( class hkaAnimatedSkeleton* mixer, hkaAnimationBinding* binding )
		: hkaDefaultAnimationControl( binding ), m_mixer( mixer )
	{
		m_mixer->addReference();
	}

	virtual ~AnimationMixerControl()
	{
		m_mixer->removeReference();
	}

	virtual void update( hkReal stepDelta )
	{
		// Update this control
		hkaDefaultAnimationControl::update( stepDelta );

		// Set the weights of the individual controls based on this one.
		for (int i=0 ; i < m_mixer->getNumAnimationControls(); i++)
		{
			// Scale each by our weight
			hkaDefaultAnimationControl* control = (hkaDefaultAnimationControl*)m_mixer->getAnimationControl(i);
			control->setMasterWeight( getWeight() );
		}

		// Update each of the controls in the mixer
		m_mixer->stepDeltaTime( stepDelta );
	}

	class hkaAnimatedSkeleton* m_mixer;
};



void Gdc2005Demo::initPoseMatching()
{
	// Select the bones to use for pose matching
	hkInt16 root = hkaSkeletonUtils::findBoneWithName( *m_ragdollInstance->getSkeleton(), "Biped_Root");
	hkInt16 other = hkaSkeletonUtils::findBoneWithName( *m_ragdollInstance->getSkeleton(), "Biped_Head");
	hkInt16 another = hkaSkeletonUtils::findBoneWithName( *m_ragdollInstance->getSkeleton(), "Biped_RightShoulder");

	// Construct a pose matching database
	m_poseMatchUtils = new hkaPoseMatchingUtility( root, other, another, UP);

	// Create a mixer for blending the get up poses
	hkaAnimatedSkeleton* getupPoseMixer = new hkaAnimatedSkeleton( const_cast<hkaSkeleton*>(m_animatedSkeleton->getSkeleton()) );

	// Convert this mixer into an animation source
	m_matchingPose = new AnimationMixerSkeletalAnimation( getupPoseMixer );

	// Wrap the controls in a master control
	m_matchingPoseControl = new AnimationMixerControl( getupPoseMixer , &m_matchingPose->m_dummyBinding );

	m_currentMatch = -1;
	m_timeSinceBetterMatch = 0.0f;

	// This control is added to the skeleton in the slot GDC_DEATH_CONTROL
	m_matchingPoseControl->easeOut(0.0f);
	m_animatedSkeleton->addAnimationControl( m_matchingPoseControl );

	// The get-up animations for the demo.
	hkArray<const char*> getUpAnimations;
	getUpAnimations.pushBack( "Resources/Animation/Showcase/Gdc2005/Animations/hkKnockdown2.hkx" );
	getUpAnimations.pushBack( "Resources/Animation/Showcase/Gdc2005/Animations/hkGetupBack1.hkx" );
	getUpAnimations.pushBack( "Resources/Animation/Showcase/Gdc2005/Animations/hkGetupFront1.hkx" );

	// Iterate through all the animations
	for( int curAnimation = 0; curAnimation < getUpAnimations.getSize(); curAnimation++ )
	{
		hkString assetFile = hkAssetManagementUtil::getFilePath( getUpAnimations[curAnimation] );
		hkRootLevelContainer* container = m_loader->load( assetFile.cString()  );
		hkaAnimationContainer* ac = reinterpret_cast<hkaAnimationContainer*>( container->findObjectByType( hkaAnimationContainerClass.getName() ));

		// Scan the animation for get up points
		for (int t=0; t< ac->m_bindings[0]->m_animation->m_numAnnotationTracks; t++)
		{
			hkaAnnotationTrack& track = *ac->m_bindings[0]->m_animation->m_annotationTracks[t];
			for (int a=0; a < track.m_numAnnotations; a++)
			{
				hkaAnnotationTrack::Annotation& note = track.m_annotations[a];

				// If we find an annotation
				if ( hkString::strCmp( "hkGetupYes", note.m_text) )
				{
					// Create a control that can be used to blend in and out this pose
					hkaDefaultAnimationControl* control = new hkaDefaultAnimationControl( ac->m_bindings[0] );
					control->easeOut(0.0f);
					// Set the local time fro the control to be the time annotated
					control->setLocalTime( note.m_time );
					// Set its speed to 0 so it holds the pose
					control->setPlaybackSpeed(0.0f);
					// Add it to the mixer
					getupPoseMixer->addAnimationControl( control );
					control->removeReference();

					// Sample the animation at this time and add ti to the pose database
					{
						// Grab the pose in model space at this frame
						hkaAnimatedSkeleton mixer(const_cast<hkaSkeleton*>(m_animatedSkeleton->getSkeleton()));
						mixer.addAnimationControl(control);
						const hkaSkeleton* aSkel = m_animatedSkeleton->getSkeleton();
						hkLocalBuffer< hkQsTransform > animPoseModelSpace( aSkel->m_numBones );
						mixer.sampleAndCombineAnimations( animPoseModelSpace.begin(), HK_NULL );

						hkaSkeletonUtils::transformLocalPoseToModelPose( aSkel->m_numBones, aSkel->m_parentIndices, animPoseModelSpace.begin(), animPoseModelSpace.begin() );

						// Map to lower res ragdoll
						const hkaSkeleton* skel = m_ragdollInstance->getSkeleton();
						hkLocalBuffer< hkQsTransform > referencePoseModel( m_ragdollInstance->getSkeleton()->m_numBones );
						hkaSkeletonUtils::transformLocalPoseToModelPose( skel->m_numBones, skel->m_parentIndices, skel->m_referencePose, referencePoseModel.begin() );
						m_highResToRagdollMapper->mapPose( animPoseModelSpace.begin(), skel->m_referencePose, referencePoseModel.begin(), hkaSkeletonMapper::CURRENT_POSE );

						// Add this pose to the database along with the time and binding info
						m_poseMatchUtils->addCandidatePose(referencePoseModel.begin(), ac->m_bindings[0], note.m_time);
					}
				}
			}
		}
	}
	getupPoseMixer->removeReference();
}


// matchPose() is called whenever you are in the dead state, it sets up the pose that the ragdoll will be driven to and prepares the
// get up animation to play from that pose.
void Gdc2005Demo::matchPose(hkaPose &thePose, hkReal& error)
{
	// Get the current ragdoll pose in world space
	const hkaSkeleton* rSkel = m_ragdollInstance->getSkeleton();
	hkLocalBuffer<hkQsTransform> ragdollWorldSpace( rSkel->m_numBones );
	m_ragdollInstance->getPoseModelSpace( ragdollWorldSpace.begin() , hkQsTransform::getIdentity() );

	// Find the right pose to map to
	const int match = m_poseMatchUtils->findBestCandidatePoseIndex( ragdollWorldSpace.begin(), error );

	// If no matches were found (maybe there were no candidates?) return
	if (match<0)
	{
		return;
	}

	// No previous match, we just started doing pose matching?
	if (m_currentMatch == -1)
	{
		// Ensure all other animations (controls) are out immediately
		for (int i=0; i < m_matchingPoseControl->m_mixer->getNumAnimationControls(); i++)
		{
			hkaDefaultAnimationControl* control = (hkaDefaultAnimationControl*)m_matchingPoseControl->m_mixer->getAnimationControl(i);
			control->easeOut(0.0f);
		}

		// And switch this one (the matched pose) on immediately
		hkaDefaultAnimationControl* control = (hkaDefaultAnimationControl*)m_matchingPoseControl->m_mixer->getAnimationControl(match);
		control->easeIn(0.0f);
		m_currentMatch = match;

		// Finally, prepare the getup animation for the chosen match
		hkaAnimationControl* getUpControl = m_animatedSkeleton->getAnimationControl( GDC_GET_UP_CONTROL );
		getUpControl->setLocalTime( m_poseMatchUtils->m_candidatePoses[match].m_time );
		getUpControl->setAnimationBinding( m_matchingPoseControl->m_mixer->getAnimationControl(match)->getAnimationBinding() );

		// We're done.
		return;
	}

	// Add some hysteresis : We want to wait for sometime before we switch, to avoid continuous toggling between very similar matches
	if (match == m_bestMatch)
	{
		m_timeSinceBetterMatch += m_timestep;
	}
	else
	{
		m_timeSinceBetterMatch = 0.0f;
	}
	m_bestMatch = match;

	// If we have changed from the last matched pose. We use a time threshold based on the error.
	hkReal timeThreshold = m_options.m_Matching.m_hysterisis * (1.0f - error);
	const bool switchToNewMatch = (match != m_currentMatch) && (m_timeSinceBetterMatch > timeThreshold);

	// We also don't want to switch to a new match if the ragdoll is settled. We decide this by checking the linear velocity
	// of the pelvis.
	const hkReal pelvisVelocity = m_ragdollInstance->getRigidBodyOfBone(0)->getLinearVelocity().length3();
	const bool deactivateMatching = (pelvisVelocity < 0.2f);
	if (deactivateMatching && m_options.m_Matching.m_showDeactivation)
	{
		HK_DISPLAY_STAR( m_ragdollInstance->getRigidBodyOfBone(0)->getPosition(), 1.0f, 0xffff0000);
	}

	// If we decide to switch..
	if (!deactivateMatching && switchToNewMatch)
	{
		// Blend in the new matched pose
		hkaDefaultAnimationControl* newControl = (hkaDefaultAnimationControl*)m_matchingPoseControl->m_mixer->getAnimationControl(match);
		newControl->easeIn(m_options.m_Matching.m_matchBlendSpeed);

		// Blend out the old one
		hkaDefaultAnimationControl* oldControl = (hkaDefaultAnimationControl*)m_matchingPoseControl->m_mixer->getAnimationControl(m_currentMatch);
		oldControl->easeOut(m_options.m_Matching.m_matchBlendSpeed);

		m_currentMatch  = match;
		m_timeSinceBetterMatch = 0.0f;
	}

	// We only update the the get up animation to point to our new matching pose
	// when we have blended in more than 50%
	if (m_matchingPoseControl->m_mixer->getAnimationControl(m_currentMatch)->getWeight() > 0.5f)
	{
		// Prepare the getup animation
		hkaAnimationControl* getUpControl = m_animatedSkeleton->getAnimationControl( GDC_GET_UP_CONTROL );
		getUpControl->setLocalTime( m_poseMatchUtils->m_candidatePoses[m_currentMatch].m_time );
		getUpControl->setAnimationBinding( m_matchingPoseControl->m_mixer->getAnimationControl(m_currentMatch)->getAnimationBinding() );
	}
}

void Gdc2005Demo::quitPoseMatching()
{
	m_matchingPose->removeReference();
	m_matchingPoseControl->removeReference();
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
