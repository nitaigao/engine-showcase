/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/Physics/Api/Dynamics/RigidBodies/FrictionChange/FrictionChangeDemo.h>


#include <Demos/DemoCommon/DemoFramework/hkTextDisplay.h>


#include <Physics/Dynamics/Collide/hkpContactUpdater.h>


FrictionChangeDemo::FrictionChangeDemo( hkDemoEnvironment* env ) :
	hkDefaultPhysicsDemo( env )
{

	// Setup the camera.
	{
		hkVector4 from(0.0f, 10.0f, 40.0f);
		hkVector4 to  (0.0f, 0.0f,  0.0f);
		hkVector4 up  (0.0f, 1.0f,  0.0f);
		setupDefaultCameras(env, from, to, up);
	}

	// Create the world.
	{
		hkpWorldCinfo info;
		
		info.setBroadPhaseWorldSize( 1000.0f );
		m_world = new hkpWorld(info);
		m_world->lock();

		setupGraphics();
	}

	//
	// Setup game logic variables
	//

	m_frameCount = 0;
	m_frictionChangePeriod = 100;

	//
	// Register the agents
	//
	{
		hkpAgentRegisterUtil::registerAllAgents(m_world->getCollisionDispatcher());
	}


	// In this demo we have two rigid bodies (both hkBoxShapes); the floor, which is fixed, and the sliding dynamic box.
	// These are constructed in the usual manner by filling out the hkpRigidBodyCinfo template for each. Both
	// bodies have initially been given a frictional value of 1.0:

	//
	// Create a fixed body
	//
	{	
		const hkVector4 halfExtents(20.0f, 2.f, 10.0f);
		hkpShape* shape = new hkpBoxShape(halfExtents, 0 );

		// Compute the inertia tensor from the shape
		hkpMassProperties massProperties;
		hkpInertiaTensorComputer::computeShapeVolumeMassProperties(shape, 5.0f, massProperties);

		// Assign the rigid body properties
		hkpRigidBodyCinfo bodyInfo;
		bodyInfo.m_motionType = hkpMotion::MOTION_FIXED;
		bodyInfo.m_shape = shape;

		hkVector4 axis(0.0f, 0.0f, 1.0f);
		bodyInfo.m_rotation = hkQuaternion(axis, -0.3f);
	
		bodyInfo.m_friction = 1.f;

		hkpRigidBody *body = new hkpRigidBody(bodyInfo);
		m_world->addEntity( body );
		body->removeReference();

		shape->removeReference();
	}

	//
	// Create a moving body - we update the friction to this in the stepGame function
	//
	{	
		const hkVector4 halfExtents(2.0f, .5f, 2.0f);
		hkpShape* shape = new hkpBoxShape(halfExtents, 0 );

		// Compute the inertia tensor from the shape
		hkpMassProperties massProperties;
		hkpInertiaTensorComputer::computeShapeVolumeMassProperties(shape, 5.0f, massProperties);

		// Assign the rigid body properties
		hkpRigidBodyCinfo bodyInfo;
		bodyInfo.m_mass = massProperties.m_mass;
		bodyInfo.m_centerOfMass = massProperties.m_centerOfMass;
		bodyInfo.m_inertiaTensor = massProperties.m_inertiaTensor;
		bodyInfo.m_shape = shape;
		bodyInfo.m_motionType = hkpMotion::MOTION_BOX_INERTIA;
		bodyInfo.m_position.set(-10.0f, 8.0f, 0.0f);

		bodyInfo.m_friction = 1.f;

		m_movingBody = new hkpRigidBody(bodyInfo);
		m_world->addEntity( m_movingBody );

		m_movingBody->removeReference();

		shape->removeReference();
	}
	
	/// As the demo runs we will dynamically alter the friction value for the moving body, alternatively setting it to 0.0 and 1.0.

	m_world->unlock();
}



// The period between friction information switching is determined by the 'game logic' variables,
// which simply signals a change every 'm_frictionChangePeriod' frames (every 100 by default).

hkDemo::Result FrictionChangeDemo::stepDemo()
{
	m_world->lock();

	// We have an additional condition check to see if the moving block has dropped below -5.0 units.
	// This will only occur if the moving block as fallen off the edge of the large fixed block and so we
	// know that it's time to reset the simulation if this is true.

	//
	// If the moving body has fallen off the edge (below -5 units), replace it back above the "floor"
	//

	if(m_movingBody->getPosition()(1) < - 5.0f)
	{
		m_movingBody->setLinearVelocity(hkVector4::getZero());
		m_movingBody->setAngularVelocity(hkVector4::getZero());
		hkTransform t;
		t.setIdentity();
		hkVector4 trans(-10.0f, 8.0f, 0.0f);
		t.setTranslation(trans);
		m_movingBody->setTransform(t);

	}

	// Otherwise we check to see if 100 frames have elapsed since the last change, and if so, we toggle the friction.

	if ( m_frameCount++ % m_frictionChangePeriod == 0 )
	{
		//
		// Here we oscillate the friction between 1 and 0
		//
		
		if ( m_movingBody->getMaterial().getFriction() == 1 )
		{
			 m_movingBody->getMaterial().setFriction(0);
			 m_env->m_textDisplay->outputText( "Friction set to 0", 30,230, 0xffffffff, m_frictionChangePeriod);
		}
		else
		{
			 m_movingBody->getMaterial().setFriction(1);
			 m_env->m_textDisplay->outputText( "Friction set to 1", 30,230, 0xffffffff, m_frictionChangePeriod);
		}

		//
		// You have to call this utility function to update the friction on the existing contact points of the rigid body
		//

		hkpContactUpdater::updateContacts( m_movingBody );

		//
		// Make sure the rigid body is active (so it moves again if it had stopped).
		//
		m_movingBody->activate();

	}

	// As you can see we simply read the current friction value for the moving block and proceed to set it to the alternate
	// value. Once we have done this we must inform Havok to update the contacts points for the body. The call to
	// updateContacts(...) forces Havok to review all contact point information for the body and update it as neccessary.
	// Lastly, we call activate(...) to ensure the body will react to the changes as it may have deactivated itself
	// due to remaining stationary for a long period of time.

	m_world->unlock();

	return hkDefaultPhysicsDemo::stepDemo(); 
}


#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
"A demo which shows how to dynamically update the friction of a rigid body, including all contacts associated with the body. " \
"Merely changing the body's material property will not correctly update the friction associated with any contact point which " \
"currently exist between the body and other bodies. ";

HK_DECLARE_DEMO(FrictionChangeDemo, HK_DEMO_TYPE_PRIME, "Dynamically updating the friction of a rigidbody", helpString);

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
