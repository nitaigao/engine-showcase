/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/Animation/Api/Playback/Masking/MaskingDemo.h>

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

// Scene Data
#include <Common/SceneData/Scene/hkxScene.h>
#include <Animation/Animation/Rig/hkaPose.h>

// Common animation Utilities
#include <Demos/DemoCommon/Utilities/Animation/AnimationUtils.h>

// Graphics Stuff
#include <Demos/DemoCommon/DemoFramework/hkTextDisplay.h>
#include <Graphics/Bridge/SceneData/hkgSceneDataConverter.h>

// Graphics Stuff
#include <Graphics/Common/Window/hkgWindow.h>
#include <Common/Visualize/hkDebugDisplay.h>




MaskingDemo::MaskingDemo( hkDemoEnvironment* env )
:	hkDefaultAnimationDemo(env)
{

	//
	// Setup the camera
	//
	{
		hkVector4 from( 6,0,2 );
		hkVector4 to  ( 0,0,0 );
		hkVector4 up  ( 0.0f, 0.0f, 1.0f );
		setupDefaultCameras( env, from, to, up, 0.1f, 100 );
	}

	m_loader = new hkLoader();

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
		hkString assetFile = hkAssetManagementUtil::getFilePath("Resources/Animation/HavokGirl/hkIdle.hkx");
		hkRootLevelContainer* container = m_loader->load( assetFile.cString() );
		HK_ASSERT2(0x27343437, container != HK_NULL , "Could not load asset");
		hkaAnimationContainer* ac = reinterpret_cast<hkaAnimationContainer*>( container->findObjectByType( hkaAnimationContainerClass.getName() ));

		HK_ASSERT2(0x27343435, ac && (ac->m_numAnimations > 0 ), "No animation loaded");
		m_animation[0] =  ac->m_animations[0];

		HK_ASSERT2(0x27343435, ac && (ac->m_numBindings > 0), "No binding loaded");
		m_binding[0] = ac->m_bindings[0];

		assetFile = hkAssetManagementUtil::getFilePath("Resources/Animation/HavokGirl/hkWalkLoop.hkx");
		container = m_loader->load( assetFile.cString() );
		ac = reinterpret_cast<hkaAnimationContainer*>( container->findObjectByType( hkaAnimationContainerClass.getName() ));

		HK_ASSERT2(0x27343435, ac && (ac->m_numAnimations > 0 ), "No animation loaded");
		m_animation[1] =  ac->m_animations[0];

		HK_ASSERT2(0x27343435, ac && (ac->m_numBindings > 0), "No binding loaded");
		m_binding[1] = ac->m_bindings[0];
	}

	// Create the skeleton
	m_skeletonInstance = new hkaAnimatedSkeleton( m_skeleton );

	// If the weight on any bone drops below this then it is filled with the T-Pose
	m_skeletonInstance->setReferencePoseWeightThreshold(0.1f);

	// Grab the animations
	for (int i=0; i < NUM_ANIMS; i++)
	{
		m_control[i] = new hkaDefaultAnimationControl( m_binding[i] );
		m_control[i]->setMasterWeight( 0.5f );
		m_control[i]->setPlaybackSpeed( 1.0f );

		m_skeletonInstance->addAnimationControl( m_control[i] );
		m_control[i]->removeReference();
	}

	setupGraphics( );

	m_selected = 0;
}

MaskingDemo::~MaskingDemo()
{
	delete m_loader;
	m_skeletonInstance->removeReference();
}

hkDemo::Result MaskingDemo::stepDemo()
{
	bool anim1Selected, anim2Selected, skelSelected;
	anim1Selected = anim2Selected = skelSelected = false;
	skelSelected = true;

	hkReal weight = 0;
	{
		const hkgPad* pad = m_env->m_gamePad;
		hkReal delta = 0;

		if ( pad->wasButtonPressed(HKG_PAD_DPAD_LEFT))
		{
			m_selected--;
		}
		if ( pad->wasButtonPressed(HKG_PAD_DPAD_RIGHT))
		{
			m_selected++;
		}

		if ((pad->getButtonState() & HKG_PAD_DPAD_UP) != 0)
		{
			delta = 0.01f;
		}
		if ((pad->getButtonState() & HKG_PAD_DPAD_DOWN) != 0)
		{
			delta = -0.01f;
		}

		if ((pad->getButtonState() & HKG_PAD_BUTTON_1) != 0)
		{
			anim1Selected = true;
			int count = m_animation[0]->m_numberOfTransformTracks;
			m_selected =  ( m_selected + count ) % count ;
			weight = m_control[0]->getTransformTrackWeight( m_selected ) / 255.0f;
		}
		if (!anim1Selected && ((pad->getButtonState() & HKG_PAD_BUTTON_2) != 0))
		{
			anim2Selected = true;
			int count = m_animation[1]->m_numberOfTransformTracks;
			m_selected =  ( m_selected + count ) % count ;
			weight = m_control[1]->getTransformTrackWeight( m_selected ) / 255.0f;
		}

		// clip track value
		{
			int count;
			if ( anim1Selected )
			{
				count = m_animation[0]->m_numberOfTransformTracks;
			}
			else if ( anim2Selected )
			{
				count = m_animation[1]->m_numberOfTransformTracks;
			}
			else
			{
				count = m_skeleton->m_numBones;
			}

			if ( m_selected < 0 )
			{
				m_selected = count - 1;
			}
			else
			{
				m_selected = ( m_selected % count );
			}
		}

		weight = weight + delta;
		weight = (weight < 0.0f)? 0.0f : weight;
		weight = (weight > 1.0f)? 1.0f : weight;

		if (anim1Selected)
		{
			m_control[0]->setTransformTrackWeight( m_selected, (hkUint8)(weight*255.0f) );
		}
		else if (anim2Selected)
		{
			m_control[1]->setTransformTrackWeight( m_selected, (hkUint8)(weight*255.0f) );
		}
	}

	int boneCount = m_skeleton->m_numBones;
	int selectedBone = 0;

	hkaPose selectedPose (m_skeleton);

	// Advance the active animations
	m_skeletonInstance->stepDeltaTime( m_timestep );

	hkaPose pose (m_skeleton);

	// Sample the active animations and combine into a single pose
	m_skeletonInstance->sampleAndCombineAnimations( pose.accessUnsyncedPoseLocalSpace().begin(), pose.getFloatSlotValues().begin() );
	AnimationUtils::drawPose( pose, hkQsTransform::getIdentity(), 0xffff0000 );

	if (skelSelected)
	{
		selectedPose = pose;
		selectedBone = m_selected;
	}


	// Create a buffer to store the animation in
	hkLocalArray<hkQsTransform> trackSample(boneCount);
	trackSample.setSizeUnchecked(boneCount);

	for (int i=0; i < NUM_ANIMS; i++)
	{
		trackSample.setSizeUnchecked(m_control[i]->getAnimationBinding()->m_animation->m_numberOfTransformTracks);
		m_binding[i]->m_animation->sampleTracks( m_control[i]->getLocalTime(), trackSample.begin(), HK_NULL, HK_NULL );

		pose.setToReferencePose();

		hkaAnimation* animation = m_binding[i]->m_animation;
		for (hkInt16 t=0; t < animation->m_numberOfTransformTracks; t++)
		{
			hkInt16 boneIdx = m_binding[i]->m_transformTrackToBoneIndices[t];
			if (boneIdx !=-1)
			{
				pose.setBoneLocalSpace(boneIdx, trackSample[t]);
			}
		}

		hkQsTransform worldFromModel(hkQsTransform::IDENTITY);
		worldFromModel.m_translation.set(-2, i*4.0f-2 ,0.0f ,0.0f);

		pose.accessBoneLocalSpace( 0 ).setMul( worldFromModel, pose.getBoneLocalSpace(0) );
		AnimationUtils::drawPose( pose, hkQsTransform::getIdentity(), hkColor::BLUE);

		if ((i==0) && (anim1Selected))
		{
			selectedPose = pose;
			selectedBone = m_binding[0]->m_transformTrackToBoneIndices[m_selected];
		}

		if ((i==1) && (anim2Selected))
		{
			selectedPose = pose;
			selectedBone = m_binding[1]->m_transformTrackToBoneIndices[m_selected];
		}
	}

	// Show currently selected bone
	{
		hkRotation rot;
		rot.set(selectedPose.getBoneModelSpace(selectedBone).getRotation());
		hkVector4 pos = selectedPose.getBoneModelSpace(selectedBone).getTranslation();

		HK_DISPLAY_ARROW(pos, rot.getColumn(0), 0xffff0000);
		HK_DISPLAY_ARROW(pos, rot.getColumn(1), 0xff00ff00);
		HK_DISPLAY_ARROW(pos, rot.getColumn(2), 0xff0000ff);

		char buf[10];
		hkString::sprintf(buf, "%0.3f ", weight  );
		hkString name(buf);
		m_env->m_textDisplay->outputText3D( name, pos(0), pos(1), pos(2), 0xffffffff, 1);
	}

	return hkDemo::DEMO_OK;
}

#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
"Use DPad Up/Down to alter weight.\n" \
"Left/Right selected a bone.\n" \
"Hold \221 to switch to select animation 1\n" \
"Hold \222 to switch to select animation 2";

HK_DECLARE_DEMO(MaskingDemo, HK_DEMO_TYPE_ANIMATION | HK_DEMO_TYPE_SERIALIZE, "Per Bone/Track Masking", helpString);

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
