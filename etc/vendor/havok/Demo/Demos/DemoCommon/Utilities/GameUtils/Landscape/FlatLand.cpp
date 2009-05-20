/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/DemoCommon/Utilities/GameUtils/Landscape/FlatLand.h>
#include <Common/Base/System/Io/IStream/hkIStream.h>

// We will need these shapes
#include <Physics/Collide/Shape/Compound/Tree/Mopp/hkpMoppBvTreeShape.h>
#include <Physics/Collide/Shape/Compound/Collection/ExtendedMeshShape/hkpExtendedMeshShape.h>

#include <Physics/Collide/Shape/Compound/Tree/Mopp/hkpMoppUtility.h>
//#include <hkinternal/collide/mopp/machine/hkpMoppStatisticsVirtualMachine.h>
#include <Common/Base/Types/Geometry/Aabb/hkAabb.h>

#include <Common/Serialize/Packfile/Binary/hkBinaryPackfileWriter.h>
#include <Common/Serialize/Packfile/Binary/hkBinaryPackfileReader.h>
#include <Common/Serialize/Util/hkRootLevelContainer.h>
#include <Physics/Utilities/Serialize/hkpHavokSnapshot.h>

#include <Demos/DemoCommon/Utilities/GameUtils/Landscape/LandscapeRepository.h>



FlatLand::FlatLand(int side)
{
	m_side = side;
	m_numVertices = m_side * m_side;
	m_numTriangles = 2 * (m_side - 1) * (m_side -1 );

	m_cachedBuildEnabled = false;

	m_vertices.setSize(m_numVertices * 4);
	m_indices.setSize(m_numTriangles * 4);
	m_scaling.set( 20.0f, 1.0f, 20.0f );
	m_triangleRadius = 0.01f;
	m_borderHeight = 3.f;

}

FlatLand::~FlatLand()
{
}


hkReal FlatLand::getHeight( int i, int j )
{
	hkReal h = (hkMath::cos(0.2f*((hkReal)j + i)) + 0.5f * hkMath::sin( 0.2f*2.0f * i)) * 1.0f;

	h = h * h * h * h *h * h - 6.0f;
	return h;
}
	

hkReal FlatLand::getBorderHeight()
{
	return m_borderHeight;
}


void FlatLand::setBorderHeight( hkReal borderHeight )
{
	m_borderHeight = borderHeight;
}


void FlatLand::setScaling( const hkVector4& scaling )
{
	m_scaling = scaling;
}

void FlatLand::setTriangleRadius( const hkReal s )
{
	m_triangleRadius = s;
}

void FlatLand::enableCachedBuild( hkBool enable )
{
	m_cachedBuildEnabled = enable;
}

void FlatLand::setupVertexAndTriangleData()
{
	//
	// Create the vertices (a "heightfield" of triangles)
	//
	{
		const float borderHeight = getBorderHeight();
		{
			for(int i = 0; i < m_side; i++)
			{
				for (int j = 0; j < m_side; j++ )
				{ 

					float h = getHeight( i, j );

					m_vertices[(i * m_side + j) * 4  + 0] = i * 1.0f - m_side * 0.5f;
					m_vertices[(i * m_side + j) * 4  + 1] = h;
					m_vertices[(i * m_side + j) * 4  + 2] = j * 1.0f - m_side * 0.5f;
					m_vertices[(i * m_side + j) * 4  + 3] = 0.0f;	// we need to set this to avoid denormals on P4
				}
			}
		}
		//
		//	Add a border
		//
		{
			for(int i = 0; i < m_side; i++)
			{
				m_vertices[ (          i * m_side + 0     ) * 4  + 1] = borderHeight;
				m_vertices[ (          i * m_side + m_side-1) * 4  + 1] = borderHeight;
				m_vertices[ ( (m_side-1) * m_side + i       ) * 4  + 1] = borderHeight;
				m_vertices[ (i                ) * 4  + 1] = borderHeight;
			}
		}
	}

	//
	// Create the triangles
	//
	{
		hkUint16 corner = 0;
		int index = 0;
		for(int i = 0; i < m_side - 1; i++)
		{
			for (int j = 0; j < m_side - 1; j++ )
			{
				m_indices[index]     = hkUint16(corner);
				m_indices[index + 1] = hkUint16(corner + 1);
				m_indices[index + 2] = hkUint16(corner + m_side);
				// m_indices[index + 3] Dummy

				m_indices[index + 4] = hkUint16(corner + 1);
				m_indices[index + 5] = hkUint16(corner + m_side + 1);
				m_indices[index + 6] = hkUint16(corner + m_side);
				// m_indices[index + 7] Dummy

				index += 8;
				corner++; 
			}
			corner++; 
		}
	}
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


hkpExtendedMeshShape* FlatLand::createExtendedMeshShape()
{
	setupVertexAndTriangleData();

	{
		for (int vertexIdx = 0; vertexIdx < m_vertices.getSize(); vertexIdx+=4)
		{
			m_vertices[vertexIdx+0] *= m_scaling(0);
			m_vertices[vertexIdx+1] *= m_scaling(1);
			m_vertices[vertexIdx+2] *= m_scaling(2);
			m_vertices[vertexIdx+3] *= m_scaling(3);
		}
	}

	hkpExtendedMeshShape* extendedMeshShape = new hkpExtendedMeshShape();
	{
		hkpExtendedMeshShape::TrianglesSubpart part;
		part.m_numTriangleShapes	= m_numTriangles;
		part.m_numVertices			= m_numVertices;
		part.m_vertexBase			= &m_vertices[0];
		part.m_stridingType			= hkpExtendedMeshShape::INDICES_INT16;
		part.m_vertexStriding		= sizeof(float) * 4;
		part.m_indexBase			= &m_indices[0];
		part.m_indexStriding		= sizeof( hkUint16 ) * 4;

		extendedMeshShape->addTrianglesSubpart( part );
	}

	int numTriangles = extendedMeshShape->getNumChildShapes();
	numTriangles ++;

	return extendedMeshShape;
}

hkpMoppBvTreeShape* FlatLand::createMoppShapeForSpu()
{
	hkpExtendedMeshShape* extendedMeshShape = createExtendedMeshShape();

	return createMoppShapeForSpu(extendedMeshShape);
}

hkpMoppBvTreeShape* FlatLand::createMoppShape()
{
	hkpExtendedMeshShape* meshShape = createExtendedMeshShape();
	hkpMoppBvTreeShape* mopp = createMoppShape(meshShape);

	meshShape->computeWeldingInfo(mopp, hkpWeldingUtility::WELDING_TYPE_ANTICLOCKWISE);

	return mopp;
}

hkpMoppBvTreeShape* FlatLand::createMoppShape(hkpShapeCollection* meshShape)
{
	hkpMoppCompilerInput mci;

	mci.m_cachePrimitiveExtents = m_cachedBuildEnabled;
	mci.m_enableChunkSubdivision = true;

	hkpMoppCode* code = hkpMoppUtility::buildCode( meshShape , mci);

	hkpMoppBvTreeShape* moppShape = new hkpMoppBvTreeShape(meshShape, code);

	code->removeReference();
	meshShape->removeReference();

	return moppShape;
}

hkpMoppBvTreeShape* FlatLand::createMoppShapeForSpu(hkpShapeCollection* meshShape)
{
	hkpMoppCompilerInput mci;
	{
		mci.m_cachePrimitiveExtents	 = m_cachedBuildEnabled;
		mci.m_enableChunkSubdivision = true;
	}

	hkpMoppCode* code = hkpMoppUtility::buildCode( meshShape , mci);

	hkpMoppBvTreeShape* moppShape = new hkpMoppBvTreeShape(meshShape, code);

	code->removeReference();
	meshShape->removeReference();

	return moppShape;
}


class FlatlandLandscapeContainer : public LandscapeContainer
{
	public:
		FlatlandLandscapeContainer()
		{
			m_flatland = HK_NULL;
		}

		~FlatlandLandscapeContainer()
		{
			if (m_flatland)
			{
				m_flatland->removeReference();
			}
		}
		FlatLand* m_flatland;

};

static LandscapeContainer* createFlatLand( const int landscapeSize, const hkVector4& scaling )
{
	FlatlandLandscapeContainer* landscapeContainer = new FlatlandLandscapeContainer();

	landscapeContainer->m_flatland = new FlatLand(landscapeSize);
	landscapeContainer->m_flatland->setScaling(scaling);
	landscapeContainer->m_shape = landscapeContainer->m_flatland->createMoppShapeForSpu();
	
	hkAabb aabb(hkVector4(-95, 2, -95), hkVector4(95, 8, 95));

	landscapeContainer->m_spawnVolumes.pushBack(aabb);
	landscapeContainer->m_cameraFrom.set(138, 60, -136);
	landscapeContainer->m_cameraTo.set(0,0,0);

	return landscapeContainer;
}

LandscapeContainer* HK_CALL createDefaultFlatLandOneMetreTris( void* param )
{
	const int landscapeSize = 200;
	const hkVector4 scaling( 1.f, .1f, 1.f );

	return createFlatLand(landscapeSize, scaling);
}

LandscapeContainer* HK_CALL createSimpleFlatLandOneMetreTris( void* param )
{
	const int landscapeSize = 50;
	const hkVector4 scaling( 4.f, .1f, 4.f );

	return createFlatLand(landscapeSize, scaling);
}

HK_DECLARE_LANDSCAPE( createSimpleFlatLandOneMetreTris, simpleFlatLandOneMetreTris, 0);
HK_DECLARE_LANDSCAPE( createDefaultFlatLandOneMetreTris, defaultFlatLandOneMetreTris, 0);  // The default flatland comes last so that it's first in the landscape registry.

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
