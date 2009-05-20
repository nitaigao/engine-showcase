/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/Physics/Api/Dynamics/RigidBodies/Modifiers/SurfaceVelocity/Unfiltered/UnfilteredSurfaceVelocityDemo.h>

#include <Physics/Utilities/Collide/ContactModifiers/SurfaceVelocity/hkpSurfaceVelocityUtil.h>


UnfilteredSurfaceVelocityDemo::UnfilteredSurfaceVelocityDemo(hkDemoEnvironment* env)
:	hkDefaultPhysicsDemo(env)
{

	//
	// Setup the camera
	//
	{
		hkVector4 from(10.0f, 4.0f, 0.0f);
		hkVector4 to  ( 0.0f, 0.0f, 0.0f);
		hkVector4 up  ( 0.0f, 1.0f, 0.0f);
		setupDefaultCameras(env, from, to, up);
	}

	//
	// Create the world
	//
	{
		hkpWorldCinfo info;
		info.m_gravity.set(0.0f, -9.81f, 0.0f);
		info.setBroadPhaseWorldSize(200.0f);
		info.setupSolverInfo(hkpWorldCinfo::SOLVER_TYPE_4ITERS_HARD);
		info.m_collisionTolerance = 0.05f;
		//info.m_contactRestingVelocity = 0.01f;

		m_world = new hkpWorld(info);
		m_world->lock();

		// Register ALL agents (though some may not be necessary)
		hkpAgentRegisterUtil::registerAllAgents(m_world->getCollisionDispatcher());

		setupGraphics();
	}

    
	// 
	// Create ground box
	//
	{
		hkVector4 boxSize(10.0f, 0.5f, 10.0f);
		hkpBoxShape* baseShape = new hkpBoxShape(boxSize, 0);

		hkpRigidBodyCinfo boxInfo;

		boxInfo.m_shape = baseShape;
		boxInfo.m_motionType = hkpMotion::MOTION_FIXED;
		boxInfo.m_position = hkVector4(0.0f, 0.0f, 0.0f);
		hkpRigidBody* baseRigidBody = new hkpRigidBody(boxInfo);

		m_world->addEntity( baseRigidBody);
		baseRigidBody->removeReference();
		baseShape->removeReference();

		//
		// add surface velocity
		//
		hkVector4 surfaceVelocity(0, 0, 1.0f);
		(new hkpSurfaceVelocityUtil(baseRigidBody, surfaceVelocity))->removeReference();

	}


	//
	// create two small boxes
	//
	{
		hkpBoxShape* boxShape;
		{
			hkVector4 boxSize(0.2f, 0.2f, 0.2f);
			boxShape = new hkpBoxShape(boxSize, 0);
		}

		hkpRigidBodyCinfo boxInfo;
		{
			boxInfo.m_shape = boxShape;
			boxInfo.m_motionType = hkpMotion::MOTION_BOX_INERTIA;
			boxInfo.m_rotation.setAxisAngle( hkVector4( 1, 0, 0 ), 45.0f * (HK_REAL_PI/180.0f) );

			hkpInertiaTensorComputer::setShapeVolumeMassProperties(boxInfo.m_shape, 1.0f, boxInfo);
		}

		//
		// left box
		//
		{
			boxInfo.m_position = hkVector4(0.0f, 3.0f, 1.5f);

			hkpRigidBody* rigidBody = new hkpRigidBody(boxInfo);

			m_world->addEntity(rigidBody);

			rigidBody->removeReference();
		}

		//
		// right box
		//
		{
			boxInfo.m_position = hkVector4(0.0f, 3.0f, -1.5f);

			hkpRigidBody* rigidBody = new hkpRigidBody(boxInfo);

			m_world->addEntity(rigidBody);

			rigidBody->removeReference();
		}

		boxShape->removeReference();
	}

	m_world->unlock();
}



UnfilteredSurfaceVelocityDemo::~UnfilteredSurfaceVelocityDemo()
{
}



#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
"Unfiltered Surface Velocity Demo";

HK_DECLARE_DEMO(UnfilteredSurfaceVelocityDemo, HK_DEMO_TYPE_PRIME, "Enable surface velocity for each object hitting the ground.", helpString);

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
