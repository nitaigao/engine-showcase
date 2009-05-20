/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


#include <Demos/demos.h>

#include <Physics/Collide/Query/Multithreaded/RayCastQuery/hkpRayCastQueryJobs.h>

#include <Common/Visualize/hkDebugDisplay.h>

#include <Demos/Physics/Api/Collide/RayCasting/ShapeRayCastMultithreading/ShapeRaycastMultithreadingApiDemo.h>

#include <Physics/Collide/Query/Multithreaded/RayCastQuery/hkpRayCastQueryJobQueueUtils.h>
#include <Physics/Collide/Query/CastUtil/hkpWorldRayCastOutput.h>
#include <Common/Base/Thread/Job/ThreadPool/Cpu/hkCpuJobThreadPool.h>


#if defined(HK_PLATFORM_PS3_PPU)
//	# of real SPUs
#	define NUM_SPUS 6
#else
//	# of simulated SPUs
#	define NUM_SPUS 1
#endif


struct ShapeRayCastMultithreadingApiDemoVariant
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
	"This demo demonstrates how to emulate the hkpShape::castRay() functionality in a multithreaded "	\
	"environment through the use of a dedicated hkpShapeRayCastJob. By putting such a job on "			\
	"the job queue it can be processed by other threads or SPUs (on PS3). "								\
	"One command of this job will cast exactly one ray against an arbitrary number of collidables. "	\
	"You can place an arbitrary number of commands into one job, as the job will automatically split "	\
	"itself into parallel sub-jobs. "																	\
	"For more information on the original hkpShape::castRay() functionality, see ShapeRayCastDemo. ";

static const ShapeRayCastMultithreadingApiDemoVariant g_ShapeRayCastMultithreadingApiDemoVariants[] =
{
	{ "Multithreaded",				ShapeRayCastMultithreadingApiDemoVariant::MULTITHREADED_ON_PPU,			helpString },
};


ShapeRayCastMultithreadingApiDemo::ShapeRayCastMultithreadingApiDemo(hkDemoEnvironment* env) : hkDefaultPhysicsDemo(env, DEMO_FLAGS_NO_SERIALIZE),
	m_semaphore(0, 1000)
{
	const ShapeRayCastMultithreadingApiDemoVariant& variant = g_ShapeRayCastMultithreadingApiDemoVariants[m_variantId];

	// Disable backface culling
	setGraphicsState(HKG_ENABLED_CULLFACE, false);
	
	// Setup the camera.
	{
		hkVector4 from(-6.0f, 1.0f, 10.0f);
		hkVector4 to  (2.0f, 0.0f, 0.0f);
		hkVector4 up  (0.0f, 1.0f, 0.0f);
		setupDefaultCameras(env, from, to, up);
	}

	// Create the world.
	{
		hkpWorldCinfo info;
		
		// Set gravity to zero so body floats.
		info.setBroadPhaseWorldSize( 100.0f );
		m_world = new hkpWorld(info);
		m_world->lock();

		m_world->setGravity(hkVector4::getZero());

		setupGraphics();
	}

	//
	// Create a simple box to cast our ray against.
	//
	{
		hkpBoxShape* boxShape;
		{
			hkVector4 oneOneOne = hkVector4(1.0f, 1.0f, 1.0f);
			boxShape = new hkpBoxShape(oneOneOne, 0 );
		}

		hkpRigidBodyCinfo rigidBodyInfo;
		{
			rigidBodyInfo.m_angularDamping	= 0;
			rigidBodyInfo.m_angularVelocity	. set(.05f, .25f, .5f);
			rigidBodyInfo.m_motionType		= hkpMotion::MOTION_DYNAMIC;
			rigidBodyInfo.m_position		. set(0.0f, 0.0f, 0.0f);
			rigidBodyInfo.m_shape			= boxShape;
		}

		m_box = new hkpRigidBody(rigidBodyInfo);

		boxShape->removeReference();

		m_world->addEntity(m_box);
	}

	m_world->unlock();


	//
	// Setup multithreading.
	//
	hkpRayCastQueryJobQueueUtils::registerWithJobQueue(m_jobQueue);

	// Special case for this demo variant: we do not allow the # of active SPUs to drop to zero as this can cause a deadlock.
	if ( variant.m_demoType == ShapeRayCastMultithreadingApiDemoVariant::MULTITHREADED_ON_SPU ) m_allowZeroActiveSpus = false;

}



hkDemo::Result ShapeRayCastMultithreadingApiDemo::stepDemo()
{
//	const ShapeRayCastMultithreadingApiDemoVariant& variant = g_ShapeRayCastMultithreadingApiDemoVariants[m_variantId];

	//
	// Get the box's collidable.
	//
	const hkpCollidable* boxCollidable;
	{
		// hkpWorldObject::getCollidable() needs a read-lock on the object
		m_box->markForRead();
		boxCollidable = m_box->getCollidable();
		m_box->unmarkForRead();
	}

	//
	// Setup the array of collidables which we want to cast our ray against.
	// In this case we only have one collidable.
	//
	const hkpCollidable** collidableArray;
	{
		collidableArray	   = hkAlignedAllocate<const hkpCollidable*>(16, 1, HK_MEMORY_CLASS_DEMO);
		collidableArray[0] = boxCollidable;
	}

	//
	// Setup the output array where the ray's results (in this case only one) will be returned.
	//
	hkpWorldRayCastOutput* rayOutput = hkAllocateChunk<hkpWorldRayCastOutput>(1, HK_MEMORY_CLASS_DEMO);

	//
	// Setup the raycast command.
	//
	hkpShapeRayCastCommand* command;
	{
		command = hkAllocateChunk<hkpShapeRayCastCommand>(1, HK_MEMORY_CLASS_DEMO);

		// Init shape data.
		{
			command->m_collidables	  = collidableArray;
			command->m_numCollidables = 1;
		}

		// Init ray data.
		{
			command->m_rayInput.m_from					   . set(0.0f, 0.0f, 15.0f);
			command->m_rayInput.m_to					   . set(0.0f, 0.0f, 0.0f);
			command->m_rayInput.m_rayShapeCollectionFilter = HK_NULL;
			command->m_rayInput.m_filterInfo			   = 0;
			command->m_filterType = hkpCollisionFilter::HK_FILTER_UNKNOWN;
			command->m_filterSize = 0;
		}

		// Init output struct.
		{
			command->m_results		   = rayOutput;
			command->m_resultsCapacity = 1;
			command->m_numResultsOut   = 0;
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
	// Setup the raycast job.
	//
	m_world->markForRead();
	hkpShapeRayCastJob shapeRayCastJob(m_world->getCollisionInput(), jobHeader, command, 1, &m_semaphore);
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
		// Display results (just one in this case).
		if ( command->m_numResultsOut > 0 )
		{
			hkpWorldRayCastOutput* output = &command->m_results[0];

			hkVector4 intersectionPointWorld;
			intersectionPointWorld.setInterpolate4(command->m_rayInput.m_from, command->m_rayInput.m_to, output->m_hitFraction );

			// Display hitting ray in RED.
			HK_DISPLAY_LINE(command->m_rayInput.m_from, intersectionPointWorld, hkColor::RED);

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
		}
		else
		{
			// Draw infinite ray as GREY.
			HK_DISPLAY_LINE(command->m_rayInput.m_from, command->m_rayInput.m_to, hkColor::rgbFromChars(200, 200, 200));
		}
	}

	//
	// Free temporarily allocated memory.
	//
	hkDeallocateChunk(jobHeader, 1, HK_MEMORY_CLASS_DEMO);
	hkDeallocateChunk(command,   1, HK_MEMORY_CLASS_DEMO);
	hkDeallocateChunk(rayOutput, 1, HK_MEMORY_CLASS_DEMO);
	hkAlignedDeallocate(collidableArray);


	return hkDefaultPhysicsDemo::stepDemo();
}


#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif


HK_DECLARE_DEMO_VARIANT_USING_STRUCT( ShapeRayCastMultithreadingApiDemo, HK_DEMO_TYPE_OTHER, ShapeRayCastMultithreadingApiDemoVariant, g_ShapeRayCastMultithreadingApiDemoVariants, HK_NULL );

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
