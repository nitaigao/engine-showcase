/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HKG_MESH_ASSET_UTIL_H
#define HKG_MESH_ASSET_UTIL_H

#include <Graphics/Common/Material/hkgMaterialManager.h>

class hkMeshSystem;
class hkgDisplayContext;
class hkxMesh;
class hkxScene;
class hkxNode;

class hkgMeshAssetUtil
{
    public:
        HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_SCENE_DATA, hkgMeshAssetUtil );

		static hkgMaterial* HK_CALL createBuiltIn(hkgDisplayContext* context, hkgSceneDataConverter* sceneConverter, HKG_TEXTURE_DEFAULTS builtIn);

        static hkgMaterial* HK_CALL createMaterialFromFile(hkgDisplayContext* context, hkgSceneDataConverter* sceneConverter, const char* fileName, hkBool normalMapped = false, hkBool specularMapped = false, hkBool displacementMapped = false);

		static hkResult HK_CALL addMaterial(hkMeshSystem* meshSystem, const char* materialName, hkgDisplayContext* context, hkgMaterial* material);

        static hkResult HK_CALL addMaterial(hkMeshSystem* meshSystem, const char* materialName, hkgDisplayContext* context, hkgSceneDataConverter* sceneConverter, const char* fileName, hkBool normalMapped = false, hkBool specularMapped = false, hkBool displacementMapped = false);

        static void HK_CALL findRootNodes(hkxScene* scene, hkArray<hkxNode*>& nodes);
};

#endif // HKG_MESH_ASSET_UTIL_H

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
