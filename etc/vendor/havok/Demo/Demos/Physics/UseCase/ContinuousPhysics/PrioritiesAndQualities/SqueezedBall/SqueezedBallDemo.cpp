/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>

#include <Physics/Collide/Agent/ConvexAgent/Gjk/hkpPredGskfAgent.h>

#include <Physics/Utilities/VisualDebugger/Viewer/Collide/hkpBroadphaseViewer.h>
#include <Common/Visualize/hkDebugDisplay.h>
#include <Demos/Physics/UseCase/ContinuousPhysics/PrioritiesAndQualities/SqueezedBall/SqueezedBallDemo.h>

SqueezedBallDemo::SqueezedBallDemo(hkDemoEnvironment* env)
:	hkDefaultPhysicsDemo(env)
{
	enableDisplayingToiInformation( true );

	// Disable warnings
	hkError::getInstance().setEnabled(0xf0de4356, false);	// 'Your m_contactRestingVelocity seems to be too small'
	hkError::getInstance().setEnabled(0xad45d441, false);	// 'SCR generated invalid velocities'
	hkError::getInstance().setEnabled(0xafe97523, false); //'This utility is intended primarily for Havok demo use. If you wish to step the world asynchronously,...'

	
	//
	// Setup the camera
	//
	{
		hkVector4 from( 6, 0, 50);
		hkVector4 to  ( 6, 0, 0);
		hkVector4 up  ( 0, 1, 0);
		setupDefaultCameras(env, from, to, up);
	}


	//
	// Setup and create the hkpWorld
	//
	{
		hkpWorldCinfo info;
		info.setBroadPhaseWorldSize(350.0f);
		info.m_collisionTolerance = .03f; 
		info.m_gravity.set(0.0f, -100.0f, 0.0f);
		info.m_enableDeactivation = false;
		info.m_simulationType = info.SIMULATION_TYPE_CONTINUOUS;

		m_world = new hkpWorld( info );
		m_world->lock();

		m_debugViewerNames.pushBack( hkpBroadphaseViewer::getName() );

		hkRegisterAlternateShapeTypes(  m_world->getCollisionDispatcher() );
		hkpPredGskfAgent::registerAgent( m_world->getCollisionDispatcher() );

		setupGraphics();		
	}


	// Create a row of boxes
	int numBodies = 0;

	for (int r = 0; r < 1; r++)
	{
		for (int i = 0; i < 1; i++)
		{
			//hkVector4 boxSize(  0.5f, 0.5f, 0.5f);			// the end pos
			//hkpConvexShape* shape = new hkpBoxShape( boxSize, 0.05f );
			hkpConvexShape* shape = new hkpSphereShape( 0.5f ); 

			hkpRigidBodyCinfo ci;
			ci.m_motionType = hkpMotion::MOTION_DYNAMIC;
			ci.m_shape = shape;
			ci.m_mass = 1.0f;
			ci.m_angularDamping = 0.0f;
			ci.m_linearDamping = 0.0f;
			hkReal d = 1.0f;
			ci.m_inertiaTensor.setDiagonal( d,d,d );
			ci.m_position.set( i * -5.0f, i * -5.0f, 0);
			ci.m_restitution = 0.9f;
			ci.m_qualityType = HK_COLLIDABLE_QUALITY_CRITICAL;
			ci.m_maxLinearVelocity = 1000.0f;
	
			hkpRigidBody* body = new hkpRigidBody(ci);
			char buff[10];
			hkString::sprintf(buff, "body%d", numBodies++);
			body->setName(buff);
			hkVector4 vel(1500.0f, 500.0f, 0.0f);
			body->setLinearVelocity(vel);

			m_world->addEntity( body )->removeReference();

			shape->removeReference();


		}
	}

	hkVector4 halfSize(40.0f, 0.5f, 10.0f);

	// Create bottom fixed body
	{
		hkpConvexShape* shape = new hkpBoxShape(halfSize, 0.0f); 

		hkpRigidBodyCinfo ci;
		ci.m_motionType = hkpMotion::MOTION_FIXED;
		ci.m_shape = shape;
		ci.m_mass = 1.0f;
		ci.m_angularDamping = 0.0f;
		ci.m_linearDamping = 0.0f;
		hkReal d = 1.0f;
		ci.m_inertiaTensor.setDiagonal( d,d,d );
		ci.m_position.set( halfSize(0), -2.0f, 0);
		ci.m_restitution = 0.9f;

		hkpRigidBody* body = new hkpRigidBody(ci);
		
		char buff[10];
		hkString::sprintf(buff, "wall%d", 0);
		body->setName(buff);

		//bodies.pushBack( body );
		m_world->addEntity( body );
		body->removeReference();
		shape->removeReference();

	}

	// Create top body
	{
		hkpConvexShape* shape = new hkpBoxShape(halfSize, 0.0f); 

		hkpRigidBodyCinfo ci;
		ci.m_motionType = hkpMotion::MOTION_DYNAMIC;
		ci.m_shape = shape;
		ci.m_mass = 1000.0f;
		ci.m_angularDamping = 0.0f;
		ci.m_linearDamping = 0.0f;
		ci.m_inertiaTensor.setDiagonal( 10e7,10e7,10e5 );
		ci.m_position.set( halfSize(0), 2.1f, 0);
		ci.m_restitution = 0.9f;
		ci.m_rotation = hkQuaternion(hkVector4(0,0,-1), 4.0f * HK_REAL_PI / 180.0f);
		ci.m_qualityType = HK_COLLIDABLE_QUALITY_MOVING;

		hkpMassProperties mp;
		hkpInertiaTensorComputer::computeBoxVolumeMassProperties(halfSize, ci.m_mass, mp);

		hkpRigidBody* body = new hkpRigidBody(ci);

		char buff[10];
		hkString::sprintf(buff, "wall%d", 1);
		body->setName(buff);

		//bodies.pushBack( body );
		m_world->addEntity( body );
		body->removeReference();

		shape->removeReference();
	}

	m_world->unlock();
}

SqueezedBallDemo::~SqueezedBallDemo()
{
	// Re-enable warnings
	hkError::getInstance().setEnabled(0xf0de4356, true);
	hkError::getInstance().setEnabled(0xad45d441, true);
	hkError::getInstance().setEnabled(0xafe97523, true);

	// bodies->removeReference
}


hkDemo::Result SqueezedBallDemo::stepDemo()
{
	hkReal physicsDeltaTime = 1.0f / 60.0f;
	hkReal frameDeltaTime = physicsDeltaTime * 0.1f;
	hkDefaultPhysicsDemo::stepAsynchronously(m_world, frameDeltaTime, physicsDeltaTime);
	return DEMO_OK;
}


#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
"If a light ball gets squeezed between two heavy objects, an infinit number of " \
"collision will occour. If you want to simulate this scenario correctly, you have "	\
"to invest unlimited amount of CPU. This demo shows that the Havok physics engine " \
"handles this case gracefully: It lets the ball penetrate and tunnel the upper moving " \
"object, which is set to lower priority than the lower fixed object";

HK_DECLARE_DEMO(SqueezedBallDemo, HK_DEMO_TYPE_PRIME, "A ball gets squeezed between two very heavy objects", helpString);

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
