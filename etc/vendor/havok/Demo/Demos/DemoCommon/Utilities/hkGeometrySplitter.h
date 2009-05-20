/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#include <Common/Base/hkBase.h>


struct hkSplitGeometryInput
{
	//
	// Mesh
	//

	unsigned char* m_vertices;
	int m_numVertices;
	int m_stride;

	struct IndexBuf
	{
		hkUint16* m_indices;
		int m_numIndices;

		hkUint16 m_startOffset;
		hkUint16 m_triangleStride;
	};

	hkArray<IndexBuf> m_indexBufs;

	//
	// Plane to cut mesh with
	//
	hkVector4 m_plane;

};

struct hkSplitGeometryOutput
{
	// Vertices from the original geometry in the new geometry
	hkArray<hkUint16> m_reusedVertices;

	// Vertices created 
	struct CutVertex
	{
		hkInt16 m_origVertA; 
		hkInt16 m_origVertB;
		hkReal m_weight;
	};
	hkArray<CutVertex> m_cutVertices;

	// Vertices are arranged in a triangle fan around the centre of the new
	// surface
	//hkVector4 m_centreVertex;

	// Triangle Lists for existing surfaces
	// m_outsideFaceIndices index as follows:
	// reusedVertices
	// cutVertices
	hkArray<hkUint16> m_outsideFaceIndices;

	// Triangle Lists for new surface now exposed
	// m_insideFaceIndices index as follows:
	// centreVertex (0) = centroid of m_cutVertices
	// cutVertices
	hkArray<hkUint16> m_insideFaceIndices;
};

struct hkTempSplitGeom : public hkReferencedObject
{
	struct Triangle
	{
		int m_a;
		int m_b;
		int m_c;

		void set( int a, int b, int c )
		{ 
			m_a = a; 
			m_b = b; 
			m_c = c; 
		}
	};


	hkArray<hkVector4> m_vertices;

	struct VertData
	{
		hkVector4 m_normal;
		hkReal m_u;
		hkReal m_v;
	};

	hkArray<VertData> m_vertDatas;

	/// Array of vertices that the triangles can index into.
	//hkArray<hkVector4> m_vertices;
	//hkArray<hkReal> m_u;
	//hkArray<hkReal> m_v;
	//hkArray<hkVector4> m_normals;


	/// Array of triangles.  The triangles are triples of ints that are indices into the m_vertices array.
	hkArray<hkUint16> m_indices;


};

class hkgDisplayObject;

extern "C"
{
	void splitGeometryWithPlane( hkSplitGeometryInput& input, hkSplitGeometryOutput& output );

	hkgDisplayObject* createSplitDisplayObject( hkgDisplayObject* displayObjectIn, hkgDisplayContext* context, const hkVector4& plane );

	// OR:

	// Create the temp rep
	hkTempSplitGeom* createTempFromDisplayGeom( hkgDisplayObject* origDisplayObject );

	// Recursively split
	hkTempSplitGeom* createTempSplitDisplayObject( hkTempSplitGeom* displayObjectIn, const hkVector4& plane );

	// Create the final display object
	hkgDisplayObject* createDisplayGeomFromTemp( hkgDisplayObject* origDisplayObject, hkTempSplitGeom* tempGeom, hkgDisplayContext* context );


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
