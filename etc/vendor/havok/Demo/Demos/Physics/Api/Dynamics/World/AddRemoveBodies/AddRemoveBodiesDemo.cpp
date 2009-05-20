/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/Physics/Api/Dynamics/World/AddRemoveBodies/AddRemoveBodiesDemo.h>


AddRemoveBodiesDemo::AddRemoveBodiesDemo(hkDemoEnvironment* env)
:	hkDefaultPhysicsDemo(env),
	m_frameCount(0),
	m_currentBody(0)
{
	for( int i = 0; i < NUM_BODIES; i++ )
	{
		m_bodies[i] = HK_NULL;		
	}	

	//
	// Setup the camera
	//
	{
		hkVector4 from(0.0f, 30.0f, 50.0f);
		hkVector4 to  (0.0f,  0.0f,  0.0f);
		hkVector4 up  (0.0f,  1.0f,  0.0f);
		setupDefaultCameras(env, from, to, up);

		float lightDir[] = { -1, -0.5f , -0.25f};
		float lightAabbMin[] = { -15, -15, -15};
		float lightAabbMax[] = { 15, 15, 15};
		setLightAndFixedShadow(lightDir, lightAabbMin, lightAabbMax );
	}


	//
	// Setup and create the hkpWorld
	//
	{
		hkpWorldCinfo info;
		info.m_gravity = hkVector4(0.0f, -9.8f, 0.0f);
		info.setBroadPhaseWorldSize(1000.0f);
		info.setupSolverInfo(hkpWorldCinfo::SOLVER_TYPE_8ITERS_MEDIUM);
		info.m_collisionTolerance = 0.01f; 
		
		m_world = new hkpWorld( info );
		m_world->lock();

		setupGraphics();		
	}

	// Register the single agent required (a hkpBoxBoxAgent)
	{
		hkpAgentRegisterUtil::registerAllAgents(m_world->getCollisionDispatcher());
	}

	// Create the rigid bodies
	createBodies();

	// Create the ground
	createGround();

	m_world->unlock();
}

hkDemo::Result AddRemoveBodiesDemo::stepDemo()
{
	m_world->lock();

	// add/remove a rigid body
	if(m_frameCount == 0)
	{
		// remove the body if it is already in the world
		if(m_bodies[m_currentBody]->isAddedToWorld())
		{
			removeBody(m_bodies[m_currentBody]);
		}

		// add the body
		addBody(m_bodies[m_currentBody]);

		// move onto the next body
		m_currentBody = (++m_currentBody % NUM_BODIES);
	}

	// update the frame counter
	m_frameCount = (++m_frameCount % NUM_FRAMES);

	m_world->unlock();

	return hkDefaultPhysicsDemo::stepDemo();
}

AddRemoveBodiesDemo::~AddRemoveBodiesDemo()
{
	m_world->lock();

	for( int i = 0; i < NUM_BODIES; i++ )
	{
		m_bodies[i]->removeReference();
	}

	m_world->unlock();
}


void AddRemoveBodiesDemo::createGround()
{
	const hkVector4 halfExtents(20.0f, 4.0f, 20.0f);
	hkpShape* groundShape = new hkpBoxShape(halfExtents, 0.05f );

	hkpRigidBodyCinfo bodyInfo;
	bodyInfo.m_mass = 0.0f;	
	bodyInfo.m_shape = groundShape;
	bodyInfo.m_motionType = hkpMotion::MOTION_FIXED;
	bodyInfo.m_position.set(0.0f, 0.0f, 0.0f);
	hkVector4 axis(0.0f, 0.0f, 1.0f);
	bodyInfo.m_rotation = hkQuaternion(axis, -0.3f);

	hkpRigidBody* groundBody = new hkpRigidBody(bodyInfo);
	groundShape->removeReference();

	m_world->addEntity(groundBody);
	groundBody->removeReference();	
}

void AddRemoveBodiesDemo::createBodies()
{	
	// Each body needs at least one shape
	const hkVector4 halfExtents(2.0f, 0.5f, 2.0f);
	hkpShape* shape = new hkpBoxShape(halfExtents, 0.05f );

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

	for(int i = 0; i < NUM_BODIES; i++)
	{
		hkpRigidBody *body = new hkpRigidBody(bodyInfo);
		m_bodies[i] = body;		
	}
	shape->removeReference();

}

void AddRemoveBodiesDemo::addBody(hkpRigidBody* currentBody)
{
	//
	// Add the current body to the simulation
	//

	// Zero velocites (still preserved perhaps from previous simulation)
	currentBody->setLinearVelocity(hkVector4::getZero());
	currentBody->setAngularVelocity(hkVector4::getZero());

	// Place above ground, unrotated
	hkVector4 position(-5.0f, 17.0f, 0.0f);
	currentBody->setPosition(position);
	hkQuaternion rot;
	rot.setIdentity();
	currentBody->setRotation(rot);

	// Add body
	m_world->addEntity(currentBody);
}

void AddRemoveBodiesDemo::removeBody(hkpRigidBody* body)
{	
	//
	// Remove the current body to the simulation
	//
	m_world->removeEntity(body);	
}


#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
"A demo which shows continuous addition/removal of bodies from a hkpWorld. " \
"Every 60 frames a body is added to the world. At most 10 are allowed in the  " \
"World, so once 10 have been added, the added body is recycled by removing oldest  " \
"body, and re-adding it.\n" \
"The actual removal/addition is done in the removeBody()/addBody() methods using calls to " \
"hkpWorld::removeEntity() and hkpWorld::addEntity(). ";

HK_DECLARE_DEMO(AddRemoveBodiesDemo, HK_DEMO_TYPE_PRIME, "Dynamic body addition/removal", helpString);

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
