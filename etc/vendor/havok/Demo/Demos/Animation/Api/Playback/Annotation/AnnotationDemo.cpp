/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


#include <Demos/demos.h>
#include <Demos/Animation/Api/Playback/Annotation/AnnotationDemo.h>

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

// Scene Data
#include <Common/SceneData/Scene/hkxScene.h>
#include <Common/SceneData/Mesh/hkxMesh.h>

// Common animation Utilities
#include <Demos/DemoCommon/Utilities/Animation/AnimationUtils.h>

// Graphics Stuff
#include <Demos/DemoCommon/DemoFramework/hkTextDisplay.h>
#include <Graphics/Bridge/SceneData/hkgSceneDataConverter.h>
#include <Common/Visualize/hkDebugDisplay.h>
#include <Graphics/Common/Camera/hkgCamera.h>


AnnotationDemo::AnnotationDemo( hkDemoEnvironment* env )
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
		hkVector4 from(  365, -330, 200 );
		hkVector4 to  ( 0,0,0 );
		hkVector4 up  ( 0.0f,  0.0f, 1.0f);
		setupDefaultCameras( env, from, to, up, 0.1f, 10000.0f );
	}

	m_loader = new hkLoader();

	// Convert the scene
	{
		hkString assetFile = hkAssetManagementUtil::getFilePath("Resources/Animation/Teapot/teapot.hkx");
		hkRootLevelContainer* container = m_loader->load( assetFile.cString() );
		HK_ASSERT2(0x27343437, container != HK_NULL , "Could not load asset");
		hkaAnimationContainer* ac = reinterpret_cast<hkaAnimationContainer*>( container->findObjectByType( hkaAnimationContainerClass.getName() ));
		hkxScene* scene = reinterpret_cast<hkxScene*>( container->findObjectByType( hkxSceneClass.getName() ));

		HK_ASSERT2(0x27343635, scene, "No scene loaded");
		env->m_sceneConverter->convert( scene );

		HK_ASSERT2(0x27343435, ac && (ac->m_numSkeletons > 0), "No skeleton loaded");
		m_skeleton = ac->m_skeletons[0];

		HK_ASSERT2(0x27343435, ac && (ac->m_numAnimations > 0), "No animation loaded");
		m_animation = ac->m_animations[0];

		HK_ASSERT2(0x27343435, ac && (ac->m_numBindings > 0), "No binding loaded");
		m_binding = ac->m_bindings[0];

		HK_ASSERT2(0x27343435, ac->m_numSkins > 0, "No skins loaded");
		m_skinBindings = ac->m_skins;
		m_numSkinBindings = ac->m_numSkins;
	}

	m_skeletonInstance = new hkaAnimatedSkeleton( m_skeleton );

	m_loopControl = new hkaDefaultAnimationControl( m_binding );

	m_loopControl->setPlaybackSpeed(1.0f);

	m_skeletonInstance->addAnimationControl( m_loopControl );

	// set up the vdb
	setupGraphics();
}

AnnotationDemo::~AnnotationDemo()
{
	// Re-enable warnings
	hkError::getInstance().setEnabled(0xf0d1e423, true);

	m_skeletonInstance->removeReference();
	delete m_loader;
	m_loopControl->removeReference();

	hkDefaultDemo::setupLights(m_env); // restore lights to default.
}

hkDemo::Result AnnotationDemo::stepDemo()
{
	const hkReal delta = 0.016f;

	
	// Grab any track note annotations that are present and valid for the time step
	// Usually we would just query for this delta, but for demo purpose we extend this.
	// This means the annotations are displayed on screen for longer
	hkReal delay = 0.25f;

	// Query to see how many annotation tracks we have.
	const hkUint32 numAnnotations = m_skeletonInstance->getNumAnnotations( delta + delay );

	// Create a temporary array and query for the annotations.
	hkLocalArray<hkaAnimatedSkeleton::BoneAnnotation> annotations( numAnnotations );
	annotations.setSize( numAnnotations );
	m_skeletonInstance->getAnnotations( delta + delay, annotations.begin() );

	// Advance the active animations
	m_skeletonInstance->stepDeltaTime( delta );

	const int boneCount = m_skeleton->m_numParentIndices;
	

	// Sample the active animations and combine into a single pose
	hkaPose pose (m_skeleton);
	m_skeletonInstance->sampleAndCombineAnimations( pose.accessUnsyncedPoseLocalSpace().begin(), pose.getFloatSlotValues().begin()  );
	AnimationUtils::drawPose( pose, hkQsTransform::getIdentity() );


	// Construct the composite world transform
	{
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
	}

	// We use/display the annotation info here
	{
		hkReal offset = 140;

		const hkArray<hkQsTransform>& poseInWorld = pose.getSyncedPoseModelSpace();

		// Iterate through the annotations and display them
		for( hkUint32 a = 0; a < numAnnotations; a++ )
		{
			// Get the bone ID
			const hkUint16 boneID = annotations[ a ].m_boneID;

			// get the bone position of the text
			const hkVector4& position = poseInWorld[ boneID ].m_translation;

			// Move the text position so it is clearly visible
			hkVector4 textPosition = position;
			textPosition(2) = offset; offset +=20;

			// display the text
			m_env->m_textDisplay->outputText3D( annotations[ a ].m_annotation.m_text , textPosition(0), textPosition(1), textPosition(2), 0xffffffff );

			// Draw an arrow to the actual position of the bone
			hkVector4 direction; direction.setSub4( position, textPosition );
			HK_DISPLAY_ARROW( textPosition, direction, hkColor::GREEN );
		}
	}

	return hkDemo::DEMO_OK;
}

#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

HK_DECLARE_DEMO(AnnotationDemo, HK_DEMO_TYPE_ANIMATION | HK_DEMO_TYPE_SERIALIZE, "Annotations", "Display track note (3dsMax) annotations");

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
