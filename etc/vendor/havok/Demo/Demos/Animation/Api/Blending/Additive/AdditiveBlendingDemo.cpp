/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/Animation/Api/Blending/Additive/AdditiveBlendingDemo.h>

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
#include <Animation/Animation/Animation/Interleaved/hkaInterleavedUncompressedAnimation.h>
#include <Animation/Animation/Animation/Util/hkaAdditiveAnimationUtility.h>

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
#include <Demos/DemoCommon/DemoFramework/hkTextDisplay.h>
#include <Graphics/Bridge/SceneData/hkgSceneDataConverter.h>
#include <Graphics/Common/Window/hkgWindow.h>
#include <Graphics/Common/DisplayObject/hkgDisplayObject.h>

// Attachments
#include <Animation/Animation/Rig/hkaBoneAttachment.h>

static const int HK_WALK_ANIM=0;
static const int HK_ADDITIVE_ANIM=1;

AdditiveBlendingDemo::AdditiveBlendingDemo( hkDemoEnvironment* env )
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
		hkVector4 from( 0.3f, -1, 1 );
		hkVector4 to  ( 0, 0, 0.5f );
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

	// Get the animations and the binding
	{
		hkString assetFile = hkAssetManagementUtil::getFilePath("Resources/Animation/HavokGirl/hkWalkLoop.hkx");
		hkRootLevelContainer* container = m_loader->load( assetFile.cString() );
		HK_ASSERT2(0x27343437, container != HK_NULL , "Could not load asset");
		hkaAnimationContainer* ac = reinterpret_cast<hkaAnimationContainer*>( container->findObjectByType( hkaAnimationContainerClass.getName() ));

		HK_ASSERT2(0x27343435, ac && (ac->m_numAnimations > 0 ), "No animation loaded");
		m_animation[HK_WALK_ANIM] =  ac->m_animations[0];

		HK_ASSERT2(0x27343435, ac && (ac->m_numBindings > 0), "No binding loaded");
		m_binding[HK_WALK_ANIM] = ac->m_bindings[0];

		assetFile = hkAssetManagementUtil::getFilePath("Resources/Animation/HavokGirl/hkHeadMovement.hkx");
		container = m_loader->load( assetFile.cString() );
		ac = reinterpret_cast<hkaAnimationContainer*>( container->findObjectByType( hkaAnimationContainerClass.getName() ));

		HK_ASSERT2(0x27343435, ac && (ac->m_numAnimations > 0 ), "No animation loaded");
		m_animation[HK_ADDITIVE_ANIM] =  ac->m_animations[0];

		HK_ASSERT2(0x27343435, ac && (ac->m_numBindings > 0), "No binding loaded");
		m_binding[HK_ADDITIVE_ANIM] = ac->m_bindings[0];
	}

	// Create an additive animation
	// This can also be done offline in the toolchain using the CreateAdditiveAnimation filter
	// See the Additive configuration of hkHeadMovement.max
	{
		hkaInterleavedUncompressedAnimation* interleavedAnim = static_cast< hkaInterleavedUncompressedAnimation* >(m_animation[ HK_ADDITIVE_ANIM ]);

		hkaAdditiveAnimationUtility::Input input;
		input.m_originalData = interleavedAnim->m_transforms;
		input.m_numberOfPoses = interleavedAnim->m_numTransforms / interleavedAnim->m_numberOfTransformTracks;
		input.m_numberOfTransformTracks = interleavedAnim->m_numberOfTransformTracks;
		input.m_baseData = interleavedAnim->m_transforms;


		// We create an additive animation by subtracting off the iniital pose for the first frame of the animation
		// This is done by passing the same animation for both the originalData and the baseData
		// Note that only the first frame of the basedata is used so this initial frame is subtracted
		// from each of the frames in the animation
		hkaAdditiveAnimationUtility::createAdditiveFromPose( input, interleavedAnim->m_transforms );

		// Switch the binding to additive so this animation will be blended differently in sample and combine.
		m_binding[HK_ADDITIVE_ANIM]->m_blendHint = hkaAnimationBinding::ADDITIVE;
	}

	// Convert the skin
	{
		const char* skinFile = "Resources/Animation/HavokGirl/hkLowResSkinWithEyes.hkx";
		hkString assetFile = hkAssetManagementUtil::getFilePath( skinFile );
		hkRootLevelContainer* container = m_loader->load( assetFile.cString() );
		HK_ASSERT2(0x27343437, container != HK_NULL , "Could not load asset");

		hkxScene* scene = reinterpret_cast<hkxScene*>( container->findObjectByType( hkxSceneClass.getName() ));
		HK_ASSERT2(0x27343435, scene , "No scene loaded");

		hkaAnimationContainer* ac = reinterpret_cast<hkaAnimationContainer*>( container->findObjectByType( hkaAnimationContainerClass.getName() ));
		HK_ASSERT2(0x27343435, ac && (ac->m_numSkins > 0), "No animation loaded");

		m_numSkinBindings = ac->m_numSkins;
		m_skinBindings = ac->m_skins;

		m_numAttachments = ac->m_numAttachments;
		m_attachments = ac->m_attachments;

		// Make graphics output buffers for the skins
		env->m_sceneConverter->convert( scene );

		for (int a=0; a < m_numAttachments; ++a)
		{
			hkaBoneAttachment* ba = m_attachments[a];
			hkgDisplayObject* hkgObject = HK_NULL;

			//Check the attachment is a mesh
			if ( hkString::strCmp(ba->m_attachment.m_class->getName(), hkxMeshClass.getName()) == 0)
			{
				hkgObject = env->m_sceneConverter->findFirstDisplayObjectUsingMesh((hkxMesh*)ba->m_attachment.m_object);
				if (hkgObject)
				{
					hkgObject->setStatusFlags( hkgObject->getStatusFlags() | HKG_DISPLAY_OBJECT_DYNAMIC);
				}
			}

			m_attachmentObjects.pushBack(hkgObject);
		}

	}

	// Create the animated skeleton
	m_skeletonInstance = new hkaAnimatedSkeleton( m_skeleton );

	// Set the fill threshold
	m_skeletonInstance->setReferencePoseWeightThreshold( 0.05f );

	// Grab the animations and build controls
	for (int i=0; i < NUM_ANIMS; i++)
	{
		m_control[i] = new hkaDefaultAnimationControl( m_binding[i] );
		m_control[i]->setMasterWeight( 1.0f );
		m_control[i]->setPlaybackSpeed( 1.0f );

		m_skeletonInstance->addAnimationControl( m_control[i] );
		m_control[i]->removeReference();
	}

	// We initially turn the additive animation off and allow the user to ramp it in
	m_control[HK_ADDITIVE_ANIM]->setMasterWeight( 0.0f );
	m_control[HK_ADDITIVE_ANIM]->setPlaybackSpeed( 1.0f );
	setupGraphics( );

}

AdditiveBlendingDemo::~AdditiveBlendingDemo()
{
	// Re-enable warnings									
	hkError::getInstance().setEnabled(0xf0d1e423, true); 

	m_skeletonInstance->removeReference();

	delete m_loader;

	hkDefaultDemo::setupLights(m_env); // assumes the hkx will have some.
}

hkDemo::Result AdditiveBlendingDemo::stepDemo()
{
	hkReal w1 = m_control[HK_ADDITIVE_ANIM]->getMasterWeight();

	// Update input
	{
		const  hkReal inc = 0.01f;
		const hkgPad* pad = m_env->m_gamePad;
		if ((pad->getButtonState() & HKG_PAD_DPAD_UP) != 0)
			w1+=inc;
		if ((pad->getButtonState() & HKG_PAD_DPAD_DOWN) != 0)
			w1-=inc;

		w1 = hkMath::min2(w1, 1.0f);
		w1 = hkMath::max2(w1, 0.0f);
	}


	// Set the additive animation weight
	{
		m_control[HK_ADDITIVE_ANIM]->setMasterWeight( w1 );
	}

	// Show animation weight
	{
		char buf[255];
		hkString::sprintf( buf, "weight:%0.3f", w1 );

		const int h = m_env->m_window->getHeight();
		m_env->m_textDisplay->outputText( buf, 20, h-40, 0xffffffff, 1);
	}

	// Advance the active controls
	m_skeletonInstance->stepDeltaTime( m_timestep );

	const int boneCount = m_skeleton->m_numBones;

	// Sample the active animations and combine into a single pose
	hkaPose pose(m_skeleton);
	m_skeletonInstance->sampleAndCombineAnimations( pose.accessUnsyncedPoseLocalSpace().begin(), pose.getFloatSlotValues().begin() );
	AnimationUtils::drawPose( pose, hkQsTransform::getIdentity() );

	// Construct the composite world transform
	hkLocalArray<hkTransform> compositeWorldInverse( boneCount );
	compositeWorldInverse.setSize( boneCount );

	const hkArray<hkQsTransform>& poseInWorld = pose.getSyncedPoseModelSpace();

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
			compositeWorldInverse[p].setMul( poseInWorld[ boneIndex ], m_skinBindings[i]->m_boneFromSkinMeshTransforms[ boneIndex ] );
		}

		AnimationUtils::skinMesh( *m_skinBindings[i]->m_mesh, hkTransform::getIdentity(), compositeWorldInverse.begin(), *m_env->m_sceneConverter );
	}

	// Move the attachments
	{
		HK_ALIGN(float f[16], 16);
		for (int a=0; a < m_numAttachments; a++)
		{
			if (m_attachmentObjects[a])
			{
				hkaBoneAttachment* ba = m_attachments[a];
				pose.getBoneModelSpace( ba->m_boneIndex ).get4x4ColumnMajor(f);
				hkMatrix4 worldFromBone; worldFromBone.set4x4ColumnMajor(f);
				hkMatrix4 worldFromAttachment; worldFromAttachment.setMul(worldFromBone, ba->m_boneFromAttachment);
				m_env->m_sceneConverter->updateAttachment(m_attachmentObjects[a], worldFromAttachment);
			}
		}
	}

	return hkDemo::DEMO_OK;
}

#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
"This demo allows an additive animation to be composited on top of the base animation. "
"Use up and down on the DPad to control the weight of the additive animation.\n";

HK_DECLARE_DEMO(AdditiveBlendingDemo, HK_DEMO_TYPE_ANIMATION | HK_DEMO_TYPE_SERIALIZE, "Additive Animation Blending", helpString);

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
