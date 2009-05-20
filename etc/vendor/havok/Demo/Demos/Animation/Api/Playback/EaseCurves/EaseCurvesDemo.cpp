/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/Animation/Api/Playback/EaseCurves/EaseCurvesDemo.h>

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


const int GRAPH_CANVAS_HEIGHT=64;
const int GRAPH_CANVAS_WIDTH=64;

EaseCurvesDemo::EaseCurvesDemo( hkDemoEnvironment* env )
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
		hkVector4 from( 2,-2,1 );
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

	// Get the animation and the binding
	{
		hkString assetFile = hkAssetManagementUtil::getFilePath("Resources/Animation/HavokGirl/hkRunLoop.hkx");
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

		assetFile = hkAssetManagementUtil::getFilePath("Resources/Animation/HavokGirl/hkIdle.hkx");
		container = m_loader->load( assetFile.cString() );
		ac = reinterpret_cast<hkaAnimationContainer*>( container->findObjectByType( hkaAnimationContainerClass.getName() ));

		HK_ASSERT2(0x27343435, ac && (ac->m_numAnimations > 0 ), "No animation loaded");
		m_animation[2] =  ac->m_animations[0];

		HK_ASSERT2(0x27343435, ac && (ac->m_numBindings > 0), "No binding loaded");
		m_binding[2] = ac->m_bindings[0];
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
	m_skeletonInstance->setReferencePoseWeightThreshold( 0.1f );

	// Grab the animations
	for (int i=0; i < NUM_ANIMS; i++)
	{
		const hkBool startEasedIn = ( i == 2 ); // Start with hands on hips only
		m_control[i] = new hkaDefaultAnimationControl( m_binding[i], startEasedIn );
		m_control[i]->setMasterWeight( 1.0f );
		m_control[i]->setPlaybackSpeed( 1.0f );

		m_skeletonInstance->addAnimationControl( m_control[i] );
		m_control[i]->removeReference();
	}

	// Set ease curves explicitly
	m_control[0]->setEaseInCurve(0, 0, 1, 1);	// Smooth
	m_control[0]->setEaseOutCurve(1, 1, 0, 0);	// Smooth

	m_control[1]->setEaseInCurve(0, 0.33f, 0.66f, 1);	// Linear
	m_control[1]->setEaseOutCurve(1, 0.66f, 0.33f, 0);	// Linear

	m_control[2]->setEaseInCurve(0, 0, 0, 1);	// Fast
	m_control[2]->setEaseOutCurve(1, 0, 0, 0);	// Fast

	// make a world so that we can auto create a display world to hold the skin
	setupGraphics( );

	//
	//	Create the texture
	//
	{
		m_graphCanvasData = hkAllocate<unsigned char> (GRAPH_CANVAS_HEIGHT*GRAPH_CANVAS_WIDTH*4, HK_MEMORY_CLASS_DEMO);
		m_context = env->m_window->getContext();
		m_texture = hkgTexture::create( m_context );
		m_texture->allocateSurface( GRAPH_CANVAS_WIDTH, GRAPH_CANVAS_HEIGHT, true, false, m_graphCanvasData );
		m_textureRealized = false;
	}
}

EaseCurvesDemo::~EaseCurvesDemo()
{
	// Re-enable warnings									
	hkError::getInstance().setEnabled(0xf0d1e423, true); 
	

	m_skeletonInstance->removeReference();

	delete m_loader;

	hkDeallocate<unsigned char>(m_graphCanvasData);

	m_env->m_window->getContext()->lock();
	m_texture->free();
	m_texture->release();
	m_env->m_window->getContext()->unlock();
}

const hkUint32 curveCols[3] =
{
	0xffffff00,
		0xffff00ff,
		0xff00ffff
};

hkDemo::Result EaseCurvesDemo::stepDemo()
{

	// Display current settings
	{
		char buf[255];
		for (int i=0; i< NUM_ANIMS; i++)
		{
			hkString::sprintf(buf, "anim%d:%0.3f", i, m_control[i]->getWeight());
			m_env->m_textDisplay->outputText( buf, 10, 240+14*i, curveCols[i], 1);
		}
	}

	// Check user input
	{
		for (int i=0; i < 3; i++)
		{
			if (m_env->m_gamePad->wasButtonPressed( 1 << (i+1) ))
			{
				if (( m_control[i]->getEaseStatus() == hkaDefaultAnimationControl::EASING_IN ) ||
					( m_control[i]->getEaseStatus() == hkaDefaultAnimationControl::EASED_IN ))
				{
					m_control[i]->easeOut( 1.0f );
				}
				else
				{
					m_control[i]->easeIn( 1.0f );
				}
			}
		}
	}

	const int boneCount = m_skeleton->m_numBones;

	// Advance the active animations
	m_skeletonInstance->stepDeltaTime( 1.0f / 60.0f );

	// Sample the active animations and combine into a single pose
	hkaPose pose (m_skeleton);
	m_skeletonInstance->sampleAndCombineAnimations( pose.accessUnsyncedPoseLocalSpace().begin(), pose.getFloatSlotValues().begin()  );
	AnimationUtils::drawPose( pose, hkQsTransform::getIdentity() );

	// Construct the composite world transform
	hkLocalArray<hkTransform> compositeWorldInverse( boneCount );
	compositeWorldInverse.setSize( boneCount );

	const hkArray<hkQsTransform>& poseModelSpace = pose.getSyncedPoseModelSpace();

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
			compositeWorldInverse[p].setMul( poseModelSpace[ boneIndex ], m_skinBindings[i]->m_boneFromSkinMeshTransforms[ boneIndex ] );
		}

		AnimationUtils::skinMesh( *m_skinBindings[i]->m_mesh, hkTransform::getIdentity(), compositeWorldInverse.begin(), *m_env->m_sceneConverter );
	}

	// Show curves
	{
		int* data = reinterpret_cast<int*>(m_texture->getDataPointer());

		//Shuffle Texture
		for (int w=0; w< GRAPH_CANVAS_WIDTH-1; w++)
		{
			for (int h=0; h< GRAPH_CANVAS_HEIGHT; h++)
			{
				data[h * GRAPH_CANVAS_WIDTH + w] = data[h * GRAPH_CANVAS_WIDTH + w+1];
			}
		}

		// Clear the line
		for (int h=0; h< GRAPH_CANVAS_HEIGHT; h++)
		{
			data[h * GRAPH_CANVAS_WIDTH + GRAPH_CANVAS_WIDTH-1] = 0xff7f7f7f;
		}

		// Take a new sample
		for (int c=0; c< 3; c++)
		{
			int y = (int)(m_control[c]->getWeight() * (GRAPH_CANVAS_HEIGHT-5));
			data[(GRAPH_CANVAS_HEIGHT-2 - (y+c+1)) * GRAPH_CANVAS_WIDTH + GRAPH_CANVAS_WIDTH-1] = curveCols[c];
			data[(GRAPH_CANVAS_HEIGHT-2 - (y+c)) * GRAPH_CANVAS_WIDTH + GRAPH_CANVAS_WIDTH-1] &= 0xff0f0f0f;
		}
	}

	m_context->lock();

	if (m_textureRealized)
	{
		m_texture->free();
	}
	m_texture->realize(true);
	m_textureRealized = true;
	drawTexture( m_texture );

	m_context->unlock();

	return hkDemo::DEMO_OK;
}


void EaseCurvesDemo::drawTexture( hkgTexture* texture )
{
	hkgViewport* orthoView = m_env->m_window->getWindowOrthoView();  //2D ortho, width by height
	float windowHeight = (float)m_env->m_window->getHeight();

	m_context->lock();

	orthoView->setAsCurrent(m_context); // 2D

	m_context->setDepthReadState(false);
	m_context->setDepthWriteState(true);

	m_context->setCurrentSoleTexture( texture, HKG_TEXTURE_MODULATE );
	m_context->setTexture2DState( true ); // turn on textures for this
	m_context->setBlendState( false );

	float white[4] = { 1.0f, 1.0f, 1.0f, 1.0f};
	m_context->setCurrentColor4( white );

	float p[3];
	float uv[2];
	p[2] = -0.01f; //depth (0..-2)

	float tl[3];
	float lr[3];

	hkReal wOffset = 120;
	hkReal hOffset = 230;

	tl[0] = 0.0f + wOffset;
	tl[1] = windowHeight  - hOffset;

	const int TEXTURE_SCALE = 2;
	lr[0] = float( GRAPH_CANVAS_WIDTH * TEXTURE_SCALE) + wOffset;
	lr[1] = windowHeight - float( GRAPH_CANVAS_HEIGHT) - hOffset;

	m_context->beginGroup( HKG_IMM_TRIANGLE_LIST );

	p[0] = tl[0]; p[1] = tl[1];
	uv[0] = 0.0f; uv[1] = 0.0f;
	m_context->setCurrentTextureCoord( uv );
	m_context->setCurrentPosition( p );

	p[0] = tl[0]; p[1] = lr[1];
	uv[0] = 0.0f; uv[1] = 1.0f;
	m_context->setCurrentTextureCoord( uv );
	m_context->setCurrentPosition( p );

	p[0] = lr[0]; p[1] = tl[1];
	uv[0] = 1.0f; uv[1] = 0.0f;
	m_context->setCurrentTextureCoord( uv );
	m_context->setCurrentPosition( p );

	p[0] = tl[0]; p[1] = lr[1];
	uv[0] = 0.0f; uv[1] = 1.0f;
	m_context->setCurrentTextureCoord( uv );
	m_context->setCurrentPosition( p );

	p[0] = lr[0]; p[1] = tl[1];
	uv[0] = 1.0f; uv[1] = 0.0f;
	m_context->setCurrentTextureCoord( uv );
	m_context->setCurrentPosition( p );

	p[0] = lr[0]; p[1] = lr[1];
	uv[0] = 1.0f; uv[1] = 1.0f;
	m_context->setCurrentTextureCoord( uv );
	m_context->setCurrentPosition( p );

	m_context->endGroup();

	m_context->setDepthReadState(true);
	m_context->setDepthWriteState(true);
	m_context->setCurrentSoleTexture( HK_NULL, HKG_TEXTURE_MODULATE );

	m_context->unlock();
}

#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

HK_DECLARE_DEMO(EaseCurvesDemo, HK_DEMO_TYPE_ANIMATION | HK_DEMO_TYPE_SERIALIZE, "Ease Curves", "Press \221,\222,\223 to ease in / out different animations");

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
