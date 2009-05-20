/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HKG_MESH_CONVERTER_H
#define HKG_MESH_CONVERTER_H

class hkgMeshShape;
class hkMeshShape;
class hkMeshMaterial;
class hkgMeshSystem;
class hkMeshSystem;
class hkgSceneDataConverter;
class hkgDisplayContext;
class hkgDisplayObject;
class hkxScene;
class hkxNode;

class hkgMeshConverter
{
    public:
        HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_SCENE_DATA, hkgMeshConverter );

            /// Convert a hkx \a node from \a scene into a hkgMeshShape. The display objects are built by the \a converter.
        static hkgMeshShape* HK_CALL createHavokMeshShapeFromScene(hkgMeshSystem* system, hkMeshMaterial* overrideMaterial, hkgSceneDataConverter* converter, const hkxScene* scene, const hkxNode* node );

			/// Convert a hkg graphics \a displayObject that is made for \a displayContext into a hkMeshShape using the default mesh \a system
		static hkMeshShape* HK_CALL createMeshShapeFromDisplayObject(hkMeshSystem* system, hkMeshMaterial* overrideMaterial, hkgDisplayObject* displayObject, hkgDisplayContext* displayContext);
};

#endif // HKG_MESH_CONVERTER_H

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
