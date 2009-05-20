/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/DemoCommon/Utilities/GameUtils/Landscape/RoundTrack.h>

// We will need these shapes
#include <Physics/Collide/Shape/Compound/Tree/Mopp/hkpMoppBvTreeShape.h>
#include <Physics/Collide/Shape/Deprecated/FastMesh/hkpFastMeshShape.h>

#include <Physics/Collide/Shape/Compound/Tree/Mopp/hkpMoppUtility.h>


RoundTrackDemo::RoundTrackDemo()
{
}

RoundTrackDemo::~RoundTrackDemo()
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

hkpMoppBvTreeShape* RoundTrackDemo::createMoppShape()
{
	//
	// Create the vertices (a "heightfield" of triangles)
	//
	{
		const float borderHeight = 5.0f;
		const hkReal radius = 500.0f;
		const hkReal width = 55.0f;
		const hkReal jumps = 0.15f;
		const hkReal bendsFrequency  = 10.0f;
		const hkReal bendsSize = 60.0f;
		{
			for(int i = 0; i < SIDEA; i++)
			{
				for (int j = 0; j < SIDEB; j++ )
				{

					float h = (hkMath::cos(0.2f*((hkReal)j + i)) + 0.5f * hkMath::sin( 0.2f*2.0f * i));
					h = h * h * h * h *h * h - 6;
					h *= jumps;

					//
					//	Now bend the track in a circle
					//
					hkReal angle = i / float(SIDEA) * 6.32f;
					float si = hkMath::sin( angle );
					float co = hkMath::cos( angle );

					int j2 = j-1; if ( j2<0 ) j2++;
					if ( j2 >= SIDEB - 2) j2--;

					float sj = j2 / float(SIDEB - 2);

					float bends = bendsSize * hkMath::sin( angle * bendsFrequency ) * hkMath::sin( angle * bendsFrequency * 0.34f ); 
					float x = si * (radius + sj * width + bends) + radius;
					float y = co * (radius + sj * width + bends);

					m_vertices[(i * SIDEB + j) * 3  + 0] = x;
					m_vertices[(i * SIDEB + j) * 3  + 1] = h;
					m_vertices[(i * SIDEB + j) * 3  + 2] = y;
				}
			}
		}
		//
		//	Add a border
		//
		{
			for(int i = 0; i < SIDEA; i++)
			{
				m_vertices[ (i * SIDEB + 0     ) * 3  + 1] = borderHeight;
				m_vertices[ (i * SIDEB + SIDEB-1) * 3  + 1] = borderHeight;
			}

		}
	}

	//
	// Create the triangles
	//
	{
		hkUint16 corner = 0;
		int index = 0;
		for(int i = 0; i < SIDEA - 1; i++)
		{
			for (int j = 0; j < SIDEB - 1; j++ )
			{
				m_indices[index]     = hkUint16(corner);
				m_indices[index + 1] = hkUint16(corner + 1);
				m_indices[index + 2] = hkUint16(corner + SIDEB);

				m_indices[index + 3] = hkUint16(corner + 1);
				m_indices[index + 4] = hkUint16(corner + SIDEB);
				m_indices[index + 5] = hkUint16(corner + SIDEB + 1);

				index += 6;
				corner++; 
			}
			corner++; 
		}
	}

	hkpMeshShape* meshShape = new hkpFastMeshShape( );
	meshShape->setRadius(0.05f);
	{
		hkpMeshShape::Subpart part;

		part.m_vertexBase = m_vertices;
		part.m_vertexStriding = sizeof(float) * 3;
		part.m_numVertices = NUM_VERTICES;

		part.m_indexBase = m_indices;
		part.m_indexStriding = sizeof( hkUint16 ) * 3;
		part.m_numTriangles = NUM_TRIANGLES;
		part.m_stridingType = hkpMeshShape::INDICES_INT16;

		meshShape->addSubpart( part );
	}

	hkpMoppCompilerInput mci;
	hkpMoppCode* code = hkpMoppUtility::buildCode( meshShape , mci);

	hkpMoppBvTreeShape* moppShape = new hkpMoppBvTreeShape(meshShape, code);
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
