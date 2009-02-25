/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2008 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK_MESH_SYSTEM_H
#define HK_MESH_SYSTEM_H

class hkMeshShape;
class hkMeshBody;
class hkMeshMaterial;
class hkMeshMaterialRegistry;
class hkMeshVertexBuffer;
struct hkMeshSectionCinfo;
struct hkVertexFormat;
struct hkIndexedTransformSetCinfo;

/// hkIndexedTransformSetCinfo describes transforms needed by a hkMeshBody that has multiple transforms (over and
/// above the basic position transform)
///
/// A hkMeshBody needs to have indexed transforms, if all of its shapes' mesh sections have an index. Such a mesh shape
/// is constructed when the method createCompoundShape is called on hkMeshShape. Each of the meshes is given a transform
/// index, the first shape has index 0, the next 1 and so forth.
/// When a hkMeshBody is constructed from the mesh shape, the positions of the 'child' meshes can be controlled via
/// indexed transforms. This structure holds information about the child transforms.
///
/// The other situation where indexed transforms are used is when skinning is in operation. Skinning occurs when
/// the vertex buffer for a mesh section has skinning information defined on it.
/// The actual sequence of transforms performed of a vertex will be as follows (the displayTransform
/// is the main transform, set via setTransform on the hkMeshBody):
///
/// vertex -> inverse -> matrix -> displayTransform
///
/// or as column major matrix multiplication (Havok style)
///
/// vertexOut = displayTransform * matrix * inverse * vertexIn
///
/// Only the displayMatrix and the matrices can be modified on a per body basis - the inverses are immutable throughout
/// the life of the hkMeshBody.
///
/// The inverse transforms are there for skinning and are the 'bone space' transform. Setting the value to HK_NULL
/// will indicate that inverse transforms are not needed, and may provide a significant speed improvement.
///
/// \sa hkMeshBody

struct hkIndexedTransformSetCinfo
{
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_SCENE_DATA, hkIndexedTransformSetCinfo );

	hkIndexedTransformSetCinfo();

	hkMatrix4* m_inverseMatrices;               ///
	hkMatrix4* m_matrices;                      ///
	int m_numMatrices;                          ///
	hkBool m_allTransformsAreAffine;            ///
};

extern const hkClass hkMeshSystemClass;

/// The hkMeshSystem interface is designed to be a fairly low level interface to a rendering system.
///
/// The interfaces involved with the hkMeshSystem are
///
/// hkMeshMaterial      - A wrapper for a render specific implementation of a material
/// hkMeshVertexBuffer  - An interface to an array of vertices, whose style is described by an hkVertexFormat object.
/// hkMeshShape         - A set of one or more mesh sections - a mesh section defines a material, a vertex buffer and a number of render primitives.
/// hkMeshBody          - A renderable instantiation of a hkMeshShape. Holds the position of the shape in the world - may hold per instance vertex data.
/// hkMeshSystem        - Can construct all of the above, and holds the set of all of the hkMeshBodys which are being rendered.
///
/// Only objects created on the hkMeshSystem can be used to create other hkMeshSystem objects. For example behavior is
/// undefined if you create a vertex buffer on one instantiation of the hkMeshSystem interface, and try and use it on
/// another.
///
/// In the SDK there are currently two implementations of the hkMeshSystem interface - hkgMeshSystem and hkMemoryMeshSystem. The hkgMeshSystem
/// implements the hkMeshSystem for Havoks' platform independent 'hkg' graphics library. The hkMemoryMeshSystem is an implementation
/// which cannot perform any rendering, but holds all of the information about vertex buffers, materials and so forth in memory.
/// Thus it allows you to use all of the hkMeshSystem converters and utilities even in an environment with no graphics support.
/// For example you could use the hkMemoryMeshSystem implementation in an exporter to provide processing support.
/// Also depending on your target hkMeshSystem implementation, using the hkMemoryMeshSystem may be significantly faster
/// during processing, as it uses cached system memory - whereas a hardware implementation of a vertex buffer may return
/// memory which is slow.
///
/// An important point about hkMeshShape construction is that the hkMeshVertexBuffers that the hkMeshShape references, must already be filled in with
/// information. This is necessary - because an implementation may use that information to create a bounding volume around the object.
///
/// NOTE! That once a hkMeshShape has been constructed with a hkVertexBuffer it should be considered immutable. Altering a vertex buffer
/// once a hkMeshShape has been constructed using it produces undefined results, and is implementation specific. Not having the
/// restriction allows an implementation more opportunities to optimize vertex data, and for simpler implementation.
///
/// If there is a need for modifications of vertex data - this can be achieved by specifying the elements of the vertex buffer
/// as being not shared - and so they can be modified via the vertex buffer retrieved from the hkMeshBody. The only limitation
/// is that it is not possible to have all instantiations of bodies vertex buffers change by altering a single vertex buffer.
///
/// \sa hkMeshMaterial hkMeshVertexBuffer hkMeshShape hkMeshBody

class hkMeshSystem: public hkReferencedObject
{
	public:
        HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_SCENE_DATA);

			/// Get the material registry
		virtual hkMeshMaterialRegistry* getMaterialRegistry() const = 0;

            /// Create a mesh shape
            /// Returns HK_NULL if not successful.
        virtual hkMeshShape* createShape(const hkMeshSectionCinfo* sections, int numSections) = 0;

			/// Create a compound shape. The compound will produce a new shape, with each contained
			/// shape, having a transform index to allow control of each 'contained' shape individually.
			/// An implementation can change the structure of the object (for example producing a single shared vertex buffer), but
			/// graphically must look the same. Also note there is no way to find out the shapes used to construct the compound.
			/// It is possible to create a shape that has the same functionality as a shape created this way by using the m_transformIndex in the hkMeshSection.
            /// Returns HK_NULL if not successful.
		virtual hkMeshShape* createCompoundShape(const hkMeshShape*const* shapes, int numShapes) = 0;

			/// Create mesh body with the specified local position, and transform set
			/// The transform set is only needed for compound, or skinned usage. Passing in HK_NULL will disable skinning or per hkMeshSection transforms
            /// Returns HK_NULL if not successful.
        virtual hkMeshBody* createBody(const hkMeshShape* shape, const hkMatrix4& mat, hkIndexedTransformSetCinfo* transformSet) = 0;

            /// Create a vertex buffer. Returns HK_NULL if not successful.
        virtual hkMeshVertexBuffer* createVertexBuffer(const hkVertexFormat& vertexFormat, int numVertices) = 0;

            /// Returns a vertex format which is suitable for this mesh system, based on the format passed in.
        virtual void findSuitableVertexFormat(const hkVertexFormat& format, hkVertexFormat& formatOut) = 0;

            /// Returns an 'empty' material. Will be of the right type for the mesh system. In order to make the material
            /// appear correctly code for a specific engine implementation needs to set the internal state appropriately.
        virtual hkMeshMaterial* createMaterial() = 0;

			/// Add a body
		virtual void addBody(hkMeshBody* body) = 0;

			/// Remove a body
		virtual void removeBody(hkMeshBody* body) = 0;

            /// Called once per frame - required to perform any system wide per frame operations.
		virtual void update() = 0;
};

#endif // HK_MESH_SYSTEM_H

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
