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
#include <Physics/Collide/Shape/Convex/Capsule/hkpCapsuleShape.h>

#include <Physics/Collide/Shape/Convex/ConvexTranslate/hkpConvexTranslateShape.h>
#include <Physics/Collide/Shape/Compound/Collection/List/hkpListShape.h>

#include <Graphics/Bridge/DisplayHandler/hkgDisplayHandler.h>

#include <Common/Base/Algorithm/PseudoRandom/hkPseudoRandomGenerator.h>

#include <Physics/Utilities/VisualDebugger/Viewer/hkpShapeDisplayBuilder.h>

#include <Common/Visualize/hkDebugDisplay.h>
#include <Common/Visualize/Shape/hkDisplayGeometry.h>

#include <Demos/DemoCommon/Utilities/GameUtils/GameUtils.h>
#include <Demos/Physics/Test/Feature/Collide/AsynchronousQueries/PairLinearCastMultithreaded/PairLinearCastMultithreadedDemo.h>

#include <Physics/Collide/Query/Multithreaded/CollisionQuery/hkpCollisionQueryJobQueueUtils.h>
#include <Common/Base/Thread/Job/ThreadPool/Cpu/hkCpuJobThreadPool.h>


#if defined(HK_PLATFORM_PS3_PPU)
//	# of real SPUs
#	define NUM_SPUS 6
#else
//	# of simulated SPUs
#	define NUM_SPUS 1
#endif


struct PairLinearCastMultithreadedDemoVariant
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
	"This examples calculates a series of linear casts (on the SPUs where available).\n" \
	"The colors indicate different collision layers.  \n" \
	"The collision filtering on SPU only applies to composite objects \n" \
	"Composite shapes only accept linear casts from shapes either black or of the same color \n"\
	"Simple shapes accept linear casts from all shapes\n"\
	"Red lines indicate an active linear cast, and black lines inactive ones";


static const PairLinearCastMultithreadedDemoVariant g_PairLinearCastMultithreadedDemoVariants[] =
{
	{ "Multithreaded",				PairLinearCastMultithreadedDemoVariant::MULTITHREADED_ON_PPU,			helpString },
};


PairLinearCastMultithreadedDemo::PairLinearCastMultithreadedDemo(hkDemoEnvironment* env)
	: hkDefaultPhysicsDemo(env, DEMO_FLAGS_NO_SERIALIZE),
	m_semaphore(0,1000),
	m_time(0.0f)
{
	const PairLinearCastMultithreadedDemoVariant& variant = g_PairLinearCastMultithreadedDemoVariants[m_variantId];

	// create world without any shapes that are unsupported on spu
	setupWorld(env, true);

	hkpCollisionQueryJobQueueUtils::registerWithJobQueue(m_jobQueue);

	// Special case for this demo variant: we do not allow the # of active SPUs to drop to zero as this can cause a deadlock.
	if ( variant.m_demoType == PairLinearCastMultithreadedDemoVariant::MULTITHREADED_ON_SPU ) m_allowZeroActiveSpus = false;

	// register the default addCdPoint() function; you are free to register your own implementation here though
	hkpFixedBufferCdPointCollector::registerDefaultAddCdPointFunction();
}


PairLinearCastMultithreadedDemo::~PairLinearCastMultithreadedDemo()
{

	int n = m_bodies.getSize();
	int numCasts = (n*(n-1))/2;

	for ( int i = 0; i < numCasts; i++ )
	{
		m_env->m_displayHandler->removeGeometry(4*i+1, 0, 0);
	}
}


void PairLinearCastMultithreadedDemo::setupWorld(hkDemoEnvironment* env, bool spuSupportedShapesOnly)
{
	// Disable face culling
	setGraphicsState(HKG_ENABLED_CULLFACE, false);

	//
	// Setup the camera
	//
	{
		hkVector4 from(0.0f, 25.0f, 10.0f);
		hkVector4 to(0.0f, 0.0f, 0.0f);
		hkVector4 up(0.0f, 1.0f, 0.0f);
		setupDefaultCameras( env, from, to, up );
	}


	//
	// Create the world
	//
	{
		hkpWorldCinfo info;
		info.setupSolverInfo(hkpWorldCinfo::SOLVER_TYPE_4ITERS_MEDIUM);
		info.setBroadPhaseWorldSize( 1000.0f );
		info.m_gravity.setZero4();
		info.m_enableDeactivation = false;
		info.m_contactPointGeneration = info.CONTACT_POINT_ACCEPT_ALWAYS;
		info.m_collisionTolerance = 0.0f;

		m_world = new hkpWorld( info );
		m_world->lock();

		setupGraphics();
	}

	//
	// Register the collision agents
	//
	{
		// Register ALL agents (though some may not be necessary)
		hkpAgentRegisterUtil::registerAllAgents(m_world->getCollisionDispatcher());
	}

	//
	//	Create the filter: every layer collides only with itself except layer 0, which collides with everything
	//
	{
		hkpGroupFilter* filter = new hkpGroupFilter();

		// disable all collisions by default
		filter->disableCollisionsUsingBitfield( 0xfffffffe, 0xfffffffe );

		for (int i = 0; i < 32; i++ )
		{
			filter->enableCollisionsBetween( i, i );
		}

		m_world->setCollisionFilter( filter );
		filter->removeReference();
	}

	//
	//	Create master template for some of our special shapes
	//
	hkpRigidBodyCinfo tmplt;
	tmplt.m_linearDamping = 1.0f;
	tmplt.m_angularDamping = 1.0f;

	// We use several different shape types in this demo, including hkBoxShapes, hkSphereShapes and hkCapsuleShapes.
	// For the purposes of this tutorial we'll only outline the creation process for the capsule shape. This is
	// given below:

	//	Create Capsule
	{
		// End points for the capsule
		hkVector4 A( 0.f, 1.2f, 0.f);
		hkVector4 B( 0.f,-1.2f, 0.f);

		// Radius for the capsule
		hkReal radius = 1.0f;

		hkpCapsuleShape* shape = new hkpCapsuleShape(A, B, radius);

		// Set up construction info for this rigid body
		hkpRigidBodyCinfo ci = tmplt;

		ci.m_motionType = hkpMotion::MOTION_SPHERE_INERTIA;
		ci.m_inertiaTensor.setDiagonal( 2.f, 2.f, 2.f );

		ci.m_mass = 1.0f;
		ci.m_shape = shape;
		ci.m_position.set(0,0,10);

		hkpRigidBody* rigidBody = new hkpRigidBody(ci);

		m_world->addEntity(rigidBody)->removeReference();
		m_bodies.pushBack(rigidBody);

		// Remove our reference to the shape
		shape->removeReference();
	}

	for (int k=0; k<3; k++)
	{
		//
		// Create a convex shape
		//
		{
			hkReal th = 2.0f*HK_REAL_PI*hkReal(k)/5.0f;

			hkVector4 pos; pos.set(10.0f*hkMath::cos(th),0,-10.0f*hkMath::sin(th));

			hkPseudoRandomGenerator generator(12);
			hkpRigidBody* body = GameUtils::createRandomConvexGeometric(1.0f, 1.0f, pos, 7, &generator);
			body->setCollisionFilterInfo(hkpGroupFilter::calcFilterInfo(k%3 + 1));

			m_world->addEntity(body);
			body->removeReference();
			m_bodies.pushBack(body);
		}
	}

	// Create convex list shapes made of 3 spheres
	//
	for (int k=0; k<5; k++)
	{
		hkpShape* listShape;
		{
			hkVector4 pos0( 0,0,.5f);
			hkVector4 pos1( 0,0,-.5f);
			hkVector4 pos2( 0,0.5f,0);
			hkpSphereShape* sphere = new hkpSphereShape( 0.3f );
			hkpConvexTranslateShape* cts0 = new hkpConvexTranslateShape( sphere, pos0);
			hkpConvexTranslateShape* cts1 = new hkpConvexTranslateShape( sphere, pos1);
			hkpConvexTranslateShape* cts2 = new hkpConvexTranslateShape( sphere, pos2);

			const hkpShape* shapes[3] = { cts0, cts1, cts2 };
			listShape = new hkpListShape( &shapes[0], 3 );
			for (hkpShapeKey key = ((hkpListShape*)listShape)->getFirstKey(); key != HK_INVALID_SHAPE_KEY; key = ((hkpListShape*)listShape)->getNextKey( key ) )
			{
				((hkpListShape*)listShape)->setCollisionFilterInfo(key,hkpGroupFilter::calcFilterInfo(k%3 + 1));
			}

			cts2->removeReference();
			cts1->removeReference();
			cts0->removeReference();
			sphere->removeReference();
		}

		hkpRigidBodyCinfo ci = tmplt;

		ci.m_motionType = hkpMotion::MOTION_SPHERE_INERTIA;
		ci.m_inertiaTensor.setDiagonal( 2.f, 2.f, 2.f );

		ci.m_mass = 1.0f;
		ci.m_shape = listShape;

		hkReal th = 2.0f*HK_REAL_PI*hkReal(k)/5.0f;
		hkVector4 pos;

		hkReal s = hkMath::sin(th);
		hkReal c = hkMath::cos(th);

		ci.m_position.set(-10.0f*s,0,-10.0f*c);

		hkpRigidBody* rigidBody = new hkpRigidBody(ci);

		// one listShape on one of 3 different layers

		rigidBody->setCollisionFilterInfo(hkpGroupFilter::calcFilterInfo(k%3 + 1));

		m_world->addEntity(rigidBody)->removeReference();

		m_bodies.pushBack(rigidBody);

		listShape->removeReference();
	}

	//
	// Create a Mopp
	//
	{
		hkpRigidBodyCinfo rigidBodyInfo;
		rigidBodyInfo.m_motionType = hkpMotion::MOTION_DYNAMIC;
		rigidBodyInfo.m_position.set( 0, 3, 0 );

		hkpMoppBvTreeShape* shape = GameUtils::createMoppShape(2);
		rigidBodyInfo.m_shape = shape;

		hkpRigidBody* rigidBody = new hkpRigidBody(rigidBodyInfo);
		m_world->addEntity(rigidBody)->removeReference();
		m_bodies.pushBack(rigidBody);
		shape->removeReference();
	}

	// Create display objects
	int k = 0;
	for (int i=0; i < m_bodies.getSize(); i++)
	{
		int color[] = {  hkColor::BLACK, hkColor::RED,  hkColor::GREEN, hkColor::BLUE };
		m_env->m_displayHandler->setGeometryColor(color[m_bodies[i]->getCollisionFilterInfo()], (hkUlong) m_bodies[i]->getCollidable(), 0);

		for (int j=i+1; j < m_bodies.getSize(); j++)
		{
			// for each command, create a display geometry
			// display IDs are 4k + 1

			hkArray<hkDisplayGeometry*> geom;
			hkpShapeDisplayBuilder::hkpShapeDisplayBuilderEnvironment sdbe;
			hkpShapeDisplayBuilder builder(sdbe);
			//builder.buildShapeDisplay(  m_bodies[i]->getCollidable()->getShape(), m_bodies[i]->getCollidable()->getTransform()	, geom );
			builder.buildShapeDisplay( m_bodies[i]->getCollidable()->getShape(), hkTransform::getIdentity()	, geom );

			// Id must not be divisible by four if we do not want it to be picked
			m_env->m_displayHandler->addGeometry(geom, m_bodies[i]->getCollidable()->getTransform() , 4*k + 1 , 0, 0 );
			m_env->m_displayHandler->setGeometryColor(color[m_bodies[i]->getCollisionFilterInfo()]&0x66ffffff, 4*k + 1, 0);
			k++;

			for (int g=0; g<geom.getSize(); g++)
			{
				delete geom[g];
			}
		}
	}

	m_world->unlock();
}


hkDemo::Result PairLinearCastMultithreadedDemo::stepDemo()
{
//	const PairLinearCastMultithreadedDemoVariant& variant = g_PairLinearCastMultithreadedDemoVariants[m_variantId];

	// the maximum number of 'linear cast hits' that will be returned
	const int maxNumResultsPerPair = 1;

	int numBodies = m_bodies.getSize();
	int numPairs = (numBodies*(numBodies-1))/2;
	hkArray<hkpRootCdPoint> resultsArray(numPairs*maxNumResultsPerPair);

	//
	// setup commands: one command for each object pair
	//
	hkArray<hkpPairLinearCastCommand> commands;
	commands.reserve(1); // this forces the actual array to be 16byte aligned!
	int resultIdx = 0;
	{
		for (int i=0; i < m_bodies.getSize(); i++)
		{
			for (int j=i+1; j < m_bodies.getSize(); j++)
			{
				hkpPairLinearCastCommand& command = commands.expandOne();
				{
					// hkpWorldObject::getCollidable() needs a read-lock on the object
					m_bodies[i]->markForRead();
					m_bodies[j]->markForRead();

					command.m_collidableA = m_bodies[i]->getCollidable();
					command.m_collidableB = m_bodies[j]->getCollidable();

					m_bodies[j]->unmarkForRead();
					m_bodies[i]->unmarkForRead();

					// We perform a linear cast of the first object moving towards the second.
					command.m_from = m_bodies[i]->getPosition();
					command.m_to   = m_bodies[j]->getPosition();

					// init output struct
					{
						command.m_results			= &resultsArray[resultIdx];
						command.m_resultsCapacity	= maxNumResultsPerPair;
						command.m_numResultsOut		= 0;

						command.m_startPointResults			= HK_NULL;
						command.m_startPointResultsCapacity = 0;
						command.m_startPointNumResultsOut	= 0;
					}
				}

				resultIdx += maxNumResultsPerPair;
			}
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
	hkpPairLinearCastJob pairLinearCastJob(m_world->getCollisionInput(), jobHeader, commands.begin(), commands.getSize(), m_world->m_collisionInput->m_filter, queryTolerance, &m_semaphore);
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

	static hkReal animationParam = 0.f;

	//
	// Display results.
	//
	{
		for ( int commandIdx = 0; commandIdx < commands.getSize(); commandIdx++)
		{
			hkpPairLinearCastCommand& command = commands[commandIdx];

			{
				for ( int r = 0; r < command.m_numResultsOut; r++)
				{
					const hkContactPoint& contactPoint = command.m_results[r].m_contact;

					hkVector4 from	= command.m_from;
					hkVector4 to	= command.m_to;
					hkVector4 path;
					path.setSub4(to, from);

					hkReal fraction = contactPoint.getDistance();
					if (fraction > 0.0)
					{
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
						// Display animated casted collidable as well as collision normal.
						//
						{
							hkTransform castTransform = command.m_collidableA->getTransform();

							hkVector4 animatedPoint;

							// goes exponentially from 0 to 1
							animatedPoint.setInterpolate4(from,pointOnPath, 1.0f - hkMath::pow(2,-100.0f*animationParam));
							castTransform.setTranslation(animatedPoint);

							HK_DISPLAY_ARROW( animatedPoint, contactPoint.getNormal(), hkColor::BLUE );

							// Update copies of the first object on the linear cast, getting exponentially closer to the contact point
							m_env->m_displayHandler->updateGeometry(castTransform, commandIdx*4+1, 0);
						}
					}
					else
					{
						// Display the whole linear cast line.
						HK_DISPLAY_LINE( from, to, hkColor::BLACK );

						// Display the casted collidable at cast's endpoint.
						hkTransform castTransform = command.m_collidableA->getTransform();
						m_env->m_displayHandler->updateGeometry(castTransform, commandIdx*4+1, 0);
					}
				}
			}
		}
	}

	//
	// Free temporarily allocated memory.
	//
	hkDeallocateChunk( jobHeader, 1, HK_MEMORY_CLASS_DEMO );

	// Animation resets in 10 seconds.
	animationParam += m_timestep/10.0f;

	// Resets the parameter at 1.0.
	animationParam = animationParam - floor(animationParam);

	return hkDefaultPhysicsDemo::stepDemo();
}


#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif


HK_DECLARE_DEMO_VARIANT_USING_STRUCT( PairLinearCastMultithreadedDemo, HK_DEMO_TYPE_OTHER, PairLinearCastMultithreadedDemoVariant, g_PairLinearCastMultithreadedDemoVariants, HK_NULL );

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
