/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2008 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_RAGDOLL_DEMO_CHARACTER_H
#define HK_RAGDOLL_DEMO_CHARACTER_H

#include <Demos/DemoCommon/Utilities/Character/DemoCharacter/SimpleDemoCharacter/SimpleDemoCharacter.h>
#include <Demos/DemoCommon/Utilities/Character/DemoCharacter/AnimatedDemoCharacter/AnimatedDemoCharacter.h>

class hkLoader;

struct RagdollDemoCharacterAnimationSet : public AnimatedDemoCharacterAnimationSet
{
	class hkaAnimationBinding*	m_die;
	class hkaAnimationBinding*	m_getUp;

	class hkaAnimationBinding*	m_poseMatchingAnims[3];

	class hkaRagdollInstance* m_ragdollInstance;
	class hkaSkeletonMapper* m_highResToRagdollMapper;
	class hkaSkeletonMapper* m_ragdollToHighResMapper;
};

class RagdollDemoCharacterLoadingUtils
{
public:

	static void HK_CALL loadBasicAnimations( hkLoader* loader, struct RagdollDemoCharacterAnimationSet* set, hkVector4& animFwdLocal, hkVector4& animUpLocal, hkInt16 poseMatchingBones[3], CharacterChoice choice = HK_CHARACTER_LEON );
	//static void HK_CALL loadRagdollRig( hkLoader* loader );
};

struct RagdollDemoCharacterCinfo : public DemoCharacterCinfo
{
	RagdollDemoCharacterCinfo():
		m_world(HK_NULL),
		m_ragdollFilterLayer(0),
		m_proxyNormalCollisionFilterInfo(0), 
		m_proxyNoCollideCollisionFilterInfo(0),
		m_proxyGettingUpCollisionFilterInfo(0),
		m_motorForce(40.0f), 
		m_motorTau(.8f), 
		m_motorProportionalRecoveryVelocity(2.0f), 
		m_motorConstantRecoveryVelocity(1.0f)
		{}


	hkVector4		m_gravity;
	hkTransform		m_initialTransform;

	// For animation
	hkVector4		m_animationForwardLocal;
	hkVector4		m_animationUpLocal;

	struct RagdollDemoCharacterAnimationSet* m_animationSet;
	hkBool			m_shouldCloneRagdollInstance;
	class hkpWorld* m_world; // TODO get this from the character proxy?

	hkUint32 m_ragdollFilterLayer;
	hkUint32 m_proxyNormalCollisionFilterInfo; // most of the time
	hkUint32 m_proxyNoCollideCollisionFilterInfo; // when ragdoll, doesn't collide with anything
	hkUint32 m_proxyGettingUpCollisionFilterInfo; // doesn't collide with ragdoll, but collides with landscape/other characters

	hkInt16 m_poseMatchingBones[3];

	// Motor parameters
	// These default to reasonable values in the constructor
	hkReal m_motorForce;
	hkReal m_motorTau;
	hkReal m_motorProportionalRecoveryVelocity;
	hkReal m_motorConstantRecoveryVelocity;
};

class RagdollDemoCharacter : public DemoCharacter
{
	public:

			// Create a default first person character
		RagdollDemoCharacter( RagdollDemoCharacterCinfo& cinfo );

		~RagdollDemoCharacter();

		// Update the character position
		void update( hkReal timestep, const CharacterStepInput& input, const struct CharacterActionInfo& actionInfo );

		virtual void display( hkReal timestep, hkDemoEnvironment* env );

		virtual hkReal getMaxVelocity() const;

			// Sample animations
		void updateAnimation( hkReal timestep, hkQsTransform* poseLS );

		void updatePoseFromRagdoll(class hkaPose& pose);

		void updateRagdollDriving(hkReal timestep);

			// Do skinning etc.
		void updateDisplay( int numBones, const hkQsTransform* poseMS, hkDemoEnvironment* env  );

		void doGetup ( const hkVector4& characterPosition, hkaPose &pose, hkReal timestep );

		// Get the forward direction in local space
		const hkVector4& getAnimationForwardLocal();

		// The the up direction in local space
		const hkVector4& getAnimationUpLocal();

		const hkaSkeleton* getSkeleton() const;

			
		void loadSkin( hkLoader* loader, hkDemoEnvironment* env , CharacterChoice choice = HK_CHARACTER_LEON);

		void getWorldFromModel(hkQsTransform& tOut) const;
		void getWorldFromModel(hkTransform& tOut) const;

	public:

		// Utility update functions
		void updatePosition( hkReal timestep, const CharacterStepInput& input, bool& isSupportedOut );

		void addRagdollToWorld(hkReal timestep, hkQsTransform* oldAnimationModelSpace);

		void removeRagdollFromWorld();


	protected:

			// Creates animated skeletons and stats machine etc.
		virtual void initAnimation( const RagdollDemoCharacterAnimationSet* set );

		virtual void initPoseMatching();

			// Fix up motors, etc on the ragdoll
		virtual void initRagdoll(int ragdollLayer, hkReal force, hkReal tau, hkReal propRecoveryVel, hkReal conRecoveryVel);

		void updatePoseMatchWeights();
		void matchPose( hkReal& error, hkReal timestep );

			
		void startGetUp(); // Get on up
		void stopGetUp();


			// Renders the skin around the given pose
		void doSkinning ( int numBones, const hkQsTransform* poseMS, const hkTransform& worldFromModel, hkDemoEnvironment* env );

	public:

		hkVector4						m_gravity;

			// Animation
		hkVector4						m_animationForwardLocal;
		hkVector4						m_animationUpLocal;
		class RagdollBipedStateManager*	m_stateManager;
		class AnimationEventQueue*		m_animationMachine;
		class RagdollBipedStateContext*	m_wrapperStateMachine;
		class SimpleBipedStateContext*	m_animationStateMachine;
		class hkaAnimatedSkeleton*		m_animatedSkeleton;
		class hkaPose*					m_currentAnimationPose;

			// Ragdoll
		class hkaRagdollInstance*		m_ragdollInstance;
		class hkaSkeletonMapper* m_highResToRagdollMapper;
		class hkaSkeletonMapper* m_ragdollToHighResMapper;
		class hkpWorld* m_world;

			// PoseMatching
		struct PoseMatchingSystem
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO, PoseMatchingSystem);

			hkQsTransform m_ragdollFrame;
			hkQsTransform m_animFrame;
			class hkaAnimationBinding*	m_poseMatchingAnims[3];

			//class AnimationMixerSkeletalAnimation* m_matchingPose;
			//class AnimationMixerControl* m_matchingPoseControl;
			class hkaDefaultAnimationControl* m_blendControl;
			class hkaPoseMatchingUtility* m_poseMatchUtils;
			hkArray<class hkaDefaultAnimationControl*> m_mixerControls;

			hkBool m_initGetup;
			int m_currentMatch;
			int m_bestMatch;
			hkReal m_timeSinceBetterMatch;
			hkReal m_poseMatchingHysterisis;
			hkReal m_poseMatchingBlendSpeed;
			hkReal m_getupBlendSpeed;

			hkInt16 m_rootIdx;
			hkInt16 m_otherIdx;
			hkInt16 m_anotherIdx;

			PoseMatchingSystem() :
				m_blendControl(HK_NULL),
				m_poseMatchingHysterisis(0.2f),
				m_poseMatchingBlendSpeed(0.5f),
				m_getupBlendSpeed(.5f)
				{
					m_ragdollFrame.setIdentity();
					m_animFrame.setIdentity();
				}

		};

		PoseMatchingSystem* m_poseMatchingSystem;
		

			// Cached
		hkRotation				m_characterFromAnimation;

			// Used to filter the "supported" signal
		hkReal					m_timeUnsupported;

			// For skinning
		hkBool					m_skinsLoaded;
		hkBool					m_hardwareSkinning;
		class hkaMeshBinding**	m_skinBindings;
		hkInt32					m_numSkinBindings;
		hkBool					m_bUseHardwareSkinning;
		hkBool					m_bUseWorldHardwareSkinning;

			// For locomotion
		hkReal	m_walkVelocity;
		hkReal	m_runVelocity;

		hkUint32 m_proxyNormalCollisionFilterInfo; // most of the time
		hkUint32 m_proxyNoCollideCollisionFilterInfo; // when ragdoll, doesn't collide with anything
		hkUint32 m_proxyGettingUpCollisionFilterInfo; // doesn't collide with ragdoll, but collides with landscape/other characters
};

#endif // HK_RAGDOLL_DEMO_CHARACTER_H

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
