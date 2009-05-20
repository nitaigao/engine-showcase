/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>

#include <Demos/Physics/Api/Collide/RayCasting/KdTree/KdTreeVsBroadphase/KdTreeVsBroadphaseDemo.h>
#include <Demos/DemoCommon/Utilities/KdTree/KdTreeDemoUtils.h>

#include <Common/Internal/KdTree/hkKdTree.h>
#include <Physics/Dynamics/World/Util/KdTree/hkpKdTreeWorldManager.h>
#include <Common/Internal/KdTree/Build/hkKdTreeBuilder.h>
#include <Common/Internal/KdTree/Build/hkKdTreeBuildingUtils.h>
#include <Common/Internal/KdTree/QueryUtils/hkKdTreeUtils.h>

#include <Physics/Collide/Query/Collector/KdTreeCollector/hkpKdTreeCollidableRaycaster.h>
#include <Physics/Collide/Query/Collector/KdTreeCollector/hkpKdTreeWorldLinearCaster.h>
#include <Physics/Collide/Util/KdTree/Mediator/hkpCollidableMediator.h>

// Used for graphics and I/O
#include <Common/Base/Monitor/hkMonitorStream.h>
#include <Common/Base/Container/LocalArray/hkLocalArray.h>
#include <Common/Visualize/hkDebugDisplay.h>
#include <Demos/DemoCommon/Utilities/GameUtils/GameUtils.h>
#include <Demos/DemoCommon/DemoFramework/hkTextDisplay.h>

#include <Common/Visualize/hkDebugDisplay.h>
#include <Demos/DemoCommon/Utilities/KdTree/KdTreeDebugUtils.h>

// Serialization
#include <Common/Base/Container/LocalArray/hkLocalBuffer.h>

#include <Physics/Collide/Util/hkpAabbUtil.h>

#include <Physics/Collide/Query/Collector/RayCollector/hkpClosestRayHitCollector.h>
#include <Physics/Collide/Query/CastUtil/hkpWorldRayCastInput.h>
#include <Physics/Collide/Query/CastUtil/hkpWorldRayCastOutput.h>

#include <Physics/Internal/Collide/BroadPhase/hkpBroadPhase.h>


#include <Physics/Collide/Query/Collector/PointCollector/hkpClosestCdPointCollector.h>
#include <Physics/Collide/Query/CastUtil/hkpLinearCastInput.h>
#include <Physics/Collide/Agent/hkpCollisionAgentConfig.h>




static int numBodies = 500;


KdTreeVsBroadphaseDemo::KdTreeVsBroadphaseDemo(hkDemoEnvironment* env)
:	hkDefaultPhysicsDemo(env), 
	m_flattenRays(false),
	m_rand(1337)
{

	{
		m_worldSizeX = 2.0f * hkMath::sqrt(hkReal(m_env->m_cpuMhz));
		m_worldSizeY = 3;
		m_worldSizeZ = m_worldSizeX;
	}

	// Setup the camera and graphics
	{
		// setup the camera
		hkVector4 from(0.0f, m_worldSizeZ*2.f, -m_worldSizeZ);
		hkVector4 to  (0.0f, 0.0f,  0.0f);
		hkVector4 up  (0.0f, 1.0f,  0.0f);
		setupDefaultCameras( env, from, to, up, 1.0f, 5000.0f );
	}


	{
		hkpWorldCinfo cinfo;
		cinfo.m_gravity.setAll(0);
		cinfo.m_broadPhaseWorldAabb.m_max.set( m_worldSizeX,  10.0f*m_worldSizeY,  m_worldSizeZ);
		cinfo.m_broadPhaseWorldAabb.m_min.setNeg4(	cinfo.m_broadPhaseWorldAabb.m_max );	
		cinfo.m_useKdTree = true;
		m_world = new hkpWorld(cinfo);
		m_world->lock();
	}

	{
		hkpAgentRegisterUtil::registerAllAgents(m_world->getCollisionDispatcher());
	}

	//
	// Add a lot of rigid bodies to the world
	//
	{
		hkAabb worldAabb; 
		worldAabb.m_max.set( m_worldSizeX,  10.0f*m_worldSizeY,  m_worldSizeZ);
		worldAabb.m_min.setNeg4( worldAabb.m_max );	

		hkpMotion::MotionType motionType = hkpMotion::MOTION_FIXED;
		KdTreeDemoUtils::createRandomBodies(m_world, worldAabb, numBodies, motionType, &m_rand, m_collidables);

	}


	setupGraphics();
	
	m_world->unlock();
}

KdTreeVsBroadphaseDemo::~KdTreeVsBroadphaseDemo()
{
}

//
// Do some random raycasts into the world
// Both the kd-tree and the (deprecated) hkp3AxisSweep versions are used for comparison
//
void KdTreeVsBroadphaseDemo::doRaycasts()
{
	const int numRays = 100;
	
	hkLocalArray<hkpWorldRayCastInput> inputs( numRays );
	inputs.setSize(numRays);

	// Need fixed-size collector arrays to make sure the constructors get called

	hkpWorldRayCastOutput iterativeCollectors[numRays];
	hkpClosestRayHitCollector worldCollectors[numRays];

	for (int i=0; i < numRays; i++)
	{
		hkVector4 start, end;
		start.set(	hkMath::randRange(-m_worldSizeX, m_worldSizeX),
					hkMath::randRange(-m_worldSizeY, m_worldSizeY),
					hkMath::randRange(-m_worldSizeZ, m_worldSizeZ));

		end.set(	hkMath::randRange(-m_worldSizeX, m_worldSizeX),
					hkMath::randRange(-m_worldSizeY, m_worldSizeY),
					hkMath::randRange(-m_worldSizeZ, m_worldSizeZ));

		// Flatten out the rays in one component - this triggers a special case in the raycasting code
		{
			end(i%3) = start(i%3);
		}

		inputs[i].m_from = start;
		inputs[i].m_to = end;
		inputs[i].m_filterInfo = 0;
	}

	//
	// Raycast using the world's kd-tree
	//
	HK_TIMER_BEGIN("kdTreeRaycast", HK_NULL);

	// Check that the tree isn't dirty
	HK_ASSERT(0x3fe8daf1, m_world->m_kdTreeManager->isUpToDate());
	for (int i=0; i < numRays; i++)
	{
		m_world->castRay(inputs[i], iterativeCollectors[i]);

	}
	HK_TIMER_END();


	HK_TIMER_BEGIN("worldRc", HK_NULL);
	{ 
		//
		// Mark the world's kd-tree as dirty, forcing raycasting to go through the old (slow) hkp3AxisSweep algorithm
		// You should NOT usually be doing this.
		//
		m_world->markKdTreeDirty();
		for (int i=0; i < numRays; i++)
		{
			m_world->castRay(inputs[i], worldCollectors[i]);
		}
	
	}
	HK_TIMER_END();
		
	// Check that the results agree, and draw the results
	{ 
		for (int i=0; i<numRays; i++)
		{

			HK_ASSERT(0x0, iterativeCollectors[i].hasHit() == worldCollectors[i].hasHit());
			HK_ASSERT(0x0, hkMath::equal(iterativeCollectors[i].m_hitFraction, worldCollectors[i].m_earlyOutHitFraction));

			if (iterativeCollectors[i].hasHit())
			{
				hkVector4 hitpoint;
				hitpoint.setInterpolate4(inputs[i].m_from, inputs[i].m_to, iterativeCollectors[i].m_hitFraction);
				HK_DISPLAY_STAR(hitpoint, .1f, hkColor::RED);
				HK_DISPLAY_ARROW(hitpoint, iterativeCollectors[i].m_normal, hkColor::CYAN);
				HK_DISPLAY_LINE(inputs[i].m_from, hitpoint, hkColor::BLUE);
			}
			else
			{
				HK_DISPLAY_LINE(inputs[i].m_from, inputs[i].m_to, hkColor::WHITE);
			}
		}
	}
}

void KdTreeVsBroadphaseDemo::doLinearCasts()
{
	const int numCasts = 100;

	hkObjectArray<hkpClosestCdPointCollector> worldCollectors(numCasts);
	hkObjectArray<hkpClosestCdPointCollector> treeCollectors(numCasts);
	hkArray<hkpLinearCastInput> lcInput (numCasts);

	hkArray<const hkpCollidable*> castCollidables(numCasts);

	for (int i=0; i < numCasts; i++)
	{
		//hkprintf("random seed = %d\n", m_rand.getCurrent());
		castCollidables[i] = m_collidables[ m_rand.getRand32() % m_collidables.getSize() ];

		hkVector4 start, end;
		start = hkGetRigidBody(castCollidables[i])->getPosition();

		hkVector4 worldSize(m_worldSizeX, m_worldSizeY, m_worldSizeZ);
		m_rand.getRandomVector11(end);
		end.mul4(worldSize);

		// Flatten out the rays in one component - this triggers a special case in the raycasting code
		{
			end(i%3) = start(i%3);
		}

		lcInput[i].m_to = end;
		lcInput[i].m_maxExtraPenetration = HK_REAL_EPSILON;
		lcInput[i].m_startPointTolerance = HK_REAL_EPSILON;
		worldCollectors[i].reset();
		treeCollectors[i].reset();

	}

	{
		HK_ASSERT(0x3fe8daf1, m_world->m_kdTreeManager->isUpToDate());
		HK_TIME_CODE_BLOCK("kdtreeLinearCast", HK_NULL);
		for (int i=0; i < numCasts; i++)
		{
			m_world->linearCast(castCollidables[i], lcInput[i], treeCollectors[i] );
		}

	}

	{ 
		HK_TIME_CODE_BLOCK("worldLinearCast", HK_NULL);
		//
		// Mark the world's kd-tree as dirty, forcing raycasting to go through the old (slow) hkp3AxisSweep algorithm
		// You should NOT usually be doing this.
		//
		m_world->markKdTreeDirty();
		for (int i=0; i < numCasts; i++)
		{
			m_world->linearCast(castCollidables[i], lcInput[i], worldCollectors[i] );
		}
	}
	
	
	// Check that the results agree, and draw the results
	{ 
		for (int i=0; i<numCasts; i++)
		{

  			HK_ASSERT(0x0, worldCollectors[i].hasHit() == treeCollectors[i].hasHit() );
  			
  			if (worldCollectors[i].hasHit())
  			{
  				hkReal tolerance = m_world->getCollisionInput()->m_config->m_iterativeLinearCastEarlyOutDistance;
  				
  				hkBool hitFractionsEqual = hkMath::equal(worldCollectors[i].getEarlyOutDistance(), treeCollectors[i].getEarlyOutDistance(), 2.0f*tolerance);
  				hkBool hitCollidablesEqual =  worldCollectors[i].getHit().m_rootCollidableB == treeCollectors[i].getHit().m_rootCollidableB ;
  				HK_ASSERT(0x0, hitFractionsEqual || hitCollidablesEqual );
  			}

			hkVector4 start = hkGetRigidBody(castCollidables[i])->getPosition();

			if (treeCollectors[i].hasHit())
			{
				hkVector4 hitpoint;
				hitpoint.setInterpolate4(start, lcInput[i].m_to, treeCollectors[i].getEarlyOutDistance() );

				HK_DISPLAY_STAR(hitpoint, .1f, hkColor::RED);
				HK_DISPLAY_ARROW(hitpoint, treeCollectors[i].getHit().m_contact.getNormal(),  hkColor::CYAN);
				HK_DISPLAY_LINE(start, hitpoint, hkColor::BLUE);
			}
			else
			{
				HK_DISPLAY_LINE(start, lcInput[i].m_to, hkColor::WHITE);
			}
		}
	}
}


hkDemo::Result KdTreeVsBroadphaseDemo::stepDemo()
{
	hkDemo::Result res = hkDefaultPhysicsDemo::stepDemo();

	m_rand.setSeed( m_physicsStepCounter / 60 );

	if (m_env->m_gamePad->wasButtonPressed(HKG_PAD_BUTTON_0))
	{
		m_flattenRays = !m_flattenRays;
	}

	m_world->markForRead();

	if (m_variantId == 0)
	{
		doRaycasts();
	}
	else
	{
		doLinearCasts();
	}

	m_world->unmarkForRead();

	return res;
}

#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
"This demo compares the performance of the using the world's kd-tree for raycasting and linear casting, " \
"versus the same cast through the broadphase (hkp3AxisSweep).";

HK_DECLARE_DEMO_VARIANT(KdTreeVsBroadphaseDemo, HK_DEMO_TYPE_PHYSICS, "Raycast comparison", 0, "Raycast comparison", helpString);
HK_DECLARE_DEMO_VARIANT(KdTreeVsBroadphaseDemo, HK_DEMO_TYPE_PHYSICS, "Linear cast comparison", 1, "Linear cast comparison", helpString);

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
