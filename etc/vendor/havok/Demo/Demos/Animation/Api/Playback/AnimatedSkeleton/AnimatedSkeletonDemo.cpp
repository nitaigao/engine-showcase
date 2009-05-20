/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/Animation/Api/Playback/AnimatedSkeleton/AnimatedSkeletonDemo.h>

#include <Common/Base/Reflection/hkClass.h>
#include <Common/Base/Container/LocalArray/hkLocalArray.h>

// Serialization
#include <Common/Serialize/Util/hkLoader.h>
#include <Common/Serialize/Util/hkRootLevelContainer.h>
#include <Common/Base/Reflection/Registry/hkTypeInfoRegistry.h>

// Asset mgt
#include <Demos/DemoCommon/Utilities/Asset/hkAssetManagementUtil.h>

// Scene data
#include <Common/SceneData/Scene/hkxScene.h>
#include <Animation/Animation/Rig/hkaPose.h>

// Skeletal Animation
#include <Animation/Animation/hkaAnimationContainer.h>
#include <Animation/Animation/Animation/Interleaved/hkaInterleavedUncompressedAnimation.h>
#include <Animation/Animation/Rig/hkaSkeleton.h>
#include <Animation/Animation/Playback/hkaAnimatedSkeleton.h>
#include <Animation/Animation/Animation/hkaAnimationBinding.h>
#include <Animation/Animation/Playback/Control/Default/hkaDefaultAnimationControl.h>

	// For debug drawing
#include <Demos/DemoCommon/Utilities/Animation/AnimationUtils.h>

#define NUM_SKELETONS 5

AnimatedSkeletonDemo::AnimatedSkeletonDemo( hkDemoEnvironment* env )
:	hkDefaultAnimationDemo(env)
{

	//
	// Setup the camera
	//
	{
		hkVector4 from(  -1.0f, -7.0f,  1.0f);
		hkVector4 to  (  0.0f,  0.0f,   0.0f);
		hkVector4 up  (  0.0f,  0.0f,   1.0f);
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

	// Get the animation and the binding
	{
		hkString assetFile = hkAssetManagementUtil::getFilePath("Resources/Animation/HavokGirl/hkJumpLandLoop.hkx");
		hkRootLevelContainer* container = m_loader->load( assetFile.cString() );
		HK_ASSERT2(0x27343437, container != HK_NULL , "Could not load asset");
		hkaAnimationContainer* ac = reinterpret_cast<hkaAnimationContainer*>( container->findObjectByType( hkaAnimationContainerClass.getName() ));

		HK_ASSERT2(0x27343435, ac && (ac->m_numAnimations > 0), "No animation loaded");
		m_animation = ac->m_animations[0];

		HK_ASSERT2(0x27343435, ac && (ac->m_numBindings > 0), "No binding loaded");
		m_binding = ac->m_bindings[0];
	}

	for ( int i = 0; i < NUM_SKELETONS; ++i )
	{
		// Create an animation control
		hkaDefaultAnimationControl* ac = new hkaDefaultAnimationControl(m_binding);
		ac->setLocalTime( i * .2f );

		// Create a new animated skeleton
		hkaAnimatedSkeleton* skeleton = new hkaAnimatedSkeleton( m_skeleton );
		m_activeSkeletons.pushBack( skeleton );

		// Bind the control to the skeleton
		skeleton->addAnimationControl( ac );

		// The animated skeleton now owns the control
		ac->removeReference();
	}

	setupGraphics( );
}

AnimatedSkeletonDemo::~AnimatedSkeletonDemo()
{
	delete m_loader;

	// Delete the active skeletons
	for (int s=0; s< m_activeSkeletons.getSize(); s++)
	{
		m_activeSkeletons[s]->removeReference();
	}
}

hkDemo::Result AnimatedSkeletonDemo::stepDemo()
{
	for (int i = 0; i < m_activeSkeletons.getSize(); ++i )
	{
		hkaAnimatedSkeleton* inst = m_activeSkeletons[i];

		// Advance the animation
		inst->stepDeltaTime( m_timestep );

		hkaPose pose (inst->getSkeleton());
		inst->sampleAndCombineAnimations( pose.accessUnsyncedPoseLocalSpace().begin(), pose.getFloatSlotValues().begin()  );

		// Draw
		hkQsTransform worldFromModel (hkQsTransform::IDENTITY);
		worldFromModel.m_translation.set( hkReal(i - 1 - (NUM_SKELETONS>>2)), 0, 0);
		AnimationUtils::drawPose( pose, worldFromModel );
	}

	return DEMO_OK;
}

#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
"A single animation and rig is loaded. This is instanced 5 times. Each instance has a different initial local time." ;

HK_DECLARE_DEMO(AnimatedSkeletonDemo, HK_DEMO_TYPE_ANIMATION | HK_DEMO_TYPE_SERIALIZE | HK_DEMO_TYPE_CRITICAL, "Shows 5 instances of the same skeletal animation", helpString);

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
