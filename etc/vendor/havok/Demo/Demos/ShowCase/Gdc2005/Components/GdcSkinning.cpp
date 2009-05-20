/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/ShowCase/Gdc2005/Gdc2005Demo.h>

// Debug graphics
#include <Common/Visualize/hkDebugDisplay.h>

#include <Animation/Animation/Playback/hkaAnimatedSkeleton.h>
#include <Animation/Animation/Rig/hkaSkeleton.h>
#include <Animation/Animation/Rig/hkaSkeletonUtils.h>
#include <Animation/Animation/Rig/hkaPose.h>

// Vertex Deformation
#include <Animation/Animation/Deform/Skinning/hkaMeshBinding.h>

// Common animation Utilities
#include <Demos/DemoCommon/Utilities/Animation/AnimationUtils.h>


#include <Common/Base/Monitor/hkMonitorStream.h>

/*
** SKINNING Component
**
*/

// doSkinning() just renders (skins) the given pose
void Gdc2005Demo::doSkinning (const hkaPose& thePose)
{
	const int boneCount = m_animatedSkeleton->getSkeleton()->m_numBones;

	// Construct the composite world transform
	hkLocalArray<hkTransform> compositeWorldInverse( boneCount );
	compositeWorldInverse.setSize( boneCount );

	const hkArray<hkQsTransform>& poseMS = thePose.getSyncedPoseModelSpace();

	// Skin the meshes
	for (int i=0; i < m_numSkinBindings; i++)
	{
		const hkxMesh* inputMesh = m_skinBindings[i]->m_mesh;

		hkTransform graphicsTransform;
		m_currentTransform.copyToTransform(graphicsTransform);

		if (m_bUseHardwareSkinning)
		{
			int numBones = m_skinBindings[i]->m_numBoneFromSkinMeshTransforms;

			// Multiply through by the bind pose inverse world inverse matrices, according to the skel to mesh bone mapping
			for (int p=0; p < numBones; p++)
			{
				hkTransform tWorld; poseMS[ p ].copyToTransform(tWorld);
				compositeWorldInverse[p].setMul( tWorld, m_skinBindings[i]->m_boneFromSkinMeshTransforms[ p ] );
				if (m_bUseWorldHardwareSkinning) // DX s/w mode
				{
					tWorld.setMul( graphicsTransform, compositeWorldInverse[p] );
					compositeWorldInverse[p] = tWorld;
				}
			}

			// get a rough aabb to approx where the hardware skinning is going to skin the mesh to
			hkgAabb aabb;

			hkTransform tHack; poseMS[2].copyToTransform(tHack);
			hkTransform tHack2; m_currentTransform.copyToTransform(tHack2);
			hkTransform tHack3;
			tHack3.setMul(tHack2, tHack);
			hkgVec3Set(aabb.m_min, -1,-1,-1);
			hkgVec3Set(aabb.m_max,  1, 1, 1);
			if (m_bUseWorldHardwareSkinning)
			{
				aabb.transform((float*)&tHack3);
			}

			hkAabb hkaabb; hkaabb.m_min.set(aabb.m_min[0],aabb.m_min[1],aabb.m_min[2]);
			hkaabb.m_max.set(aabb.m_max[0],aabb.m_max[1],aabb.m_max[2]);

			if (m_options.m_Display.m_skinning)
				m_env->m_sceneConverter->updateSkin( inputMesh, compositeWorldInverse, m_bUseWorldHardwareSkinning? hkTransform::getIdentity() : graphicsTransform, &hkaabb  );
		}
		else
		{

			// assumes either a straight map (null map) or a single one (1 palette)
			hkInt16* usedBones = m_skinBindings[i]->m_mappings? m_skinBindings[i]->m_mappings[0].m_mapping : HK_NULL;
			int numUsedBones = usedBones? m_skinBindings[i]->m_mappings[0].m_numMapping : boneCount;

			// Multiply through by the bind pose inverse world inverse matrices
			for (int p=0; p < numUsedBones; p++)
			{
				int boneIndex = usedBones? usedBones[p] : p;
				hkTransform tWorld; poseMS[ boneIndex ].copyToTransform(tWorld);
				compositeWorldInverse[p].setMul( tWorld, m_skinBindings[i]->m_boneFromSkinMeshTransforms[ boneIndex ] );
			}

			// use FPU skinning
			if (m_options.m_Display.m_skinning)
				AnimationUtils::skinMesh( *inputMesh, graphicsTransform, compositeWorldInverse.begin(), *m_env->m_sceneConverter );
		}
	}

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
