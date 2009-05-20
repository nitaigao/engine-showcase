/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/Physics/Api/Constraints/ConstraintKit/ConstraintKitDemo.h>


// We will need these collision agents
#include <Physics/Collide/Agent/ConvexAgent/BoxBox/hkpBoxBoxAgent.h>

// The construction kit used to build the generic constraint
#include <Physics/Dynamics/Constraint/ConstraintKit/hkpConstraintConstructionKit.h>

// We need to display the constraint
#include <Common/Visualize/hkDebugDisplay.h>


ConstraintKitDemo::ConstraintKitDemo(hkDemoEnvironment* env)
:	hkDefaultPhysicsDemo(env)
{
	//
	// Setup the camera
	//
	{
		hkVector4 from(0.0f, 5.0f, 15.0f);
		hkVector4 to  (0.0f, -2.0f, 0.0f);
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
		info.m_enableDeactivation = false;
		m_world = new hkpWorld( info );
		m_world->lock();

		setupGraphics();
	}

	//
	// Register the box-box collision agent
	//
	{
		hkpAgentRegisterUtil::registerAllAgents( m_world->getCollisionDispatcher() );
	}

	//
	// Create vectors to be used for setup
	//

	hkVector4 halfSize(0.5f, 0.5f, 0.5f);

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
		info.m_position.set(0.0f, 0.0f, 0.0f);
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
		info.m_position.set(2.0f, 2.0f, 0.0f);
		info.m_shape = boxShape;
		
			// Compute the box inertia tensor
		hkpMassProperties massProperties;
		info.m_mass = 10.0f;
		hkpInertiaTensorComputer::computeBoxVolumeMassProperties(halfSize, info.m_mass, massProperties);
		info.m_inertiaTensor = massProperties.m_inertiaTensor;
		info.m_centerOfMass = massProperties.m_centerOfMass;	
		info.m_motionType = hkpMotion::MOTION_BOX_INERTIA;

		moveableBody = new hkpRigidBody(info);

			// Add some damping to this body to allow it to come to rest.
		moveableBody->setAngularDamping(0.1f);
		moveableBody->setLinearDamping(0.1f);

		m_world->addEntity(moveableBody);
		moveableBody->removeReference();	
	}

	// Done with shape - remove reference since bodies have ownership
	boxShape->removeReference();



	//
	// CREATE GENERIC CONSTRAINT
	// 
	{
		
		hkpGenericConstraintData* constraintData = new hkpGenericConstraintData();
		
		hkpConstraintConstructionKit kit;

			// Must always start with this command (the constraintData works like a program, every
			// command being executed sequentially).
		kit.begin(constraintData);
		{
				// Set the pivots. These will be the points which are constrained by any linear constraintData
				// specified later. The fixed body has its pivot in the middle of its base. The movable
				// body has its pivot on one corner.
			hkVector4 pivotB(0.0f, -0.5f, 0.0f);
			m_pivotBIndex = kit.setPivotB(pivotB);
			hkVector4 pivotA(-0.5f, 0.5f, 0.5f);
			m_pivotAIndex = kit.setPivotA(pivotA);

				// Set the axis	in World space. This means we can easily update it
				// to pass through the pivots of the two bodies in our step code. Initially
				// we calculate this using the current location of the bodies and their pivots
			// <programlisting id="setInWorldSpace">
			const int axisId = 0;
			hkVector4 axis0;
			{
				hkVector4 pivotAW, pivotBW;
				pivotAW.setTransformedPos(moveableBody->getTransform(), pivotA);
				pivotBW.setTransformedPos(fixedBody->getTransform(), pivotB);
				axis0.setSub4(pivotBW, pivotAW);
				axis0.normalize3();
			}
				// Record the index we get back when we set this axis so we can query the
				// constraintData later using it as a "parameter ID".
			m_axisIndex = kit.setLinearDofWorld(axis0, axisId);
			

				// Set the limits of this axis ([0, 5])
			
			kit.setLinearLimit(axisId, 0.0f, 5.0f);
			
		}
			// Must always end with this command.
		
		kit.end();
		
				
		hkpConstraintInstance* constraint = new hkpConstraintInstance( moveableBody, fixedBody, constraintData );
		m_world->addConstraint(constraint);
		m_constraintData = constraintData;
		m_constraint = constraint;
	}

	m_world->unlock();
}


ConstraintKitDemo::~ConstraintKitDemo()
{
	m_constraint->removeReference();
	m_constraintData->removeReference();
}

	// Display the constraint, and update this constrained axis (which is specified in World space).
	// If we update the axis to always pass through the pivots of the two bodies it will act
	// correctly no matter how the bodies are oriented.
hkDemo::Result ConstraintKitDemo::stepDemo()
{
	//
	// Work out pivots in World space
	//
	hkVector4 pivotAW, pivotBW;
	{
		hkVector4 pivotA = *(m_constraintData->getParameters(m_pivotAIndex));
		hkVector4 pivotB = *(m_constraintData->getParameters(m_pivotBIndex));

		hkpRigidBody* ra = (hkpRigidBody*) m_constraint->getEntityA();
		hkpRigidBody* rb = (hkpRigidBody*) m_constraint->getEntityB();

		pivotAW.setTransformedPos(ra->getTransform(), pivotA);
		pivotBW.setTransformedPos(rb->getTransform(), pivotB);
	}

	hkVector4 newAxis;
	newAxis.setSub4(pivotAW, pivotBW);
	hkReal length = newAxis.length3();

	// Only update axis if length non-zero (otherwise we can't normalize it correctly)
	if(length > 1e-3f)
	{
		newAxis.normalize3();
		m_constraintData->setParameters(m_axisIndex, 1, &newAxis);
	}

	// Set colour as RED if outside limits (constraint forces applied)
	// or as YELLOW if inside (constraint has no effect, rope is "slack").
	unsigned int colour = (length > 5.0f) ? hkColor::RED : hkColor::YELLOW;

	HK_DISPLAY_LINE(pivotAW, pivotBW, colour);
	
	return hkDefaultPhysicsDemo::stepDemo();
}

////////////////////////////////////////////////////////////////////////////////

#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
"This demo illustrates how to use the constraint toolkit to build a user-defined constraint. " \
"In this case we build a rope constraint by using a single linear constraint, modified during simulation " \
"to always be in the direction from the pivot on one body to the pivot on the other. We  " \
"illustrate this by fixing one of the bodies. Thus the constraint has the effect of restricting  " \
"the motion of the other body so that it is free to move only within a sphere of radius equal to the " \
"length of the rope, i.e. the linear limit specified. Inside the sphere, the constraint has no effect " \
"(the rope is slack), it only prevents the body moving outside the sphere.";

HK_DECLARE_DEMO(ConstraintKitDemo, HK_DEMO_TYPE_PRIME, "Using the ConstaintKit to build a 'rope' constraint", helpString);

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
