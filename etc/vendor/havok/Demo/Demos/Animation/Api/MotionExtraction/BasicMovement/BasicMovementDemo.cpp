/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/Animation/Api/MotionExtraction/BasicMovement/BasicMovementDemo.h>

#include <Common/Base/Reflection/hkClass.h>
#include <Common/Base/Container/LocalArray/hkLocalArray.h>

// Serialization
#include <Common/Serialize/Util/hkLoader.h>
#include <Common/Serialize/Util/hkRootLevelContainer.h>
#include <Common/Base/Reflection/Registry/hkTypeInfoRegistry.h>

// Asset mgt
#include <Demos/DemoCommon/Utilities/Asset/hkAssetManagementUtil.h>

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
#include <Animation/Animation/Rig/hkaPose.h>

// Common animation Utilities
#include <Demos/DemoCommon/Utilities/Animation/AnimationUtils.h>

// Graphics Stuff
#include <Demos/DemoCommon/DemoFramework/hkTextDisplay.h>
#include <Graphics/Bridge/SceneData/hkgSceneDataConverter.h>

// Graphics Stuff
#include <Graphics/Common/Window/hkgWindow.h>

// for PlayStation(R)2 metrowerks
#include <Demos/DemoCommon/DemoFramework/hkDoubleConvCheck.h>

BasicMovementDemo::BasicMovementDemo( hkDemoEnvironment* env )
:	hkDefaultAnimationDemo(env)
{
	// Disable warnings: if no renderer									
	if( hkString::strCmp( m_env->m_options->m_renderer, "n" ) == 0 )
	{
		hkError::getInstance().setEnabled(0xf0d1e423, false); //'Could not realize an inplace texture of type PNG.'
	}

	//
	// Setup the camera
	//
	{
		hkVector4 from( -4,-4,2 );
		hkVector4 to  ( 0,0,0 );
		hkVector4 up  ( 0.0f, 0.0f, 1.0f );
		setupDefaultCameras( env, from, to, up, 0.1f, 100 );

		// so we can use the sticks on consoles
		if (m_env->m_options->m_trackballMode == 0)
		{
			m_forcedOnTrackball = true;
			m_env->m_window->getViewport(0)->setNavigationMode(HKG_CAMERA_NAV_TRACKBALL);
		}
	}

	m_loader = new hkLoader();

	m_env->m_sceneConverter->setAllowHardwareSkinning(false);

	// Convert the scene
	{
		hkString assetFile = hkAssetManagementUtil::getFilePath("Resources/Animation/Scene/hkScene.hkx");
		hkRootLevelContainer* container = m_loader->load( assetFile.cString() );
		HK_ASSERT2(0x27343437, container != HK_NULL , "Could not load asset");
		hkxScene* scene = reinterpret_cast<hkxScene*>( container->findObjectByType( hkxSceneClass.getName() ));

		removeLights(m_env); // assume scene has them

		HK_ASSERT2(0x27343635, scene, "No scene loaded");
		env->m_sceneConverter->convert( scene );
	}

	// Get the rig
	{
		hkString assetFile = hkAssetManagementUtil::getFilePath("Resources/Animation/HavokGirl/hkRig.hkx");
		hkRootLevelContainer* container = m_loader->load( assetFile.cString() );
		HK_ASSERT2(0x27343437, container != HK_NULL , "Could not load asset");
		hkaAnimationContainer* ac = reinterpret_cast<hkaAnimationContainer*>( container->findObjectByType( hkaAnimationContainerClass.getName() ));

		HK_ASSERT2(0x27343435, ac && (ac->m_numSkeletons > 0), "No skeleton loaded");
		m_skeleton = ac->m_skeletons[0];
	}

	// Get the animation and the binding
	{
		hkString assetFile = hkAssetManagementUtil::getFilePath("Resources/Animation/HavokGirl/hkRunLoop.hkx");
		hkRootLevelContainer* container = m_loader->load( assetFile.cString() );
		HK_ASSERT2(0x27343437, container != HK_NULL , "Could not load asset");
		hkaAnimationContainer* ac = reinterpret_cast<hkaAnimationContainer*>( container->findObjectByType( hkaAnimationContainerClass.getName() ));

		HK_ASSERT2(0x27343435, ac && (ac->m_numAnimations > 0 ), "No animation loaded");
		m_animation[0] =  ac->m_animations[0];

		HK_ASSERT2(0x27343435, ac && (ac->m_numBindings > 0), "No binding loaded");
		m_binding[0] = ac->m_bindings[0];

		assetFile = hkAssetManagementUtil::getFilePath("Resources/Animation/HavokGirl/hkJumpLandLoop.hkx");
		container = m_loader->load( assetFile.cString() );
		ac = reinterpret_cast<hkaAnimationContainer*>( container->findObjectByType( hkaAnimationContainerClass.getName() ));

		HK_ASSERT2(0x27343435, ac && (ac->m_numAnimations > 0 ), "No animation loaded");
		m_animation[1] =  ac->m_animations[0];

		HK_ASSERT2(0x27343435, ac && (ac->m_numBindings > 0), "No binding loaded");
		m_binding[1] = ac->m_bindings[0];

		assetFile = hkAssetManagementUtil::getFilePath("Resources/Animation/HavokGirl/hkIdle.hkx");
		container = m_loader->load( assetFile.cString() );
		ac = reinterpret_cast<hkaAnimationContainer*>( container->findObjectByType( hkaAnimationContainerClass.getName() ));

		HK_ASSERT2(0x27343435, ac && (ac->m_numAnimations > 0 ), "No animation loaded");
		m_animation[2] =  ac->m_animations[0];

		HK_ASSERT2(0x27343435, ac && (ac->m_numBindings > 0), "No binding loaded");
		m_binding[2] = ac->m_bindings[0];
	}

	// Convert the skin
	{
		const char* skinFile = "Resources/Animation/HavokGirl/hkLowResSkin.hkx";
		hkString assetFile = hkAssetManagementUtil::getFilePath( skinFile );
		hkRootLevelContainer* container = m_loader->load( assetFile.cString() );
		HK_ASSERT2(0x27343437, container != HK_NULL , "Could not load asset");

		hkxScene* scene = reinterpret_cast<hkxScene*>( container->findObjectByType( hkxSceneClass.getName() ));
		HK_ASSERT2(0x27343435, scene , "No scene loaded");

		hkaAnimationContainer* ac = reinterpret_cast<hkaAnimationContainer*>( container->findObjectByType( hkaAnimationContainerClass.getName() ));
		HK_ASSERT2(0x27343435, ac && (ac->m_numSkins > 0), "No skins loaded");

		m_numSkinBindings = ac->m_numSkins;
		m_skinBindings = ac->m_skins;

		// Make graphics output buffers for the skins
		env->m_sceneConverter->convert( scene );
	}

	// Create the skeleton
	m_skeletonInstance = new hkaAnimatedSkeleton( m_skeleton );
	m_skeletonInstance->setReferencePoseWeightThreshold( 0.0f );

	// Grab the animations
	for (int i=0; i < NUM_ANIMS; i++)
	{
		m_control[i] = new hkaDefaultAnimationControl( m_binding[i] );
		m_control[i]->setMasterWeight( 1.0f );
		m_control[i]->setPlaybackSpeed( 1.0f );

		// Ease all controls out initially
		m_control[i]->easeOut(0.0f);

		m_skeletonInstance->addAnimationControl( m_control[i] );
		m_control[i]->removeReference();
	}

	// Set ease curves explicitly
	m_control[0]->setEaseInCurve(0, 0, 1, 1);	// Smooth
	m_control[0]->setEaseOutCurve(1, 1, 0, 0);	// Smooth
	m_control[2]->setMasterWeight( 0.1f );
	m_control[2]->easeIn(0.0f);

	// make a world so that we can auto create a display world to hold the skin
	setupGraphics( );

	m_currentMotion.setIdentity();
}

BasicMovementDemo::~BasicMovementDemo()
{
	// Re-enable warnings									
	hkError::getInstance().setEnabled(0xf0d1e423, true); 

	m_skeletonInstance->removeReference();
	delete m_loader;

	hkDefaultDemo::setupLights(m_env);

	if (m_forcedOnTrackball)
		m_env->m_window->getViewport(0)->setNavigationMode(HKG_CAMERA_NAV_FLY);
}

const hkUint32 curveCols[3] =
{
	0xffffff00,
	0xffff00ff,
	0xff00ffff
};

hkDemo::Result BasicMovementDemo::stepDemo()
{
	// for PlayStation(R)2 metrowerks
	pushDoubleConversionCheck( false );

	// Display current settings
	{
		char buf[255];
		for (int i=0; i< NUM_ANIMS; i++)
		{
			hkString::sprintf(buf, "anim%d: %0.3f", i, m_control[i]->getWeight());
			const int h = m_env->m_window->getHeight();
			m_env->m_textDisplay->outputText( buf, 20, h-70+16*i, curveCols[i], 1);
		}
	}

	popDoubleConversionCheck();

	// Handle the keys
	{
		bool jumping = (m_control[1]->getWeight() > 0.001f);

		if (!jumping)
		{
			hkVector4 upVec(0,0,1);
			hkReal up, turn;
			up = m_env->m_gamePad->getStickPosY( 1 ); // 1 is the only one on PSP

			const hkBool upPressed = (up > 0.0f);
			const hkBool easeIn = (m_control[0]->getEaseStatus() == hkaDefaultAnimationControl::EASING_IN ) || ( m_control[0]->getEaseStatus() == hkaDefaultAnimationControl::EASED_IN );
			if ( easeIn  && ( !upPressed ))
			{
				// Ease in stand
				m_control[0]->easeOut( 0.9f );
			}
			else if (( !easeIn )  && upPressed )
			{
				// Ease in walk cycle
				m_control[0]->easeIn( 0.9f );
			}

			turn = m_env->m_gamePad->getStickPosX(1);

			turn *= -5.0f * HK_REAL_PI / 180.0f * m_control[0]->getWeight();

			hkQuaternion q;
			q.setAxisAngle(upVec, turn);
			m_currentMotion.m_rotation.mul(q);
		}
		else
		{
			hkReal remaining = m_control[1]->getAnimationBinding()->m_animation->m_duration - m_control[1]->getLocalTime();

			// If we've played through then fade out
			const hkBool easeIn = (m_control[1]->getEaseStatus() == hkaDefaultAnimationControl::EASING_IN ) || ( m_control[1]->getEaseStatus() == hkaDefaultAnimationControl::EASED_IN );
			if ( easeIn && (remaining < 0.2f))
			{
				m_control[1]->easeOut( remaining );
				m_control[0]->setLocalTime( m_control[0]->getAnimationBinding()->m_animation->m_duration - remaining );
				m_control[0]->easeIn( remaining );
			}


		}

		if (m_env->m_gamePad->wasButtonPressed( HKG_PAD_BUTTON_2) && !jumping)
		{
			// Reset jump animation and play
			m_control[1]->setLocalTime(0.0f);

			m_control[1]->easeIn( 0.1f );
			m_control[0]->easeOut( 0.1f );
		}

		if (m_env->m_gamePad->wasButtonPressed( HKG_PAD_BUTTON_1))
		{
			m_currentMotion.setIdentity();
		}

	}

	// Grab accumulated motion
	{
		hkQsTransform deltaMotion;
		deltaMotion.setIdentity();
		m_skeletonInstance->getDeltaReferenceFrame( m_timestep, deltaMotion);

		hkQsTransform temp;
		temp.setMul(m_currentMotion, deltaMotion);
		m_currentMotion = temp;
	}

	const int boneCount = m_skeleton->m_numBones;

	// Advance the active animations
	m_skeletonInstance->stepDeltaTime( 1.0f / 60.0f );

	// Sample the active animations and combine into a single pose
	hkaPose pose (m_skeleton);
	m_skeletonInstance->sampleAndCombineAnimations( pose.accessUnsyncedPoseLocalSpace().begin(), pose.getFloatSlotValues().begin()  );
	AnimationUtils::drawPose( pose, hkQsTransform::getIdentity() );

	// Construct the composite world transform
	hkLocalArray<hkTransform> compositeWorldInverse( boneCount );
	compositeWorldInverse.setSize( boneCount );

	// Convert accumlated info to graphics matrix
	hkTransform graphicsTransform;
	m_currentMotion.copyToTransform(graphicsTransform);

	// Skin the meshes
	{
		const hkArray<hkQsTransform>& poseInWorld = pose.getSyncedPoseModelSpace();

		for (int i=0; i < m_numSkinBindings; i++)
		{
			// assumes either a straight map (null map) or a single one (1 palette)
			hkInt16* usedBones = m_skinBindings[i]->m_mappings? m_skinBindings[i]->m_mappings[0].m_mapping : HK_NULL;
			int numUsedBones = usedBones? m_skinBindings[i]->m_mappings[0].m_numMapping : boneCount;

			// Multiply through by the bind pose inverse world inverse matrices
			for (int p=0; p < numUsedBones; p++)
			{
				int boneIndex = usedBones? usedBones[p] : p;
				compositeWorldInverse[p].setMul( poseInWorld[ boneIndex ], m_skinBindings[i]->m_boneFromSkinMeshTransforms[ boneIndex ] );
			}


			AnimationUtils::skinMesh( *m_skinBindings[i]->m_mesh, graphicsTransform, compositeWorldInverse.begin(), *m_env->m_sceneConverter );
		}
	}

	return hkDemo::DEMO_OK;
}

#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
"Demo showing basic character movement. " \
"The demo uses animation easing and motion extraction to produce the final result\n" \
"Use joystick / arrow keys to move. " \
"Press \221 to reset position. " \
"Press \222 to jump.";


HK_DECLARE_DEMO(BasicMovementDemo, HK_DEMO_TYPE_ANIMATION | HK_DEMO_TYPE_SERIALIZE | HK_DEMO_TYPE_CRITICAL, "Character movement", helpString);

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
