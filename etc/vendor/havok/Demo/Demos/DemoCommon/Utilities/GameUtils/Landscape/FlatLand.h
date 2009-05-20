/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


#ifndef HK_FLATLAND_H
#define HK_FLATLAND_H


class hkpMeshShape;
class hkpMoppCode;
class hkpMoppBvTreeShape;
class hkpShapeCollection;
class hkpExtendedMeshShape;

#include <Common/Base/hkBase.h>



	/// A simple landscape generator.
class FlatLand: public hkReferencedObject
{
	public:
		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO);

		FlatLand( int side = 50 );
		~FlatLand();

		virtual hkReal getHeight( int x, int y );
		virtual hkReal getBorderHeight();
		void setBorderHeight( hkReal borderHeight );

		void setScaling( const hkVector4& scaling );
		void setTriangleRadius( const hkReal s );

			/// enables faster but more memory-intense cached mopp building
		void enableCachedBuild( hkBool enable );

			// create "ground" MOPP shape from supplied mesh shape
		hkpMoppBvTreeShape* createMoppShape(hkpShapeCollection* meshShape);

			// This helper function facilitates construction of a "ground" shape on which to drive.
		hkpMoppBvTreeShape* createMoppShape();

		hkpExtendedMeshShape* createExtendedMeshShape();
		hkpMoppBvTreeShape* createMoppShapeForSpu();
		hkpMoppBvTreeShape* createMoppShapeForSpu(hkpShapeCollection* meshShape);


	protected:
		void setupVertexAndTriangleData();

		//
		//	Static data for the landscape
		//
		hkReal		m_triangleRadius;
		hkVector4   m_scaling;
		hkBool      m_cachedBuildEnabled;
		int m_side;
		int m_numVertices;
		int m_numTriangles;
		hkReal m_borderHeight;
		hkArray<hkReal> m_vertices;
		hkArray<hkUint16> m_indices;
};

#endif //HK_FLATLAND_H


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
