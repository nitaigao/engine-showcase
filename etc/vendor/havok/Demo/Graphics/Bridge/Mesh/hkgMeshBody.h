/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HKG_MESH_BODY_H
#define HKG_MESH_BODY_H

#include <Common/GeometryUtilities/Mesh/hkMeshShape.h>
#include <Common/GeometryUtilities/Mesh/hkMeshBody.h>
#include <Common/GeometryUtilities/Mesh/IndexedTransformSet/hkIndexedTransformSet.h>

extern const hkClass hkgMeshBodyClass;
class hkgMeshShape;

class hkgMeshBody: public hkMeshBody
{
    public:
        HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_SCENE_DATA);

			/// Ctor
        hkgMeshBody( hkgDisplayContext* context, const hkgMeshShape* shape, const hkMatrix4& transform, hkIndexedTransformSetCinfo* transformSet);

            /// Dtor
        virtual ~hkgMeshBody();


            // hkMeshBody implementation
        virtual const hkMeshShape* getMeshShape() const;

            // hkMeshBody implementation
        virtual void getTransform( hkMatrix4& transform ) const;

            // hkMeshBody implementation
        virtual void setTransform(const hkMatrix4& matrix);

            // hkMeshBody implementation
        virtual hkResult setPickingData(int id, void* data);

            // hkMeshBody implementation
        virtual hkMeshVertexBuffer* getVertexBuffer(int sectionIndex);

            // hkMeshBody implementation
        virtual int getNumIndexedTransforms() { return m_transformSet ? m_transformSet->getNumMatrices() : 0; }

            // hkMeshBody implementation
        virtual void setIndexedTransforms(int startIndex, const hkMatrix4* matrices, int numMatrices);

            // hkMeshBody implementation
        virtual void getIndexedTransforms(int startIndex, hkMatrix4* matrices, int numMatrices) { HK_ASSERT(0x9fc68102, m_transformSet); m_transformSet->getMatrices(startIndex, matrices, numMatrices); }

            // hkMeshBody implementation
        virtual void getIndexedInverseTransforms(int startIndex, hkMatrix4* matrices, int numMatrices) { HK_ASSERT(0x9fc68101, m_transformSet); m_transformSet->getInverseMatrices(startIndex, matrices, numMatrices); }

            // hkMeshBody implementation
        virtual void completeUpdate();

			/// Calling this, will force completeUpdate() to  update everything
		void flagTransformsChanged();

            // hkReferencedObject Implementation
        virtual const hkClass* getClassType() const { return &hkgMeshBodyClass; }

            // hkReferencedObject Implementation
        virtual void calcContentStatistics( hkStatisticsCollector* collector,const hkClass* cls ) const;

	protected:

		hkgDisplayObject* _createDisplayObject( hkgDisplayContext* context, const hkgMeshShape* shape);
		
		void _updateSkin();

		//
		//	Members
		//
	public:

        hkArray<hkMeshVertexBuffer*> m_vertexBuffers;		///

        hkRefPtr<const hkgMeshShape> m_shape;				///
        hkgDisplayObject* m_displayObject;					///
        hkRefPtr<hkIndexedTransformSet> m_transformSet;		///

		hkgDisplayContext* m_context;						///

		hkBool m_transformChanged;							///
		hkBool m_transformSetChanged;						///

		hkMatrix4 m_transform;
};

#endif // HKG_MESH_BODY_H

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
