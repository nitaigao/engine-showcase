/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/Physics/Test/Performance/BenchmarkSuite/BenchmarkSuiteDemo.h>

#include <Physics/Collide/Shape/Compound/Tree/Mopp/hkpMoppBvTreeShape.h>
#include <Physics/Collide/Shape/Compound/Collection/SimpleMesh/hkpSimpleMeshShape.h>
#include <Physics/Collide/Shape/Compound/Collection/List/hkpListShape.h>
#include <Physics/Collide/Shape/Compound/Collection/StorageExtendedMesh/hkpStorageExtendedMeshShape.h>

#include <Physics/Collide/Shape/Compound/Tree/Mopp/hkpMoppUtility.h>
#include <Physics/Collide/Shape/Compound/Tree/Mopp/hkpMoppCompilerInput.h>

// To enable instancing in shape viewer
#include <Physics/Utilities/VisualDebugger/Viewer/Collide/hkpShapeDisplayViewer.h>

#include <Common/Base/Memory/Memory/hkMemory.h>

#include <Physics/Collide/Util/hkpTriangleCompressor.h>

#include <Common/Base/Algorithm/PseudoRandom/hkPseudoRandomGenerator.h>
#include <Demos/DemoCommon/Utilities/GameUtils/GameUtils.h>

enum VariantType
{
	TYPE_10x10x1_ON_BOX,
	TYPE_5x5x5_ON_BOX,
	TYPE_300_BODY_PILE,
	TYPE_OBJECTS_ON_MOPP_SMALL,
	TYPE_LIST_PILE_SMALL,
	TYPE_30x30x1_ON_BOX,
	TYPE_12x12x10_ON_BOX,
	TYPE_20x20x5_ON_BOX,
	TYPE_3000_BODY_PILE,
	TYPE_OBJECTS_ON_MOPP_LARGE,
	TYPE_LIST_PILE_LARGE,

};

struct BenchmarkSuiteVariant
{
	const char*	 m_name;
	VariantType  m_type;
	const char* m_details;
};


static const BenchmarkSuiteVariant g_variants[] =
{
	{ "1 Cube Pyramid of 10 Rows", TYPE_10x10x1_ON_BOX, "A single standing pyramid all objects in one island"},
	{ "5 Cube Pyramids of 10 Rows", TYPE_5x5x5_ON_BOX, "5 pyramids allow for better multithreading" },
	{ "1 Cube Pyramid of 30 Rows", TYPE_30x30x1_ON_BOX, "A single standing pyramid all objects in one island"},
	{ "5 Cube Pyramids of 20 Rows", TYPE_20x20x5_ON_BOX, "5 pyramids allow for better multithreading" },
	{ "10 Cube Pyramids of 12 Rows", TYPE_12x12x10_ON_BOX, "A default demo" },
	{ "64 Columns of 5 Cubes", TYPE_300_BODY_PILE, "The worst case scenario for a physics engine" },
	{ "64 Columns of 50 Cubes", TYPE_3000_BODY_PILE, "The worst case scenario for a physics engine" },
	{ "144 Cubes on 500-Triangle MOPP", TYPE_OBJECTS_ON_MOPP_SMALL, "Testing landscape performance" },
	{ "2000 Cubes on 8000-Triangle MOPP", TYPE_OBJECTS_ON_MOPP_LARGE, "Testing landscape performance" },
	{ "81 3-Piece ListShapes on 500-Triangle MOPP", TYPE_LIST_PILE_SMALL, "Testing list performance" },
	{ "125 5-Piece ListShapes on 500-Triangle MOPP", TYPE_LIST_PILE_LARGE, "Testing list performance" },
};

static hkpShape* createMoppShape(hkpShapeCollection* meshShape)
{
	hkpMoppCompilerInput moppInput;
	moppInput.m_enableChunkSubdivision = true;

	hkpMoppCode* code = hkpMoppUtility::buildCode( meshShape, moppInput );
	hkpShape* moppShape = new hkpMoppBvTreeShape(meshShape, code);

	code->removeReference();

	return moppShape;
}

static void CreateGroundPlane( hkpWorld* world )
{
	hkVector4 baseSize( 135.0f, 0.5f, 135.0f);
	hkpConvexShape* shape = new hkpBoxShape( baseSize , 0 );

	hkpRigidBodyCinfo ci;

	ci.m_shape = shape;
	ci.m_restitution = 0.5f;
	ci.m_friction = 0.3f;
	ci.m_position.set( 0.0f, -0.5f, 0.0f );
	ci.m_motionType = hkpMotion::MOTION_FIXED;

	world->addEntity( new hkpRigidBody( ci ) )->removeReference();
	shape->removeReference();
}

static void CreateStack( hkpWorld* world, int size, float zPos = 0.0f )
{
	const hkReal cubeSize  = 1.0f;	// This is the size of the cube side of the box
	const hkReal boxRadius = cubeSize * 0.01f;
	const hkReal gapx    = cubeSize * 0.05f;		// This is the gap between boxes
	const hkReal gapy    = boxRadius;
	const hkReal heightOffGround = 0.0f;	// This is the height of the BenchmarkSuite off the gound

	hkReal extendedBoxDimX = cubeSize + gapx;
	hkReal extendedBoxDimY = cubeSize + gapy;


	hkVector4 startPos( 0.0f , heightOffGround + gapy + cubeSize * 0.5f, zPos);
	// Build BenchmarkSuite
	{
		hkVector4 boxRadii(cubeSize *.5f, cubeSize *.5f, cubeSize *.5f);

		hkpShape* boxShape = new hkpBoxShape( boxRadii , boxRadius );

		for(int i=0; i<size; i++)
		{
			// This constructs a row, from left to right
			int rowSize = size - i;
			hkVector4 start(-rowSize * extendedBoxDimX * 0.5f + extendedBoxDimX * 0.5f, i * extendedBoxDimY, 0);
			for(int j=0; j< rowSize; j++)
			{
				hkVector4 boxPos(start);
				hkVector4 shift(j * extendedBoxDimX, 0.0f, 0.0f);
				boxPos.setAdd4(boxPos, shift);
				boxPos.setAdd4(boxPos, startPos);

				///
				hkpRigidBodyCinfo boxInfo;

				boxInfo.m_mass = 100.0f;
				// calculate the correct inertia
				hkReal d = boxInfo.m_mass * cubeSize * cubeSize / 6.0f;

				// for small boxes increase inertia slightly
				if ( boxRadius < 0.1f )
				{
					d *= 2.0f;
					if ( boxRadius < 0.03f )
					{
						d *= 2.0f;
					}
				}
				boxInfo.m_inertiaTensor.setDiagonal(d,d,d);

				boxInfo.m_shape = boxShape;
				boxInfo.m_motionType = hkpMotion::MOTION_DYNAMIC;
				boxInfo.m_position = boxPos;
				boxInfo.m_restitution = 0.5f;
				boxInfo.m_friction = 0.6f;
				boxInfo.m_solverDeactivation = hkpRigidBodyCinfo::SOLVER_DEACTIVATION_MAX;
				///>

				hkpRigidBody* boxRigidBody = new hkpRigidBody(boxInfo);

				// Now add to world. Body is "ready to go" as soon as this is called, and display
				// is (as a registered listener) automatically notified to build a new display object.
				world->addEntity( boxRigidBody );
				boxRigidBody->removeReference();	// Remove reference, since we no longer want to remember this
			}
		}
		boxShape->removeReference();
	}
}
static void CreateFall( hkpWorld* world, int height )
{
	hkReal boxDim = 1.0f;
	hkReal boxRadius = 0.01f;
	hkVector4 boxRadii(boxDim *.5f, boxDim *.5f - boxRadius, boxDim *.5f);
	hkpShape* boxShape = new hkpBoxShape( boxRadii, boxRadius );

	// 64 columns of cubes, of height 'height'
	for (int x = -4; x < 4; x++)
	{
		for (int y = -4; y < 4; y++)
		{
			for (int z = 0; z < height; z++)
			{
				hkpRigidBodyCinfo boxInfo;

				boxInfo.m_mass = 100.0f;
				hkReal d = boxInfo.m_mass * boxDim * boxDim / 6.0f;
				boxInfo.m_inertiaTensor.setDiagonal(d,d,d);

				boxInfo.m_shape = boxShape;
				boxInfo.m_motionType = hkpMotion::MOTION_DYNAMIC;
				boxInfo.m_position.set( x* 3.0f, z * 1.0f, y * 3.0f);
				boxInfo.m_restitution = 0.0f;
				boxInfo.m_angularDamping = 1.0f;
				boxInfo.m_linearDamping = .0f;
				boxInfo.m_friction = 1.0f;
				boxInfo.m_solverDeactivation = hkpRigidBodyCinfo::SOLVER_DEACTIVATION_MAX;

				hkpRigidBody* boxRigidBody = new hkpRigidBody(boxInfo);
				world->addEntity( boxRigidBody );
				boxRigidBody->removeReference();	// Remove reference, since we no long
			}
		}
	}

	boxShape->removeReference();
}

static void CreateFlatCubeGrid( hkpWorld* world, int size, float height = 0 )
{
	hkReal boxDim = 1.0f;
	float delta = boxDim * 3.0f;

	hkReal boxRadius = 0.01f;
	hkVector4 boxRadii(boxDim, boxDim, boxDim);
	hkpShape* boxShape = new hkpBoxShape( boxRadii, boxRadius );

	// flat cube grid, made up of (1.5*size)*(1.5*size) cubes
	for (int x = -size/2; x < size; x++)
	{
		for (int y = -size/2; y < size; y++)
		{
			hkpRigidBodyCinfo boxInfo;

			boxInfo.m_mass = 100.0f;
			hkReal d = boxInfo.m_mass * boxDim * boxDim / 6.0f;
			boxInfo.m_inertiaTensor.setDiagonal(d,d,d);

			boxInfo.m_shape = boxShape;
			boxInfo.m_motionType = hkpMotion::MOTION_DYNAMIC;
			boxInfo.m_position.set( x * delta, boxDim + height, y * delta);
			boxInfo.m_restitution = 0.0f;
			boxInfo.m_solverDeactivation = hkpRigidBodyCinfo::SOLVER_DEACTIVATION_MAX;

			hkpRigidBody* boxRigidBody = new hkpRigidBody(boxInfo);
			world->addEntity( boxRigidBody );
			boxRigidBody->removeReference();	// Remove reference, since we no long
		}
	}
	boxShape->removeReference();
}

static void CreateListGrid( hkpWorld* world, hkReal bound, int numPerCubeSide, int numPerList )
{
	// Add List Shapes
	{
		hkPseudoRandomGenerator gen(123);

		for (int i = 0; i < numPerCubeSide; i++)
		{
			for (int j = 0; j < numPerCubeSide; j++)
			{
				for (int k = 0; k < numPerCubeSide; k++)
				{
					hkArray<hkpShape*> listElems;
					for (int e = 0; e < numPerList; e++)
					{
						// Make a random convex vertex shape
						hkVector4 cen; gen.getRandomVector11( cen ); cen.mul4( bound / (numPerCubeSide * 4.0f) );
						hkVector4 extent; extent.setAll3( bound /  (numPerCubeSide * 4) );
						listElems.pushBack( GameUtils::createConvexVerticesBoxShape( cen, extent, hkConvexShapeDefaultRadius ) );
					}

					hkpShape* listShape = new hkpListShape( listElems.begin(), listElems.getSize() );
					for (int s=0; s < listElems.getSize(); s++)
					{
						listElems[s]->removeReference();
					}

					hkpRigidBodyCinfo ci;
					ci.m_motionType = hkpMotion::MOTION_BOX_INERTIA;
					ci.m_shape = listShape;
					ci.m_mass = 1.f;

					ci.m_position.set( hkReal(i - numPerCubeSide / 2), hkReal(j + 1), hkReal(k - numPerCubeSide / 2) );
					ci.m_position.mul4( bound / numPerCubeSide );

					hkpRigidBody* body = new hkpRigidBody( ci );
					world->addEntity(body);
					body->removeReference();
					listShape->removeReference();
				}
			}
		}
	}
}

void BenchmarkSuiteDemo::CreatePhysicsTerrain( hkpWorld* world, const int side, float deltaHeight, float triangleEdgeLen )
{
	hkpSimpleMeshShape* meshShape = new hkpSimpleMeshShape( 0.05f /*radius*/);

	{
		meshShape->m_vertices.setSize( side * side );
		for(int x = 0; x < side; x++)
		{
			for (int y = 0; y < side; y++ )
			{
				float height = 0.0f;
				if ( (x&1) && (y&1) )
				{
					height = -deltaHeight;
				}

				hkVector4 vertex (
					triangleEdgeLen * (x * 1.0f - (side-1) * 0.5f),
					triangleEdgeLen * height,
					triangleEdgeLen * (y * 1.0f - (side-1) * 0.5f));
				meshShape->m_vertices[x*side + y] = vertex ;
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

	// No longer need the simple mesh
	meshShape->removeReference();

	hkpRigidBodyCinfo ci;

	hkpShape* moppShape = createMoppShape( extendedMesh );
	extendedMesh->removeReference();

	ci.m_shape = moppShape;
	ci.m_restitution = 0.5f;
	ci.m_friction = 0.3f;
	ci.m_position.set( 0.0f, 0.0f, 0.0f );
	ci.m_motionType = hkpMotion::MOTION_FIXED;

	world->addEntity( new hkpRigidBody( ci ) )->removeReference();
	moppShape->removeReference();
}



BenchmarkSuiteDemo::BenchmarkSuiteDemo(hkDemoEnvironment* env)
	:	hkDefaultPhysicsDemo(env)
{
	const BenchmarkSuiteVariant& variant =  g_variants[m_variantId];

	// Disable warnings:									
	hkError::getInstance().setEnabled(0x7dd65995, false); //'The system has requested a heap allocation on stack overflow.'
	hkError::getInstance().setEnabled(0xaf55adde, false); //'No runtime block of size 637136 currently available. Allocating new block from unmanaged memory.'


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
			worldInfo.m_gravity.set(-0.0f, -9.81f, -0.0f);
			worldInfo.setBroadPhaseWorldSize(500.0f);
			worldInfo.setupSolverInfo( hkpWorldCinfo::SOLVER_TYPE_4ITERS_MEDIUM );

			if (variant.m_type == TYPE_3000_BODY_PILE)
			{
				worldInfo.m_enableSimulationIslands = false;
			}
			worldInfo.m_enableDeactivation = false;
		}
		m_world = new hkpWorld(worldInfo);
		m_world->lock();

		// Register ALL agents (though some may not be necessary)
		hkpAgentRegisterUtil::registerAllAgents(m_world->getCollisionDispatcher());

		setupGraphics();

		// enable instancing (if present on platform). Call this after setup graphics (as it makes the local viewers..)
		hkpShapeDisplayViewer* shapeViewer = (hkpShapeDisplayViewer*)getLocalViewerByName( hkpShapeDisplayViewer::getName() );
		if (shapeViewer)
		{
			shapeViewer->setInstancingEnabled( true );
		}
	}

	//
	// Setup the camera
	//
	{
		hkVector4 from(15.0f, 15.0f, 15.0f);
		hkVector4 to  (0.0f, 0.0f, 0.0f);
		hkVector4 up  (0.0f, 1.0f, 0.0f);
		setupDefaultCameras( env, from, to, up );
	}




	switch (variant.m_type)
	{
	case TYPE_10x10x1_ON_BOX:
		{
			CreateGroundPlane( m_world );
			CreateStack(m_world, 10);
			break;
		}
	case TYPE_5x5x5_ON_BOX:
		{
			CreateGroundPlane(m_world);
			for (int q = -3; q <= 2; q++  )
			{
				CreateStack(m_world,5, q * 10.0f);
			}
			break;
		}
	case TYPE_300_BODY_PILE:
		{
			CreateGroundPlane(m_world);
			CreateFall(m_world, 5);
			break;
		}
	case TYPE_OBJECTS_ON_MOPP_SMALL:
		{
			CreatePhysicsTerrain(m_world, 16, 1.0f);
			CreateFlatCubeGrid(m_world,8);
			break;
		}
	case TYPE_LIST_PILE_SMALL:
		{
			int side = 16;
			CreatePhysicsTerrain(m_world, side, 1.0f);
			CreateListGrid(m_world, hkReal(side), 3, 3);
			break;
		}
	case TYPE_30x30x1_ON_BOX:
		{
			CreateGroundPlane( m_world );
			CreateStack(m_world, 30);
			break;
		}
	case TYPE_20x20x5_ON_BOX:
		{
			CreateGroundPlane(m_world);
			for (int q = -3; q <= 2; q++  )
			{
				CreateStack(m_world,20, q * 10.0f);
			}
			break;
		}
	case TYPE_12x12x10_ON_BOX:
		{
			CreateGroundPlane(m_world);
			for (int q = -5; q <= 4; q++  )
			{
				CreateStack(m_world,12, q * 2.5f);
			}
			break;
		}
	case TYPE_3000_BODY_PILE:
		{
			CreateGroundPlane(m_world);
			CreateFall(m_world, 50);
			break;
		}
	case TYPE_OBJECTS_ON_MOPP_LARGE:
		{
			CreatePhysicsTerrain(m_world, 64, 1.0f);
			CreateFlatCubeGrid(m_world,30);
			break;
		}
	case TYPE_LIST_PILE_LARGE:
		{
			int side = 16;
			CreatePhysicsTerrain(m_world, side, 1.0f);
			CreateListGrid(m_world, hkReal(side), 5, 5);
			break;
		}
	default:
		{
			CreateGroundPlane(m_world);
			CreateStack(m_world, 20);
			break;
		}
	}

	//
	//	Some values
	//

	m_world->unlock();
}

BenchmarkSuiteDemo::~BenchmarkSuiteDemo()
{
	// Re-enable warnings:									
	hkError::getInstance().setEnabled(0x7dd65995, true); 
	hkError::getInstance().setEnabled(0xaf55adde, true); 	
}

#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = "A different set of simple benchmarks.";

HK_DECLARE_DEMO_VARIANT_USING_STRUCT( BenchmarkSuiteDemo, HK_DEMO_TYPE_PRIME | HK_DEMO_TYPE_STATS, BenchmarkSuiteVariant, g_variants, helpString );


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
