/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


#include <Demos/demos.h>
#include <Demos/Physics/Api/Collide/AsynchronousQueries/WorldLinearCastMultithreading/WorldLinearCastMultithreadingApiDemo.h>

#include <Physics/Collide/Query/Collector/PointCollector/hkpFixedBufferCdPointCollector.h>
#include <Physics/Collide/Query/Multithreaded/CollisionQuery/hkpCollisionQueryJobs.h>

#include <Graphics/Bridge/DisplayHandler/hkgDisplayHandler.h>

#include <Physics/Utilities/VisualDebugger/Viewer/hkpShapeDisplayBuilder.h>

#include <Common/Visualize/hkDebugDisplay.h>
#include <Common/Visualize/Shape/hkDisplayGeometry.h>

#include <Demos/DemoCommon/Utilities/GameUtils/GameUtils.h>

#include <Physics/Collide/Query/Multithreaded/CollisionQuery/hkpCollisionQueryJobQueueUtils.h>

// kd-tree includes
#include <Physics/Collide/Query/Multithreaded/RayCastQuery/hkpRayCastQueryJobs.h>
#include <Physics/Collide/Query/Multithreaded/RayCastQuery/hkpRayCastQueryJobQueueUtils.h>
#include <Physics/Dynamics/World/Util/KdTree/hkpKdTreeWorldManager.h>


#if defined(HK_PLATFORM_PS3_PPU)
//	# of real SPUs
#	define NUM_SPUS 6
#else
//	# of simulated SPUs
#	define NUM_SPUS 1
#endif

#define BOX_CIRCLE_RADIUS 15.0f
#define CAST_CIRCLE_RADIUS (BOX_CIRCLE_RADIUS + 5.0f)


struct WorldLinearCastMultithreadingApiDemoVariant
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
	"One bpCommand of this job will cast exactly one collidable into the world, using the broadphase to "	\
	"avoid expensive low-level intersection tests. You can place an arbitrary number of commands into "	\
	"one job, as the job will automatically split itself into parallel sub-jobs. "						\
	"For more information on the original hkpWorld::linearCast() functionality, see WorldRayCastDemo. ";


static const WorldLinearCastMultithreadingApiDemoVariant g_WorldLinearCastMultithreadingApiDemoVariants[] =
{
	{ "Multithreaded",				WorldLinearCastMultithreadingApiDemoVariant::MULTITHREADED_ON_PPU,			helpString },
};


WorldLinearCastMultithreadingApiDemo::WorldLinearCastMultithreadingApiDemo(hkDemoEnvironment* env)
	: hkDefaultPhysicsDemo(env, DEMO_FLAGS_NO_SERIALIZE),
	m_time(0.0f)
{
	const WorldLinearCastMultithreadingApiDemoVariant& variant = g_WorldLinearCastMultithreadingApiDemoVariants[m_variantId];

	//
	// Setup the camera.
	//
	{
		hkVector4 from(0.0f, 30.0f, 45.0f);
		hkVector4 to(0.0f, 0.0f, 5.0f);
		hkVector4 up(0.0f, 1.0f, 0.0f);
		setupDefaultCameras( env, from, to, up );
	}

	//
	// Create the world.
	//
	{
		hkpWorldCinfo info;
		{
			info.setBroadPhaseWorldSize( 1000.0f );
			info.m_gravity.setZero4();
		}

		m_world = new hkpWorld( info );
		m_world->lock();

		// Register ALL agents (though some may not be necessary)
		hkpAgentRegisterUtil::registerAllAgents(m_world->getCollisionDispatcher());

		setupGraphics();
	}

	//
	// Create a simple list shape.
	//

	hkpListShape* listShape;
	{
		hkVector4 boxSize1(1,2,1);
		hkpBoxShape* box1 = new hkpBoxShape(boxSize1);

		hkVector4 boxSize2(2,1,1);
		hkpBoxShape* box2 = new hkpBoxShape(boxSize2);

		hkpShape* list[2] = {box1, box2};
		listShape = new hkpListShape(list, 2);

		box1->removeReference();
		box2->removeReference();
	}

	{
		hkVector4 position(0.0f, 0.0f, 0.0f);
		m_castBody = GameUtils::createRigidBody(listShape, 0.0f, position);
		m_world->addEntity(m_castBody);
		m_castBody->removeReference();
	}

	//
	// Create a circle of simple list shapes.
	//
	{
		

		int numBoxesInCircle = 10;
		hkReal angleStep = (2.0f * HK_REAL_PI) / hkReal(numBoxesInCircle);
		hkReal radius = 15.0f;
		for (float angle = 0.0f; angle < 2.0f * HK_REAL_PI; angle += angleStep)
		{
			hkVector4 position(radius * hkMath::sin(angle), 0.0f, radius * hkMath::cos(angle));
			hkVector4 size(3.0f, 3.0f, 3.0f);
			hkpRigidBody* body = GameUtils::createRigidBody(listShape, 0.0f, position);

			m_world->addEntity(body);
			body->removeReference();
		}

		listShape->removeReference();
	}

	{
		hkpGroupFilter* filter = new hkpGroupFilter();
		m_world->setCollisionFilter(filter);
		filter->removeReference();
	}

	//
	// Some magic to create a second graphical representation of the cast body.
	//
	{
		hkArray<hkDisplayGeometry*> geom;
		hkpShapeDisplayBuilder::hkpShapeDisplayBuilderEnvironment sdbe;
		hkpShapeDisplayBuilder builder(sdbe);
		builder.buildShapeDisplay( m_castBody->getCollidable()->getShape(), hkTransform::getIdentity(), geom );
		m_env->m_displayHandler->addGeometry(geom, m_castBody->getCollidable()->getTransform(), 1 , 0, 0 );
		hkReferencedObject::removeReferences(geom.begin(), geom.getSize());
	}

	m_world->unlock();



	// Special case for this demo variant: we do not allow the # of active SPUs to drop to zero as this can cause a deadlock.
	if ( variant.m_demoType == WorldLinearCastMultithreadingApiDemoVariant::MULTITHREADED_ON_SPU ) m_allowZeroActiveSpus = false;

	// Register the collision query functions
	hkpCollisionQueryJobQueueUtils::registerWithJobQueue(m_jobQueue);
	hkpRayCastQueryJobQueueUtils::registerWithJobQueue(m_jobQueue);

	// register the default addCdPoint() function; you are free to register your own implementation here though
	hkpFixedBufferCdPointCollector::registerDefaultAddCdPointFunction();

}


WorldLinearCastMultithreadingApiDemo::~WorldLinearCastMultithreadingApiDemo()
{

	m_env->m_displayHandler->removeGeometry(1, 0, 0);
}

hkDemo::Result WorldLinearCastMultithreadingApiDemo::stepDemo()
{
//	const WorldLinearCastMultithreadingApiDemoVariant& variant = g_WorldLinearCastMultithreadingApiDemoVariants[m_variantId];

	//
	// Advance time (used for calculating the rotating linear cast path).
	//
	m_time += m_timestep;

	//
	// Setup the output array where the resulting collision points will be returned.
	//
	hkpRootCdPoint* bpCollisionPoints = hkAllocateChunk<hkpRootCdPoint>(1, HK_MEMORY_CLASS_DEMO);
	hkpRootCdPoint* kdCollisionPoints = hkAllocateChunk<hkpRootCdPoint>(1, HK_MEMORY_CLASS_DEMO);

	//
	// Setup the hkpWorldLinearCastCommand bpCommand.
	//
	hkpWorldLinearCastCommand* bpCommand;
	hkpWorldLinearCastCommand* kdCommand;
	{
		bpCommand = hkAllocateChunk<hkpWorldLinearCastCommand>(1, HK_MEMORY_CLASS_DEMO);
		kdCommand = hkAllocateChunk<hkpWorldLinearCastCommand>(1, HK_MEMORY_CLASS_DEMO);

		// Init input data.
		{
			bpCommand->m_input.m_to.setZero4();
			bpCommand->m_input.m_to(0) = (CAST_CIRCLE_RADIUS * hkMath::sin(m_time * 1.0f));
			bpCommand->m_input.m_to(2) = (CAST_CIRCLE_RADIUS * hkMath::cos(m_time * 1.0f));

			bpCommand->m_input.m_maxExtraPenetration = HK_REAL_EPSILON;
			bpCommand->m_input.m_startPointTolerance = HK_REAL_EPSILON;

			// hkpWorldObject::getCollidable() needs a read-lock on the object
			m_castBody->markForRead();
			bpCommand->m_collidable = m_castBody->getCollidable();
			m_castBody->unmarkForRead();

		}

		// Init output data.
		{
			bpCommand->m_results			= bpCollisionPoints;
			bpCommand->m_resultsCapacity	= 1;
			bpCommand->m_numResultsOut	= 0;
		}
		
		// Copy the data for the kd-tree command
		*kdCommand = *bpCommand;
		// Different results ptr and numResultsOut, but everything else is the same;
		kdCommand->m_results = kdCollisionPoints;
	}

	//
	// create the job header
	//
	hkpCollisionQueryJobHeader* bpJobHeader;
	hkpRayCastQueryJobHeader* kdJobHeader;
	{
		bpJobHeader = hkAllocateChunk<hkpCollisionQueryJobHeader>(1, HK_MEMORY_CLASS_DEMO);
		kdJobHeader = hkAllocateChunk<hkpRayCastQueryJobHeader>(1, HK_MEMORY_CLASS_DEMO);
	}

	//
	// Setup the jobs.
	//
	hkSemaphoreBusyWait* semaphore = new hkSemaphoreBusyWait(0, 1000); // must be allocated on heap, for SPU to work

	m_world->markForRead();
	hkpWorldLinearCastJob worldLinearCastJob( m_world->getCollisionInput(), bpJobHeader, bpCommand, 1, m_world->m_broadPhase, semaphore);
	m_world->unmarkForRead();
	

	m_world->markForWrite();
	m_world->m_kdTreeManager->updateFromWorld(m_jobQueue, m_jobThreadPool);
	hkpKdTreeLinearCastJob kdTreeLinearCastJob(m_world->getCollisionInput(), kdJobHeader, kdCommand, 1, semaphore);
	m_world->unmarkForWrite();
	
	{
		// The jobs support multiple trees, but we only have one in the world right now
		kdTreeLinearCastJob.m_numTrees = 1;
		kdTreeLinearCastJob.m_trees[0] = m_world->m_kdTreeManager->getTree();

		kdTreeLinearCastJob.setRunsOnSpuOrPpu();
	}
	

	//
	// Put the job on the queue, kick-off the PPU/SPU threads and wait for everything to finish.
	//
	{
		m_world->lockReadOnly();

		worldLinearCastJob.setRunsOnSpuOrPpu();
		m_jobQueue->addJob( worldLinearCastJob, hkJobQueue::JOB_LOW_PRIORITY );

		m_jobThreadPool->processAllJobs( m_jobQueue );
		m_jobThreadPool->waitForCompletion();

		//
		// Wait for the one single job we started to finish.
		//
		semaphore->acquire();

		m_world->unlockReadOnly();
	}

	//
	// Now do the same for the kd-tree job
	//
	{
		m_jobQueue->addJob(kdTreeLinearCastJob, hkJobQueue::JOB_HIGH_PRIORITY );

		m_jobThreadPool->processAllJobs( m_jobQueue );
		m_jobThreadPool->waitForCompletion();

		semaphore->acquire();
	}

	delete semaphore;


	//
	// Display results.
	//
	{
		hkVector4 from	= bpCommand->m_collidable->getTransform().getTranslation();
		hkVector4 to	= bpCommand->m_input.m_to;
		hkVector4 path;
		path.setSub4(to, from);

		if ( bpCommand->m_numResultsOut > 0 )
		{
			for ( int r = 0; r < bpCommand->m_numResultsOut; r++)
			{
				const hkContactPoint& contactPoint = bpCommand->m_results[r].m_contact;

				hkReal fraction = contactPoint.getDistance();

				// Calculate the position on the linear cast's path where the first collidable hit the second collidable.
				hkVector4 pointOnPath;
				{
					pointOnPath.setAddMul4( from, path, fraction );
				}

				// Draw the linear cast line from startpoint to hitpoint.
				HK_DISPLAY_LINE( from, pointOnPath, hkColor::RED );

				// Draw the collision normal.
				HK_DISPLAY_ARROW( pointOnPath, contactPoint.getNormal(), hkColor::BLUE );

				// Draw the linear cast line from hitpoint to endpoint.
				HK_DISPLAY_LINE( pointOnPath, to, hkColor::BLACK );

				//
				// Display body's position at 'time of collision'.
				//
				{
					hkTransform castTransform = bpCommand->m_collidable->getTransform();
					castTransform.setTranslation(pointOnPath);
					m_env->m_displayHandler->updateGeometry(castTransform, 1, 0);
				}
			}
		}
		else
		{
			// Display the whole linear cast line.
			HK_DISPLAY_LINE( from, to, hkColor::BLACK );

			// Display the casted collidable at cast's endpoint.
			{
				hkTransform castTransform = bpCommand->m_collidable->getTransform();
				m_env->m_displayHandler->updateGeometry(castTransform, 1, 0);
			}
		}
	}

	//
	// Free temporarily allocated memory.
	//
	hkDeallocateChunk(bpJobHeader,       1, HK_MEMORY_CLASS_DEMO);
	hkDeallocateChunk(bpCommand,         1, HK_MEMORY_CLASS_DEMO);
	hkDeallocateChunk(kdCommand,         1, HK_MEMORY_CLASS_DEMO);
	hkDeallocateChunk(bpCollisionPoints, 1, HK_MEMORY_CLASS_DEMO);
	hkDeallocateChunk(kdCollisionPoints, 1, HK_MEMORY_CLASS_DEMO);


	return hkDefaultPhysicsDemo::stepDemo();
}


#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif


HK_DECLARE_DEMO_VARIANT_USING_STRUCT( WorldLinearCastMultithreadingApiDemo, HK_DEMO_TYPE_OTHER, WorldLinearCastMultithreadingApiDemoVariant, g_WorldLinearCastMultithreadingApiDemoVariants, HK_NULL );

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
