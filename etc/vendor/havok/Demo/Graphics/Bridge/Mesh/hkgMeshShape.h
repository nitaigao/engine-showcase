/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HKG_MESH_SHAPE_H
#define HKG_MESH_SHAPE_H

#include <Common/GeometryUtilities/Mesh/hkMeshShape.h>

#include <Graphics/Common/DisplayObject/hkgDisplayObject.h>

class hkgMeshMaterial;
class hkgMeshVertexBuffer;

extern const hkClass hkgMeshShapeClass;

	/// Havoks mesh wrapper around hkg Graphics
class hkgMeshShape: public hkMeshShape
{
    public:

        HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_SCENE_DATA);

			/// Constructor with existing graphics lib display object.
		hkgMeshShape(hkgDisplayContext* system, hkgDisplayObject* dsp);

			/// Constructor with a number of \a sections. This will create a new graphics lib display object for the sections.
		hkgMeshShape(hkgDisplayContext* system, const hkMeshSectionCinfo* sections, int numSections);

			/// Dtor
		virtual ~hkgMeshShape();

            // hkMeshShape implementation
        virtual int getNumSections() const;

            // hkMeshShape implementation
        virtual void lockSection(int sectionIndex, hkUint8 accessFlags, hkMeshSection& sectionOut) const;

            // hkMeshShape implementation
        virtual void unlockSection(const hkMeshSection& section) const;

            /// Returns true if all vertex buffers are shared
        bool areAllVertexBuffersSharable() const;

		hkgMeshShape* clone(bool shareGeometry = true) const;

			// hkReferencedObject Implementation
        virtual const hkClass* getClassType() const { return &hkgMeshShapeClass; }

            // hkReferencedObject Implementation
        virtual void calcContentStatistics( hkStatisticsCollector* collector,const hkClass* cls ) const;

	private:

			// Do not construct without data.
		hkgMeshShape();

		//
		//	Members
		//
	public:

			/// Internal storage of a mesh section
        struct Section
        {
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_SCENE_DATA, hkgMeshShape::Section );
            hkRefPtr<hkgMeshMaterial>    m_material;		 ///
            hkRefPtr<hkMeshVertexBuffer> m_vertexBuffer;	 ///
			hkUint8                      m_dspGeom;          ///
			hkUint8                      m_dspGeomMfs;       ///
			hkUint8                      m_dspGeomMfsFs;     ///
		};

        hkgDisplayObject*		m_displayObject;			///
		hkgDisplayContext*		m_context;					///
        hkObjectArray<Section>	m_sections;                 ///
};

#endif // HKG_MESH_SHAPE_H

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
