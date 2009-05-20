/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/Animation/Api/InverseKinematics/Reaching/ReachingDemo.h>

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
#include <Animation/Animation/Rig/hkaPose.h>
#include <Animation/Animation/Playback/hkaAnimatedSkeleton.h>
#include <Animation/Animation/Playback/Control/Default/hkaDefaultAnimationControl.h>
#include <Animation/Animation/Animation/hkaAnimationBinding.h>

// Vertex Deformation
#include <Animation/Animation/Deform/Skinning/hkaMeshBinding.h>

// IK
#include <Animation/Animation/Ik/Ccd/hkaCcdIkSolver.h>

// Scene Data
#include <Common/SceneData/Scene/hkxScene.h>
#include <Common/SceneData/Mesh/hkxMesh.h>

// Common animation Utilities
#include <Demos/DemoCommon/Utilities/Animation/AnimationUtils.h>

// Graphics Stuff
#include <Demos/DemoCommon/DemoFramework/hkTextDisplay.h>
#include <Graphics/Bridge/SceneData/hkgSceneDataConverter.h>
#include <Graphics/Common/Window/hkgWindow.h>

// Debug graphics
#include <Common/Visualize/hkDebugDisplay.h>


ReachingDemo::ReachingDemo( hkDemoEnvironment* env )
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
		hkVector4 from( -2,-2,2 );
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

	// Get the idle animation and the binding
	{
		hkString assetFile = hkAssetManagementUtil::getFilePath("Resources/Animation/HavokGirl/hkIdle.hkx");
		hkRootLevelContainer* container = m_loader->load( assetFile.cString() );
		HK_ASSERT2(0x27343437, container != HK_NULL , "Could not load asset");
		hkaAnimationContainer* ac = reinterpret_cast<hkaAnimationContainer*>( container->findObjectByType( hkaAnimationContainerClass.getName() ));

		HK_ASSERT2(0x27343435, ac && (ac->m_numAnimations > 0 ), "No animation loaded");
		m_animation =  ac->m_animations[0];

		HK_ASSERT2(0x27343435, ac && (ac->m_numBindings > 0), "No binding loaded");
		m_binding = ac->m_bindings[0];
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

	// Set the fill threshold
	m_skeletonInstance->setReferencePoseWeightThreshold( 0.05f );

	// animation controller
	{
		m_control = new hkaDefaultAnimationControl( m_binding );
		m_control->setPlaybackSpeed( 1.0f );

		m_skeletonInstance->addAnimationControl( m_control );
		m_control->removeReference();
	}

	// setup the graphics
	setupGraphics( );

	// find the bone indices for the left hand index finger and the left clavicle
	m_lFingerIndex = hkaSkeletonUtils::findBoneWithName( *m_skeleton, "HavokBipedRig L Hand" );
	m_lClavicleIndex = hkaSkeletonUtils::findBoneWithName( *m_skeleton, "HavokBipedRig L Clavicle" );

	// set the initial end position for the left finger
	m_targetPosition.set( 0.3f, -0.2f, 0.1f );

}


ReachingDemo::~ReachingDemo()
{
	// Re-enable warnings									
	hkError::getInstance().setEnabled(0xf0d1e423, true);

	m_skeletonInstance->removeReference();

	delete m_loader;

	hkDefaultDemo::setupLights(m_env); // assumes the hkx will have some.
}


hkDemo::Result ReachingDemo::stepDemo()
{
	const hkReal inc = 0.01f;

	// check for any input changes
	{
		const hkgPad* pad = m_env->m_gamePad;
		if ((pad->getButtonState() & HKG_PAD_DPAD_UP) != 0)
		{
			m_targetPosition(2) += inc;
		}

		if ((pad->getButtonState() & HKG_PAD_DPAD_LEFT) != 0)
		{
			m_targetPosition(0) -= inc;
		}

		if ((pad->getButtonState() & HKG_PAD_DPAD_RIGHT) != 0)
		{
			m_targetPosition(0) += inc;
		}

		if ((pad->getButtonState() & HKG_PAD_DPAD_DOWN) != 0)
		{
			m_targetPosition(2) -= inc;
		}
	}

	// display a blue star at the target finger position
	HK_DISPLAY_STAR( m_targetPosition, 0.15f, 0x000000ff );

	// Advance the active animations
	m_skeletonInstance->stepDeltaTime( 0.016f );

	hkaPose thePose (m_skeleton);
	// Sample the active animations and combine into a single pose
	m_skeletonInstance->sampleAndCombineAnimations( thePose.accessUnsyncedPoseLocalSpace().begin(), thePose.getFloatSlotValues().begin()  );

	// Solve the IK
	{
		// an array of constraints to be solved
		hkLocalArray<hkaCcdIkSolver::IkConstraint> constraints(1);
		constraints.setSize(1);

		// set the IK bone parameters
		constraints[0].m_endBone = m_lFingerIndex;
		constraints[0].m_startBone = m_lClavicleIndex;
		// set the IK target paramter
		constraints[0].m_targetMS = m_targetPosition;

		// perform the CCD IK
		hkaCcdIkSolver ccdIkSolver;
		ccdIkSolver.solve( constraints, thePose );
	}

	// get the bone indices for the fore arm and twist bones ( to allow re-sync'ing after IK )
	// The twist bones are procedurally linked to the lower arm in Character studio.
	hkInt16 twist = hkaSkeletonUtils::findBoneWithName( *m_skeleton, "HavokBipedRig L ForeTwist" );
	hkInt16 foreArm = hkaSkeletonUtils::findBoneWithName( *m_skeleton, "HavokBipedRig L ForeArm" );

	// re-sync the twist bone with the forearm
	thePose.setBoneLocalSpace(twist, thePose.getBoneLocalSpace(foreArm));

	// draw the skeleton pose
	AnimationUtils::drawPose( thePose, hkQsTransform::getIdentity() );

	// Skin
	{
		// Get the pose in world space
		const hkArray<hkQsTransform>& poseInWorld = thePose.getSyncedPoseModelSpace();

		int boneCount = m_skeleton->m_numBones;

		// Construct the composite world transform
		hkLocalArray<hkTransform> compositeWorldInverse( boneCount );
		compositeWorldInverse.setSize( boneCount );

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
	}

	return hkDemo::DEMO_OK;
}

#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
"A simple IK example that moves a character's hand to a given position.\n" \
"The target position may be changed by pressing up, down, left and right.";

HK_DECLARE_DEMO(ReachingDemo, HK_DEMO_TYPE_ANIMATION | HK_DEMO_TYPE_SERIALIZE, "Character reaching for a given target", helpString);

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
