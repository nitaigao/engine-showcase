/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/Physics/Api/Constraints/BreakableConstraint/BreakableConstraintDemo.h>


// We will need these collision agents
#include <Physics/Collide/Agent/ConvexAgent/BoxBox/hkpBoxBoxAgent.h>

// We need to create constraints
#include <Physics/Dynamics/Constraint/Bilateral/Hinge/hkpHingeConstraintData.h>
#include <Physics/Dynamics/Constraint/Breakable/hkpBreakableConstraintData.h>



BreakableConstraintDemo::BreakableConstraintDemo(hkDemoEnvironment* env)
:	hkDefaultPhysicsDemo(env)
{
	//
	// Setup the camera
	//
	{
		hkVector4 from(0.0f, 0.0f, 10.0f);
		hkVector4 to  (0.0f, 0.0f, 0.0f);
		hkVector4 up  (0.0f, 1.0f, 0.0f);
		setupDefaultCameras( env, from, to, up );
	}

	//
	// Create the world
	//
	{
		hkpWorldCinfo info;
		info.setupSolverInfo(hkpWorldCinfo::SOLVER_TYPE_4ITERS_MEDIUM); 
		info.setBroadPhaseWorldSize( 100.0f );
		m_world = new hkpWorld( info );
		m_world->lock();

		hkpAgentRegisterUtil::registerAllAgents(m_world->getCollisionDispatcher());

		setupGraphics();
	}

	//
	// Create vectors to be used for setup
	//
	hkVector4 smallHalfSize(0.2f, 0.2f, 0.2f );
	hkVector4 bigHalfSize(0.5f, 0.5f, 0.5f );
	
	//
	// Create Box Shapes
	//
	hkpBoxShape* smallBoxShape;
	hkpBoxShape* bigBoxShape;
	{
		smallBoxShape = new hkpBoxShape( smallHalfSize , 0 );
		bigBoxShape   = new hkpBoxShape( bigHalfSize , 0 );
	}

	//
	// Create fixed rigid body
	//
	hkpRigidBody* fixedBody;
	{
		hkpRigidBodyCinfo info;
		info.m_position.set(1.0f, 0.0f, 0.0f);
		info.m_shape = smallBoxShape;
		info.m_motionType = hkpMotion::MOTION_FIXED;
		fixedBody = new hkpRigidBody(info);
		m_world->addEntity(fixedBody);
		fixedBody->removeReference();	
	}


	//
	// Create movable rigid body with increased inertia
	//
	hkpRigidBody* moveableBody;
	{

		hkpRigidBodyCinfo info;
		info.m_position.set( 1.0f, -1.1f, 0.0f );
		info.m_shape = bigBoxShape;
		
			// Compute the box inertia tensor
		hkReal mass = 10.0f;
		hkReal inertiaIncrease = 5.0f;
		hkpInertiaTensorComputer::setShapeVolumeMassProperties(bigBoxShape, mass * inertiaIncrease, info);
		info.m_mass = mass;

		info.m_motionType = hkpMotion::MOTION_DYNAMIC;

		moveableBody = new hkpRigidBody(info);
		m_world->addEntity(moveableBody);
		moveableBody->removeReference();	
	}
	

	
	//
	// CREATE BREAKABLE CONSTRAINT
	// 
	{
			// set pivot
		hkVector4 pivot( 0.5f, -0.6f, .5f );
		hkVector4 axleDir( 0.f,0.f,1.f );

		// Create hinge constraint
		// Do not add it to the world, only add the "wrapped" breakable version below
		
		hkpHingeConstraintData*  axle = new hkpHingeConstraintData(); 
		axle->setInWorldSpace(moveableBody->getTransform(), fixedBody->getTransform(), pivot, axleDir );

		// Create breakable constraint wrapper
		hkpBreakableConstraintData* breaker = new hkpBreakableConstraintData( axle );
		breaker->setThreshold( 10 );
		breaker->setRemoveWhenBroken(true);
		breaker->setRevertBackVelocityOnBreak( true );

		// Remove reference to "axle" since the BreakableConstraint now owns it.
		axle->removeReference();

		// Add the breakable constraint to the world
		hkpConstraintInstance* constraint = new hkpConstraintInstance(moveableBody, fixedBody, breaker);
		m_world->addConstraint( constraint); 	

		constraint->removeReference();
		breaker->removeReference();
	}

	
	//
	// Create a heavy sphere
	//
	{
		hkpRigidBodyCinfo info;
		info.m_position.set( 0.2f, 5.0f, 0.0f );
		info.m_shape = new hkpSphereShape( .1f );
		
			// Compute the box inertia tensor
		hkpInertiaTensorComputer::setShapeVolumeMassProperties(smallBoxShape, 20.0f, info);
		info.m_motionType = hkpMotion::MOTION_DYNAMIC;

		hkpRigidBody* r = new hkpRigidBody(info);
		m_world->addEntity(r);
		r->removeReference();	
		info.m_shape->removeReference();
	}
	

	//
	// Clean up references to shapes
	//
	smallBoxShape->removeReference();
	bigBoxShape->removeReference();

	m_world->unlock();
}

#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
"This demo shows how to use the breakable constraint wrapper " \
"around a hinge constraint. When the constrained body receives an " \
"impact from another (falling) body, the constraint threshold is " \
"exceeded and the constraint breaks. ";

HK_DECLARE_DEMO(BreakableConstraintDemo, HK_DEMO_TYPE_PRIME, "A single breakable constraint wrapping a hinge", helpString);

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
