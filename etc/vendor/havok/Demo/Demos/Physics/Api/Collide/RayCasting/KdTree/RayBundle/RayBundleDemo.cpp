/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>

#include <Demos/Physics/Api/Collide/RayCasting/KdTree/RayBundle/RayBundleDemo.h>
#include <Demos/DemoCommon/Utilities/KdTree/KdTreeDemoUtils.h>

#include <Common/Internal/KdTree/hkKdTree.h>
#include <Physics/Dynamics/World/Util/KdTree/hkpKdTreeWorldManager.h>
#include <Common/Internal/KdTree/QueryUtils/hkKdTreeUtils.h>
#include <Physics/Collide/Query/Collector/KdTreeCollector/hkpKdTreeCollidableRaycaster.h>

// Used for graphics and I/O
#include <Common/Base/Monitor/hkMonitorStream.h>
#include <Common/Visualize/hkDebugDisplay.h>
#include <Common/Base/Algorithm/PseudoRandom/hkPseudoRandomGenerator.h>


#include <Physics/Collide/Query/Collector/RayCollector/hkpClosestRayHitCollector.h>
#include <Physics/Collide/Query/CastUtil/hkpWorldRayCastInput.h>
#include <Physics/Collide/Query/CastUtil/hkpWorldRayCastOutput.h>

#include <Physics/Collide/Util/KdTree/Mediator/hkpCollidableMediator.h>
#include <Physics/Internal/Collide/BroadPhase/hkpBroadPhase.h>

static int numBodies = 500;

RayBundleDemo::RayBundleDemo(hkDemoEnvironment* env)
:	hkDefaultPhysicsDemo(env), m_rand(1337)
{
	hkpWorld::IgnoreForceMultithreadedSimulation fixTimers;

	{
		m_worldSizeX = 2.0f * hkMath::sqrt(hkReal(m_env->m_cpuMhz));
		m_worldSizeY = 3;
		m_worldSizeZ = m_worldSizeX;
	}

	// Setup the camera and graphics
	{
		// setup the camera
		hkVector4 from(0.0f, 5.0f, 10.0f);
		hkVector4 to  (0.0f, 0.0f,  0.0f);
		hkVector4 up  (0.0f, 1.0f,  0.0f);
		setupDefaultCameras(env, from, to, up);
	}


	{
		hkpWorldCinfo cinfo;
		cinfo.m_gravity.setAll(0);
		cinfo.m_broadPhaseWorldAabb.m_max.set( 1.1f * m_worldSizeX,  10.0f*m_worldSizeY,  1.1f* m_worldSizeZ);
		cinfo.m_broadPhaseWorldAabb.m_min.setNeg4(	cinfo.m_broadPhaseWorldAabb.m_max );	
		cinfo.m_useKdTree = true;
		m_world = new hkpWorld(cinfo);
	}

	m_world->lock();

	//
	// Create a random batch of boxes in the world
	//
	{
		hkAabb worldAabb; 
		worldAabb.m_max.set( m_worldSizeX,  m_worldSizeY, m_worldSizeZ );
		worldAabb.m_min.setNeg4( worldAabb.m_max );	

		hkpMotion::MotionType motionType = hkpMotion::MOTION_SPHERE_INERTIA;
		hkPseudoRandomGenerator rand(100);

		KdTreeDemoUtils::createRandomBodies(m_world, worldAabb, numBodies, motionType, &rand, m_collidables);
	}
	

	setupGraphics();

	m_world->unlock();
}

RayBundleDemo::~RayBundleDemo()
{
}



void RayBundleDemo::doRaycasts()
{
	hkVector4 offset;
	hkVector4 extent; 
	
	m_world->getBroadPhase()->getExtents(m_bounds.m_min, m_bounds.m_max);

	hkpWorldRayCastInput inputs[4];
	hkpWorldRayCastOutput recursCollectors[4];

	int numBundles = 25;
	m_rand.setSeed( m_physicsStepCounter / 60 );

	for (int b=0; b<numBundles; b++)
	{
		hkVector4 starts[4], ends[4];
		hkKdTreeCollidableRayBundleCaster rc;

		m_rand.getRandomVector11(starts[0]);
		m_rand.getRandomVector11(ends[0]);

		// Flatten one component - this forces a special case in the kd-tree raycasting
		ends[0](b%3) = starts[0](b%3);

		hkVector4 worldSize(m_worldSizeX, m_worldSizeY, m_worldSizeZ);

		starts[0].mul4(worldSize);
		ends[0].mul4(worldSize);

		//
		// Randomizes the ray bundle but keeps the ray directions in the same octant
		//
		KdTreeDemoUtils::makeRaysForBundle(starts[0], ends[0], starts, ends, 0.1f, m_rand);

		hkKdTreeUtils::RayBundle fromBundle;
		hkKdTreeUtils::RayBundle toBundle;
			
		{
			for (int i=0; i<4; i++)
			{
				recursCollectors[i].reset();

				inputs[i].m_from = starts[i];
				inputs[i].m_to = ends[i];
				inputs[i].m_enableShapeCollectionFilter = false;
				inputs[i].m_filterInfo = 0;
			}
		}

		rc.m_input = inputs;
		rc.m_output = recursCollectors;

		KdTreeDemoUtils::makeBundleFromInputs(inputs, fromBundle, toBundle);

		{
			HK_TIME_CODE_BLOCK("BundledRaycast", HK_NULL);
			hkKdTreeUtils::castRayBundleIterative(m_world->m_kdTreeManager->getTree(), fromBundle, toBundle, &rc);	
		}

		hkVector4 hitFractions;
		rc.getHitFractions(hitFractions);

		{
			for (int i=0; i<4; i++)
			{
				if (recursCollectors[i].hasHit())
				{
					hkVector4 hitpoint; hitpoint.setInterpolate4(inputs[i].m_from, inputs[i].m_to, hitFractions(i) );
					HK_DISPLAY_STAR(hitpoint, .1f, hkColor::RED);
					HK_DISPLAY_ARROW(hitpoint, recursCollectors[i].m_normal, hkColor::CYAN);
					HK_DISPLAY_LINE(inputs[i].m_from, hitpoint, hkColor::LIME);
				}
				else
				{
					HK_DISPLAY_LINE(inputs[i].m_from, inputs[i].m_to, hkColor::RED);
				}
			}
		}

		// Sanity check
		{
			for (int i=0; i<4; i++)
			{
				HK_TIME_CODE_BLOCK("SingleRaycast", HK_NULL);

				hkpWorldRayCastOutput rcOut;
				m_world->castRay(inputs[i], rcOut);

				HK_ASSERT(0x0, rcOut.hasHit() == recursCollectors[i].hasHit());
				HK_ASSERT(0x0, hkMath::equal(rcOut.m_hitFraction, hitFractions(i)) );
			}
		}
	}

}

hkDemo::Result RayBundleDemo::stepDemo()
{
	hkDefaultPhysicsDemo::stepDemo();

	m_world->markForWrite();
	doRaycasts();
	m_world->unmarkForWrite();

	return hkDemo::DEMO_OK;
}

#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
"This demo compares raycasting four rays at once through the world's kd-tree, versus casting the rays individually.";

HK_DECLARE_DEMO(RayBundleDemo, HK_DEMO_TYPE_PHYSICS, "Raycast bundles", helpString);

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
