/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>

#include <Common/Base/Thread/Job/ThreadPool/Cpu/hkCpuJobThreadPool.h>
#include <Common/Visualize/hkDebugDisplay.h>

/// Need some shapes
#include <Physics/Collide/Shape/Convex/Box/hkpBoxShape.h>
#include <Physics/Collide/Shape/Compound/Tree/Mopp/hkpMoppUtility.h>
#include <Physics/Collide/Shape/Compound/Collection/ExtendedMeshShape/hkpExtendedMeshShape.h>
#include <Physics/Collide/Shape/Convex/ConvexTranslate/hkpConvexTranslateShape.h>

#include <Physics/Collide/Shape/Query/hkpShapeRayCastInput.h>
#include <Physics/Collide/Query/CastUtil/hkpWorldRayCastOutput.h>
#include <Physics/Collide/Query/Multithreaded/RayCastQuery/hkpRayCastQueryJobs.h>
#include <Physics/Collide/Query/Multithreaded/RayCastQuery/hkpRayCastQueryJobQueueUtils.h>

#include <Demos/Physics/Api/Collide/RayCasting/RayCastFiltering/RaycastFilteringDemo.h>

RaycastFilteringDemo::RaycastFilteringDemo(hkDemoEnvironment* env)
	:	hkDefaultPhysicsDemo(env), 
		m_time(0.0f),
		m_semaphore(0, 1000)
{

	//	
	// Setup the camera.
	//
	{
		hkVector4 from(-12.0f, 30.0f, 35.0f);
		hkVector4 to  (0.0f, 5.0f, 10.0f);
		hkVector4 up  (0.0f, 1.0f, 0.0f);
		setupDefaultCameras(env, from, to, up);

		// disable back face culling
		setGraphicsState(HKG_ENABLED_CULLFACE, false);
	}



	//
	// Create the world.
	//
	{
		hkpWorldCinfo info;
		info.setBroadPhaseWorldSize( 100.0f );

		m_world = new hkpWorld(info);
		m_world->lock();

		setupGraphics();

		hkpAgentRegisterUtil::registerAllAgents( m_world->getCollisionDispatcher() );
	}

	//
	//	Create the filter: every layer collides only with itself except layer 0, which collides with everything
	//
	{
			hkpGroupFilter* filter = new hkpGroupFilter();
			filter->disableCollisionsUsingBitfield( 0xfffffffe, 0xfffffffe );
			for (int i = 0; i < 32; i++ )
			{
				filter->enableCollisionsBetween( i, i );
			}
			m_world->setCollisionFilter( filter );
			filter->removeReference();
	}


	//
	//	Create a row of colored rigid bodies
	//
	{
		hkVector4 halfExtents(.5f, .5f, .5f );
		hkpShape* shape = new hkpBoxShape( halfExtents , 0 );

		int colors[4] = { hkColor::RED, hkColor::GREEN, hkColor::BLUE, hkColor::YELLOW };
		for (int i = 0; i < 4; i++ )
		{
			hkpRigidBodyCinfo ci;
			ci.m_motionType = hkpMotion::MOTION_FIXED;
			ci.m_shape = shape;
			ci.m_collisionFilterInfo = hkpGroupFilter::calcFilterInfo(i+1);

			for (int j = 0; j < 4; j++ )
			{
				hkReal fx = i * 3.0f;	// we need to declare these variables to avoid a bug in the VS2005 compiler
				hkReal fz = j * 2.0f + 2.0f;
				ci.m_position.set( fx, 0.f, fz);

				hkpRigidBody* body = new hkpRigidBody( ci );
				m_world->addEntity(body);
				body->removeReference();

				// show the objects in nice transparent colors
				colors[i] &= ~hkColor::rgbFromChars( 0, 0, 0 );
				colors[i] |= hkColor::rgbFromChars( 0, 0, 0, 120 );
				HK_SET_OBJECT_COLOR((hkUlong)body->getCollidable(), colors[i]);
			}
		}
		shape->removeReference();
	}

	//
	//	Create a mopp shape, which looks like __--__--__--__--
	//
	hkpShape* moppShape;
	{
		//
		//	Set our vertices and indices
		//  Note: we reuse the index triples in an overlapping way to
		//  get a triangle strip
		//
		{
			for (short i = 0; i < NUM_VERTICES; i++ )
			{
				m_indices[i] = i;
				m_vertices[i].x = ( (i+2)>>2 )  * 1.5f - 2.0f;
				m_vertices[i].y = ( (i>>2) & 1) * 1.0f - 0.5f;
				m_vertices[i].z = ( (i>>0) & 1) * (-9.5f);
				m_vertices[i].w = 0.f;
			}
		}
		//
		//	Set our materialIndices per triangle
		//
		{
			for (short i = 0; i < NUM_TRIANGLES; i++ )
			{
				m_materialIndices[i] = char(i>>3);
			}
		}
		//
		// Our material lib
		//
		{
			for ( int i = 0; i < 4; i++)
			{
				m_materials[i].m_filterInfo = hkpGroupFilter::calcFilterInfo(i+1);
			}
		}

		hkpExtendedMeshShape* mesh = new hkpExtendedMeshShape();
		mesh->setRadius(0.05f);
		{
			hkpExtendedMeshShape::TrianglesSubpart part;

			part.m_vertexBase = &m_vertices[0].x;
			part.m_vertexStriding = sizeof(MyVector4);
			part.m_numVertices = NUM_VERTICES;

			part.m_indexBase = m_indices;
			part.m_indexStriding = sizeof( hkUint16);
			part.m_numTriangleShapes = NUM_TRIANGLES;
			part.m_stridingType = hkpExtendedMeshShape::INDICES_INT16;

			part.m_materialIndexStridingType = hkpExtendedMeshShape::MATERIAL_INDICES_INT8;
			part.m_materialIndexBase = m_materialIndices;
			part.m_materialIndexStriding = sizeof(hkUint8);

			part.m_materialBase = m_materials;
			part.m_materialStriding = hkSizeOf( hkpMeshMaterial );
			part.m_numMaterials = 4;

			mesh->addTrianglesSubpart( part );
		}

		hkpMoppCompilerInput mci;
		mci.m_enableChunkSubdivision = true;
		hkpMoppCode* code = hkpMoppUtility::buildCode( mesh ,mci);

		moppShape = new hkpMoppBvTreeShape( mesh, code );

		mesh->removeReference();
		code->removeReference();
	}

	//
	//	Create the landscape body
	//
	{
		hkpRigidBodyCinfo ci;
		ci.m_motionType = hkpMotion::MOTION_FIXED;
		ci.m_shape = moppShape;
		hkpRigidBody* body = new hkpRigidBody( ci );
		m_world->addEntity(body);
		body->removeReference();
	}
	moppShape->removeReference();


	//
	//	Create a row of colored list shape rigid bodies
	//
	{
		hkVector4 halfExtents(.5f, .5f, .5f );
		hkpConvexShape* boxShape = new hkpBoxShape( halfExtents , 0 );

		int colors[4] = { hkColor::RED, hkColor::GREEN, hkColor::BLUE, hkColor::YELLOW };

		for (int j = 0; j < 4; j++ )
		{
			hkpRigidBodyCinfo ci;
			ci.m_motionType = hkpMotion::MOTION_FIXED;
			ci.m_collisionFilterInfo = 0;

			hkpListShape* shape;
			{
				hkpShape* shapes[4];
				for (int i = 0; i < 4; i++ )
				{
					hkReal fx = i * 3.0f;	// we need to declare these variables to avoid a bug in the VS2005 compiler
					hkReal fz = (j+4) * 2.0f + 4.0f;
					hkVector4 offset( fx, 0.f, fz);

					shapes[i] = new hkpConvexTranslateShape(boxShape, offset );
				}
				shape = new hkpListShape( &shapes[0], 4 );
				for (int k = 0; k < 4; k++ )
				{
					shape->setCollisionFilterInfo(k, hkpGroupFilter::calcFilterInfo(k+1) );
					shapes[k]->removeReference();
				}
			}
			ci.m_shape = shape;
			hkpRigidBody* body = new hkpRigidBody( ci );
			shape->removeReference();
			
			m_world->addEntity(body);
			body->removeReference();

			// show the objects in nice transparent colors
			colors[j] &= ~hkColor::rgbFromChars( 0, 0, 0 );
			colors[j] |= hkColor::rgbFromChars( 0, 0, 0, 120 );
			HK_SET_OBJECT_COLOR((hkUlong)body->getCollidable(), colors[j]);
		}
		boxShape->removeReference();
	}

	m_world->unlock();

	//
	// Setup multithreading.
	//
	hkpRayCastQueryJobQueueUtils::registerWithJobQueue(m_jobQueue);

	// Special case for this demo variant: we do not allow the # of active SPUs to drop to zero as this can cause a deadlock.
	m_allowZeroActiveSpus = false;

}


hkDemo::Result RaycastFilteringDemo::stepDemo()
{
	if (m_jobThreadPool->getNumThreads() == 0)
	{
		 HK_WARN(0x34561f23, "This demo does not run with only one thread");
		 return DEMO_STOP;
	}

	m_world->lock();

	//
	// shoot two times four colored rays.
	//  1. each ray should only hit the object with the same color
	//  2. each ray should only hit the triangles (which are at the same x-ccord as the object)
	//
	m_time += m_timestep * 3.0f;
	int colors[12] = {	hkColor::RED, hkColor::GREEN, hkColor::BLUE, hkColor::YELLOW,
						hkColor::RED, hkColor::GREEN, hkColor::BLUE, hkColor::YELLOW,
						hkColor::RED, hkColor::GREEN, hkColor::BLUE, hkColor::YELLOW };

	int numCommands = 12;
	int currentCommand = 0;

	hkpWorldRayCastOutput*     rayOutputs = hkAllocateChunk<hkpWorldRayCastOutput>(numCommands, HK_MEMORY_CLASS_DEMO);
	hkpWorldRayCastCommand*      commands = hkAllocateChunk<hkpWorldRayCastCommand>(numCommands, HK_MEMORY_CLASS_DEMO);

	for (int i = -1; i <= 3 ; i+=2 )
	{
		for (int j = 0; j < 4; j++ )
		{
			hkpWorldRayCastCommand* command = &commands[currentCommand];

			//	Setup the ray coordinates
			hkpWorldRayCastInput& ray = command->m_rayInput;
			{
				ray.m_from.set(-4.f, 0.f, i*5.0f + j * 2.0f - 3.0f);
				ray.m_to.set ( 25.f, 0.f, i*5.0f + j * 2.0f - 3.0f + 0.5f * hkMath::cos(m_time));
				ray.m_filterInfo = hkpGroupFilter::calcFilterInfo(j+1);
				ray.m_enableShapeCollectionFilter = true;
			}

			// setup the output structure
			{
				command->m_results = rayOutputs + currentCommand;
				command->m_resultsCapacity = 1;
				command->m_numResultsOut = 0;
				currentCommand++;
			}
		}
	}

	//
	//	Do the raycaster
	//
	{
		// setup hkpWorldRayCastJob
		hkpCollisionQueryJobHeader* jobHeader = hkAllocateChunk<hkpCollisionQueryJobHeader>(1, HK_MEMORY_CLASS_DEMO);
		hkpWorldRayCastJob worldRayCastJob(m_world->getCollisionInput(), jobHeader, commands, numCommands, m_world->m_broadPhase, &m_semaphore);

		// Unlock the world before processing the jobs
		m_world->unlock();

		//
		// Put the job on the queue, kick-off the PPU/SPU threads and wait for everything to finish.
		//
		{
			worldRayCastJob.setRunsOnSpuOrPpu();
			m_jobQueue->addJob( worldRayCastJob, hkJobQueue::JOB_LOW_PRIORITY );

			m_jobThreadPool->processAllJobs( m_jobQueue );
			m_jobThreadPool->waitForCompletion();

			m_semaphore.acquire();
		}
		hkDeallocateChunk(jobHeader, 1, HK_MEMORY_CLASS_DEMO);
	}


	//
	//	Check the hits
	//
	{
		for (int i =0; i < numCommands; i++)
		{
			hkpWorldRayCastCommand* command = &commands[i];
			hkpWorldRayCastInput& ray = command->m_rayInput;
			hkpWorldRayCastOutput& output = rayOutputs[i];

			hkVector4 intersectionPointWorld;
			intersectionPointWorld.setInterpolate4( ray.m_from, ray.m_to, output.m_hitFraction );		
			HK_DISPLAY_LINE( ray.m_from, intersectionPointWorld, colors[i]);
		}
	}


	//
	// Free temporarily allocated memory.
	//
	hkDeallocateChunk(commands,  numCommands, HK_MEMORY_CLASS_DEMO);
	hkDeallocateChunk(rayOutputs, numCommands, HK_MEMORY_CLASS_DEMO);


	return hkDefaultPhysicsDemo::stepDemo();
}



////////////////////////////////////////////////////////////////////
#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
"This illustrates the use of raycast filtering using: \n" \
"  a. An hkpMeshShape where only selected triangles are hit. \n" \
"  b. 12 objects using different collision layers. \n" \
"  c. 4 list shapes with 4 boxes each.";

HK_DECLARE_DEMO(RaycastFilteringDemo, HK_DEMO_TYPE_PRIME, "Raycast filtering", helpString);

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
