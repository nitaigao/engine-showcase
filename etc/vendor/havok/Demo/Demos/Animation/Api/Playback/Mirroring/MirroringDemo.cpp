/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/Animation/Api/Playback/Mirroring/MirroringDemo.h>

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

	// Graphics Stuff
#include <Graphics/Common/hkGraphics.h>
#include <Graphics/Common/Texture/hkgTexture.h>
#include <Graphics/Common/Window/hkgWindow.h>
#include <Graphics/Common/DisplayObject/hkgDisplayObject.h>
#include <Graphics/Bridge/SceneData/hkgSceneDataConverter.h>

#include <Animation/Animation/Animation/Mirrored/hkaMirroredAnimation.h>
#include <Animation/Animation/Animation/Mirrored/hkaMirroredSkeleton.h>

MirroringDemo::MirroringDemo( hkDemoEnvironment* env )
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
		hkVector4 from( 2,-2,1 );
		hkVector4 to  ( 0,0,0 );
		hkVector4 up  ( 0.0f, 0.0f, 1.0f );
		setupDefaultCameras( env, from, to, up, 0.1f, 100 );
	}

	m_loader = new hkLoader();

	// Convert the scene
	{
		hkString assetFile = hkAssetManagementUtil::getFilePath("Resources/Animation/Scene/hkScene.hkx");
		hkRootLevelContainer* container = m_loader->load( assetFile.cString() );
		HK_ASSERT2(0x27343437, container != HK_NULL , "Could not load asset");
		hkxScene* scene = reinterpret_cast<hkxScene*>( container->findObjectByType( hkxSceneClass.getName() ));

		HK_ASSERT2(0x27343635, scene, "No scene loaded");
		removeLights(m_env);
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

		assetFile = hkAssetManagementUtil::getFilePath("Resources/Animation/HavokGirl/hkWaveLoop.hkx");
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

		// Overwrite animation 2 with a mirrored version of 1 (the wave)
		// Set the various parameters for the mirrored animation
		{
			hkObjectArray< hkString > ltag;
			hkObjectArray< hkString > rtag;

			ltag.pushBack( " L " );
			ltag.pushBack( "EyeL" );

			rtag.pushBack( " R " );
			rtag.pushBack( "EyeR" );

			hkaMirroredSkeleton *mirroredSkeleton = new hkaMirroredSkeleton( m_skeleton );

			mirroredSkeleton->computeBonePairingFromNames( ltag, rtag );

			// Mirror this character about the x axis
			hkQuaternion v_mir( 1.0f, 0.0f, 0.0f, 0.0f );
			mirroredSkeleton->setAllBoneInvariantsFromReferencePose( v_mir, 0.0f );

			hkaMirroredAnimation *mirroredAnimation = new hkaMirroredAnimation( m_animation[1], m_binding[1], mirroredSkeleton );
			m_binding[0] = mirroredAnimation->createMirroredBinding();
			m_animation[0] = mirroredAnimation;

			mirroredSkeleton->removeReference();
		}
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
	m_skeletonInstance->setReferencePoseWeightThreshold( 0.1f );

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

	m_control[1]->setEaseInCurve(0, 0.33f, 0.66f, 1);	// Linear
	m_control[1]->setEaseOutCurve(1, 0.66f, 0.33f, 0);	// Linear

	m_control[2]->setEaseInCurve(0, 0, 0, 1);	// Fast
	m_control[2]->setEaseOutCurve(1, 0, 0, 0);	// Fast


	// make a world so that we can auto create a display world to hold the skin
	setupGraphics( );
}

MirroringDemo::~MirroringDemo()
{
	// Re-enable warnings									
	hkError::getInstance().setEnabled(0xf0d1e423, true); 

	m_skeletonInstance->removeReference();
	m_animation[0]->removeReference();
	hkaMirroredAnimation::destroyMirroredBinding( m_binding[0] );
	delete m_loader;
}

const hkUint32 curveCols[3] =
{
	0xffffff00,
		0xffff00ff,
		0xff00ffff
};

hkDemo::Result MirroringDemo::stepDemo()
{

	// Display current settings
	{
		char buf[255];
		for (int i=0; i< NUM_ANIMS; i++)
		{
			hkString::sprintf(buf, "anim%d:%0.3f", i, m_control[i]->getWeight());
			m_env->m_textDisplay->outputText( buf, 10, 240+14*i, curveCols[i], 1);
		}
	}

	// Check user input
	{
		for (int i=0; i < 3; i++)
		{
			if (m_env->m_gamePad->wasButtonPressed( 1 << (i+1) ))
			{
				if (( m_control[i]->getEaseStatus() == hkaDefaultAnimationControl::EASING_IN ) ||
					( m_control[i]->getEaseStatus() == hkaDefaultAnimationControl::EASED_IN ))
				{
					m_control[i]->easeOut( 1.0f );
				}
				else
				{
					m_control[i]->easeIn( 1.0f );
				}
			}
		}
	}

	const int boneCount = m_skeleton->m_numBones;

	// Advance the active animations
	m_skeletonInstance->stepDeltaTime( 1.0f / 60.0f );

	// Sample the active animations and combine into a single pose
	hkaPose pose (m_skeleton);
	m_skeletonInstance->sampleAndCombineAnimations( pose.accessUnsyncedPoseLocalSpace().begin(), pose.getFloatSlotValues().begin() );
	AnimationUtils::drawPose( pose, hkQsTransform::getIdentity() );

	// Construct the composite world transform
	hkLocalArray<hkTransform> compositeWorldInverse( boneCount );
	compositeWorldInverse.setSize( boneCount );

	const hkArray<hkQsTransform>& poseModelSpace = pose.getSyncedPoseModelSpace();

	// Skin the meshes
	for (int i=0; i < m_numSkinBindings; i++)
	{
		// assumes either a straight map (null map) or a single one (1 palette)
		hkInt16* usedBones = m_skinBindings[i]->m_mappings? m_skinBindings[i]->m_mappings[0].m_mapping : HK_NULL;
		int numUsedBones = usedBones? m_skinBindings[i]->m_mappings[0].m_numMapping : boneCount;

		// Multiply through by the bind pose inverse world inverse matrices
		for (int p=0; p < numUsedBones; p++)
		{
			int boneIndex = usedBones? usedBones[p] : p;
			compositeWorldInverse[p].setMul( poseModelSpace[ boneIndex ], m_skinBindings[i]->m_boneFromSkinMeshTransforms[ boneIndex ] );
		}

		AnimationUtils::skinMesh( *m_skinBindings[i]->m_mesh, hkTransform::getIdentity(), compositeWorldInverse.begin(), *m_env->m_sceneConverter );
	}
	return hkDemo::DEMO_OK;
}


#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

HK_DECLARE_DEMO(MirroringDemo, HK_DEMO_TYPE_ANIMATION | HK_DEMO_TYPE_SERIALIZE, "Mirroring Demo", "Press \221,\222,\223 to ease in / out different animations.\n\221 is a mirrored version of \222.");

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
