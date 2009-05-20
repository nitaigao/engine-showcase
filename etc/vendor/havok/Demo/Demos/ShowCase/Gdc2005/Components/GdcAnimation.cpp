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
#include <Common/Serialize/Util/hkBuiltinTypeRegistry.h>

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
#include <Demos/ShowCase/Gdc2005/StateMachine/GdcState.h>
#include <Demos/ShowCase/Gdc2005/StateMachine/GdcStateTypes.h>
#include <Demos/ShowCase/Gdc2005/StateMachine/States/GdcStandState.h>
#include <Demos/ShowCase/Gdc2005/StateMachine/States/GdcWalkState.h>
#include <Demos/ShowCase/Gdc2005/StateMachine/States/GdcJumpState.h>
#include <Demos/ShowCase/Gdc2005/StateMachine/States/GdcDiveState.h>
#include <Demos/ShowCase/Gdc2005/StateMachine/States/GdcInAirState.h>
#include <Demos/ShowCase/Gdc2005/StateMachine/States/GdcLandState.h>
#include <Demos/ShowCase/Gdc2005/StateMachine/States/GdcDyingState.h>
#include <Demos/ShowCase/Gdc2005/StateMachine/States/GdcDeadState.h>
#include <Demos/ShowCase/Gdc2005/StateMachine/States/GdcGettingUpState.h>

// Graphics & Window Stuff
#include <Graphics/Bridge/SceneData/hkgSceneDataConverter.h>
#include <Graphics/Common/Window/hkgWindow.h>

void Gdc2005Demo::initAnimation()
{
	// Initialize the state machine.
	m_stateManager = new GdcStateManager();
	{
		GdcState* state = new GdcStandState();
		m_stateManager->registerState (GDC_STAND_STATE, state);
		state->removeReference();

		state = new GdcWalkState();
		m_stateManager->registerState (GDC_WALK_STATE, state);
		state->removeReference();

		state = new GdcJumpState();
		m_stateManager->registerState (GDC_JUMP_STATE, state);
		state->removeReference();

		state = new GdcInAirState();
		m_stateManager->registerState (GDC_IN_AIR_STATE, state);
		state->removeReference();

		state = new GdcLandState();
		m_stateManager->registerState (GDC_LAND_STATE, state);
		state->removeReference();

		state = new GdcDyingState();
		m_stateManager->registerState (GDC_DYING_STATE, state);
		state->removeReference();

		state = new GdcDeadState();
		m_stateManager->registerState (GDC_DEAD_STATE, state);
		state->removeReference();

		state = new GdcGettingUpState();
		m_stateManager->registerState (GDC_GETTING_UP_STATE, state);
		state->removeReference();

		state = new GdcDiveState();
		m_stateManager->registerState (GDC_DIVE_STATE, state);
		state->removeReference();
	}

	//Initialize the skeleton
	{

		// Get the rig
		{
			hkString assetFile = hkAssetManagementUtil::getFilePath("Resources/Animation/Showcase/Gdc2005/Model/Firefighter_Rig.hkx");
			m_rigContainer = m_loader->load( assetFile.cString() );
			HK_ASSERT2(0x27343437, m_rigContainer != HK_NULL , "Could not load asset");
			hkaAnimationContainer* ac = reinterpret_cast<hkaAnimationContainer*>( m_rigContainer->findObjectByType( hkaAnimationContainerClass.getName() ));

			HK_ASSERT2(0x27343435, ac && (ac->m_numSkeletons > 0), "No skeleton loaded");

			m_animatedSkeleton = new hkaAnimatedSkeleton( ac->m_skeletons[0] );

		}

		// Get the skin (different file)
		{
			hkString assetFile;
			if (m_bUseHardwareSkinning)
				assetFile = hkAssetManagementUtil::getFilePath("Resources/Animation/Showcase/Gdc2005/Model/Firefighter_PS2_18Bones_Skin.hkx");
			else
				assetFile = hkAssetManagementUtil::getFilePath("Resources/Animation/Showcase/Gdc2005/Model/Firefighter_Skin.hkx");

			hkRootLevelContainer* rootContainer = m_loader->load( assetFile.cString() );
			HK_ASSERT2(0x27343437, rootContainer != HK_NULL , "Could not load asset");
			hkaAnimationContainer* ac = reinterpret_cast<hkaAnimationContainer*>( rootContainer->findObjectByType( hkaAnimationContainerClass.getName() ));

			HK_ASSERT2(0x27343435, ac && (ac->m_numSkins > 0), "No skins loaded");

			m_numSkinBindings = ac->m_numSkins;
			m_skinBindings = ac->m_skins;

			hkxScene* scene = reinterpret_cast<hkxScene*>( rootContainer->findObjectByType( hkxSceneClass.getName() ));
			HK_ASSERT2(0x27343435, scene , "No scene loaded");

			// Make graphics output buffers for the skins
			m_env->m_sceneConverter->setAllowHardwareSkinning(m_bUseHardwareSkinning);
			m_env->m_sceneConverter->convert( scene );

			if (m_bUseHardwareSkinning)
			{
				for (int ms=0; ms < m_numSkinBindings; ++ms)
				{
					hkaMeshBinding* skinBinding = m_skinBindings[ms];

					if (! m_env->m_sceneConverter->setupHardwareSkin( m_env->m_window->getContext(), skinBinding->m_mesh,
						   reinterpret_cast<hkgAssetConverter::IndexMapping*>( skinBinding->m_mappings ),
						   skinBinding->m_numMappings, (hkInt16)skinBinding->m_skeleton->m_numBones ) )
					{
						HK_WARN(0x0, "Could not setup hardware skinning from given asset!");
					}
				}
			}
		}

		// Add animations in order
		{
			// GDC_MOVE_CONTROL - A Dummy control
			hkaDefaultAnimationControl* control;

			// GDC_WALK_CONTROL
			control = new hkaDefaultAnimationControl( HK_NULL );
			control->easeOut(0.0f);
			control->setMasterWeight(0.01f);
			m_animatedSkeleton->addAnimationControl( control );
			control->removeReference();

			// GDC_IDLE_CONTROL
			control = AnimationUtils::loadControl( *m_loader, "Resources/Animation/Showcase/Gdc2005/Animations/hkIdle1.hkx" );
			m_animatedSkeleton->addAnimationControl( control );
			control->removeReference();

			// GDC_JUMP_CONTROL
			control = AnimationUtils::loadControl( *m_loader, "Resources/Animation/Showcase/Gdc2005/Animations/hkRunJump.hkx" );
			m_animatedSkeleton->addAnimationControl( control );
			control->removeReference();

			// GDC_IN_AIR_CONTROL
			control = AnimationUtils::loadControl( *m_loader, "Resources/Animation/Showcase/Gdc2005/Animations/hkInAir.hkx" );
			m_animatedSkeleton->addAnimationControl( control );
			control->removeReference();

			// GDC_LAND_CONTROL
			control = AnimationUtils::loadControl( *m_loader,"Resources/Animation/Showcase/Gdc2005/Animations/hkHardLand.hkx" );
			m_animatedSkeleton->addAnimationControl( control );
			control->removeReference();

			// GDC_DYING_CONTROL
			control = AnimationUtils::loadControl( *m_loader, "Resources/Animation/Showcase/Gdc2005/Animations/hkDie.hkx" );
			m_animatedSkeleton->addAnimationControl( control );
			control->removeReference();

			// GDC_GET_UP_CONTROL
			control = AnimationUtils::loadControl( *m_loader, "Resources/Animation/Showcase/Gdc2005/Animations/hkKnockdown2.hkx" );
			m_animatedSkeleton->addAnimationControl( control );
			control->setPlaybackSpeed(0.0f);
			control->removeReference();

			// GDC_WALK_CONTROL
			control = AnimationUtils::loadControl( *m_loader, "Resources/Animation/Showcase/Gdc2005/Animations/hkWalk.hkx");
			control->setMasterWeight( 0.0f );
			control->easeIn( 0.0f );
			m_animatedSkeleton->addAnimationControl( control );
			control->removeReference();

			// GDC_RUN_CONTROL
			control = AnimationUtils::loadControl( *m_loader, "Resources/Animation/Showcase/Gdc2005/Animations/hkRun.hkx");
			// To sync with walk
			control->setLocalTime( 17.0f / 60.0f );
			control->setMasterWeight( 0.0f );
			control->easeIn( 0.0f );
			m_animatedSkeleton->addAnimationControl( control );
			control->removeReference();

			// GDC_DIVE_CONTROL
			control = AnimationUtils::loadControl( *m_loader,"Resources/Animation/Showcase/Gdc2005/Animations/hkDive.hkx" );
			m_animatedSkeleton->addAnimationControl( control );
			control->removeReference();

		}

		// Initialize the animation command processor
		{
			m_animationMachine = new GdcAnimationMachine(m_animatedSkeleton);
			m_animatedSkeleton->removeReference();
		}

		// Initialize the state machine
		{
			m_animationStateMachine = new GdcStateContext( m_stateManager );
			m_stateManager->removeReference();
			m_animationStateMachine->setCurrentState(GDC_STAND_STATE, m_animationMachine );
		}

		// Add constraints to skeleton
		{
			const hkaSkeleton* skeleton = m_animatedSkeleton->getSkeleton();

			// Lock translations (except root, named "position")
			hkaSkeletonUtils::lockTranslations(*skeleton);

			// and except also the children "reference" and "root"
			const hkInt16 referenceBoneIdx = hkaSkeletonUtils::findBoneWithName(*skeleton, "reference");
			const hkInt16 rootBoneIdx = hkaSkeletonUtils::findBoneWithName(*skeleton, "root");
			skeleton->m_bones[referenceBoneIdx]->m_lockTranslation = false;
			skeleton->m_bones[rootBoneIdx]->m_lockTranslation = false;

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
