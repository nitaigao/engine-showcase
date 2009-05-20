/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/Animation/Api/InverseKinematics/LookAt/LookAtDemo.h>

#include <Common/Base/Reflection/hkClass.h>
#include <Common/Base/Container/LocalArray/hkLocalArray.h>

// Serialization
#include <Common/Serialize/Util/hkLoader.h>
#include <Common/Serialize/Util/hkRootLevelContainer.h>

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

// IK
#include <Animation/Animation/Ik/LookAt/hkaLookAtIkSolver.h>

// Scene Data
#include <Common/SceneData/Scene/hkxScene.h>
#include <Common/SceneData/Mesh/hkxMesh.h>
#include <Animation/Animation/Rig/hkaPose.h>

// Common animation Utilities
#include <Demos/DemoCommon/Utilities/Animation/AnimationUtils.h>

// Graphics Stuff
#include <Demos/DemoCommon/DemoFramework/hkTextDisplay.h>
#include <Graphics/Bridge/SceneData/hkgSceneDataConverter.h>
#include <Graphics/Common/Window/hkgWindow.h>
#include <Graphics/Common/DisplayObject/hkgDisplayObject.h>

// Debug graphics
#include <Common/Visualize/hkDebugDisplay.h>

// Attachments
#include <Animation/Animation/Rig/hkaBoneAttachment.h>

LookAtDemo::LookAtDemo( hkDemoEnvironment* env )
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
		hkVector4 from( -1, -1, 1 );
		hkVector4 to  ( 0,0,0.75 );
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
		const char* skinFile = "Resources/Animation/HavokGirl/hkLowResSkinWithEyes.hkx";

		hkString assetFile = hkAssetManagementUtil::getFilePath( skinFile );
		hkRootLevelContainer* container = m_loader->load( assetFile.cString() );
		HK_ASSERT2(0x27343437, container != HK_NULL , "Could not load asset");

		hkxScene* scene = reinterpret_cast<hkxScene*>( container->findObjectByType( hkxSceneClass.getName() ));
		HK_ASSERT2(0x27343435, scene , "No scene loaded");

		hkaAnimationContainer* ac = reinterpret_cast<hkaAnimationContainer*>( container->findObjectByType( hkaAnimationContainerClass.getName() ));
		HK_ASSERT2(0x27343435, ac && (ac->m_numSkins > 0), "No skins loaded");

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
					hkgObject->setStatusFlags(hkgObject->getStatusFlags() | HKG_DISPLAY_OBJECT_DYNAMIC);
				}
			}

			m_attachmentObjects.pushBack(hkgObject);
		}
	}
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

	// find the bone indices for the neck, head and eyes.
	m_neckIndex = hkaSkeletonUtils::findBoneWithName( *m_skeleton, "HavokBipedRig Neck" );
	m_headIndex = hkaSkeletonUtils::findBoneWithName( *m_skeleton, "HavokBipedRig Head" );
	m_lEyeIndex = hkaSkeletonUtils::findBoneWithName( *m_skeleton, "LowEyeL" );
	m_rEyeIndex = hkaSkeletonUtils::findBoneWithName( *m_skeleton, "LowEyeR" );
}


LookAtDemo::~LookAtDemo()
{	
	// Re-enable warnings									
	hkError::getInstance().setEnabled(0xf0d1e423, true);

	m_skeletonInstance->removeReference();

	delete m_loader;

	hkDefaultDemo::setupLights(m_env); // assumes the hkx will have some.
}


hkDemo::Result LookAtDemo::stepDemo()
{
	// Advance the active animations
	m_skeletonInstance->stepDeltaTime( 0.016f );

	int boneCount = m_skeleton->m_numBones;	
	hkaPose pose (m_skeleton);

	// Sample the active animations and combine into a single pose
	m_skeletonInstance->sampleAndCombineAnimations( pose.accessUnsyncedPoseLocalSpace().begin(), pose.getFloatSlotValues().begin()  );

	// m_targetPosition is similar to camera
	hkVector4 newTargetPosition;
	m_env->m_window->getCurrentViewport()->getCamera()->getFrom((float*)&newTargetPosition);

	hkaLookAtIkSolver::Setup setup;
	setup.m_fwdLS.set(0,1,0);
	setup.m_eyePositionLS.set(0.1f,0.1f,0);
	setup.m_limitAxisMS.setRotatedDir(pose.getBoneModelSpace(m_neckIndex).getRotation(), hkVector4(0,1,0));
	setup.m_limitAngle = HK_REAL_PI / 3.0f;

	// Check if the angle-limited variant is in use
	if ( m_variantId == 0 )
	{
		// Solve normally
		hkaLookAtIkSolver::solve (setup, newTargetPosition, 1.0f, pose.accessBoneModelSpace(m_headIndex, hkaPose::PROPAGATE), HK_NULL );
	}
	else
	{
		// Define the range of motion
		hkaLookAtIkSolver::RangeLimits range;
		range.m_limitAngleLeft = 90.0f * HK_REAL_DEG_TO_RAD;
		range.m_limitAngleRight = -90.0f * HK_REAL_DEG_TO_RAD;
		range.m_limitAngleUp = 30.0f * HK_REAL_DEG_TO_RAD;
		range.m_limitAngleDown = -10.0f * HK_REAL_DEG_TO_RAD;

		hkVector4 upLS;
		upLS.set(1,0,0);
		range.m_upAxisMS.setRotatedDir(pose.getBoneModelSpace(m_neckIndex).getRotation(), upLS);

		// Solve using user-defined range of motion
		hkaLookAtIkSolver::solve (setup, newTargetPosition, 1.0f, pose.accessBoneModelSpace(m_headIndex, hkaPose::PROPAGATE), &range );
	}


	setup.m_fwdLS.set(0,-1,0);
	setup.m_eyePositionLS.set(0.0f,0.0f,0);
	setup.m_limitAxisMS.setRotatedDir(pose.getBoneModelSpace(m_headIndex).getRotation(), hkVector4(0,1,0));
	setup.m_limitAngle = HK_REAL_PI / 10.0f;
	hkaLookAtIkSolver::solve (setup, newTargetPosition, 1.0f, pose.accessBoneModelSpace(m_rEyeIndex, hkaPose::PROPAGATE));

	setup.m_fwdLS.set(0,-1,0);
	setup.m_eyePositionLS.set(0.0f,0.0f,0);
	setup.m_limitAxisMS.setRotatedDir(pose.getBoneModelSpace(m_headIndex).getRotation(), hkVector4(0,1,0));
	setup.m_limitAngle = HK_REAL_PI / 10.0f;
	hkaLookAtIkSolver::solve (setup, newTargetPosition, 1.0f, pose.accessBoneModelSpace(m_lEyeIndex, hkaPose::PROPAGATE));
	
	//
	// draw the skeleton pose
	//
	AnimationUtils::drawPose( pose, hkQsTransform::getIdentity() );

	// Skin
	{
		// Work with the pose in world
		const hkArray<hkQsTransform>& poseInWorld = pose.getSyncedPoseModelSpace();

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
"A simple IK example that moves a character's head and eyes to look at a given position.\n" \
"This demo uses the camera position as the target. Move the camera to see it in action.";

HK_DECLARE_DEMO_VARIANT( LookAtDemo, HK_DEMO_TYPE_ANIMATION | HK_DEMO_TYPE_SERIALIZE, "Character looking at a camera with single limit angle", 0, helpString, "Character's head moves within cone of 60 degrees.");
HK_DECLARE_DEMO_VARIANT( LookAtDemo, HK_DEMO_TYPE_ANIMATION | HK_DEMO_TYPE_SERIALIZE, "Character looking at a camera with individual limit angles", 1, helpString, "Character's head moves +90/-90 L/R and +30/-10 Up/Dn");

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
