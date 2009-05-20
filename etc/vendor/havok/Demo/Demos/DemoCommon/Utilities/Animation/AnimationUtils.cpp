/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


#include <Demos/demos.h>
#include <Demos/DemoCommon/Utilities/Animation/AnimationUtils.h>
#include <Common/Base/Container/LocalArray/hkLocalArray.h>


// Vertex Deformation
#include <Common/Base/Config/hkConfigSimd.h>
#include <Animation/Animation/Deform/Skinning/Fpu/hkaFPUSkinningDeformer.h>
#include <Animation/Animation/Deform/Skinning/Simd/hkaSimdSkinningDeformer.h>

// Mesh info
#include <Common/SceneData/Mesh/hkxMesh.h>
#include <Common/SceneData/Mesh/hkxMeshSection.h>
#include <Common/SceneData/Mesh/hkxVertexBuffer.h>
#include <Common/SceneData/Mesh/hkxVertexUtil.h>

// Skeletal Animation
#include <Animation/Animation/hkaAnimationContainer.h>
#include <Animation/Animation/Animation/hkaAnimationBinding.h>
#include <Animation/Animation/Animation/hkaAnnotationTrack.h>
#include <Animation/Animation/Animation/hkaAnimation.h>
#include <Animation/Animation/Animation/Interleaved/hkaInterleavedUncompressedAnimation.h>
#include <Animation/Animation/Animation/DeltaCompressed/hkaDeltaCompressedAnimation.h>
#include <Animation/Animation/Animation/WaveletCompressed/hkaWaveletCompressedAnimation.h>
#include <Animation/Animation/Motion/hkaAnimatedReferenceFrame.h>
#include <Animation/Animation/Motion/Default/hkaDefaultAnimatedReferenceFrame.h>
#include <Animation/Animation/Rig/hkaBone.h>
#include <Animation/Animation/Rig/hkaSkeleton.h>
#include <Animation/Animation/Rig/hkaPose.h>
#include <Animation/Animation/Rig/hkaBoneAttachment.h>
#include <Animation/Animation/Rig/hkaSkeletonUtils.h>
#include <Animation/Animation/Playback/Control/Default/hkaDefaultAnimationControl.h>

// Deformation
#include <Animation/Animation/Deform/Skinning/hkaMeshBinding.h>

// Display
#include <Common/Visualize/hkDebugDisplay.h>
#include <Demos/DemoCommon/DemoFramework/hkTextDisplay.h>

// Graphics Stuff
#include <Graphics/Common/Math/hkgMath.h>
#include <Graphics/Common/DisplayObject/hkgDisplayObject.h>
#include <Graphics/Common/Geometry/VertexSet/hkgVertexSet.h>
#include <Graphics/Common/DisplayContext/hkgDisplayContext.h>
#include <Graphics/Bridge/SceneData/hkgSceneDataConverter.h>
#include <Graphics/Bridge/SceneData/hkgVertexBufferWrapper.h>

// Asset Management
#include <Demos/DemoCommon/Utilities/Asset/hkAssetManagementUtil.h>

// Serialization
#include <Common/Base/Reflection/hkClass.h>
#include <Common/Serialize/Util/hkLoader.h>
#include <Common/Serialize/Util/hkRootLevelContainer.h>

void HK_CALL AnimationUtils::drawWeightedVertices( const hkxVertexBuffer& buffer, int bone )
{
	const hkxVertexDescription& vdesc = *buffer.getVertexDesc();
	hkUint32 usageMask = vdesc.getMask();

	// If we have position, weights and indices
	if ((usageMask & hkxVertexDescription::HKX_DU_POSITION) &&
	    (usageMask & hkxVertexDescription::HKX_DU_BLENDWEIGHTS) &&
		(usageMask & hkxVertexDescription::HKX_DU_BLENDINDICES))
	{
		hkInt8* base = (hkInt8*)buffer.getVertexData();
		hkUint32 stride = vdesc.m_stride;

		int posOffset = vdesc.getElementByteOffset( hkxVertexDescription::HKX_DU_POSITION, 0 );
		int weightOffset = vdesc.getElementByteOffset( hkxVertexDescription::HKX_DU_BLENDWEIGHTS, 0 ); 
		int indicesOffset = vdesc.getElementByteOffset( hkxVertexDescription::HKX_DU_BLENDINDICES, 0 );

		int numVerts = buffer.getNumVertices();

		for( int vertIndex = 0; vertIndex < numVerts; ++vertIndex )
		{
			float* pos = reinterpret_cast<float*>(base + (stride * vertIndex) + posOffset);
			hkUint8* weights = reinterpret_cast<hkUint8*>(base + (stride * vertIndex) + weightOffset);
			hkUint8* indices = reinterpret_cast<hkUint8*>(base + (stride * vertIndex) + indicesOffset);

			hkReal col=0.0f;
			for (int i=0; i < 4; i++)
			{
				if (indices[i] == bone)
				{
					col += hkReal(weights[i])/255.0f;
				}
			}
			
			int color = hkColor::rgbFromFloats(col,col,col);
			hkVector4 position( pos[0], pos[1], pos[2] );
			HK_DISPLAY_STAR(position, 5.0f, color);
		}
	}
}

void HK_CALL AnimationUtils::drawVertices( const hkxVertexBuffer& buffer, int color )
{
	const hkxVertexDescription& vdesc = *buffer.getVertexDesc();
	hkUint32 usageMask = vdesc.getMask();

	// If we have position
	if (usageMask & hkxVertexDescription::HKX_DU_POSITION)
	{
		hkInt8* base = (hkInt8*)buffer.getVertexData();
		hkUint32 stride = vdesc.m_stride;

		int posOffset = vdesc.getElementByteOffset(hkxVertexDescription::HKX_DU_POSITION, 0);
		int numVerts = buffer.getNumVertices();
		
		for (hkInt32 i=0; i< numVerts; i++)
		{
			float* pos = reinterpret_cast<float*>(base + (stride * i) + posOffset);
			hkVector4 position( pos[0], pos[1], pos[2] );
			HK_DISPLAY_STAR(position, 5.0f, color);
		}
	}
}

void HK_CALL AnimationUtils::drawPoseModelSpace( const hkaSkeleton& skeleton, const hkQsTransform* poseModelSpace, const hkQsTransform& worldFromModel, int color, hkReal size, hkBool showRootTranslation )
{
	const hkInt16 numBones = static_cast<hkInt16> (skeleton.m_numBones);

	for( hkInt16 i = 0; i < numBones; i++ )
	{
		hkVector4 p1;

		p1.setTransformedPos(worldFromModel, poseModelSpace[i].getTranslation());

		hkReal boneLen = 1.0f;

		const hkInt16 parent = skeleton.m_parentIndices[i];

		// Display connections between child and parent
		if( parent == -1 )
		{
			if( showRootTranslation )
			{
				hkVector4 p2;
				p2.set(0,0,0);

				p2.setTransformedPos(worldFromModel, p2);
				HK_DISPLAY_LINE( p1, p2, color );
			}
		}
		else
		{
			hkVector4 p2;
			p2.setTransformedPos(worldFromModel, poseModelSpace[parent].getTranslation());
			HK_DISPLAY_LINE( p1, p2, color );

			hkVector4 bone; bone.setSub4(p1,p2);
			boneLen = bone.length3();
			boneLen = (boneLen > 10.0f) ? 10.0f : boneLen;
			boneLen = (boneLen < 0.1f) ? 0.1f : boneLen;
		}

		if (1)
		{
			const hkVector4 worldPosition = p1;
			hkQuaternion worldRotation; worldRotation.setMul(worldFromModel.getRotation(), poseModelSpace[i].getRotation());
			
			// Display local axis 
			{ 
				hkVector4 p2;
				p1 = worldPosition;
				p2.setRotatedDir( worldRotation, hkVector4( size * boneLen, 0, 0 ) );
				p2.add4( p1 );
				HK_DISPLAY_LINE( p1, p2, 0x7fff0000 );
				p1 = worldPosition;
				p2.setRotatedDir( worldRotation, hkVector4( 0, size * boneLen, 0 ) );
				p2.add4( p1 );
				HK_DISPLAY_LINE( p1, p2, 0x7f00ff00);
				p1 = worldPosition;
				p2.setRotatedDir( worldRotation, hkVector4( 0, 0, size * boneLen) );
				p2.add4( p1 );
				HK_DISPLAY_LINE( p1, p2, 0x7f0000ff );
			}
		}		
	}
}

void AnimationUtils::drawPose( const hkaPose& pose, const hkQsTransform& worldFromModel, int color, hkReal size, hkBool showOrigin )
{
	drawPoseModelSpace( *pose.getSkeleton(), pose.getSyncedPoseModelSpace().begin(), worldFromModel, color, size, showOrigin );
}



void HK_CALL AnimationUtils::drawSkeleton (const hkaSkeleton& skeleton, int color, hkReal size)
{
	hkaPose pose (&skeleton);

	pose.setToReferencePose();

	drawPose(pose, hkQsTransform::getIdentity(), color, size);
}



void HK_CALL AnimationUtils::setupNonInterleavedFloatBinding( const hkaVertexDeformerInput* input, const hkxVertexBuffer* iBuffer, hkgVertexSet* oBuffer, hkaFPUSkinningDeformer::hkaFloatBinding* binding)
{
	binding->m_iPosBase = input->m_deformPosition ? reinterpret_cast<const float*>( iBuffer->getVertexData() + iBuffer->getVertexDesc()->getElementByteOffset(hkxVertexDescription::HKX_DU_POSITION, 0) ) : HK_NULL;
	binding->m_iNormBase = input->m_deformNormal ? reinterpret_cast<const float*>( iBuffer->getVertexData() + iBuffer->getVertexDesc()->getElementByteOffset(hkxVertexDescription::HKX_DU_NORMAL, 0) ) : HK_NULL;
	binding->m_iBinormBase = input->m_deformBinormal ? reinterpret_cast<const float*>( iBuffer->getVertexData() + iBuffer->getVertexDesc()->getElementByteOffset(hkxVertexDescription::HKX_DU_BINORMAL, 0)) : HK_NULL;
	binding->m_iTangentBase = input->m_deformTangent ? reinterpret_cast<const float*>( iBuffer->getVertexData() + iBuffer->getVertexDesc()->getElementByteOffset(hkxVertexDescription::HKX_DU_TANGENT, 0) ) : HK_NULL;

	const hkxVertexDescription::ElementDecl* boneWeights = iBuffer->getVertexDesc()->getElementDecl( hkxVertexDescription::HKX_DU_BLENDWEIGHTS, 0 );
	const hkxVertexDescription::ElementDecl* boneIndices = iBuffer->getVertexDesc()->getElementDecl( hkxVertexDescription::HKX_DU_BLENDINDICES, 0 );
	binding->m_iWeightBase = boneWeights? reinterpret_cast<const hkUint8*>( iBuffer->getVertexData() + boneWeights->m_byteOffset) : HK_NULL;
	binding->m_iIndexBase = boneIndices? reinterpret_cast<const hkUint8*>( iBuffer->getVertexData() + boneIndices->m_byteOffset) : HK_NULL;

	int iStride = iBuffer->getVertexDesc()->m_stride;
	binding->m_iPosStride = (hkUint8)( iStride / sizeof(float) );
	binding->m_iNormStride = (hkUint8)( iStride / sizeof(float) );
	binding->m_iBinormStride = (hkUint8)( iStride / sizeof(float) );
	binding->m_iTangentStride = (hkUint8)( iStride  / sizeof(float) );
	binding->m_iWeightStride = (hkUint8)( iStride / sizeof(float) );
	binding->m_iIndexStride = (hkUint8)( iStride  / sizeof(float) );
	binding->m_bonesPerVertex = 4;

	HKG_VERTEX_FORMAT f = oBuffer->getVertexFormat();
	binding->m_oPosBase = (f & HKG_VERTEX_FORMAT_POS) && input->m_deformPosition ? (float*)oBuffer->getVertexComponentStartPointer(HKG_VERTEX_COMPONENT_POS) : HK_NULL;
	binding->m_oNormBase = (f & HKG_VERTEX_FORMAT_NORMAL) && input->m_deformNormal ? (float*)oBuffer->getVertexComponentStartPointer(HKG_VERTEX_COMPONENT_NORMAL) : HK_NULL;
	binding->m_oBinormBase = (f & HKG_VERTEX_FORMAT_BITANGENT) && input->m_deformBinormal ? (float*)oBuffer->getVertexComponentStartPointer(HKG_VERTEX_COMPONENT_BITANGENT) : HK_NULL;
	binding->m_oTangentBase = (f & HKG_VERTEX_FORMAT_TANGENT) && input->m_deformTangent ? (float*)oBuffer->getVertexComponentStartPointer(HKG_VERTEX_COMPONENT_TANGENT) : HK_NULL;
	binding->m_oPosStride = (hkUint8)( binding->m_oPosBase ? oBuffer->getVertexComponentStride(HKG_VERTEX_COMPONENT_POS) / sizeof(float) : HK_NULL );
	binding->m_oNormStride = (hkUint8)( binding->m_oNormBase ? oBuffer->getVertexComponentStride(HKG_VERTEX_COMPONENT_NORMAL) / sizeof(float) : HK_NULL );
	binding->m_oBinormStride = (hkUint8)( binding->m_oBinormBase ? oBuffer->getVertexComponentStride(HKG_VERTEX_COMPONENT_BITANGENT) / sizeof(float) : HK_NULL );
	binding->m_oTangentStride = (hkUint8)( binding->m_oTangentBase ? oBuffer->getVertexComponentStride(HKG_VERTEX_COMPONENT_TANGENT) / sizeof(float) : HK_NULL );

	binding->m_numVerts = iBuffer->getNumVertices();
}

void HK_CALL AnimationUtils::setupNonInterleavedSimdBinding( const hkaVertexDeformerInput* input, const hkxVertexBuffer* iBuffer, hkgVertexSet* oBuffer, hkaSimdSkinningDeformer::hkaSimdBinding* binding)
{
	binding->m_iPosBase = input->m_deformPosition ? reinterpret_cast<const hkVector4*>(iBuffer->getVertexData() + iBuffer->getVertexDesc()->getElementByteOffset(hkxVertexDescription::HKX_DU_POSITION, 0) ) : HK_NULL;
	binding->m_iNormBase = input->m_deformNormal ? reinterpret_cast<const hkVector4*>( iBuffer->getVertexData() + iBuffer->getVertexDesc()->getElementByteOffset(hkxVertexDescription::HKX_DU_NORMAL, 0)) : HK_NULL;
	binding->m_iBinormBase = input->m_deformBinormal ? reinterpret_cast<const hkVector4*>( iBuffer->getVertexData() + iBuffer->getVertexDesc()->getElementByteOffset(hkxVertexDescription::HKX_DU_BINORMAL, 0) ) : HK_NULL;
	binding->m_iTangentBase = input->m_deformTangent ? reinterpret_cast<const hkVector4*>( iBuffer->getVertexData() + iBuffer->getVertexDesc()->getElementByteOffset(hkxVertexDescription::HKX_DU_TANGENT, 0)) : HK_NULL;
	const hkxVertexDescription::ElementDecl* boneWeights = iBuffer->getVertexDesc()->getElementDecl( hkxVertexDescription::HKX_DU_BLENDWEIGHTS, 0 );
	const hkxVertexDescription::ElementDecl* boneIndices = iBuffer->getVertexDesc()->getElementDecl( hkxVertexDescription::HKX_DU_BLENDINDICES, 0 );
	binding->m_iWeightBase = boneWeights? reinterpret_cast<const hkUint8*>( iBuffer->getVertexData() + boneWeights->m_byteOffset) : HK_NULL;
	binding->m_iIndexBase = boneIndices? reinterpret_cast<const hkUint8*>( iBuffer->getVertexData() + boneIndices->m_byteOffset) : HK_NULL;

	int iStride = iBuffer->getVertexDesc()->m_stride;
	binding->m_iPosVectorStride = (hkUint8)( iStride / sizeof(hkVector4) );
	binding->m_iNormVectorStride = (hkUint8)( iStride / sizeof(hkVector4) );
	binding->m_iBinormVectorStride = (hkUint8)( iStride / sizeof(hkVector4) );
	binding->m_iTangentVectorStride = (hkUint8)( iStride / sizeof(hkVector4) );
	binding->m_iWeightByteStride = (hkUint8)( iStride );
	binding->m_iIndexByteStride = (hkUint8)( iStride );
	binding->m_bonesPerVertex = 4;

	HKG_VERTEX_FORMAT f = oBuffer->getVertexFormat();
	binding->m_oPosBase = (f & HKG_VERTEX_FORMAT_POS) && input->m_deformPosition ? (float*)oBuffer->getVertexComponentStartPointer(HKG_VERTEX_COMPONENT_POS) : HK_NULL;
	binding->m_oNormBase = (f & HKG_VERTEX_FORMAT_NORMAL) && input->m_deformNormal ? (float*)oBuffer->getVertexComponentStartPointer(HKG_VERTEX_COMPONENT_NORMAL) : HK_NULL;
	binding->m_oBinormBase = (f & HKG_VERTEX_FORMAT_BITANGENT) && input->m_deformBinormal ? (float*)oBuffer->getVertexComponentStartPointer(HKG_VERTEX_COMPONENT_BITANGENT) : HK_NULL;
	binding->m_oTangentBase = (f & HKG_VERTEX_FORMAT_TANGENT) && input->m_deformTangent ? (float*)oBuffer->getVertexComponentStartPointer(HKG_VERTEX_COMPONENT_TANGENT) : HK_NULL;
	binding->m_oPosFloatStride = (hkUint8)( binding->m_oPosBase ? oBuffer->getVertexComponentStride(HKG_VERTEX_COMPONENT_POS) / sizeof(float) : HK_NULL );
	binding->m_oNormFloatStride = (hkUint8)( binding->m_oNormBase ? oBuffer->getVertexComponentStride(HKG_VERTEX_COMPONENT_NORMAL) / sizeof(float) : HK_NULL );
	binding->m_oBinormFloatStride = (hkUint8)( binding->m_oBinormBase ? oBuffer->getVertexComponentStride(HKG_VERTEX_COMPONENT_BITANGENT) / sizeof(float) : HK_NULL );
	binding->m_oTangentFloatStride = (hkUint8)( binding->m_oTangentBase ? oBuffer->getVertexComponentStride(HKG_VERTEX_COMPONENT_TANGENT) / sizeof(float) : HK_NULL );

	// 16 byte aligned outputs or not? Input have to be aligned and are with our demos and scene data
	binding->m_outputAligned = binding->m_oPosBase? (hkxVertexUtil::getAlignment((hkUlong)binding->m_oPosBase) >= 0x10) && (binding->m_oNormFloatStride % 4 == 0) : true;
	binding->m_outputAligned = binding->m_outputAligned && ( binding->m_oNormBase? (hkxVertexUtil::getAlignment((hkUlong)binding->m_oNormBase) >= 0x10) && (binding->m_oNormFloatStride % 4 == 0) : true );
	binding->m_outputAligned = binding->m_outputAligned && ( binding->m_oBinormBase? (hkxVertexUtil::getAlignment((hkUlong)binding->m_oBinormBase) >= 0x10) && (binding->m_oBinormFloatStride % 4 == 0) : true );
	binding->m_outputAligned = binding->m_outputAligned && ( binding->m_oTangentBase? (hkxVertexUtil::getAlignment((hkUlong)binding->m_oTangentBase) >= 0x10) && (binding->m_oTangentFloatStride % 4 == 0) : true );

	binding->m_numVerts = iBuffer->getNumVertices();
}


void HK_CALL AnimationUtils::skinMesh(const class hkxMesh& inputMesh, const hkTransform& worldTransform, const class hkTransform* worldCompositeMatrices, class hkgSceneDataConverter& graphicsScene)
{

	// Find the display obejct that corresponds to this mesh
	const int displayObjIdx = hkgAssetConverter::findFirstMapping( graphicsScene.m_meshes, &inputMesh );
	if ( displayObjIdx >=0 )
	{
		// Ste the graphics transform
		hkgDisplayObject* dObj = (hkgDisplayObject*)graphicsScene.m_meshes[ displayObjIdx ].m_hkgObject;
		hkTransform t = worldTransform;
		t.getTranslation()(3) = 1.0f;	// hkTransform makes no guarantees about this entry, but display objects assume all 16 components are used!
		t.getColumn(0)(3) = 0.0f;
		t.getColumn(1)(3) = 0.0f;
		t.getColumn(2)(3) = 0.0f;
		dObj->setTransform( &t(0,0) );
	}

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

	graphicsScene.m_context->lock();

	// For each mesh section
	for (int s=0; s < inputMesh.m_numSections; ++s)
	{
		const hkxMeshSection& msc = *inputMesh.m_sections[s];

		hkxVertexBuffer* vIn = msc.m_vertexBuffer;

		// Find the vertex buffer for display that corresponds to this loaded vertex buffer
		const int ivOut = hkgAssetConverter::findFirstMapping( graphicsScene.m_vertexBuffers, vIn );

		if (ivOut >=0)
		{
			hkgReferencedObject* hkgObj = graphicsScene.m_vertexBuffers[ ivOut ].m_hkgObject;
			hkgVertexBufferWrapper* vOut = static_cast<hkgVertexBufferWrapper*> ( hkgObj );
			
			if (!vOut || !vOut->getVertexData()) 
				continue;

			hkgVertexSet* graphicsBuffer = vOut->getUnderlyingVertexData();

			vOut->lock(); // will be writeonly normally

			// Our hkxVertexFormat only describes interleaved formats
			// Internally on some platforms we use non interleaved formats
			if (graphicsBuffer->isInterleaved())
			{
				
				// Bind it to input and output meshes
				hkBool bound = skinner.bind(vdi, vIn, vOut);
				HK_ASSERT2(0x7af71fb2, bound, "Could not bind to skin..");

				// skin using the current pose
				skinner.deform( worldCompositeMatrices );
			}
			else
			{
				// Instead of using the bind, we will setup the info ourselves and just deform 
				#if (HK_CONFIG_SIMD == HK_CONFIG_SIMD_DISABLED)
					hkaFPUSkinningDeformer::hkaFloatBinding binding;
					setupNonInterleavedFloatBinding(&vdi, vIn, graphicsBuffer, &binding);
					hkaFPUSkinningDeformer::deform(worldCompositeMatrices, binding);
				#else
					hkaSimdSkinningDeformer::hkaSimdBinding binding;
					setupNonInterleavedSimdBinding(&vdi, vIn, graphicsBuffer, &binding);
					hkaSimdSkinningDeformer::deform(worldCompositeMatrices, binding);
				#endif
			}

			vOut->unlock();
		}
	}

	graphicsScene.m_context->unlock();
}

hkaAnimationBinding* AnimationUtils::loadAnimation( hkLoader& loader, const char* assetName )
{
	hkString assetFile = hkAssetManagementUtil::getFilePath(assetName);
	hkRootLevelContainer* container = loader.load( assetFile.cString() );
	HK_ASSERT2(0x27343437, container != HK_NULL , "Could not load asset");
	hkaAnimationContainer* ac = reinterpret_cast<hkaAnimationContainer*>( container->findObjectByType( hkaAnimationContainerClass.getName() ));
	HK_ASSERT2(0x27343435, ac && (ac->m_numAnimations > 0 ), "No animation loaded");
	HK_ASSERT2(0x27343435, ac && (ac->m_numBindings > 0), "No binding loaded");
	return ac->m_bindings[0];
}

hkaDefaultAnimationControl* AnimationUtils::loadControl( hkLoader& loader, const char* assetName )
{
	hkaAnimationBinding* binding = loadAnimation( loader, assetName );
	hkaDefaultAnimationControl* control = new hkaDefaultAnimationControl( binding );
	control->easeOut(0.0f);
	return control;
}

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
