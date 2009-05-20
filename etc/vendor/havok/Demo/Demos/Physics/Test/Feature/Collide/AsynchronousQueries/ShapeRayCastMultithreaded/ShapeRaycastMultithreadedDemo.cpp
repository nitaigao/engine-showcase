/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


#include <Demos/demos.h>

#include <Physics/Collide/Query/Multithreaded/RayCastQuery/hkpRayCastQueryJobs.h>
#include <Physics/Collide/Shape/Convex/Capsule/hkpCapsuleShape.h>
#include <Physics/Collide/Shape/Convex/ConvexTransform/hkpConvexTransformShape.h>
#include <Physics/Collide/Shape/Convex/Cylinder/hkpCylinderShape.h>
#include <Physics/Collide/Shape/Compound/Collection/List/hkpListShape.h>
#include <Physics/Collide/Shape/Compound/Tree/Mopp/hkpMoppUtility.h>
#include <Physics/Collide/Shape/Compound/Collection/SimpleMesh/hkpSimpleMeshShape.h>

#include <Physics/Dynamics/Phantom/hkpAabbPhantom.h>
#include <Physics/Dynamics/Phantom/hkpSimpleShapePhantom.h>

#include <Common/Internal/ConvexHull/hkGeometryUtility.h>

#include <Common/Base/Algorithm/PseudoRandom/hkPseudoRandomGenerator.h>

#include <Common/Visualize/hkDebugDisplay.h>

#include <Demos/DemoCommon/Utilities/GameUtils/GameUtils.h>

#include <Demos/Physics/Test/Feature/Collide/AsynchronousQueries/ShapeRayCastMultithreaded/ShapeRaycastMultithreadedDemo.h>

#include <Physics/Collide/Query/Multithreaded/CollisionQuery/hkpCollisionQueryJobQueueUtils.h>
#include <Common/Base/Thread/Job/ThreadPool/Cpu/hkCpuJobThreadPool.h>
#include <Physics/Collide/Query/CastUtil/hkpWorldRayCastOutput.h>

#if defined(HK_PLATFORM_PS3_PPU)
//	# of real SPUs
#	define NUM_SPUS 6
#else
//	# of simulated SPUs
#	define NUM_SPUS 1
#endif


struct ShapeRayCastMultithreadedDemoVariant
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
	"A demo which shows use of the hkpShape raycast running in separate threads (PPU, SPU, or both may be used on PS3)"; 


static const ShapeRayCastMultithreadedDemoVariant g_ShapeRayCastMultithreadedDemoVariants[] =
{
	{ "Multithreaded",				ShapeRayCastMultithreadedDemoVariant::MULTITHREADED_ON_PPU,			helpString },
};


ShapeRayCastMultithreadedDemo::ShapeRayCastMultithreadedDemo(hkDemoEnvironment* env) : hkDefaultPhysicsDemo(env, DEMO_FLAGS_NO_SERIALIZE),
	m_semaphore(0, 1000)
{
	const ShapeRayCastMultithreadedDemoVariant& variant = g_ShapeRayCastMultithreadedDemoVariants[m_variantId];

	// Disable backface culling
	setGraphicsState(HKG_ENABLED_CULLFACE, false);
	
	// Setup the camera.
	{
		hkVector4 from(0.0f, 5.0f, 40.0f);
		hkVector4 to  (2.0f, 0.0f, 0.0f);
		hkVector4 up  (0.0f, 1.0f, 0.0f);
		setupDefaultCameras(env, from, to, up);
	}

	// Create the world.
	{
		hkpWorldCinfo info;
		
		// Set gravity to zero so body floats.
		info.setBroadPhaseWorldSize( 100.0f );
		info.m_gravity.setZero4();
		m_world = new hkpWorld(info);

		m_world->lock();

		setupGraphics();
	}

	createBodies(m_bodies);
	createPhantoms(m_phantoms);

	int lightGrey = hkColor::rgbFromChars(128,128,128);
	for ( int i =0; i< m_bodies.getSize(); i++ )
	{
		m_world->addEntity( m_bodies[i] );
		HK_SET_OBJECT_COLOR((hkUlong)m_bodies[i]->getCollidable(), lightGrey);
		m_objects.pushBack( m_bodies[i] );
	}

	for (int i=0; i<m_phantoms.getSize(); i++)
	{
		m_world->addPhantom( m_phantoms[i] );
		m_objects.pushBack( m_phantoms[i] );
	}

	m_world->unlock();

	//
	// Setup multithreading.
	//

	hkpCollisionQueryJobQueueUtils::registerWithJobQueue(m_jobQueue);

	// Special case for this demo variant: we do not allow the # of active SPUs to drop to zero as this can cause a deadlock.
	if ( variant.m_demoType == ShapeRayCastMultithreadedDemoVariant::MULTITHREADED_ON_SPU ) m_allowZeroActiveSpus = false;

}


ShapeRayCastMultithreadedDemo::~ShapeRayCastMultithreadedDemo()
{

	m_world->lock();

	for ( int i =0; i< m_objects.getSize(); i++ )
	{
		m_objects[i]->removeReference();
	}

	m_world->unlock();
}

void HK_CALL ShapeRayCastMultithreadedDemo::createPhantoms(hkArray<hkpPhantom*>&	phantomsOut)
{
	hkVector4 phantomPosition;
	const hkReal SHIFT_AMOUNT = 6.0f;
	phantomPosition.set(-(2/2-0.5f)*SHIFT_AMOUNT, 5.0f + SHIFT_AMOUNT, 0.0f);
	hkVector4 shift; shift.set( SHIFT_AMOUNT, 0.0f, 0.0f );

	// Create an AABB phantom
	{
		hkAabb aabb;
		hkVector4 one; one.setAll(1.0f);

		aabb.m_min.setSub4(phantomPosition, one);
		aabb.m_max.setAdd4(phantomPosition, one);
		hkpAabbPhantom* aabbPhantom = new hkpAabbPhantom(aabb);
		phantomsOut.pushBack( aabbPhantom);

		phantomPosition.add4(shift);
	}

	// Create a shape phantom
	{
		hkpSphereShape* sphere = new hkpSphereShape(1.0f);
		hkTransform transform(hkQuaternion::getIdentity(), phantomPosition);
		hkpSimpleShapePhantom* shapePhantom = new hkpSimpleShapePhantom(sphere, transform);
		phantomsOut.pushBack( shapePhantom );

		sphere->removeReference();

		phantomPosition.add4(shift);
	}

}

// In this demo the raycast is performed against a variety of shape types; MOPP, Convex Vertices, Box, Sphere and Triangle.
// The construction of each of these is quite similar and for the purposes of this tutorial we will just outline
// that of the box object.

void ShapeRayCastMultithreadedDemo::createBodies( hkArray<hkpRigidBody*>& bodiesOut )
{
	// Use a common base hkpRigidBodyCinfo for all bodies.

	hkpRigidBodyCinfo rigidBodyInfo;

	rigidBodyInfo.m_angularDamping = 0;
	rigidBodyInfo.m_angularVelocity.set(.01f, .05f, .1f);
	rigidBodyInfo.m_angularVelocity.mul4(5);
	rigidBodyInfo.m_motionType = hkpMotion::MOTION_DYNAMIC;

	const hkReal SHIFT_AMOUNT = 6.0f;
	rigidBodyInfo.m_position.set(-(6/2-0.5f)*SHIFT_AMOUNT, 1.0f + SHIFT_AMOUNT, 0.0f);
	hkVector4 shift; shift.set( SHIFT_AMOUNT, 0.0f, 0.0f );

	//
	//	Create Capsule
	//
	{
		hkVector4 A( 0.f, 1.2f, 0.f);
		hkVector4 B( 0.f,-1.2f, 0.f);
		hkReal radius = 1.0f;

		hkpCapsuleShape* shape = new hkpCapsuleShape(A, B, radius);
		rigidBodyInfo.m_shape = shape;

		bodiesOut.pushBack(new hkpRigidBody(rigidBodyInfo));
		shape->removeReference();
		rigidBodyInfo.m_position.add4( shift );
	}

	//
	//	Create Cylinder
	//
	{
		hkVector4 A( 0.f, 1.2f, 0.f);
		hkVector4 B( 0.f,-1.2f, 0.f);
		hkReal radius = 1.0f;

		hkpCylinderShape* shape = new hkpCylinderShape(A, B, radius);

		rigidBodyInfo.m_shape = shape;
		rigidBodyInfo.m_rotation = hkQuaternion( hkVector4(1.0f, 0.0f, 0.0f), HK_REAL_PI / 6.f);

		bodiesOut.pushBack(new hkpRigidBody(rigidBodyInfo));
		shape->removeReference();
		rigidBodyInfo.m_position.add4( shift );
	}

	//
	// Create MOPP Shape body
	//
	{
		hkpMoppBvTreeShape* shape = GameUtils::createMoppShape();
		rigidBodyInfo.m_shape = shape;

		bodiesOut.pushBack(new hkpRigidBody(rigidBodyInfo));
		shape->removeReference();
		rigidBodyInfo.m_position.add4( shift );
	}

	//
	// Create ConvexVertices body
	//
	{
		// Data specific to this shape.
		const int numVertices = 200;

		// 16 = 4 (size of "each float group", 3 for x,y,z, 1 for padding) * 4 (size of float)
		int stride = hkSizeOf(hkVector4);

		hkVector4 vertices[ numVertices ];

		hkPseudoRandomGenerator rand(12345);
		for (int i=0; i < numVertices; i++)
		{
			rand.getRandomVector11(vertices[i]);
			vertices[i].normalize3();
		}

		hkpConvexVerticesShape* shape;
		hkArray<hkVector4> planeEquations;

		hkGeometry geom;
		{
			hkStridedVertices stridedVerts;
			{
				stridedVerts.m_numVertices = numVertices;
				stridedVerts.m_striding = stride;
				stridedVerts.m_vertices = (const float*)vertices;
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

		bodiesOut.pushBack(new hkpRigidBody(rigidBodyInfo));
		shape->removeReference();
		rigidBodyInfo.m_position.add4( shift );
	}

	// We then proceed to fill in the various details of the hkpRigidBodyCinfo 'blueprint' for this body.
	// Note the reference removal for the shape and the rigid body, this is done as new references are added by the
	// rigid body itself to the shape and by the world to the rigid body. Havok is now responsible for these
	// objects and will delete them when appropriate.

	//
	//	Create Box
	//
	{
		// Data specific to this shape.
		hkVector4 oneOneOne = hkVector4(1.0f, 1.0f, 1.0f);
		hkpBoxShape* boxshape = new hkpBoxShape(oneOneOne, 0 );
		//hkpShape* shape = new hkpConvexTranslateShape(boxshape, oneOneOne);
		hkTransform trans;
		hkPseudoRandomGenerator prg(0);
		prg.getRandomRotation(trans.getRotation());
		prg.getRandomVector11(trans.getTranslation());
		hkpShape* shape = new hkpConvexTransformShape(boxshape, trans);
		boxshape->removeReference();

		rigidBodyInfo.m_shape = shape;

		bodiesOut.pushBack(new hkpRigidBody(rigidBodyInfo));
		shape->removeReference();
		rigidBodyInfo.m_position.add4( shift );
	}


	//
	// Create Sphere
	//
	{
		hkReal radius = 1.5f;
		hkpConvexShape* shape = new hkpSphereShape(radius);
		rigidBodyInfo.m_shape = shape;

		bodiesOut.pushBack(new hkpRigidBody(rigidBodyInfo));
		shape->removeReference();
		rigidBodyInfo.m_position.add4( shift );
	}

	//
	// Create Triangle body
	//
	{
		hkVector4 a(-1.5f, -1.5f,  0.0f);
		hkVector4 b(1.5f, -1.5f,  0.0f);
		hkVector4 c(0.0f,  1.5f,  0.0f);

		hkpTriangleShape* shape = new hkpTriangleShape(a, b, c);

		rigidBodyInfo.m_shape = shape;

		bodiesOut.pushBack(new hkpRigidBody(rigidBodyInfo));
		shape->removeReference();
		rigidBodyInfo.m_position.add4( shift );
	}

	//
	// Create a listshape
	//
	{
		hkArray<hkpShape*> shapes;
		{
			hkVector4 A( 1.0f, 0.2f, 1.4f);
			hkVector4 B( -1.0f,-0.2f, 1.4f);
			hkReal radius = 0.6f;

			hkpCapsuleShape* shape = new hkpCapsuleShape(A, B, radius);
			shapes.pushBack(shape);
		}
		{
			hkVector4 halfExtents( 0.5f, 0.6f, 1.5f);

			hkpBoxShape* shape = new hkpBoxShape(halfExtents);
			shapes.pushBack(shape);
		}
		hkpListShape* list = new hkpListShape(shapes.begin(), shapes.getSize());
		shapes[0]->removeReference();
		shapes[1]->removeReference();
		rigidBodyInfo.m_shape = list;

		bodiesOut.pushBack(new hkpRigidBody(rigidBodyInfo));
		list->removeReference();
		rigidBodyInfo.m_position.add4( shift );
	}
}


hkDemo::Result ShapeRayCastMultithreadedDemo::stepDemo()
{
//	const ShapeRayCastMultithreadedDemoVariant& variant = g_ShapeRayCastMultithreadedDemoVariants[m_variantId];

	const int numShapes			= m_objects.getSize();
	const int numRaysPerShape	= 20;
	const int numRays			= numShapes * numRaysPerShape;

	// Ray outputs (one for each ray).
	hkArray<hkpWorldRayCastOutput> rayOutputs(numRays);

	//
	// Create one command for each collidable-raycast pair.
	// We will cast a total of numRaysPerShape rays onto each collidable. Note that the rays' endpoints are slightly jittered.
	//
	const int numCommands = numRays;
	hkArray<hkpShapeRayCastCommand> commands(numCommands);
	{
		hkPseudoRandomGenerator rand(12345);
		hkVector4 startWorld(0.0f, 0.0f, 15.0f);
		int commandIdx = 0;

		{
			for (int shapeIdx = 0; shapeIdx < numShapes; shapeIdx++)
			{
				for (int rayIdx = 0; rayIdx < numRaysPerShape; rayIdx++)
				{

					hkpShapeRayCastCommand& command = commands[commandIdx];

					// hkpWorldObject::getCollidable() needs a read-lock on the object
					m_objects[shapeIdx]->markForRead();

					const hkpCollidable* collidable = m_objects[shapeIdx]->getCollidable();

					m_objects[shapeIdx]->unmarkForRead();

					// init shape data
					{
						const hkpCollidable** collidableArray = hkAlignedAllocate<const hkpCollidable*>(16, 1, HK_MEMORY_CLASS_DEMO);
						collidableArray[0] = collidable;

						command.m_collidables		= collidableArray;
						command.m_numCollidables	= 1;
					}

					// init ray data
					{
						hkVector4 endWorld;
						{
							// Fire ray roughly at shape's center
							rand.getRandomVector11( endWorld ); endWorld.mul4( 0.25f );
							
							hkVector4 translation; 

							// Can't call getTransform on AABB phantoms
							if ( collidable->getShape() )
							{
								translation = collidable->getTransform().getTranslation();
							}
							else
							{
								hkAabb aabb;
								hkpPhantom* phantom = hkGetPhantom( collidable );
								phantom->calcAabb(aabb);
								translation.setInterpolate4(aabb.m_max, aabb.m_min, .5f);
							}

							endWorld.add4( translation );
							hkVector4 dir; dir.setSub4(endWorld, startWorld);
							dir.mul4(1.5f);
							endWorld.add4( dir );
						}

						command.m_rayInput.m_from						= startWorld;
						command.m_rayInput.m_to							= endWorld;
						command.m_rayInput.m_rayShapeCollectionFilter	= HK_NULL;
						command.m_rayInput.m_filterInfo					= 0;
						command.m_filterType = hkpCollisionFilter::HK_FILTER_UNKNOWN;
						command.m_filterSize = 0;

					}

					// init output struct
					{
						command.m_results			= &rayOutputs[commandIdx];
						command.m_resultsCapacity	= 1;
						command.m_numResultsOut		= 0;
					}

					commandIdx++;
				}
			}
		}
	}

	//
	// Create the job header. Can't be on the stack due to PLAYSTATION(R)3 spu restrictions
	//
	hkpCollisionQueryJobHeader* jobHeader;
	{
		jobHeader = hkAllocateChunk<hkpCollisionQueryJobHeader>(1, HK_MEMORY_CLASS_DEMO);
	}

	//
	// Setup hkpShapeRayCastJob.
	// Note that for this case we will override the default granularity with a value that allows for a better split across all available SPUs.
	//
	m_world->markForRead();
	hkpShapeRayCastJob shapeRayCastJob(m_world->getCollisionInput(), jobHeader, commands.begin(), numCommands, &m_semaphore, 20);
	m_world->unmarkForRead();

	//
	// Put the job on the queue, kick-off the PPU/SPU threads and wait for everything to finish.
	//
	{
		//
		// Put the raycast on the job queue.
		//

		shapeRayCastJob.setRunsOnSpuOrPpu();
		m_jobQueue->addJob( shapeRayCastJob, hkJobQueue::JOB_LOW_PRIORITY );

		m_jobThreadPool->processAllJobs( m_jobQueue );

		m_jobThreadPool->waitForCompletion();

		//
		// Wait for the one single job we started to finish.
		//
		m_semaphore.acquire();

	}

	//
	// Output results.
	//
	{
		for (int i = 0; i < commands.getSize(); i++)
		{
			hkpShapeRayCastCommand& command = commands[i];

			// Display results (just one in this case).
			if ( command.m_numResultsOut > 0 )
			{
				hkpWorldRayCastOutput* output = &command.m_results[0];

				hkVector4 intersectionPointWorld;
				intersectionPointWorld.setInterpolate4(command.m_rayInput.m_from, command.m_rayInput.m_to, output->m_hitFraction );

				// Display hitting ray in RED.
				HK_DISPLAY_LINE(command.m_rayInput.m_from, intersectionPointWorld, hkColor::RED);

				// Display a small RED cross at the point of intersection.
				{
					hkVector4 p = intersectionPointWorld;
					hkVector4 q = intersectionPointWorld;
					p(0) -= 0.2f;
					q(0) += 0.2f;		
					HK_DISPLAY_LINE(p, q, hkColor::RED);
					p = intersectionPointWorld;
					q = intersectionPointWorld;
					p(1) -= 0.2f;
					q(1) += 0.2f;	
					HK_DISPLAY_LINE(p, q, hkColor::RED);
				}

				// Display hit normal.
				HK_DISPLAY_ARROW( intersectionPointWorld, output->m_normal, hkColor::CYAN );

				// You can also use the collidable pointer
				// output->m_rootCollidable

			}
			else
			{
				// Draw infinite ray as GREY.
				HK_DISPLAY_LINE(command.m_rayInput.m_from, command.m_rayInput.m_to, hkColor::rgbFromChars(200, 200, 200));
			}

			// Cleanup command.
			hkAlignedDeallocate(command.m_collidables);
		}
	}

	hkDeallocateChunk(jobHeader, 1, HK_MEMORY_CLASS_DEMO);

	return hkDefaultPhysicsDemo::stepDemo();
}


#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif


HK_DECLARE_DEMO_VARIANT_USING_STRUCT( ShapeRayCastMultithreadedDemo, HK_DEMO_TYPE_OTHER, ShapeRayCastMultithreadedDemoVariant, g_ShapeRayCastMultithreadedDemoVariants, HK_NULL );

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
