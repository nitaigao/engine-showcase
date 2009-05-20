/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/Physics/Api/Constraints/SpringDamperMotor/SpringDamperMotorDemo.h>



// The construction kit used to build the generic constraint
#include <Physics/Dynamics/Constraint/Bilateral/Prismatic/hkpPrismaticConstraintData.h>
#include <Physics/Dynamics/Constraint/Motor/SpringDamper/hkpSpringDamperConstraintMotor.h>
#include <Physics/Dynamics/Constraint/Motor/Callback/hkpCallbackConstraintMotor.h>
#include <Physics/Utilities/Actions/Spring/hkpSpringAction.h>
#include <Physics/ConstraintSolver/Constraint/hkpConstraintQueryIn.h>
#include <Physics/ConstraintSolver/Constraint/Motor/hkpMotorConstraintInfo.h>

static const char helpStringA[] = \
"In this demo we want to compare a simple hkpSpringAction with a hkSpringDamperMotor. "
"So we create two prismatic constraints, the left one uses a spring, the other uses a motor. "
"Use the mouse spring to see the stiffness and behavior";

static const char helpStringB[] = \
"In this demo we want to compare a simple hkpSpringAction with a hkSpringDamperMotor. "
"So we create two prismatic constraints, the left one uses a spring, the other uses a motor. "
"We use a higher spring constant and damping";

static const char helpStringC[] = \
"In this demo we want to compare a simple hkpSpringAction with a hkSpringDamperMotor. "
"So we create two prismatic constraints, the left one uses a spring, the other uses a motor. "
"We use a very high spring constant and damping, as a result the normal spring version explodes "
"(oscillates between two states). The motor is still stable, however to hard to get effected by "
"the mouse spring";

static const char helpStringD[] = \
"In this demo we show usage of hkpCallbackConstraintMotor. Note: This demo does not run correctly "
"on PS3 if SPUs are enabled/available.";


struct SpringDamperMotorVariant
{
	const char*	 m_name;
	float m_strength;
	float m_damping;
	bool m_useCallbackMotor;
	const char* m_details;
};


static const SpringDamperMotorVariant g_variants[] =
{
	{ "SoftAndSpringy", 100.0f,		5.0f,	false, helpStringA },
	{ "Hard",			10000.0f,	20.0f,  false, helpStringB },
	{ "CriticalHard",	400000.0f,	400.0f, false, helpStringC },
	{ "SoftAndSpringy (with callback motor)",	100.0f,		5.0f,	true, helpStringD }
};

	// This is the callback function used on Cpu and Ppu. 
	// When solving on Spu, we can only define a single callback function, which is shared among all hkCallbackConstraintMotors.
	// The function is defined in Physics/Dynamics/Constraint/Motor/Callback/hkSpuCallbackConstraintMotorDispatcher.h
	//
	// Note that you also have to uncomment the function implementation in the above file, if you run this demo for spu.
void HK_CALL SampleCallbackFunc(const hkpCallbackConstraintMotor& motor, const hkpConstraintMotorInput* input, hkpConstraintMotorOutput* output)
{
	// We're not interested in velocity
	output->m_targetVelocity = 0.0f;

	// We compute relative targetPosition for the motor directly from the given input.
	output->m_targetPosition = input->m_deltaTarget + input->m_positionError;

	hkReal invMass = 1.0f / input->m_virtualMass;
	hkReal dt = input->m_stepInfo->m_substepDeltaTime;

	// These are desired spring constant and dumping.
	// Those values need to be converted into simple tau and damping parameters used by the solver.
	hkReal springConstant = reinterpret_cast<const hkReal&>(motor.m_userData0);
	hkReal springDamping  = reinterpret_cast<const hkReal&>(motor.m_userData1);

	// Forces applied by the solver are proportional to tau and virtual mass of involved bodies.
	// Spring force is independent of the bodies' masses therefore we divide tau by their virtual mass.
	// To convert tau to solver space we multiply it by dt^2
	hkReal tau = springConstant * dt * dt * invMass ;
	output->m_tau = hkMath::clamp( tau, 0.0f, 1.0f );

	// Solver damping is computed analogically to tau.
	// To convert damping to solver space we multiply it by dt
	hkReal damp = springDamping * dt * invMass;
	output->m_damping = hkMath::clamp( damp, 0.0f, 1.0f );

	// Explicitly specify minimum and maximum forces that the motor can apply.
	output->m_maxForce = motor.m_maxForce;
	output->m_minForce = motor.m_minForce;
}

SpringDamperMotorDemo::SpringDamperMotorDemo(hkDemoEnvironment* env)
:	hkDefaultPhysicsDemo(env, DEMO_FLAGS_NO_SERIALIZE) //XXX
{
	const SpringDamperMotorVariant& variant =  g_variants[m_variantId];

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
		hkpWorldCinfo winfo;
		winfo.setupSolverInfo(hkpWorldCinfo::SOLVER_TYPE_4ITERS_MEDIUM); 
		winfo.setBroadPhaseWorldSize( 100.0f );
		winfo.m_enableDeactivation = false;
		m_world = new hkpWorld( winfo );
		m_world->lock();

		setupGraphics();
	}



	//
	// Create Box Shape
	//
	hkpBoxShape* boxShape;
	{
		hkVector4 halfSize(0.5f, 0.5f, 0.5f);
		boxShape = new hkpBoxShape( halfSize , 0 );
	}


	//
	// Create movable rigid bodies
	//
	hkpRigidBody* rb0;
	hkpRigidBody* rb1;
	{

		hkpRigidBodyCinfo info;
		info.m_shape = boxShape;
		
			// Compute the box inertia tensor
		hkpInertiaTensorComputer::setShapeVolumeMassProperties( boxShape, 10.0f, info);
		info.m_motionType = hkpMotion::MOTION_BOX_INERTIA;

		info.m_position.set(-1.0f, 0.0f, 0.0f);
		rb0 = new hkpRigidBody( info );

		info.m_position.set(1.0f, 0.0f, 0.0f);
		rb1 = new hkpRigidBody(info);

		m_world->addEntity(rb0)->removeReference();	
		m_world->addEntity(rb1)->removeReference();	
	}

	// Done with shape - remove reference since bodies have ownership
	boxShape->removeReference();


	const hkReal springConstant = variant.m_strength;
	const hkReal springDamping  = variant.m_damping;

	hkpRigidBody* fixedBody = m_world->getFixedRigidBody();
	//
	// CREATE SPRING POWERED PRISMATIC
	// 
	{
		hkpPrismaticConstraintData* pris = new hkpPrismaticConstraintData(); 

		// Create constraint
		hkVector4 axis(0.0f, 1.0f, 0.0f);
		pris->setInWorldSpace(rb0->getTransform(), fixedBody->getTransform(), rb0->getPosition(), axis);
		pris->setMaxLinearLimit(3.0f);
		pris->setMinLinearLimit(-3.0f);
				
		hkpConstraintInstance* constraint = new hkpConstraintInstance( rb0, fixedBody, pris );
		m_world->addConstraint(constraint);
		constraint->removeReference();
		pris->removeReference();

		// create spring
		hkpSpringAction* spring = new hkpSpringAction( rb0, fixedBody );
		spring->setPositionsInWorldSpace( rb0->getPosition(), rb0->getPosition() );
		spring->setStrength( springConstant );
		spring->setDamping( springDamping );
		spring->setRestLength( 0.0f );
		m_world->addAction( spring )->removeReference();
	}

	//
	// CREATE MOTOR POWERED PRISMATIC
	// 
	{
		hkpPrismaticConstraintData* pris = new hkpPrismaticConstraintData(); 

		// Create constraint
		hkVector4 axis(0.0f, 1.0f, 0.0f);
		pris->setInWorldSpace(rb1->getTransform(), fixedBody->getTransform(), rb1->getPosition(), axis);
		pris->setMaxLinearLimit(3.0f);
		pris->setMinLinearLimit(-3.0f);
				
		// create motor
		if (!variant.m_useCallbackMotor)
		{
			hkpSpringDamperConstraintMotor* spring = new hkpSpringDamperConstraintMotor( springConstant, springDamping );
			pris->setMotor( spring );
			spring->removeReference();
		}
		else
		{
			hkpCallbackConstraintMotor* ccm = new hkpCallbackConstraintMotor( hkpCallbackConstraintMotor::CALLBACK_MOTOR_TYPE_HAVOK_DEMO_SPRING_DAMPER, SampleCallbackFunc );

			ccm->m_userData0 = reinterpret_cast<const hkUlong&>(springConstant);
			ccm->m_userData1 = reinterpret_cast<const hkUlong&>(springDamping);

			pris->setMotor( ccm );
			ccm->removeReference();
		}
	
		hkpConstraintInstance* constraint = new hkpConstraintInstance( rb1, fixedBody, pris );
		m_world->addConstraint(constraint);
		pris->setMotorActive(constraint, true);
		constraint->removeReference();
		pris->removeReference();
	}

	m_world->unlock();
}


SpringDamperMotorDemo::~SpringDamperMotorDemo()
{
}


////////////////////////////////////////////////////////////////////////////////

#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif


HK_DECLARE_DEMO_VARIANT_USING_STRUCT( SpringDamperMotorDemo, HK_DEMO_TYPE_PRIME, SpringDamperMotorVariant, g_variants, helpStringA ); 

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
