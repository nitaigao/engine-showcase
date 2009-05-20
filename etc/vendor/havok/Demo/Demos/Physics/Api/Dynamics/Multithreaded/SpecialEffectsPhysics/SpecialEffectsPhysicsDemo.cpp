/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Common/Base/System/hkBaseSystem.h>
#include <Common/Base/Thread/CriticalSection/hkCriticalSection.h>
#include <Common/Base/Thread/Semaphore/hkSemaphore.h>
#include <Common/Base/Monitor/hkMonitorStream.h>

#include <Physics/Collide/Shape/Convex/Capsule/hkpCapsuleShape.h>
#include <Physics/Dynamics/World/hkpPhysicsSystem.h>
#include <Physics/Utilities/Constraint/Keyframe/hkpKeyFrameUtility.h>
#include <Physics/Utilities/VisualDebugger/Viewer/Collide/hkpShapeDisplayViewer.h>

#include <Common/Visualize/hkDebugDisplay.h>

#include <Demos/DemoCommon/Utilities/GameUtils/TankModel/TankModel.h>
#include <Demos/Physics/UseCase/ContinuousPhysics/BrickWall/BrickWallDemo.h>
#include <Demos/DemoCommon/Utilities/GameUtils/GameUtils.h>
#include <Demos/DemoCommon/Utilities/GameUtils/Landscape/FlatLand.h>
#include <Demos/Physics/Api/Dynamics/Multithreaded/SpecialEffectsPhysics/SpecialEffectsPhysicsDemo.h>

#include <Common/Base/Thread/Job/ThreadPool/Cpu/hkCpuJobThreadPool.h>
#include <Common/Base/Thread/JobQueue/hkJobQueue.h>

struct SpecialEffectsPhysicsDemoVariant
{
	const char*	 m_name;
	bool m_useMultithreaded;
	const char*  m_details;
};

static const char* detail = 
"This demo shows how to run 2 simulations: one for game physics and one for special effects";
static const SpecialEffectsPhysicsDemoVariant g_variants[] =
{
	{ "Single threaded",  false, detail },
	{ "Dual   threaded: GamePlay + SpecialEffects",true,  detail },
};


SpecialEffectsPhysicsDemo::SpecialEffectsPhysicsDemo(hkDemoEnvironment* env): hkDefaultPhysicsDemo( env )
{
	m_tank = HK_NULL;
	m_track = HK_NULL;

	const SpecialEffectsPhysicsDemoVariant& variant = g_variants[ this->getDemoVariant() ];

	//
	// Setup the camera
	//
	{
		hkVector4 from(0.0f, 0.0f, 1.0f);
		hkVector4 to  (0.0f, 0.0f, 0.0f);
		hkVector4 up(0.0f, 1.0f, 0.0f);
		setupDefaultCameras( env, from, to, up, 1.f, 1000.0f );
	}

	//	Create the world
	{
		hkpWorldCinfo info;
		info.m_gravity.set( 0.0f, -10.0f, 0.0f );
		info.m_simulationType = hkpWorldCinfo::SIMULATION_TYPE_CONTINUOUS;
		m_world = new hkpWorld( info );
		m_world->lock();


		hkJobQueueCinfo jobQueueInfo;
		m_specialEffectsJobQueue = new hkJobQueue(jobQueueInfo);
		info.m_simulationType = hkpWorldCinfo::SIMULATION_TYPE_MULTITHREADED;
		m_specialEffectsWorld = new hkpWorld( info );
		m_specialEffectsWorld->lock();

		m_specialEffectsWorld->markForWrite();

		// Register ALL agents (though some may not be necessary)
		hkpAgentRegisterUtil::registerAllAgents(m_world->getCollisionDispatcher());
		hkpAgentRegisterUtil::registerAllAgents(m_specialEffectsWorld->getCollisionDispatcher());

	}

	// Setup graphics.
	// This is the usual stuff except that we want to update the transforms 
	// of the graphics engine by hand. So we disable the default behaviour
	// of our shape display viewer
	//
	{
		setupGraphics();

		for (int i = 0; i < m_debugProcesses.getSize(); i++ )
		{
			if ( m_debugProcesses[i]->getProcessTag() == hkpShapeDisplayViewer::m_tag )
			{
				m_shapeDisplayViewer = static_cast<hkpShapeDisplayViewer*>(	m_debugProcesses[i] );		
			}
		}
	}

	//
	//	setup graphics for special effects world
	//
	{
		m_shapeDisplayViewer->worldAddedCallback( m_specialEffectsWorld );
	}

	//
	// Collision Filter
	//
	{ 
		hkpGroupFilter* filter = new hkpGroupFilter();
		filter->disableCollisionsBetween(1,1);
		m_world->setCollisionFilter(filter);
		m_specialEffectsWorld->setCollisionFilter(filter);
		filter->removeReference();
	}

	//
	//	Create our tank in our game engine world
	//
	{
		m_tank = new TankModel( m_world, hkVector4( -20,0,0 ), 10000.0f );
		
		// clone the tank in our special effects world
		cloneTankBodies( m_tank, m_specialEffectsWorld, m_proxyTank );
	}


	// create ground and add it to both worlds
	{
		hkpRigidBodyCinfo groundInfo;
		hkpShape* groundShape = createGround();
		groundInfo.m_shape = groundShape;
		groundInfo.m_position.set(0.0f, 0.0f, 0.0f);
		groundInfo.m_motionType = hkpMotion::MOTION_FIXED;
		groundInfo.m_friction = 2.0f;

		{
			hkpRigidBody* groundbody = new hkpRigidBody(groundInfo);
			m_world->addEntity( groundbody )->removeReference();
		}
		{
			hkpRigidBody* groundbody = new hkpRigidBody(groundInfo);
			// do not create a second graphics object
			groundbody->addProperty(HK_PROPERTY_DISPLAY_PTR, 1);
			m_specialEffectsWorld->addEntity( groundbody )->removeReference();
		}
		groundShape->removeReference();
	}

	//
	//	Create a series of brick walls
	//
	{
		hkVector4 pos(20,0,0);
		createWalls( m_specialEffectsWorld, pos, 7 );
	}

	m_specialEffectsWorld->unlock();

	//
	// Lets create our special effects physics thread
	//
	if ( variant.m_useMultithreaded )
	{
		hkCpuJobThreadPoolCinfo ci;
		ci.m_numThreads = 1;

		m_specialEffectsWorld->initMtStep( m_specialEffectsJobQueue, m_timestep );
		m_jobThreadPool = new hkCpuJobThreadPool(ci);

		m_jobThreadPool->processAllJobs( m_specialEffectsJobQueue );

	}

	m_world->unlock();
}




SpecialEffectsPhysicsDemo::~SpecialEffectsPhysicsDemo()
{
	if (m_jobThreadPool)
	{
		m_jobThreadPool->waitForCompletion();

		m_specialEffectsWorld->finishMtStep( m_specialEffectsJobQueue );
		//m_jobThreadPool deleted in hkDefaultPhysicsDemo();
	}

	m_shapeDisplayViewer->worldRemovedCallback( m_specialEffectsWorld );

	m_specialEffectsWorld->markForWrite();
	m_specialEffectsWorld->removeReference();
	m_specialEffectsWorld = HK_NULL;

	m_world->markForWrite();
	m_world->removeReference();
	m_world = HK_NULL;

	if ( m_tank )
	{
		delete m_tank;
	}

	if (m_track)
	{
		m_track->removeReference();
	}
}


hkDemo::Result SpecialEffectsPhysicsDemo::stepDemo()
{
	m_world->lock();

	//
	//	simulate game physics
	//
	{
		HK_TIMER_BEGIN("TankPhysics",HK_NULL);

		m_tank->stepTank( m_env, m_timestep );

		m_world->unlock();
		hkDefaultPhysicsDemo::stepDemo();
		m_world->lock();

		m_tank->updateCamera( m_env, m_timestep );

		m_shapeDisplayViewer->synchronizeTransforms( m_world );

		HK_TIMER_END();
	}

	//
	//	do special effects physics
	//
	{
		if ( m_jobThreadPool )
		{
				// wait for the thread to finish
			m_jobThreadPool->waitForCompletion();
			m_specialEffectsWorld->finishMtStep( m_specialEffectsJobQueue );

			m_specialEffectsWorld->lock();

			m_specialEffectsWorld->markForWrite();

				// get our transforms for the graphics
			m_shapeDisplayViewer->synchronizeTransforms( m_specialEffectsWorld );

				// Explicitly copy timer information from the physics threads to the main thread.
			addOrRemoveThreads();

				// apply key-frame information
			keyframeProxyTank(m_tank, m_specialEffectsWorld, m_proxyTank );

			m_specialEffectsWorld->unmarkForWrite();

			m_specialEffectsWorld->initMtStep( m_specialEffectsJobQueue, m_timestep );
			m_jobThreadPool->processAllJobs( m_specialEffectsJobQueue );

		}
		else
		{

			m_specialEffectsWorld->markForWrite();

			// using single thread
			// Note: we do the functions in the wrong order to simulate the delay caused by using multithreading
			m_specialEffectsWorld->lock();
			m_shapeDisplayViewer->synchronizeTransforms( m_specialEffectsWorld );
			keyframeProxyTank(m_tank, m_specialEffectsWorld, m_proxyTank );

			m_specialEffectsWorld->unlock();
			
			HK_TIMER_BEGIN("SpecialEffects", HK_NULL);
			m_specialEffectsWorld->stepDeltaTime( m_timestep );
			HK_TIMER_END();

			m_specialEffectsWorld->unmarkForWrite();
		}
	}

	m_world->unlock();

	return DEMO_OK;
}

//////////////////////////////////////////////////////////////////////////
//
//  Functions to copy/clone game-play objects from the game-play simulation to the 
//  visual effects simulation.
//
//////////////////////////////////////////////////////////////////////////

	// create a cloned body and turn it into a keyframe body,
	// this will replicate a game play body in the special effects physics
static hkpRigidBody* createProxyBody( hkpRigidBody* body, hkpWorld* world ) 
{
	hkpRigidBody* b = body->clone();
	b->setMotionType( hkpMotion::MOTION_KEYFRAMED );
	world->addEntity( b )->removeReference();
	HK_SET_OBJECT_COLOR( hkUlong( b->getCollidable()), 0x40000080 );
	return b;
}

	// clone and keyframe the whole tank
void SpecialEffectsPhysicsDemo::cloneTankBodies( TankModel* tank, hkpWorld* world, SpecialEffectsPhysicsDemo::TankBodies& proxyTank )
{
	proxyTank.m_body = createProxyBody(tank->m_body,world);
	proxyTank.m_skiL = createProxyBody(tank->m_skiL,world);
	proxyTank.m_skiR = createProxyBody(tank->m_skiR,world);
	proxyTank.m_gun  = createProxyBody(tank->m_gun,world);
	proxyTank.m_tower = createProxyBody(tank->m_tower,world);
}


	// take the transform of a rigid body and keyframe another
static void keyframeSingleBody( hkpRigidBody* source, hkpWorld* world, hkReal deltaTime, hkReal invDeltaTime, hkpRigidBody* dest )
{
	hkReal lookAheadTime = world->getCurrentTime() + deltaTime;
	hkTransform transform;
	source->approxTransformAt( lookAheadTime, transform );

	hkQuaternion q( transform.getRotation());
	hkpKeyFrameUtility::applyHardKeyFrame( transform.getTranslation(), q, invDeltaTime, dest );

}

	// keyframe the tank proxy in the special effects physics based on the transforms of the
	// tank in the game play physics
void SpecialEffectsPhysicsDemo::keyframeProxyTank( class TankModel* tank, hkpWorld* world, TankBodies& proxyTank )
{
	hkReal dt = 1.0f / 60.0f;
	hkReal invDt = 60.0f;
	keyframeSingleBody( tank->m_body, world, dt, invDt, proxyTank.m_body);
	keyframeSingleBody( tank->m_skiL, world, dt, invDt, proxyTank.m_skiL);
	keyframeSingleBody( tank->m_skiR, world, dt, invDt, proxyTank.m_skiR);
	keyframeSingleBody( tank->m_gun, world, dt, invDt, proxyTank.m_gun);
	keyframeSingleBody( tank->m_tower, world, dt, invDt, proxyTank.m_tower);

}


//////////////////////////////////////////////////////////////////////////
//
// World setup helper functions
//
//////////////////////////////////////////////////////////////////////////

void SpecialEffectsPhysicsDemo::createWalls( hkpWorld* world, const hkVector4& pos, int size )
{
	hkVector4 boxSize( 0.5f, 0.3f, 0.3f);
	hkpBoxShape* box = new hkpBoxShape( boxSize , 0 );

	for (int i = 0; i < 10; i++ )
	{		
		hkVector4 position = pos;
		position(2) += i * 10.0f;
		BrickWallDemo::createBrickWall( world, size, size, position, 0.2f, box, boxSize );
	}
	box->removeReference();
}

hkpShape* SpecialEffectsPhysicsDemo::createGround()
{
	FlatLand* fl = new FlatLand();
	fl->setScaling( hkVector4( 8.0f, 0.5f, 8.0f ) );
	m_track = fl;
	return fl->createMoppShape();
}


//////////////////////////////////////////////////////////////////////////
//
// Explicit display handling functions
//
//////////////////////////////////////////////////////////////////////////


void SpecialEffectsPhysicsDemo::setupContexts(hkArray<hkProcessContext*>& contexts)
{
	//
	//	Tell our shape display viewer not to update the transforms in a postSimulateCallback
	//
	hkpShapeDisplayViewer::ShapeDisplayViewerOptions* options = new hkpShapeDisplayViewer::ShapeDisplayViewerOptions();
	options->m_enableShapeTransformUpdate = false;
	contexts.pushBack( options );
	hkDefaultPhysicsDemo::setupContexts( contexts );
}



#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
"Lots of special effects physics running in a separate thread.";

//HK_DECLARE_DEMO_VARIANT_USING_STRUCT( SpecialEffectsPhysicsDemo, HK_DEMO_TYPE_PRIME, SpecialEffectsPhysicsDemoVariant, g_variants, helpString); 

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
