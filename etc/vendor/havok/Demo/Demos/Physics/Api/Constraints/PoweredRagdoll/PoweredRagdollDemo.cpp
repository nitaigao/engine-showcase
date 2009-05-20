/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/Physics/Api/Constraints/PoweredRagdoll/PoweredRagdollDemo.h>


// We will need these collision agents
#include <Physics/Collide/Agent/ConvexAgent/BoxBox/hkpBoxBoxAgent.h>

// We need to create a constraint
#include <Physics/Dynamics/Constraint/Bilateral/Ragdoll/hkpRagdollConstraintData.h>

// We want to turn on constraint viewers for this demo
#include <Physics/Utilities/VisualDebugger/Viewer/Dynamics/hkpConstraintViewer.h>

#include <Physics/Dynamics/Constraint/Motor/Position/hkpPositionConstraintMotor.h>

PoweredRagdollDemo::PoweredRagdollDemo(hkDemoEnvironment* env)
: hkDefaultPhysicsDemo(env)
{
	m_time = 0.0f;

	//
	// Setup the camera
	//
	{
		hkVector4 from(5.0f, 2.0f, 5.0f);
		hkVector4 to(0.0f, 0.0f, 0.0f);
		hkVector4 up(0.0f, 1.0f, 0.0f);
		setupDefaultCameras( env, from, to, up );
	}


	//
	// Create the world
	//
	{
		hkpWorldCinfo info;
		info.setupSolverInfo(hkpWorldCinfo::SOLVER_TYPE_8ITERS_MEDIUM); 
		info.m_enableDeactivation = false;
		m_world = new hkpWorld( info );
		m_world->lock();

		//
		// Create constraint viewer
		//
		m_debugViewerNames.pushBack( hkpConstraintViewer::getName() );
		hkpConstraintViewer::m_scale = 1.0f;
		
		setupGraphics();

		// Register the single agent required (a hkpBoxBoxAgent)
		hkpAgentRegisterUtil::registerAllAgents( m_world->getCollisionDispatcher() );
	}
	
	//
	// Create vectors to be used for setup
	//
	hkVector4 pivot(0.0f, 0.0f, 0.0f);
	hkVector4 halfSize(1.0f, 0.25f, 0.5f);

	//
	// Create Box Shape
	//
	hkpBoxShape* boxShape;
	{
		boxShape = new hkpBoxShape( halfSize , 0 );
	}

	//
	// Create fixed rigid body
	//
	hkpRigidBody* fixedBody;
	{
		hkpRigidBodyCinfo info;
		info.m_position.set(-halfSize(0) - 1.0f, 0.0f, 0.0f);
		info.m_shape = boxShape;
		info.m_motionType = hkpMotion::MOTION_FIXED;
		fixedBody = new hkpRigidBody(info);
		m_world->addEntity(fixedBody);
		fixedBody->removeReference();	
	}


	//
	// Create movable rigid body
	//
	hkpRigidBody* moveableBody;
	{
		hkpRigidBodyCinfo info;
		info.m_position.set(halfSize(0) + 1.0f, 0.0f, 0.0f);
		info.m_shape = boxShape;
		
			// Compute the box inertia tensor
		hkpMassProperties massProperties;
		info.m_mass = 10.0f;
		hkpInertiaTensorComputer::computeBoxVolumeMassProperties(halfSize, info.m_mass, massProperties);
		info.m_inertiaTensor = massProperties.m_inertiaTensor;
		info.m_centerOfMass = massProperties.m_centerOfMass;	
		info.m_motionType = hkpMotion::MOTION_BOX_INERTIA;
		moveableBody = new hkpRigidBody(info);
		m_world->addEntity(moveableBody);
		moveableBody->removeReference();	
	}


	//
	//	Cleanup shape references
	//
	boxShape->removeReference();
	boxShape = HK_NULL;

	
	//
	// CREATE POWERED RAGDOLL CONSTRAINT
	// 
	{
		hkReal planeMin =  HK_REAL_PI * -0.2f;
		hkReal planeMax =  HK_REAL_PI * 0.1f;
		hkReal twistMin =  HK_REAL_PI * -0.1f;
		hkReal twistMax =  HK_REAL_PI *  0.4f;
		hkReal coneMin  =  HK_REAL_PI * 0.3f;

		hkpRagdollConstraintData* rdc = new hkpRagdollConstraintData();

		rdc->setConeAngularLimit(coneMin);
		rdc->setPlaneMinAngularLimit(planeMin);
		rdc->setPlaneMaxAngularLimit(planeMax);
		rdc->setTwistMinAngularLimit(twistMin);
		rdc->setTwistMaxAngularLimit(twistMax);

		hkVector4 twistAxis(1.0f, 0.0f, 0.0f);
		hkVector4 planeAxis(0.0f, 1.0f, 0.0f);
		pivot.set(0.0f, 0.0f, 0.0f);
		rdc->setInWorldSpace(moveableBody->getTransform(), fixedBody->getTransform(), pivot, twistAxis, planeAxis);


		//
		//	Create and add the constraint
		//
		hkpConstraintInstance* constraint = new hkpConstraintInstance(moveableBody, fixedBody, rdc );
		m_world->addConstraint(constraint);


		hkpPositionConstraintMotor* motor = new hkpPositionConstraintMotor( 0 );
		motor->m_tau = 0.6f;
		motor->m_maxForce = 1000.0f;
		motor->m_constantRecoveryVelocity = 0.1f;

		rdc->setTwistMotor( motor ); 
		rdc->setConeMotor( motor ); 
		rdc->setPlaneMotor( motor ); 
		rdc->setMotorsActive(constraint, true);

		motor->removeReference();

		hkRotation bRa;
		bRa.setTranspose( fixedBody->getTransform().getRotation() );
		bRa.mul( moveableBody->getTransform().getRotation() );
		
		rdc->setTargetRelativeOrientationOfBodies( bRa );

		m_constraintData = rdc;
		constraint->removeReference();
	}

	
	//
	//	So that we can actually see the motor in action, rotate the constrained body slightly.
	//  When simulation starts it will then by driven to the target position (frame).
	//
	{
		hkQuaternion rot;
		hkVector4 axis( 1.0f, 0.0f, 0.0f );
		axis.normalize3();
		rot.setAxisAngle( axis, 0.4f );
		moveableBody->setRotation( rot );
	}

	m_world->unlock();
}


PoweredRagdollDemo::~PoweredRagdollDemo()
{
	m_constraintData->removeReference();
}

hkDemo::Result PoweredRagdollDemo::stepDemo()
{
	m_time += 0.016f;

	hkReal sina = 0.3f * hkMath::sin( m_time * 2.0f);
	hkReal sinb = 0.3f * hkMath::sin( m_time * 0.2f);
	hkQuaternion q0; q0.setAxisAngle( hkTransform::getIdentity().getColumn(0), sina );
	hkQuaternion q1; q1.setAxisAngle( hkTransform::getIdentity().getColumn(1), sinb );
	hkQuaternion q; q.setMul( q1, q0 );

	hkRotation frame; frame.set( q );
	m_constraintData->setTargetRelativeOrientationOfBodies( frame );


	return hkDefaultPhysicsDemo::stepDemo();
}


#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
"A powered ragdoll constraint is a ragdoll constraint with an internal motor which can be used " \
"to drive the constrained bodies to a target position (relative frame). "
"Use Case: The motor can be used to make ragdolls appear stiffer as they fall.";

HK_DECLARE_DEMO(PoweredRagdollDemo, HK_DEMO_TYPE_PHYSICS, "A single Powered Ragdoll constraint", helpString);

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
