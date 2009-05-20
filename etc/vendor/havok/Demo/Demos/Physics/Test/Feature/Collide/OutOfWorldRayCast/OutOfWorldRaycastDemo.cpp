/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#include <Demos/demos.h>



#include <Physics/Collide/Query/CastUtil/hkpWorldRayCastInput.h>
#include <Physics/Collide/Query/CastUtil/hkpWorldRayCastOutput.h>

#include <Physics/Dynamics/World/BroadPhaseBorder/hkpBroadPhaseBorder.h>

// We need to display the results of the raycast to confirm correctness.
#include <Common/Visualize/hkDebugDisplay.h>

#include <Physics/Collide/Query/Collector/RayCollector/hkpClosestRayHitCollector.h>


#include <Common/Base/Algorithm/PseudoRandom/hkPseudoRandomGenerator.h>

#include <Demos/DemoCommon/DemoFramework/hkDefaultPhysicsDemo.h>

#include <Physics/Collide/Query/Collector/PointCollector/hkpAllCdPointCollector.h>
#include <Physics/Collide/Query/CastUtil/hkpLinearCastInput.h>

class hkpRigidBody;
class hkpMoppBvTreeShape;

struct OutOfWorldRaycastDemo : public hkDefaultPhysicsDemo
{
	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO);

	OutOfWorldRaycastDemo(hkDemoEnvironment* env);
	~OutOfWorldRaycastDemo();
	Result stepDemo(); 

	hkPseudoRandomGenerator m_rand;

	hkReal m_time;

	hkpRigidBody* m_boxes[2];

	void doRaycast(hkpRigidBody* rb1, hkpRigidBody* rb2);
	void doLinearCast(hkpRigidBody* rb1, hkpRigidBody* rb2);
};


OutOfWorldRaycastDemo::OutOfWorldRaycastDemo(hkDemoEnvironment* env)
:	hkDefaultPhysicsDemo(env), m_rand(747)
{
	//
	// Setup the camera.
	//
	{
		hkVector4 from(30.0f, 8.0f, 25.0f);
		hkVector4 to  ( 4.0f, 0.0f, -3.0f);
		hkVector4 up  ( 0.0f, 1.0f,  0.0f);
		setupDefaultCameras(env, from, to, up);
	}

	hkError::getInstance().setEnabled(0xad367291, false); // Warning for linear cast going outside the broadphase.

	//
	// Create the world.
	//
	{
		hkpWorldCinfo info;
		info.setupSolverInfo(hkpWorldCinfo::SOLVER_TYPE_4ITERS_MEDIUM); 
		
		// Set gravity to zero so body floats.
		info.m_gravity.set(0.0f, 0.0f, 0.0f);	
		info.setBroadPhaseWorldSize( 10.0f );
		info.m_broadPhaseBorderBehaviour = hkpWorldCinfo::BROADPHASE_BORDER_DO_NOTHING;
		m_world = new hkpWorld(info);
		m_world->lock();

		// for drawing purposes
		hkpBroadPhaseBorder* border = new hkpBroadPhaseBorder( m_world, hkpWorldCinfo::BROADPHASE_BORDER_DO_NOTHING );
		m_world->setBroadPhaseBorder(border);
		border->removeReference();

		setupGraphics();
	}

	// register all agents(however, we put all objects into the some group,
	// so no collision will be detected
	hkpAgentRegisterUtil::registerAllAgents( m_world->getCollisionDispatcher() );

	//
	// Create some bodies
	//
	hkVector4 boxsize(.5f, .5f, .5f);
	hkpBoxShape* boxshape = new hkpBoxShape(boxsize);

	hkpRigidBodyCinfo info;
	info.m_shape = boxshape;
	info.m_mass = 1.0f;
	info.m_linearDamping = 5.0f;
	info.m_motionType = hkpMotion::MOTION_BOX_INERTIA;

	// These values will crash in linear casting if HVK-3495 isn't fixed
	hkVector4 startPos[2];
	startPos[0].set(-6.4253216f, -5.0469332f,-3.4372473f);
	startPos[1].set(-3.8643494f, -8.8486900f, 0.48582274f);

	for (int i=0; i<2; i++)
	{
		info.m_position = startPos[i];
		m_boxes[i] = new hkpRigidBody(info);
		m_world->addEntity(m_boxes[i]);
	}
	boxshape->removeReference();

	
	m_world->unlock();
}

OutOfWorldRaycastDemo::~OutOfWorldRaycastDemo()
{
	hkError::getInstance().setEnabled(0xad367291, true);
	m_world->lock();
	m_boxes[0]->removeReference();
	m_boxes[1]->removeReference();
	m_world->unlock();
}


hkDemo::Result OutOfWorldRaycastDemo::stepDemo()
{
	m_world->lock();

	//if( m_env->m_gamePad->wasButtonPressed(HKG_PAD_BUTTON_1) )
	{
		for (int i=0; i<2; i++)
		{
			hkVector4 p( m_rand.getRandReal01(), m_rand.getRandReal01(), m_rand.getRandReal01() );
			p.sub4( hkVector4(0.5f,0.5f,0.5f) );
			p.mul4( 19.0f ); // change to larger value to put one or both outside
			m_boxes[i]->setPosition(p);
		}
	}

	if (m_variantId == 0)
	{
		doRaycast(m_boxes[0], m_boxes[1]);
		doRaycast(m_boxes[1], m_boxes[0]);
	}
	else
	{
		doLinearCast(m_boxes[0], m_boxes[1]);
		doLinearCast(m_boxes[1], m_boxes[0]);
	}
	

	m_world->unlock();

	return hkDefaultPhysicsDemo::stepDemo();
}


void OutOfWorldRaycastDemo::doRaycast(hkpRigidBody* rb1, hkpRigidBody* rb2)
{
	hkVector4 to, from, midpoint;
	to = rb1->getPosition();
	from = rb2->getPosition();
	midpoint.setInterpolate4(to, from, .5f);

	hkpWorldRayCastInput input;
	input.m_from = from;
	input.m_to = to;

	hkpClosestRayHitCollector output;
	m_world->castRay(input, output );

	int color;
	color = output.hasHit() ? 0xFF00FF00 : 0xFFFF0000;

	hkVector4 dir;
	dir.setSub4(to, midpoint);

	HK_DISPLAY_ARROW(midpoint,dir, color);
}

void OutOfWorldRaycastDemo::doLinearCast(hkpRigidBody* rb1, hkpRigidBody* rb2)
{
	hkVector4 to, from, midpoint;
	to = rb1->getPosition();
	from = rb2->getPosition();
	midpoint.setInterpolate4(to, from, .5f);

	// try a linear cast
	hkpLinearCastInput lci;
	hkpAllCdPointCollector collector;
	lci.m_to = to;
	m_world->linearCast(rb2->getCollidable(), lci, collector);
		
	int color;
	color = collector.hasHit() ? 0xFF00FF00 : 0xFFFF0000;

	const hkArray<hkpRootCdPoint> &hits = collector.getHits();
	// check for duplicates // HVK-3126
	for( int i = 0; i < hits.getSize(); ++i )
	{
		for( int j = i+1; j < hits.getSize(); ++j )
		{
			HK_ON_DEBUG(const hkpRootCdPoint& pi = hits[i]);
			HK_ON_DEBUG(const hkpRootCdPoint& pj = hits[j]);
			HK_ASSERT(0, (pi.m_shapeKeyA != pj.m_shapeKeyA) || (pi.m_shapeKeyB != pj.m_shapeKeyB) );
		}
	}

	hkVector4 dir;
	dir.setSub4(to, midpoint);

	HK_DISPLAY_ARROW(midpoint,dir, color);
}

#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char* helpString = "Casting outside of the broadphase may miss but shouldn't crash.\
 Press \221 to warp the bodies randomly";

HK_DECLARE_DEMO_VARIANT(OutOfWorldRaycastDemo, HK_DEMO_TYPE_PRIME, "Ray casting", 0, "Raycasting outside of the broadphase", helpString);
HK_DECLARE_DEMO_VARIANT(OutOfWorldRaycastDemo, HK_DEMO_TYPE_PRIME, "Linear casting", 1, "Raycasting outside of the broadphase", helpString);

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
