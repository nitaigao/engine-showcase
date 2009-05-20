/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/Physics/Api/Dynamics/MemoryIssues/LimitingMemory/LimitingMemoryDemo.h>
#include <Common/Base/Algorithm/PseudoRandom/hkPseudoRandomGenerator.h>
#include <Demos/DemoCommon/Utilities/GameUtils/GameUtils.h>

#include <Common/Base/DebugUtil/StatisticsCollector/Simple/hkSimpleStatisticsCollector.h>


#include <Physics/Collide/Shape/Convex/Capsule/hkpCapsuleShape.h>
#include <Physics/Collide/Shape/Compound/Tree/Mopp/hkpMoppBvTreeShape.h>
#include <Physics/Collide/Shape/Compound/Tree/Mopp/hkpMoppUtility.h>
#include <Physics/Collide/Shape/Compound/Collection/ExtendedMeshShape/hkpExtendedMeshShape.h>
#include <Common/Internal/ConvexHull/hkGeometryUtility.h>

#include <Common/Visualize/hkDebugDisplay.h>

#include <Common/Serialize/Util/hkBuiltinTypeRegistry.h>

#include <Demos/DemoCommon/DemoFramework/hkTextDisplay.h>


static const hkReal SPHERE_RADIUS = 3.0f;

struct LimitingMemoryDemoVariant
{
	const char*	 m_name;
	int m_numRagdolls;
	int m_memorySize;
	const char*  m_details;
};

static const char* detail =
"This demo shows what happens if you run out of memory: "	\
"hkRigidBodies (e.g. a ragdoll) with an " \
"m_autoRemoveLevel>0 will automatically be removed "	\
"from the physics to free extra memory.";
static const LimitingMemoryDemoVariant g_variants[] =
{
    { " 400k",   1, 400*1024, detail },
    { "1000k",  10, 1024*1024, detail },
};

/// This is the amount of extra space needed for normal operation
/// The value here is larger than single threaded operations needs - in multi threaded environment
/// each thread consumes a chunk of memory for caches, thus the larger value

static const int wiggleSize = 256*1024;


void MySimpleObjectWatchDog::freeMemory( hkpWorld* world )
{
    int targetAvailable = m_minMemoryAvailable;

	hkMemory& mem = hkMemory::getInstance();
	while (!mem.hasMemoryAvailable(targetAvailable))
	{
		if ( m_debris.getSize() > 0 )
		{
			// Check if the debris object has been removed elsewhere
			if (m_debris[0]->getWorld() != HK_NULL)
			{
				world->lock();
				world->removeEntity(m_debris[0]);
				world->unlock();
			}
			m_debris[0]->removeReference();
			m_debris.removeAtAndCopy(0);
		}
		else
		{
			if ( m_ragdolls.getSize() > 0 )
			{
				world->lock();
				// Check if the ragdoll object has been removed elsewhere (just check the first entity)
				if (m_ragdolls[0]->getRigidBodies()[0]->getWorld() != HK_NULL )
				{
					world->removePhysicsSystem(m_ragdolls[0]);
				}
				m_ragdolls[0]->removeReference();
				m_ragdolls.removeAtAndCopy(0);
				world->unlock();
			}
			else
			{
				break;
			}
		}
	}
}



MySimpleObjectWatchDog::~MySimpleObjectWatchDog()
{
	{
		for (int i = 0; i < m_debris.getSize(); ++i)
		{
			m_debris[i]->removeReference();
		}
	}
	{
		for (int i = 0; i < m_ragdolls.getSize(); ++i)
		{
			m_ragdolls[i]->removeReference();
		}
	}
}

// Create fixed body for the mopp
//
// Info: this function creates a fixed mopp, or a keyframed rotating mopp
void LimitingMemoryDemo::createHollowSphereMopp(hkpMotion::MotionType motionType)
{
	// generate points
	hkArray<hkVector4> points;
	hkPseudoRandomGenerator generator(747);
	{
		for (int i = 0; i < 300; i++)
		{
			hkVector4& vec = points.expandOne();
			generator.getRandomVector11(vec);
			vec(0) *= 3;
			vec(0) += 2;
			vec.normalize3();
		}
	}

	m_geom = new hkGeometry();
	{
		hkStridedVertices stridedVerts;
		{
			stridedVerts.m_numVertices = points.getSize();
			stridedVerts.m_striding = sizeof(points[0]);
			stridedVerts.m_vertices = reinterpret_cast<float*>(&points[0]);
		}

		hkArray<hkVector4> planeEquations;
		hkGeometryUtility::createConvexGeometry( stridedVerts, *m_geom, planeEquations );
	}

	// generate noise
	{
		for (int i = 0; i < m_geom->m_vertices.getSize(); i++)
		{
			hkVector4& vec = m_geom->m_vertices[i];

			hkVector4 dirY(0.0f, 1.0f, 0.0f);
			hkReal dot = (hkReal(dirY.dot3(vec)) + 1.0f) * 0.5f; // range [0,1]
			hkReal rand = generator.getRandRange(1.0f - dot * 0.1f, 1.0f + dot * 0.1f);
			vec.mul4(rand);
			vec.mul4(SPHERE_RADIUS);
		}
	}

	hkpMoppBvTreeShape* mopp;
	{
		hkpExtendedMeshShape* mesh = new hkpExtendedMeshShape();

		hkpExtendedMeshShape::TrianglesSubpart subPart;
		subPart.m_vertexBase = reinterpret_cast<float*>(&m_geom->m_vertices[0]);
		subPart.m_vertexStriding = sizeof(hkVector4);
		subPart.m_numVertices = m_geom->m_vertices.getSize();

		subPart.m_indexBase = &m_geom->m_triangles[0];
		switch( sizeof(m_geom->m_triangles[0].m_a) )
		{
		case 2: subPart.m_stridingType = hkpExtendedMeshShape::INDICES_INT16; break;
		case 4: subPart.m_stridingType = hkpExtendedMeshShape::INDICES_INT32; break;
		default: HK_ASSERT2(0xad000750, 0, "Unknown type used to index vertices.");
		}
		subPart.m_indexStriding = 3 * sizeof(m_geom->m_triangles[0].m_a);

		subPart.m_numTriangleShapes	= m_geom->m_triangles.getSize();
		mesh->addTrianglesSubpart(subPart);

		hkpMoppCompilerInput mci;
		hkpMoppCode* code = hkpMoppUtility::buildCode(mesh, mci);
		mopp = new hkpMoppBvTreeShape(mesh, code);

		mesh->removeReference();
		code->removeReference();
	}


	hkpRigidBodyCinfo info;
	info.m_shape = mopp;
	info.m_motionType = hkpMotion::MOTION_KEYFRAMED;
	hkVector4 angVel; angVel.set(0.0f, -1.0f, 0.0f);
	info.m_angularVelocity = angVel;
	hkVector4 axis (1.0f, 0.0f, 0.0f);
	info.m_rotation.setAxisAngle(axis, HK_REAL_PI * 0.5f);


	hkpRigidBody* rb = new hkpRigidBody(info);
	mopp->removeReference();
	mopp = HK_NULL;

	m_world->addEntity(rb);
	HK_SET_OBJECT_COLOR( hkUlong(rb->getCollidable()), 0x4080ffff);
	rb->removeReference();
}



LimitingMemoryDemo::LimitingMemoryDemo(hkDemoEnvironment* env): hkDefaultPhysicsDemo( env )
{
	// Disable warnings:									
	hkError::getInstance().setEnabled(0x86bc014d, false); //'For the default implementation to work the class must be passed in' 

	m_geom = HK_NULL;
	m_timeToSpawnNewRagdoll = 0.5f;
	m_timeToSpawnDebris = 0.4f;
	m_frameCount = 0;

	//
	// Setup the camera
	//
	{
		hkVector4 from(0.0f, 10.0f, 0.0f);
		hkVector4 to  (0.0f, 0.0f, 0.0f);
		hkVector4 up(0.0f, 0.0f, 1.0f);
		setupDefaultCameras( env, from, to, up, 1.f, 1000.0f );
	}

	//	Create the world
	{
		hkpWorldCinfo info;
		info.m_gravity.set( 0.0f, 0.0f, -10.0f );
		info.m_simulationType = hkpWorldCinfo::SIMULATION_TYPE_CONTINUOUS;

			/// If we have less than the wiggle room available some stuff will need to be freed
        m_watchDog = new MySimpleObjectWatchDog(wiggleSize);

		info.m_memoryWatchDog = m_watchDog;

		m_world = new hkpWorld( info );
		m_world->lock();

		m_watchDog->removeReference();

		// Register ALL agents (though some may not be necessary)
		hkpAgentRegisterUtil::registerAllAgents(m_world->getCollisionDispatcher());
		setupGraphics();
	}

	//
	// Collision Filter
	//
	{
		hkpGroupFilter* filter = new hkpGroupFilter();
		hkpGroupFilterSetup::setupGroupFilter( filter );
		m_world->setCollisionFilter(filter);
		filter->removeReference();
		m_filter = filter;
	}

	// Okay - the most straight forward thing to do now, would be to set the soft memory limit
	// You could do this by doing a garbage collect + then a calculateStatistics to find out
	// how much memory is actually used. Then we can set the soft limit. This works fine in
	// single threading mode, but in multi-threading mode, the threads consume a considerable
	// amount of extra memory, which makes the physics fail.
	// The solution used here is a little messy. Instead of setting the soft limit immediately
	// it's set after 2 simulation steps such that its known that all the threads are up and running
	// and have grabbed the memory they need.
	//
	// To see where the soft limit is set, check out stepDemo method
	//
	// In a real application this messiness is unnessary as generally the soft limit it set to
	// some pre determined size appropriate for the application, as opposed to here, where the
	// limit is set such that memory will fail - so as it can be demonstrated.

	//
	// Create objects
	//
	createHollowSphereMopp(hkpMotion::MOTION_KEYFRAMED);

	createAndAddRagdoll();

	m_world->unlock();

}

LimitingMemoryDemo::~LimitingMemoryDemo()
{
	// Re-enable warnings:									
	hkError::getInstance().setEnabled(0x86bc014e, true); 

    // Turn off the memory limit
    hkMemory::getInstance().setMemorySoftLimit(0);

	m_world->markForWrite();
	m_world->removeReference();
	m_world = HK_NULL;

	delete m_geom;
	m_geom = HK_NULL;
}


hkDemo::Result LimitingMemoryDemo::stepDemo()
{
	hkReal physicsDeltaTime = 1.0f / 60.0f;

	hkDefaultPhysicsDemo::stepDemo();

	m_frameCount ++;
	if (m_frameCount ==2)
	{
		///
		const LimitingMemoryDemoVariant& variant = g_variants[ this->getDemoVariant() ];

		/// We want to know how much memory is really used
		hkMemory::getInstance().garbageCollect();

		/// Get the stats
		hkMemoryStatistics stats;
		hkMemory::getInstance().calculateStatistics(stats);
		// Set the limit

		/// We can just set the memory limit
		hkMemory::getInstance().setMemorySoftLimit(stats.m_used + variant.m_memorySize + wiggleSize);
	}

	m_world->lock();

	m_timeToSpawnDebris -= physicsDeltaTime;
	if (m_timeToSpawnDebris < 0)
	{
		m_timeToSpawnDebris = .3f;
		createAndAddDebris();
	}

	m_timeToSpawnNewRagdoll -= physicsDeltaTime;
	if (m_timeToSpawnNewRagdoll < 0 )
	{
		m_timeToSpawnNewRagdoll = 1.0f;
		createAndAddRagdoll();
	}

	{
        int currentPhysicsMemory;
		{
			hkSimpleStatisticsCollector collector(hkBuiltinTypeRegistry::getInstance().getVtableClassRegistry());
            collector.beginSnapshot( 0 );
            collector.addReferencedObject( "World", m_world );
			collector.commitSnapshot(  );
			hkMonitorStreamAnalyzer::Node* rootNode = collector.m_snapshots[0];
			hkReal* values = &rootNode->m_children[0]->m_value[0];
            currentPhysicsMemory = int(values[1]);
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
		//hkOstream stream( "pool.txt" );
		//hkMemory::getInstance().printStatistics( &stream );
	}

	m_world->unlock();

	return DEMO_OK;
}


void LimitingMemoryDemo::createAndAddDebris()
{
	hkpBoxShape* boxShape = new hkpBoxShape(hkVector4(.1f,.1f,.1f));
	hkpRigidBody* rb =  GameUtils::createRigidBody(boxShape, 10, hkVector4(1,0,0));
	m_world->addEntity(rb);
	boxShape->removeReference();
	m_watchDog->addDebris(rb);
	rb->removeReference();
}

	// creates ragdolls in a car with an initial velocity
void LimitingMemoryDemo::createAndAddRagdoll()
{
	hkVector4		position = hkVector4(0.0f, 0.0f, 0.0f);
	hkQuaternion    rotation; rotation.setIdentity();

	hkpPhysicsSystem* system = GameUtils::createRagdoll( 1.2f, position, rotation, m_filter->getNewSystemGroup(), GameUtils::RPT_CAPSULE );

	m_world->addPhysicsSystem(system);
	m_watchDog->addRagdoll(system);
	system->removeReference();
}

#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
"Running out of memory: The hkWorldMemoryAvailableWatchDog in action.";

HK_DECLARE_DEMO_VARIANT_USING_STRUCT( LimitingMemoryDemo, HK_DEMO_TYPE_PHYSICS, LimitingMemoryDemoVariant, g_variants, helpString);

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
