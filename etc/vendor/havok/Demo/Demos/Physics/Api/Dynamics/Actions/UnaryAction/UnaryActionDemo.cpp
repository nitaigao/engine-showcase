/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>

//
// This is the action.
//
#include <Demos/Physics/Api/Dynamics/Actions/UnaryAction/AntiGravityAction.h>
#include <Demos/Physics/Api/Dynamics/Actions/UnaryAction/UnaryActionDemo.h>


// A demo which shows creation and addition of an Action which has an "antigravity" effect
// on a single RigidBody. It shows one box which gets controlled by a single action counteracting gravity.
UnaryActionDemo::UnaryActionDemo(hkDemoEnvironment* env)
	: hkDefaultPhysicsDemo(env)
{
	// Define gravity.
	hkVector4 gravity( 0.0f, -9.8f, 0.0f );
	
	//
	// Set up the camera.
	//
	/// We are using the simple demo framework so we first we set up the camera 
	/// with the camera looking approximately at the origin.
	{
		hkVector4 from(0.0f, 7.0f, 20.0f);
		hkVector4 to  (0.0f, 3.0f, 0.0f);
		hkVector4 up  (0.0f, 1.0f, 0.0f);
		setupDefaultCameras( env, from, to, up );
	}
	
	//
	// Create the world.
	//
	/// We create our world with broadphase extents set to +/- 100 units and specify
	/// hkpWorldCinfo::SOLVER_TYPE_4ITERS_MEDIUM which is a reasonably all purpose solver type.
	{
		hkpWorldCinfo info;
		info.setupSolverInfo(hkpWorldCinfo::SOLVER_TYPE_4ITERS_MEDIUM); 
		info.m_gravity = gravity;	
		info.setBroadPhaseWorldSize( 100.0f );
		m_world = new hkpWorld( info );
		m_world->lock();

		setupGraphics();
	}

	//
	// Create Rigid Body.
	//
	hkpRigidBody* boxRigidBody;
	{
		/// Here we construct a simple cube with sides 2 units and mass 1.
		hkVector4 boxHalfExtents( 1.0f, 1.0f, 1.0f );  
		hkpBoxShape* geom = new hkpBoxShape( boxHalfExtents , 0 );

		const hkReal mass = 1.0f;
		hkVector4 pos( 0.0f, 0.0f, 0.0f );

		hkpRigidBodyCinfo boxInfo;
		hkpMassProperties massProperties;
		hkpInertiaTensorComputer::computeBoxVolumeMassProperties(boxHalfExtents, mass, massProperties);
		boxInfo.m_mass = massProperties.m_mass;
		boxInfo.m_centerOfMass = massProperties.m_centerOfMass;
		boxInfo.m_inertiaTensor = massProperties.m_inertiaTensor;
		boxInfo.m_shape = geom;
		boxInfo.m_motionType = hkpMotion::MOTION_BOX_INERTIA;
		boxInfo.m_position = pos;

		boxRigidBody = new hkpRigidBody(boxInfo);

		/// and add it to the world.
		m_world->addEntity( boxRigidBody );

		boxRigidBody->removeReference();

		// As the rigid bodies now references our shape, we no longer need to.
		geom->removeReference();
	}
	
	/// The Antigravity action is specified in AntiGravityAction.inl. We override the
	/// applyAction method inherited from UnaryAction. When a unary action is created it takes a
	/// rigid body as a parameter. 

	/// The Antigravity action uses the time information it receives at each time step
	/// to apply a time varying force (negative gravity in this case) to the object.

	//
	// Create the action and add it to the world.
	//
	{
		/// Create the AntiGravityAction.
		AntiGravityAction* antiGravityAction = new AntiGravityAction( boxRigidBody, gravity );

		
		// Add antiGravityAction to the world. From now on antiGravityAction will automatically
		// be applied to its body (boxRigidBody) during integration so long as boxRigidBody is
		// active. If boxRigidBody becomes inactive, antiGravityAction is not applied. If
		// boxRigidBody reactivates, antiGravityAction again starts applying to it.
		// antiGravityAction can also be removed using hkpWorld::removeAction(antiGravityAction)
		// after which it is no longer included in simulation.

		/// Add the action.
		m_world->addAction( antiGravityAction );

		// After addAction() m_world references antiGravityAction so the local reference can
		// safely be removed.
		antiGravityAction->removeReference();
		
	}

	m_world->unlock();
}


#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
"This demo shows creation and addition of an hkpAction which has an antigravity effect " \
"on a single RigidBody. It shows one box which gets controlled by a single action counteracting gravity.";

HK_DECLARE_DEMO(UnaryActionDemo, HK_DEMO_TYPE_PRIME, "Demonstrates a simple action on a single body", helpString);

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
