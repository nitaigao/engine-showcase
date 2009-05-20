/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/Physics/Api/Dynamics/Actions/GravityAction/GravityActionDemo.h>

// This is the action.
#include <Demos/Physics/Api/Dynamics/Actions/GravityAction/GravityAction.h>

#include <Common/Base/Algorithm/PseudoRandom/hkPseudoRandomGenerator.h>

//#include <hkdemoframework/hkDemoFramework.h>

#include <Physics/Collide/hkpCollide.h>


GravityActionDemo::GravityActionDemo(hkDemoEnvironment* env)
:	hkDefaultPhysicsDemo(env)
{
	// Define world gravity as zero.
	hkVector4 gravity( 0.0f, 0.0f, 0.0f );
	
	//
	// Set up the camera.
	//
	{
		hkVector4 from(0.0f, 7.0f, 65.0f);
		hkVector4 to  (0.0f, 3.0f, 0.0f);
		hkVector4 up  (0.0f, 1.0f, 0.0f);
		setupDefaultCameras( env, from, to, up );

		forceShadowState(false);

	}
	
	//
	// Create the world.
	//
	{
		hkpWorldCinfo info;
		info.setupSolverInfo(hkpWorldCinfo::SOLVER_TYPE_4ITERS_MEDIUM); 
		info.m_gravity = gravity;	
		info.setBroadPhaseWorldSize( 1000.0f );
		info.m_broadPhaseBorderBehaviour = info.BROADPHASE_BORDER_REMOVE_ENTITY;
		m_world = new hkpWorld( info );
		m_world->lock();

		setupGraphics();
	}

	// Register all agents
	
	hkpAgentRegisterUtil::registerAllAgents( m_world->getCollisionDispatcher() );


	//
	// Create the "planet" shape to orbit around.
	//
	hkpRigidBody* planet = HK_NULL;
	const hkVector4 gravityCenter(0.0f, 0.0f, 0.0f);
	{
		hkReal radius = 5.0f;
		hkpSphereShape* sphereShape = new hkpSphereShape(radius);

		hkpRigidBodyCinfo sphereInfo;

		sphereInfo.m_shape = sphereShape;
		sphereInfo.m_position.set(gravityCenter(0), gravityCenter(1), gravityCenter(2));
		sphereInfo.m_angularDamping = 0.0f;
		sphereInfo.m_linearDamping = 0.0f;
		sphereInfo.m_friction = 0.0f;

		// Planet is fixed.
		sphereInfo.m_motionType = hkpMotion::MOTION_FIXED;

		// Create a rigid body (using the template above).
		planet = new hkpRigidBody(sphereInfo);

		// Remove reference since the hkpRigidBody now "owns" the Shape.
		sphereShape->removeReference();

		// Add rigid body so we can see it, and remove reference since the hkpWorld now "owns" it.
		m_world->addEntity(planet);
	}

	// Limit number of polygons as Havok renderer is not perfect!
#if defined(HK_PLATFORM_PS2) || defined(HK_PLATFORM_PS3_PPU)
		const int numBodies = 100;
#elif defined HK_PLATFORM_GC
		const int numBodies = 250;
#else
		const int numBodies = 100;
#endif

	//
	// Create the satellites.
	//
	{
		hkPseudoRandomGenerator generator(234);
		for(int i = 0; i < numBodies; i++)
		{
			hkVector4 halfExtents(0.25f, 0.25f, 0.25f);
			hkpBoxShape* satelliteShape = new hkpBoxShape(halfExtents, 0 );

			hkpRigidBodyCinfo satelliteInfo;

			satelliteInfo.m_shape = satelliteShape;
			satelliteInfo.m_position.set(   generator.getRandRange( 7.0f, 25.0f ),
											generator.getRandRange( 7.0f, 25.0f ),
											generator.getRandRange( 7.0f, 25.0f ) );

			satelliteInfo.m_angularDamping = 0.0f;
			satelliteInfo.m_linearDamping = 0.0f;
			satelliteInfo.m_friction = 0.0f;

			satelliteInfo.m_motionType = hkpMotion::MOTION_BOX_INERTIA;

			hkReal mass = 10.0f;
			hkpMassProperties massProperties;
			hkpInertiaTensorComputer::computeBoxVolumeMassProperties(halfExtents, mass, massProperties);

			satelliteInfo.m_inertiaTensor = massProperties.m_inertiaTensor;
			satelliteInfo.m_centerOfMass = massProperties.m_centerOfMass;
			satelliteInfo.m_mass = massProperties.m_mass;			
					
			// Create a rigid body (using the template above).
			hkpRigidBody* satellite = new hkpRigidBody(satelliteInfo);

			// Give each hkpRigidBody an initial velocity so they orbit in the same direction.
			hkVector4 lv(0.0f, -5.0f, 0.0f);
			satellite->setLinearVelocity( lv );

			// Remove reference since the body now "owns" the Shape
			satelliteShape->removeReference();

			// Finally add body so we can see it, and remove reference since the world now "owns" it.
			m_world->addEntity(satellite);

			const hkReal gravityConstant = 1000.0f;

			GravityAction* gravityAction = new GravityAction( satellite, gravityCenter, gravityConstant );

			
			m_world->addAction( gravityAction );

			// After addAction() m_world references antiGravityAction so the local reference can safely be removed.
			gravityAction->removeReference();

			satellite->removeReference();
			
		}
	}
	planet->removeReference();

	m_world->unlock();
}


#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
"A demonstration of the use of an action derived from hkpBinaryAction. A satellite body " \
"is attracted with a gravity action to a fixed planet body. " \
"NB: This is NOT the same as the gravity setting that is normally applied to the hkpWorld and " \
"set using hkpWorldCinfo::m_gravity ";

HK_DECLARE_DEMO(GravityActionDemo, HK_DEMO_TYPE_PRIME | HK_DEMO_TYPE_CRITICAL, "A unary action forcing rigid bodies toward a point, like gravity", helpString);

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
