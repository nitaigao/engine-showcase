/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2008 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_SCENE_DATA_TO_MESH_CONVERTER_H
#define HK_SCENE_DATA_TO_MESH_CONVERTER_H

#include <Common/Base/Math/Util/hkMathUtil.h>

#include <Common/GeometryUtilities/Mesh/hkMeshShape.h>
#include <Common/GeometryUtilities/Mesh/hkMeshVertexBuffer.h>

#include <Common/SceneData/Mesh/hkxVertexDescription.h>

class hkxScene;
class hkxNode;
class hkxMeshSection;
class hkxVertexBuffer;
class hkxMesh;
class hkMeshSystem;
class hkMeshMaterial;
class hkMeshSectionBuilder;

/// A converter from the Havok hkx formats into hkMeshShape

class hkSceneDataToMeshConverter
{
    public:
        HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DESTRUCTION, hkSceneDataToMeshConverter );

        //typedef hkMeshVertexBuffer::LockedVertices::Buffer Buffer;

            /// Converts a hkxNode into a hkxMeshShape
            /// The node has to be an hkxMeshNode, and the scene has to be the scene the node came from. The hkMeshShape will be constructed
            /// using the mesh system object passed in.
            /// The overrideMaterial parameter, if passed as HK_NULL the materials will be looked up on the material repository on the
            /// mesh system. If it is non-null the whole shape will use in the passed in material - overriding the materials specified in
            /// the hkxMeshNode
        static hkMeshShape* HK_CALL convert(hkMeshSystem* meshSystem, hkMeshMaterial* overrideMaterial, const class hkxScene* scene, const hkxNode* node);

			/// Converts a hkxMesh into a hkMeshShape - see previous convert method for details. 
		static hkMeshShape* HK_CALL convert(hkMeshSystem* meshSystem, hkMeshMaterial* overrideMaterial, const hkMatrix4& worldTransform, hkxMesh* mesh);

            /// Creates a vertex buffer from a hkxVertexBuffer. The decomposition parameter is used for baking in transform information -
            /// for example if the modeler had skew or scaling in its transform matrices.
        static hkMeshVertexBuffer* HK_CALL convertVertexBuffer(hkMeshSystem* meshSystem, hkMathUtil::Decomposition& decomposition, hkxVertexBuffer* srcVertex);

            /// Convert usage value to hkVertexFormat usage type
        static hkVertexFormat::DataUsage HK_CALL convertUsage(hkxVertexDescription::DataUsage usage);

            /// Convert a element desc to hkVertexFormat::Element
        static void HK_CALL convertToElement(const hkxVertexDescription::ElementDecl* decl, hkVertexFormat::Element& ele);

            /// Convert the indices specified in the section, and add them to the current mesh section in the
            /// mesh section builder.
        static hkResult HK_CALL convertIndices(const hkxMeshSection& section, hkMeshSectionBuilder& builder );
};

#endif // HK_SCENE_DATA_TO_MESH_CONVERTER_H

/*
* Havok SDK - NO SOURCE PC DOWNLOAD, BUILD(#20080925)
* 
* Confidential Information of Havok.  (C) Copyright 1999-2008
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
