/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/Animation/Api/Playback/IndividualBones/IndividualBoneSamplingDemo.h>

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
#include <Common/Visualize/hkDebugDisplay.h>



IndividualBoneSamplingDemo::IndividualBoneSamplingDemo( hkDemoEnvironment* env )
:	hkDefaultAnimationDemo(env)
{

	//
	// Setup the camera
	//
	{
		hkVector4 from(  -1.0f, -3.0f,  3.0f);
		hkVector4 to  (  0.0f,  0.0f,   1.0f);
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

	// Get the animation and the complete binding
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

	{
		// Create an animation control
		hkaDefaultAnimationControl* ac = new hkaDefaultAnimationControl( m_binding );

		// Create a new animated skeleton
		m_character = new hkaAnimatedSkeleton( m_skeleton );

		// Bind the control to the skeleton
		m_character->addAnimationControl( ac );

		// The animated skeleton now owns the control
		ac->removeReference();
	}

	// Draw the skeleton by default
	m_drawSkeleton = true;

	setupGraphics( );
}

IndividualBoneSamplingDemo::~IndividualBoneSamplingDemo()
{
	delete m_loader;
	delete m_character;
}

hkDemo::Result IndividualBoneSamplingDemo::stepDemo()
{
	// Define the chain of bones from the root through the finger
	const int numIndividualBones = 13;
	static hkInt16 individualBones[ numIndividualBones ] = { 0, 1, 2, 3, 4, 5, 10, 11, 12, 13, 14, 15, 16 };
	hkQsTransform individualTransforms[ numIndividualBones ];

	// Advance the time of the animation
	m_character->stepDeltaTime( m_timestep );

	// Test if a button was pressed
	if ( m_env->m_gamePad->wasButtonPressed( HKG_PAD_BUTTON_1 ) )
	{
		m_drawSkeleton = !m_drawSkeleton;
	}

	if ( m_drawSkeleton )
	{
		// Sample the full pose to draw the skeleton
		hkaPose pose( m_character->getSkeleton() );
		m_character->sampleAndCombineAnimations( pose.accessUnsyncedPoseLocalSpace().begin(), pose.getFloatSlotValues().begin()  );

		// Draw
		hkQsTransform worldFromModel (hkQsTransform::IDENTITY);
		worldFromModel.m_translation.set( 0, 0, 0);
		AnimationUtils::drawPose( pose, worldFromModel );
	}

	{
		// Sample only the arm bones
		// This feature is enabled for Spline compressed and Interleaved animation assets only
		// It is not available for Wavelet or Delta compressed animations
		// Mirrored Spline compressed or Mirrored Interleaved assets will also work,
		// but not Mirrored Wavelet compressed or Mirrored Delta compressed assets
		m_character->sampleAndCombineIndividualBones( individualTransforms, individualBones, numIndividualBones );

		hkVector4 point;
		point.setZero4();

		// Concatenate the transforms in reverse order
		for ( int i = numIndividualBones; i--; )
		{
			hkVector4 tmp = point;
			point.setTransformedPos( individualTransforms[ i ], tmp );
		}

		HK_DISPLAY_STAR( point, 0.2f, 0xFFFFFF00 );
	}


	return DEMO_OK;
}

#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] =
	"Demonstrates the use of inidivual bone sampling\n"
	"The chain of bones representing the arm are sampled individually\n"
	"A cross is drawn at the location of the fingertip\n"
	"Press \221 to toggle sampling/drawing of the full skeleton\n"
	"NOTE: This feature is available for spline compressed and interleaved animations. "
	"This feature is NOT available for wavelet compressed and delta compressed animations";

HK_DECLARE_DEMO( IndividualBoneSamplingDemo, HK_DEMO_TYPE_ANIMATION | HK_DEMO_TYPE_SERIALIZE | HK_DEMO_TYPE_CRITICAL, "Demonstrates the use of inidivual bone sampling", helpString );

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
