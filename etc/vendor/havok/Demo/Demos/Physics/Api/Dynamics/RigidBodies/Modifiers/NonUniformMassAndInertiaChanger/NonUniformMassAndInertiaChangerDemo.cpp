/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/Physics/Api/Dynamics/RigidBodies/Modifiers/NonUniformMassAndInertiaChanger/NonUniformMassAndInertiaChangerDemo.h>

#include <Physics/Dynamics/Constraint/Bilateral/StiffSpring/hkpStiffSpringConstraintData.h>

#include <Physics/Utilities/Collide/ContactModifiers/MassChanger/hkpCollisionMassChangerUtil.h>

#include <Physics/Collide/Shape/Convex/Cylinder/hkpCylinderShape.h>

	// we need to access our game pad
#include <Graphics/Common/Input/Pad/hkgPad.h>




NonUniformMassChangerDemo::NonUniformMassChangerDemo(hkDemoEnvironment* env)
:	hkDefaultPhysicsDemo(env)
{
	// Disable warnings:									
	hkError::getInstance().setEnabled(0xf0de4356, false); // 'Your m_contactRestingVelocity seems to be too small'

	//
	// Setup the camera
	//
	{
		hkVector4 from(0.0f, 8.0f, 24.0f);
		hkVector4 to  (0.0f, 0.0f,  0.0f);
		hkVector4 up  (0.0f, 1.0f,  0.0f);
		setupDefaultCameras( env, from, to, up );
	}

	//
	// Create the world
	//
	{
		hkpWorldCinfo info;

		info.m_gravity.set(0.0f, -9.8f, 0.0f);
		info.setupSolverInfo(hkpWorldCinfo::SOLVER_TYPE_4ITERS_MEDIUM); 
		info.m_collisionTolerance = 0.01f;
		info.m_contactRestingVelocity = 0.01f;
		info.m_gravity.setZero4();

		m_world = new hkpWorld( info );
		m_world->lock();

		// Register ALL agents (though some may not be necessary)
		hkpAgentRegisterUtil::registerAllAgents(m_world->getCollisionDispatcher());

		setupGraphics();
	}


	//
	// Create boxes & bullets
	//
	{
		hkpRigidBody* body;

		hkpRigidBodyCinfo boxInfo;
		boxInfo.m_shape = new hkpBoxShape(hkVector4(1.0f, 1.0f, 0.1f, 0.0f), 0.0f);
		hkpInertiaTensorComputer::setShapeVolumeMassProperties(boxInfo.m_shape, 1.0f, boxInfo);
		boxInfo.m_qualityType = HK_COLLIDABLE_QUALITY_CRITICAL;
		boxInfo.m_motionType = hkpMotion::MOTION_BOX_INERTIA;
		boxInfo.m_friction = 0.0f;
		boxInfo.m_restitution = 1.0f;

		hkpRigidBody* ball;

		hkpRigidBodyCinfo ballInfo;
		ballInfo.m_shape = new hkpSphereShape(0.3f);
		hkpInertiaTensorComputer::setShapeVolumeMassProperties(ballInfo.m_shape, 1.0f, ballInfo);
		ballInfo.m_qualityType = HK_COLLIDABLE_QUALITY_BULLET;
		ballInfo.m_motionType = hkpMotion::MOTION_BOX_INERTIA;
		ballInfo.m_friction = 0.0f;
		ballInfo.m_restitution = 1.0f;
		ballInfo.m_linearVelocity.set(0.0f, 0.0f, -5.0f);

		
		// box 0
		//
		boxInfo.m_position.set(-5.0f, 0.0f, 0.0f);
		body = new hkpRigidBody(boxInfo);
		m_world->addEntity(body)->removeReference();

		// bullet 0
		//
		ballInfo.m_position.set(-5.5f,-0.5f, 10.0f);
		ball = new hkpRigidBody(ballInfo);
		m_world->addEntity(ball)->removeReference();

		{
			hkVector4 scaleBody(1.0f, 1.0f, 1.0f, 0.0f); // normal inv inertia + infinite mass
			hkVector4 scaleBall(1.0f, 1.0f, 1.0f, 1.0f);
			hkpCollisionMassChangerUtil* cmcu = new hkpCollisionMassChangerUtil( body, ball, scaleBody, scaleBall );
			m_utils.pushBack(cmcu);
		}


		// box 1
		//
		boxInfo.m_position.set(0.0f, 0.0f, 0.0f);
		body = new hkpRigidBody(boxInfo);
		m_world->addEntity(body)->removeReference();

		// bullet 1
		//
		ballInfo.m_position.set(-0.5f,-0.5f, 10.0f);
		ball = new hkpRigidBody(ballInfo);
		m_world->addEntity(ball)->removeReference();

		{
			hkVector4 scaleBody(0.0f, 0.0f, 0.0f, 1.0f); // infinite inertia + normal inv mass
			hkVector4 scaleBall(1.0f, 1.0f, 1.0f, 1.0f);
			hkpCollisionMassChangerUtil* cmcu = new hkpCollisionMassChangerUtil( body, ball, scaleBody, scaleBall );
			m_utils.pushBack(cmcu);
		}


		// box 2
		//
		boxInfo.m_position.set( 5.0f, 0.0f, 0.0f);
		body = new hkpRigidBody(boxInfo);
		m_world->addEntity(body)->removeReference();

		// bullet 2
		//
		ballInfo.m_position.set( 4.5f,-0.5f, 10.0f);
		ball = new hkpRigidBody(ballInfo);
		m_world->addEntity(ball)->removeReference();

		{
			hkVector4 scaleBody(1.0f, 0.1f, 0.1f, 0.5f); // two inertia components scaled x10 + doubled mass
			hkVector4 scaleBall(1.0f, 1.0f, 1.0f, 1.0f);
			hkpCollisionMassChangerUtil* cmcu = new hkpCollisionMassChangerUtil( body, ball, scaleBody, scaleBall );
			m_utils.pushBack(cmcu);
		}

		boxInfo.m_shape->removeReference();
		ballInfo.m_shape->removeReference();
	}

	m_world->unlock();
}


hkDemo::Result NonUniformMassChangerDemo::stepDemo()
{
	return hkDefaultPhysicsDemo::stepDemo();
}

NonUniformMassChangerDemo::~NonUniformMassChangerDemo()
{
	// Re-enable warnings:									
	hkError::getInstance().setEnabled(0xf0de4356, true);

	m_world->lock();
	while(m_utils.getSize())
	{
		delete m_utils.back(); 
		m_utils.popBack();
	}
	m_world->unlock();
}


#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
"A demo which creates three boxes and then uses a hkpCenterOfMassChangerUtil to change the properties " \
"of two of the objects during their collisions.";

HK_DECLARE_DEMO(NonUniformMassChangerDemo, HK_DEMO_TYPE_PRIME, "Changing center of mass properties during a collision", helpString);

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
