/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/Animation/Api/Playback/RuntimeBinding/RuntimeBindingDemo.h>

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

// This function is used to generate the binding. It compares bone names and track names and indicates when they match (returns 0)
int HK_CALL bindingCompare( const char* boneName, const char* trackName )
{
	return ( hkString(trackName).beginsWith( boneName ) ? 0 : 1);
}

RuntimeBindingDemo::RuntimeBindingDemo( hkDemoEnvironment* env )
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

	// Get the animation
	{
		hkString assetFile = hkAssetManagementUtil::getFilePath("Resources/Animation/HavokGirl/hkIdle.hkx");
		hkRootLevelContainer* container = m_loader->load( assetFile.cString() );
		HK_ASSERT2(0x27343437, container != HK_NULL , "Could not load asset");
		hkaAnimationContainer* ac = reinterpret_cast<hkaAnimationContainer*>( container->findObjectByType( hkaAnimationContainerClass.getName() ));

		HK_ASSERT2(0x27343435, ac && (ac->m_numAnimations > 0 ), "No animation loaded");
		m_animation =  ac->m_animations[0];

	}

	// Create a binding by matching names
	{
		// Inititalize the binding
		m_runtimeBinding = new hkaAnimationBinding();
		m_runtimeBinding->m_animation = m_animation;
		m_runtimeBinding->m_blendHint = hkaAnimationBinding::NORMAL;
		m_runtimeBinding->m_numTransformTrackToBoneIndices = m_animation->m_numberOfTransformTracks;
		m_runtimeBinding->m_transformTrackToBoneIndices = hkAllocateChunk<hkInt16>( m_runtimeBinding->m_numTransformTrackToBoneIndices, HK_MEMORY_CLASS_DEMO );

		// USe the binding utility to construct the track to bone map
		HK_ON_DEBUG(hkResult res =)
			hkaSkeletonUtils::bindByName( *m_skeleton, *m_animation, bindingCompare, *m_runtimeBinding);
		HK_ASSERT2( 0x65e45e32, res == HK_SUCCESS, "Runtime binding failed");
	}

	// Create the skeleton
	m_skeletonInstance = new hkaAnimatedSkeleton( m_skeleton );

	// If the weight on any bone drops below this then it is filled with the T-Pose
	m_skeletonInstance->setReferencePoseWeightThreshold(0.1f);

	// Create a control
	{
		m_control = new hkaDefaultAnimationControl( m_runtimeBinding );
		m_control->setMasterWeight( 1.0f );
		m_control->setPlaybackSpeed( 1.0f );

		m_skeletonInstance->addAnimationControl( m_control );
		m_control->removeReference();
	}

	// make a world so that we can auto create a display world to hold the skin
	setupGraphics( );
}

RuntimeBindingDemo::~RuntimeBindingDemo()
{
	delete m_loader;
	m_skeletonInstance->removeReference();

	// Clean up the binding
	{
		hkDeallocateChunk<hkInt16>( m_runtimeBinding->m_transformTrackToBoneIndices, m_runtimeBinding->m_numTransformTrackToBoneIndices, HK_MEMORY_CLASS_DEMO );
		delete m_runtimeBinding;
	}
}

hkDemo::Result RuntimeBindingDemo::stepDemo()
{
	// Advance the active animations
	m_skeletonInstance->stepDeltaTime( .016f );

	// Sample the active animations and combine into a single pose
	hkaPose pose (m_skeleton);
	m_skeletonInstance->sampleAndCombineAnimations( pose.accessUnsyncedPoseLocalSpace().begin(), pose.getFloatSlotValues().begin() );
	AnimationUtils::drawPose( pose, hkQsTransform::getIdentity(), 0xffff0000 );

	return hkDemo::DEMO_OK;
}

#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
"This demos shows how an animation can be bound to a rig at runtime using bone and track names.";

HK_DECLARE_DEMO(RuntimeBindingDemo, HK_DEMO_TYPE_ANIMATION | HK_DEMO_TYPE_SERIALIZE, "Runtime Binding", helpString);

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
