/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


#include <Demos/demos.h>

#include <Physics/Collide/Agent/hkpCollisionAgentConfig.h>
#include <Physics/Collide/Agent/hkpProcessCollisionInput.h>
#include <Physics/Collide/Query/CastUtil/hkpLinearCastInput.h>
#include <Physics/Collide/Query/Collector/PointCollector/hkpClosestCdPointCollector.h>
#include <Physics/Collide/Query/Collector/PointCollector/hkpFixedBufferCdPointCollector.h>
#include <Physics/Collide/Dispatch/hkpCollisionDispatcher.h>
#include <Physics/Collide/Query/Multithreaded/CollisionQuery/hkpCollisionQueryJobs.h>
#include <Physics/Collide/Shape/Convex/Capsule/hkpCapsuleShape.h>

#include <Common/Internal/ConvexHull/hkGeometryUtility.h>

#include <Common/Base/Algorithm/PseudoRandom/hkPseudoRandomGenerator.h>

#include <Physics/Utilities/VisualDebugger/Viewer/hkpShapeDisplayBuilder.h>

#include <Common/Visualize/hkDebugDisplay.h>
#include <Common/Visualize/Shape/hkDisplayGeometry.h>

#include <Demos/DemoCommon/Utilities/GameUtils/GameUtils.h>

#include <Demos/Physics/Test/Feature/Collide/AsynchronousQueries/WorldLinearCastMultithreaded/WorldLinearCastMultithreadedDemo.h>

#include <Physics/Collide/Query/Multithreaded/CollisionQuery/hkpCollisionQueryJobQueueUtils.h>
#include <Common/Base/Thread/Job/ThreadPool/Cpu/hkCpuJobThreadPool.h>


#if defined(HK_PLATFORM_PS3_PPU)
//	# of real SPUs
#	define NUM_SPUS 6
#else
//	# of simulated SPUs
#	define NUM_SPUS 1
#endif


// Number of query repetitions (for better timing info)
#if defined(HK_DEBUG)  || defined( HK_PS2 )
#	define NUM_ITER 1
#else
#	define NUM_ITER 10
#endif


struct WorldLinearCastMultithreadedDemoVariant
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
	"This demo demonstrates how to emulate the hkpWorld::linearCast() functionality in a multithreaded "	\
	"environment through the use of a dedicated hkpWorldLinearCastJob. By putting such a job on "		\
	"the job queue it can be processed by other threads or SPUs (on PS3). "								\
	"One command of this job will cast exactly one collidable into the world, using the broadphase to "	\
	"avoid expensive low-level intersection tests. You can place an arbitrary number of commands into "	\
	"one job, as the job will automatically split itself into parallel sub-jobs. "						\
	"For more information on the original hkpWorld::linearCast() functionality, see WorldRayCastDemo. ";


static const WorldLinearCastMultithreadedDemoVariant g_WorldLinearCastMultithreadedDemoVariants[] =
{
	{ "Multithreaded",				WorldLinearCastMultithreadedDemoVariant::MULTITHREADED_ON_PPU,			helpString },
};


WorldLinearCastMultithreadedDemo::WorldLinearCastMultithreadedDemo(hkDemoEnvironment* env)
	: hkDefaultPhysicsDemo(env),
	m_time(0),
	m_semaphore(0,1000)
{
	const WorldLinearCastMultithreadedDemoVariant& variant = g_WorldLinearCastMultithreadedDemoVariants[m_variantId];

	//
	// Setup the camera.
	//
	{
		hkVector4 from(0.0f, 10.0f, 30.0f);
		hkVector4 to  (0.0f, 0.0f, 0.0f);
		hkVector4 up  (0.0f, 1.0f, 0.0f);
		setupDefaultCameras(env, from, to, up);
	}

	//
	// Create the world.
	//
	{
		hkpWorldCinfo info;
		{
			info.m_gravity.set(0.0f, -9.81f, 0.0f);	
			info.setBroadPhaseWorldSize( 100.0f );
			info.m_collisionTolerance = .001f;
			info.m_simulationType = hkpWorldCinfo::SIMULATION_TYPE_MULTITHREADED;
		}

		m_world = new hkpWorld(info);
		m_world->lock();

		//	Register collision agents
		hkpAgentRegisterUtil::registerAllAgents(m_world->getCollisionDispatcher());

		setupGraphics();
	}

	
	//
	// Create the underlying landscape
	//
	hkAabb aabb;
	{
		hkVector4 scaling(1.0f, 0.3f, 1.0f );
		m_groundShapeFactory.setScaling( scaling );
		m_groundShapeFactory.setTriangleRadius( 0.0f );

		hkpMoppBvTreeShape* groundShape = m_groundShapeFactory.createMoppShapeForSpu();
		{
			hkpRigidBodyCinfo groundInfo;
			groundInfo.m_shape = groundShape;
			groundInfo.m_motionType = hkpMotion::MOTION_FIXED;
			hkpRigidBody* b = new hkpRigidBody(groundInfo);
			
			b->getCollidable()->getShape()->getAabb( b->getTransform(), 0.0f, aabb );

			m_world->addEntity(b);
			b->removeReference();
		}
		groundShape->removeReference();
	}

	//	Create a selection of shapes
	hkArray<hkpShape*> shapes;
	createShapes( shapes );

	// We would like to ensure that each object is placed on the surface of the landscape such
	// that it is not penetrating and is resting on the surface. To do this we make use of world 
	// linear casting which will allow us to shape cast our objects against the landscape.
	// Using the results of this cast we can easily position our objects in the desired manner.
	{
		hkPseudoRandomGenerator random(501);
		int xRes = 8;
		int yRes = 8;
		for (int i = 0; i < xRes * yRes; i ++ )
		{
			// create a fixed rigid body info with a random orientation
			hkpRigidBodyCinfo ci;
			ci.m_shape = shapes[ random.getRandChar( shapes.getSize() ) ];
			ci.m_motionType = hkpMotion::MOTION_FIXED;
			random.getRandomRotation( ci.m_rotation );

			// place it above the plane
			ci.m_position.set(  (1.8f * aabb.m_max(0)) * (-.5f + (i%xRes)/ float(xRes)), 
								aabb.m_max(1) + 10.0f,
								(1.8f * aabb.m_max(2)) * (-.5f + (i/xRes)/ float(yRes))  );
			
			// create the rigid body
			hkpRigidBody* b = new hkpRigidBody(ci);


			// To perform the linear casting we use a hkpLinearCastInput structure and
			// set the end position of the cast (the start position is given by the
			// location of our object). We then create a hkpClosestCdPointCollector
			// to gather the results of the cast and then ask the world to perform
			// the cast:
			{
				hkpLinearCastInput li;
				li.m_to.set( ci.m_position(0), aabb.m_min(1), ci.m_position(2) );

				hkpClosestCdPointCollector hitCollector;
				m_world->linearCast( b->getCollidable(), li, hitCollector );

				//	Check for hits
				if ( hitCollector.hasHit() )
				{
					hkVector4 position; position.setInterpolate4( ci.m_position, li.m_to, hitCollector.getHitContact().getDistance() );
					b->setPosition( position );
				}
			}

			m_world->addEntity( b );
			m_rocksOnTheFloor.pushBack( b );
		}
	}

	hkPseudoRandomGenerator generator(3245);

	// Create query objects and phantoms from shapes
	buildQueryObects( shapes, m_queryObjects );


	//	Remove shape references
	{
		for (int i = 0; i < shapes.getSize(); i++)
		{ 
			shapes[i]->removeReference();
			shapes[i] = 0;
		}
	}

	m_world->unlock();

	hkpCollisionQueryJobQueueUtils::registerWithJobQueue(m_jobQueue);

	// Special case for this demo variant: we do not allow the # of active SPUs to drop to zero as this can cause a deadlock.
	if ( variant.m_demoType == WorldLinearCastMultithreadedDemoVariant::MULTITHREADED_ON_SPU ) m_allowZeroActiveSpus = false;

	// register the default addCdPoint() function; you are free to register your own implementation here though
	hkpFixedBufferCdPointCollector::registerDefaultAddCdPointFunction();

}


WorldLinearCastMultithreadedDemo::~WorldLinearCastMultithreadedDemo()
{

	m_world->lock();

	while( m_rocksOnTheFloor.getSize() )
	{
		m_rocksOnTheFloor[0]->removeReference();
		m_rocksOnTheFloor.removeAt(0);
	}

	{
		for ( int i = 0; i < m_queryObjects.getSize(); i++ )
		{
			QueryObject& qo = m_queryObjects[i];
			hkDebugDisplay::getInstance().removeGeometry( (hkUlong)qo.m_collidable, 0, 0 );
			delete qo.m_transform;
			delete qo.m_collidable;
		}
	}

	m_world->unlock();
}


void WorldLinearCastMultithreadedDemo::createShapes(hkArray<hkpShape*>&	shapesOut)
{
	const hkReal s = 1.25f;

	// Create Sphere body
	{
		hkpConvexShape* shape = new hkpSphereShape(s);
		shapesOut.pushBack(shape );
	}

	// Create Capsule body
	{
		hkVector4 a(-s * .5f,  0.f,   0.f);
		hkVector4 b( s * .5f,  0.f,   0.f);
		hkpCapsuleShape* shape = new hkpCapsuleShape(a, b, s);
		shapesOut.pushBack(shape );
	}

	// Create Box body
	{
		hkVector4 halfExtents; halfExtents.setAll3( s );
		hkpBoxShape* shape = new hkpBoxShape(halfExtents, 0 );
		shapesOut.pushBack(shape );
	}
	

	// Create ConvexVertices body
	{
		int numVertices = 12;

			// 3 for x,y,z (size of float)
		int stride = 3 * sizeof(float);
		const hkReal t = s * 0.7f;
		const hkReal o = 0.0f;

		float vertices[] = {	 // 4 vertices 
			 -s,  o, -s,
			 -s,  o,  s,
			  s,  o, -s,
			  s,  o,  s,

			  o,  s, -t,
			  o,  s,  t,
			  t,  s,  o,
			 -t,  s,  o, 
			  o, -s, -t,
			  o, -s,  t,
			  t, -s,  o,
			 -t, -s,  o, 
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

		shapesOut.pushBack( shape );
	}
}


void WorldLinearCastMultithreadedDemo::buildQueryObects( hkArray<hkpShape*>& shapes, hkArray<QueryObject>& objects )
{
	hkpShapeDisplayBuilder::hkpShapeDisplayBuilderEnvironment env;
	hkpShapeDisplayBuilder builder(env);

	for (int i = 0; i < shapes.getSize(); i++)
	{
		QueryObject qo;
		qo.m_transform = new hkTransform();
		qo.m_transform->setIdentity();

		qo.m_collidable = new hkpCollidable( shapes[i], qo.m_transform );
		objects.pushBack( qo );

		hkArray<hkDisplayGeometry*> displayGeometries;

		builder.buildDisplayGeometries( shapes[i], displayGeometries );
		hkDebugDisplay::getInstance().addGeometry( displayGeometries, hkTransform::getIdentity(), (hkUlong)qo.m_collidable, 0, 0 );

		while( displayGeometries.getSize() )
		{
			delete displayGeometries[0];
			displayGeometries.removeAt(0);
		}

		// Set green color
		HK_SET_OBJECT_COLOR((hkUlong)qo.m_collidable, hkColor::rgbFromChars(0,255,0,120));
	}
}


hkDemo::Result WorldLinearCastMultithreadedDemo::stepDemo()
{
	if (m_jobThreadPool->getNumThreads() == 0)
	{
		 HK_WARN(0x34561f23, "This demo does not run with only one thread");
		 return DEMO_STOP;
	}

//	const WorldLinearCastMultithreadedDemoVariant& variant = g_WorldLinearCastMultithreadedDemoVariants[m_variantId];

	m_world->lock();

	//	Reset all object colors
	{
		for (int i = 0; i < m_rocksOnTheFloor.getSize(); i++)
		{
			HK_SET_OBJECT_COLOR((hkUlong)m_rocksOnTheFloor[i]->getCollidable(), hkColor::rgbFromChars(70,70,70));
		}
	}


	// For this cast we use a hkpClosestCdPointCollector to gather the results:
	hkpClosestCdPointCollector collectors[10];

	// Since we're not too concerned with perfect accuracy, we can set the early out
	// distance to give the algorithm a chance to exit more quickly:
	m_world->getCollisionInput()->m_config->m_iterativeLinearCastEarlyOutDistance = 0.1f;
	
	m_world->unlock();

	// Cast direction & length.
	hkVector4 castVector( 0.0f, -30.0f, 0.0f );

	int numQueryObjects = m_queryObjects.getSize();

	//
	// Setup the output array where the resulting collision points will be returned.
	//
	hkpRootCdPoint* collisionPoints = hkAllocateChunk<hkpRootCdPoint>(numQueryObjects, HK_MEMORY_CLASS_DEMO);

	//
	// Setup commands: one command for each query object.
	//
	hkArray<hkpWorldLinearCastCommand> commands;
	{
		for ( int i = 0; i < numQueryObjects; i++ )
		{
			QueryObject& queryObject = m_queryObjects[i];

			//
			// Let QueryObjects move in circles.
			//
			hkVector4 position;
			{
				hkReal t = m_time + HK_REAL_PI * 2 * i / m_queryObjects.getSize();
				position.set( hkMath::sin( t ) * 10.0f, 10.0f, hkMath::cos( t ) * 10.0f );
			}

			queryObject.m_transform->setTranslation(position);

			hkpWorldLinearCastCommand& command = commands.expandOne();
			{
				// Init input data.
				{
					command.m_input.m_to				  . setAdd4( position, castVector );
					command.m_input.m_maxExtraPenetration = HK_REAL_EPSILON;
					command.m_input.m_startPointTolerance = HK_REAL_EPSILON;

					command.m_collidable				  = queryObject.m_collidable;
				}

				// Init output data.
				{
					command.m_results			= &collisionPoints[i];
					command.m_resultsCapacity	= 1;
					command.m_numResultsOut		= 0;
				}
			}
		}
	}

	//
	// Create the job header.
	//
	hkpCollisionQueryJobHeader* jobHeader;
	{
		jobHeader = hkAllocateChunk<hkpCollisionQueryJobHeader>(1, HK_MEMORY_CLASS_DEMO);
	}

	//
	// Setup hkpWorldLinearCastJob.
	//
	m_world->markForRead();
	hkpWorldLinearCastJob worldLinearCastJob(m_world->getCollisionInput(), jobHeader, commands.begin(), commands.getSize(), m_world->m_broadPhase, &m_semaphore);
	m_world->unmarkForRead();

	//
	// Put the job on the queue, kick-off the PPU/SPU threads and wait for everything to finish.
	//
	{
		m_world->lockReadOnly();

		//
		// Put the raycast job on the job queue.
		//

		worldLinearCastJob.setRunsOnSpuOrPpu();
		m_jobQueue->addJob( worldLinearCastJob, hkJobQueue::JOB_LOW_PRIORITY );

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
	{
		m_world->lock();
		{
			for (int i = 0; i < commands.getSize(); i++)
			{
				QueryObject& queryObject = m_queryObjects[i];

				hkpWorldLinearCastCommand& command = commands[i];
				if ( command.m_numResultsOut > 0 )
				{
					// move our position to the hit and draw a line along the cast direction
					hkVector4& pos = queryObject.m_transform->getTranslation();
					hkVector4 to; to.setAdd4( pos, castVector );
					hkVector4 newPos; newPos.setInterpolate4( pos, to, command.m_results->m_contact.getDistance() );
					HK_DISPLAY_LINE(pos, newPos, hkColor::GREEN);

					// Update our QO
					queryObject.m_transform->setTranslation( newPos );
					hkDebugDisplay::getInstance().updateGeometry( queryObject.m_collidable->getTransform(), (hkUlong)queryObject.m_collidable, 0);

					// call a function to display the details
					displayRootCdPoint( m_world, *command.m_results );
				}
				else
				{
					// only draw a line along the cast direction
					hkVector4& pos = queryObject.m_transform->getTranslation();
					hkVector4 to; to.setAdd4( pos, castVector );
					HK_DISPLAY_LINE(pos, to, hkColor::GREEN);

					// Update our QO
					hkVector4 nirvana(10000.0f, 10000.0f, 10000.0f);
					queryObject.m_transform->setTranslation( nirvana );
					hkDebugDisplay::getInstance().updateGeometry( queryObject.m_collidable->getTransform(), (hkUlong)queryObject.m_collidable, 0);
				}
			}
		}
		m_world->unlock();
	}

	//
	// Free temporarily allocated memory.
	//
	hkDeallocateChunk( jobHeader, 1, HK_MEMORY_CLASS_DEMO );
	hkDeallocateChunk(collisionPoints, numQueryObjects, HK_MEMORY_CLASS_DEMO);

	m_time += 0.005f;

	return hkDefaultPhysicsDemo::stepDemo();
}


void WorldLinearCastMultithreadedDemo::displayRootCdPoint( hkpWorld* world, const hkpRootCdPoint& cp )
{
	displayRootCdBody( world, cp.m_rootCollidableB, cp.m_shapeKeyB );

	// Display contact point
	HK_DISPLAY_ARROW( cp.m_contact.getPosition(), cp.m_contact.getNormal(), hkColor::YELLOW );
}


void WorldLinearCastMultithreadedDemo::displayRootCdBody( hkpWorld* world, const hkpCollidable* collidable, hkpShapeKey key)
{
	hkpShapeCollection::ShapeBuffer shapeBuffer;

	//	Check, whether we have a triangle from the landscape or a single object
	if ( key == HK_INVALID_SHAPE_KEY )
	{
		// now we have a single object as we are not part of a hkpShapeCollection
		HK_SET_OBJECT_COLOR((hkUlong)collidable, hkColor::rgbFromChars(160,160,160));
	}
	else
	{
		// now we need to get our triangle.
		// Attention: The next lines make certain assumptions about how the landscape is constructed:
		//   1. The landscape is a single hkMoppShape with wraps a hkpShapeCollection, which
		//      produces only triangles.
		//   2. All hkShapeCollections are wrapped with a hkpBvTreeShape

		HK_ASSERT(0x3e93321f,  world->getCollisionDispatcher()->hasAlternateType( collidable->getShape()->getType(), HK_SHAPE_BV_TREE ) );

		const hkpBvTreeShape* bvTreeShape = static_cast<const hkpBvTreeShape*>(collidable->getShape());

		hkpShapeKey triangleId = key;

		const hkpShape* childShape = bvTreeShape->getContainer()->getChildShape( triangleId, shapeBuffer );

		HK_ASSERT(0x23f67112,  childShape->getType() == HK_SHAPE_TRIANGLE );
		const hkpTriangleShape* triangle = static_cast<const hkpTriangleShape*>( childShape );

		hkVector4 vertex[3];
		vertex[0].setTransformedPos(collidable->getTransform(), triangle->getVertex(0));
		vertex[1].setTransformedPos(collidable->getTransform(), triangle->getVertex(1));
		vertex[2].setTransformedPos(collidable->getTransform(), triangle->getVertex(2));

		//	Draw the border of the triangle
		HK_DISPLAY_LINE(vertex[0], vertex[1], hkColor::WHITE);
		HK_DISPLAY_LINE(vertex[2], vertex[1], hkColor::WHITE);
		HK_DISPLAY_LINE(vertex[0], vertex[2], hkColor::WHITE);

		if ( (const void*)childShape != (const void*)shapeBuffer && shapeBuffer[10] == 0 )
		{
			HK_ASSERT(0x20b8765d, 0);
		}
	}
}


#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif


HK_DECLARE_DEMO_VARIANT_USING_STRUCT( WorldLinearCastMultithreadedDemo, HK_DEMO_TYPE_PRIME, WorldLinearCastMultithreadedDemoVariant, g_WorldLinearCastMultithreadedDemoVariants, HK_NULL ); 

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
