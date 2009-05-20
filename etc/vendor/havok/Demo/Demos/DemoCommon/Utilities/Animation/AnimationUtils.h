/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef ANIMATION_UTILS_H
#define ANIMATION_UTILS_H

#include <Animation/Animation/Deform/Skinning/Fpu/hkaFPUSkinningDeformer.h>
#include <Animation/Animation/Deform/Skinning/Simd/hkaSimdSkinningDeformer.h>

class hkaPose;
class hkaSkeleton;
class hkaDefaultAnimationControl;
class hkLoader;
class hkaAnimationBinding;

class AnimationUtils
{
	public:
		
		/// Skinning utility. N.B. 4th row of worldTransform is not used (assumed == (0,0,0,1))
		static void HK_CALL skinMesh(const class hkxMesh& inputMesh, const class hkTransform& worldTransform, const class hkTransform* worldCompositeMatrices, class hkgSceneDataConverter& graphicsScene);

		/// Draw utils
		static void HK_CALL drawPose( const hkaPose& pose, const hkQsTransform& worldFromModel, int color=0x7fffffff, hkReal size = 0.25f, hkBool showRootTranslation = false );
		static void HK_CALL drawPoseModelSpace( const hkaSkeleton& skeleton, const hkQsTransform* poseModelSpace, const hkQsTransform& worldFromModel, int color=0x7fffffff, hkReal size = 0.25f, hkBool showRootTranslation = false );

		/// Draw utils : draws a full skeleton in its initial pose
		static void HK_CALL drawSkeleton (const class hkaSkeleton& skeleton, int color=0x7fffffff , hkReal size = 0.25f);

		static void HK_CALL drawVertices(const class hkxVertexBuffer& vertexBuffer, int color);

		static void HK_CALL drawWeightedVertices(const class hkxVertexBuffer& vertexBuffer, int boneHighlight = 0 );

		static void HK_CALL setupNonInterleavedFloatBinding( const hkaVertexDeformerInput* input, const class hkxVertexBuffer* iBuffer, class hkgVertexSet* oBuffer, hkaFPUSkinningDeformer::hkaFloatBinding* binding);
		static void HK_CALL setupNonInterleavedSimdBinding( const hkaVertexDeformerInput* input, const class hkxVertexBuffer* iBuffer, class hkgVertexSet* oBuffer, hkaSimdSkinningDeformer::hkaSimdBinding* binding);

		static hkaDefaultAnimationControl* HK_CALL loadControl( hkLoader& loader, const char* assetName );
		static hkaAnimationBinding* HK_CALL loadAnimation( hkLoader& loader, const char* assetName );
};


#endif // ANIMATION_UTILS_H

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
