/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/Physics/Api/Dynamics/World/AddRemoveConstraints/AddRemoveConstraintsDemo.h>


// We need to create a constraint
#include <Physics/Dynamics/Constraint/Bilateral/BallAndSocket/hkpBallAndSocketConstraintData.h>

// We want to turn on constraint viewers for this demo
#include <Physics/Utilities/VisualDebugger/Viewer/Dynamics/hkpConstraintViewer.h>


AddRemoveConstraintsDemo::AddRemoveConstraintsDemo(hkDemoEnvironment* env)
:	hkDefaultPhysicsDemo(env),
	m_fixedBody(HK_NULL),
	m_dynamicBody(HK_NULL),
	m_constraint(HK_NULL),
	m_constraintAdded(false)
{
	//
	// Setup the camera
	//
	{
		hkVector4 from(0.0f, 10.0f, 30.0f);
		hkVector4 to  (0.0f,  0.0f,  0.0f);
		hkVector4 up  (0.0f,  1.0f,  0.0f);
		setupDefaultCameras(env, from, to, up);
	}

	//
	// Create the world
	//
	{
		hkpWorldCinfo info;
		info.setupSolverInfo(hkpWorldCinfo::SOLVER_TYPE_4ITERS_MEDIUM); 
		info.setBroadPhaseWorldSize( 100.0f );
		
		// Turn off deactivation so dyanmic body keeps moving.
		info.m_enableDeactivation = false;
		m_world = new hkpWorld( info );
		m_world->lock();

		// Create constraint viewer
		m_debugViewerNames.pushBack( hkpConstraintViewer::getName() );
		hkpConstraintViewer::m_scale = 1.0f;

		// setup the other viewers
		setupGraphics();
	}

	// Create the rigid bodies, one fixed, one dynamic.
	createBodies();	

	// Create and add the constraint
	hkpBallAndSocketConstraintData* bs = new hkpBallAndSocketConstraintData();
	bs->setInWorldSpace(m_fixedBody->getTransform(), m_dynamicBody->getTransform(), m_fixedBody->getPosition());

	m_constraintData = bs;
	m_constraint = new hkpConstraintInstance(m_fixedBody, m_dynamicBody, bs);

	m_world->unlock();
}


hkDemo::Result AddRemoveConstraintsDemo::stepDemo()
{
	m_world->lock();

	const hkVector4& dynamicPos = m_dynamicBody->getPosition();
	
	// If not yet added and (moved) below Y-plane, add constraint
	if ( !m_constraintAdded && (dynamicPos(1) < 0.0f) )
	{
		addConstraint();		
	}

	// If added and (moved) above Y-plane, remove constraint
	if ( m_constraintAdded && (dynamicPos(1) > 0.0f) )
	{
		removeConstraint();		
	}

	m_world->unlock();

	return hkDefaultPhysicsDemo::stepDemo();
}

AddRemoveConstraintsDemo::~AddRemoveConstraintsDemo()
{	
	m_world->lock();
	m_constraint->removeReference();
	m_constraintData->removeReference();
	m_world->unlock();
}


void AddRemoveConstraintsDemo::createBodies()
{	
	const hkVector4 halfSize(0.5f, 0.5f, 0.5f);

	// Create Box Shape
	hkpBoxShape* boxShape;
	{
		boxShape = new hkpBoxShape( halfSize , 0 );
	}

	// Create fixed rigid body
	{
		hkpRigidBodyCinfo info;
		info.m_position.set(0.0f, 0.0f, 0.0f);
		info.m_shape = boxShape;
		info.m_motionType = hkpMotion::MOTION_FIXED;
		m_fixedBody = new hkpRigidBody(info);
		m_world->addEntity(m_fixedBody);
		m_fixedBody->removeReference();	
		boxShape->removeReference();
	}

	// Create movable rigid body
	{

		hkpRigidBodyCinfo info;
		info.m_position.set(-6.0f, 10.0f, 0.0f);
		info.m_shape = boxShape;
		
		// Compute the box inertia tensor
		hkpMassProperties massProperties;
		info.m_mass = 10.0f;
		hkpInertiaTensorComputer::computeBoxVolumeMassProperties(halfSize, info.m_mass, massProperties);
		info.m_inertiaTensor = massProperties.m_inertiaTensor;
		info.m_centerOfMass = massProperties.m_centerOfMass;	
		info.m_motionType = hkpMotion::MOTION_BOX_INERTIA;

		m_dynamicBody = new hkpRigidBody(info);
		m_world->addEntity(m_dynamicBody);
		m_dynamicBody->removeReference();	
	}

}

void AddRemoveConstraintsDemo::addConstraint()
{	
	m_constraintData->setInWorldSpace(m_fixedBody->getTransform(), m_dynamicBody->getTransform(), m_fixedBody->getPosition());
	
	m_world->addConstraint(m_constraint);
	m_constraintAdded = true;
}

void AddRemoveConstraintsDemo::removeConstraint()
{
	m_world->removeConstraint(m_constraint);
	m_constraintAdded = false;
}


#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
"A demo which shows continuous addition/removal of a constraints from a hkpWorld. " \
"There are two bodies, a fixed one and a dynamic one. Every time the dynamic one falls " \
"below the Y=0 plane, a constraint is added to the simulation, and every time it moves  " \
"above the plane the same constraint is removed. This also shows how a constraint between the " \
"*same* bodies can be dynamically modified. It is only necessary to reset the pivots/rest length  " \
"etc. of the constraint to ensure that the constraint is once again valid. " \
"In this case there are only two pivots for a ball and socket constraint. It is currently *not* " \
"possible to alter the bodies of a constraint, as this will cause simulation islands to break. ";

HK_DECLARE_DEMO(AddRemoveConstraintsDemo, HK_DEMO_TYPE_PRIME, "Dynamic constraint addition/removal", helpString);

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
