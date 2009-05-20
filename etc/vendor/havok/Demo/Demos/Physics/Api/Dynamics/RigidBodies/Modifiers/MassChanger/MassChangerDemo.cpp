/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/Physics/Api/Dynamics/RigidBodies/Modifiers/MassChanger/MassChangerDemo.h>

#include <Physics/Dynamics/Constraint/Bilateral/StiffSpring/hkpStiffSpringConstraintData.h>

#include <Physics/Utilities/Collide/ContactModifiers/MassChanger/hkpCollisionMassChangerUtil.h>

	// we need to access our game pad
#include <Graphics/Common/Input/Pad/hkgPad.h>




MassChangerDemo::MassChangerDemo(hkDemoEnvironment* env)
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
		//info.m_contactRestingVelocity = 10000.01f;

		m_world = new hkpWorld( info );
		m_world->lock();

		// Register ALL agents (though some may not be necessary)
		hkpAgentRegisterUtil::registerAllAgents(m_world->getCollisionDispatcher());

		setupGraphics();
	}


	//
	// Create the base
	//
	{
		hkVector4 baseSize( 15.0f, 0.3f, 15.0f);
		hkpConvexShape* shape = new hkpBoxShape( baseSize , 0 );

		hkpRigidBodyCinfo ci;

		ci.m_shape = shape;
		ci.m_motionType = hkpMotion::MOTION_FIXED;

		m_world->addEntity( new hkpRigidBody( ci ) )->removeReference();
		shape->removeReference();
	}	

	{
		//
		//	The dimensions of the boxes etc.
		//

		const hkReal boxDim = 1.0f;				// This is the size of the cubes
		const hkReal extBoxDim = 1.1f * boxDim;	// This is an extended size (used to shorten the pendulums)
		const hkReal heightOffGround = 5.0f;	// This is the height of the fixed box from the ground

		//create the shared properties/attributes for the boxes
		
		hkVector4 boxRadii(boxDim *.5f, boxDim *.5f, boxDim *.5f);
		hkpShape* boxShape = new hkpBoxShape( boxRadii , 0 ); 
		hkpMassProperties massProperties;
		hkpInertiaTensorComputer::computeBoxVolumeMassProperties(boxRadii, 100.0f, massProperties);
	
		hkpRigidBodyCinfo boxInfo;

		boxInfo.m_inertiaTensor = massProperties.m_inertiaTensor;
		boxInfo.m_mass = massProperties.m_mass;
		boxInfo.m_shape = boxShape;
		boxInfo.m_qualityType = HK_COLLIDABLE_QUALITY_CRITICAL;
		boxInfo.m_motionType = hkpMotion::MOTION_BOX_INERTIA;

		
		// create four boxes, one fixed and above the base, two attached by springs
		// to either side of the fixed box, and one below it. 
		// one of the pendulums will be in a higher position so that it hits the bottom
		// box later than the first	

		// Note: only one of the "pendulum" boxes will be 
		// able to influence the box on the ground (achieved
		// with mass changer utility)

		hkVector4 boxPos;

		// the box on the base
		{
			boxPos.set(0.0f,boxDim,0.0f,0.0f);
			boxInfo.m_position = boxPos;
			hkpRigidBody* boxRigidBody = new hkpRigidBody(boxInfo);
			m_bodies[0] = boxRigidBody;

			// Now add to world. Body is "ready to go" as soon as this is called, and display
			// is (as a registered listener) automatically notified to build a new display object.
			m_world->addEntity( boxRigidBody );
			boxRigidBody->removeReference();	// Remove reference, since we no longer want to remember this
		}

		// the pendulum boxes, number 1
		{
			float offset = hkMath::sqrtInverse(2.0f) * (heightOffGround - extBoxDim);
			boxPos.set( offset, heightOffGround + offset , 0.0f, 0.0f );
			boxInfo.m_position = boxPos;
			hkpRigidBody* boxRigidBody = new hkpRigidBody(boxInfo);
			m_bodies[1] = boxRigidBody;

			m_world->addEntity( boxRigidBody );
			boxRigidBody->removeReference();	// Remove reference, since we no longer want to remember this
		}

		// the pendulum boxes, number 2
		{
			boxPos.set(-heightOffGround + extBoxDim,heightOffGround,0.0f,0.0f);
			boxInfo.m_position = boxPos;
			hkpRigidBody* boxRigidBody = new hkpRigidBody(boxInfo);
			m_bodies[2] = boxRigidBody;

			m_world->addEntity( boxRigidBody );
			boxRigidBody->removeReference();	// Remove reference, since we no longer want to remember this
		}

		// the fixed box
		{
			boxPos.set(0.0f,heightOffGround,0.0f,0.0f);
			boxInfo.m_position = boxPos;
			boxInfo.m_motionType = hkpMotion::MOTION_FIXED;
			boxInfo.m_qualityType = HK_COLLIDABLE_QUALITY_FIXED;

			hkpRigidBody* boxRigidBody = new hkpRigidBody(boxInfo);
			m_bodies[3] = boxRigidBody;

			m_world->addEntity( boxRigidBody );
			boxRigidBody->removeReference();	// Remove reference, since we no longer want to remember this
		}

		//
		// create stiff spring constraint for pendulum 1
		// 
		{
			hkpStiffSpringConstraintData* spring = new hkpStiffSpringConstraintData();
			// Create constraint
			spring->setInWorldSpace(m_bodies[3]->getTransform(), m_bodies[1]->getTransform(), m_bodies[3]->getPosition(), m_bodies[1]->getPosition());
			m_world->createAndAddConstraintInstance( m_bodies[3], m_bodies[1], spring )->removeReference(); 	

			spring->removeReference();  
		}

		//
		// create stiff spring constraint for pendulum 2
		// 
		{
			// Create constraint
			hkpStiffSpringConstraintData* spring = new hkpStiffSpringConstraintData(); 
			spring->setInWorldSpace(m_bodies[3]->getTransform(), m_bodies[2]->getTransform(), m_bodies[3]->getPosition(), m_bodies[2]->getPosition());
			m_world->createAndAddConstraintInstance( m_bodies[3], m_bodies[2], spring )->removeReference(); 	
			spring->removeReference();  
		}

		//create a mass changer utility for the object
		const hkReal bodyARelInvMass = 0;
		const hkReal bodyBRelInvMass = 1;
		m_cmcu = new hkpCollisionMassChangerUtil( m_bodies[0], m_bodies[2], bodyARelInvMass, bodyBRelInvMass );

		boxShape->removeReference();
	}

	m_world->unlock();
}


hkDemo::Result MassChangerDemo::stepDemo()
{
	return hkDefaultPhysicsDemo::stepDemo();
}

MassChangerDemo::~MassChangerDemo()
{
	// Re-enable warnings:									
	hkError::getInstance().setEnabled(0xf0de4356, true);

	m_world->lock();
	delete m_cmcu;
	m_world->unlock();
}


#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
"A demo which creates three boxes and then uses a hkpCollisionMassChangerUtil to change the properties " \
"of two of the objects during their collisions.";

HK_DECLARE_DEMO(MassChangerDemo, HK_DEMO_TYPE_PRIME, "Changing mass properties during a collision", helpString);

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
