/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HKG_MESH_SYSTEM_H
#define HKG_MESH_SYSTEM_H

#include <Common/GeometryUtilities/Mesh/Memory/hkMemoryMeshSystem.h>

#include <Graphics/Common/Geometry/VertexSet/hkgVertexSetDefines.h>
#include <Graphics/Common/DisplayWorld/hkgDisplayWorld.h>


extern const hkClass hkgMeshSystemClass;

class hkgVertexSet;

/// An implementation of a hkMeshSystem that maps to the hkg graphics lib
///
/// This implementation does not create the index and vertex buffers in memory
/// but rather forwards all calls to hkg graphics objects. When requested,
/// the data is retrieved from the graphics system. If a mesh object is not handled
/// by hkg graphics, the parent memory mesh system object is used.
///
/// \sa hkMeshSystem

class hkgMeshSystem: public hkMemoryMeshSystem
{
    public:
        HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_SCENE_DATA);

			/// Ctor
		hkgMeshSystem(hkgDisplayWorld* dispWorld, hkgDisplayContext* context);

			/// Dtor
		virtual ~hkgMeshSystem();


			// hkMeshSystem implementation
        virtual hkMeshShape* createShape(const hkMeshSectionCinfo* sections, int numSections);

	        // hkMeshSystem implementation
        virtual hkMeshBody* createBody(const hkMeshShape* shape, const hkMatrix4& mat, hkIndexedTransformSetCinfo* transformSet);

			// hkMeshSystem implementation
        virtual hkMeshVertexBuffer* createVertexBuffer(const hkVertexFormat& vertexFormat, int numVertices);

			// hkMeshSystem implementation
		virtual void findSuitableVertexFormat(const hkVertexFormat& format, hkVertexFormat& formatOut);

			// hkMeshSystem implementation
        virtual hkMeshMaterial* createMaterial();

			// hkMeshSystem implementation
        virtual void addBody(hkMeshBody* body);

			// hkMeshSystem implementation
        virtual void removeBody(hkMeshBody* body);

			/// Get the display context that is used
 		HK_FORCE_INLINE hkgDisplayContext* getDisplayContext() const { return m_context; }

		//
		//	Internal section
		//


			// hkReferencedObjectImplementation
		virtual const hkClass* getClassType() const { return &hkgMeshSystemClass; }

			// hkReferencedObjectImplementation
		virtual void calcContentStatistics( hkStatisticsCollector* collector,const hkClass* cls ) const;

            // Given a registry will look for a material registered
        static hkMeshMaterial* HK_CALL findMaterial(hkMeshMaterialRegistry* registry, const hkgMaterial* material);

	public:

		virtual hkMeshVertexBuffer* createSkinnedVertexBuffer(const hkVertexFormat& vertexFormat, int numVertices);

			/// The hkgGraphics display world
        hkRefPtr<hkgDisplayWorld> m_displayWorld;

			/// The hkgGraphics display context
		hkRefPtr<hkgDisplayContext> m_context;
};

#endif // HKG_MESH_SYSTEM_H

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
