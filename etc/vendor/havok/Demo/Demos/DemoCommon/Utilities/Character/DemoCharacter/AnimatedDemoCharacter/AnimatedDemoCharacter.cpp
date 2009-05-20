/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


#include <Demos/demos.h>

#include <Demos/DemoCommon/Utilities/Character/DemoCharacter/AnimatedDemoCharacter/AnimatedDemoCharacter.h>
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
#include <Demos/DemoCommon/Utilities/Character/DemoCharacter/AnimatedDemoCharacter/StateMachine/States/SimpleBipedStates.h>

// See what renderer we are using etc
#include <Graphics/Bridge/System/hkgSystem.h> // to figure put if we should hardware skin
#include <Graphics/Common/Shader/hkgShader.h>
#include <Graphics/Common/Shader/hkgShaderCollection.h>
#include <Graphics/Common/DisplayObject/hkgDisplayObject.h>


#include <Demos/DemoCommon/Utilities/Character/CharacterProxy/RigidBodyCharacterProxy/RigidBodyCharacterProxy.h>
#include <Physics/Utilities/CharacterControl/CharacterRigidBody/hkpCharacterRigidBody.h>

AnimatedCharacterFactory::AnimatedCharacterFactory( CharacterType defaultType )
{
	m_type = defaultType;
	m_loader = new hkLoader();

	for (int i = 0; i < MAX_CHARACTER_TYPE; ++i)
	{
		m_animSet[i].m_skeleton = HK_NULL;
	}

	loadBasicAnimations( m_type );
}

AnimatedCharacterFactory::~AnimatedCharacterFactory()
{
	m_loader->removeReference();
}

DemoCharacter* AnimatedCharacterFactory::createCharacterUsingProxy( CharacterProxy* proxy, const hkVector4& gravity, hkDemoEnvironment* env )
{
	proxy->getWorldObject()->removeProperty(HK_PROPERTY_DEBUG_DISPLAY_COLOR);
	proxy->getWorldObject()->addProperty(HK_PROPERTY_DEBUG_DISPLAY_COLOR, 0x00FFFFFF);

	// Animated Character
	AnimatedDemoCharacterCinfo info;
	info.m_characterProxy = proxy;
	info.m_gravity = gravity;
	info.m_animationForwardLocal = m_animSet[m_type].m_animFwdLocal;
	info.m_animationUpLocal = m_animSet[m_type].m_animUpLocal;
	info.m_animationSet = &m_animSet[m_type];

	AnimatedDemoCharacter* animCharacter = new AnimatedDemoCharacter( info );
	animCharacter->loadSkin( m_loader, env, m_type );
	return animCharacter;	
}



void AnimatedCharacterFactory::loadBasicAnimations( CharacterType type )
{
	hkString rigFile;
	char* refBone = HK_NULL;
	char* rootBone = HK_NULL;

	AnimatedDemoCharacterAnimationSet* set = &m_animSet[type];

	set->m_walkRunSyncOffset = 0.0f;


	switch (type)
	{
		case HAVOK_GIRL:
			{
				rigFile = hkAssetManagementUtil::getFilePath("Resources/Animation/HavokGirl/hkRig.hkx");
				refBone = "reference";
				rootBone = "root";

				// Load the animations
				{
					set->m_idle	 = AnimationUtils::loadAnimation( *m_loader, "Resources/Animation/HavokGirl/hkIdle.hkx" );
					set->m_jump	 = AnimationUtils::loadAnimation( *m_loader, "Resources/Animation/HavokGirl/hkJumpLandLoop.hkx" );
					set->m_inAir = AnimationUtils::loadAnimation( *m_loader, "Resources/Animation/HavokGirl/hkProtect.hkx" );
					set->m_land	 = AnimationUtils::loadAnimation( *m_loader, "Resources/Animation/HavokGirl/hkProtect.hkx" );
					set->m_walk	 = AnimationUtils::loadAnimation( *m_loader, "Resources/Animation/HavokGirl/hkWalkLoop.hkx");
					set->m_run	 = AnimationUtils::loadAnimation( *m_loader, "Resources/Animation/HavokGirl/hkRunLoop.hkx");
					set->m_dive	 = AnimationUtils::loadAnimation( *m_loader, "Resources/Animation/HavokGirl/hkScale.hkx" );
				}

				set->m_animFwdLocal.set(1,0,0);
				set->m_animUpLocal.set(0,0,1);
			}
			break;
		case FIREFIGHTER:
			{
				rigFile = hkAssetManagementUtil::getFilePath("Resources/Animation/ShowCase/Gdc2005/Model/Firefighter_Rig.hkx");
				refBone = "reference";
				rootBone = "root";
				set->m_walkRunSyncOffset = 17.0f;

				// Load the animations
				{
					set->m_idle	 = AnimationUtils::loadAnimation( *m_loader, "Resources/Animation/ShowCase/Gdc2005/Animations/hkIdle1.hkx" );
					set->m_jump	 = AnimationUtils::loadAnimation( *m_loader, "Resources/Animation/ShowCase/Gdc2005/Animations/hkRunJump.hkx" );
					set->m_inAir = AnimationUtils::loadAnimation( *m_loader, "Resources/Animation/ShowCase/Gdc2005/Animations/hkInAir.hkx" );
					set->m_land	 = AnimationUtils::loadAnimation( *m_loader, "Resources/Animation/ShowCase/Gdc2005/Animations/hkHardLand.hkx" );
					set->m_walk	 = AnimationUtils::loadAnimation( *m_loader, "Resources/Animation/ShowCase/Gdc2005/Animations/hkWalk.hkx");
					set->m_run	 = AnimationUtils::loadAnimation( *m_loader, "Resources/Animation/ShowCase/Gdc2005/Animations/hkRun.hkx");
					set->m_dive	 = AnimationUtils::loadAnimation( *m_loader, "Resources/Animation/ShowCase/Gdc2005/Animations/hkDive.hkx" );
				}

				set->m_animFwdLocal.set(0,0,1);
				set->m_animUpLocal.set(0,1,0);
			}
			break;
		default:
			{
				HK_ASSERT2(0x0, 0, "Invalid character type");
			}
	}

	// Get the rig
	{
		hkError::getInstance().setEnabled(0x9fe65234, false); // "Unsupported simulation type..."
		set->m_rigContainer = m_loader->load( rigFile.cString() );
		HK_ASSERT2(0x27343437, set->m_rigContainer != HK_NULL , "Could not load asset");
		hkaAnimationContainer* ac = reinterpret_cast<hkaAnimationContainer*>( set->m_rigContainer->findObjectByType( hkaAnimationContainerClass.getName() ));
		HK_ASSERT2(0x27343435, ac && (ac->m_numSkeletons > 0), "No skeleton loaded");
		set->m_skeleton = ac->m_skeletons[0];
		hkError::getInstance().setEnabled(0x9fe65234, true);
	}


	// Add constraints to skeleton
	{
		const hkaSkeleton* skeleton = set->m_skeleton;

		// Lock translations (except root, named "position")
		hkaSkeletonUtils::lockTranslations(*skeleton);

		// and except also the children "reference" and "root"
		const hkInt16 referenceBoneIdx = hkaSkeletonUtils::findBoneWithName(*skeleton, refBone );
		const hkInt16 rootBoneIdx = hkaSkeletonUtils::findBoneWithName(*skeleton, rootBone);
		if (referenceBoneIdx != -1)
			skeleton->m_bones[referenceBoneIdx]->m_lockTranslation = false;
		if (rootBoneIdx != -1)
			skeleton->m_bones[rootBoneIdx]->m_lockTranslation = false;
	}


}


AnimatedDemoCharacter::AnimatedDemoCharacter( AnimatedDemoCharacterCinfo& info )
: DemoCharacter(info)
{
	m_gravity = info.m_gravity;

	//
	// Setup the proxyFromAnimation transform
	//
	m_animationUpLocal = info.m_animationUpLocal;
	m_animationForwardLocal = info.m_animationForwardLocal;

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
	m_hardwareSkinning = false;

	initAnimation( info.m_animationSet );

}

AnimatedDemoCharacter::~AnimatedDemoCharacter()
{	
	m_animationStateMachine->removeReference();
	m_animationMachine->removeReference();
}

const hkaSkeleton* AnimatedDemoCharacter::getSkeleton() const
{ 
	return m_animatedSkeleton->getSkeleton(); 
}

bool AnimatedDemoCharacter_supportsHardwareSkinning(hkDemoEnvironment* env)
{


	// Only reason not supported in DX10 is just because I haven't written the hkgBlendMatrixSet for them, which is easy if required.
	bool hardwareSkinning = (hkgSystem::g_RendererType != hkgSystem::HKG_RENDERER_DX10) && (hkgSystem::g_RendererType != hkgSystem::HKG_RENDERER_NULL);
	if (hardwareSkinning )
	{
		// check if it has enough shader support to run. We will assume we require 2.0 (so get 256 vshader constants to do fast skinning), not unreasonable, may run on 1.1
		hardwareSkinning = env->m_window->vertexShaderMajorVersion() >= 2; 
	}
	return hardwareSkinning;

}

hkgShaderCollection* AnimatedDemoCharacter_loadSkinningShader(hkDemoEnvironment* env)
{
	bool shouldCompileShaders = (hkgSystem::g_RendererType == hkgSystem::HKG_RENDERER_OGL) || (hkgSystem::g_RendererType == hkgSystem::HKG_RENDERER_DX9) || (hkgSystem::g_RendererType == hkgSystem::HKG_RENDERER_DX9S);
	hkgShaderCollection* ret  = HK_NULL;
	if (shouldCompileShaders)
	{
		const char* shaderFile;
		if ( hkgSystem::g_RendererType == hkgSystem::HKG_RENDERER_OGL ) // PC, (.. PS3(with different render type), Linux, Mac)
			shaderFile = "./Resources/Animation/Shaders/SimpleSkinningShader.cg"; 
		else // 360 and PC 
			shaderFile = "./Resources/Animation/Shaders/SimpleSkinningShader.hlsl";
		
		hkgShader* vertexShader = hkgShader::createVertexShader( env->m_window->getContext() );
		hkgShader* pixelShader = hkgShader::createPixelShader( env->m_window->getContext() );

		vertexShader->realizeCompileFromFile( shaderFile, "mainVS", HK_NULL, HK_NULL, HK_NULL);
		pixelShader->realizeCompileFromFile( shaderFile, "mainPS", HK_NULL, HK_NULL, HK_NULL);

		ret = hkgShaderCollection::create();
		ret->addShaderGrouping(vertexShader, pixelShader);	
	}
	return ret;
}

void AnimatedDemoCharacter_setSkinningShader( hkgShaderCollection* shader, hkgDisplayObject* skin)
{
	for (int g=0; g < skin->getNumGeometry(); ++g)
	{
		hkgGeometry* geom = skin->getGeometry(g);
		for (int m=0; geom && m < geom->getNumMaterialFaceSets(); ++m)
		{
			hkgMaterial* mat = geom->getMaterialFaceSet(m)->getMaterial();
			if (mat)
			{
				mat->setShaderCollection( shader );
			}
		}
	}
}

void AnimatedDemoCharacter::loadSkin( hkLoader* m_loader, hkDemoEnvironment* env, AnimatedCharacterFactory::CharacterType type )
{
	m_hardwareSkinning = AnimatedDemoCharacter_supportsHardwareSkinning(env);

	hkString assetFile;

	switch ( type )
	{
	case AnimatedCharacterFactory::HAVOK_GIRL:
			assetFile = hkAssetManagementUtil::getFilePath("Resources/Animation/HavokGirl/hkLowResSkinWithEyes.hkx");
			m_hardwareSkinning = false;
		break;
	case AnimatedCharacterFactory::FIREFIGHTER:
		{
			assetFile = hkAssetManagementUtil::getFilePath("Resources/Animation/Showcase/Gdc2005/Model/Firefighter_Skin.hkx"); 
		}
		break;
	default:
		{
			HK_ASSERT2(0x0, 0, "Invalid character type");
		}
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



void AnimatedDemoCharacter::initAnimation( const AnimatedDemoCharacterAnimationSet* set )
{
	// Initialize the state machine.
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
		control->setLocalTime( set->m_walkRunSyncOffset );
		control->setMasterWeight( 0.0f );
		m_animatedSkeleton->addAnimationControl( control );
		control->removeReference();

		// DIVE_CONTROL
		control = new hkaDefaultAnimationControl( set->m_dive ); 
		control->easeOut(0.0f);
		m_animatedSkeleton->addAnimationControl( control );
		control->removeReference();

	}

	// Initialize the animation command processor
	{
		m_animationMachine = new AnimationEventQueue(m_animatedSkeleton);
		m_animatedSkeleton->removeReference();
	}

	// Initialize the state machine
	{
		m_animationStateMachine = new SimpleBipedStateContext( stateManager );
		stateManager->removeReference();
		m_animationStateMachine->setCurrentState(STAND_STATE, m_animationMachine );
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
}

void AnimatedDemoCharacter::updatePosition( hkReal timestep, const CharacterStepInput& input, bool& isSupportedOut )
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

	// Advance the active animations
	m_animatedSkeleton->stepDeltaTime( timestep );

	const hkUint32 currentAnimationState = m_animationStateMachine->getCurrentState();

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

	hkReal turnAngle = input.m_turnVelocity * timestep;
	{
		hkQuaternion newRotation; newRotation.setAxisAngle( m_characterProxy->getUpLocal(), turnAngle );

		hkTransform wFc;
		m_characterProxy->getTransform( wFc );
		hkRotation r; r.set( newRotation );
		wFc.getRotation().mul( r );
		m_characterProxy->setTransform( wFc );
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

	// Adjust velocity
	{
		hkVector4 characterLinearVelocity;
		m_characterProxy->getLinearVelocity( characterLinearVelocity );

		if (currentAnimationState != JUMP_STATE)
		{
			// in these states we ignore the motion coming from the animation
			if (currentAnimationState == IN_AIR_STATE)
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

	// Store turn velocity
	m_characterProxy->setTurnVelocity( input.m_turnVelocity );

}



void AnimatedDemoCharacter::updateDisplay( int numBones, const hkQsTransform* poseMS, hkDemoEnvironment* env )
{
	hkTransform worldFromModel;
	m_characterProxy->getTransform( worldFromModel );
	worldFromModel.getRotation().mul( m_characterFromAnimation ); 

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

void AnimatedDemoCharacter::display( hkReal timestep, hkDemoEnvironment* env )
{
	hkaPose pose( getSkeleton() );

	updateAnimation( timestep, pose.accessUnsyncedPoseLocalSpace().begin());
	updateDisplay( getSkeleton()->m_numBones, pose.getSyncedPoseModelSpace().begin(),  env);
}


void AnimatedDemoCharacter::updateAnimation( hkReal timestep, hkQsTransform* poseLS )
{
	m_animatedSkeleton->sampleAndCombineAnimations( poseLS, HK_NULL );
}


void AnimatedDemoCharacter::update( hkReal timestep, hkpWorld* world, const CharacterStepInput& input, struct CharacterActionInfo* actionInfo )
{
	bool isSupported;
	updatePosition( timestep, input, isSupported );

	SimpleBipedStateInput stateInput;
	stateInput.m_shouldWalk = input.m_forwardVelocity > 0.01f;
	stateInput.m_isSupported = isSupported;
	stateInput.m_shouldJump = input.m_jumpVelocity > 0;
	stateInput.m_shouldDive = actionInfo != HK_NULL ? actionInfo->m_wasDivePressed : (hkBool) false;
	stateInput.m_animMachine = m_animationMachine;
	stateInput.m_context = m_animationStateMachine;

	// Update animation state machine
	m_animationStateMachine->update( timestep, &stateInput );
	m_animationMachine->update( timestep );
}


// doSkinning() just renders (skins) the given pose
void AnimatedDemoCharacter::doSkinning (const int boneCount, const hkQsTransform* poseMS, const hkTransform& worldFromModel, hkDemoEnvironment* env )
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
		if (!m_hardwareSkinning)
		{
			// FPU or SIMD skining
			AnimationUtils::skinMesh( *inputMesh, worldFromModel, compositeWorldInverse.begin(), *env->m_sceneConverter );
		}
		else // shader/VU/blend based skinning
		{
			env->m_sceneConverter->updateSkin( inputMesh, compositeWorldInverse, worldFromModel );
		}
	}
}

hkReal AnimatedDemoCharacter::getMaxVelocity() const
{
	return m_runVelocity;
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
