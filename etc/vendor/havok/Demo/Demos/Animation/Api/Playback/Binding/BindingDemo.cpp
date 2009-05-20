/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/Animation/Api/Playback/Binding/BindingDemo.h>

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
#include <Common/Base/Types/Color/hkColor.h>

BindingDemo::BindingDemo( hkDemoEnvironment* env )
:	hkDefaultAnimationDemo(env)
{
	//
	// Setup the camera
	//
	{
		hkVector4 from( 0, -2.5f, 0.0f );
		hkVector4 to  ( 0,0,0 );
		hkVector4 up  ( 0,0,1 );
		setupDefaultCameras( env, from, to, up );
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

		assetFile = hkAssetManagementUtil::getFilePath("Resources/Animation/HavokGirl/hkWaveLoop.hkx");
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
		m_control[i]->setMasterWeight( 0.0f );
		m_control[i]->setPlaybackSpeed( 1.0f );

		m_skeletonInstance->addAnimationControl( m_control[i] );
		m_control[i]->removeReference();
	}

	// make a world so that we can auto create a display world to hold the skin
	setupGraphics( );
}

BindingDemo::~BindingDemo()
{
	delete m_loader;
	m_skeletonInstance->removeReference();
}

hkDemo::Result BindingDemo::stepDemo()
{
	hkReal w1 = m_control[0]->getMasterWeight();
	hkReal w2 = m_control[1]->getMasterWeight();

	const  hkReal inc = 0.01f;
	{
		const hkgPad* pad = m_env->m_gamePad;
		if ((pad->getButtonState() & HKG_PAD_DPAD_UP) != 0)
			w1+=inc;
		if ((pad->getButtonState() & HKG_PAD_DPAD_LEFT) != 0)
			w2+=inc;
		if ((pad->getButtonState() & HKG_PAD_DPAD_RIGHT) != 0)
			w2-=inc;
		if ((pad->getButtonState() & HKG_PAD_DPAD_DOWN) != 0)
			w1-=inc;

		//clamp
		w1 = (w1 > 1) ? 1 : w1;
		w1 = (w1 < 0) ? 0 : w1;
		w2 = (w2 > 1) ? 1 : w2;
		w2 = (w2 < 0) ? 0 : w2;
	}

	m_control[0]->setMasterWeight( w1 );
	m_control[1]->setMasterWeight( w2 );

	char buf[255];
	hkString::sprintf(buf, "wave weight :%0.3f idle weight:%0.3f fill thresh:%0.3f", w2, w1, m_skeletonInstance->getReferencePoseWeightThreshold());
	const int h = m_env->m_window->getHeight();
	m_env->m_textDisplay->outputText( buf, 20, h-40, 0xffffffff, 1);

	// Advance the active animations
	m_skeletonInstance->stepDeltaTime( .016f );

	const int boneCount = m_skeleton->m_numBones;

	// Sample the active animations and combine into a single pose
	hkaPose pose (m_skeleton);
	m_skeletonInstance->sampleAndCombineAnimations( pose.accessUnsyncedPoseLocalSpace().begin(), pose.getFloatSlotValues().begin()  );
	AnimationUtils::drawPose( pose, hkQsTransform::getIdentity(), 0xffff0000 );

	hkLocalArray<hkQsTransform> trackSample(boneCount);
	trackSample.setSizeUnchecked(boneCount);

	for (int i=0; i < NUM_ANIMS; i++)
	{
		trackSample.setSizeUnchecked( m_control[i]->getAnimationBinding()->m_animation->m_numberOfTransformTracks );
		m_binding[i]->m_animation->sampleTracks( m_control[i]->getLocalTime(), trackSample.begin(), HK_NULL , HK_NULL);

		pose.setToReferencePose();

		hkaAnimation* anim = m_binding[i]->m_animation;
		for (hkInt16 t=0; t < anim->m_numberOfTransformTracks; t++)
		{
			hkInt16 boneIdx = m_binding[i]->m_transformTrackToBoneIndices[t];
			if (boneIdx !=-1)
			{
				pose.setBoneLocalSpace(boneIdx, trackSample[t]);
			}
		}

		hkQsTransform worldFromModel (hkQsTransform::IDENTITY);
		worldFromModel.m_translation.set(i * 2.0f - 1.0f , 0, 0);

		hkReal gray = m_control[i]->getMasterWeight() * 0.4f + 0.6f;
		int color = hkColor::rgbFromFloats( gray, gray, gray );

		AnimationUtils::drawPose( pose, worldFromModel, color);
	}

	return hkDemo::DEMO_OK;
}

#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
"Playback partially bound bones. The wave animation contains only animation tracks for the " \
"upper half of the body. The blended animation is shown in the middle (in red) " \
"This demo shows the per bone renormalization of weights - even though the idle animation is played at 0.1 it " \
"gets renormalized to 1 for the unbound bones.";

HK_DECLARE_DEMO(BindingDemo, HK_DEMO_TYPE_ANIMATION | HK_DEMO_TYPE_SERIALIZE, "Partial Binding", helpString);

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
