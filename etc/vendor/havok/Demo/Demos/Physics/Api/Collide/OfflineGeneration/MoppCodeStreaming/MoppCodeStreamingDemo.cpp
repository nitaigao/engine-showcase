/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


#include <Demos/demos.h>
#include <Demos/Physics/Api/Collide/OfflineGeneration/MoppCodeStreaming/MoppCodeStreamingDemo.h>


// We will need these shapes
#include <Physics/Collide/Shape/Compound/Tree/Mopp/hkpMoppBvTreeShape.h>
#include <Physics/Collide/Shape/Compound/Collection/ExtendedMeshShape/hkpExtendedMeshShape.h>

/// Used to create the Mopp 'code' object
#include <Physics/Collide/Shape/Compound/Tree/Mopp/hkpMoppUtility.h>


// For streaming raw data, we need these platform-independent classes.
#include <Common/Base/System/Io/OArchive/hkOArchive.h>
#include <Common/Base/System/Io/IArchive/hkIArchive.h>

// This utility class helps extract the relevant data from the mopp code/write it back again.
#include <Physics/Utilities/Collide/ShapeUtils/MoppCodeStreamer/hkpMoppCodeStreamer.h>
#include <Common/Base/Algorithm/PseudoRandom/hkPseudoRandomGenerator.h>

enum { SIDE = 50 };
enum { NUM_VERTICES = SIDE * SIDE };
enum { NUM_TRIANGLES = 2 * (SIDE-1) * (SIDE-1) };


MoppCodeStreamingDemo::MoppCodeStreamingDemo(hkDemoEnvironment* env)
:	hkDefaultPhysicsDemo(env)
{
	m_vertices.setSize(NUM_VERTICES * 3);
	m_indices.setSize(NUM_TRIANGLES * 3);

	//
	// Setup the camera.
	//
	{
		hkVector4 from(0.0f, 35.0f, 60.0f);
		hkVector4 to(0.0f, 0.0f, 0.0f);
		hkVector4 up(0.0f, 1.0f, 0.0f);
		setupDefaultCameras(env, from, to, up);
	}

	//
	// Create the world.
	//
	{
		hkpWorldCinfo info;
		info.setupSolverInfo(hkpWorldCinfo::SOLVER_TYPE_4ITERS_MEDIUM); 		
		info.setBroadPhaseWorldSize( 300.0f );
		info.m_collisionTolerance = 0.1f;
		m_world = new hkpWorld(info);
		m_world->lock();

		setupGraphics();
	}

	//
	// Register the agents required 
	//
	{
		hkpAgentRegisterUtil::registerAllAgents(m_world->getCollisionDispatcher());
	}


	// This demo shows how you can implement a 'safe' mechanism to stream your MOPP data file by allowing for
	// a MOPP generation fallback method. The idea is simply to try to load the MOPP data and if, for any reason,
	// the load fails to commence correctly proceed to (re)generate the data and save it once more.

	//
	// Create MOPP body
	//
	{	
		hkpRigidBodyCinfo rigidBodyInfo;

		rigidBodyInfo.m_motionType = hkpMotion::MOTION_FIXED;

		hkString fileName( "Resources/Physics/Api/Collide/OfflineGeneration/MoppCodeStreaming/mopp.dat" );

		hkpExtendedMeshShape* meshShape = createMeshShape();
		hkpMoppCode* code = HK_NULL;

		// try to load it first
		if (1)
		{
			code = loadMoppCode( fileName.cString() );
		}

		// else build it and save it
		if ( !code )
		{
			hkpMoppCompilerInput mci;
			code = hkpMoppUtility::buildCode( meshShape , mci);
			saveMoppCode( code, fileName.cString() );
			code->removeReference();

			// Reset the error status for this demo and continue
			m_error = "";

			// Once we're sure the file is there, actually test loading it
			code = loadMoppCode( fileName.cString() );

			HK_ASSERT2(0x04534451, code, "Couldn't load the mopp file.");
		}

		hkpMoppBvTreeShape* moppShape = new hkpMoppBvTreeShape(meshShape, code);

		// The first method to be called above is createMeshShape(...) which simply generates a 'heightfield' landscape
		// using a cosine function. Note that any alterations to the default parameters will cause a slightly different
		// landscape to be generated and thus the previously saved MOPP data will be invalid. If you decide to make any changes
		// you must ensure that any streamed MOPP data files are in sync with the hkpMoppBvTreeShape you assign them to,
		// otherwise, collision detection will fail. It may be advisable to 'hash' the shape into a checksum, and write this as
		// additional data into the file to avoid any such conflicts.

		// Remove reference (mopp code will now be deleted when the hkpMoppBvTreeShape is deleted)
		code->removeReference();
		meshShape->removeReference();


		rigidBodyInfo.m_shape = moppShape;
		rigidBodyInfo.m_position.set(0.0f, 0.0f, 0.0f);

		hkpRigidBody* rigidBody = new hkpRigidBody(rigidBodyInfo);
		moppShape->removeReference();

		m_world->addEntity(rigidBody);
		rigidBody->removeReference();
	}


	// Once we have successfully loaded (or generated) our MOPP all that remains to do is to provide some good old fashioned
	// crates to throw onto the landscape. These are created in the usual manner using a hkpRigidBodyCinfo 'blueprint'
	// and are added to the world. Each box is given a random position and orientation.

	//
	// Create a box shape (used for the cubes falling on the landscape)
	//

	hkpBoxShape* shape;
	{
		hkVector4 halfExtents(1.0f, 1.0f, 1.0f);
		shape = new hkpBoxShape(halfExtents, 0 );
	}


	//
	// Create 10 randomly positioned boxes which fall on the MOPP
	//
	{
		hkReal boxMass = 1.0f;
 		hkpRigidBodyCinfo rigidBodyInfo;
		hkpMassProperties massProperties;
		hkpInertiaTensorComputer::computeBoxVolumeMassProperties( shape->getHalfExtents(), boxMass, massProperties );
		rigidBodyInfo.m_mass = massProperties.m_mass;
		rigidBodyInfo.m_inertiaTensor = massProperties.m_inertiaTensor;
		rigidBodyInfo.m_motionType = hkpMotion::MOTION_BOX_INERTIA;
		rigidBodyInfo.m_shape = shape;

		hkPseudoRandomGenerator random(10);
		const hkReal he = 10.0f;

		for( int i = 0; i < 10; i++)
		{
			// Set random position
			rigidBodyInfo.m_position.set( random.getRandRange(-he, he), random.getRandRange(10.0f, 30.0f), random.getRandRange(-he, he));

			// Set random rotation
			hkVector4 v( random.getRandRange(-1, 1), random.getRandRange(-1, 1), random.getRandRange(-1, 1));
			v.normalize3();
			hkQuaternion rot(v, random.getRandRange(0, HK_REAL_PI));
			rigidBodyInfo.m_rotation = rot;

			hkpRigidBody* rigidBody = new hkpRigidBody(rigidBodyInfo);

			m_world->addEntity(rigidBody);
			rigidBody->removeReference();
		}
	}

	shape->removeReference();

	m_world->unlock();
}

	// Create a random "landscape". If you change the parameters here the hkpMoppBvTreeShape will be
	// different, hence so will its MOPP code. It is up to you to ensure any "streamed" MOPP data
	// files are in sync with the hkpMoppBvTreeShape you assign them to, otherwise collision detection
	// will fail. It may be advisable to "hash" the shape into a checksum, and write this as additional
	// data into the file, to avoid any such conflicts.

hkpExtendedMeshShape* MoppCodeStreamingDemo::createMeshShape()
{
	const hkReal scale = 5.0f;


	//
	// Create the vertices (a "heightfield" of triangles)
	//
	{
		for(int i = 0; i < SIDE; i++)
		{
			for (int j = 0; j < SIDE; j++ )
			{
				m_vertices[(i * SIDE + j) * 3 + 0] = scale * (i * 1.0f - SIDE * 0.5f);
				m_vertices[(i * SIDE + j) * 3  + 1] = scale * (hkMath::cos((hkReal)j / (hkReal) SIDE * 10 + i / (hkReal) SIDE * 20) + 0.5f * hkMath::sin( 2.0f * i / (hkReal) SIDE * 20));
				m_vertices[(i * SIDE + j) * 3  + 2] = scale * (j * 1.0f - SIDE * 0.5f);

			}
		}
	}
	//
	// Create the triangles
	//
	{
		hkUint16 corner = 0;
		int index = 0;
		for(int i = 0; i < SIDE - 1; i++)
		{
			for (int j = 0; j < SIDE - 1; j++ )
			{
				m_indices[index]     = hkUint16(corner);
				m_indices[index + 1] = hkUint16(corner + 1);
				m_indices[index + 2] = hkUint16(corner + SIDE);

				m_indices[index + 3] = hkUint16(corner + 1);
				m_indices[index + 4] = hkUint16(corner + SIDE + 1);
				m_indices[index + 5] = hkUint16(corner + SIDE);

				index += 6;
				corner++; 
			}
			corner++; 
		}
	}


	//
	// Create the mesh shape
	//
	hkpExtendedMeshShape* meshShape = new hkpExtendedMeshShape();
	meshShape->setRadius( 0.05f );
	{
		hkpExtendedMeshShape::TrianglesSubpart part;

		part.m_vertexBase = m_vertices.begin();
		part.m_vertexStriding = sizeof(float) * 3;
		part.m_numVertices = NUM_VERTICES;

		part.m_indexBase = m_indices.begin();
		part.m_indexStriding = sizeof( hkUint16 ) * 3;
		part.m_numTriangleShapes = NUM_TRIANGLES;
		part.m_stridingType = hkpExtendedMeshShape::INDICES_INT16;

		meshShape->addTrianglesSubpart( part );
	}

	return meshShape;
}


// Following this calls are made to either loadMoppCode(...) or saveMoppCode(...) these methods are shown below:

void MoppCodeStreamingDemo::saveMoppCode( hkpMoppCode* moppCode, const char* moppFilename )
{
	// Now write out to file
	hkOArchive outputArchive(moppFilename);
	if(outputArchive.isOk())
	{
		// Write mopp data out. 
		hkpMoppCodeStreamer::writeMoppCodeToArchive(moppCode, outputArchive);
	}
}


hkpMoppCode*	MoppCodeStreamingDemo::loadMoppCode(const char* moppFilename )
{
	hkIArchive inputArchive(moppFilename);
	if(inputArchive.isOk())
	{
		// Read mopp data in. This method allocates the hkpMoppCode too. N.B. The MOPP actually holds its
		// data *immediately* after the class itself, so the number of bytes actually allocated is NOT
		// sizeof(hkpMoppCode), but rather sizeof(hkpMoppCode) + [number bytes required for byte data]
		// which is stored in the class as m_totalSize. You can query this using the getSize() method of
		// hkpMoppCode. The code initially has reference count of 1.
		hkpMoppCode* code = hkpMoppCodeStreamer::readMoppCodeFromArchive(inputArchive);			

		HK_ASSERT2(0x15d0655f, code != HK_NULL, "hkpMoppCode failed to read in correctly from Input Archive. Is the filename/path correct? Is the file corrupt?");

		return code;
	}
	else
	{
		return HK_NULL;
	}
}

// As you can see from the above code snippets both the load and save methods themselves make use of 'helper'
// methods provided by the hkpMoppCodeStreamer class, and in essence, all that you really need to stream the MOPP data is a filename!
	

#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
"This demo shows how to stream in/out MOPP data (compressed bounding volume collision data) " \
"used by hkShapeCollections (e.g. a hkpExtendedMeshShape for arbitrary triangle soups). Once data has been " \
"calculated it can be streamed out to a file, and this file can be loaded at runtime to avoid " \
"the overhead of recomputation (which can be costly if the MOPP is optimized for runtime speed " \
"of collision detection). " ;

HK_DECLARE_DEMO(MoppCodeStreamingDemo, HK_DEMO_TYPE_PRIME, "Streaming in pre-calculated MOPP (bounding volume) data.\n", helpString);

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
