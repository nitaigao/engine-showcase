/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/Animation/Api/MeshAndDeformation/HardwareSkinning/HardwareSkinningDemo.h>
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
#include <Animation/Animation/Rig/hkaPose.h>
#include <Animation/Animation/Playback/hkaAnimatedSkeleton.h>
#include <Animation/Animation/Playback/Control/Default/hkaDefaultAnimationControl.h>
#include <Animation/Animation/Animation/hkaAnimationBinding.h>

// Scene Data
#include <Common/SceneData/Scene/hkxScene.h>
#include <Common/SceneData/Mesh/hkxMesh.h>
#include <Common/SceneData/Mesh/hkxMeshSection.h>

// Common animation Utilities
#include <Demos/DemoCommon/Utilities/Animation/AnimationUtils.h>

// Graphics Stuff
#include <Graphics/Common/Window/hkgWindow.h>
#include <Graphics/Common/Shader/hkgShaderCollection.h>
#include <Graphics/Common/DisplayObject/hkgDisplayObject.h>
#include <Graphics/Common/Geometry/hkgGeometry.h>
#include <Graphics/Common/Geometry/hkgMaterialFaceSet.h>
#include <Graphics/Bridge/SceneData/hkgSceneDataConverter.h>
#include <Graphics/Bridge/SceneData/hkgVertexBufferWrapper.h>
#include <Graphics/Bridge/System/hkgSystem.h> // to figure put if we should hardware skin


HardwareSkinningDemo::HardwareSkinningDemo( hkDemoEnvironment* env )
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

	// We want to use this demo to do the skinning, so we don't let our graphics engine do it.

	m_canSkin = (hkgSystem::g_RendererType != hkgSystem::HKG_RENDERER_NULL); 
	if (hkgSystem::g_RendererType != hkgSystem::HKG_RENDERER_CONSOLE)
	{
		m_canSkin = m_env->m_window->shaderSupportGreaterOrEqualTo(1); // has shaders..
	}
	env->m_sceneConverter->setAllowHardwareSkinning(m_canSkin);

	// Convert the scene
	{
		hkString assetFile = hkAssetManagementUtil::getFilePath("Resources/Animation/Scene/hkScene.hkx");
		hkRootLevelContainer* container = m_loader->load( assetFile.cString() );
		HK_ASSERT2(0x27343437, container != HK_NULL , "Could not load asset");
		hkxScene* scene = reinterpret_cast<hkxScene*>( container->findObjectByType( hkxSceneClass.getName() ));

		HK_ASSERT2(0x27343635, scene, "No scene loaded");
		removeLights(m_env); // assume we have some in the file
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
		hkString assetFile = hkAssetManagementUtil::getFilePath("Resources/Animation/HavokGirl/hkIdle.hkx");
		hkRootLevelContainer* container = m_loader->load( assetFile.cString() );
		HK_ASSERT2(0x27343437, container != HK_NULL , "Could not load asset");
		hkaAnimationContainer* ac = reinterpret_cast<hkaAnimationContainer*>( container->findObjectByType( hkaAnimationContainerClass.getName() ));

		HK_ASSERT2(0x27343435, ac && (ac->m_numAnimations > 0), "No animation loaded");
		m_animation = ac->m_animations[0];

		HK_ASSERT2(0x27343435, ac && (ac->m_numBindings > 0), "No binding loaded");
		m_binding = ac->m_bindings[0];
	}

	// Convert the skin
	{

		const char* skinFile = "Resources/Animation/HavokGirl/hkLowResSkin18Bones.hkx";

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

		// If we are hardware skinning, we need to inform the graphics of the
		// mappings between the skeleton bones and the primitive subsets with
		// possibly limited palettes. Looks worse than it is as our scene converter
		// has no dependancy on the animation system, so we need to cast a bit.
		if (m_canSkin)
		{

			
			hkgShaderCollection* platformShader; 
			if (m_env->m_window->supportsShaderCompilation())
			{
				platformShader = compileShader();
			}
			else
			{
				platformShader = loadPrecompiledShader();
			}	

			for (int ms=0; ms < m_numSkinBindings; ++ms)
			{
				hkaMeshBinding* skinBinding = m_skinBindings[ms];
				if ( m_env->m_sceneConverter->setupHardwareSkin( m_env->m_window->getContext(), skinBinding->m_mesh,
						reinterpret_cast<hkgAssetConverter::IndexMapping*>( skinBinding->m_mappings ),
						skinBinding->m_numMappings, (hkInt16)skinBinding->m_skeleton->m_numBones ) )
				{
					// based on the hkxMesh will find the hkg object and set the shader in the material(s)
					setShader( skinBinding->m_mesh, platformShader );
				}
			}
			platformShader->release();


		}
	}

	// Create the skeleton
	m_skeletonInstance = new hkaAnimatedSkeleton( m_skeleton );
	{
		hkaDefaultAnimationControl* ac = new hkaDefaultAnimationControl( m_binding );

		ac->setPlaybackSpeed(1.0f);
		m_skeletonInstance->addAnimationControl( ac );
		ac->removeReference();
	}

	// set up the vdb
	setupGraphics();

}

HardwareSkinningDemo::~HardwareSkinningDemo()
{
	// Re-enable warnings									
	hkError::getInstance().setEnabled(0xf0d1e423, true);

	m_skeletonInstance->removeReference();
	delete m_loader;

	hkDefaultDemo::setupLights(m_env); // restore lights to default.

	// want to do software skinning by default in the demos
	m_env->m_sceneConverter->setAllowHardwareSkinning(false);
}

hkDemo::Result HardwareSkinningDemo::stepDemo()
{
	// Advance the active animations
	if (m_canSkin)
	{
		m_skeletonInstance->stepDeltaTime( .016f );

		hkaPose pose (m_skeleton);

		// Sample the active animations and combine into a single pose
		m_skeletonInstance->sampleAndCombineAnimations( pose.accessUnsyncedPoseLocalSpace().begin(), pose.getFloatSlotValues().begin()  );

		// Grab the pose in model space
		const hkArray<hkQsTransform>& poseModelSpace = pose.getSyncedPoseModelSpace();

		const int numSkins = m_numSkinBindings;
		for (int i=0; i < numSkins; i++)
		{
			const hkxMesh* inputMesh = m_skinBindings[i]->m_mesh;
			int numBones = m_skinBindings[i]->m_numBoneFromSkinMeshTransforms;

			// Compute the skinning matrix palette
			hkLocalArray<hkTransform> compositeWorldInverse( numBones );
			compositeWorldInverse.setSize( numBones );

			// Multiply through by the bind pose inverse world inverse matrices, according to the skel to mesh bone mapping
			for (int p=0; p < numBones; p++)
			{
				compositeWorldInverse[p].setMul( poseModelSpace[ p ], m_skinBindings[i]->m_boneFromSkinMeshTransforms[ p ] );
			}

			// reflect the matrices in the palette used by the shader
			m_env->m_sceneConverter->updateSkin( inputMesh, compositeWorldInverse, hkTransform::getIdentity() );
		}
	}

	return hkDemo::DEMO_OK;
}

hkgShaderCollection* HardwareSkinningDemo::compileShader()
{
	hkgShaderCollection* ret  = HK_NULL;

	hkgDisplayContext* ctx = m_env->m_window->getContext();
	hkgShader* vertexShader = hkgShader::createVertexShader( ctx );
	hkgShader* pixelShader = hkgShader::createPixelShader( ctx );

	hkString shaderFile("./Resources/Animation/Shaders/SimpleSkinningShader"); 
	shaderFile += hkString(vertexShader->getDefaultFileNameExtension());

	vertexShader->realizeCompileFromFile( shaderFile.cString(), "mainVS", HK_NULL, HK_NULL, HK_NULL);
	pixelShader->realizeCompileFromFile( shaderFile.cString(), "mainPS", HK_NULL, HK_NULL, HK_NULL);

	ret = hkgShaderCollection::create();
	ret->addShaderGrouping(vertexShader, pixelShader);	

	pixelShader->removeReference();
	vertexShader->removeReference();

	return ret;
}

hkgShaderCollection* HardwareSkinningDemo::loadPrecompiledShader()
{

	const char* filePostfix = hkgSystem::g_RendererType == hkgSystem::HKG_RENDERER_DX8? "_vs11_dx81" : "_vs20_dx9";
	const char* filePrefix = ".\\";


	hkString shaderFile;
	hkString pshaderFile;
	hkString resources("resources\\animation\\shaders\\");
	hkString filename;
	filename = hkString(filePrefix) + resources + "Skinning" + filePostfix;
	shaderFile = filename + ".vso";
	pshaderFile = filename + ".pso";

	hkIstream shaderFileStream( shaderFile.cString() );
	HK_ASSERT(0x0, shaderFileStream.isOk());
	HKG_SHADER_RENDER_STYLE style = HKG_SHADER_RENDER_1LIGHTS;  // one spot light
	hkgShader* shader = hkgShader::create(HKG_VERTEX_SHADER, m_env->m_window->getContext());
	shader->setShaderFileName( shaderFile.cString() );
	shader->realize(shaderFileStream, style);

	// add a pixel shader if require (just always use one..)
	hkgShader* pshader = HK_NULL;
    if (hkgSystem::g_RendererType == hkgSystem::HKG_RENDERER_DX9S)
	{
		// need a precompiled pixel shader..
		HK_ASSERT(0x0, "Demo does not make a precompiled pixel shader for d3d9s");
	}


	/*	// Vertex Shader
	//   -------------- ----- ----
	//   mWorldMatrixArray c0      78
	//   mViewProj         c78      4
	//   lhtDir            c82      1
	//   MaterialAmbient   c83      1
	//   MaterialDiffuse   c84      1
	*/

	// table of 26 matrices first
	for (int i=0; i < 26; ++i)
		shader->addMatInput(HKG_SHADER_MAT_INPUT_4x4, 12);

	// add the rest of the actually auto set constants:
	shader->addMatInput(HKG_SHADER_MAT_INPUT_VP, 16);
	shader->addNormalInput(HKG_SHADER_INPUT_LIGHT_DIR, 4);
	shader->addNormalInput(HKG_SHADER_INPUT_AMBIENT_COLOR, 4);
	shader->addNormalInput(HKG_SHADER_INPUT_DIFFUSE_COLOR, 4);


	/*	// Pixel Shader
	//   -------------- ----- ----
	// just the texture in s0
	*/

	// Group the set of shaders together
	hkgShaderCollection* effect = hkgShaderCollection::create();
	effect->addShaderGrouping(shader, pshader);
	if (shader) shader->release();
	if (pshader) pshader->release();

	return effect;
}

void HardwareSkinningDemo::setShader( const hkxMesh* refMesh, hkgShaderCollection* shader)
{
	hkArray<int> dispObjsIndices;
	hkgAssetConverter::findAllMappings(m_env->m_sceneConverter->m_meshes, refMesh, dispObjsIndices);
	for (int di=0; di < dispObjsIndices.getSize(); ++di)
	{
		hkgDisplayObject* dispObj = (hkgDisplayObject*)( m_env->m_sceneConverter->m_meshes[ dispObjsIndices[di] ].m_hkgObject );
		for (int gi=0; gi < dispObj->getNumGeometry(); ++gi)
		{
			hkgGeometry* geom = dispObj->getGeometry(gi);
			for (int mfsi=0; mfsi < geom->getNumMaterialFaceSets(); ++mfsi)
			{
				hkgMaterialFaceSet* mfs = geom->getMaterialFaceSet(mfsi);
				hkgMaterial* m = mfs->getMaterial();
				if (m && !m->isDefaultMaterial() )
				{
					m->setShaderCollection(shader);
				}
			}
		}
	}
}

#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

HK_DECLARE_DEMO(HardwareSkinningDemo, HK_DEMO_TYPE_ANIMATION | HK_DEMO_TYPE_SERIALIZE, "HardwareSkinning", "Play back a skinned mesh in available hardware.");

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
