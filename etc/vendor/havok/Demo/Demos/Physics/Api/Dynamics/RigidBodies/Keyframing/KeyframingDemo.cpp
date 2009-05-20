/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/Physics/Api/Dynamics/RigidBodies/Keyframing/KeyframingDemo.h>

#include <Physics/Utilities/Constraint/Keyframe/hkpKeyFrameUtility.h>



KeyframingDemo::KeyframingDemo(hkDemoEnvironment* env)
:	hkDefaultPhysicsDemo(env)
{
	//
	// Set parameters for keyframes, and number of bodies
	//
	m_speed = 0.2f;
	m_radius = 5.0f;
	m_numBodies = 15;

	m_time = 0.0f;


	//
	// Setup the camera
	//
	{
		hkVector4 from(0, 40, 40);
		hkVector4 to(0, 0, 0);
		hkVector4 up(0, 1, 0);
		setupDefaultCameras(env, from, to, up);
	}


	//
	// Setup and create the hkpWorld.
	//
	{
		hkpWorldCinfo info;
		info.m_gravity = hkVector4(0.0f, -9.8f, 0.0f);
		info.setBroadPhaseWorldSize(150.0f);
		info.setupSolverInfo(hkpWorldCinfo::SOLVER_TYPE_4ITERS_MEDIUM);
		
		m_world = new hkpWorld( info );
		m_world->lock();

		setupGraphics();		
	}

	//
	// Register the single agent required (a hkpBoxBoxAgent).
	//
	{
		hkpAgentRegisterUtil::registerAllAgents(m_world->getCollisionDispatcher());
	}


	//
	// Create the rigid bodies.
	//
	createBodies();

	//
	// Create the ground.
	//
	createGround();



	//
	// Create the keyframed body.
	//
	{
		const hkVector4 halfExtents(3.0f, 1.75f, 0.3f);
		hkpBoxShape* shape = new hkpBoxShape(halfExtents, 0 );

		// Assign the rigid body properties
		hkpRigidBodyCinfo bodyInfo;
		bodyInfo.m_shape = shape;

		// By setting the motion type to hkpMotion::MOTION_KEYFRAMED we are essentially telling Havok that this
		// body has infinite mass. Thus neither applying forces nor impulses can change the velocity of the body and
		// as a result, the body is considered totally immovable during interactions (collisions) with other bodies.
		// This means that the user can force the body to follow a set of keyframes by directly setting the
		// velocity required to move to the next keyframe before each world step.
		// The body is guaranteed to reach the next keyframe, even if other bodies collide with it.
		// To help convert keyframes to velocities we have provided some useful methods in hkpKeyFrameUtility.
		// To create a keyframed object simply set the motion type as follows:

		bodyInfo.m_motionType = hkpMotion::MOTION_KEYFRAMED;

		{
			// Get inital keyframe.
			getKeyframePositionAndRotation(0.0f, bodyInfo.m_position, bodyInfo.m_rotation);
		}


		// Add our keyframed body.
		m_keyframedBody = new hkpRigidBody(bodyInfo);
		m_world->addEntity(m_keyframedBody);
		m_keyframedBody->removeReference();

		shape->removeReference();
	}

	m_world->unlock();
}


// During the stepDemo(...) method we update the movement of the keyframed body by setting
// new velocites using the hkpKeyFrameUtility to convert from position/rotation to linear/angular velocites.
// To do so, we make a call to the getKeyframePositionAndRotation(...) method which contains the following code:

hkDemo::Result KeyframingDemo::stepDemo()
{
	{
		m_world->lock();

		hkVector4 pos;
		hkQuaternion rot;

		// Use 'required' position/rotation as the one at the end of this timestep.
		// The hkpKeyFrameUtility finds the correct velocites to ensure this keyframe is reached
		// in the next timestep.

		getKeyframePositionAndRotation(m_time + m_timestep, pos, rot);

		// As you can see this code causes the 'stirrer' to be keyframed in a circular motion. Since our 'bowl' causes
		// the boxes to constantly tumble down into the 'stirrer' we see a continuous mixing effect.
		//
		// After calculating the correct position and orientation for our object we need to place it there. This can we
		// done with the help of some methods in the hkpKeyFrameUtility class. However, before we outline the required
		// method a brief description of so-called 'hard' and 'soft' keyframes would be useful.

		// A 'hard' keyframe allows for no 'give' in trying to get to the next keyframe. The velocity
		// will be set to absolutely ensure the next keyframe will be reached after the next timestep,
		// no matter where the body currently is (and no matter what other bodies it may collide with).
		// The alternative is a 'soft' keyframe, where the velocity is based on how far the body 
		// currently is from both the last and next keyframes, and will not ensure the keyframes
		// are always reached.

		hkpKeyFrameUtility::applyHardKeyFrame(pos, rot, 1.0f / m_timestep, m_keyframedBody);

		// Once we have keyframed our object all that remains to do is step the simulation:

		m_world->unlock();
	}

	hkDefaultPhysicsDemo::stepDemo();

	{
		m_world->lock();

		// At this point the keyframed body will have transform equal to (pos,rot)

		m_time += m_timestep;

		m_world->unlock();
	}

	return DEMO_OK;
}

KeyframingDemo::~KeyframingDemo()
{

}


// In this demo we have, by default, fifteen boxes being mixed around a 'bowl' by a 'stirrer', all of
// which are hkBoxShapes. The 'bowl' is constructed with four boxes, each given a different orientation
// and position, such that they form the sides of the object. Since we have set each of the components
// of the bowl to have a hkpMotion::MOTION_FIXED motion type they may safe overlap without any performance penalty
// (Havok will ignore FIXED - FIXED interpenetrations).

void KeyframingDemo::createGround()
{
	///
	const hkVector4 halfExtents(12.0f, 1.0f, 12.0f);
	hkpShape* groundShape = new hkpBoxShape(halfExtents, 0 );

	hkpRigidBodyCinfo bodyInfo;
	bodyInfo.m_mass = 0.0f;	
	bodyInfo.m_shape = groundShape;
	bodyInfo.m_motionType = hkpMotion::MOTION_FIXED;
	bodyInfo.m_position.set(0.0f, 0.0f, 0.0f);
	hkVector4 axis(1.0f, 0.0f, 0.0f);


	{
		axis.set(1,0,1);
		axis.normalize3();
		bodyInfo.m_rotation = hkQuaternion(axis, 0.5f);
		bodyInfo.m_position.set(10.0f, 0.0f, -10.0f);
		hkpRigidBody* groundBody = new hkpRigidBody(bodyInfo);

		m_world->addEntity(groundBody);
		groundBody->removeReference();
	}

	{
		axis.set(-1,0,1);
		axis.normalize3();
		bodyInfo.m_rotation = hkQuaternion(axis, 0.5f);
		bodyInfo.m_position.set(10.0f, 0.0f, 10.0f);
		hkpRigidBody* groundBody = new hkpRigidBody(bodyInfo);

		m_world->addEntity(groundBody);
		groundBody->removeReference();
	}

	{
		axis.set(1,0,-1);
		axis.normalize3();
		bodyInfo.m_rotation = hkQuaternion(axis, 0.5f);
		bodyInfo.m_position.set(-10.0f, 0.0f, -10.0f);
		hkpRigidBody* groundBody = new hkpRigidBody(bodyInfo);

		m_world->addEntity(groundBody);
		groundBody->removeReference();
	}


	{
		axis.set(-1,0,-1);
		axis.normalize3();
		bodyInfo.m_rotation = hkQuaternion(axis, 0.5f);
		bodyInfo.m_position.set(-10.0f, 0.0f, 10.0f);
		hkpRigidBody* groundBody = new hkpRigidBody(bodyInfo);

		m_world->addEntity(groundBody);
		groundBody->removeReference();
	}
	
	groundShape->removeReference();
}


// The boxes are created in the usual manner by filling in the various members of the hkpRigidBodyCinfo template,
// instantiating a new object and adding it to the world. Each box is given a different position in the 'bowl' using the
// following code:

void KeyframingDemo::createBodies()
{	

	// Each body needs at least one shape
	const hkVector4 halfExtents(1.0f, 0.75f, 1.0f);
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


	bodyInfo.m_friction = 0.1f;

	for(int i = 0; i < m_numBodies; i++)
	{
		bodyInfo.m_position.set( hkMath::cos(hkReal(i)) - 2.0f, i * 2.0f, hkMath::sin(hkReal(i)));
		hkpRigidBody* body = new hkpRigidBody(bodyInfo);
		m_world->addEntity(body);
		body->removeReference();
	}	
	shape->removeReference();

}

void KeyframingDemo::getKeyframePositionAndRotation(hkReal t, hkVector4& pos, hkQuaternion& rot)
{
	t *= 2 * HK_REAL_PI * m_speed;
	pos.set(m_radius * hkMath::cos(t), -2.0f, -m_radius * hkMath::sin(t));

	hkVector4 axis(0,1,0);
	rot.setAxisAngle(axis, t);

}

#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
"This demo shows how a rigid body can be 'keyframed' (i.e. the body's position/rotation is entirely under user " \
"control) and still allow it to interact correctly with other dynamic rigid bodies. " \
"It uses the hkpKeyFrameUtility to convert user keyframes into " \
"velocities which are applied to a rigid body with motion type set to hkpMotion::MOTION_KEYFRAMED. ";

HK_DECLARE_DEMO(KeyframingDemo, HK_DEMO_TYPE_PRIME | HK_DEMO_TYPE_CRITICAL, "Keyframing bodies. A 'stirrer' is keyframed to 'mix' other bodies.", helpString);

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
