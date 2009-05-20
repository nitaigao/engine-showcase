/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#include <Demos/demos.h>


#include <Demos/Physics/Test/Stress/TowerStressTest/TowerLand.h>

// We will need these shapes
#include <Physics/Collide/Shape/Compound/Tree/Mopp/hkpMoppBvTreeShape.h>

#include <Physics/Collide/Shape/Compound/Tree/Mopp/hkpMoppUtility.h>
#include <Physics/Collide/Shape/Compound/Collection/ExtendedMeshShape/hkpExtendedMeshShape.h>


TowerLand::TowerLand(hkReal size, int side)
{
	m_size					= size;
	m_side					= side;
	m_numVertices			= m_side * m_side;
	m_numTriangles			= 2 * (m_side - 1) * (m_side -1 );
	m_shift					= 0.0f;

	m_cachedBuildEnabled	= false;

	m_vertices				.setSize(m_numVertices * 4);
	m_indices				.setSize(m_numTriangles * 4);
	m_scaling				.set( 20.0f, 1.0f, 20.0f );
	m_triangleRadius		= 0.01f;
}

TowerLand::~TowerLand()
{
}


hkReal TowerLand::getHeight( int i, int j )
{
	hkReal h = (hkMath::cos(0.2f*((hkReal)j + i)) + 0.5f * hkMath::sin( 0.2f*2.0f * i)) * 1.0f;

	h = h * h * h * h *h * h - 6.0f;
	return h;
}
	

hkReal TowerLand::getBorderHeight()
{
	return 3.0f;
}

void TowerLand::setScaling( const hkVector4& scaling )
{
	m_scaling = scaling;
}

void TowerLand::setTriangleRadius( const hkReal s )
{
	m_triangleRadius = s;
}

void TowerLand::setShift( const hkReal shift )
{
	m_shift = shift;
}

void TowerLand::enableCachedBuild( hkBool enable )
{
	m_cachedBuildEnabled = enable;
}

void TowerLand::setupVertexAndTriangleData()
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

					m_vertices[ (i * m_side + j) * 4  + 0 ] = ((i - m_side * 0.5f) / m_side) * m_size;
					m_vertices[ (i * m_side + j) * 4  + 1 ] = h;
					m_vertices[ (i * m_side + j) * 4  + 2 ] = ((j - m_side * 0.5f) / m_side) * m_size - ( (i%2) * m_shift);
					m_vertices[ (i * m_side + j) * 4  + 3 ] = 0.0f;	// we need to set this to avoid denormals on P4
				}
			}
		}
		//
		//	Add a border
		//
		{
			for(int i = 0; i < m_side; i++)
			{
				m_vertices[ (i * m_side + 0     )                 * 4  + 1] = borderHeight;
				m_vertices[ (i * m_side + m_side-1)               * 4  + 1] = borderHeight;
				m_vertices[ ( (m_side-1) * (m_side-1) + i       ) * 4  + 1] = borderHeight;
				m_vertices[ (i                                  ) * 4  + 1] = borderHeight;
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
				m_indices[index + 3] = 0; // padding needed for spus

				m_indices[index + 4] = hkUint16(corner + 1);
				m_indices[index + 5] = hkUint16(corner + m_side + 1);
				m_indices[index + 6] = hkUint16(corner + m_side);
				m_indices[index + 7] = 0; // padding needed for spus

				index += 8;
				corner++; 
			}
			corner++; 
		}
	}
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

hkpExtendedMeshShape* TowerLand::createMeshShape()
{
	setupVertexAndTriangleData();

	hkpExtendedMeshShape* meshShape = new hkpExtendedMeshShape();
	meshShape->setRadius( m_triangleRadius );
	meshShape->setScaling( m_scaling );

	{
		hkpExtendedMeshShape::TrianglesSubpart part;

		part.m_vertexBase = &m_vertices[0];
		part.m_vertexStriding = sizeof(float) * 4;
		part.m_numVertices = m_numVertices;

		part.m_indexBase = &m_indices[0];
		part.m_indexStriding = sizeof( hkUint16 ) * 4;
		part.m_numTriangleShapes = m_numTriangles;
		part.m_stridingType = hkpExtendedMeshShape::INDICES_INT16;

		meshShape->addTrianglesSubpart( part );
	}

	return meshShape;
}

hkpMoppBvTreeShape* TowerLand::createMoppShape()
{
	hkpExtendedMeshShape* meshShape = createMeshShape();

	return createMoppShape(meshShape);
}

hkpMoppBvTreeShape* TowerLand::createMoppShape(hkpExtendedMeshShape* meshShape)
{
	hkpMoppCompilerInput mci;

	mci.m_cachePrimitiveExtents = m_cachedBuildEnabled;

	//hkStopwatch stopwatch;	stopwatch.start();

	hkpMoppCode* code = hkpMoppUtility::buildCode( meshShape , mci);

	//stopwatch.stop();
	//hkReal timeBuildMoppCode = stopwatch.getElapsedSeconds();
	//hkprintf("Time TowerLand : %g msec\n", timeBuildMoppCode*1000.0f);

	hkpMoppBvTreeShape* moppShape = new hkpMoppBvTreeShape(meshShape, code);


	//hkpMoppStatisticsVirtualMachine vm;
	//vm.queryAll(code);
	//vm.printStatistics(code);

	code->removeReference();
	meshShape->removeReference();

	return moppShape;
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
