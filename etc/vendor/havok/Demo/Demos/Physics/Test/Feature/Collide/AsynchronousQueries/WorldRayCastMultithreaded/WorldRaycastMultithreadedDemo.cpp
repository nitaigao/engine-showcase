/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


#include <Demos/demos.h>

#include <Physics/Collide/Query/CastUtil/hkpWorldRayCastOutput.h>
#include <Physics/Collide/Query/Multithreaded/RayCastQuery/hkpRayCastQueryJobs.h>
#include <Physics/Collide/Shape/Compound/Tree/Mopp/hkpMoppUtility.h>
#include <Physics/Collide/Shape/Compound/Collection/SimpleMesh/hkpSimpleMeshShape.h>

#include <Common/Internal/ConvexHull/hkGeometryUtility.h>

#include <Common/Base/Algorithm/PseudoRandom/hkPseudoRandomGenerator.h>

#include <Common/Visualize/hkDebugDisplay.h>

#include <Demos/Physics/Test/Feature/Collide/AsynchronousQueries/WorldRayCastMultithreaded/WorldRaycastMultithreadedDemo.h>

#include <Physics/Collide/Query/Multithreaded/CollisionQuery/hkpCollisionQueryJobQueueUtils.h>
#include <Common/Base/Thread/Job/ThreadPool/Cpu/hkCpuJobThreadPool.h>


#if defined(HK_PLATFORM_PS3_PPU)
//	# of real SPUs
#	define NUM_SPUS 6
#else
//	# of simulated SPUs
#	define NUM_SPUS 1
#endif


struct WorldRayCastMultithreadedDemoVariant
{
	enum DemoType
	{
		MULTITHREADED_ON_PPU,
		MULTITHREADED_ON_SPU,
		MULTITHREADED_ON_PPU_AND_SPU,
	};

	const char*	m_name;
	DemoType	m_demoType;
	const char*	m_details;
};


static const char helpString[] = \
	"This demo demonstrates how to emulate the hkpWorld::castRay() functionality in a multithreaded "	\
	"environment through the use of a dedicated hkpWorldRayCastJob. By putting such a job on "			\
	"the job queue it can be processed by other threads or SPUs (on PS3). "								\
	"One command of this job will cast exactly one ray into the world, using the broadphase to avoid "		\
	"expensive low-level intersection tests. You can place an arbitrary number of commands into one "		\
	"job, as the job will automatically split itself into parallel sub-jobs. "							\
	"For more information on the original hkpWorld::castRay() functionality, see WorldRayCastDemo. ";


static const WorldRayCastMultithreadedDemoVariant g_WorldRayCastMultithreadedDemoVariants[] =
{
	{ "Multithreaded",				WorldRayCastMultithreadedDemoVariant::MULTITHREADED_ON_PPU,			helpString },
};


static void createMeshShape( const int side, hkpSimpleMeshShape* meshShape )
{
	{
		meshShape->m_vertices.setSize( side * side );
		for(int i = 0; i < side; i++)
		{
			for (int j = 0; j < side; j++ )
			{
				hkVector4 vertex ( i * 1.0f - side * 0.5f,
					j * 1.0f - side * 0.5f,
					0.6f * hkMath::cos((hkReal)j + i) + 0.3f * hkMath::sin( 2.0f * i) );
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
				meshShape->m_triangles[curTri].m_a = corner;
				meshShape->m_triangles[curTri].m_b = corner+side;
				meshShape->m_triangles[curTri].m_c = corner+1;
				curTri++;

				meshShape->m_triangles[curTri].m_a = corner+1;
				meshShape->m_triangles[curTri].m_b = corner+side;
				meshShape->m_triangles[curTri].m_c = corner+side+1;
				curTri++;
				corner++; 
			}
			corner++; 
		}
	}
}


static hkpMoppBvTreeShape* createMoppShape()
{
	const int side = 5;

	//
	//	We use a storage mesh in our example, which copies all data.
	//  If you want to share graphics and physics, use the hkpMeshShape instead
	//
	hkpSimpleMeshShape* meshShape = new hkpSimpleMeshShape( 0.05f );

	createMeshShape( side, meshShape );

	hkpMoppCompilerInput mci;
	mci.m_enableChunkSubdivision = true;
	hkpMoppCode* code = hkpMoppUtility::buildCode( meshShape ,mci);

	hkpMoppBvTreeShape* moppShape = new hkpMoppBvTreeShape(meshShape, code);
	code->removeReference();
	meshShape->removeReference();

	return moppShape;
}


WorldRayCastMultithreadedDemo::WorldRayCastMultithreadedDemo(hkDemoEnvironment* env)
	: hkDefaultPhysicsDemo(env),
	m_semaphore(0,1000)
{
	const WorldRayCastMultithreadedDemoVariant& variant = g_WorldRayCastMultithreadedDemoVariants[m_variantId];

	//
	// Setup the camera.
	//
	{
		hkVector4 from(30.0f, 8.0f, 25.0f);
		hkVector4 to  ( 4.0f, 0.0f, -3.0f);
		hkVector4 up  ( 0.0f, 1.0f,  0.0f);
		setupDefaultCameras(env, from, to, up);
	}

	//
	// Create the world.
	//
	{
		hkpWorldCinfo info;
		info.setupSolverInfo(hkpWorldCinfo::SOLVER_TYPE_4ITERS_MEDIUM); 
		
		// Set gravity to zero so body floats.
		info.m_gravity.set(0.0f, 0.0f, 0.0f);	
		info.setBroadPhaseWorldSize( 100.0f );
		m_world = new hkpWorld(info);
		m_world->lock();

		// Disable backface culling, since we have mopp's etc.
		setGraphicsState(HKG_ENABLED_CULLFACE, false);

		setupGraphics();
	}

	// register all agents(however, we put all objects into the some group,
	// so no collision will be detected
	hkpAgentRegisterUtil::registerAllAgents( m_world->getCollisionDispatcher() );

	// Add a collision filter to the world to allow the bodies interpenetrate
	{
		hkpGroupFilter* filter = new hkpGroupFilter();
		filter->disableCollisionsBetween( hkpGroupFilterSetup::LAYER_DEBRIS, hkpGroupFilterSetup::LAYER_DEBRIS );
		m_world->setCollisionFilter( filter );
		filter->removeReference();
	}

	//
	// Set the simulation time to 0
	//
	m_time = 0;

	//
	// Create some bodies (reuse the ShapeRayCastApi demo)
	//
	createBodies();

	m_world->unlock();


	hkpCollisionQueryJobQueueUtils::registerWithJobQueue(m_jobQueue);


	// Special case for this demo variant: we do not allow the # of active SPUs to drop to zero as this can cause a deadlock.
	if ( variant.m_demoType == WorldRayCastMultithreadedDemoVariant::MULTITHREADED_ON_SPU )
	{
		m_allowZeroActiveSpus = false;
	}

}


void WorldRayCastMultithreadedDemo::createBodies()
{
	hkpRigidBodyCinfo rigidBodyInfo;
	rigidBodyInfo.m_collisionFilterInfo = hkpGroupFilter::calcFilterInfo(1, 1);
	hkPseudoRandomGenerator rand(100);


	// Note: with a SPU MOPP cache of 32768 bytes we can currently go to 811 objects and still get the broadphase onto SPU.
	for( int i = 0; i < 100; i++)
	{
		// All bodies created below are movable
		rigidBodyInfo.m_motionType = hkpMotion::MOTION_SPHERE_INERTIA;

		// A collection of 100 rigid bodies is randomly created by generating an integer in the range(0,4) and choosing
		// one of the following shapes; MOPP, Convex Vertices, Box, Sphere or Triangle depending on the outcome:
		int shapeType = (int) (rand.getRandRange(0,1) * 5);
		switch(shapeType)
		{
			case 0:
			// Create MOPP body
			{
				hkpMoppBvTreeShape* shape = createMoppShape();
				rigidBodyInfo.m_shape = shape;
				break;
			}

			// The construction of each of these is quite similar and for the purposes of this tutorial we will just outline
			// that of the Convex Vertices object, the code for which is presented below.
			// Create ConvexVertices body
			case 1:
			{
				// Data specific to this shape.
				int numVertices = 4;

					// 16 = 4 (size of "each float group", 3 for x,y,z, 1 for padding) * 4 (size of float)
				int stride = 16;

				float vertices[] = { // 4 vertices plus padding
					-2.0f, 1.0f, 1.0f, 0.0f, // v0
					 1.0f, 2.0f, 0.0f, 0.0f, // v1
					 0.0f, 0.0f, 3.0f, 0.0f, // v2
					 1.0f, -1.0f, 0.0f, 0.0f  // v3
				};
				

				hkpConvexVerticesShape* shape;
				hkArray<hkVector4> planeEquations;
				hkGeometry geom;
				{
					hkStridedVertices stridedVerts;
					{
						stridedVerts.m_numVertices = numVertices;
						stridedVerts.m_striding = stride;
						stridedVerts.m_vertices = vertices;
					}

					hkGeometryUtility::createConvexGeometry( stridedVerts, geom, planeEquations );

					{
						stridedVerts.m_numVertices = geom.m_vertices.getSize();
						stridedVerts.m_striding = sizeof(hkVector4);
						stridedVerts.m_vertices = &(geom.m_vertices[0](0));
					}

					shape = new hkpConvexVerticesShape(stridedVerts, planeEquations);
				}

				rigidBodyInfo.m_shape = shape;
				break;
			}

			// Create Box body
			case 2:
			{
				// Data specific to this shape.
				hkVector4 halfExtents = hkVector4(1.0f, 2.0f, 3.0f);
				hkpBoxShape* shape = new hkpBoxShape(halfExtents );
				rigidBodyInfo.m_shape = shape;
				break;
			}

			// Create Sphere body
			case 3:
			{
				hkReal radius = rand.getRandRange(0.5f, 2.0f);
				hkpConvexShape* shape = new hkpSphereShape(radius);
				rigidBodyInfo.m_shape = shape;		
				break;
			}

			// Create Triangle body
			case 4:
			{
				hkVector4 a(-1.5f, -1.5f,  0.0f);
				hkVector4 b(1.5f, -1.5f,  0.0f);
				hkVector4 c(0.0f,  1.5f,  0.0f);

				hkpTriangleShape* shape = new hkpTriangleShape(a, b, c);
				rigidBodyInfo.m_shape = shape;
				break;
			}
		}	// end case

		// As usual we fill out the hkpRigidBodyCinfo 'blueprint' for the rigidbody, with the code above specifying
		// the necessary information for the 'm_shape' member. To create a hkpConvexVerticesShape we need a set of vertices and
		// we must generate a set of plane equations from these points. As you can see from the code this is all performed 
		// prior to instantiating the shape.

		// Fake Inertia tensor for simplicity, assume it's a unit cube
		{
			hkVector4 halfExtents(0.5f, 0.5f, 0.5f);
			hkReal mass = 10.0f;
			hkpMassProperties massProperties;
			hkpInertiaTensorComputer::computeBoxVolumeMassProperties(halfExtents, mass, massProperties);

			rigidBodyInfo.m_inertiaTensor = massProperties.m_inertiaTensor;
			rigidBodyInfo.m_centerOfMass = massProperties.m_centerOfMass;
			rigidBodyInfo.m_mass = massProperties.m_mass;			
		}	

		// The object is then assigned a random position, orientation and angular velocity and added to the world:

		rigidBodyInfo.m_position.set( rand.getRandRange(-10.0f, 10.0f), rand.getRandRange(-10.0f, 10.0f), rand.getRandRange(0.0f, -40.0f));
		rand.getRandomRotation( rigidBodyInfo.m_rotation );

		rigidBodyInfo.m_collisionFilterInfo = hkpGroupFilterSetup::LAYER_DEBRIS;

		hkpRigidBody* rigidBody = new hkpRigidBody(rigidBodyInfo);
		rigidBodyInfo.m_shape->removeReference();

		// Give them an initial velocity
		hkVector4 angularVel(rand.getRandRange(-1.0f, 1.0f), rand.getRandRange(-1.0f, 1.0f), rand.getRandRange(-1.0f, 1.0f));
		rigidBody->setAngularVelocity(angularVel);
		rigidBody->setAngularDamping(0.0f);

		m_world->addEntity(rigidBody);
		rigidBody->removeReference();
	}
}


hkDemo::Result WorldRayCastMultithreadedDemo::stepDemo()
{
	// const WorldRayCastMultithreadedDemoVariant& variant = g_WorldRayCastMultithreadedDemoVariants[m_variantId];

	m_time += m_timestep;

	// The start point of the ray remains fixed in world space with the destination point of the
	// ray being varied in both the X and Y directions. This is achieved with simple
	// sine and cosine functions calls using the current time as the varying parameter:

	hkReal xDir = 12.0f * hkMath::sin(m_time * 0.3f);
	hkReal yDir = 12.0f * hkMath::cos(m_time * 0.3f);

	// For this demo an array of size 1 is sufficient.
	hkArray<hkpWorldRayCastOutput> resultArray(1);

	const int numCommands = 1;
	hkArray<hkpWorldRayCastCommand> commands(numCommands);
	{
		hkpWorldRayCastCommand& command = commands[0];
		command.m_rayInput.m_from							. set(0.0f, 0.0f, 15.0f);
		command.m_rayInput.m_to								. set( xDir, yDir, -15.0f);
		command.m_rayInput.m_enableShapeCollectionFilter	= false;
		command.m_rayInput.m_filterInfo						= 0;
		command.m_results									= resultArray.begin();
		command.m_resultsCapacity							= 1;
		command.m_numResultsOut								= 0;
	}

	//
	// create the job header
	//
	hkpCollisionQueryJobHeader* jobHeader;
	{
		jobHeader = hkAllocateChunk<hkpCollisionQueryJobHeader>(1, HK_MEMORY_CLASS_DEMO);
	}

	//
	// setup hkpWorldRayCastJob
	//
	m_world->markForRead();
	hkpWorldRayCastJob worldRayCastJob(m_world->getCollisionInput(), jobHeader, commands.begin(), numCommands, m_world->m_broadPhase, &m_semaphore);
	m_world->unmarkForRead();

	//
	// Put the job on the queue, kick-off the PPU/SPU threads and wait for everything to finish.
	//
	{
		m_world->lockReadOnly();

		//
		// Put the raycast job on the job queue.
		//


		worldRayCastJob.setRunsOnSpuOrPpu();
		m_jobQueue->addJob( worldRayCastJob, hkJobQueue::JOB_LOW_PRIORITY );

		m_jobThreadPool->processAllJobs( m_jobQueue );

		m_jobThreadPool->waitForCompletion();

		//
		// Wait for the one single job we started to finish.
		//
		m_semaphore.acquire();

		m_world->unlockReadOnly();
	}

	//
	// Display results.
	//
	if( commands[0].m_numResultsOut > 0 )
	{
		hkVector4 intersectionPointWorld;
		intersectionPointWorld.setInterpolate4( commands[0].m_rayInput.m_from, commands[0].m_rayInput.m_to, commands[0].m_results->m_hitFraction );
		HK_DISPLAY_LINE( commands[0].m_rayInput.m_from, intersectionPointWorld, hkColor::RED);
		HK_DISPLAY_ARROW( intersectionPointWorld, commands[0].m_results->m_normal, hkColor::CYAN);
	}
	else
	{
		// Otherwise draw as GREY
		HK_DISPLAY_LINE(commands[0].m_rayInput.m_from, commands[0].m_rayInput.m_to, hkColor::rgbFromChars(200, 200, 200));
	}

	//
	// Free temporarily allocated memory.
	//
	hkDeallocateChunk(jobHeader, 1, HK_MEMORY_CLASS_DEMO);

	return hkDefaultPhysicsDemo::stepDemo();
}


#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif


HK_DECLARE_DEMO_VARIANT_USING_STRUCT( WorldRayCastMultithreadedDemo, HK_DEMO_TYPE_OTHER, WorldRayCastMultithreadedDemoVariant, g_WorldRayCastMultithreadedDemoVariants, HK_NULL );

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
