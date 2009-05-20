/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#include <Demos/demos.h>

#include <Common/Serialize/Packfile/hkPackfileReader.h>
#include <Physics/Utilities/Serialize/hkpHavokSnapshot.h>
#include <Physics/Utilities/Serialize/hkpPhysicsData.h>
#include <Common/Base/System/Io/IStream/hkIStream.h>
#include <Physics/Collide/Shape/Misc/Bv/hkpBvShape.h>

#include <Physics/Collide/Query/CastUtil/hkpWorldRayCastInput.h>
#include <Physics/Collide/Query/CastUtil/hkpWorldRayCastOutput.h>

#include <Physics/Dynamics/World/BroadPhaseBorder/hkpBroadPhaseBorder.h>

// We need to display the results of the raycast to confirm correctness.
#include <Common/Visualize/hkDebugDisplay.h>

#include <Physics/Collide/Query/Collector/RayCollector/hkpClosestRayHitCollector.h>
#include <Demos/Physics/UseCase/CustomCollectors/FirstRayHitCollector/FirstRayHitCollector.h>

//#include <Physics/Internal/PreProcess/ConvexHull/hkpGeometryUtility.h>

#include <Physics/Utilities/Collide/Filter/constraint/hkpConstraintCollisionFilter.h>

#include <Physics/Utilities/Constraint/Keyframe/hkpKeyFrameUtility.h>

#include <Demos/DemoCommon/DemoFramework/hkDefaultPhysicsDemo.h>

class hkpRigidBody;
class hkpMoppBvTreeShape;

struct FirstRayHitCollectorDemo : public hkDefaultPhysicsDemo
{
	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO);

	FirstRayHitCollectorDemo(hkDemoEnvironment* env);
	~FirstRayHitCollectorDemo();
	Result stepDemo(); 

	hkpPhysicsData* m_physicsData;
	hkPackfileReader::AllocatedData* m_loadedData;

	hkpRigidBody* m_hemisphereRb;
	hkpRigidBody* m_boxRb;

	void doRaycast(hkpRigidBody* rb1, hkpRigidBody* rb2);

	int m_counter;
};


FirstRayHitCollectorDemo::FirstRayHitCollectorDemo(hkDemoEnvironment* env)
: hkDefaultPhysicsDemo(env), m_counter(0)
{
	//
	// Setup the camera.
	//
	{
		hkVector4 from( 7.0f, 3.0f, -10.0f);
		hkVector4 to  ( 0.0f, 0.0f, 0.0f);
		hkVector4 up  ( 0.0f, 1.0f,  0.0f);
		setupDefaultCameras(env, from, to, up);
	}

	//
	// Create the world.
	//
	{
		hkString filename; // We have a different binary file depending on the compiler and platform
		filename.printf("Resources/Physics/Objects/hemisphere_L%d%d%d%d.hkx",
			hkStructureLayout::HostLayoutRules.m_bytesInPointer,
			hkStructureLayout::HostLayoutRules.m_littleEndian? 1 : 0,
			hkStructureLayout::HostLayoutRules.m_reusePaddingOptimization? 1 : 0,
			hkStructureLayout::HostLayoutRules.m_emptyBaseClassOptimization? 1 : 0);
		hkIstream infile( filename.cString() );
		HK_ASSERT( 0x215d080c, infile.isOk() );
		m_physicsData = hkpHavokSnapshot::load(infile.getStreamReader(), &m_loadedData);

		m_boxRb = m_physicsData->findRigidBodyByName("box");
		m_hemisphereRb = m_physicsData->findRigidBodyByName("hemisphere");
		m_hemisphereRb->setMotionType(hkpMotion::MOTION_KEYFRAMED);

		hkpWorldCinfo *info = m_physicsData->getWorldCinfo();
 		info->setupSolverInfo(hkpWorldCinfo::SOLVER_TYPE_4ITERS_MEDIUM);
 		info->m_simulationType = hkpWorldCinfo::SIMULATION_TYPE_CONTINUOUS;

		// Set gravity to zero so body floats.
		info->m_gravity.set(0.0f, 0.0f, 0.0f);	
		info->setBroadPhaseWorldSize( 1000.0f );
		info->m_broadPhaseBorderBehaviour = hkpWorldCinfo::BROADPHASE_BORDER_DO_NOTHING;

		m_world = m_physicsData->createWorld();
		m_world->lock();
		m_world->markForWrite();

		hkpConstraintCollisionFilter* collisionFilter = new hkpConstraintCollisionFilter();
		m_world->setCollisionFilter(collisionFilter);
		collisionFilter->removeReference();
		
		// for drawing purposes
		hkpBroadPhaseBorder* border = new hkpBroadPhaseBorder( m_world, hkpWorldCinfo::BROADPHASE_BORDER_DO_NOTHING );
		m_world->setBroadPhaseBorder(border);
		border->removeReference();

		setupGraphics();

		m_world->unmarkForWrite();
		m_world->unlock();
	}

}

FirstRayHitCollectorDemo::~FirstRayHitCollectorDemo()
{
	m_world->markForWrite();
	m_world->removeReference();
	m_world = HK_NULL;

	m_physicsData->removeReference();
	m_loadedData->removeReference();
}


hkDemo::Result FirstRayHitCollectorDemo::stepDemo()
{
	m_world->lock();
	m_world->markForRead();

	{
		hkpWorldRayCastInput input;
		input.m_from = hkVector4(2, -2, -2);
		input.m_to = hkVector4(-2, 2, 2);

		hkpFirstRayHitCollector output;
		m_world->castRay(input, output );

		int color;
		color = output.hasHit() ? 0xFF00FF00 : 0xFFFF0000;

		hkVector4 difference;
		difference.setSub4(input.m_to, input.m_from);
		if (output.hasHit())
		{
			difference.mul4(output.getHit().m_hitFraction);
		}

		HK_DISPLAY_ARROW(input.m_from, difference, color);
	}

	{
		hkpWorldRayCastInput input;
		input.m_from = hkVector4(-2, -2, -2);
		input.m_to = hkVector4(2, 2, 2);

		hkpClosestRayHitCollector output;
		m_world->castRay(input, output );

		int color;
		color = output.hasHit() ? 0xFFFFFF00 : 0xFFFF0000;

		hkVector4 difference;
		difference.setSub4(input.m_to, input.m_from);
		if (output.hasHit())
		{
			difference.mul4(output.getHit().m_hitFraction);
		}

		HK_DISPLAY_ARROW(input.m_from, difference, color);
	}

	m_world->unmarkForRead();

	m_world->markForWrite();
	{
		// Move the hemisphere back and forth around the cube
		hkReal z = 1.6f + 3.0f * hkMath::cos(m_counter++ / 60.0f);
		hkpKeyFrameUtility::applyHardKeyFrame(hkVector4(0,0,z), m_hemisphereRb->getRotation(), 1.0f / m_timestep, m_hemisphereRb);
	}
	m_world->unmarkForWrite();
	m_world->unlock();

	return hkDefaultPhysicsDemo::stepDemo();
}

#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

HK_DECLARE_DEMO(FirstRayHitCollectorDemo, HK_DEMO_TYPE_PRIME, "Shows difference between 'first' and 'closest' ray hit.", "Yellow arrow = hkpClosestRayHitCollector\nGreen arrow = FirstRayHitCollector\nFirst hit is not always the closest as broadphase sorting isn't enough to give a perfect result in all situations.");

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
