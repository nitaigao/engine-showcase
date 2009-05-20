/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>

#include <Common/Visualize/hkDebugDisplay.h>

#include <Physics/Utilities/VisualDebugger/Viewer/Collide/hkpActiveContactPointViewer.h>
#include <Physics/Utilities/VisualDebugger/Viewer/Collide/hkpBroadphaseViewer.h>

#include <Demos/Physics/UseCase/ContinuousPhysics/PrioritiesAndQualities/HighSpeedCollision/HighSpeedCollisionDemo.h>



HighSpeedCollisionDemo::HighSpeedCollisionDemo(hkDemoEnvironment* env)
:	hkDefaultPhysicsDemo(env)
{
	// Disable warnings:									
	hkError::getInstance().setEnabled(0xafe97523, false); //'This utility is intended primarily for Havok demo use. If you wish to step the world asynchronously,...'

	enableDisplayingToiInformation(true);

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
		info.setBroadPhaseWorldSize(150.0f);
		info.m_collisionTolerance = .03f; 
		info.m_gravity.setZero4();
		info.m_simulationType = info.SIMULATION_TYPE_CONTINUOUS;
		info.m_expectedMaxLinearVelocity = 700.0f;
		info.m_processToisMultithreaded = true;

		m_world = new hkpWorld( info );
		m_world->lock();

		m_debugViewerNames.pushBack(hkpActiveContactPointViewer::getName());
		//m_debugViewerNames.pushBack(hkpBroadphaseViewer::getName());
		//m_debugViewerNames.pushBack(hkpToiContactPointViewer::getName());

		hkpAgentRegisterUtil::registerAllAgents( m_world->getCollisionDispatcher() );

		setupGraphics();
	}


	// Create a row of boxes
	hkArray<hkpRigidBody*> bodies;

	for (int r = -2; r < 4; r++)
	{
		for (int i = 0; i < 6 /*(r >= 0 ? 6-1-r: 8)*/; i++)
		{
			hkVector4 boxSize(  0.45f, 0.45f, 0.45f);			// the end pos
			//hkpConvexShape* shape = new hkpBoxShape( boxSize, 0.05f );
			hkpConvexShape* shape = new hkpSphereShape( 0.5f ); 

			hkpRigidBodyCinfo ci;
			ci.m_motionType = hkpMotion::MOTION_DYNAMIC;
			ci.m_shape = shape;
			ci.m_mass = 1.0f;
			ci.m_angularDamping = 0.0f;
			ci.m_linearDamping = 0.0f;
			ci.m_qualityType = HK_COLLIDABLE_QUALITY_CRITICAL;
			hkReal d = 1.0f;
			ci.m_inertiaTensor.setDiagonal( d,d,d );
			ci.m_position.set( i * 1.0f, r * 1.0f, 0);
			ci.m_allowedPenetrationDepth = 0.5f;
			if (0 == bodies.getSize())
			{
				ci.m_position.set( i * 1.0f - 5.0f, r * 1.0f - 5.0f, 0);
			}

			hkpRigidBody* body = new hkpRigidBody(ci);

			char buff[10];
			hkString::sprintf(buff, "[%d]", bodies.getSize());
			body->setName(buff);
			bodies.pushBack( body );
			m_world->addEntity( body );
			body->removeReference();

			shape->removeReference();
		}
	}
	//
	//	Heavily push the first object
	//
	hkVector4 impulse( 500.0f, 500.0f, 0.0f );
	impulse.mul4( 1.016f );
	bodies[0]->applyLinearImpulse( impulse );

	m_world->unlock();
}

HighSpeedCollisionDemo::~HighSpeedCollisionDemo()
{
	// Re-enable warnings:									
	hkError::getInstance().setEnabled(0xafe97523, true); 

	//m_env->m_window->getCurrentViewport()->toggleState(HKG_ENABLED_WIREFRAME);
}


hkDemo::Result HighSpeedCollisionDemo::stepDemo()
{
	hkReal physicsDeltaTime = 1.0f / 60.0f;
	hkReal frameDeltaTime = physicsDeltaTime / 5.0f;

	hkDefaultPhysicsDemo::stepAsynchronously(m_world, frameDeltaTime, physicsDeltaTime );

	return DEMO_OK;
}


#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
"A single bullet is shot at a group of objects. " \
"This is a pretty hard case for the TOI solver. Note that the spheres penetrate, because they have a very large " \
"allowedPenetrationDepth equal to their radius.";

HK_DECLARE_DEMO(HighSpeedCollisionDemo, HK_DEMO_TYPE_PRIME, "High speed bullet fired at a group of objects", helpString);

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
