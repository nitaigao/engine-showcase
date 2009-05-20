/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


#include <Demos/demos.h>

#include <Demos/DemoCommon/Utilities/Character/DemoCharacter/AnimatedDemoCharacter/AnimatedDemoCharacter.h>
#include <Demos/DemoCommon/Utilities/Character/DemoCharacter/AnimatedDemoCharacter/RagdollDemoCharacter.h>
#include <Demos/DemoCommon/Utilities/Character/CharacterStepInput.h>
#include <Demos/DemoCommon/Utilities/Character/CharacterProxy/CharacterProxy.h>
#include <Demos/DemoCommon/Utilities/Character/CharacterUtils.h>

#include <Common/Visualize/hkDebugDisplay.h>
#include <Graphics/Bridge/SceneData/hkgSceneDataConverter.h>

// Serialization
#include <Common/Base/Reflection/hkClass.h>
#include <Common/Serialize/Util/hkLoader.h>
#include <Common/Serialize/Util/hkRootLevelContainer.h>
#include <Common/Base/Reflection/Registry/hkTypeInfoRegistry.h>
#include <Common/Serialize/Util/hkBuiltinTypeRegistry.h>
#include <Common/Base/Container/LocalArray/hkLocalBuffer.h>

// Utilities
#include <Demos/DemoCommon/Utilities/Asset/hkAssetManagementUtil.h>
#include <Demos/DemoCommon/Utilities/Animation/AnimationUtils.h>

// Skeletal Animation
#include <Animation/Animation/hkaAnimationContainer.h>
#include <Animation/Animation/Rig/hkaSkeleton.h>
#include <Animation/Animation/Rig/hkaSkeletonUtils.h>
#include <Animation/Animation/Playback/hkaAnimatedSkeleton.h>
#include <Animation/Animation/Playback/Control/Default/hkaDefaultAnimationControl.h>
#include <Animation/Animation/Animation/hkaAnimationBinding.h>

// Vertex Deformation
#include <Animation/Animation/Deform/Skinning/hkaMeshBinding.h>

// Scene Data
#include <Common/SceneData/Scene/hkxScene.h>
#include <Common/SceneData/Mesh/hkxMesh.h>
#include <Common/SceneData/Mesh/hkxMeshSection.h>
#include <Common/SceneData/Mesh/hkxIndexBuffer.h>
#include <Common/SceneData/Mesh/hkxVertexBuffer.h>
#include <Animation/Animation/Rig/hkaPose.h>

// State Machine
#include <Demos/DemoCommon/Utilities/Character/DemoCharacter/AnimatedDemoCharacter/StateMachine/SimpleBipedStateMachine.h>
#include <Demos/DemoCommon/Utilities/Character/DemoCharacter/AnimatedDemoCharacter/StateMachine/RagdollBipedStateMachine.h>
#include <Demos/DemoCommon/Utilities/Character/DemoCharacter/AnimatedDemoCharacter/StateMachine/States/SimpleBipedStates.h>
#include <Demos/DemoCommon/Utilities/Character/DemoCharacter/AnimatedDemoCharacter/StateMachine/States/RagdollBipedStates.h>

// Ragdoll
#include <Animation/Ragdoll/Instance/hkaRagdollInstance.h>
#include <Animation/Animation/Mapper/hkaSkeletonMapper.h>
#include <Animation/Ragdoll/Controller/PoweredConstraint/hkaRagdollPoweredConstraintController.h>
#include <Physics/Dynamics/Constraint/Bilateral/LimitedHinge/hkpLimitedHingeConstraintData.h>
#include <Physics/Dynamics/Constraint/Bilateral/Ragdoll/hkpRagdollConstraintData.h>
#include <Physics/Dynamics/Constraint/Motor/Position/hkpPositionConstraintMotor.h>
#include <Physics/Collide/Filter/Group/hkpGroupFilter.h>

// Pose matching/get up
#include <Animation/Ragdoll/PoseMatching/hkaPoseMatchingUtility.h>
#include <Physics/Collide/Query/CastUtil/hkpWorldRayCastInput.h>
#include <Physics/Collide/Query/CastUtil/hkpWorldRayCastOutput.h>
#include <Physics/Collide/Query/Collector/RayCollector/hkpClosestRayHitCollector.h>

// See what renderer we are using etc
#include <Graphics/Bridge/System/hkgSystem.h> // to figure put if we should hardware skin
#include <Graphics/Common/Shader/hkgShader.h>
#include <Graphics/Common/Shader/hkgShaderCollection.h>
#include <Graphics/Common/DisplayObject/hkgDisplayObject.h>


RagdollCharacterFactory::RagdollCharacterFactory( CharacterType defaultType )
:	AnimatedCharacterFactory( defaultType )
{
	loadRagdollAnimations( m_type );

	m_proxyNoCollideCollisionFilterInfo = hkpGroupFilter::calcFilterInfo(LAYER_COLLIDE_NONE);
	m_proxyGettingUpCollisionFilterInfo = hkpGroupFilter::calcFilterInfo(LAYER_GET_UP);
	m_ragdollFilterLayer = LAYER_RAGDOLL;

}

DemoCharacter* RagdollCharacterFactory::createCharacterUsingProxy( CharacterProxy* proxy, const hkVector4& gravity, hkDemoEnvironment* env )
{
	proxy->getWorldObject()->removeProperty(HK_PROPERTY_DEBUG_DISPLAY_COLOR);
	proxy->getWorldObject()->addProperty(HK_PROPERTY_DEBUG_DISPLAY_COLOR, 0x00FFFFFF);

	// Ragdoll Character
	RagdollDemoCharacterCinfo info;
	info.m_characterProxy = proxy;
	info.m_gravity = gravity;
	info.m_animationForwardLocal = m_animSet[m_type].m_animFwdLocal;

	info.m_animationUpLocal = m_animSet[m_type].m_animUpLocal;
	info.m_animationSet = &m_animSet[m_type];
	info.m_ragdollAnimationSet = &m_ragdollAnimSet[m_type];
	info.m_poseMatchingBones[0] = m_ragdollAnimSet[m_type].m_poseMatchingBones[0];
	info.m_poseMatchingBones[1] = m_ragdollAnimSet[m_type].m_poseMatchingBones[1];
	info.m_poseMatchingBones[2] = m_ragdollAnimSet[m_type].m_poseMatchingBones[2];
	
	
	// RagdollDemoCharacter will create its own clone of the ragdoll instance and handle the reference counting there.
	info.m_shouldCloneRagdollInstance = true;

	// TODO
	info.m_ragdollFilterLayer = m_ragdollFilterLayer;
	info.m_proxyNormalCollisionFilterInfo = proxy->getWorldObject()->getCollidable()->getCollisionFilterInfo();
	info.m_proxyNoCollideCollisionFilterInfo = m_proxyNoCollideCollisionFilterInfo;
	info.m_proxyGettingUpCollisionFilterInfo = m_proxyGettingUpCollisionFilterInfo;

	RagdollDemoCharacter* ragdollCharacter = new RagdollDemoCharacter( info );
	ragdollCharacter->loadSkin( m_loader, env, m_type );
	proxy->removeReference();

	return ragdollCharacter;
	
}

void RagdollCharacterFactory::loadRagdollAnimations( CharacterType type )
{
	hkRootLevelContainer* ragdollContainer = HK_NULL;

	RagdollDemoCharacterAnimationSet* set = &m_ragdollAnimSet[type];

	switch (type)
	{
	case FIREFIGHTER:
		// Load the ragdoll-specific animations
		{
			set->m_die	 = AnimationUtils::loadAnimation( *m_loader, "Resources/Animation/ShowCase/Gdc2005/Animations/hkDie.hkx" );
			set->m_getUp = AnimationUtils::loadAnimation( *m_loader, "Resources/Animation/ShowCase/Gdc2005/Animations/hkKnockdown2.hkx" );

			set->m_poseMatchingAnims[0] =  AnimationUtils::loadAnimation( *m_loader, "Resources/Animation/ShowCase/Gdc2005/Animations/hkKnockdown2.hkx" );
			set->m_poseMatchingAnims[1] =  AnimationUtils::loadAnimation( *m_loader, "Resources/Animation/ShowCase/Gdc2005/Animations/hkGetupBack1.hkx" );
			set->m_poseMatchingAnims[2] =  AnimationUtils::loadAnimation( *m_loader, "Resources/Animation/ShowCase/Gdc2005/Animations/hkGetupFront1.hkx" );
			ragdollContainer =  m_animSet[type].m_rigContainer;
		}
		break;
	case HAVOK_GIRL:
		// Load the ragdoll-specific animations
		{
			set->m_die	 = AnimationUtils::loadAnimation( *m_loader, "Resources/Animation/HavokGirl/hkProtect.hkx" );
			set->m_getUp = AnimationUtils::loadAnimation( *m_loader, "Resources/Animation/HavokGirl/hkIdle.hkx" );

			set->m_poseMatchingAnims[0] =  AnimationUtils::loadAnimation( *m_loader, "Resources/Animation/HavokGirl/hkIdle.hkx" );
			set->m_poseMatchingAnims[1] =  AnimationUtils::loadAnimation( *m_loader, "Resources/Animation/HavokGirl/hkIdle.hkx" );
			set->m_poseMatchingAnims[2] =  AnimationUtils::loadAnimation( *m_loader, "Resources/Animation/HavokGirl/hkIdle.hkx" );

			// Get the rig
			{
				hkString assetFile = hkAssetManagementUtil::getFilePath("Resources/Animation/Ragdoll/hires_ragdoll.hkx");
				ragdollContainer = m_loader->load(  assetFile.cString() );
			}
		}
		break;
	default:
		{
			HK_ASSERT2(0x0, 0, "Invalid character type");
		}
	}


	// Load the ragdoll instance and skeleton mappers
	{
		// The m_rigContainer was set in AnimatedDemoCharacterLoadingUtils
		HK_ASSERT2(0x27343437, ragdollContainer != HK_NULL , "Could not load asset");
		set->m_ragdollInstance = reinterpret_cast<hkaRagdollInstance*>( ragdollContainer->findObjectByType( hkaRagdollInstanceClass.getName() ));

		// Mappers
		const hkaSkeleton* ragdollSkeleton = set->m_ragdollInstance->getSkeleton();
		set->m_highResToRagdollMapper = HK_NULL;
		set->m_ragdollToHighResMapper = HK_NULL;
		void *objectFound = ragdollContainer->findObjectByType(hkaSkeletonMapperClass.getName());
		while (objectFound)
		{
			hkaSkeletonMapper* mapperFound = reinterpret_cast<hkaSkeletonMapper*> (objectFound);

			// Use the skeleton to determine which mapper is which
			if (mapperFound->m_mapping.m_skeletonA == ragdollSkeleton)
			{
				set->m_ragdollToHighResMapper = mapperFound;
			}
			else
			{
				HK_ASSERT(0,mapperFound->m_mapping.m_skeletonB == ragdollSkeleton);
				set->m_highResToRagdollMapper = mapperFound;
			}

			objectFound = ragdollContainer->findObjectByType(hkaSkeletonMapperClass.getName(), objectFound);
		}
		HK_ASSERT2(0, set->m_highResToRagdollMapper, "Couldn't load high-to-ragdoll mapping");
		HK_ASSERT2(0, set->m_ragdollToHighResMapper, "Couldn't load ragdoll-to-high mapping");
	}

	switch (type)
	{
	case FIREFIGHTER:
		set->m_poseMatchingBones[0] = hkaSkeletonUtils::findBoneWithName( *set->m_ragdollInstance->getSkeleton(), "Biped_Root");
		set->m_poseMatchingBones[1] = hkaSkeletonUtils::findBoneWithName( *set->m_ragdollInstance->getSkeleton(), "Biped_Head");
		set->m_poseMatchingBones[2] = hkaSkeletonUtils::findBoneWithName( *set->m_ragdollInstance->getSkeleton(), "Biped_RightShoulder");
		break;
	case HAVOK_GIRL:
		set->m_poseMatchingBones[0] = hkaSkeletonUtils::findBoneWithName( *set->m_ragdollInstance->getSkeleton(), "Ragdoll Head");
		set->m_poseMatchingBones[1] = hkaSkeletonUtils::findBoneWithName( *set->m_ragdollInstance->getSkeleton(), "HavokBipedRig Pelvis");
		set->m_poseMatchingBones[2] = hkaSkeletonUtils::findBoneWithName( *set->m_ragdollInstance->getSkeleton(), "Ragdoll R Clavicle");
		break;
	default:
		{
			HK_ASSERT2(0x0, 0, "Invalid character type");
		}
	}
}


RagdollDemoCharacter::RagdollDemoCharacter( RagdollDemoCharacterCinfo& info )
: DemoCharacter(info)
{
	m_gravity = info.m_gravity;

	//
	// Setup the proxyFromAnimation transform
	//
	m_animationUpLocal = info.m_animationUpLocal;
	m_animationForwardLocal = info.m_animationForwardLocal;

	m_proxyNormalCollisionFilterInfo = info.m_proxyNormalCollisionFilterInfo;
	m_proxyGettingUpCollisionFilterInfo = info.m_proxyGettingUpCollisionFilterInfo;
	m_proxyNoCollideCollisionFilterInfo = info.m_proxyNoCollideCollisionFilterInfo;

	hkVector4 right; right.setCross( m_animationForwardLocal, m_animationUpLocal );
	hkRotation animRot;	animRot.setCols(m_animationUpLocal, m_animationForwardLocal, right );
	animRot.transpose();

	hkVector4 characterUpLocal = m_characterProxy->getUpLocal();
	hkVector4 characterForwardLocal = m_characterProxy->getForwardLocal();
	HK_ASSERT( 0, characterUpLocal.dot3(characterForwardLocal) == 0);

	hkVector4 charRight; charRight.setCross( characterForwardLocal, characterUpLocal );
	hkRotation charRot;	charRot.setCols(characterUpLocal, characterForwardLocal, charRight);

	m_characterFromAnimation.setMul( charRot, animRot );

	m_skinsLoaded = false;

	// Clone?
	if (info.m_shouldCloneRagdollInstance)
	{
		m_ragdollInstance = info.m_ragdollAnimationSet->m_ragdollInstance->clone(hkpConstraintInstance::CLONE_DATAS_WITH_MOTORS);
	}
	else
	{
		m_ragdollInstance = info.m_ragdollAnimationSet->m_ragdollInstance;
		m_ragdollInstance->addReference();
	}

	initRagdoll(info.m_ragdollFilterLayer, info.m_motorForce, info.m_motorTau, info.m_motorProportionalRecoveryVelocity, info.m_motorConstantRecoveryVelocity);

	m_ragdollToHighResMapper = info.m_ragdollAnimationSet->m_ragdollToHighResMapper;
	m_highResToRagdollMapper = info.m_ragdollAnimationSet->m_highResToRagdollMapper;

	// This also updates the current animation pose
	initAnimation( info.m_ragdollAnimationSet, info.m_animationSet );

	m_poseMatchingSystem = new RagdollDemoCharacter::PoseMatchingSystem();
	m_poseMatchingSystem->m_rootIdx = info.m_poseMatchingBones[0];
	m_poseMatchingSystem->m_otherIdx = info.m_poseMatchingBones[1];
	m_poseMatchingSystem->m_anotherIdx = info.m_poseMatchingBones[2];

	m_poseMatchingSystem->m_poseMatchingAnims[0] = info.m_ragdollAnimationSet->m_poseMatchingAnims[0];
	m_poseMatchingSystem->m_poseMatchingAnims[1] = info.m_ragdollAnimationSet->m_poseMatchingAnims[1];
	m_poseMatchingSystem->m_poseMatchingAnims[2] = info.m_ragdollAnimationSet->m_poseMatchingAnims[2];

	initPoseMatching();

}

RagdollDemoCharacter::~RagdollDemoCharacter()
{
	delete m_poseMatchingSystem->m_poseMatchUtils;

	for (int i=0; i<m_poseMatchingSystem->m_mixerControls.getSize(); i++)
	{
		m_poseMatchingSystem->m_mixerControls[i]->removeReference();
	}

	if(m_poseMatchingSystem->m_blendControl)
	{
		m_poseMatchingSystem->m_blendControl->removeReference();
	}

	delete m_poseMatchingSystem;

	delete m_currentAnimationPose;

	m_wrapperStateMachine->removeReference();
	m_animationStateMachine->removeReference();
	m_animationMachine->removeReference();

	m_ragdollInstance->removeReference();
}

const hkaSkeleton* RagdollDemoCharacter::getSkeleton() const
{
	return m_animatedSkeleton->getSkeleton();
}

void AnimatedDemoCharacter_setSkinningShader( hkgShaderCollection* shader, hkgDisplayObject* skin);
hkgShaderCollection* AnimatedDemoCharacter_loadSkinningShader(hkDemoEnvironment* env);
bool AnimatedDemoCharacter_supportsHardwareSkinning(hkDemoEnvironment* env);

void RagdollDemoCharacter::loadSkin( hkLoader* m_loader, hkDemoEnvironment* env, AnimatedCharacterFactory::CharacterType choice )
{
	m_hardwareSkinning = AnimatedDemoCharacter_supportsHardwareSkinning(env);

	hkString assetFile;

	switch ( choice )
	{
	case AnimatedCharacterFactory::HAVOK_GIRL:
		assetFile = hkAssetManagementUtil::getFilePath("Resources/Animation/HavokGirl/hkLowResSkinWithEyes.hkx");
		m_hardwareSkinning = false;
		break;
	case AnimatedCharacterFactory::FIREFIGHTER:
		{
			assetFile = hkAssetManagementUtil::getFilePath("Resources/Animation/Showcase/Gdc2005/Model/Firefighter_Skin.hkx"); 
			break;
		}
		break;
	default: 
		break;
	}


	hkRootLevelContainer* rootContainer = m_loader->load( assetFile.cString() );
	HK_ASSERT2(0x27343437, rootContainer != HK_NULL , "Could not load asset");
	hkaAnimationContainer* ac = reinterpret_cast<hkaAnimationContainer*>( rootContainer->findObjectByType( hkaAnimationContainerClass.getName() ));

	HK_ASSERT2(0x27343435, ac && (ac->m_numSkins > 0), "No skins loaded");

	m_numSkinBindings = ac->m_numSkins;
	m_skinBindings = ac->m_skins;

	hkxScene* scene = reinterpret_cast<hkxScene*>( rootContainer->findObjectByType( hkxSceneClass.getName() ));
	HK_ASSERT2(0x27343435, scene , "No scene loaded");

	int numPrevSkins = env->m_sceneConverter->m_addedSkins.getSize();

	// Make graphics output buffers for the skins
	env->m_sceneConverter->setAllowHardwareSkinning( m_hardwareSkinning ); // will enable the added blend info in the vertex buffers
	env->m_sceneConverter->convert( scene );

	m_skinsLoaded = true;

	if (m_hardwareSkinning)
	{
		for (int ms=0; ms < m_numSkinBindings; ++ms)
		{
			hkaMeshBinding* skinBinding = m_skinBindings[ms];
			if ( !env->m_sceneConverter->setupHardwareSkin( env->m_window->getContext(), skinBinding->m_mesh,
				reinterpret_cast<hkgAssetConverter::IndexMapping*>( skinBinding->m_mappings ),
				skinBinding->m_numMappings, (hkInt16)skinBinding->m_skeleton->m_numBones ) ) 
			{

				HK_WARN_ALWAYS( 0x4327defe, "Could not setup the blend matrices for a skin. Will not be able to skin in h/w.");
			}
		}

		int numSkins = env->m_sceneConverter->m_addedSkins.getSize();

		if (m_hardwareSkinning && (numPrevSkins < numSkins) )
		{
			hkgShaderCollection* shaderSet = AnimatedDemoCharacter_loadSkinningShader(env);
			for (int s=numPrevSkins; m_hardwareSkinning && (s < numSkins); ++s )
			{
				AnimatedDemoCharacter_setSkinningShader(shaderSet, env->m_sceneConverter->m_addedSkins[s]->m_skin);
			}
		}

	}

}

void RagdollDemoCharacter::initAnimation( const RagdollDemoCharacterAnimationSet* ragdollSet,  const AnimatedDemoCharacterAnimationSet* set  )
{
	// Initialize the state machine.
	RagdollBipedStateManager* wrapperManager = new RagdollBipedStateManager();
	SimpleBipedStateManager* stateManager = new SimpleBipedStateManager();

	{
		SimpleBipedState* state = new SimpleBipedStandState();
		stateManager->registerState (STAND_STATE, state);
		state->removeReference();

		state = new SimpleBipedWalkState(set->m_walkRunSyncOffset);
		stateManager->registerState (WALK_STATE, state);
		state->removeReference();

		state = new SimpleBipedJumpState();
		stateManager->registerState (JUMP_STATE, state);
		state->removeReference();

		state = new SimpleBipedInAirState();
		stateManager->registerState (IN_AIR_STATE, state);
		state->removeReference();

		state = new SimpleBipedLandState();
		stateManager->registerState (LAND_STATE, state);
		state->removeReference();

		state = new SimpleBipedDiveState();
		stateManager->registerState (DIVE_STATE, state);
		state->removeReference();
	}

	{
		// <ce.todo.aa> Only use one wrapper state?
		// Setup the wrappers around the pure-animation states
		RagdollBipedWrapperState* wrapperState = new RagdollBipedWrapperState(stateManager->getState(STAND_STATE));
		wrapperManager->registerState(STAND_STATE, wrapperState);
		wrapperState->removeReference();

		wrapperState = new RagdollBipedWrapperState(stateManager->getState(WALK_STATE));
		wrapperManager->registerState(WALK_STATE, wrapperState);
		wrapperState->removeReference();

		wrapperState = new RagdollBipedWrapperState(stateManager->getState(JUMP_STATE));
		wrapperManager->registerState(JUMP_STATE, wrapperState);
		wrapperState->removeReference();

		wrapperState = new RagdollBipedWrapperState(stateManager->getState(IN_AIR_STATE));
		wrapperManager->registerState(IN_AIR_STATE, wrapperState);
		wrapperState->removeReference();

		wrapperState = new RagdollBipedWrapperState(stateManager->getState(LAND_STATE));
		wrapperManager->registerState(LAND_STATE, wrapperState);
		wrapperState->removeReference();

		wrapperState = new RagdollBipedWrapperState(stateManager->getState(DIVE_STATE));
		wrapperManager->registerState(DIVE_STATE, wrapperState);
		wrapperState->removeReference();


		// Set up the ragdoll-specific states
		RagdollBipedState* state = new RagdollBipedDeadState();
		wrapperManager->registerState(DEAD_STATE, state);
		state->removeReference();

		state = new RagdollBipedDyingState();
		wrapperManager->registerState(DYING_STATE, state);
		state->removeReference();

		state = new RagdollBipedGettingUpState();
		wrapperManager->registerState(GETTING_UP_STATE, state);
		state->removeReference();
	}

	m_animatedSkeleton = new hkaAnimatedSkeleton( set->m_skeleton );

	// Add animations in order
	{
		// MOVE_CONTROL - A Dummy control
		hkaDefaultAnimationControl* control;

		// WALK_CONTROL
		control = new hkaDefaultAnimationControl( HK_NULL );
		control->easeOut(0.0f);
		control->setMasterWeight(0.01f);
		m_animatedSkeleton->addAnimationControl( control );
		control->removeReference();

		// IDLE_CONTROL
		control = new hkaDefaultAnimationControl( set->m_idle );
		control->easeOut(0.0f);
		m_animatedSkeleton->addAnimationControl( control );
		control->removeReference();

		// JUMP_CONTROL
		control = new hkaDefaultAnimationControl( set->m_jump );
		control->easeOut(0.0f);
		m_animatedSkeleton->addAnimationControl( control );
		control->removeReference();

		// IN_AIR_CONTROL
		control = new hkaDefaultAnimationControl( set->m_inAir );
		control->easeOut(0.0f);
		m_animatedSkeleton->addAnimationControl( control );
		control->removeReference();

		// LAND_CONTROL
		control = new hkaDefaultAnimationControl( set->m_land );
		control->easeOut(0.0f);
		m_animatedSkeleton->addAnimationControl( control );
		control->removeReference();

		// WALK_CONTROL
		control = new hkaDefaultAnimationControl( set->m_walk );
		control->setMasterWeight( 0.0f );
		m_animatedSkeleton->addAnimationControl( control );
		control->removeReference();

		// RUN_CONTROL
		control = new hkaDefaultAnimationControl( set->m_run );
		// To sync with walk
		control->setLocalTime( 17.0f / 60.0f );
		control->setMasterWeight( 0.0f );
		m_animatedSkeleton->addAnimationControl( control );
		control->removeReference();

		// DIVE_CONTROL
		control = new hkaDefaultAnimationControl( set->m_dive );
		control->easeOut(0.0f);
		m_animatedSkeleton->addAnimationControl( control );
		control->removeReference();

		// DYING_CONTROL
		control = new hkaDefaultAnimationControl( ragdollSet->m_die );
		control->easeOut(0.0f);
		m_animatedSkeleton->addAnimationControl( control );
		control->removeReference();

		// GET_UP_CONTROL
		control = new hkaDefaultAnimationControl( ragdollSet->m_getUp );
		control->easeOut(0.0f);
		control->setPlaybackSpeed(0.0f);
		m_animatedSkeleton->addAnimationControl( control );
		control->removeReference();

		// DEATH_CONTROL gets added later
	}

	// Initialize the animation command processor
	{
		m_animationMachine = new AnimationEventQueue(m_animatedSkeleton);
		m_animatedSkeleton->removeReference();
	}

	// Initialize the state machine
	{
		m_wrapperStateMachine = new RagdollBipedStateContext( wrapperManager );
		m_animationStateMachine = new SimpleBipedStateContext( stateManager );
		stateManager->removeReference();
		wrapperManager->removeReference();
		m_wrapperStateMachine->setCurrentState(STAND_STATE, m_animationMachine );
	}

	// Work out motion constraints
	hkaDefaultAnimationControl* walkControl = (hkaDefaultAnimationControl*)m_animatedSkeleton->getAnimationControl( WALK_CONTROL );
	hkaAnimation* walkAnimation = walkControl->getAnimationBinding()->m_animation;
	hkQsTransform walkMotion;
	walkAnimation->getExtractedMotionReferenceFrame(walkAnimation->m_duration, walkMotion );
	m_walkVelocity = hkReal(walkMotion.m_translation.length3()) / walkAnimation->m_duration;

	hkaDefaultAnimationControl* runControl = (hkaDefaultAnimationControl*)m_animatedSkeleton->getAnimationControl( RUN_CONTROL );
	hkaAnimation* runAnimation = runControl->getAnimationBinding()->m_animation;
	hkQsTransform runMotion;
	runAnimation->getExtractedMotionReferenceFrame(runAnimation->m_duration, runMotion );
	m_runVelocity = hkReal(runMotion.m_translation.length3()) / runAnimation->m_duration;

	// Set up the persistent pose
	m_currentAnimationPose = new hkaPose(getSkeleton());
	m_currentAnimationPose->setToReferencePose();
	m_animatedSkeleton->sampleAndCombineAnimations(m_currentAnimationPose->accessUnsyncedPoseLocalSpace().begin(), HK_NULL);
}

void RagdollDemoCharacter::updatePosition( hkReal timestep, const CharacterStepInput& input, bool& isSupportedOut )
{
	// Synchronize walk and run so it transitions smoothly
	{
		hkaDefaultAnimationControl* walkControl = (hkaDefaultAnimationControl*)m_animatedSkeleton->getAnimationControl( WALK_CONTROL );
		hkaDefaultAnimationControl* runControl = (hkaDefaultAnimationControl*)m_animatedSkeleton->getAnimationControl( RUN_CONTROL );

		hkReal forwardWalkRunBlend, walkSpeed, runSpeed;
		CharacterUtils::computeBlendParams( input.m_forwardVelocity, m_walkVelocity, m_runVelocity, 
			walkControl->getAnimationBinding()->m_animation->m_duration, 
			runControl->getAnimationBinding()->m_animation->m_duration,
			forwardWalkRunBlend,
			walkSpeed,
			runSpeed);

		runControl->setPlaybackSpeed( runSpeed );
		walkControl->setPlaybackSpeed( walkSpeed );

		const hkaDefaultAnimationControl* control = (hkaDefaultAnimationControl*)m_animatedSkeleton->getAnimationControl( MOVE_CONTROL );
		const hkReal controlWeight = control->getWeight() / control->getMasterWeight();
		runControl->setMasterWeight( forwardWalkRunBlend * controlWeight );
		walkControl->setMasterWeight( (1.0f - forwardWalkRunBlend) * controlWeight );
	}	

	/*
	// Map forward speed to blend between walk and run animations
	hkReal forwardWalkRunBlend = hkMath::clamp( (( input.m_forwardVelocity - m_walkVelocity) / (m_runVelocity - m_walkVelocity)), 0.f, 1.f);


	// Synchronize walk and run so it transitions smoothly
	{
		hkaDefaultAnimationControl* walkControl = (hkaDefaultAnimationControl*)m_animatedSkeleton->getAnimationControl( WALK_CONTROL );
		hkaDefaultAnimationControl* runControl = (hkaDefaultAnimationControl*)m_animatedSkeleton->getAnimationControl( RUN_CONTROL );

		const hkReal runWeight = forwardWalkRunBlend;
		const hkReal walkWeight = 1.0f - forwardWalkRunBlend;

		// Sync playback speeds
		{
			const hkReal totalW = runWeight+walkWeight+1e-6f;
			const hkReal walkP = walkWeight / totalW;
			const hkReal runP = runWeight / totalW;
			const hkReal runWalkRatio = runControl->getAnimationBinding()->m_animation->m_duration / walkControl->getAnimationBinding()->m_animation->m_duration;
			runControl->setPlaybackSpeed( (1-runP) * runWalkRatio + runP );
			walkControl->setPlaybackSpeed( (1-walkP) * (1.0f / runWalkRatio) + walkP );
		}

		const hkaDefaultAnimationControl* control = (hkaDefaultAnimationControl*)m_animatedSkeleton->getAnimationControl( MOVE_CONTROL );
		const hkReal controlWeight = control->getWeight() / control->getMasterWeight();
		runControl->setMasterWeight( runWeight * controlWeight );
		walkControl->setMasterWeight( walkWeight * controlWeight );
	}
	*/

	// Advance the active animations
	m_animatedSkeleton->stepDeltaTime( timestep );
	updatePoseMatchWeights();

	const hkUint32 currentAnimationState = m_wrapperStateMachine->getCurrentState();

	// Check support and filter
	bool supported = m_characterProxy->isSupported( timestep );
	{
		m_timeUnsupported = supported ? 0.0f : m_timeUnsupported + timestep;
		isSupportedOut = supported || (m_timeUnsupported < 0.45f);
	}

	// Compute the motion for the proxy
	hkVector4 extractedMotionCS;
	{
		hkQsTransform desiredMotionAS;
		m_animatedSkeleton->getDeltaReferenceFrame( timestep, desiredMotionAS );
		extractedMotionCS.setRotatedDir( m_characterFromAnimation, desiredMotionAS.getTranslation() );
	}

	// Calculate the velocity we need stateInput order to achieve the desired motion
	hkVector4 desiredVelocityWS;
	{

		hkTransform wFc;
		m_characterProxy->getTransform( wFc );

		hkVector4 desiredMotionWorld;
		desiredMotionWorld.setRotatedDir( wFc.getRotation(), extractedMotionCS );

		// Divide motion by time
		desiredVelocityWS.setMul4 ( 1.0f / timestep, desiredMotionWorld );
	}

	hkReal turnAngle = input.m_turnVelocity * timestep;
	// Ignore turn input when dead/dying/getting up
	if (currentAnimationState != DYING_STATE && currentAnimationState != DEAD_STATE && currentAnimationState != GETTING_UP_STATE)
	{
		hkQuaternion newRotation; newRotation.setAxisAngle( m_characterProxy->getUpLocal(), turnAngle );

		hkTransform wFc;
		m_characterProxy->getTransform( wFc );
		hkRotation r; r.set( newRotation );
		wFc.getRotation().mul( r );
		m_characterProxy->setTransform( wFc );
	}

	// Adjust velocity
	{
		hkVector4 characterLinearVelocity;
		m_characterProxy->getLinearVelocity( characterLinearVelocity );

		if (currentAnimationState != JUMP_STATE)
		{
			// in these states we ignore the motion coming from the animation
			if ((currentAnimationState == IN_AIR_STATE) || (currentAnimationState == DYING_STATE) || (currentAnimationState == DEAD_STATE))
			{
				desiredVelocityWS = characterLinearVelocity;
			}
			else
			{

				// this is the common case: add the motion velocity to the downward part of the proxy velocity (not upward though)
				hkReal vertComponent = hkMath::min2(0.0f, static_cast<hkReal>(characterLinearVelocity.dot3( m_characterProxy->getUpLocal() )));
				desiredVelocityWS.addMul4(vertComponent, m_characterProxy->getUpLocal() );
			}
		}
		else
		{
			// when jumping, add the motion velocity to the vertical proxy velocity
			hkReal vertComponent = characterLinearVelocity.dot3(m_characterProxy->getUpLocal());
			desiredVelocityWS.addMul4(vertComponent, m_characterProxy->getUpLocal());
		}


		// Just started jumping? Add some extra impulse
		if (m_characterProxy->isSupported(timestep))
		{
			desiredVelocityWS.addMul4( input.m_jumpVelocity, m_characterProxy->getUpLocal() );
		}
		else
		{
			// Add some gravity if not supported
			desiredVelocityWS.addMul4( timestep, m_gravity );
		}
	}

	// Apply velocity to character
	m_characterProxy->setLinearVelocity( desiredVelocityWS );

}

void RagdollDemoCharacter::getWorldFromModel(hkTransform& tOut) const
{
	m_characterProxy->getTransform( tOut );
	tOut.getRotation().mul( m_characterFromAnimation );
}

void RagdollDemoCharacter::getWorldFromModel(hkQsTransform& tOut) const
{
	hkTransform worldFromModel;
	getWorldFromModel(worldFromModel);
	tOut.setFromTransformNoScale( worldFromModel );
}

void RagdollDemoCharacter::updateDisplay( int numBones, const hkQsTransform* poseMS, hkDemoEnvironment* env )
{
	hkTransform worldFromModel; getWorldFromModel(worldFromModel);

	if ( m_skinsLoaded )
	{
		doSkinning ( numBones, poseMS, worldFromModel, env);
	}
	else
	{
		hkQsTransform tWorldFromModel; tWorldFromModel.setFromTransformNoScale( worldFromModel );
		AnimationUtils::drawPoseModelSpace( *m_animatedSkeleton->getSkeleton(), poseMS, tWorldFromModel );
	}
}

void RagdollDemoCharacter::display( hkReal timestep, hkDemoEnvironment* env )
{
	// If it makes sense, grab the ragdoll's pose and use that for skinning.
	// Otherwise, we'll end up using the last sampled pose.
	if(m_ragdollInstance->getWorld())
	{
		updatePoseFromRagdoll(*m_currentAnimationPose);
	}

	updateDisplay( getSkeleton()->m_numBones, m_currentAnimationPose->getSyncedPoseModelSpace().begin(),  env);
}


void RagdollDemoCharacter::updateAnimation( hkReal timestep, hkQsTransform* poseLS )
{
	m_animatedSkeleton->sampleAndCombineAnimations( poseLS, HK_NULL );
}

void RagdollDemoCharacter::updatePoseFromRagdoll(hkaPose& pose)
{
	const hkaSkeleton* rSkel = m_ragdollInstance->getSkeleton();

	// Get the T pose of the ragdoll
	hkLocalBuffer<hkQsTransform> ragdollModelSpace( rSkel->m_numBones );

	hkTransform worldFromModel; getWorldFromModel(worldFromModel);

	hkQuaternion rot(worldFromModel.getRotation());
	hkQsTransform worldFromModelQst(worldFromModel.getTranslation(), rot);

	m_ragdollInstance->getPoseModelSpace(ragdollModelSpace.begin(), worldFromModelQst );

	const hkQsTransform* localBIn  = pose.getSyncedPoseLocalSpace().begin();
	hkQsTransform*       modelBOut = pose.accessSyncedPoseModelSpace().begin();
	m_ragdollToHighResMapper->mapPose( ragdollModelSpace.begin(), localBIn, modelBOut, hkaSkeletonMapper::CURRENT_POSE );

}


void RagdollDemoCharacter::update( hkReal timestep, hkpWorld* world, const CharacterStepInput& input, struct CharacterActionInfo* actionInfo )
{
	bool isSupported;

	hkUint32 oldState = m_wrapperStateMachine->getCurrentState();
	hkBool deadOrDying = (oldState == DYING_STATE) || (oldState == DEAD_STATE);

	updatePosition( timestep, input, isSupported );

	RagdollBipedStateInput stateInput;
	stateInput.m_shouldWalk = input.m_forwardVelocity > 0.01f;
	stateInput.m_isSupported = isSupported;
	stateInput.m_shouldJump = input.m_jumpVelocity > 0;
	stateInput.m_animMachine = m_animationMachine;
	stateInput.m_context = m_animationStateMachine;
	stateInput.m_ragdollContext = m_wrapperStateMachine;

	if (actionInfo != HK_NULL )
	{
		stateInput.m_shouldDive = actionInfo->m_wasDivePressed;

		// Only die if not already dead
		stateInput.m_shouldDie = deadOrDying ? hkBool(false) : actionInfo->m_wasDiePressed;
		// Can only get up if dead
		stateInput.m_shouldGetUp = (oldState != DEAD_STATE) ? hkBool(false) : actionInfo->m_wasGetUpPressed;
	}
	else
	{
		stateInput.m_shouldDive = false;

		// Only die if not already dead
		stateInput.m_shouldDie = false;
		// Can only get up if dead
		stateInput.m_shouldGetUp = false;
	}

	// check if we need to remove the ragdoll
	if (!deadOrDying && m_ragdollInstance->getWorld())
	{
		removeRagdollFromWorld();
	}

	// If we went to DYING last frame, the ragdoll isn't in the world yet, so add it.
	if (deadOrDying && !m_ragdollInstance->getWorld())
	{
		hkLocalBuffer<hkQsTransform> oldAnimationModelSpace( getSkeleton()->m_numBones );
		hkString::memCpy(oldAnimationModelSpace.begin(), m_currentAnimationPose->getSyncedPoseModelSpace().begin(), getSkeleton()->m_numBones*sizeof(hkQsTransform));
		updateAnimation( timestep, m_currentAnimationPose->accessUnsyncedPoseLocalSpace().begin());

		addRagdollToWorld(timestep, world, oldAnimationModelSpace.begin());
	}
	else
	{
		updateAnimation( timestep, m_currentAnimationPose->accessUnsyncedPoseLocalSpace().begin());
	}

	// Don't drive unless the the ragdoll is in the world
	if (deadOrDying)
	{
		updateRagdollDriving(timestep);
	}

	// Only pose match when dead. We may need to feed the error back somewhere else later...
	if(oldState == DEAD_STATE)
	{
		hkReal dummyError;
		matchPose(dummyError, timestep);
	}
	else
	{
		m_poseMatchingSystem->m_currentMatch = -1;
	}

	if (oldState == GETTING_UP_STATE)
	{
		hkTransform t; getProxy()->getTransform(t);
		doGetup(t.getTranslation(), *m_currentAnimationPose, timestep);
	}

	// Update animation state machine
	m_wrapperStateMachine->update( timestep, &stateInput );
	m_animationMachine->update( timestep );

	// Synch things up
	m_animationStateMachine->m_currentTime = m_wrapperStateMachine->getCurrentTime();
	m_animationStateMachine->m_lastStateChange = m_wrapperStateMachine->getLastStateChange();
	m_animationStateMachine->m_currentStateId = m_wrapperStateMachine->getCurrentState();

	// Check for state changes
	hkUint32 currentState = m_wrapperStateMachine->getCurrentState();
	hkBool newDeadOrDying = (currentState == DYING_STATE) || (currentState == DEAD_STATE);

	if (oldState != GETTING_UP_STATE && currentState == GETTING_UP_STATE)
	{
		startGetUp( world );
	}


	if (oldState == GETTING_UP_STATE && currentState != GETTING_UP_STATE)
	{
		stopGetUp();
	}

}


// doSkinning() just renders (skins) the given pose
void RagdollDemoCharacter::doSkinning (const int boneCount, const hkQsTransform* poseMS, const hkTransform& worldFromModel, hkDemoEnvironment* env )
{
	// Construct the composite world transform
	hkLocalArray<hkTransform> compositeWorldInverse( boneCount );
	compositeWorldInverse.setSize( boneCount );

	// Skin the meshes
	for (int i=0; i < m_numSkinBindings; i++)
	{
		const hkxMesh* inputMesh = m_skinBindings[i]->m_mesh;

		// assumes either a straight map (null map) or a single one (1 palette)
		hkInt16* usedBones = m_skinBindings[i]->m_mappings? m_skinBindings[i]->m_mappings[0].m_mapping : HK_NULL;
		int numUsedBones = usedBones? m_skinBindings[i]->m_mappings[0].m_numMapping : boneCount;

		// Multiply through by the bind pose inverse world inverse matrices
		for (int p=0; p < numUsedBones; p++)
		{
			int boneIndex = usedBones? usedBones[p] : p;
			hkTransform tWorld; poseMS[ boneIndex ].copyToTransform(tWorld);
			compositeWorldInverse[p].setMul( tWorld, m_skinBindings[i]->m_boneFromSkinMeshTransforms[ boneIndex ] );
		}

		// use FPU skinning
		AnimationUtils::skinMesh( *inputMesh, worldFromModel, compositeWorldInverse.begin(), *env->m_sceneConverter );
	}
}

hkReal RagdollDemoCharacter::getMaxVelocity() const
{
	return m_runVelocity;
}

// BEGIN Ragdoll handling code

void RagdollDemoCharacter::initRagdoll(int  ragdollLayer, hkReal force, hkReal tau, hkReal propRecoveryVel, hkReal conRecoveryVel)
{

	const hkArray<hkpRigidBody*>& bodies = m_ragdollInstance->getRigidBodyArray();
	for (int i=0; i<bodies.getSize(); i++)
	{
		hkUint32 oldFilterInfo = bodies[i]->getCollisionFilterInfo();
		hkUint32 newFilterInfo = hkpGroupFilter::calcFilterInfo(ragdollLayer, hkpGroupFilter::getSystemGroupFromFilterInfo(oldFilterInfo),
			hkpGroupFilter::getSubSystemIdFromFilterInfo(oldFilterInfo), hkpGroupFilter::getSubSystemDontCollideWithFromFilterInfo(oldFilterInfo) );
		bodies[i]->setCollisionFilterInfo(newFilterInfo);
	}

	hkLocalArray<hkpPositionConstraintMotor*> motors( m_ragdollInstance->getNumBones() );

	// Set the motor parameters on all the ragdoll's motors
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

void RagdollDemoCharacter::addRagdollToWorld(hkReal timestep, hkpWorld* world, hkQsTransform* oldAnimationModelSpace)
{
	// First of all, disable the proxy's collisions
	getProxy()->setCollisionFilterInfo(m_proxyNoCollideCollisionFilterInfo);

	hkTransform worldFromModel; getWorldFromModel(worldFromModel);
	hkQsTransform worldFromModelQst; getWorldFromModel(worldFromModelQst);

	hkQsTransform futureQst = worldFromModelQst;
	hkVector4 vel; getProxy()->getLinearVelocity(vel);
	futureQst.m_translation.addMul4(timestep, vel);

	hkaPose ragdollPose (m_ragdollInstance->getSkeleton());
	ragdollPose.setToReferencePose();
	m_highResToRagdollMapper->mapPose(*m_currentAnimationPose, ragdollPose, hkaSkeletonMapper::CURRENT_POSE);

	// Set velocities correctly when spawning

	hkaPose oldRagdollPose(m_ragdollInstance->getSkeleton());
	oldRagdollPose.setToReferencePose();

	const hkQsTransform* oldRagdollPoseLocal = oldRagdollPose.getSyncedPoseLocalSpace().begin();
	hkQsTransform* oldRagdollPoseModel = oldRagdollPose.accessSyncedPoseModelSpace().begin();
	m_highResToRagdollMapper->mapPose(	oldAnimationModelSpace, oldRagdollPoseLocal, oldRagdollPoseModel, hkaSkeletonMapper::CURRENT_POSE);

	m_ragdollInstance->setPoseAndVelocitiesModelSpace(oldRagdollPose.getSyncedPoseModelSpace().begin(), worldFromModelQst,
														ragdollPose.getSyncedPoseModelSpace().begin(), futureQst, timestep);

	m_ragdollInstance->addToWorld( world, false);

	for (int i=0 ; i < m_ragdollInstance->getRigidBodyArray().getSize(); i++)
	{
		HK_SET_OBJECT_COLOR( (hkUlong)m_ragdollInstance->getRigidBodyArray()[i]->getCollidable(), 0x0);
	}

	hkaRagdollPoweredConstraintController::startMotors( m_ragdollInstance );
}

void RagdollDemoCharacter::removeRagdollFromWorld()
{
	hkaRagdollPoweredConstraintController::stopMotors( m_ragdollInstance );
	m_ragdollInstance->removeFromWorld();
	getProxy()->setCollisionFilterInfo(m_proxyNormalCollisionFilterInfo);
}

void RagdollDemoCharacter::initPoseMatching()
{


	// Construct a pose matching database
	m_poseMatchingSystem->m_poseMatchUtils = new hkaPoseMatchingUtility( m_poseMatchingSystem->m_rootIdx, m_poseMatchingSystem->m_otherIdx, m_poseMatchingSystem->m_anotherIdx, getProxy()->getUpLocal() );

	m_poseMatchingSystem->m_currentMatch = -1;
	m_poseMatchingSystem->m_timeSinceBetterMatch = 0.0f;

	// Put a dummy control in the DEATH_CONTROL slot
	// Each frame, we'll query the dummy control for its weight, then adjust the weights on all the static poses based on this
	// (see ::updatePoseMatchWeights)
	hkaDefaultAnimationControl* dummy = new hkaDefaultAnimationControl(HK_NULL) ;
	dummy->easeOut(0.0f);
	m_animatedSkeleton->addAnimationControl(dummy);
	dummy->removeReference();

	// Iterate through all the animations
	for( int curAnimation = 0; curAnimation < 3; curAnimation++ )
	{
		hkaAnimationBinding* binding = m_poseMatchingSystem->m_poseMatchingAnims[curAnimation];

		// Scan the animation for get up points
		for (int t=0; t< binding->m_animation->m_numAnnotationTracks; t++)
		{
			hkaAnnotationTrack& track = *binding->m_animation->m_annotationTracks[t];
			for (int a=0; a < track.m_numAnnotations; a++)
			{
				hkaAnnotationTrack::Annotation& note = track.m_annotations[a];

				// If we find an annotation
				if ( hkString::strCmp( "hkGetupYes", note.m_text) )
				{
					// Create a control that can be used to blend in and out this pose
					hkaDefaultAnimationControl* control = new hkaDefaultAnimationControl( binding );
					control->easeOut(0.0f);
					// Set the local time for the control to be the time annotated
					control->setLocalTime( note.m_time );
					// Set its speed to 0 so it holds the pose
					control->setPlaybackSpeed(0.0f);

					m_animatedSkeleton->addAnimationControl(control);
					m_poseMatchingSystem->m_mixerControls.pushBack(control);

					// Sample the animation at this time and add ti to the pose database
					{
						// Grab the pose in model space at this frame
						hkaAnimatedSkeleton mixer(const_cast<hkaSkeleton*>(m_animatedSkeleton->getSkeleton()));
						mixer.addAnimationControl(control);
						const hkaSkeleton* aSkel = m_animatedSkeleton->getSkeleton();
						hkLocalBuffer< hkQsTransform > animPoseModelSpace( aSkel->m_numBones );
						hkLocalBuffer< hkReal > animFloatSlots( aSkel->m_numFloatSlots );
						mixer.sampleAndCombineAnimations( animPoseModelSpace.begin(), animFloatSlots.begin() );
						hkaSkeletonUtils::transformLocalPoseToModelPose( aSkel->m_numBones, aSkel->m_parentIndices, animPoseModelSpace.begin(), animPoseModelSpace.begin() );

						// Map to lower res ragdoll
						const hkaSkeleton* skel = m_ragdollInstance->getSkeleton();
						hkLocalBuffer< hkQsTransform > referencePoseModel( m_ragdollInstance->getSkeleton()->m_numBones );
						hkaSkeletonUtils::transformLocalPoseToModelPose( skel->m_numBones, skel->m_parentIndices, skel->m_referencePose, referencePoseModel.begin() );
						m_highResToRagdollMapper->mapPose( animPoseModelSpace.begin(), skel->m_referencePose, referencePoseModel.begin(), hkaSkeletonMapper::CURRENT_POSE );

						// Add this pose to the database along with the time and binding info
						m_poseMatchingSystem->m_poseMatchUtils->addCandidatePose(referencePoseModel.begin(), binding, note.m_time);
					}
				}
			}
		}
	}
}

void RagdollDemoCharacter::updatePoseMatchWeights()
{
	hkaAnimationControl* poseMatchControl = m_animatedSkeleton->getAnimationControl( DEATH_CONTROL );
	hkReal poseMatchWeight = poseMatchControl->getWeight();

	for (int i=0; i<m_poseMatchingSystem->m_mixerControls.getSize(); i++)
	{
		m_poseMatchingSystem->m_mixerControls[i]->setMasterWeight(poseMatchWeight);
		m_poseMatchingSystem->m_mixerControls[i]->update(0.0f); // propagate the master weight change to hkaAnimationControl::m_weight
	}
}

void RagdollDemoCharacter::matchPose( hkReal& error, hkReal timestep )
{
	PoseMatchingSystem* sys = m_poseMatchingSystem;

	// Get the current ragdoll pose in world space
	const hkaSkeleton* rSkel = m_ragdollInstance->getSkeleton();
	hkLocalBuffer<hkQsTransform> ragdollWorldSpace( rSkel->m_numBones );
	m_ragdollInstance->getPoseModelSpace( ragdollWorldSpace.begin() , hkQsTransform::getIdentity() );

	// Find the right pose to map to
	const int match = sys->m_poseMatchUtils->findBestCandidatePoseIndex( ragdollWorldSpace.begin(), error );

	// If no matches were found (maybe there were no candidates?) return
	if (match<0)
	{
		return;
	}

	// No previous match, we just started doing pose matching?
	if (sys->m_currentMatch == -1)
	{
		// Ensure all other animations (controls) are out immediately
		for (int i=0; i < sys->m_mixerControls.getSize(); i++)
		{
			hkaDefaultAnimationControl* control = sys->m_mixerControls[i];
			control->easeOut(0.0f);
		}

		// And switch this one (the matched pose) on immediately
		hkaDefaultAnimationControl* matchControl = sys->m_mixerControls[match];
		matchControl->easeIn(0.0f);
		sys->m_currentMatch = match;

		// Finally, prepare the getup animation for the chosen match
		hkaAnimationControl* getUpControl = m_animatedSkeleton->getAnimationControl( GET_UP_CONTROL );
		getUpControl->setLocalTime( sys->m_poseMatchUtils->m_candidatePoses[match].m_time );
		getUpControl->setAnimationBinding( matchControl->getAnimationBinding() );

		// We're done.
		return;
	}

	// Add some hysteresis : We want to wait for sometime before we switch, to avoid continuous toggling between very similar matches
	if (match == sys->m_bestMatch)
	{
		sys->m_timeSinceBetterMatch += timestep;
	}
	else
	{
		sys->m_timeSinceBetterMatch = 0.0f;
	}
	sys->m_bestMatch = match;

	// If we have changed from the last matched pose. We use a time threshold based on the error.
	hkReal timeThreshold = sys->m_poseMatchingHysterisis * (1.0f - error);
	const bool switchToNewMatch = (match != sys->m_currentMatch) && (sys->m_timeSinceBetterMatch > timeThreshold);

	// We also don't want to switch to a new match if the ragdoll is settled. We decide this by checking the linear velocity
	// of the pelvis.
	const hkReal pelvisVelocity = m_ragdollInstance->getRigidBodyOfBone(0)->getLinearVelocity().length3();
	const bool deactivateMatching = (pelvisVelocity < 0.2f);

	// If we decide to switch..
	if (!deactivateMatching && switchToNewMatch)
	{
		// Blend in the new matched pose
		hkaDefaultAnimationControl* newControl = sys->m_mixerControls[match];
		newControl->easeIn( sys->m_poseMatchingBlendSpeed );

		// Blend out the old one
		hkaDefaultAnimationControl* oldControl = sys->m_mixerControls[sys->m_currentMatch];
		oldControl->easeOut( sys->m_poseMatchingBlendSpeed );

		sys->m_currentMatch  = match;
		sys->m_timeSinceBetterMatch = 0.0f;
	}

	// We only update the the get up animation to point to our new matching pose
	// when we have blended in more than 50%
	if (sys->m_mixerControls[sys->m_currentMatch]->getWeight() > .5f * m_animatedSkeleton->getAnimationControl(DEATH_CONTROL)->getWeight())
	{
		// Prepare the getup animation
		hkaAnimationControl* getUpControl = m_animatedSkeleton->getAnimationControl( GET_UP_CONTROL );
		getUpControl->setLocalTime( sys->m_poseMatchUtils->m_candidatePoses[sys->m_currentMatch].m_time );
		getUpControl->setAnimationBinding(sys->m_mixerControls[sys->m_currentMatch]->getAnimationBinding() );
	}
}

void RagdollDemoCharacter::updateRagdollDriving(hkReal timestep)
{
	const hkaSkeleton* rSkel = m_ragdollInstance->getSkeleton();

	hkaPose ragdollPose(rSkel);
	ragdollPose.setToReferencePose();
	m_highResToRagdollMapper->mapPose(*m_currentAnimationPose, ragdollPose, hkaSkeletonMapper::REFERENCE_POSE);
	hkaRagdollPoweredConstraintController::driveToPose( m_ragdollInstance, ragdollPose.getSyncedPoseLocalSpace().begin() );

	// Uncomment this to display the results from the pose matching system
	/*{
		hkQsTransform wFm; getWorldFromModel(wFm);
		wFm.m_translation(0) += 2.0f;
		AnimationUtils::drawPose(*m_currentAnimationPose, wFm);
	}*/

	// drag the proxy around
	{
		hkTransform t; getProxy()->getTransform(t);
		t.setTranslation(m_ragdollInstance->getRigidBodyOfBone(0)->getTransform().getTranslation());
		getProxy()->setTransform(t);
	}
}

void RagdollDemoCharacter::stopGetUp()
{
	m_poseMatchingSystem->m_blendControl->removeReference();
	m_poseMatchingSystem->m_blendControl = HK_NULL;
	// Reset the current match!
	m_poseMatchingSystem->m_currentMatch = -1;
}

void RagdollDemoCharacter::startGetUp( hkpWorld* world )
{
	const hkaSkeleton* rSkel = m_ragdollInstance->getSkeleton();
	PoseMatchingSystem* sys = m_poseMatchingSystem;



	// Get the ragdoll T-Pose
	hkLocalBuffer<hkQsTransform> referenceModelSpace( rSkel->m_numBones );
	hkaSkeletonUtils::transformLocalPoseToModelPose( rSkel->m_numBones, rSkel->m_parentIndices, rSkel->m_referencePose, referenceModelSpace.begin() );

	// Map from animation to reduced number of bones (using ragdoll T-pose)
	m_highResToRagdollMapper->mapPose( m_currentAnimationPose->getSyncedPoseModelSpace().begin(), rSkel->m_referencePose, referenceModelSpace.begin(), hkaSkeletonMapper::CURRENT_POSE );

	// Grab the pose from the ragdoll
	hkLocalBuffer<hkQsTransform> ragdollWorldSpace( rSkel->m_numBones );
	m_ragdollInstance->getPoseModelSpace( ragdollWorldSpace.begin() , hkQsTransform::getIdentity());


	
	// Work out reference frames for both the ragdoll and the animation
	hkQsTransform charAnimRot; charAnimRot.setIdentity();
	charAnimRot.m_rotation.set( m_characterFromAnimation );
	hkaSkeletonUtils::transformModelPoseToWorldPose( rSkel->m_numBones , charAnimRot, referenceModelSpace.begin(), referenceModelSpace.begin() );

	m_poseMatchingSystem->m_poseMatchUtils->computeReferenceFrame(referenceModelSpace.begin(), ragdollWorldSpace.begin(), sys->m_animFrame, sys->m_ragdollFrame);


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
		raycastIn.m_to.setAddMul4( raycastIn.m_from, getProxy()->getUpLocal(), -5.f );

		//	raycastIn.m_filterInfo=hkpGroupFilter::calcFilterInfo(LAYER_RAYCAST,0); XXX

		// cast the ray into the landscape
		world->castRay( raycastIn, rayCollector );

		const hkBool didHit = rayCollector.hasHit();
		if (didHit)
		{
			const hkpWorldRayCastOutput& raycastOut = rayCollector.getHit();
			{
				// Adjust the reference frame
				hkVector4 hitPointWS; hitPointWS.setInterpolate4(raycastIn.m_from, raycastIn.m_to, raycastOut.m_hitFraction);
				sys->m_animFrame.m_translation = hitPointWS;
			}
		}
	}

	// take care of the proxy
	{
		hkTransform t;
		getProxy()->getTransform(t);
		t.getTranslation() = sys->m_animFrame.m_translation;
		getProxy()->setTransform(t);
		getProxy()->setCollisionFilterInfo(m_proxyGettingUpCollisionFilterInfo);
	}


	// <ce.todo.aa> Clean this up - we only use the control as an ease curve
	// We use the ease curve support in a control for our blend weight
	sys->m_blendControl = new hkaDefaultAnimationControl( HK_NULL );
	sys->m_blendControl->setMasterWeight(1.0f);
	sys->m_blendControl->easeOut(sys->m_getupBlendSpeed);
}

void RagdollDemoCharacter::doGetup ( const hkVector4& characterPosition, hkaPose &pose, hkReal timestep )
{
	const hkaSkeleton* rSkel = m_ragdollInstance->getSkeleton();
	PoseMatchingSystem* sys = m_poseMatchingSystem;

	// Update the control - adjusting the weight
	sys->m_blendControl->update( timestep ); //XXX

	// We change the shape of the ease curve to slow down the blend
	hkReal blendWeight =  hkMath::sqrt(hkMath::sqrt(sys->m_blendControl->getWeight()));

	// We have set up the proxy with the initially calculated animation frame
	// We now retrieve this from the proxy. The proxy may be moved during get up
	// because it is interacting, or falling or resolving a penetration
	sys->m_animFrame.m_translation = characterPosition;

	// Interpolate our reference frame from the ragdoll frame and the animation frame

	hkQsTransform currentTransform = sys->m_ragdollFrame;
	currentTransform.setInterpolate4(sys->m_animFrame, sys->m_ragdollFrame, blendWeight);
	hkTransform t;	currentTransform.copyToTransformNoScale( t );
	getProxy()->setTransform(t);

	// Blend the poses in local space
	{
		// We map the ragdoll pose in model space to a higher resolution pose in model space
		hkLocalBuffer<hkQsTransform> ragdollHighResModelSpace( pose.getSkeleton()->m_numBones );
		hkString::memCpy( ragdollHighResModelSpace.begin(), pose.getSyncedPoseModelSpace().begin(), pose.getSkeleton()->m_numBones * sizeof(hkQsTransform) );

		// Get the current pose form the ragdoll
		hkLocalBuffer<hkQsTransform> ragdollWorldSpace( rSkel->m_numBones );
		m_ragdollInstance->getPoseModelSpace( ragdollWorldSpace.begin() , hkQsTransform::getIdentity());

		hkQuaternion charFromAnim( m_characterFromAnimation );

		hkLocalBuffer<hkQsTransform> ragdollAnimModelSpace( rSkel->m_numBones );
		hkQsTransform s = sys->m_ragdollFrame; 
		s.m_rotation.mul( charFromAnim );

		m_ragdollInstance->getPoseModelSpace( ragdollAnimModelSpace.begin() , s );


		// Map ragdoll pose in model space from low res to high res
		m_ragdollToHighResMapper->mapPose( ragdollAnimModelSpace.begin(), pose.getSyncedPoseLocalSpace().begin(), ragdollHighResModelSpace.begin(), hkaSkeletonMapper::CURRENT_POSE );

		// Compute the local space pose for the ragdoll
		hkLocalBuffer<hkQsTransform> ragdollHighResLocalSpace( pose.getSkeleton()->m_numBones );
		hkaSkeletonUtils::transformModelPoseToLocalPose( pose.getSkeleton()->m_numBones, pose.getSkeleton()->m_parentIndices, ragdollHighResModelSpace.begin(), ragdollHighResLocalSpace.begin() );
		
		const hkQsTransform* originalLocal = pose.getSyncedPoseLocalSpace().begin();
		hkaSkeletonUtils::blendPoses(  getSkeleton()->m_numBones,
			originalLocal,
			ragdollHighResLocalSpace.begin(),
			blendWeight,
			pose.accessUnsyncedPoseLocalSpace().begin() );
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
