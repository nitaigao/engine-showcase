/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Physics/Utilities/Collide/ContactModifiers/ViscoseSurface/hkpViscoseSurfaceUtil.h>
#include <Demos/Physics/Api/Dynamics/RigidBodies/Modifiers/ViscousSurface/ViscousSurfaceDemo.h>
#include <Demos/DemoCommon/Utilities/GameUtils/GameUtils.h>
#include <Physics/Dynamics/Constraint/Bilateral/StiffSpring/hkpStiffSpringConstraintData.h>
#include <Common/Visualize/hkDebugDisplay.h>


ViscousSurfaceDemo::ViscousSurfaceDemo(hkDemoEnvironment* env)
:	hkDefaultPhysicsDemo(env)
{
	//
	// Setup the camera
	//
	{
		hkVector4 from( 0.0f, 0.0f, 8.0f);
		hkVector4 to  ( 0.0f, 0.0f, 0.0f);
		hkVector4 up  ( 0.0f, 1.0f, 0.0f);
		setupDefaultCameras( env, from, to, up );
	}


	//
	// Create the world
	//
	{
		hkpWorldCinfo info;
		info.setupSolverInfo(hkpWorldCinfo::SOLVER_TYPE_4ITERS_MEDIUM); 
		info.setBroadPhaseWorldSize( 100.0f );
		info.m_simulationType = hkpWorldCinfo::SIMULATION_TYPE_DISCRETE;
		info.m_enableDeactivation = false;
		m_world = new hkpWorld( info );
		m_world->lock();

		setupGraphics();
	}

	//
	// Register the agents
	//
	{
		hkpAgentRegisterUtil::registerAllAgents(m_world->getCollisionDispatcher());
	}

	for (int i = 0; i < 2; i++)
	{
		hkReal penetration = 0.01f;

		hkpRigidBody* body;

		hkReal xOffset = - 1.0f + 2.0f * hkReal(i);
		hkVector4 wallSize(0.2f, 2.0f, 2.0f);
		hkVector4 boxSize(1.0f, 1.0f, 1.0f);

		// Create fixed walls
		body = GameUtils::createBox(wallSize, 0.0f, hkVector4(-0.6f + penetration + xOffset, 0.0f, 0.0f) );
		body->setFriction(1.0f);
		body->setRestitution(0.0f);
		m_world->addEntity(body)->removeReference();
		body = GameUtils::createBox(wallSize, 0.0f, hkVector4( 0.6f - penetration + xOffset, 0.0f, 0.0f) );
		body->setFriction(1.0f);
		body->setRestitution(0.0f);
		m_world->addEntity(body)->removeReference();

		// Create moving body
		body = GameUtils::createBox(boxSize, 1.0f, hkVector4(0.0f + xOffset, 0.0f, 0.0f) );
		body->setFriction(1.0f);
		body->setRestitution(0.0f);
		if (i)
		{
			hkpViscoseSurfaceUtil::makeSurfaceViscose(body);
			HK_SET_OBJECT_COLOR(hkUlong(body->getCollidable()), 0xffffffaa);
		}
		m_world->addEntity(body)->removeReference();
	}

	m_world->unlock();
}

hkDemo::Result ViscousSurfaceDemo::stepDemo()
{
	m_world->lock();
	{
		hkVector4 g = m_world->getGravity();
		//g.mul4(1.01f);
		m_world->setGravity(g);
	}
	m_world->unlock();
	
	return hkDefaultPhysicsDemo::stepDemo();
}

#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
"This demo demonstrates the use of the viscous surface modifier. Both dynamic boxes are stuck between fixed objects. "\
"The box on the right has viscous surface modifier applied. When you pull the boxes with mouse spring, you'll see "\
"that it is possible to pull that box out. The box without the modifier is stuck for good. "\
"This modifier might allow you to resolve situations when e.g. a high velocity dynamic object hits your landscape "\
"and gets trapped between some features of the landscape. ";

HK_DECLARE_DEMO(ViscousSurfaceDemo, HK_DEMO_TYPE_PRIME, "Viscous Surface Modifier", helpString);

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
