/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


#include <Demos/demos.h>

#include <Physics/Collide/Query/Collector/PointCollector/hkpFixedBufferCdPointCollector.h>
#include <Physics/Collide/Query/Multithreaded/CollisionQuery/hkpCollisionQueryJobs.h>

#include <Graphics/Bridge/DisplayHandler/hkgDisplayHandler.h>

#include <Physics/Utilities/VisualDebugger/Viewer/hkpShapeDisplayBuilder.h>

#include <Common/Visualize/hkDebugDisplay.h>
#include <Common/Visualize/Shape/hkDisplayGeometry.h>

#include <Demos/DemoCommon/Utilities/GameUtils/GameUtils.h>

#include <Demos/Physics/Api/Collide/AsynchronousQueries/PairLinearCastMultithreading/PairLinearCastMultithreadingApiDemo.h>

#include <Physics/Collide/Query/Multithreaded/CollisionQuery/hkpCollisionQueryJobQueueUtils.h>
#include <Common/Base/Thread/Job/ThreadPool/Cpu/hkCpuJobThreadPool.h>


#if defined(HK_PLATFORM_PS3_PPU)
//	# of real SPUs
#	define NUM_SPUS 6
#else
//	# of simulated SPUs
#	define NUM_SPUS 1
#endif


struct PairLinearCastMultithreadingApiDemoVariant
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
	"This demo demonstrates how to perform a linear cast in a multithreaded "							\
	"environment through the use of a dedicated hkpPairLinearCastJob. By putting such a job on "			\
	"the job queue it can be processed by other threads or SPUs (on PS3). "								\
	"One command of this job will cast exactly one collidable against another collidable along a "		\
	"predefined path. You can place an arbitrary number of commands into one job, as the job will "		\
	"automatically split itself into parallel sub-jobs. ";


static const PairLinearCastMultithreadingApiDemoVariant g_PairLinearCastMultithreadingApiDemoVariants[] =
{
	{ "Multithreaded",				PairLinearCastMultithreadingApiDemoVariant::MULTITHREADED_ON_PPU,			helpString },
};


PairLinearCastMultithreadingApiDemo::PairLinearCastMultithreadingApiDemo(hkDemoEnvironment* env)
	: hkDefaultPhysicsDemo(env, DEMO_FLAGS_NO_SERIALIZE),
	m_semaphore(0,1000),
	m_time(0.0f)
{
	const PairLinearCastMultithreadingApiDemoVariant& variant = g_PairLinearCastMultithreadingApiDemoVariants[m_variantId];

	//
	// Setup the camera.
	//
	{
		hkVector4 from(0.0f, 45.0f, 10.0f);
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
			info.setupSolverInfo(hkpWorldCinfo::SOLVER_TYPE_4ITERS_MEDIUM);
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
		hkVector4 boxSize1(1,1,2);
		hkpBoxShape* box1 = new hkpBoxShape(boxSize1);

		hkVector4 boxSize2(2,1,1);
		hkpBoxShape* box2 = new hkpBoxShape(boxSize2);

		hkpShape* list[2] = {box1, box2};
		listShape = new hkpListShape(list, 2);

		box1->removeReference();
		box2->removeReference();
	}

	//
	// Create a the body that we'll linear cast
	//
	{
		hkVector4 position(0.0f, 0.0f, 20.0f);
		m_castBody = GameUtils::createRigidBody(listShape, 0.0f, position);
		m_world->addEntity(m_castBody);
		m_castBody->removeReference();
	}

	

	{
		hkpRigidBodyCinfo rbinfo;

		hkVector4 position(0.0f, 0.0f, 5.0f);
		
		m_targetBody = GameUtils::createRigidBody(listShape, 0.0f, position);

		hkVector4 axis(0,1,0);
		axis.normalize3();
		hkQuaternion rotation(axis, HK_REAL_PI * 0.25f );
		m_targetBody->setRotation(rotation);

		m_world->addEntity(m_targetBody);
		m_targetBody->removeReference();
		listShape->removeReference();
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

	{
		hkpGroupFilter* filter = new hkpGroupFilter();
		m_world->setCollisionFilter(filter);
		filter->removeReference();
	}

	m_world->unlock();

	// Special case for this demo variant: we do not allow the # of active SPUs to drop to zero as this can cause a deadlock.
	if ( variant.m_demoType == PairLinearCastMultithreadingApiDemoVariant::MULTITHREADED_ON_SPU ) m_allowZeroActiveSpus = false;

	// Register the collision query functions
	hkpCollisionQueryJobQueueUtils::registerWithJobQueue(m_jobQueue);

	// register the default addCdPoint() function; you are free to register your own implementation here though
	hkpFixedBufferCdPointCollector::registerDefaultAddCdPointFunction();

}


PairLinearCastMultithreadingApiDemo::~PairLinearCastMultithreadingApiDemo()
{

	m_env->m_displayHandler->removeGeometry(1, 0, 0);
}


hkDemo::Result PairLinearCastMultithreadingApiDemo::stepDemo()
{
//	const PairLinearCastMultithreadingApiDemoVariant& variant = g_PairLinearCastMultithreadingApiDemoVariants[m_variantId];

	//
	// Setup the output array where the resulting collision points will be returned.
	//
	hkpRootCdPoint* collisionPoints = hkAllocateChunk<hkpRootCdPoint>(1, HK_MEMORY_CLASS_DEMO);

	//
	// Setup the hkpPairLinearCastCommand command.
	//
	hkpPairLinearCastCommand* command;
	{
		command = hkAllocateChunk<hkpPairLinearCastCommand>(1, HK_MEMORY_CLASS_DEMO);

		// Init input data.
		{
			// hkpWorldObject::getCollidable() needs a read-lock on the object
			m_castBody->markForRead();
			m_targetBody->markForRead();

			command->m_collidableA = m_castBody->getCollidable();
			command->m_collidableB = m_targetBody->getCollidable();

			m_targetBody->unmarkForRead();
			m_castBody->unmarkForRead();

			// We perform a linear cast of the first object moving towards the second.
			command->m_from = m_castBody->getPosition();
			command->m_to   = m_targetBody->getPosition();

			//
			// Move around target position.
			//
			hkReal xShift;
			//hkReal yShift;
			{
				m_time += m_timestep;
				xShift = 5.5f * hkMath::sin(m_time * 1.0f);
				//yShift = 5.5f * hkMath::cos(m_time * 1.0f);
			}
			command->m_to(0) += xShift;
			//command->m_to(1) += yShift;
			command->m_to(2) -= 5.0f;
		}

		// Init output data.
		{
			command->m_results			= collisionPoints;
			command->m_resultsCapacity	= 1;
			command->m_numResultsOut	= 0;

			command->m_startPointResults = HK_NULL;
			command->m_startPointResultsCapacity = 0;
			command->m_startPointNumResultsOut = 0;
		}
	}

	//
	// create the job header
	//
	hkpCollisionQueryJobHeader* jobHeader;
	{
		jobHeader = hkAllocateChunk<hkpCollisionQueryJobHeader>(1, HK_MEMORY_CLASS_DEMO);
	}

	//
	// Setup hkpPairLinearCastJob.
	//
	hkReal queryTolerance = 100.0f;
	m_world->markForRead();
	hkpPairLinearCastJob pairLinearCastJob(m_world->getCollisionInput(), jobHeader, command, 1, m_world->m_collisionInput->m_filter, queryTolerance, &m_semaphore);
	m_world->unmarkForRead();

	//
	// Put the job on the queue, kick-off the PPU/SPU threads and wait for everything to finish.
	//
	{
		//
		// Put the pairGetClosestPointsJob on the job queue.
		//
		pairLinearCastJob.setRunsOnSpuOrPpu();
		m_jobQueue->addJob( pairLinearCastJob, hkJobQueue::JOB_LOW_PRIORITY );

		m_jobThreadPool->processAllJobs( m_jobQueue );
		m_jobThreadPool->waitForCompletion();

		//
		// Wait for the one single job we started to finish.
		//
		m_semaphore.acquire();

	}

	//
	// Display results.
	//
	{
		hkVector4 from	= command->m_from;
		hkVector4 to	= command->m_to;
		hkVector4 path;
		path.setSub4(to, from);

		if ( command->m_numResultsOut > 0 )
		{
			for ( int r = 0; r < command->m_numResultsOut; r++)
			{
				const hkContactPoint& contactPoint = command->m_results[r].m_contact;

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
				// Display sphere's position at 'time of collision'.
				//
				{
					hkTransform castTransform = command->m_collidableA->getTransform();
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
				hkTransform castTransform = command->m_collidableA->getTransform();
				m_env->m_displayHandler->updateGeometry(castTransform, 1, 0);
			}
		}
	}

	//
	// Free temporarily allocated memory.
	//
	hkDeallocateChunk(jobHeader,       1, HK_MEMORY_CLASS_DEMO);
	hkDeallocateChunk(command,         1, HK_MEMORY_CLASS_DEMO);
	hkDeallocateChunk(collisionPoints, 1, HK_MEMORY_CLASS_DEMO);


	return hkDefaultPhysicsDemo::stepDemo();
}


#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif


HK_DECLARE_DEMO_VARIANT_USING_STRUCT( PairLinearCastMultithreadingApiDemo, HK_DEMO_TYPE_OTHER, PairLinearCastMultithreadingApiDemoVariant, g_PairLinearCastMultithreadingApiDemoVariants, HK_NULL );

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
