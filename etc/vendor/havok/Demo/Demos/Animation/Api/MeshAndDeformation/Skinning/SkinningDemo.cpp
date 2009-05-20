/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/Animation/Api/MeshAndDeformation/Skinning/SkinningDemo.h>
#include <Common/Base/Reflection/hkClass.h>
#include <Common/Base/Container/LocalArray/hkLocalArray.h>

// Serialization
#include <Common/Serialize/Util/hkLoader.h>
#include <Common/Serialize/Util/hkRootLevelContainer.h>
#include <Common/Base/Reflection/Registry/hkTypeInfoRegistry.h>

// Asset mgt
#include <Demos/DemoCommon/Utilities/Asset/hkAssetManagementUtil.h>

// Vertex Deformation
#include <Common/Base/Config/hkConfigSimd.h>
#include <Animation/Animation/Deform/Skinning/hkaMeshBinding.h>
#include <Animation/Animation/Deform/Skinning/Fpu/hkaFPUSkinningDeformer.h>
#include <Animation/Animation/Deform/Skinning/Simd/hkaSimdSkinningDeformer.h>

// Skeletal Animation
#include <Animation/Animation/hkaAnimationContainer.h>
#include <Animation/Animation/Animation/Interleaved/hkaInterleavedUncompressedAnimation.h>
#include <Animation/Animation/Rig/hkaSkeleton.h>
#include <Animation/Animation/Rig/hkaSkeletonUtils.h>
#include <Animation/Animation/Playback/hkaAnimatedSkeleton.h>
#include <Animation/Animation/Playback/Control/Default/hkaDefaultAnimationControl.h>
#include <Animation/Animation/Animation/hkaAnimationBinding.h>

// Scene Data
#include <Common/SceneData/Scene/hkxScene.h>
#include <Common/SceneData/Mesh/hkxMesh.h>
#include <Common/SceneData/Mesh/hkxMeshSection.h>
#include <Animation/Animation/Rig/hkaPose.h>

// Common animation Utilities
#include <Demos/DemoCommon/Utilities/Animation/AnimationUtils.h>

// Graphics Stuff
#include <Graphics/Common/Window/hkgWindow.h>
#include <Graphics/Common/Geometry/VertexSet/hkgVertexSet.h>
#include <Graphics/Bridge/SceneData/hkgSceneDataConverter.h>
#include <Graphics/Bridge/SceneData/hkgVertexBufferWrapper.h>

#include <Graphics/Bridge/System/hkgSystem.h>


SkinningDemo::SkinningDemo( hkDemoEnvironment* env )
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
		hkVector4 from(  -0.92f, -1.2f, 0.63f );
		hkVector4 to  ( 0.35f,-0.35f, 0.30f );
		hkVector4 up  ( 0.0f,  0.0f, 1.0f);
		setupDefaultCameras( env, from, to, up, 0.01f, 10000.0f );
	}

	m_loader = new hkLoader();

	// want to do software skinning always in this demo
	// see HardwareSkinningDemo for how to setup hardware palettes etc
	m_env->m_sceneConverter->setAllowHardwareSkinning(false);

	// Get the rig ( == skeleton )
	{
		hkString assetFile = hkAssetManagementUtil::getFilePath("Resources/Animation/HavokGirl/hkRig.hkx");
		hkRootLevelContainer* container = m_loader->load( assetFile.cString() );
		HK_ASSERT2(0x27343437, container != HK_NULL , "Could not load asset");
		hkaAnimationContainer* ac = reinterpret_cast<hkaAnimationContainer*>( container->findObjectByType( hkaAnimationContainerClass.getName() ));

		HK_ASSERT2(0x27343435, ac && (ac->m_numSkeletons > 0), "No skeleton loaded");
		m_skeleton = ac->m_skeletons[0];
	}

	// Get and convert the skin mesh to hkgDisplayObject
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

	// Get the animation and the animation binding
	{
		hkString assetFile = hkAssetManagementUtil::getFilePath("Resources/Animation/HavokGirl/hkIdle.hkx");
		hkRootLevelContainer* container = m_loader->load( assetFile.cString() );
		HK_ASSERT2(0x27343437, container != HK_NULL , "Could not load asset");
		hkaAnimationContainer* ac = reinterpret_cast<hkaAnimationContainer*>( container->findObjectByType( hkaAnimationContainerClass.getName() ));

		HK_ASSERT2(0x27343435, ac && (ac->m_numAnimations > 0), "No animation loaded");
		m_animation = ac->m_animations[0];

		HK_ASSERT2(0x27343435, ac && (ac->m_numBindings > 0), "No binding loaded");
		m_binding = ac->m_bindings[0];
	}

	// Create the animated skeleton with one animation control (using the animation loaded in)
	m_skeletonInstance = new hkaAnimatedSkeleton( m_skeleton );
	{
		hkaDefaultAnimationControl* control = new hkaDefaultAnimationControl( m_binding );

		control->setPlaybackSpeed(1.0f);
		m_skeletonInstance->addAnimationControl( control );
		control->removeReference();
	}

	// Get and convert the scene - this is just a ground grid plane and a camera
	{
		hkString assetFile = hkAssetManagementUtil::getFilePath("Resources/Animation/Scene/hkScene.hkx");
		hkRootLevelContainer* container = m_loader->load( assetFile.cString() );
		HK_ASSERT2(0x27343437, container != HK_NULL , "Could not load asset");
		hkxScene* scene = reinterpret_cast<hkxScene*>( container->findObjectByType( hkxSceneClass.getName() ));

		HK_ASSERT2(0x27343635, scene, "No scene loaded");
		removeLights(m_env); // assume we have some in the file
		env->m_sceneConverter->convert( scene );
	}

	// Init graphics
	setupGraphics();

}

SkinningDemo::~SkinningDemo()
{
	// Re-enable warnings									
	hkError::getInstance().setEnabled(0xf0d1e423, true);

	m_skeletonInstance->removeReference();
	delete m_loader;

	hkDefaultDemo::setupLights(m_env); // restore lights to default.
}

hkDemo::Result SkinningDemo::stepDemo()
{

// Advance the active animations
	m_skeletonInstance->stepDeltaTime( .016f );

	// Sample the active animations and combine into a single pose
	hkaPose pose (m_skeleton);
	m_skeletonInstance->sampleAndCombineAnimations( pose.accessUnsyncedPoseLocalSpace().begin(), pose.getFloatSlotValues().begin()  );

	// Create a deformer
#if (HK_CONFIG_SIMD == HK_CONFIG_SIMD_ENABLED)
	// If you outputs (pos, norm etc) are not aligned (which is common for some rendering engines)
	// then the simd deformer will be slower than it would if the outputs are aligned. It will still
	// be quicker than using the float only version though.
	hkaSimdSkinningDeformer skinner;
#else
	hkaFPUSkinningDeformer skinner;
#endif

	hkaVertexDeformerInput vdi;
	vdi.m_deformPosition = true;
	vdi.m_deformNormal = true;

	const hkArray<hkQsTransform>& poseInWorld = pose.getSyncedPoseModelSpace();

	const int numSkins = m_numSkinBindings;

	for (int i=0; i < numSkins; i++)
	{
		const hkxMesh* inputMesh = m_skinBindings[i]->m_mesh;
		// assumes either a straight map (null map) or a single one (1 palette)
		hkInt16* usedBones = m_skinBindings[i]->m_mappings? m_skinBindings[i]->m_mappings[0].m_mapping : HK_NULL;
		int numUsedBones = usedBones? m_skinBindings[i]->m_mappings[0].m_numMapping : m_skeleton->m_numBones;

		// Compute the skinning matrix palette
		hkLocalArray<hkTransform> compositeWorldInverse( numUsedBones );
		compositeWorldInverse.setSize( numUsedBones );

		// Multiply through by the bind pose inverse world inverse matrices
		for (int p=0; p < numUsedBones; p++)
		{
			int boneIndex = usedBones? usedBones[p] : p;
			compositeWorldInverse[p].setMul( poseInWorld[ boneIndex ], m_skinBindings[i]->m_boneFromSkinMeshTransforms[ boneIndex ] );
		}

		m_env->m_window->getContext()->lock();

		for (int s=0; s < inputMesh->m_numSections; ++s)
		{
			const hkxMeshSection& msc = *inputMesh->m_sections[s];
			hkxVertexBuffer* vIn = msc.m_vertexBuffer;

			// Find the vertex buffer for display that corresponds to this loaded vertex buffer
			const int ivOut = hkgAssetConverter::findFirstMapping( m_env->m_sceneConverter->m_vertexBuffers, vIn );

			if (ivOut >=0)
			{
				hkgReferencedObject* hkgObj = m_env->m_sceneConverter->m_vertexBuffers[ ivOut ].m_hkgObject;
				hkgVertexBufferWrapper* vOut = static_cast<hkgVertexBufferWrapper*> ( hkgObj );
				hkgVertexSet* graphicsBuffer = vOut->getUnderlyingVertexData();

				if (!vOut || !vOut->getUnderlyingVertexData()) 
					continue;

				vOut->lock();

				if ( hkgSystem::getRendererSubSystemAPI() != hkgSystem::HKG_RENDERER_SUBSYSTEM_NULL)
				{
					// Our hkxVertexFormat only describes interleaved formats
					// Internally on some platforms we use non interleaved formats
					if (graphicsBuffer->isInterleaved())
					{

						// Bind it to input and output meshes
						hkBool bound = skinner.bind(vdi, vIn, vOut);
						HK_ASSERT2(0x7af71fb2, bound, "Could not bind to skin..");

						// skin using the current pose
						skinner.deform( compositeWorldInverse.begin() );
					}
					else
					{
						// Instead of using the bind, we will setup the info ourselves and just deform
					#if (HK_CONFIG_SIMD == HK_CONFIG_SIMD_DISABLED)
						hkaFPUSkinningDeformer::hkaFloatBinding binding;
						AnimationUtils::setupNonInterleavedFloatBinding(&vdi, vIn, graphicsBuffer, &binding);
						hkaFPUSkinningDeformer::deform(compositeWorldInverse.begin(), binding);
					#else
						hkaSimdSkinningDeformer::hkaSimdBinding binding;
						AnimationUtils::setupNonInterleavedSimdBinding(&vdi, vIn, graphicsBuffer, &binding);
						hkaSimdSkinningDeformer::deform(compositeWorldInverse.begin(), binding);
					#endif
					}
				}

				vOut->unlock();
			}
		}

		m_env->m_window->getContext()->unlock();
	}

	return hkDemo::DEMO_OK;
}

#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

HK_DECLARE_DEMO(SkinningDemo, HK_DEMO_TYPE_ANIMATION | HK_DEMO_TYPE_SERIALIZE, "Skinning", "Play back a skinned mesh");

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
