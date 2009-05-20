/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/Physics/Api/Dynamics/Actions/BinaryAction/BinaryActionDemo.h>

// Need to display debug lines.
#include <Common/Visualize/hkDebugDisplay.h>


BinaryActionDemo::BinaryActionDemo(hkDemoEnvironment* env)
	: hkDefaultPhysicsDemo(env)
{
	// Define gravity as zero for this demo.
	hkVector4 gravity( 0.0f, 0.0f, 0.0f );

	//
	// Set up the camera.
	//
	// We are using the simple demo framework so we first we set up the camera 
	// with the camera looking approximately at the origin.
	{
		hkVector4 from(0.0f, 5.0f, 20.0f);
		hkVector4 to  (0.0f, 0.0f, 0.0f);
		hkVector4 up  (0.0f, 1.0f, 0.0f);
		setupDefaultCameras( env, from, to, up );
	}
	
	//
	// Create the world.
	//
	// We create our world with broadphase extents set to +/- 100 units and specify
	// hkpWorldCinfo::SOLVER_TYPE_4ITERS_MEDIUM which is a reasonably all purpose solver type.
	{
		hkpWorldCinfo info;
		info.setupSolverInfo(hkpWorldCinfo::SOLVER_TYPE_4ITERS_MEDIUM); 
		info.m_gravity = gravity;	
		info.setBroadPhaseWorldSize( 100.0f );
		m_world = new hkpWorld( info );
		m_world->lock();
		setupGraphics();
	}

	// Register all collision agents
	hkpAgentRegisterUtil::registerAllAgents( m_world->getCollisionDispatcher() );

	//
	// Create Rigid Body.
	//
	{
		/// Here we construct a simple cube with sides 2 units and mass 1.
		hkVector4 boxHalfExtents( 1.0f, 1.0f, 1.0f );  
		hkpBoxShape* geom = new hkpBoxShape( boxHalfExtents , 0 );

		const hkReal mass = 1.0f;
		hkVector4 pos( -2.0f, 0.0f, 0.0f );

		hkpRigidBodyCinfo boxInfo;
		hkpMassProperties massProperties;
		hkpInertiaTensorComputer::computeBoxVolumeMassProperties(boxHalfExtents, mass, massProperties);
		boxInfo.m_mass = massProperties.m_mass;
		boxInfo.m_centerOfMass = massProperties.m_centerOfMass;
		boxInfo.m_inertiaTensor = massProperties.m_inertiaTensor;
		boxInfo.m_shape = geom;
		boxInfo.m_motionType = hkpMotion::MOTION_BOX_INERTIA;
		boxInfo.m_position = pos;

		m_boxRigidBody1 = new hkpRigidBody(boxInfo);

		// As the rigid bodies now references our shape, we no longer need to.
		geom->removeReference();

		// and add it to the world.
		m_world->addEntity( m_boxRigidBody1 );
	}

	//
	// Create Rigid Body.
	//
	{
		// Here we construct a simple cube with sides 2 units and mass 1.
		hkVector4 boxHalfExtents( 1.0f, 1.0f, 1.0f );  
		hkpBoxShape* geom = new hkpBoxShape( boxHalfExtents , 0 );

		const hkReal mass = 1.0f;
		hkVector4 pos( 2.0f, 0.0f, 0.0f );

		hkpRigidBodyCinfo boxInfo;
		hkpMassProperties massProperties;
		hkpInertiaTensorComputer::computeBoxVolumeMassProperties(boxHalfExtents, mass, massProperties);
		boxInfo.m_mass = massProperties.m_mass;
		boxInfo.m_centerOfMass = massProperties.m_centerOfMass;
		boxInfo.m_inertiaTensor = massProperties.m_inertiaTensor;
		boxInfo.m_shape = geom;
		boxInfo.m_motionType = hkpMotion::MOTION_BOX_INERTIA;
		boxInfo.m_position = pos;

		m_boxRigidBody2 = new hkpRigidBody(boxInfo);

		// As the rigid bodies now references our shape, we no longer need to.
		geom->removeReference();

		// and add it to the world.
		m_world->addEntity( m_boxRigidBody2 );
	}
	
	//
	// Create the action and add it to the world.
	//
	{
		// hkpSpringAction applies forces to keep the two hkRigidBodies restLength apart
		// hkpSpringAction is defined in 'hkutilities/actions/spring/hkpSpringAction.h'.
		m_springAction = new hkpSpringAction(m_boxRigidBody1, m_boxRigidBody2);
		m_springAction->setPositionsInBodySpace( m_boxRigidBody1->getCenterOfMassLocal(), m_boxRigidBody2->getCenterOfMassLocal());
		m_springAction->setRestLength(6.0f);
		m_springAction->setDamping(0.5f);
		m_springAction->setStrength(10.0f);

		// Add springAction to the world. From now on springAction will automatically
		// be applied to its bodies during integration so long as the bodies are active.
		// If the bodies become inactive, antiGravityAction is not applied. If they
		// reactivate, springAction again starts applying to them.
		// springAction can also be removed using hkpWorld::removeAction(springAction)
		// after which it is no longer included in simulation.

		m_world->addAction( m_springAction );
	}

	m_world->unlock();
}

BinaryActionDemo::~BinaryActionDemo()
{
	m_world->lock();
	m_boxRigidBody1->removeReference();
	m_boxRigidBody2->removeReference();
	m_springAction->removeReference();
	m_world->unlock();
}

hkDemo::Result BinaryActionDemo::stepDemo()
{
	//
	// Draw a line between the rigid bodies to show the spring.
	//
	{
		hkVector4 pos1 = m_boxRigidBody1->getPosition();
		hkVector4 pos2 = m_boxRigidBody2->getPosition();
		hkVector4 lenVec;
		lenVec.setSub4(pos1, pos2);
		
		if (lenVec.length3() > m_springAction->getRestLength())
		{
			// Line is red if the spring is streched.
			HK_DISPLAY_LINE(pos1, pos2, hkColor::RED);
		}
		else
		{
			// Line is blue is spring is at rest or compressed.
			HK_DISPLAY_LINE(pos1, pos2, hkColor::BLUE);	
		}
	}

	return hkDefaultPhysicsDemo::stepDemo();
}


#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] =					\
"A demo which shows creation and addition of an "	\
"hkpSpringAction to a pair of rigid bodies, both "	\
"of which are boxes.";

HK_DECLARE_DEMO(BinaryActionDemo, HK_DEMO_TYPE_PRIME, "Demonstrates an hkpSpringAction on two rigid bodies", helpString);

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
