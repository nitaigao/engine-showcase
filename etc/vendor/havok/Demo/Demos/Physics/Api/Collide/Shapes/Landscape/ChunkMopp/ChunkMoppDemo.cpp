/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/Physics/Api/Collide/Shapes/Landscape/ChunkMopp/ChunkMoppDemo.h>

#include <Common/Base/Container/LocalArray/hkLocalArray.h>
#include <Common/Base/Algorithm/PseudoRandom/hkPseudoRandomGenerator.h>
#include <Common/Base/Memory/Memory/hkMemory.h>

#include <Physics/Collide/Shape/Compound/Collection/SimpleMesh/hkpSimpleMeshShape.h>
#include <Physics/Collide/Shape/Compound/Collection/StorageExtendedMesh/hkpStorageExtendedMeshShape.h>
#include <Physics/Collide/Shape/Compound/Tree/Mopp/hkpMoppUtility.h>
#include <Physics/Collide/Shape/Compound/Tree/Mopp/hkpMoppCompilerInput.h>

#include <Common/Base/System/Io/IStream/hkIStream.h>
#include <Common/Serialize/Util/hkSerializeUtil.h>

#include <Demos/DemoCommon/Utilities/Asset/hkAssetManagementUtil.h> 
#include <Demos/DemoCommon/Utilities/GameUtils/GameUtils.h>
#include <Common/Visualize/hkDebugDisplay.h>

#include <Physics/Utilities/Collide/ShapeUtils/MoppCodeStreamer/hkpMoppCodeStreamer.h>
#include <Common/Base/System/Io/IArchive/hkIArchive.h>
#include <Common/Base/System/Io/OArchive/hkOArchive.h>
#include <Common/Serialize/Version/hkVersionUtil.h>

// Artwork (c)2006 Evolution Studios
#define CLIENT_LANDSCAPE "Resources/Physics/Landscapes/motorstorm.hkx"

const int VERTS_PER_SIDE = 2;

#define DISPLAY_CHUNKS 

static hkString getFilename();
static hkpShapeCollection* createMeshShape( const int side, hkArray<hkReferencedObject*>& cleanup );
void displayChunks(const hkpMoppCode* code, hkgDisplayHandler* handler, hkArray<hkReferencedObject*>& delayedCleanup );

static hkpShape* createMoppShape(hkpShapeCollection* meshShape, bool useChunks)
{
	hkpMoppCompilerInput moppInput;
	moppInput.m_enableChunkSubdivision = useChunks;

	// Build the code at runtime
	hkpMoppCode* code = hkpMoppUtility::buildCode( meshShape, moppInput );

	hkpShape* moppShape = HK_NULL;
	moppShape = new hkpMoppBvTreeShape(meshShape, code);
	
	meshShape->removeReference();
	code->removeReference();

	return moppShape;
}

ChunkMoppDemo::ChunkMoppDemo(hkDemoEnvironment* env)
	:	hkDefaultPhysicsDemo(env)
{
	//setGraphicsState(HKG_ENABLED_WIREFRAME, true);	
	//
	// Setup the camera
	//
	{
		hkVector4 from(55.0f, 50.0f, 55.0f);
		hkVector4 to  ( 0.0f,  0.0f,   0.0f);
		hkVector4 up  ( 0.0f,  1.0f,   0.0f);
		setupDefaultCameras(env, from, to, up, 0.1f, 20000.0f);
	}

	//
	// Create the world
	//
	{
		hkpWorldCinfo worldInfo;
		{
			worldInfo.m_gravity.set(0.0f, -9.81f, 0.0f);
			worldInfo.setBroadPhaseWorldSize(10000.0f);
			worldInfo.setupSolverInfo( hkpWorldCinfo::SOLVER_TYPE_4ITERS_MEDIUM );
			worldInfo.m_enableDeactivation = false;
		}
		m_world = new hkpWorld(worldInfo);
		m_world->lock();

		// Register ALL agents (though some may not be necessary)
		hkpAgentRegisterUtil::registerAllAgents(m_world->getCollisionDispatcher());

		setupGraphics();
	}

	hkpShapeCollection* meshShape = createMeshShape( VERTS_PER_SIDE, m_delayedCleanup );
	m_shape = createMoppShape( meshShape, true );
	m_shape->getAabb( hkTransform::getIdentity(), 0.1f, m_bounds );

	//
	//  create the ground mopp
	//
	{

		hkpRigidBodyCinfo terrainInfo;
		{
			hkVector4 size(100.0f, 1.0f, 100.0f);

			terrainInfo.m_shape = m_shape;
			terrainInfo.m_motionType = hkpMotion::MOTION_FIXED;
			terrainInfo.m_friction = 0.5f;

			hkpRigidBody* terrainBody = new hkpRigidBody(terrainInfo);
			m_world->addEntity(terrainBody);
			terrainBody->removeReference();	
		}
		terrainInfo.m_shape->removeReference();
	}

	hkVector4 halfExtents;
	hkVector4 centre;
	{
		halfExtents.setSub4( m_bounds.m_max, m_bounds.m_min );
		halfExtents.mul4( 0.5f );
		centre.setAdd4( m_bounds.m_max, m_bounds.m_min );
		centre.mul4( 0.5f );

		m_minIndex = (halfExtents(0) < halfExtents(1)) ? 
			((halfExtents(0) < halfExtents(2)) ? 0 : 2) :
			((halfExtents(1) < halfExtents(2)) ? 1 : 2) ;
	}

	// Setup the camera
	{
		hkVector4 up; up.setZero4();
		up(m_minIndex) = 1.0f;

		hkVector4 from; from.setAddMul4( m_bounds.m_max, up, 5.0f);
		setupDefaultCameras(env, from, centre, up, 0.1f, 10000.0f);
	}

	//
	// Create objects at random coordinates
	//
	{
		hkVector4 size(0.5f, 0.5f, 0.5f);
		hkpBoxShape* boxShape =  new hkpBoxShape(size);

		hkpRigidBodyCinfo boxInfo;
		{
			boxInfo.m_mass = 1.0f;
			boxInfo.m_shape = boxShape;
			boxInfo.m_motionType = hkpMotion::MOTION_BOX_INERTIA;
		}

		hkPseudoRandomGenerator random(15);

		//
		// create the boxes
		//
		{
			hkVector4 range; range.setSub4( m_bounds.m_max, m_bounds.m_min );
			range.mul4( 1.0f );
			
			const int numObjects = 500;
			for (int i = 0; i < numObjects; i++)
			{
				hkVector4 pos; random.getRandomVector11(pos);
				pos.mul4(0.1f);
				pos.mul4(range);
				pos( m_minIndex ) += m_bounds.m_max( m_minIndex ) + i * (range(m_minIndex)) / numObjects + 4.0f;

				boxInfo.m_position = pos;
				hkpRigidBody* shape;
				shape = new hkpRigidBody(boxInfo);
				
				shape->setMaxLinearVelocity( 30 );
				m_world->addEntity(shape);
				shape->removeReference();
			}
		}

		boxShape->removeReference();
	}

	m_world->unlock();

	m_time = 0.0f;
}

static hkpShapeCollection* createMeshShape( const int side, hkArray<hkReferencedObject*>& delayedCleanup )
{
#ifdef CLIENT_LANDSCAPE

	hkString assetFile = hkAssetManagementUtil::getFilePath(CLIENT_LANDSCAPE);
	hkIfstream fileIn(assetFile.cString());
	hkResource* data = hkSerializeUtil::load(assetFile.cString());
	delayedCleanup.pushBack( data );
	hkpSimpleMeshShape* meshShape = data->getContents<hkpSimpleMeshShape>();

	hkpStorageExtendedMeshShape* extendedMesh = new hkpStorageExtendedMeshShape();

	hkpExtendedMeshShape::TrianglesSubpart part;
	part.m_numTriangleShapes = meshShape->m_triangles.getSize();
	part.m_numVertices = meshShape->m_vertices.getSize();
	part.m_vertexBase = (float*)meshShape->m_vertices.begin();
	part.m_stridingType = hkpExtendedMeshShape::INDICES_INT32;
	part.m_vertexStriding = sizeof(hkVector4);
	part.m_indexBase = meshShape->m_triangles.begin();
	part.m_indexStriding = sizeof(hkpSimpleMeshShape::Triangle);
	extendedMesh->addTrianglesSubpart( part );

	return extendedMesh;
#else

	hkpSimpleMeshShape* meshShape = new hkpSimpleMeshShape( 0.05f /*radius*/);

	hkReal scaleHoriz = 400.0f / side;
	hkReal scaleVert = 1.0f;
	{
		meshShape->m_vertices.setSize( side * side );
		for(int i = 0; i < side; i++)
		{
			for (int j = 0; j < side; j++ )
			{
				hkVector4 vertex ( 
					scaleHoriz * (i * 1.0f - (side-1) * 0.5f),
					scaleVert * (0.6f * hkMath::cos((hkReal)j + i) + 0.3f * hkMath::sin( 2.0f * i) ),
					scaleHoriz * (j * 1.0f - (side-1) * 0.5f));
				meshShape->m_vertices[i*side + j] = vertex ;
			}
		}
	}

	{
		meshShape->m_triangles.setSize( (side-1) * (side-1) * 2);
		int corner = 0;
		int curTri = 0;
		for(int i = 0; i < side - 1; i++)
		{
			for (int j = 0; j < side - 1; j++ )
			{
				meshShape->m_triangles[curTri].m_a = corner+1;
				meshShape->m_triangles[curTri].m_b = corner+side;
				meshShape->m_triangles[curTri].m_c = corner;
				curTri++;

				meshShape->m_triangles[curTri].m_a = corner+side+1;
				meshShape->m_triangles[curTri].m_b = corner+side;
				meshShape->m_triangles[curTri].m_c = corner+1;
				curTri++;
				corner++; 
			}
			corner++; 
		}
	}

	return meshShape;
#endif

}

#include <Common/Visualize/Shape/hkDisplayGeometry.h>
#include <Common/Visualize/Shape/hkDisplayConvex.h>
#include <Physics/Utilities/VisualDebugger/Viewer/hkpShapeDisplayBuilder.h>
#include <Physics/Collide/Shape/Compound/Collection/ExtendedMeshShape/hkpExtendedMeshShape.h>
#include <Common/Base/Types/Geometry/hkGeometry.h>
#include <Graphics/Common/DisplayObject/hkgDisplayObject.h>
#include <Graphics/Common/Geometry/hkgMaterialFaceSet.h>
#include <Graphics/Common/Geometry/FaceSet/hkgFaceSet.h>
#include <Graphics/Common/Geometry/VertexSet/hkgVertexSet.h>
#include <Physics/Collide/Util/hkpTriangleCompressor.h>
#include <Graphics/Bridge/DisplayHandler/hkgDisplayHandler.h>

void displayChunks(const hkpMoppCode* code, hkgDisplayHandler* handler, hkArray<hkReferencedObject*>& delayedCleanup )
{
	const int numChunks = code->getCodeSize() / HK_MOPP_CHUNK_SIZE;

	hkGeometry* geom = new hkGeometry();
	for (int chunkId = 0; chunkId < numChunks; ++chunkId)
	{
		const unsigned char *offsetCmd = code->m_data.begin() + ( HK_MOPP_CHUNK_SIZE * chunkId );
//		HK_ASSERT2( 0x43234564, *offsetCmd == HK_MOPP_DATA_OFFSET, "Can't find offset command");
		int offsetIntoChunk = (offsetCmd[1] << 8) | offsetCmd[2];
		int numTerminalsInChunk = (offsetCmd[3] << 8) | offsetCmd[4];

		for (int terminalId=0; terminalId < numTerminalsInChunk; terminalId++)
		{
			hkpShapeCollection::ShapeBuffer buffer;

			// Decompress into the buffer
			const void* data =  offsetCmd + offsetIntoChunk;

			hkpTriangleShape* triangleShape = new(buffer)hkpTriangleShape; 
			hkpTriangleCompressor::getTriangleShape( *triangleShape, terminalId, data );

			hkGeometry::Triangle triangle; 
			triangle.m_a = geom->m_vertices.getSize();
			triangle.m_b = geom->m_vertices.getSize() + 1;
			triangle.m_c = geom->m_vertices.getSize() + 2;

			geom->m_vertices.pushBack(triangleShape->getVertex(0));
			geom->m_vertices.pushBack(triangleShape->getVertex(1));
			geom->m_vertices.pushBack(triangleShape->getVertex(2));

			geom->m_triangles.pushBack(triangle);
		}
	}

	hkDisplayGeometry* displayGeom = new hkDisplayConvex( geom );
	delayedCleanup.pushBack(displayGeom);
	hkArray<hkDisplayGeometry*> displayGeometries( &displayGeom, 1, 1 );
	handler->addGeometry( displayGeometries, hkTransform::getIdentity(), 12345, 0, 0);

	hkgDisplayObject* obj = handler->findDisplayObject( 12345 );
	hkgVertexSet* verts = obj->getGeometry(0)->getMaterialFaceSet(0)->getFaceSet(0)->getVertexSet();
	verts->lock(HKG_LOCK_READONLY);

	// Copy vertices
	hkgFaceSet* faceSet = hkgFaceSet::create( handler->getContext()  );
	hkgVertexSet* copyVerts = hkgVertexSet::create( handler->getContext() );
	faceSet->setVertexSet(copyVerts);
	copyVerts->removeReference();

	{
		copyVerts->setNumVerts( verts->getNumVerts(), verts->getVertexFormat());
		copyVerts->lock(HKG_LOCK_WRITEDISCARD);
		for (int i=0; i < verts->getNumVerts(); i++)
		{
			copyVerts->copyExistingVertex(i,i,verts);
		}
	}

	verts->unlock();
	verts->setNumVerts( verts->getNumVerts(), verts->getVertexFormat() | HKG_VERTEX_FORMAT_COLOR);
	verts->lock(HKG_LOCK_WRITEDISCARD);

	int vertIdx = 0;
	for (int chunkId = 0; chunkId < numChunks; ++chunkId)
	{
		const unsigned char *offsetCmd = code->m_data.begin() + ( HK_MOPP_CHUNK_SIZE * chunkId );
//		HK_ASSERT2( 0x43234564, *offsetCmd == HK_MOPP_DATA_OFFSET, "Can't find offset command");
		int numTerminalsInChunk = (offsetCmd[3] << 8) | offsetCmd[4];
		int col = hkColor::getRandomColor();

		for (int terminalId=0; terminalId < numTerminalsInChunk; terminalId++)
		{

			verts->copyExistingVertex( vertIdx, vertIdx, copyVerts);
			verts->setVertexComponentData( HKG_VERTEX_COMPONENT_COLOR, vertIdx, &col);
			vertIdx++;
			verts->copyExistingVertex( vertIdx, vertIdx, copyVerts);
			verts->setVertexComponentData( HKG_VERTEX_COMPONENT_COLOR, vertIdx, &col);
			vertIdx++;
			verts->copyExistingVertex( vertIdx, vertIdx, copyVerts);
			verts->setVertexComponentData( HKG_VERTEX_COMPONENT_COLOR, vertIdx, &col);
			vertIdx++;
		}
	}

	copyVerts->unlock();
	verts->unlock();

	//Destroy tmp data
	faceSet->removeReference();
}

static hkString getFilename()
{
	// Load from file if it exists
#ifdef CLIENT_LANDSCAPE
	hkString filename(CLIENT_LANDSCAPE);
#else
	hkString filename;
	filename.printf( "node_%d", VERTS_PER_SIDE );
#endif
	filename = filename.substr( filename.lastIndexOf('/')+1 );
	filename += "_mopp.bin";

	return filename;
}



#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = "MOPP split into chunks. Artwork (c)2006 Evolution Studios";
HK_DECLARE_DEMO(ChunkMoppDemo, HK_DEMO_TYPE_OTHER|HK_DEMO_TYPE_SERIALIZE, "Mopp with Extended Mesh", helpString ); 

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
