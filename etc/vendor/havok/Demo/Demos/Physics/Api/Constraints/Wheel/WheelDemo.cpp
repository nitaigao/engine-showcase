/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/Physics/Api/Constraints/Wheel/WheelDemo.h>


#include <Graphics/Common/Input/Pad/hkgPad.h>

#include <Common/Internal/ConvexHull/hkGeometryUtility.h>

// We will need these collision agents
#include <Physics/Collide/Agent/ConvexAgent/BoxBox/hkpBoxBoxAgent.h>

// We need to create a constraint
#include <Physics/Dynamics/Constraint/Bilateral/Wheel/hkpWheelConstraintData.h>

// We want to turn on constraint viewers for this demo
#include <Physics/Utilities/VisualDebugger/Viewer/Dynamics/hkpConstraintViewer.h>

// Needed for legend
#include <Common/Base/Types/Color/hkColor.h>
#include <Graphics/Common/Window/hkgWindow.h>
#include <Demos/DemoCommon/DemoFramework/hkTextDisplay.h>


WheelDemo::WheelDemo(hkDemoEnvironment* env)
:	hkDefaultPhysicsDemo(env)
{

	//
	// Setup the camera
	//
	{
		hkVector4 from(-5.0f, 5.0f, 5.0f);
		hkVector4 to  ( 0.0f, 0.0f, 0.0f);
		hkVector4 up  ( 0.0f, 1.0f, 0.0f);
		setupDefaultCameras( env, from, to, up );
	}

	//
	// Create the world
	//
	{
		hkpWorldCinfo info;
		info.setupSolverInfo(hkpWorldCinfo::SOLVER_TYPE_4ITERS_MEDIUM); 
		info.setBroadPhaseWorldSize( 100.0f );
		info.m_enableDeactivation = false;
		m_world = new hkpWorld( info );
		m_world->lock();

		// No agents need to be registered here, as we disable all collisions between chassis and wheel below

		//
		// Create constraint viewer
		//
		m_debugViewerNames.pushBack( hkpConstraintViewer::getName() );
		hkpConstraintViewer::m_scale = 1.0f;
	

		setupGraphics();
	}

	//
	// Disable collisions between "wheel" and "chassis", 
	// In our case we use the system groups. That means every vehicle gets it's system
	// group with all collision disabled between it's members.
	// We do this because it is likely that in many situations the "wheel" will be close to
	// (or interpenetrating with) the "chassis", and we do not actually wish any contact to occur
	// (since it is the constraint which keeps the wheel in place).
	//
	int wheelSystemCollisionGroup;
	{
		hkpGroupFilter* filter = new hkpGroupFilter();
		hkpGroupFilterSetup::setupGroupFilter( filter );

		wheelSystemCollisionGroup = filter->getNewSystemGroup();

		m_world->setCollisionFilter( filter );
		filter->removeReference();
	}

	//
	// Create vectors to be used for setup
	//
	hkVector4 axle(1.0f, 0.0f, 0.0f);
	hkVector4 position(0.0f, 0.0f, 0.0f);


	//
	// Create chassis (fixed) rigid body
	//
	{
		hkpRigidBodyCinfo info;
		info.m_position = position;
		hkVector4 boxHalfSize( 0.5f, .5f, 1.0f );
		info.m_shape = new hkpBoxShape(boxHalfSize, 0 );

		info.m_motionType = hkpMotion::MOTION_FIXED;
		m_chassis = new hkpRigidBody(info);
		info.m_shape->removeReference();

		info.m_collisionFilterInfo = hkpGroupFilter::calcFilterInfo( hkpGroupFilterSetup::LAYER_DYNAMIC, wheelSystemCollisionGroup );

		m_world->addEntity(m_chassis);
	}

	//
	// Create wheel (movable) rigid body
	//
	{
		hkVector4 pos = position;
		pos(0) -= 1.0f;
		m_wheelRigidBody = createDisc(1.0f, 0.5f, 10.0f, pos);

		// Set group as wheelSystemCollisionGroup
		m_wheelRigidBody->setCollisionFilterInfo( 
			hkpGroupFilter::calcFilterInfo( hkpGroupFilterSetup::LAYER_DYNAMIC, wheelSystemCollisionGroup ) );
		m_world->addEntity(m_wheelRigidBody);

		hkVector4 angvel;
		angvel.setMul4(-0.8f, axle);
		m_wheelRigidBody->setAngularVelocity(angvel);
	}


	//
	// CREATE WHEEL CONSTRAINT
	// 
	{
		m_wheelConstraint = new hkpWheelConstraintData();

		hkVector4 suspension(1.0f, 1.0f, 0.0f);
		suspension.normalize3();
		hkVector4 steering(0.0f, 1.0f, 0.0f);

		m_wheelConstraint->setInWorldSpace( m_wheelRigidBody->getTransform(), m_chassis->getTransform(), 
											m_wheelRigidBody->getPosition(), axle, suspension, steering );
		m_wheelConstraint->setSuspensionMaxLimit(0.2f);
		m_wheelConstraint->setSuspensionMinLimit(-0.5f);
		
		m_wheelConstraint->setSuspensionStrength(0.01f);
		m_wheelConstraint->setSuspensionDamping(m_world->m_dynamicsStepInfo.m_solverInfo.m_damping * 0.25f);


	
		//
		//	Create and add the constraint
		//
		{
			hkpConstraintInstance* constraint = new hkpConstraintInstance( m_wheelRigidBody, m_chassis, m_wheelConstraint );
			m_world->addConstraint(constraint);
			constraint->removeReference();

		}		
	}


	// Initially, steering angle is zero
	m_steeringAngle = 0.0f;

	m_world->unlock();
}




WheelDemo::~WheelDemo()
{
	m_world->lock();
	m_chassis->removeReference();
	m_wheelRigidBody->removeReference();
	m_wheelConstraint->removeReference();
	m_world->unlock();
}

// Read the keypad and rotate wheel/apply torque if required.
hkDemo::Result WheelDemo::stepDemo()
{
	const hkReal maxAngle = (HK_REAL_PI * .25f);
	const hkgPad* pad = m_env->m_gamePad;

	m_wheelRigidBody->markForWrite();

	if( (pad->getButtonState() & HKG_PAD_DPAD_LEFT) )
	{
		m_steeringAngle += 1.25f * m_timestep;
			// Clip steering angle to a max of PI/4 radians
		if( m_steeringAngle < maxAngle)
		{
			m_wheelConstraint->setSteeringAngle( m_steeringAngle );
		}
		else
		{
			m_steeringAngle = maxAngle;
		}
	}

	if( (pad->getButtonState() & HKG_PAD_DPAD_RIGHT) )
	{
		m_steeringAngle -= 1.25f * m_timestep;
			// Clip steering angle to a min of -PI/4 radians
		if( m_steeringAngle > -maxAngle)
		{
			m_wheelConstraint->setSteeringAngle( m_steeringAngle );
		}
		else
		{
			m_steeringAngle = -maxAngle;
		}
	}

		// If forward/back hit, apply torque to turn wheel around the axle.
		// Torque is applied in World space, so we need to calculate where the axle
		// is in World space.
	if( (pad->getButtonState() & HKG_PAD_DPAD_UP) )
	{
		hkVector4 torque;

		torque.setRotatedDir( m_wheelRigidBody->getRotation(), m_wheelConstraint->m_atoms.m_steeringBase.m_rotationA.getColumn(hkpWheelConstraintData::Atoms::AXIS_AXLE) ); 
		torque.setMul4( -20.0f, torque ); 
		torque.mul4( m_timestep );
		m_wheelRigidBody->applyAngularImpulse( torque );
	}

	if( (pad->getButtonState() & HKG_PAD_DPAD_DOWN) )
	{
		hkVector4 torque;

		torque.setRotatedDir( m_wheelRigidBody->getRotation(), m_wheelConstraint->m_atoms.m_steeringBase.m_rotationA.getColumn(hkpWheelConstraintData::Atoms::AXIS_AXLE) ); 
		torque.setMul4( 20.0f, torque ); 
		torque.mul4( m_timestep );
		m_wheelRigidBody->applyAngularImpulse( torque );
	}

	m_wheelRigidBody->unmarkForWrite();

	// Legend
	{
		const int h = m_env->m_window->getHeight();
		m_env->m_textDisplay->outputText("Yellow: Suspension axis", 20, h-90, (hkUint32)hkColor::GREEN, 1);
		m_env->m_textDisplay->outputText("Green:  Steering axis", 20, h-70, (hkUint32)hkColor::YELLOW, 1);
		m_env->m_textDisplay->outputText("White:  Suspension limits", 20, h-50, (hkUint32)hkColor::WHITE, 1);
		m_env->m_textDisplay->outputText("Blue:   Axle", 20, h-30, (hkUint32)hkColor::BLUE, 1);
	}

	return hkDefaultPhysicsDemo::stepDemo();
}





// This helper function creates a compound body which is "wheel-like", ie is an extruded regular n-gon
hkpRigidBody*  WheelDemo::createDisc(hkReal radius, hkReal height, hkReal mass, hkVector4& position)
{
	const int numSides = 20;
	
	hkInplaceArrayAligned16<hkVector4,numSides> vertices;

	//
	// Create the vertices array
	//
	for(int i = 0 ; i < numSides; i++)
	{
		hkVector4 trans(0.0f, radius, 0.0f);

		hkReal angle = HK_REAL_PI * 2.0f * i / (hkReal) numSides;
		hkVector4 axis(1.0f, 0.0f, 0.0f);
		hkQuaternion q(axis, angle);
		trans.setRotatedDir(q, trans);

		hkVector4 v = trans;
		v(0) = -.5f * height;
		vertices.pushBack(v);
		v(0) = .5f * height;
		vertices.pushBack(v);
		
	}


	hkpConvexVerticesShape* shape;
	hkArray<hkVector4> planeEquations;
	hkGeometry geom;
	{
		hkStridedVertices stridedVerts;
		{
			stridedVerts.m_numVertices = vertices.getSize();
			stridedVerts.m_striding = sizeof(hkVector4);
			stridedVerts.m_vertices = &(vertices[0](0));
		}

		hkGeometryUtility::createConvexGeometry( stridedVerts, geom, planeEquations );

		{
			stridedVerts.m_numVertices = geom.m_vertices.getSize();
			stridedVerts.m_striding = sizeof(hkVector4);
			stridedVerts.m_vertices = &(geom.m_vertices[0](0));
		}

		shape = new hkpConvexVerticesShape(stridedVerts, planeEquations);
	}


	hkpRigidBodyCinfo rigidBodyInfo;
	rigidBodyInfo.m_shape = shape;

	//
	// Create the rigid body 
	//
	hkpMassProperties massProperties;
	hkpInertiaTensorComputer::computeShapeVolumeMassProperties(shape, mass, massProperties);
	rigidBodyInfo.m_inertiaTensor = massProperties.m_inertiaTensor;
	rigidBodyInfo.m_centerOfMass = massProperties.m_centerOfMass;
	rigidBodyInfo.m_mass = massProperties.m_mass;
	rigidBodyInfo.m_motionType = hkpMotion::MOTION_BOX_INERTIA;
	

	rigidBodyInfo.m_position = position;

	hkpRigidBody* compoundRigidBody = new hkpRigidBody(rigidBodyInfo);


	shape->removeReference();

	return compoundRigidBody;
}


/////////////////////////////////////////////////////////



#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
"A demo which shows a single wheel constraint. " \
"You can slide the wheel along the suspension axis with the mouse spring. " \
"You can steer/accelerate/decelerate the wheel using the arrow keys/direction buttons. \n" \
"Note in this example we deliberately set up the suspension axis at 45 degrees to up.";

HK_DECLARE_DEMO(WheelDemo, HK_DEMO_TYPE_PRIME, "A single Wheel constraint", helpString);

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
