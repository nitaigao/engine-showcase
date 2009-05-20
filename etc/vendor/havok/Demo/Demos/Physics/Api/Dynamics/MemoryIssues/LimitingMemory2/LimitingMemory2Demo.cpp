/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/Physics/Api/Dynamics/MemoryIssues/LimitingMemory2/LimitingMemory2Demo.h>
#include <Demos/DemoCommon/Utilities/GameUtils/Landscape/FlatLand.h>

#include <Physics/Dynamics/World/hkpSimulationIsland.h>
#include <Physics/Dynamics/World/Memory/Default/hkpDefaultWorldMemoryWatchDog.h>
#include <Physics/Dynamics/Constraint/Bilateral/StiffSpring/hkpStiffSpringConstraintData.h>
#include <Physics/Dynamics/Action/hkpBinaryAction.h>

#include <Physics/Collide/Shape/HeightField/SampledHeightField/hkpSampledHeightFieldShape.h>
#include <Physics/Collide/Shape/Deprecated/Mesh/hkpMeshShape.h>
#include <Physics/Collide/Shape/Compound/Tree/Mopp/hkpMoppBvTreeShape.h>
#include <Physics/Collide/Shape/Compound/Tree/Mopp/hkpMoppUtility.h>
#include <Physics/Internal/Collide/Mopp/Code/hkpMoppCode.h>



struct hkLimitingMemory2DemoVariant
{
	const char*							m_name;
	hkpWorldCinfo::SimulationType		m_simulationType;
	hkpCollidableQualityType				m_objectsQualityType;
	hkReal								m_initialVerticalVelocity;
	int									m_numFramesToWaitBeforeAddingConstraintBatch;
	bool								m_createMopp;
	const char*							m_details;
};

#define DISC hkpWorldCinfo::SIMULATION_TYPE_DISCRETE
#define CONT hkpWorldCinfo::SIMULATION_TYPE_CONTINUOUS



static const hkLimitingMemory2DemoVariant g_variants[] =
{
	// help text																        type, qualityType                 velocity delay, mopp
	{ "Broad-ph. coll. det. in Psi steps, use debris   objects, discrete",              DISC, HK_COLLIDABLE_QUALITY_DEBRIS,        0.0f,  -1, 0, HK_NULL },
	{ "Broad-ph. coll. det. in Psi steps, use debris   objects, continuous",            CONT, HK_COLLIDABLE_QUALITY_DEBRIS,        0.0f,  -1, 0, HK_NULL },
	{ "Broad-ph. coll. det. in Psi steps, use moving   objects, continuous",            CONT, HK_COLLIDABLE_QUALITY_MOVING,        0.0f,  -1, 0, HK_NULL },

	{ "Integration in Psi steps, use debris objects, discrete",                         DISC, HK_COLLIDABLE_QUALITY_DEBRIS,        0.0f,   3, 0, HK_NULL },
	{ "Integration in Psi steps, use debris objects, continuous",                       CONT, HK_COLLIDABLE_QUALITY_DEBRIS,        0.0f,   3, 0, HK_NULL },

	{ "Mopp coll. det. in Psi steps, use debris objects, discrete physics",             DISC, HK_COLLIDABLE_QUALITY_DEBRIS,        0.0f,  -1, 1, HK_NULL },

};

#undef DISC
#undef CONT



class hkLimitingMemory2DemoAction : public hkpBinaryAction
{
	public:
		HK_DECLARE_REFLECTION();
		hkLimitingMemory2DemoAction( hkpEntity* entityA, hkpEntity* entityB, hkUint32 userData = 0 ): hkpBinaryAction(entityA, entityB, userData) {}
		void applyAction( const hkStepInfo& stepInfo ) {}

		hkpAction* clone( const hkArray<hkpEntity*>& entitiesIn, const hkArray<hkpPhantom*>& newPhantoms ) const { return HK_NULL; }
};


LimitingMemory2Demo::LimitingMemory2Demo(hkDemoEnvironment* env)
:	hkDefaultPhysicsDemo(env, DEMO_FLAGS_NO_SERIALIZE)
{
	// Disable warnings:									
	hkError::getInstance().setEnabled(0x7dd65995, false); //'The system has requested a heap allocation on stack overflow.'

	m_numFramesToWaitBeforeAddingConstraintBatch = g_variants[m_env->m_variantId].m_numFramesToWaitBeforeAddingConstraintBatch;
	m_landscape = HK_NULL;
	
	//
	// Setup the camera
	//
	{
		hkVector4 from(0.0f, 10.0f, 30.0f);
		hkVector4 to  (0.0f,  0.0f,  0.0f);
		hkVector4 up  (0.0f,  1.0f,  0.0f);
		this->setupDefaultCameras( env, from, to, up );

		// too many boxes normally for decent graphics update with shadows
		forceShadowState(false);

	}

	//
	// Create the world
	//
	{
		hkpWorldCinfo info;
		info.setBroadPhaseWorldSize(100.0f);
		info.setupSolverInfo( hkpWorldCinfo::SOLVER_TYPE_4ITERS_MEDIUM );
		info.m_simulationType = g_variants[m_env->m_variantId].m_simulationType;
		m_world = new hkpWorld( info );
		m_world->lock();

		// Register ALL agents (though some may not be necessary)
		hkpAgentRegisterUtil::registerAllAgents(m_world->getCollisionDispatcher());

		setupGraphics();
	}


	if (g_variants[m_env->m_variantId].m_createMopp)
	{
		const int dim = 64;
		const hkReal s = 0.3f;

		hkpRigidBodyCinfo groundInfo;
		FlatLand* fl = new FlatLand( dim );

		m_landscape = fl;
		hkVector4 scaling( s,0.0f,s );
		fl->setScaling( scaling );
		groundInfo.m_shape = fl->createMoppShape();
		groundInfo.m_position(1) = 0;


		//
		//  create the ground mopp
		//

		//boxInfo.m_shape = moppShape;

		groundInfo.m_position.set(0.0f,-1.6f,0.0f);
		groundInfo.m_motionType = hkpMotion::MOTION_FIXED;

		hkpRigidBody* baseRigidBody = new hkpRigidBody(groundInfo);
		groundInfo.m_shape->removeReference();

		//baseRigidBody->addCollisionListener( this );	// needed for changing friction values by callback

		m_world->addEntity( baseRigidBody );

		baseRigidBody->removeReference();
	}
	else
	{
		//
		// Create the base
		//

		hkVector4 baseRadii( 16.0f, 0.5f, 16.0f);
		hkpBoxShape* baseShape = new hkpBoxShape(baseRadii, 0 );

		hkpRigidBodyCinfo boxInfo;

		boxInfo.m_shape = baseShape;
		boxInfo.m_motionType = hkpMotion::MOTION_FIXED;
		boxInfo.m_position = hkVector4( 0.0f, -2.0f, 0.0f );
		boxInfo.m_collisionFilterInfo = hkpGroupFilter::calcFilterInfo(31);
		hkpRigidBody* baseRigidBody = new hkpRigidBody(boxInfo);
		m_world->addEntity( baseRigidBody );
		baseRigidBody->removeReference();
		baseShape->removeReference();
	}

	//
	// Create shapes needed
	//
	hkVector4 boxRadii( 0.4f, 0.4f, 0.4f);
	hkpBoxShape* cubeShape = new hkpBoxShape(boxRadii, 0 );


	//
	// Create a rigid body construction template
	//

	{
		//
		// create an array of pairs of boxes: The top box is moveable,
		// and the top two boxes use the collision filter

		int autoRemoveLevel = 10;
		int tmp = 0;

		for (int x = 0; x < 16; x++ )
		{
			for (int y = 0; y < 16; y++ )
			{

				// Create the bottom static box, using the hkpGroupFilter collision filter
				if (!g_variants[m_env->m_variantId].m_createMopp)
				{
						hkpRigidBodyCinfo boxInfo;
						boxInfo.m_shape = cubeShape;
						boxInfo.m_motionType = hkpMotion::MOTION_FIXED;
						boxInfo.m_inertiaTensor.setDiagonal(5.0f,5.0f,5.0f);
						boxInfo.m_mass = 5.0f;
						boxInfo.m_position.set( x - 7.5f, 2.4f, y - 7.5f );
						boxInfo.m_collisionFilterInfo = hkpGroupFilter::calcFilterInfo( x );
						hkpRigidBody* boxRigidBody = new hkpRigidBody(boxInfo);
						m_world->addEntity( boxRigidBody );
						boxRigidBody->removeReference();
				}

				// Create the top moving box, using the hkpGroupFilter collision filter
				{
						hkpRigidBodyCinfo boxInfo;
						boxInfo.m_shape = cubeShape;
						boxInfo.m_position.set( x - 7.5f, 3.6f + x * 0.2f + y * 0.2f, y - 7.5f );
						boxInfo.m_linearVelocity.set(0.0f, g_variants[m_env->m_variantId].m_initialVerticalVelocity, 0.0f);
						boxInfo.m_mass = 5.0f;
						boxInfo.m_motionType = hkpMotion::MOTION_SPHERE_INERTIA;
						boxInfo.m_qualityType = g_variants[m_env->m_variantId].m_objectsQualityType;
						boxInfo.m_collisionFilterInfo = hkpGroupFilter::calcFilterInfo( y );
						boxInfo.m_autoRemoveLevel = hkChar(autoRemoveLevel);
						hkpInertiaTensorComputer::setShapeVolumeMassProperties(boxInfo.m_shape, boxInfo.m_mass, boxInfo);
						hkpRigidBody* boxRigidBody = new hkpRigidBody(boxInfo);
						m_world->addEntity( boxRigidBody );
						boxRigidBody->removeReference();

						if (autoRemoveLevel < 128)
						{
							if (++tmp == 4)
							{
								autoRemoveLevel++;
								tmp = 0;
							}
						}
				}
			}
		}
	}
	cubeShape->removeReference();

	m_watchDog = 0;


	m_world->unlock();

	// Okay - the most straight forward thing to do now, would be to set the soft memory limit
	// You could do this by doing a garbage collect + then a calculateStatistics to find out 
	// how much memory is actually used. Then we can set the soft limit. This works fine in 
	// single threading mode, but in multi-threading mode, the threads consume a considerable 
	// amount of extra memory, which makes the physics fail.
	//
	// Instead of setting the soft limit immediately
	// it's set on the first call to stepDemo - at this point the threads are up and running
	// and have grabbed the memory they need.
	// 
	// To see where the soft limit is set, check out stepDemo method
	// 
	// In a real application this messiness is unnecessary as generally the soft limit it set to 
	// some pre-determined size appropriate for the application, as opposed to here, where the 
	// limit is set such that memory will fail - so as it can be demonstrated.
	
}


LimitingMemory2Demo::~LimitingMemory2Demo()
{
	// Re-enable warnings:									
	hkError::getInstance().setEnabled(0x7dd65995, true); 

    /// Remove the limit
    hkMemory::getInstance().setMemorySoftLimit(0);

	if ( m_landscape )
	{
		m_landscape->removeReference();
		m_landscape = HK_NULL;
	}

	if ( m_watchDog )
	{
		m_watchDog->removeReference();
		m_watchDog = HK_NULL;
	}
}
	// taken from MoppGame
hkpMoppBvTreeShape* LimitingMemory2Demo::createMoppShape()
{

	{
		for (short i = 0; i < NUM_VERTICES; i++ )
		{
			m_indices[i] = i;
			m_vertices[i*3 + 0] = (i&1) * 16.0f - 8.0f;
			m_vertices[i*3 + 1] = (i&3) * 0.3f;
			m_vertices[i*3 + 2] = i * 2.0f - NUM_VERTICES;
		}
	}



	hkpMeshShape* mesh = new hkpMeshShape;
	mesh->setRadius( 0.05f );

	{
		hkpMeshShape::Subpart part;

		part.m_vertexBase = m_vertices;
		part.m_vertexStriding = sizeof(float) * 3;
		part.m_numVertices = NUM_VERTICES;

		part.m_indexBase = m_indices;
		part.m_indexStriding = sizeof( hkUint16);
		part.m_numTriangles = NUM_TRIANGLES;
		part.m_stridingType = hkpMeshShape::INDICES_INT16;

		mesh->addSubpart( part );
	}

	hkpMoppCompilerInput mci;
	hkpMoppCode* code = hkpMoppUtility::buildCode( mesh ,mci);

	hkpMoppBvTreeShape* moppShape = new hkpMoppBvTreeShape( mesh, code );

	mesh->removeReference();
	code->removeReference();

	return moppShape;
}


hkDefaultPhysicsDemo::Result LimitingMemory2Demo::stepDemo()
{
	// The wiggle space is the amount of space that the physics needs to have to keep 
	// operating happily. If we set the soft memory limit to the actual memory limit we 
	// want, it may fail during simulation processing. We want to detect low memory, and remove
	// entities - not an acutal failure. So we try and ensure through the watch dog that there is always enough
	// 'wiggleSpace' available.

	const int wiggleSpace = 128*1024;
	
	// Set the critical memory limit to be 16 K greater than the current memory used. This memory will be
	// filled up by internal caches as the simulation progresses.
	// We do this here so that our calculations include building the multithreading util

	if ( !m_watchDog )
	{
		m_watchDog = new hkpDefaultWorldMemoryWatchDog(wiggleSpace);

		/// We want to know how much memory is really used - so do a collect
		hkMemory::getInstance().garbageCollect();
		/// Get the stats
		hkMemoryStatistics stats;
		hkMemory::getInstance().calculateStatistics(stats);
		
		// So in this arrangement we have 16k of memory which we will be using for physics processing (over and 
		// above havoks general memory consumption and the wiggle room)
		hkMemory::getInstance().setMemorySoftLimit(stats.m_used+ wiggleSpace + 16*1024);
	}

	{
		m_world->lock();

		if (m_numFramesToWaitBeforeAddingConstraintBatch-- == 0)
		{
			// Connect all simulation islands into one large island.
			const hkArray<hkpSimulationIsland*>& activeIslands = m_world->getActiveSimulationIslands();
			while(activeIslands.getSize() > 1)
			{
				hkpRigidBody* bodies[2];
				bodies[0] = static_cast<hkpRigidBody*>(activeIslands[0]->m_entities[0]);
				bodies[1] = static_cast<hkpRigidBody*>(activeIslands[1]->m_entities[0]);

				hkLimitingMemory2DemoAction* action = new hkLimitingMemory2DemoAction(bodies[0], bodies[1]);
				m_world->addAction(action);
				action->removeReference();
			}

		}

		m_world->unlock();
	}

	
	hkDefaultPhysicsDemo::stepDemo();
    if ( !hkMemory::getInstance().hasMemoryAvailable(wiggleSpace))
	{
		m_world->lock();
		m_watchDog->freeMemory(m_world);
		//hkMemory::getInstance().garbageCollect();
		m_world->unlock();
	}

	{
		hkMemoryStatistics stats;
		hkMemory::getInstance().calculateStatistics(stats);

		hkString str;
		str.printf("Allocated:     %d\nUsed:   %d\nAvailable:    %d\nTotal available:   %d\nLargest block: %d",
			stats.m_allocated,stats.m_used,stats.m_available,stats.m_totalAvailable,stats.m_largestBlock);

		const int& h = m_env->m_window->getHeight();
		m_env->m_textDisplay->outputText(str, 20, h-100);
	}

	return DEMO_OK;
}



#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
"This is a memory-capping example which presents/test the mechanism for out-of-memory events "\
"occurring both during PSI and TOI-event steps. When an out-of-memory event occurs during "\
"collision detection step, collision detection is aborted, and hkpSimulation::stepDeltaTime() "\
"returns the hkMemory::MEMORY_STATUS_OUT_OF_MEMORY value. Cleanup memory, optionally using "\
"hkWorldMemoryAvailableWatchDog::free() and run hkpSimulation::stepDeltaTime() aging. "\
"Note that frameDeltaTime parameter is ignored (assumed to be zero) in a call following an out-of-memory event.";

HK_DECLARE_DEMO_VARIANT_USING_STRUCT( LimitingMemory2Demo, HK_DEMO_TYPE_OTHER, hkLimitingMemory2DemoVariant, g_variants, HK_NULL);

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
