/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/Physics/Api/Collide/OfflineGeneration/SerializeMopp/SerializeMoppDemo.h>

// We will need these shapes.
#include <Physics/Collide/Shape/Deprecated/Mesh/hkpMeshShape.h>
#include <Physics/Collide/Shape/Compound/Tree/Mopp/hkpMoppBvTreeShape.h>

// Used to create the Mopp 'code' object
#include <Physics/Collide/Shape/Compound/Tree/Mopp/hkpMoppUtility.h>

// Used for loading/saving
#include <Common/Serialize/Packfile/Binary/hkBinaryPackfileReader.h>
#include <Common/Serialize/Packfile/Binary/hkBinaryPackfileWriter.h>
#include <Physics/Utilities/Serialize/hkpHavokSnapshot.h>
#include <Common/Base/System/Io/Writer/Array/hkArrayStreamWriter.h>
#include <Common/Base/System/Io/Reader/Memory/hkMemoryStreamReader.h>
#include <Common/Base/Reflection/Registry/hkVtableClassRegistry.h>
#include <Common/Base/Reflection/Registry/hkTypeInfoRegistry.h>
#include <Common/Base/System/Stopwatch/hkStopwatch.h>
#include <Demos/DemoCommon/DemoFramework/hkTextDisplay.h>

// demo pad handling
#include <Graphics/Common/Input/Pad/hkgPad.h>
#include <Graphics/Common/Font/hkgFont.h>

SerializeMoppDemo::SerializeMoppDemo(hkDemoEnvironment* env)
:	hkDefaultPhysicsDemo(env), m_body(HK_NULL)
{
	// Setup the camera.
	{
		hkVector4 from(-75.0f, -75.0f, 75.0f);
		hkVector4 to  (0.0f, 0.0f,  0.0f);
		hkVector4 up  (0.0f, 1.0f,  0.0f);
		setupDefaultCameras(env, from, to, up);
	}

	// Create the world.
	{
		hkpWorldCinfo info;
		
		// Set gravity to zero so body floats.
		info.m_gravity.set(0.0f, 0.0f, 0.0f);	
		info.setBroadPhaseWorldSize( 200.0f );
		m_world = new hkpWorld(info);
		m_world->lock();

		setupGraphics();
	}

	

	// Create the shape and a rigid body to view it.
	// The shape we create here will be a mesh shape
	// and will have (MOPP-compressed) bounding volume
	// information added to it by wrapping it in a hkpMoppBvTreeShape.
	{

		hkpExtendedMeshShape* mesh = new hkpExtendedMeshShape();
		hkpExtendedMeshShape::TrianglesSubpart part;
		setupMeshPart(part);
		mesh->addTrianglesSubpart(part);

		/////////////////// SHAPE CONSTRUCTION ////////////////
		hkpMoppCompilerInput moppInput;
		moppInput.setAbsoluteFitToleranceOfTriangles(0.1f);
		hkpMoppCode* code = hkpMoppUtility::buildCode(mesh, moppInput);		

		hkpMoppBvTreeShape* shape = new hkpMoppBvTreeShape(mesh, code);
		// Remove references since the MoppBvTreeShape now "owns" the listShape and code
		code->removeReference();
		mesh->removeReference();
		///////////////////////////////////////////////////////


		// To illustrate using the shape, create a rigid body by first defining a template.
		hkpRigidBodyCinfo rigidBodyInfo;

		rigidBodyInfo.m_shape = shape;
		rigidBodyInfo.m_position.set(0.0f, 0.0f, 0.0f);
		// If we set this to true, the body is fixed, and no mass properties need to be computed.
		rigidBodyInfo.m_motionType = hkpMotion::MOTION_FIXED;
		// Create a rigid body (using the template above).
		m_body = new hkpRigidBody(rigidBodyInfo);

		// Remove references since the body now "owns" the Shapes.
		shape->removeReference();

		// Finally add body so we can see it.
		m_world->addEntity(m_body);
		// we keep a reference to the body.
		//m_body->removeReference();
	}

	m_world->unlock();
}

SerializeMoppDemo::~SerializeMoppDemo()
{
	m_world->lock();

	// We clean up the memory since it is not hkAllocated.
	m_world->removeEntity(m_body);
	m_body->removeReference();

	m_world->unlock();
}

void SerializeMoppDemo::setupMeshPart( hkpExtendedMeshShape::TrianglesSubpart& part )
{
	part.m_stridingType = hkpExtendedMeshShape::INDICES_INT32;
	part.m_indexBase = meshIndices;
	part.m_indexStriding = 3*sizeof(int);
	part.m_numTriangleShapes = NUM_TRIS;
	part.m_vertexBase = meshVertices;
	part.m_vertexStriding = 3*sizeof(hkReal);
	part.m_numVertices = NUM_VERTS;
}

hkDemo::Result SerializeMoppDemo::stepDemo()
{
	Result r = hkDefaultPhysicsDemo::stepDemo();
	if( r != DEMO_OK )
	{
		return r;
	}
	bool saveUsingSnapshot = m_env->m_gamePad->wasButtonPressed( HKG_PAD_BUTTON_1 );
	bool reconnectMeshByHand = m_env->m_gamePad->wasButtonPressed( HKG_PAD_BUTTON_2 );

	if( reconnectMeshByHand || saveUsingSnapshot )
	{
		m_world->lock();
		m_world->removeEntity(m_body);
		m_world->unlock();

		hkStopwatch saveTime;		

		// save everything into "buf"
		{
			hkArray<char> tmpBuf; // avoid overwriting our buffer until done
			tmpBuf.reserve( m_buffer.getSize() );

			saveTime.start();
			
			hkArrayStreamWriter streamOut(&tmpBuf, hkArrayStreamWriter::ARRAY_BORROW);

			if( saveUsingSnapshot )
			{
				// We use havok snapshot because that has the logic for making
				// mesh shapes serializable built in.
				hkpHavokSnapshot::save( m_body, hkpRigidBodyClass, &streamOut );
			}
			else // reconnectMeshByHand // we want to reconnect the mesh ourselves.
			{
				hkBinaryPackfileWriter writer;
				writer.setContents( m_body, hkpRigidBodyClass );
				hkPackfileWriter::Options options;
				writer.save( &streamOut, options);
			}
			
			m_body->removeReference();
			m_body = HK_NULL;

			m_buffer.swap(tmpBuf);
			saveTime.stop();
		}

		hkStopwatch loadTime;

		// load from buffer
		{
			loadTime.start();
			hkBinaryPackfileReader reader;
			if( reader.loadEntireFileInplace( m_buffer.begin(), m_buffer.getSize() ) != HK_SUCCESS)
			{
				HK_WARN(0x5ef4a322, "Unable to load");
				return DEMO_ERROR;
			}

			m_body = (hkpRigidBody*)reader.getContents("hkpRigidBody");
			reader.getAllocatedData()->disableDestructors();

			if( reconnectMeshByHand )
			{
				// Access the mesh we want to reconnect. Normally we would
				// explicitly save the list of mesh shapes to reconnect, but
				// for the sake of brevity, we hardcode this.
				const hkpExtendedMeshShape* mesh = static_cast<const hkpExtendedMeshShape*>(
					static_cast<const hkpMoppBvTreeShape*>(
						m_body->getCollidable()->getShape())->getShapeCollection() );
				hkpExtendedMeshShape::TrianglesSubpart& part = const_cast<hkpExtendedMeshShape*>(mesh)->getTrianglesSubpartAt(0);
				setupMeshPart(part);
			}
			loadTime.stop();
		}
		hkString s; s.printf("Saved and reloaded from memory buffer of %i bytes\n" \
			"Saved in %f ms, Loaded in %f ms",
			m_buffer.getSize(), 1000*saveTime.getElapsedSeconds(), 1000*loadTime.getElapsedSeconds() );
		int charheight = (int)m_env->m_textDisplay->getFont()->getCharHeight();
		int starty = m_env->m_window->getHeight() - 6 * charheight;
		m_env->m_textDisplay->outputText( s, 30, starty, 0xffffffff, 100 );

		m_world->lock();
		m_world->addEntity(m_body);
		m_world->unlock();
		setupGraphics();
	}

	return DEMO_OK;
}

const hkReal SerializeMoppDemo::meshVertices[SerializeMoppDemo::NUM_VERTS*3] = {
-2.25063e-006f,-2.25063e-006f,51.4883f, -2.18185e-006f,-5.8721f,49.9149f,
-1.99395e-006f,-10.1708f,45.6162f, -1.73727e-006f,-11.7442f,39.7441f,
-1.48059e-006f,-10.1708f,33.872f, -1.29269e-006f,-5.8721f,29.5733f,
-1.22392e-006f,-1.97205e-007f,27.9999f, -1.29269e-006f,5.8721f,29.5733f,
-1.48059e-006f,10.1708f,33.872f, -1.73727e-006f,11.7442f,39.7441f,
-1.99395e-006f,10.1708f,45.6162f, -2.18185e-006f,5.8721f,49.9149f,
13.3262f,-2.17394e-006f,49.7339f, 12.9189f,-5.8721f,48.2141f,
11.8063f,-10.1708f,44.0619f, 10.2865f,-11.7442f,38.3899f,
8.76672f,-10.1708f,32.7179f, 7.65414f,-5.8721f,28.5657f,
7.24691f,-1.55501e-007f,27.0458f, 7.65414f,5.8721f,28.5657f,
8.76672f,10.1708f,32.7179f, 10.2865f,11.7442f,38.3899f,
11.8063f,10.1708f,44.0619f, 12.9189f,5.8721f,48.2141f,
25.7442f,-1.9491e-006f,44.5902f, 24.9574f,-5.8721f,43.2276f,
22.8081f,-10.1708f,39.5048f, 19.8721f,-11.7442f,34.4194f,
16.936f,-10.1708f,29.334f, 14.7867f,-5.8721f,25.6113f,
14.0f,0.0f,24.2486f, 14.7867f,5.8721f,25.6113f,
16.936f,10.1708f,29.334f, 19.8721f,11.7442f,34.4194f,
22.8081f,10.1708f,39.5048f, 24.9574f,5.8721f,43.2276f,
36.4077f,-1.59143e-006f,36.4077f, 35.2952f,-5.8721f,35.2952f,
32.2555f,-10.1708f,32.2555f, 28.1033f,-11.7442f,28.1033f,
23.9511f,-10.1708f,23.9511f, 20.9115f,-5.8721f,20.9115f,
19.7989f,1.61272e-007f,19.7989f, 20.9115f,5.8721f,20.9115f,
23.9511f,10.1708f,23.9511f, 28.1033f,11.7442f,28.1033f,
32.2555f,10.1708f,32.2555f, 35.2952f,5.8721f,35.2952f,
44.5902f,-1.12531e-006f,25.7442f, 43.2276f,-5.8721f,24.9574f,
39.5048f,-10.1708f,22.8081f, 34.4194f,-11.7442f,19.8721f,
29.334f,-10.1708f,16.936f, 25.6113f,-5.8721f,14.7867f,
24.2486f,4.14753e-007f,14.0f, 25.6113f,5.8721f,14.7867f,
29.334f,10.1708f,16.936f, 34.4194f,11.7442f,19.8721f,
39.5048f,10.1708f,22.8081f, 43.2276f,5.8721f,24.9575f,
49.7339f,-5.82505e-007f,13.3262f, 48.2141f,-5.8721f,12.9189f,
44.0619f,-10.1708f,11.8063f, 38.3899f,-11.7442f,10.2865f,
32.7179f,-10.1708f,8.76672f, 28.5657f,-5.8721f,7.65414f,
27.0458f,7.09938e-007f,7.24691f, 28.5657f,5.8721f,7.65414f,
32.7179f,10.1708f,8.76672f, 38.3899f,11.7442f,10.2865f,
44.0619f,10.1708f,11.8063f, 48.2141f,5.8721f,12.9189f, 51.4883f,0.0f,0.0f,
49.9149f,-5.8721f,-2.38419e-007f, 45.6162f,-10.1708f,-4.17233e-007f,
39.7441f,-11.7442f,-5.36442e-007f, 33.872f,-10.1708f,-4.17233e-007f,
29.5733f,-5.8721f,-2.38419e-007f, 27.9999f,1.02671e-006f,0.0f,
29.5733f,5.8721f,2.38419e-007f, 33.872f,10.1708f,4.17233e-007f,
39.7441f,11.7442f,5.36442e-007f, 45.6162f,10.1708f,4.17233e-007f,
49.9149f,5.8721f,2.38419e-007f, 49.7339f,5.82505e-007f,-13.3262f,
48.2141f,-5.8721f,-12.9189f, 44.0619f,-10.1708f,-11.8063f,
38.3899f,-11.7442f,-10.2865f, 32.7179f,-10.1708f,-8.76672f,
28.5657f,-5.8721f,-7.65414f, 27.0458f,1.34348e-006f,-7.24691f,
28.5657f,5.8721f,-7.65414f, 32.7179f,10.1708f,-8.76672f,
38.3899f,11.7442f,-10.2865f, 44.0619f,10.1708f,-11.8063f,
48.2141f,5.8721f,-12.9189f, 44.5902f,1.12531e-006f,-25.7442f,
43.2276f,-5.8721f,-24.9574f, 39.5048f,-10.1708f,-22.8081f,
34.4194f,-11.7442f,-19.8721f, 29.334f,-10.1708f,-16.936f,
25.6113f,-5.8721f,-14.7867f, 24.2486f,1.63867e-006f,-14.0f,
25.6113f,5.8721f,-14.7867f, 29.334f,10.1708f,-16.936f,
34.4194f,11.7442f,-19.8721f, 39.5048f,10.1708f,-22.8081f,
43.2276f,5.8721f,-24.9575f, 36.4077f,1.59143e-006f,-36.4077f,
35.2952f,-5.8721f,-35.2952f, 32.2555f,-10.1708f,-32.2555f,
28.1033f,-11.7442f,-28.1033f, 23.9511f,-10.1708f,-23.9511f,
20.9115f,-5.8721f,-20.9115f, 19.7989f,1.89215e-006f,-19.7989f,
20.9115f,5.8721f,-20.9115f, 23.9511f,10.1708f,-23.9511f,
28.1033f,11.7442f,-28.1033f, 32.2555f,10.1708f,-32.2555f,
35.2952f,5.8721f,-35.2952f, 25.7442f,1.9491e-006f,-44.5902f,
24.9574f,-5.8721f,-43.2276f, 22.8081f,-10.1708f,-39.5048f,
19.8721f,-11.7442f,-34.4194f, 16.936f,-10.1708f,-29.334f,
14.7867f,-5.8721f,-25.6113f, 14.0f,2.08665e-006f,-24.2486f,
14.7867f,5.8721f,-25.6113f, 16.936f,10.1708f,-29.334f,
19.8721f,11.7442f,-34.4194f, 22.8081f,10.1708f,-39.5048f,
24.9574f,5.8721f,-43.2276f, 13.3262f,2.17394e-006f,-49.7339f,
12.9189f,-5.8721f,-48.2141f, 11.8063f,-10.1708f,-44.0619f,
10.2865f,-11.7442f,-38.3899f, 8.76672f,-10.1708f,-32.7179f,
7.65414f,-5.8721f,-28.5657f, 7.24691f,2.20892e-006f,-27.0458f,
7.65414f,5.8721f,-28.5657f, 8.76672f,10.1708f,-32.7179f,
10.2865f,11.7442f,-38.3899f, 11.8063f,10.1708f,-44.0619f,
12.9189f,5.8721f,-48.2141f, -2.25063e-006f,2.25063e-006f,-51.4883f,
-2.18185e-006f,-5.8721f,-49.9149f, -1.99395e-006f,-10.1708f,-45.6162f,
-1.73727e-006f,-11.7442f,-39.7441f, -1.48059e-006f,-10.1708f,-33.872f,
-1.29269e-006f,-5.8721f,-29.5733f, -1.22392e-006f,2.25063e-006f,-27.9999f,
-1.29269e-006f,5.8721f,-29.5733f, -1.48059e-006f,10.1708f,-33.872f,
-1.73727e-006f,11.7442f,-39.7441f, -1.99395e-006f,10.1708f,-45.6162f,
-2.18185e-006f,5.8721f,-49.9149f, -13.3262f,2.17394e-006f,-49.7339f,
-12.9189f,-5.8721f,-48.2141f, -11.8063f,-10.1708f,-44.0619f,
-10.2865f,-11.7442f,-38.3899f, -8.76673f,-10.1708f,-32.7179f,
-7.65415f,-5.8721f,-28.5657f, -7.24691f,2.20892e-006f,-27.0458f,
-7.65415f,5.8721f,-28.5657f, -8.76673f,10.1708f,-32.7179f,
-10.2865f,11.7442f,-38.3899f, -11.8064f,10.1708f,-44.0619f,
-12.9189f,5.8721f,-48.2141f, -25.7442f,1.9491e-006f,-44.5902f,
-24.9575f,-5.8721f,-43.2276f, -22.8081f,-10.1708f,-39.5048f,
-19.8721f,-11.7442f,-34.4194f, -16.936f,-10.1708f,-29.334f,
-14.7867f,-5.8721f,-25.6113f, -14.0f,2.08665e-006f,-24.2486f,
-14.7867f,5.8721f,-25.6113f, -16.936f,10.1708f,-29.334f,
-19.8721f,11.7442f,-34.4194f, -22.8081f,10.1708f,-39.5048f,
-24.9575f,5.8721f,-43.2276f, -36.4077f,1.59143e-006f,-36.4077f,
-35.2952f,-5.8721f,-35.2952f, -32.2555f,-10.1708f,-32.2555f,
-28.1033f,-11.7442f,-28.1033f, -23.9511f,-10.1708f,-23.9511f,
-20.9115f,-5.8721f,-20.9115f, -19.7989f,1.89215e-006f,-19.7989f,
-20.9115f,5.8721f,-20.9115f, -23.9511f,10.1708f,-23.9511f,
-28.1033f,11.7442f,-28.1033f, -32.2555f,10.1708f,-32.2555f,
-35.2952f,5.8721f,-35.2952f, -44.5902f,1.12531e-006f,-25.7442f,
-43.2276f,-5.8721f,-24.9574f, -39.5048f,-10.1708f,-22.8081f,
-34.4194f,-11.7442f,-19.8721f, -29.334f,-10.1708f,-16.936f,
-25.6113f,-5.8721f,-14.7867f, -24.2486f,1.63867e-006f,-14.0f,
-25.6113f,5.8721f,-14.7867f, -29.334f,10.1708f,-16.936f,
-34.4194f,11.7442f,-19.8721f, -39.5048f,10.1708f,-22.8081f,
-43.2276f,5.8721f,-24.9574f, -49.7339f,5.82505e-007f,-13.3262f,
-48.2141f,-5.8721f,-12.9189f, -44.0619f,-10.1708f,-11.8063f,
-38.3899f,-11.7442f,-10.2865f, -32.7179f,-10.1708f,-8.76672f,
-28.5657f,-5.8721f,-7.65414f, -27.0458f,1.34348e-006f,-7.24691f,
-28.5657f,5.8721f,-7.65414f, -32.7179f,10.1708f,-8.76672f,
-38.3899f,11.7442f,-10.2865f, -44.0619f,10.1708f,-11.8063f,
-48.2141f,5.8721f,-12.9189f, -51.4883f,0.0f,4.52995e-006f,
-49.9149f,-5.8721f,4.11272e-006f, -45.6162f,-10.1708f,3.51667e-006f,
-39.7441f,-11.7442f,2.98023e-006f, -33.872f,-10.1708f,2.5034e-006f,
-29.5733f,-5.8721f,2.32458e-006f, -27.9999f,1.02671e-006f,2.44379e-006f,
-29.5733f,5.8721f,2.86102e-006f, -33.872f,10.1708f,3.39746e-006f,
-39.7441f,11.7442f,3.99351e-006f, -45.6162f,10.1708f,4.41074e-006f,
-49.9149f,5.8721f,4.64916e-006f, -49.7339f,-5.82505e-007f,13.3262f,
-48.2141f,-5.8721f,12.9189f, -44.0619f,-10.1708f,11.8063f,
-38.3899f,-11.7442f,10.2865f, -32.7179f,-10.1708f,8.76673f,
-28.5657f,-5.8721f,7.65415f, -27.0458f,7.09938e-007f,7.24691f,
-28.5657f,5.8721f,7.65415f, -32.7179f,10.1708f,8.76673f,
-38.3899f,11.7442f,10.2865f, -44.0619f,10.1708f,11.8064f,
-48.2141f,5.8721f,12.9189f, -44.5902f,-1.12531e-006f,25.7442f,
-43.2276f,-5.8721f,24.9575f, -39.5048f,-10.1708f,22.8081f,
-34.4194f,-11.7442f,19.8721f, -29.334f,-10.1708f,16.936f,
-25.6113f,-5.8721f,14.7867f, -24.2486f,4.14753e-007f,14.0f,
-25.6113f,5.8721f,14.7867f, -29.334f,10.1708f,16.936f,
-34.4194f,11.7442f,19.8721f, -39.5048f,10.1708f,22.8081f,
-43.2276f,5.8721f,24.9575f, -36.4077f,-1.59143e-006f,36.4077f,
-35.2952f,-5.8721f,35.2952f, -32.2555f,-10.1708f,32.2555f,
-28.1033f,-11.7442f,28.1033f, -23.9511f,-10.1708f,23.9511f,
-20.9115f,-5.8721f,20.9115f, -19.7989f,1.61272e-007f,19.7989f,
-20.9115f,5.8721f,20.9115f, -23.9511f,10.1708f,23.9511f,
-28.1033f,11.7442f,28.1033f, -32.2555f,10.1708f,32.2555f,
-35.2952f,5.8721f,35.2952f, -25.7442f,-1.9491e-006f,44.5902f,
-24.9574f,-5.8721f,43.2276f, -22.8081f,-10.1708f,39.5048f,
-19.8721f,-11.7442f,34.4194f, -16.936f,-10.1708f,29.334f,
-14.7867f,-5.8721f,25.6113f, -14.0f,0.0f,24.2486f,
-14.7867f,5.8721f,25.6113f, -16.936f,10.1708f,29.334f,
-19.8721f,11.7442f,34.4194f, -22.8081f,10.1708f,39.5048f,
-24.9574f,5.8721f,43.2276f, -13.3262f,-2.17394e-006f,49.7339f,
-12.9189f,-5.8721f,48.2141f, -11.8063f,-10.1708f,44.0619f,
-10.2865f,-11.7442f,38.3899f, -8.76672f,-10.1708f,32.7179f,
-7.65414f,-5.8721f,28.5657f, -7.24691f,-1.55501e-007f,27.0458f,
-7.65414f,5.8721f,28.5657f, -8.76672f,10.1708f,32.7179f,
-10.2865f,11.7442f,38.3899f, -11.8063f,10.1708f,44.0619f,
-12.9189f,5.8721f,48.2141f };

const int SerializeMoppDemo::meshIndices[SerializeMoppDemo::NUM_TRIS*3] = { 0,13,12, 0,1,13, 1,14,13, 1,2,14,
2,15,14, 2,3,15, 3,16,15, 3,4,16, 4,17,16, 4,5,17, 5,18,17, 5,6,18,
6,19,18, 6,7,19, 7,20,19, 7,8,20, 8,21,20, 8,9,21, 9,22,21, 9,10,22,
10,23,22, 10,11,23, 11,12,23, 11,0,12, 12,25,24, 12,13,25, 13,26,25,
13,14,26, 14,27,26, 14,15,27, 15,28,27, 15,16,28, 16,29,28, 16,17,29,
17,30,29, 17,18,30, 18,31,30, 18,19,31, 19,32,31, 19,20,32, 20,33,32,
20,21,33, 21,34,33, 21,22,34, 22,35,34, 22,23,35, 23,24,35, 23,12,24,
24,37,36, 24,25,37, 25,38,37, 25,26,38, 26,39,38, 26,27,39, 27,40,39,
27,28,40, 28,41,40, 28,29,41, 29,42,41, 29,30,42, 30,43,42, 30,31,43,
31,44,43, 31,32,44, 32,45,44, 32,33,45, 33,46,45, 33,34,46, 34,47,46,
34,35,47, 35,36,47, 35,24,36, 36,49,48, 36,37,49, 37,50,49, 37,38,50,
38,51,50, 38,39,51, 39,52,51, 39,40,52, 40,53,52, 40,41,53, 41,54,53,
41,42,54, 42,55,54, 42,43,55, 43,56,55, 43,44,56, 44,57,56, 44,45,57,
45,58,57, 45,46,58, 46,59,58, 46,47,59, 47,48,59, 47,36,48, 48,61,60,
48,49,61, 49,62,61, 49,50,62, 50,63,62, 50,51,63, 51,64,63, 51,52,64,
52,65,64, 52,53,65, 53,66,65, 53,54,66, 54,67,66, 54,55,67, 55,68,67,
55,56,68, 56,69,68, 56,57,69, 57,70,69, 57,58,70, 58,71,70, 58,59,71,
59,60,71, 59,48,60, 60,73,72, 60,61,73, 61,74,73, 61,62,74, 62,75,74,
62,63,75, 63,76,75, 63,64,76, 64,77,76, 64,65,77, 65,78,77, 65,66,78,
66,79,78, 66,67,79, 67,80,79, 67,68,80, 68,81,80, 68,69,81, 69,82,81,
69,70,82, 70,83,82, 70,71,83, 71,72,83, 71,60,72, 72,85,84, 72,73,85,
73,86,85, 73,74,86, 74,87,86, 74,75,87, 75,88,87, 75,76,88, 76,89,88,
76,77,89, 77,90,89, 77,78,90, 78,91,90, 78,79,91, 79,92,91, 79,80,92,
80,93,92, 80,81,93, 81,94,93, 81,82,94, 82,95,94, 82,83,95, 83,84,95,
83,72,84, 84,97,96, 84,85,97, 85,98,97, 85,86,98, 86,99,98, 86,87,99,
87,100,99, 87,88,100, 88,101,100, 88,89,101, 89,102,101, 89,90,102,
90,103,102, 90,91,103, 91,104,103, 91,92,104, 92,105,104, 92,93,105,
93,106,105, 93,94,106, 94,107,106, 94,95,107, 95,96,107, 95,84,96,
96,109,108, 96,97,109, 97,110,109, 97,98,110, 98,111,110, 98,99,111,
99,112,111, 99,100,112, 100,113,112, 100,101,113, 101,114,113,
101,102,114, 102,115,114, 102,103,115, 103,116,115, 103,104,116,
104,117,116, 104,105,117, 105,118,117, 105,106,118, 106,119,118,
106,107,119, 107,108,119, 107,96,108, 108,121,120, 108,109,121,
109,122,121, 109,110,122, 110,123,122, 110,111,123, 111,124,123,
111,112,124, 112,125,124, 112,113,125, 113,126,125, 113,114,126,
114,127,126, 114,115,127, 115,128,127, 115,116,128, 116,129,128,
116,117,129, 117,130,129, 117,118,130, 118,131,130, 118,119,131,
119,120,131, 119,108,120, 120,133,132, 120,121,133, 121,134,133,
121,122,134, 122,135,134, 122,123,135, 123,136,135, 123,124,136,
124,137,136, 124,125,137, 125,138,137, 125,126,138, 126,139,138,
126,127,139, 127,140,139, 127,128,140, 128,141,140, 128,129,141,
129,142,141, 129,130,142, 130,143,142, 130,131,143, 131,132,143,
131,120,132, 132,145,144, 132,133,145, 133,146,145, 133,134,146,
134,147,146, 134,135,147, 135,148,147, 135,136,148, 136,149,148,
136,137,149, 137,150,149, 137,138,150, 138,151,150, 138,139,151,
139,152,151, 139,140,152, 140,153,152, 140,141,153, 141,154,153,
141,142,154, 142,155,154, 142,143,155, 143,144,155, 143,132,144,
144,157,156, 144,145,157, 145,158,157, 145,146,158, 146,159,158,
146,147,159, 147,160,159, 147,148,160, 148,161,160, 148,149,161,
149,162,161, 149,150,162, 150,163,162, 150,151,163, 151,164,163,
151,152,164, 152,165,164, 152,153,165, 153,166,165, 153,154,166,
154,167,166, 154,155,167, 155,156,167, 155,144,156, 156,169,168,
156,157,169, 157,170,169, 157,158,170, 158,171,170, 158,159,171,
159,172,171, 159,160,172, 160,173,172, 160,161,173, 161,174,173,
161,162,174, 162,175,174, 162,163,175, 163,176,175, 163,164,176,
164,177,176, 164,165,177, 165,178,177, 165,166,178, 166,179,178,
166,167,179, 167,168,179, 167,156,168, 168,181,180, 168,169,181,
169,182,181, 169,170,182, 170,183,182, 170,171,183, 171,184,183,
171,172,184, 172,185,184, 172,173,185, 173,186,185, 173,174,186,
174,187,186, 174,175,187, 175,188,187, 175,176,188, 176,189,188,
176,177,189, 177,190,189, 177,178,190, 178,191,190, 178,179,191,
179,180,191, 179,168,180, 180,193,192, 180,181,193, 181,194,193,
181,182,194, 182,195,194, 182,183,195, 183,196,195, 183,184,196,
184,197,196, 184,185,197, 185,198,197, 185,186,198, 186,199,198,
186,187,199, 187,200,199, 187,188,200, 188,201,200, 188,189,201,
189,202,201, 189,190,202, 190,203,202, 190,191,203, 191,192,203,
191,180,192, 192,205,204, 192,193,205, 193,206,205, 193,194,206,
194,207,206, 194,195,207, 195,208,207, 195,196,208, 196,209,208,
196,197,209, 197,210,209, 197,198,210, 198,211,210, 198,199,211,
199,212,211, 199,200,212, 200,213,212, 200,201,213, 201,214,213,
201,202,214, 202,215,214, 202,203,215, 203,204,215, 203,192,204,
204,217,216, 204,205,217, 205,218,217, 205,206,218, 206,219,218,
206,207,219, 207,220,219, 207,208,220, 208,221,220, 208,209,221,
209,222,221, 209,210,222, 210,223,222, 210,211,223, 211,224,223,
211,212,224, 212,225,224, 212,213,225, 213,226,225, 213,214,226,
214,227,226, 214,215,227, 215,216,227, 215,204,216, 216,229,228,
216,217,229, 217,230,229, 217,218,230, 218,231,230, 218,219,231,
219,232,231, 219,220,232, 220,233,232, 220,221,233, 221,234,233,
221,222,234, 222,235,234, 222,223,235, 223,236,235, 223,224,236,
224,237,236, 224,225,237, 225,238,237, 225,226,238, 226,239,238,
226,227,239, 227,228,239, 227,216,228, 228,241,240, 228,229,241,
229,242,241, 229,230,242, 230,243,242, 230,231,243, 231,244,243,
231,232,244, 232,245,244, 232,233,245, 233,246,245, 233,234,246,
234,247,246, 234,235,247, 235,248,247, 235,236,248, 236,249,248,
236,237,249, 237,250,249, 237,238,250, 238,251,250, 238,239,251,
239,240,251, 239,228,240, 240,253,252, 240,241,253, 241,254,253,
241,242,254, 242,255,254, 242,243,255, 243,256,255, 243,244,256,
244,257,256, 244,245,257, 245,258,257, 245,246,258, 246,259,258,
246,247,259, 247,260,259, 247,248,260, 248,261,260, 248,249,261,
249,262,261, 249,250,262, 250,263,262, 250,251,263, 251,252,263,
251,240,252, 252,265,264, 252,253,265, 253,266,265, 253,254,266,
254,267,266, 254,255,267, 255,268,267, 255,256,268, 256,269,268,
256,257,269, 257,270,269, 257,258,270, 258,271,270, 258,259,271,
259,272,271, 259,260,272, 260,273,272, 260,261,273, 261,274,273,
261,262,274, 262,275,274, 262,263,275, 263,264,275, 263,252,264,
264,277,276, 264,265,277, 265,278,277, 265,266,278, 266,279,278,
266,267,279, 267,280,279, 267,268,280, 268,281,280, 268,269,281,
269,282,281, 269,270,282, 270,283,282, 270,271,283, 271,284,283,
271,272,284, 272,285,284, 272,273,285, 273,286,285, 273,274,286,
274,287,286, 274,275,287, 275,276,287, 275,264,276, 276,1,0, 276,277,1,
277,2,1, 277,278,2, 278,3,2, 278,279,3, 279,4,3, 279,280,4, 280,5,4,
280,281,5, 281,6,5, 281,282,6, 282,7,6, 282,283,7, 283,8,7, 283,284,8,
284,9,8, 284,285,9, 285,10,9, 285,286,10, 286,11,10, 286,287,11, 287,0,11,
287,276,0 };

#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = 
"Press \221 to save the rigid body, mesh and mopp using the havok snapshot utility\n" \
"Press \222 to save using the lowlevel interface, reconnecting mesh pointers by hand\n" \
"Note that after the first snapshot save, the mesh shape is replaced by a storagemesh shape " \
"so all subsequent saves will save the mesh data instead of reconnecting.";

HK_DECLARE_DEMO(SerializeMoppDemo, HK_DEMO_TYPE_PRIME | HK_DEMO_TYPE_SERIALIZE, "Load and save a mopp and its associated mesh.\n", helpString);

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
