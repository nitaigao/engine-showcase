/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/DemoCommon/Utilities/hkGeometrySplitter.h>
#include <Common/Base/Container/LocalArray/hkLocalArray.h>
#include <Common/Base/Memory/Memory/hkMemory.h>

#include <Common/Base/Container/PointerMap/hkPointerMap.h>

#include <Graphics/Common/DisplayObject/hkgDisplayObject.h>
#include <Graphics/Common/Geometry/VertexSet/hkgVertexSet.h>
#include <Common/Visualize/hkDebugDisplay.h>



static inline hkUint16 addCutVert(	hkReal dotA, 
									hkReal dotB, 
									hkUint16 vertA, 
									hkUint16 vertB, 
									hkSplitGeometryInput& input, 
									hkSplitGeometryOutput& output, 
									hkPointerMap<hkUint32, hkUint32>& newVertMap)
{
	// Check if vertex has been added
	hkUint32 key; key = vertA + (vertB << 16);
	hkUint32 index;
	if (newVertMap.get(key, &index) == HK_SUCCESS)
	{
		return (hkUint16)index;
	}
	else
	{
		hkSplitGeometryOutput::CutVertex& cutVert = output.m_cutVertices.expandOne();
		cutVert.m_origVertA = vertA;
		cutVert.m_origVertB = vertB;
		cutVert.m_weight = -dotB / (dotA - dotB);

		newVertMap.insert(key, output.m_cutVertices.getSize() - 1);
		return (hkUint16)output.m_cutVertices.getSize() - 1;
	}
}

static inline void addTriangle( hkUint16 a, hkUint16 b, hkUint16 c, hkArray<hkUint16>& indices )
{
	indices.pushBack(a);
	indices.pushBack(b);
	indices.pushBack(c);
}

static inline void getVector(hkVector4& v, unsigned char* ptr)
{
	hkReal* vec = (hkReal*)ptr;
	v(0) = vec[0]; v(1) = vec[1]; v(2) = vec[2];
}

void splitGeometryWithPlane( hkSplitGeometryInput& input, hkSplitGeometryOutput& output )
{
	// Map of original vertices
	hkLocalArray<hkUint16> reusedVertMap(input.m_numVertices);
	reusedVertMap.setSize(input.m_numVertices);

	hkPointerMap<hkUint32, hkUint32> newVertMap;

	// copy known vertices into output
	{
		for (hkUint16 i = 0; i < input.m_numVertices; ++i)
		{
			hkVector4 a; getVector(a, &input.m_vertices[i* input.m_stride]); a(3) = 1;

			if ( a.dot4(input.m_plane) < 0 )
			{
				reusedVertMap[i] = (hkUint16)output.m_reusedVertices.getSize();
				output.m_reusedVertices.pushBack(i);
			}
			else
			{
				// vertex will be unused
				reusedVertMap[i] = 0xffff;
			}
		}
	}

	const hkUint16 newVertBase = (hkUint16)output.m_reusedVertices.getSize();

	{
		for (int i = 0; i < input.m_indexBufs.getSize(); ++i)
		{
			for (int j = 0; j < input.m_indexBufs[i].m_numIndices - 2; j += input.m_indexBufs[i].m_triangleStride)
			{

				
				hkUint16 vertA;
				hkUint16 vertB;
				hkUint16 vertC;
				if (input.m_indexBufs[i].m_indices == 0)
				{
					vertA = (hkUint16)j + input.m_indexBufs[i].m_startOffset;
					vertB = (hkUint16)j + input.m_indexBufs[i].m_startOffset + 1;
					vertC = (hkUint16)j + input.m_indexBufs[i].m_startOffset + 2;
				}
				else
				{
					vertA = input.m_indexBufs[i].m_indices[j] + input.m_indexBufs[i].m_startOffset;
					vertB = input.m_indexBufs[i].m_indices[j + 1] + input.m_indexBufs[i].m_startOffset;
					vertC = input.m_indexBufs[i].m_indices[j + 2] + input.m_indexBufs[i].m_startOffset;

					// If input is a strip, swap the windings on alternate triangles
					if ( (input.m_indexBufs[i].m_triangleStride == 1) && (j % 2 == 1) )
					{
						hkUint16 temp = vertB;
						vertB = vertC;
						vertC = temp;
					}
				}

				hkReal dotA;
				hkReal dotB;
				hkReal dotC;
				{
					hkVector4 a; getVector(a, &input.m_vertices[vertA * input.m_stride]); a(3) = 1;
					hkVector4 b; getVector(b, &input.m_vertices[vertB * input.m_stride]); b(3) = 1;
					hkVector4 c; getVector(c, &input.m_vertices[vertC * input.m_stride]); c(3) = 1;

					dotA = a.dot4(input.m_plane);
					dotB = b.dot4(input.m_plane);
					dotC = c.dot4(input.m_plane);
				}

				int numOutside = 3;
				if ( dotA < 0) numOutside--;
				if ( dotB < 0) numOutside--;
				if ( dotC < 0) numOutside--;

				// If Triangle is outside plane, ignore it.
				if (numOutside == 3)
				{
					continue;
				}

				// If triangle is fully inside, add it.
				if (numOutside == 0)
				{
					output.m_outsideFaceIndices.pushBack(reusedVertMap[vertA]);
					output.m_outsideFaceIndices.pushBack(reusedVertMap[vertB]);
					output.m_outsideFaceIndices.pushBack(reusedVertMap[vertC]);
					continue;

				}

				// If 2 verts are outside
				if (numOutside == 2)
				{
					if (dotA < 0)
					{
						hkUint16 newVertB = addCutVert( dotA, dotB, vertA, vertB, input, output, newVertMap );
						hkUint16 newVertC = addCutVert( dotA, dotC, vertA, vertC, input, output, newVertMap );
						
						// Add outside triangle
						addTriangle( reusedVertMap[vertA], newVertB + newVertBase, newVertC + newVertBase, output.m_outsideFaceIndices );

						// Add inside triangle
						addTriangle( 0, newVertC + 1, newVertB + 1, output.m_insideFaceIndices );
						continue;
					}
					if (dotB < 0)
					{
						hkUint16 newVertA = addCutVert( dotB, dotA, vertB, vertA, input, output, newVertMap );
						hkUint16 newVertC = addCutVert( dotB, dotC, vertB, vertC, input, output, newVertMap );

						// Add outside triangle
						addTriangle( reusedVertMap[vertB], newVertC + newVertBase, newVertA + newVertBase, output.m_outsideFaceIndices );

						// Add inside triangle
						addTriangle( 0, newVertA + 1, newVertC + 1, output.m_insideFaceIndices );
						continue;
					}
					if (dotC < 0)
					{
						hkUint16 newVertB = addCutVert( dotC, dotB, vertC, vertB, input, output, newVertMap );
						hkUint16 newVertA = addCutVert( dotC, dotA, vertC, vertA, input, output, newVertMap );

						// Add outside triangle
						addTriangle( reusedVertMap[vertC], newVertA + newVertBase, newVertB + newVertBase, output.m_outsideFaceIndices );

						// Add inside triangle
						addTriangle( 0, newVertB + 1, newVertA + 1, output.m_insideFaceIndices );
						continue;
					}
				}
				else
				{
					// numOutside = 1
					if (dotA >= 0)
					{
						hkUint16 vB = addCutVert(dotB, dotA, vertB, vertA, input, output, newVertMap );
						hkUint16 vC = addCutVert(dotC, dotA, vertC, vertA, input, output, newVertMap );

						addTriangle( vB + newVertBase, reusedVertMap[vertB], vC + newVertBase, output.m_outsideFaceIndices );
						addTriangle( vC + newVertBase, reusedVertMap[vertB], reusedVertMap[vertC], output.m_outsideFaceIndices );

						addTriangle( 0, vB + 1, vC + 1, output.m_insideFaceIndices);
						continue;
					}
					if (dotB >= 0)
					{
						hkUint16 vC = addCutVert(dotC, dotB, vertC, vertB, input, output, newVertMap );
						hkUint16 vA = addCutVert(dotA, dotB, vertA, vertB, input, output, newVertMap );

						addTriangle( vC + newVertBase, reusedVertMap[vertC], vA + newVertBase, output.m_outsideFaceIndices );
						addTriangle( vA + newVertBase, reusedVertMap[vertC], reusedVertMap[vertA], output.m_outsideFaceIndices );

						addTriangle( 0, vC + 1, vA + 1, output.m_insideFaceIndices);
						continue;
					}
					if (dotC >= 0)
					{
						hkUint16 vA = addCutVert(dotA, dotC, vertA, vertC, input, output, newVertMap );
						hkUint16 vB = addCutVert(dotB, dotC, vertB, vertC, input, output, newVertMap );

						addTriangle( vA + newVertBase, reusedVertMap[vertA], vB + newVertBase, output.m_outsideFaceIndices );
						addTriangle( vB + newVertBase, reusedVertMap[vertA], reusedVertMap[vertB], output.m_outsideFaceIndices );

						addTriangle( 0, vA + 1, vB + 1, output.m_insideFaceIndices);
						continue;
					}
				}
			}
		}
	}
}

hkgDisplayObject* createSplitDisplayObject( hkgDisplayObject* displayObjectIn, hkgDisplayContext* context, const hkVector4& plane )
{
	// assume 1 geometry
	HK_ASSERT(0, displayObjectIn->getNumGeometry() == 1);
	hkgGeometry* geomIn = displayObjectIn->getGeometry(0);

	// can only deal with 1 face set at the moment :-<
	HK_ASSERT(0, geomIn->getNumMaterialFaceSets() == 1);
	hkgMaterialFaceSet* matFaceSetIn = geomIn->getMaterialFaceSet(0);

	HK_ASSERT(0, matFaceSetIn->getNumFaceSets() == 1);
	hkgFaceSet* faceSetIn = matFaceSetIn->getFaceSet(0);

	hkSplitGeometryInput splitInput;
	splitInput.m_vertices = faceSetIn->getVertexSet()->lock(HKG_LOCK_READONLY);
	splitInput.m_numVertices = faceSetIn->getVertexSet()->getNumVerts();
	splitInput.m_stride = faceSetIn->getVertexSet()->getStrideInBytes();

	// if we are not using an interleaved format, then we can assume
	// the positions are packed together tightly (hkg oddity that it returns 0 of this case).
	if (splitInput.m_stride == 0)
	{
		 splitInput.m_stride = faceSetIn->getVertexSet()->getComponentSize(HKG_VERTEX_COMPONENT_POS);
	}


	//hkVector4 a; getVector(a, &splitInput.m_vertices[10 * splitInput.m_stride]); //a(3) = 1;
	//hkVector4 pos; getVector(pos, (unsigned char*)faceSetIn->getVertices()->getVertexComponentData(HKG_VERTEX_COMPONENT_POS, 10 ));


	splitInput.m_indexBufs.setSize(faceSetIn->getNumPrimitives());
	{
		for (int i = 0; i < faceSetIn->getNumPrimitives(); ++i)
		{
			splitInput.m_indexBufs[i].m_indices = (hkUint16*)faceSetIn->getPrimitive(i)->getIndices();
			splitInput.m_indexBufs[i].m_numIndices = faceSetIn->getPrimitive(i)->getLength();
			splitInput.m_indexBufs[i].m_startOffset = (hkUint16)faceSetIn->getPrimitive(i)->getVertexBufferStartIndex();

			if (faceSetIn->getPrimitive(i)->getType() == HKG_TRI_LIST)
			{
				splitInput.m_indexBufs[i].m_triangleStride = 3;
			}
			else // HKG_STRIP
			{
				splitInput.m_indexBufs[i].m_triangleStride = 1;
			}
		}
	}

	splitInput.m_plane = plane;


	hkSplitGeometryOutput output;
	splitGeometryWithPlane(splitInput, output);


	// build new display geometry
	hkgDisplayObject* displayObjectOut = hkgDisplayObject::create();
	displayObjectOut->setStatusFlags(displayObjectIn->getStatusFlags() );


	hkgGeometry* geomOut = hkgGeometry::create();
	hkgMaterialFaceSet* matFaceSetOut = hkgMaterialFaceSet::create();
	

	matFaceSetOut->setMaterial(matFaceSetIn->getMaterial());

	hkgFaceSet* faceSetOut = hkgFaceSet::create( context );

	matFaceSetOut->addFaceSet( faceSetOut );
	faceSetOut->release();

	geomOut->addMaterialFaceSet( matFaceSetOut );
	matFaceSetOut->release();

	displayObjectOut->addGeometry( geomOut );
	geomOut->removeReference();

	//
	// Populate vertex buffer
	//
	{
		hkgVertexSet* origVertexSet = faceSetIn->getVertexSet();
		HK_ASSERT(0x0, origVertexSet );
		hkgVertexSet* targetVertexSet = hkgVertexSet::create(context);
		faceSetOut->setVertexSet(targetVertexSet);
		targetVertexSet->removeReference();

		const int numVertsOut = output.m_reusedVertices.getSize() + output.m_cutVertices.getSize() * 2 + 1;
		targetVertexSet->setNumVerts( numVertsOut, faceSetIn->getVertexSet()->getVertexFormat() );
		targetVertexSet->lock(HKG_LOCK_WRITEONLY);

		HKG_VERTEX_FORMAT f = origVertexSet->getVertexFormat();

		int i = 0;
		for ( ; i < output.m_reusedVertices.getSize(); ++i )
		{
			// could do this faster with memcpy and stride
			targetVertexSet->copyExistingVertex( i, output.m_reusedVertices[i], origVertexSet );
		}

		hkLocalArray<hkVector4> interpolatedPositions( output.m_cutVertices.getSize() + 1 );
		interpolatedPositions.setSizeUnchecked( output.m_cutVertices.getSize() + 1 );

		hkVector4& centroid = interpolatedPositions[0]; centroid.setZero4();

		{
			bool colorsAsFloats = faceSetIn->getVertexSet()->getVertexComponentFormat( HKG_VERTEX_COMPONENT_COLOR ) != HKG_VERTEX_STORAGE_4UB;

			for ( int j = 0; j < output.m_cutVertices.getSize(); ++i, ++j)
			{
				// Interpolate the relevant values - not interpolating everything (e.g. weights and colors)

				hkReal aMul = output.m_cutVertices[j].m_weight;
				hkReal bMul = 1 - output.m_cutVertices[j].m_weight;
				{
					//
					// Pos (always there) - assume 3 floats
					//
					const hkReal* posA = reinterpret_cast<const hkReal*>(origVertexSet->getVertexComponentData(HKG_VERTEX_COMPONENT_POS, output.m_cutVertices[j].m_origVertA));
					const hkReal* posB = reinterpret_cast<const hkReal*>(origVertexSet->getVertexComponentData(HKG_VERTEX_COMPONENT_POS, output.m_cutVertices[j].m_origVertB));
					hkReal pos[3];
					pos[0] = posA[0] * aMul + posB[0] * bMul;
					pos[1] = posA[1] * aMul + posB[1] * bMul;
					pos[2] = posA[2] * aMul + posB[2] * bMul;

					interpolatedPositions[j+1](0) = pos[0];
					interpolatedPositions[j+1](1) = pos[1];
					interpolatedPositions[j+1](2) = pos[2];
					centroid.add4(interpolatedPositions[j+1]);

					targetVertexSet->setVertexComponentData( HKG_VERTEX_COMPONENT_POS, i, pos );
				}

				if (f & HKG_VERTEX_FORMAT_COLOR)
				{
					if (colorsAsFloats)
					{
						float col[] = { 1.0f, 1.0f, 1.0f, 1.0f };
						targetVertexSet->setVertexComponentData( HKG_VERTEX_COMPONENT_COLOR, i, &col );
					}
					else
					{
						hkUint32 col = 0xffffffff;
						targetVertexSet->setVertexComponentData( HKG_VERTEX_COMPONENT_COLOR, i, &col );
					}
				}
				// UV - assume 2 floats
				const HKG_VERTEX_COMPONENT TEX_SHIFTS[] = { HKG_VERTEX_COMPONENT_TEX0, HKG_VERTEX_COMPONENT_TEX1, HKG_VERTEX_COMPONENT_TEX2, HKG_VERTEX_COMPONENT_TEX3, 0 };
				for( const HKG_VERTEX_COMPONENT* tex = TEX_SHIFTS; *tex != 0; ++tex )
				{
					HK_COMPILE_TIME_ASSERT( (1<<HKG_VERTEX_COMPONENT_TEX0) == HKG_VERTEX_FORMAT_TEX0 );
					HK_COMPILE_TIME_ASSERT( (1<<HKG_VERTEX_COMPONENT_TEX1) == HKG_VERTEX_FORMAT_TEX1 );
					HK_COMPILE_TIME_ASSERT( (1<<HKG_VERTEX_COMPONENT_TEX2) == HKG_VERTEX_FORMAT_TEX2 );
					HK_COMPILE_TIME_ASSERT( (1<<HKG_VERTEX_COMPONENT_TEX3) == HKG_VERTEX_FORMAT_TEX3 );

					HKG_VERTEX_COMPONENT component = *tex;
					HKG_VERTEX_FORMAT format = 1 << component;
					if (f & format)
					{
						const hkReal* a = reinterpret_cast<const hkReal*>(origVertexSet->getVertexComponentData( component, output.m_cutVertices[j].m_origVertA));
						const hkReal* b = reinterpret_cast<const hkReal*>(origVertexSet->getVertexComponentData( component, output.m_cutVertices[j].m_origVertB));
						hkReal t[2];
						t[0] = a[0] * aMul + b[0] * bMul;
						t[1] = a[1] * aMul + b[1] * bMul;
						targetVertexSet->setVertexComponentData( component, i, t );
					}
				}
				//
				// Normals etc - assume 3 floats
				const HKG_VERTEX_COMPONENT VEC_SHIFTS[] = { HKG_VERTEX_COMPONENT_NORMAL, HKG_VERTEX_COMPONENT_TANGENT, HKG_VERTEX_COMPONENT_BITANGENT, 0 };
				for( const HKG_VERTEX_COMPONENT* vec = VEC_SHIFTS; *vec != 0; ++vec )
				{
					HK_COMPILE_TIME_ASSERT( (1<<HKG_VERTEX_COMPONENT_NORMAL) == HKG_VERTEX_FORMAT_NORMAL );
					HK_COMPILE_TIME_ASSERT( (1<<HKG_VERTEX_COMPONENT_TANGENT) == HKG_VERTEX_FORMAT_TANGENT );
					HK_COMPILE_TIME_ASSERT( (1<<HKG_VERTEX_COMPONENT_BITANGENT) == HKG_VERTEX_FORMAT_BITANGENT );

					HKG_VERTEX_COMPONENT component = *vec;
					HKG_VERTEX_FORMAT format = 1<<component;
					if (f & format)
					{
						const hkReal* a = reinterpret_cast<const hkReal*>(origVertexSet->getVertexComponentData( component, output.m_cutVertices[j].m_origVertA));
						const hkReal* b = reinterpret_cast<const hkReal*>(origVertexSet->getVertexComponentData( component, output.m_cutVertices[j].m_origVertB));
						hkReal n[3];
						n[0] = a[0] * aMul + b[0] * bMul;
						n[1] = a[1] * aMul + b[1] * bMul;
						n[2] = a[2] * aMul + b[2] * bMul;

						targetVertexSet->setVertexComponentData( component, i, n );
					}
				}
			}
		}

		centroid.mul4( 1.f / output.m_cutVertices.getSize() );

		//
		// Fill in info for new vertices on new surface
		//

		// This is where the new texture will be got from for the new faces. Could make this better - need to correlate with artwork.
		hkReal fakeUV[2]; fakeUV[0] = 0; fakeUV[1] = 0;

		hkVector4 normal(0,1,0);
		{
			int k = 0;
			// Normal for all triangles is the same - get first non degenerate one
			for (; k < output.m_insideFaceIndices.getSize() - 2; k += 3)
			{
				hkVector4 a; a.setSub4(interpolatedPositions[output.m_insideFaceIndices[k]], interpolatedPositions[output.m_insideFaceIndices[k + 1]]);
				hkVector4 b; b.setSub4(interpolatedPositions[output.m_insideFaceIndices[k + 2]], interpolatedPositions[output.m_insideFaceIndices[k + 1]]);
				normal.setCross(b, a);
				if ( normal.lengthSquared3() > .001f )
				{
					normal.normalize3();
					break;
				}
			}

			if (k >= output.m_insideFaceIndices.getSize() - 2)
			{
				HK_WARN_ONCE(0xf0453234, "normal broken");
			}
		}

	
		//
		// Add centroid vertex. Set position, normal and texture coords.
		//
		targetVertexSet->setVertexComponentData( HKG_VERTEX_COMPONENT_POS, i, &centroid(0) );
		targetVertexSet->setVertexComponentData( HKG_VERTEX_COMPONENT_NORMAL, i, &normal(0) );
		if ( f & HKG_VERTEX_FORMAT_TEX0 )	{	targetVertexSet->setVertexComponentData( HKG_VERTEX_COMPONENT_TEX0, i, fakeUV );		}
		if ( f & HKG_VERTEX_FORMAT_TEX1 )	{	targetVertexSet->setVertexComponentData( HKG_VERTEX_COMPONENT_TEX1, i, fakeUV );		}		
		if ( f & HKG_VERTEX_FORMAT_TEX2 )	{	targetVertexSet->setVertexComponentData( HKG_VERTEX_COMPONENT_TEX2, i, fakeUV );		}
		if ( f & HKG_VERTEX_FORMAT_TEX3 )	{	targetVertexSet->setVertexComponentData( HKG_VERTEX_COMPONENT_TEX3, i, fakeUV );		}
		i++;


		//
		// Re-add the boundary vertices
		//
		{
			for ( int j = 0; j < output.m_cutVertices.getSize(); ++j, ++i)
			{
				targetVertexSet->setVertexComponentData( HKG_VERTEX_COMPONENT_POS, i, &interpolatedPositions[j + 1] );
				targetVertexSet->setVertexComponentData( HKG_VERTEX_COMPONENT_NORMAL, i, &normal(0) );
				if ( f & HKG_VERTEX_FORMAT_TEX0 )	{	targetVertexSet->setVertexComponentData( HKG_VERTEX_COMPONENT_TEX0, i, fakeUV );		}
				if ( f & HKG_VERTEX_FORMAT_TEX1 )	{	targetVertexSet->setVertexComponentData( HKG_VERTEX_COMPONENT_TEX1, i, fakeUV );		}		
				if ( f & HKG_VERTEX_FORMAT_TEX2 )	{	targetVertexSet->setVertexComponentData( HKG_VERTEX_COMPONENT_TEX2, i, fakeUV );		}
				if ( f & HKG_VERTEX_FORMAT_TEX3 )	{	targetVertexSet->setVertexComponentData( HKG_VERTEX_COMPONENT_TEX3, i, fakeUV );		}
			}
		}

		origVertexSet->unlock();
		targetVertexSet->unlock();
	}

	// Populate index buffer
	if (output.m_outsideFaceIndices.getSize())
	{
		hkgFaceSetPrimitive* prim = new hkgFaceSetPrimitive(HKG_TRI_LIST);
		prim->setLength( output.m_insideFaceIndices.getSize() + output.m_outsideFaceIndices.getSize() );
		hkUint16* indices = prim->getIndices();

		if (indices != HK_NULL)
		{
			hkString::memCpy(indices, &output.m_outsideFaceIndices[0], output.m_outsideFaceIndices.getSize() * 2);
		}

		const hkUint16 indexBase = (hkUint16)output.m_outsideFaceIndices.getSize();
		const hkUint16 vertexBase = (hkUint16)output.m_reusedVertices.getSize() + (hkUint16)output.m_cutVertices.getSize();
		for (int i = 0; i < output.m_insideFaceIndices.getSize(); ++i )
		{
			indices[i + indexBase] = output.m_insideFaceIndices[i] + vertexBase;
		}
		
		// faceSetOut takes ownership of prim, so we forget about it
		faceSetOut->addPrimitive( prim );
	}

	faceSetOut->initialize();

	// initialize the transform of the display object
	{
		hkTransform transform; transform.setIdentity();

		HK_ALIGN(float cm16[16], 16);
		transform.get4x4ColumnMajor(cm16);
		displayObjectOut->setTransform(cm16);
		if (!displayObjectOut->hasAABB())
			displayObjectOut->computeAABB();
		float el = hkgVec3Length(displayObjectOut->getAABBExtPtr());
		if (el < 20) // objects that are too big do not become casters
		{
			displayObjectOut->setStatusFlags( displayObjectOut->getStatusFlags() | HKG_DISPLAY_OBJECT_DYNAMIC | HKG_DISPLAY_OBJECT_SHADOW );
		}
		else
		{
			displayObjectOut->setStatusFlags( displayObjectOut->getStatusFlags() | HKG_DISPLAY_OBJECT_DYNAMIC | HKG_DISPLAY_OBJECT_SHADOWRECEIVER );
		}
	}

	return displayObjectOut;

}

// todo - test tri list
hkTempSplitGeom* createTempFromDisplayGeom( hkgDisplayObject* displayObjectIn )
{
	hkTempSplitGeom* tempGeom = new hkTempSplitGeom();

	// assume 1 geometry
	if (displayObjectIn->getNumGeometry() < 1) 
		return tempGeom; // empty too..

	hkgGeometry* geomIn = displayObjectIn->getGeometry(0);

	// can only deal with 1 face set at the moment :-<
	HK_ASSERT(0, geomIn && (geomIn->getNumMaterialFaceSets() == 1));
	hkgMaterialFaceSet* matFaceSetIn = geomIn->getMaterialFaceSet(0);

	HK_ASSERT(0, matFaceSetIn->getNumFaceSets() == 1);
	hkgFaceSet* faceSetIn = matFaceSetIn->getFaceSet(0);


	
	if( faceSetIn->getNumPrimitives() > 1)
	{
		return tempGeom; // keep things simple, should be a trilist on all platforms
	}
	
	hkgFaceSetPrimitive* prim = faceSetIn->getPrimitive(0);

	hkgVertexSet* origVertexSet = faceSetIn->getVertexSet();
	int numVerts = origVertexSet ->getNumVerts();
	tempGeom->m_vertices.setSize(numVerts);
	tempGeom->m_vertDatas.setSize(numVerts);
	tempGeom->m_indices.setSize( prim->getLength() );

	HK_ASSERT(0, (origVertexSet->getVertexFormat() & HKG_VERTEX_FORMAT_POS) && 
		(origVertexSet->getVertexFormat() & HKG_VERTEX_FORMAT_TEX0) && 
		(origVertexSet->getVertexFormat() & HKG_VERTEX_FORMAT_NORMAL) );

	origVertexSet->lock(HKG_LOCK_READONLY);
	for (int i = 0; i < numVerts; ++i)
	{
		const hkReal* pos = reinterpret_cast<const hkReal*>(origVertexSet->getVertexComponentData(HKG_VERTEX_COMPONENT_POS, i) );
		tempGeom->m_vertices[i].set(pos[0], pos[1], pos[2]);

		const hkReal* a = reinterpret_cast<const hkReal*>(origVertexSet->getVertexComponentData( HKG_VERTEX_COMPONENT_TEX0, i) );
		tempGeom->m_vertDatas[i].m_u = a[0];
		tempGeom->m_vertDatas[i].m_v = a[1];

		const hkReal* n = reinterpret_cast<const hkReal*>(origVertexSet->getVertexComponentData( HKG_VERTEX_COMPONENT_NORMAL, i) );
		tempGeom->m_vertDatas[i].m_normal.set( n[0], n[1], n[2] );
	}
	origVertexSet->unlock();

	hkString::memCpy( tempGeom->m_indices.begin(), prim->getIndices(), prim->getLength() * sizeof(hkUint16) );

	

	return tempGeom;
}

hkgDisplayObject* createDisplayGeomFromTemp( hkgDisplayObject* displayObjectIn, hkTempSplitGeom* tempGeom, hkgDisplayContext* context )
{
	HK_ASSERT(0x0, tempGeom);
	hkgDisplayObject* dobj = hkgDisplayObject::create();
	if ((tempGeom->m_vertices.getSize() > 0) && (tempGeom->m_indices.getSize() > 0))
	{
		hkgGeometry* geom = hkgGeometry::create();
		geom->makeSingleFaceSet( context );
		HK_ASSERT(0x0, geom->getNumMaterialFaceSets() == 1);
		hkgMaterialFaceSet* mfs = geom->getMaterialFaceSet(0);
		HK_ASSERT(0x0, mfs->getNumFaceSets() == 1);
		hkgFaceSet* fs = mfs->getFaceSet(0);
		hkgVertexSet* newVerts = fs->getVertexSet();// makeSingleFaceSet makes one by default
		
		HKG_VERTEX_FORMAT assumedFormat = HKG_VERTEX_FORMAT_POS | HKG_VERTEX_FORMAT_TEX0 | HKG_VERTEX_FORMAT_NORMAL;
		int numVerts = tempGeom->m_vertices.getSize();
		
		// create the vertex buffers
		newVerts->setNumVerts( numVerts, assumedFormat);

		// populate the vertex buffer:
		newVerts->lock(HKG_LOCK_WRITEONLY);

		for (int vi=0; vi < numVerts; ++vi)
		{
			const hkVector4& v = tempGeom->m_vertices[vi];
			const hkTempSplitGeom::VertData& vd = tempGeom->m_vertDatas[vi];
			newVerts->setVertexComponentData( HKG_VERTEX_COMPONENT_POS, vi, &v);
			newVerts->setVertexComponentData( HKG_VERTEX_COMPONENT_NORMAL, vi, &vd.m_normal);
			newVerts->setVertexComponentData( HKG_VERTEX_COMPONENT_TEX0, vi, &vd.m_u);
		}
		newVerts->unlock();

		// populate the index buffer
		hkgFaceSetPrimitive* prim = new hkgFaceSetPrimitive(HKG_TRI_LIST);
		int numIndices = tempGeom->m_indices.getSize();
		prim->setLength( numIndices, true );
		HK_ASSERT(0x0, sizeof( unsigned short ) == sizeof( tempGeom->m_indices[0] ) );
		unsigned short* idx = prim->getIndices();
		hkString::memCpy( idx, tempGeom->m_indices.begin(), numIndices * sizeof(hkUint16)); 
		
		fs->addPrimitive( prim ); // takes owndership
		fs->initialize( HK_NULL );

		dobj->addGeometry(geom);
		geom->removeReference();

		// Set the material to be the same as the object before
		if (displayObjectIn->getNumGeometry() > 0)
		{
			HK_ASSERT(0x0, (displayObjectIn->getGeometry(0)->getNumMaterialFaceSets() > 0) && (displayObjectIn->getGeometry(0)->getMaterialFaceSet(0)->getMaterial()) );
			hkgMaterial* m = displayObjectIn->getGeometry(0)->getMaterialFaceSet(0)->getMaterial();
			mfs->setMaterial(m);
		}
	}

	return dobj;
}


hkTempSplitGeom* createTempSplitDisplayObject( hkTempSplitGeom* geomIn, const hkVector4& plane )
{
	hkSplitGeometryInput splitInput;

	if (!geomIn || (geomIn->m_vertices.getSize() < 1))
		return geomIn; // nothing to split..

	splitInput.m_vertices = (unsigned char*)geomIn->m_vertices.begin();
	splitInput.m_numVertices = geomIn->m_vertices.getSize();
	splitInput.m_stride = sizeof(hkVector4);
	splitInput.m_indexBufs.setSize(1);

	splitInput.m_indexBufs[0].m_indices = geomIn->m_indices.begin();
	splitInput.m_indexBufs[0].m_numIndices = geomIn->m_indices.getSize();
	splitInput.m_indexBufs[0].m_startOffset = 0;
	splitInput.m_indexBufs[0].m_triangleStride = 3;

	splitInput.m_plane = plane;

	hkSplitGeometryOutput output;
	
	splitGeometryWithPlane(splitInput, output);

	hkTempSplitGeom* geomOut = new hkTempSplitGeom();

	const int numVertsOut = output.m_reusedVertices.getSize() + output.m_cutVertices.getSize() * 2 + 1;
	//targetVertexSet->setNumVerts( numVertsOut, faceSetIn->getVertices()->getVertexFormat() );
	geomOut->m_vertices.setSize( numVertsOut );
	geomOut->m_vertDatas.setSize( numVertsOut );

	int i = 0;
	for ( ; i < output.m_reusedVertices.getSize(); ++i )
	{
		geomOut->m_vertices[i] = geomIn->m_vertices[ output.m_reusedVertices[i] ];
		geomOut->m_vertDatas[i] = geomIn->m_vertDatas[ output.m_reusedVertices[i] ];
	}

	hkLocalArray<hkVector4> interpolatedPositions( output.m_cutVertices.getSize() + 1 );
	interpolatedPositions.setSizeUnchecked( output.m_cutVertices.getSize() + 1 );

	hkVector4& centroid = interpolatedPositions[0]; centroid.setZero4();

	
	for ( int j = 0; j < output.m_cutVertices.getSize(); ++i, ++j)
	{
		// Interpolate the relevant values 

		hkReal aMul = output.m_cutVertices[j].m_weight;
		hkReal bMul = 1 - output.m_cutVertices[j].m_weight;

		geomOut->m_vertices[i].setInterpolate4( geomIn->m_vertices[ output.m_cutVertices[j].m_origVertA ], 
														geomIn->m_vertices[ output.m_cutVertices[j].m_origVertB ], 
														1 - output.m_cutVertices[j].m_weight ); // XXX make sure this weight is right!


		geomOut->m_vertDatas[i].m_normal.setInterpolate4( geomIn->m_vertDatas[ output.m_cutVertices[j].m_origVertA ].m_normal, 
														geomIn->m_vertDatas[ output.m_cutVertices[j].m_origVertB ].m_normal, 
														1 - output.m_cutVertices[j].m_weight );

		geomOut->m_vertDatas[i].m_u =	aMul * geomIn->m_vertDatas[ output.m_cutVertices[j].m_origVertA ].m_u + 
									bMul * geomIn->m_vertDatas[ output.m_cutVertices[j].m_origVertB ].m_u;


		geomOut->m_vertDatas[i].m_v =	aMul * geomIn->m_vertDatas[ output.m_cutVertices[j].m_origVertA ].m_v + 
									bMul * geomIn->m_vertDatas[ output.m_cutVertices[j].m_origVertB ].m_v;

		interpolatedPositions[j+1] = geomOut->m_vertices[i];
		centroid.add4(geomOut->m_vertices[i]);
	
	}

	centroid.mul4( 1.f / output.m_cutVertices.getSize() );

	//
	// Fill in info for new vertices on new surface
	//

	// This is where the new texture will be got from for the new faces. Could make this better - need to correlate with artwork.
	hkReal fakeUV[2]; fakeUV[0] = 0; fakeUV[1] = 0;

	hkVector4 normal(0,1,0);
	{
		int k = 0;
		// Normal for all triangles is the same - get first non degenerate one
		for (; k < output.m_insideFaceIndices.getSize() - 2; k += 3)
		{
			hkVector4 a; a.setSub4(interpolatedPositions[output.m_insideFaceIndices[k]], interpolatedPositions[output.m_insideFaceIndices[k + 1]]);
			hkVector4 b; b.setSub4(interpolatedPositions[output.m_insideFaceIndices[k + 2]], interpolatedPositions[output.m_insideFaceIndices[k + 1]]);
			normal.setCross(b, a);
			if ( normal.lengthSquared3() > .001f )
			{
				normal.normalize3();
				break;
			}
		}

		if (k >= output.m_insideFaceIndices.getSize() - 2)
		{
			HK_WARN_ONCE(0xf0453234, "normal broken");
		}
	}

	geomOut->m_vertices[i] = centroid;
	geomOut->m_vertDatas[i].m_normal = normal;
	geomOut->m_vertDatas[i].m_u = fakeUV[0];
	geomOut->m_vertDatas[i].m_v = fakeUV[1];
	i++;

	//
	// Re-add the boundary vertices
	//
	{
		for ( int j = 0; j < output.m_cutVertices.getSize(); ++j, ++i)
		{
			geomOut->m_vertices[i] = interpolatedPositions[j + 1];
			geomOut->m_vertDatas[i].m_normal = normal;
			geomOut->m_vertDatas[i].m_u = fakeUV[0];
			geomOut->m_vertDatas[i].m_v = fakeUV[1];
		}
	}

	// Populate index buffer
	if (output.m_outsideFaceIndices.getSize())
	{
		geomOut->m_indices.setSize(output.m_outsideFaceIndices.getSize() + output.m_insideFaceIndices.getSize());

		hkUint16* indices = geomOut->m_indices.begin();
		hkString::memCpy(indices, output.m_outsideFaceIndices.begin(), output.m_outsideFaceIndices.getSize() * sizeof (hkUint16)); 


		const hkUint16 indexBase = (hkUint16)output.m_outsideFaceIndices.getSize();
		const hkUint16 vertexBase = (hkUint16)output.m_reusedVertices.getSize() + (hkUint16)output.m_cutVertices.getSize();
		for (int idx = 0; idx < output.m_insideFaceIndices.getSize(); ++idx )
		{
			indices[idx + indexBase] = output.m_insideFaceIndices[idx] + vertexBase;
		}
	}

	return geomOut;

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
