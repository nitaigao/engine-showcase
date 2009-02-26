/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2008 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_ANIMATED_DEMO_CHARACTER_H
#define HK_ANIMATED_DEMO_CHARACTER_H

#include <Demos/DemoCommon/Utilities/Character/DemoCharacter/SimpleDemoCharacter/SimpleDemoCharacter.h>

class hkLoader;

enum CharacterChoice
{
	HK_CHARACTER_ROY,
	HK_CHARACTER_LEON,
	HK_CHARACTER_PRIS,
	HK_CHARACTER_KHORA,
	HK_CHARACTER_HAVOK_GIRL,
	HK_CHARACTER_BADDY
};

struct AnimatedDemoCharacterAnimationSet  
{
	CharacterChoice				m_choice;
	class hkaSkeleton*			m_skeleton;
	class hkaAnimationBinding*	m_idle;
	class hkaAnimationBinding*	m_walk;
	class hkaAnimationBinding*	m_run;
	class hkaAnimationBinding*	m_jump;
	class hkaAnimationBinding*	m_inAir;
	class hkaAnimationBinding*	m_land;
	class hkaAnimationBinding*	m_dive;

	hkReal m_walkRunSyncOffset;	// Sync offset for walk to run 

	// Store a pointer to the rig container, so that other classes can get at ragdolls, mappers, etc.
	class hkRootLevelContainer* m_rigContainer;
};

class AnimatedDemoCharacterLoadingUtils
{
public:

	static void HK_CALL loadBasicAnimations( hkLoader* loader, struct AnimatedDemoCharacterAnimationSet* set, hkVector4& animFwdLocal, hkVector4& animUpLocal, CharacterChoice choice = HK_CHARACTER_BADDY );
};

struct AnimatedDemoCharacterCinfo : public DemoCharacterCinfo
{
	hkVector4		m_gravity;
	hkTransform		m_initialTransform;

	// For animation
	hkVector4		m_animationForwardLocal;
	hkVector4		m_animationUpLocal;

	struct AnimatedDemoCharacterAnimationSet* m_animationSet;
};

class AnimatedDemoCharacter : public DemoCharacter
{
	public:

			// Create a default first person character
		AnimatedDemoCharacter( AnimatedDemoCharacterCinfo& cinfo );

		~AnimatedDemoCharacter();

		// Update the character position
		void update( hkReal timestep, const CharacterStepInput& input, const struct CharacterActionInfo& actionInfo );

		virtual void display( hkReal timestep, hkDemoEnvironment* env );

		virtual hkReal getMaxVelocity() const;

			// Sample animations
		void updateAnimation( hkReal timestep, hkQsTransform* poseLS );

			// Do skinning etc.
		void updateDisplay( int numBones, const hkQsTransform* poseMS, hkDemoEnvironment* env  );

		// Get the forward direction in local space
		const hkVector4& getAnimationForwardLocal();

		// The the up direction in local space
		const hkVector4& getAnimationUpLocal();

		const hkaSkeleton* getSkeleton() const;

			
		void loadSkin( hkLoader* loader, hkDemoEnvironment* env, CharacterChoice choice = HK_CHARACTER_BADDY );

	public:

		// Utility update functions
		void updatePosition( hkReal timestep, const CharacterStepInput& input, bool& isSupportedOut );


	protected:

			// Creates animated skeletons and stats machine etc.
		virtual void initAnimation( const AnimatedDemoCharacterAnimationSet* set );


			// Renders the skin around the given pose
		void doSkinning ( int numBones, const hkQsTransform* poseMS, const hkTransform& worldFromModel, hkDemoEnvironment* env );
		
	public:

		hkVector4						m_gravity;

			// Animation
		hkVector4						m_animationForwardLocal;
		hkVector4						m_animationUpLocal;
		class SimpleBipedStateManager*	m_stateManager;
		class AnimationEventQueue*		m_animationMachine;
		class SimpleBipedStateContext*	m_animationStateMachine;
		class hkaAnimatedSkeleton*		m_animatedSkeleton;

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
};

#endif // HK_ANIMATED_DEMO_CHARACTER_H

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
