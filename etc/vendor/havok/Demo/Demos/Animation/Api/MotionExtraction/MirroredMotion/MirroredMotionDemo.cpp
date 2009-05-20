/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/Animation/Api/MotionExtraction/MirroredMotion/MirroredMotionDemo.h>

#include <Common/Base/Reflection/hkClass.h>

// Asset mgt
#include <Demos/DemoCommon/Utilities/Asset/hkAssetManagementUtil.h>

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

// Attachments
#include <Animation/Animation/Rig/hkaBoneAttachment.h>

#include <Animation/Animation/Animation/Mirrored/hkaMirroredAnimation.h>
#include <Animation/Animation/Animation/Mirrored/hkaMirroredSkeleton.h>


const int GRAPH_CANVAS_HEIGHT=64;
const int GRAPH_CANVAS_WIDTH=64;


struct MirroredMotionVariant
{
	const char* m_name;
	const char* m_animationFileName;
	const char* m_details;
};

const hkReal s2o2 = hkMath::sqrt ( 2.0f ) / 2.0f;
const char *myHelpString = "\221 Toggle Animation - \222 Wireframe - \223 Show Skeleton\n\220 Toggle extracted motion - \232 Reset extracted motion - \233 Pause";

static const MirroredMotionVariant g_variants[] =
{
	{ "Havok Girl Idle", "Resources/Animation/HavokGirl/hkIdle.hkx", myHelpString },
	{ "Havok Girl Jump Land Loop", "Resources/Animation/HavokGirl/hkJumpLandLoop.hkx", myHelpString },
	{ "Havok Girl Long Anim", "Resources/Animation/HavokGirl/hkLongAnim.hkx", myHelpString },
	{ "Havok Girl Run Loop", "Resources/Animation/HavokGirl/hkRunLoop.hkx", myHelpString },
	{ "Havok Girl Run Turn Left Loop", "Resources/Animation/HavokGirl/hkRunTurnLLoop.hkx", myHelpString },
	{ "Havok Girl Run Turn Right Loop", "Resources/Animation/HavokGirl/hkRunTurnRLoop.hkx", myHelpString },
	{ "Havok Girl Walk Loop", "Resources/Animation/HavokGirl/hkWalkLoop.hkx", myHelpString },
	{ "Havok Girl Walk Turn Left Loop", "Resources/Animation/HavokGirl/hkWalkTurnLLoop.hkx", myHelpString },
	{ "Havok Girl Walk Turn Right Loop", "Resources/Animation/HavokGirl/hkWalkTurnRLoop.hkx", myHelpString },
	{ "Havok Girl Wave", "Resources/Animation/HavokGirl/hkWaveLoop.hkx", myHelpString }
};



MirroredMotionDemo::MirroredMotionDemo( hkDemoEnvironment* env )
:	hkDefaultAnimationDemo(env)
{
	// Character parameters
//	const char *sceneFileName = "Resources/Animation/Scene/hkScene.hkx";
	const char *rigFileName = "Resources/Animation/HavokGirl/hkRig.hkx";
			const char *skinFileName = "Resources/Animation/HavokGirl/hkLowResSkin.hkx";

	// Mirroring Parameters
	const char * leftPrefix[2] = { " L ", "EyeL" };
	const char *rightPrefix[2] = { " R ", "EyeR" };
	const int numPrefix = 2;
	const hkQuaternion mirrorAxis( 1.0f, 0.0f, 0.0f, 0.0f );

	// Show backfaces so that you can see inside of things
	setGraphicsState( HKG_ENABLED_CULLFACE, false );

	env->m_sceneConverter->setAllowTextureSharing( true );
	env->m_sceneConverter->setAllowMaterialSharing( true );

	//
	// Setup the camera
	//
	{
		hkVector4 from( 0,-6,1 );
		hkVector4 to  ( 0,0,0 );
		hkVector4 up  ( 0.0f, 0.0f, 1.0f );
		setupDefaultCameras( env, from, to, up, 0.01f, 100 );
	}

	m_loader = new hkLoader();

	// Get the rig
	{
		hkString assetFile = hkAssetManagementUtil::getFilePath( rigFileName );
		hkRootLevelContainer* container = m_loader->load( assetFile.cString() );
		HK_ASSERT2(0x27343437, container != HK_NULL , "Could not load asset");
		hkaAnimationContainer* ac = reinterpret_cast<hkaAnimationContainer*>( container->findObjectByType( hkaAnimationContainerClass.getName() ));

		HK_ASSERT2(0x27343435, ac && (ac->m_numSkeletons > 0), "No skeleton loaded");
		m_skeleton = ac->m_skeletons[0];
	}

	// Get the animation and the binding
	{
		hkString assetFile = hkAssetManagementUtil::getFilePath(
			g_variants[ env->m_variantId ].m_animationFileName );
		hkRootLevelContainer* container = m_loader->load( assetFile.cString() );
		HK_ASSERT2(0x27343437, container != HK_NULL , "Could not load asset");
		hkaAnimationContainer* ac = reinterpret_cast<hkaAnimationContainer*>( container->findObjectByType( hkaAnimationContainerClass.getName() ));

		HK_ASSERT2(0x27343435, ac && (ac->m_numAnimations > 0 ), "No animation loaded");
		m_animation[0] =  ac->m_animations[0];

		HK_ASSERT2(0x27343435, ac && (ac->m_numBindings > 0), "No binding loaded");
		m_binding[0] = ac->m_bindings[0];
	}

	// Create the mirrored animation and binding
	{
		hkObjectArray< hkString > ltag;
		hkObjectArray< hkString > rtag;

		for ( int j = 0; j < ( numPrefix ); j++ )
		{
			ltag.pushBack( ( leftPrefix  )[ j ] );
			rtag.pushBack( ( rightPrefix )[ j ] );
		}

		hkaMirroredSkeleton *mirroredSkeleton = new hkaMirroredSkeleton( m_skeleton );

		mirroredSkeleton->computeBonePairingFromNames( ltag, rtag );

		hkaMirroredAnimation *mirroredAnimation = new hkaMirroredAnimation( m_animation[0], m_binding[0], mirroredSkeleton );
		mirroredSkeleton->removeReference();

		mirroredSkeleton->setAllBoneInvariantsFromReferencePose( mirrorAxis, 0.0f );

		m_binding[1] = mirroredAnimation->createMirroredBinding();
		m_animation[1] = mirroredAnimation;
	}

	// Create the skeletons
	for ( int i = 0; i < 2; i++ )
	{
		m_skeletonInstance[i] = new hkaAnimatedSkeleton( m_skeleton );
		m_skeletonInstance[i]->setReferencePoseWeightThreshold( 0.1f );
	}

	// Convert the skin (once for each character standard and mirrored)
	for ( int i = 0; i < 2; i++ )
	{
		hkString assetFile = hkAssetManagementUtil::getFilePath( skinFileName );
		hkRootLevelContainer* container = m_loader->load( assetFile.cString() );
		if ( container != HK_NULL )
		{
			HK_ASSERT2(0x27343437, container != HK_NULL , "Could not load asset");

			hkxScene* scene = reinterpret_cast<hkxScene*>( container->findObjectByType( hkxSceneClass.getName() ));
			HK_ASSERT2(0x27343435, scene , "No scene loaded");

			hkaAnimationContainer* ac = reinterpret_cast<hkaAnimationContainer*>( container->findObjectByType( hkaAnimationContainerClass.getName() ));
			HK_ASSERT2(0x27343435, ac && (ac->m_numSkins > 0), "No skins loaded");

			m_numSkinBindings[i] = ac->m_numSkins;
			m_skinBindings[i] = ac->m_skins;

			m_numAttachments[i] = ac->m_numAttachments;
			m_attachments[i] = ac->m_attachments;

			// Make graphics output buffers for the skins
			env->m_sceneConverter->convert( scene );

			// Handle the attachements
			for (int a=0; a < m_numAttachments[i]; ++a)
			{
				hkaBoneAttachment* ba = m_attachments[i][a];
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

				m_attachmentObjects[i].pushBack(hkgObject);
			}
		}
		else
		{
			m_numSkinBindings[i] = 0;
			m_skinBindings[i] = 0;
			m_numAttachments[i] = 0;
		}
	}

	// Grab the animations
	for ( int i = 0; i < 2; i++ )
	{
		m_control[i] = new hkaDefaultAnimationControl( m_binding[i] );
		m_control[i]->setMasterWeight( 1.0f );
		m_control[i]->setPlaybackSpeed( 1.0f );

		// Ease all controls out initially
		m_control[i]->easeIn(1.0f);

		m_skeletonInstance[i]->addAnimationControl( m_control[i] );
		m_control[i]->removeReference();

		// Set ease curves explicitly
		m_control[i]->setEaseInCurve(0, 0, 1, 1);	// Smooth
		m_control[i]->setEaseOutCurve(1, 1, 0, 0);	// Smooth

		m_accumulatedMotion[i].setIdentity();
	}

	// make a world so that we can auto create a display world to hold the skin
	setupGraphics( );

	m_axisCounter = 0;
	m_env = env;

	m_drawSkin = true;
	m_wireframe = false;
	m_timestep = 1.0f / 60.0f;
	m_useExtractedMotion = true;
	m_paused = false;
	m_separation = 1.0f;
}

MirroredMotionDemo::~MirroredMotionDemo()
{
	m_skeletonInstance[0]->removeReference();
	m_skeletonInstance[1]->removeReference();

	m_animation[1]->removeReference();
	hkaMirroredAnimation::destroyMirroredBinding( m_binding[1] );

	delete m_loader;
}

void MirroredMotionDemo::makeFakeInput()
{
	// Fake a button press when demo is done easing
	{
		if (( m_control[0]->getEaseStatus() == hkaDefaultAnimationControl::EASED_OUT ) ||
			( m_control[0]->getEaseStatus() == hkaDefaultAnimationControl::EASED_IN ))
		{
			m_env->m_gamePad->forceButtonPressed(HKG_PAD_BUTTON_1);
		}
	}
	
}

hkDemo::Result MirroredMotionDemo::stepDemo()
{
	// Check user input
	{
		if (m_env->m_gamePad->wasButtonPressed( HKG_PAD_BUTTON_1 ))
		{
			for ( int i = 0; i < 2; i++ )
			{
				if (( m_control[i]->getEaseStatus() == hkaDefaultAnimationControl::EASING_IN ) ||
					( m_control[i]->getEaseStatus() == hkaDefaultAnimationControl::EASED_IN ))
				{
					m_control[i]->easeOut( .5f );
				}
				else
				{
					m_control[i]->easeIn( .5f );
				}
			}
		}

		if (m_env->m_gamePad->wasButtonPressed( HKG_PAD_BUTTON_2 ))
		{
			m_wireframe = !m_wireframe;
			setGraphicsState( HKG_ENABLED_WIREFRAME, m_wireframe );
		}

		if (m_env->m_gamePad->wasButtonPressed( HKG_PAD_BUTTON_3 ))
		{
			m_drawSkin = !m_drawSkin;
		}

		if ( m_env->m_gamePad->wasButtonPressed( HKG_PAD_BUTTON_0 ) )
		{
			m_useExtractedMotion = !m_useExtractedMotion;
		}

		if ( m_env->m_gamePad->wasButtonPressed( HKG_PAD_BUTTON_L1 ) )
		{
			m_accumulatedMotion[0].setIdentity();
			m_accumulatedMotion[1].setIdentity();
		}

		if ( m_env->m_gamePad->wasButtonPressed( HKG_PAD_BUTTON_R1 ) )
		{
			m_paused = !m_paused;

			if ( m_paused )
			{
				m_timestep = 0;
			}
			else
			{
				m_timestep = 1.0f / 60.0f;
			}
		}


	}

	const int boneCount = m_skeleton->m_numBones;

	for ( int j = 0; j < 2; j++ )
	{
		// Grab accumulated motion
		{
			hkQsTransform deltaMotion;
			deltaMotion.setIdentity();
			m_skeletonInstance[j]->getDeltaReferenceFrame( m_timestep, deltaMotion );
			m_accumulatedMotion[j].setMulEq( deltaMotion );
		}

		// Advance the active animations
		m_skeletonInstance[j]->stepDeltaTime( m_timestep );

		// Sample the active animations and combine into a single pose
		hkaPose pose (m_skeleton);
		m_skeletonInstance[j]->sampleAndCombineAnimations( pose.accessUnsyncedPoseLocalSpace().begin(), pose.getFloatSlotValues().begin() );

		hkReal separation = m_separation * hkReal( 2*j-1 );

		hkQsTransform Q( hkQsTransform::IDENTITY );
		Q.m_translation.set( separation, 0, 0 );

		if ( m_useExtractedMotion )
		{
			Q.setMulEq( m_accumulatedMotion[j] );
		}

		pose.syncModelSpace();

		AnimationUtils::drawPose( pose, Q );
		// Test if the skin is to be drawn
		if ( !m_drawSkin )
		{
			Q.m_translation( 2 ) -= 2.0f * m_separation;
		}

		// Construct the composite world transform
		hkLocalArray<hkTransform> compositeWorldInverse( boneCount );
		compositeWorldInverse.setSize( boneCount );

		const hkArray<hkQsTransform>& poseModelSpace = pose.getSyncedPoseModelSpace();

		// Skin the meshes
		for (int i=0; i < m_numSkinBindings[j]; i++)
		{
			// assumes either a straight map (null map) or a single one (1 palette)
			hkInt16* usedBones = m_skinBindings[j][i]->m_mappings? m_skinBindings[j][i]->m_mappings[0].m_mapping : HK_NULL;
			int numUsedBones = usedBones? m_skinBindings[j][i]->m_mappings[0].m_numMapping : boneCount;

			// Multiply through by the bind pose inverse world inverse matrices
			for (int p=0; p < numUsedBones; p++)
			{
				int boneIndex = usedBones? usedBones[p] : p;
				hkTransform tWorld; poseModelSpace[ boneIndex ].copyToTransform(tWorld);
				compositeWorldInverse[p].setMul( tWorld, m_skinBindings[j][i]->m_boneFromSkinMeshTransforms[ boneIndex ] );
			}

			AnimationUtils::skinMesh( *m_skinBindings[j][i]->m_mesh, hkTransform( Q.m_rotation, hkVector4( Q.m_translation(0), Q.m_translation(1), Q.m_translation(2), 1 ) ), compositeWorldInverse.begin(), *m_env->m_sceneConverter );
		}

		// Move the attachments
		{
			HK_ALIGN(float f[16], 16);
			for (int a=0; a < m_numAttachments[j]; a++)
			{
				if (m_attachmentObjects[j][a])
				{
					hkaBoneAttachment* ba = m_attachments[j][a];
					hkQsTransform modelFromBone = pose.getBoneModelSpace( ba->m_boneIndex );
					hkQsTransform worldFromBoneQs;
					worldFromBoneQs.setMul( Q, modelFromBone );
					worldFromBoneQs.get4x4ColumnMajor(f);
					hkMatrix4 worldFromBone; worldFromBone.set4x4ColumnMajor(f);
					hkMatrix4 worldFromAttachment; worldFromAttachment.setMul(worldFromBone, ba->m_boneFromAttachment);
					m_env->m_sceneConverter->updateAttachment(m_attachmentObjects[j][a], worldFromAttachment);
				}
			}
		}


	}

	return hkDemo::DEMO_OK;
}


#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif


HK_DECLARE_DEMO_VARIANT_USING_STRUCT( MirroredMotionDemo, HK_DEMO_TYPE_OTHER, MirroredMotionVariant, g_variants, "Demo of mirroring animations" );

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
