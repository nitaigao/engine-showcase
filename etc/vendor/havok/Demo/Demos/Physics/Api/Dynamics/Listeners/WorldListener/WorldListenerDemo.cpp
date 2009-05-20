/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/Physics/Api/Dynamics/Listeners/WorldListener/WorldListenerDemo.h>


// We need to create a constraint
#include <Physics/Dynamics/Constraint/Bilateral/BallAndSocket/hkpBallAndSocketConstraintData.h>

// We need to create an action
#include <Demos/Physics/Api/Dynamics/Listeners/WorldListener/HoverExampleAction.h>



// Called when the hkpWorld is deleted.
void MyWorldDeletionListener::worldDeletedCallback( hkpWorld* world)
{
	//hkcout << "WORLD DELETED!\n";
}

// Called when an action is added to the world.
void MyWorldActionListener::actionAddedCallback( hkpAction* action )
{
	//hkcout << "ACTION ADDED!\n";
}

// Called when an action is removed from the world.
void MyWorldActionListener::actionRemovedCallback( hkpAction* action )
{
	//hkcout << "ACTION REMOVED!\n";
}

	// Called when an entity is added to the world.
void MyWorldEntityListener::entityAddedCallback( hkpEntity* entity )
{
	//hkcout << "ENTITY ADDED!\n";
}

	// Called when an entity is removed from the world.
void MyWorldEntityListener::entityRemovedCallback( hkpEntity* entity )
{
	//hkcout << "ENTITY REMOVED!\n";
}
	
	// Called when a constraint is added to the world.
void MyWorldConstraintListener::constraintAddedCallback( hkpConstraintInstance* constraint )
{
	//hkcout << "CONSTRAINT ADDED!\n";
}

	// Called when a constraint is removed from the world.
void MyWorldConstraintListener::constraintRemovedCallback( hkpConstraintInstance* constraint )
{
	hkcout << "CONSTRAINT REMOVED!\n";
}

	// Called at the end of the hkpWorld::simulate call, note this might change
void MyWorldPostSimulationListener::postSimulationCallback( hkpWorld* world ) 
{
//	hkcout << "\t\t\tPOST SIM CALLBACK!\n";
}

	// Called after all other events have been called for any given simulation step.
void MyWorldPostCollideListener::postCollideCallback( hkpWorld* world, const hkStepInfo& stepInfo )
{
//	hkcout << "\t\t\tEND OF FRAME CALLBACK!\n";
}




WorldListenerDemo::WorldListenerDemo(hkDemoEnvironment* env)
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
		hkVector4 from(3.0f, 10.0f, 30.0f);
		hkVector4 to  (3.0f,  0.0f,  0.0f);
		hkVector4 up  (0.0f,  1.0f,  0.0f);
		setupDefaultCameras(env, from, to, up);
	}

	// During the creation of our world we take a slightly unusual step with the following line of code:

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

	/// We do this so that the dynamic body keeps moving and will not deactivate itself (remember that the
	/// body is fully dynamic and not keyframed). Usually you would not want to do this for any simulation
	/// with more than a trivial number of objects as the performance penalty for no deactivation
	/// would be significant.

	
	//
	// Create the rigid bodies, one fixed, one dynamic.
	//
	createBodies();	


	/// The next step is to add a constraint and a collection of listeners to the world:


	//
	// Create and add the constraint
	//
	hkpBallAndSocketConstraintData* bs = new hkpBallAndSocketConstraintData();
	bs->setInWorldSpace(m_fixedBody->getTransform(), m_dynamicBody->getTransform(), m_fixedBody->getPosition());
	m_constraintData = bs;

	m_constraint = new hkpConstraintInstance( m_fixedBody, m_dynamicBody, m_constraintData );

	m_worldDeletionListener = new MyWorldDeletionListener();
	m_world->addWorldDeletionListener(m_worldDeletionListener);

	m_worldActionListener = new MyWorldActionListener();
	m_world->addActionListener(m_worldActionListener);

	m_worldEntityListener = new MyWorldEntityListener();
	m_world->addEntityListener(m_worldEntityListener);

	m_worldConstraintListener = new MyWorldConstraintListener();
	m_world->addConstraintListener(m_worldConstraintListener);

	m_worldPostIntegrationListener = new MyWorldPostSimulationListener();
	m_world->addWorldPostSimulationListener(m_worldPostIntegrationListener);

	m_worldPostCollideListener = new MyWorldPostCollideListener();
	m_world->addWorldPostCollideListener(m_worldPostCollideListener);

	/// These are simply used to echo text to the output dialogue to indicate when the various events
	/// have occurred. Finally we create a new HoverExmapleAction and add it to the world:

	m_hoverExampleAction = new HoverExampleAction(m_dynamicBody, m_world->getGravity(), 9.0f);
	addAction();

	m_world->unlock();
}


// During the stepDemo(...) method we decide whether or not a constraint should be added or removed. This decision
// is based on the positional information for the two bodies. The code is given below:

hkDemo::Result WorldListenerDemo::stepDemo()
{
	const hkVector4& dynamicPos = m_dynamicBody->getPosition();
	
	if( m_actionAdded && (dynamicPos(0) > -7.0f))
	{
		removeAction();
	}
		// If not yet added constraint and (moved) below Y-plane, add constraint
	if ( !m_constraintAdded && (dynamicPos(0) < 0) && (dynamicPos(1) < 0) )
	{
		addConstraint();		
	}

		// If added and (moved) too far to the right
	if ( m_constraintAdded && (dynamicPos(0) > 5.8f) )
	{
		removeConstraint();		
	}

		// If fallen too far below
	if ( dynamicPos(1) < -10.0f )
	{
		m_world->lock();
		m_world->removeEntity(m_dynamicBody);
		m_world->unlock();

		hkVector4 startPos(-12.0f, 10.0f, 0.0f);
		m_dynamicBody->setPosition(startPos);

		hkVector4 startVel(0.0f, 0.0f, 0.0f);
		m_dynamicBody->setLinearVelocity(startVel);

		m_world->lock();
		m_world->addEntity(m_dynamicBody);
		m_world->unlock();
		
		addAction();
	}

	/// As you can see the criteria for adding or removing the constraint are simply based on the distance
	/// between the two boxes, with an additional 'reset' condition if the dynamic box falls 10 metres
	/// below the y plane.
	///
	/// We also display 'debug' lines to aid with visualisation of the constraint, and as usual to do so
	/// we make use of a macro defined in "hkDebugDisplay.h" called HK_DISPLAY_LINE.

	// If added, draw line between bodies to make constraint more obvious
	if ( m_constraintAdded )
	{
		hkVector4 start = m_dynamicBody->getPosition();
		hkVector4 end = m_fixedBody->getPosition();
		HK_DISPLAY_LINE(start, end, hkColor::YELLOW);
	}

	return hkDefaultPhysicsDemo::stepDemo();
}

WorldListenerDemo::~WorldListenerDemo()
{	
	m_world->lock();

	m_world->removeWorldDeletionListener(m_worldDeletionListener);
	m_world->removeActionListener(m_worldActionListener);
	m_world->removeEntityListener(m_worldEntityListener);
	m_world->removeConstraintListener(m_worldConstraintListener);
	m_world->removeWorldPostSimulationListener(m_worldPostIntegrationListener);
	m_world->removeWorldPostCollideListener(m_worldPostCollideListener);

	delete m_worldDeletionListener;
	delete m_worldActionListener;
	delete m_worldEntityListener;
	delete m_worldConstraintListener;
	delete m_worldPostIntegrationListener;
	delete m_worldPostCollideListener;
	
	m_hoverExampleAction->removeReference();
	m_constraint->removeReference();
	m_constraintData->removeReference();

	m_world->unlock();
}


// The demo simply consists of two boxes and so naturally use two hkBoxShapes for their physical representation. The only
// difference between the two is that one is fixed and the other is dynamic. To make this distinction we set their
// motion types appropriately as follows:

void WorldListenerDemo::createBodies()
{	
	const hkVector4 halfSize(0.5f, 0.5f, 0.5f);

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
	{
		hkpRigidBodyCinfo info;
		info.m_position.set(0.0f, 0.0f, 0.0f);
		info.m_shape = boxShape;
		///
		info.m_motionType = hkpMotion::MOTION_FIXED;
		///>	
		m_fixedBody = new hkpRigidBody(info);
		m_world->addEntity(m_fixedBody);
		m_fixedBody->removeReference();	
		boxShape->removeReference();
	}


	//
	// Create movable rigid body
	//
	{

		hkpRigidBodyCinfo info;
		info.m_position.set(-12.0f, 10.0f, 0.0f);
		info.m_shape = boxShape;

		///
			// Compute the box inertia tensor
		hkpMassProperties massProperties;
		info.m_mass = 10.0f;
		hkpInertiaTensorComputer::computeBoxVolumeMassProperties(halfSize, info.m_mass, massProperties);
		info.m_motionType = hkpMotion::MOTION_BOX_INERTIA;

		info.m_inertiaTensor = massProperties.m_inertiaTensor;
		info.m_centerOfMass = massProperties.m_centerOfMass;	
		///>

		m_dynamicBody = new hkpRigidBody(info);
		m_world->addEntity(m_dynamicBody);
		m_dynamicBody->removeReference();	

	}

	/// Note that for the dynamic box we must also compute an inertia tensor for the box and so we are required to
	/// specify a mass for the object. Given this information Havok can compute the 'mass properties' for the object,
	/// in turn allowing us to set a position for the center of mass etc..
}

void WorldListenerDemo::addConstraint()
{	
	// update the constraint data (used buy the constraint instance)
	m_constraintData->setInWorldSpace( m_fixedBody->getTransform(), m_dynamicBody->getTransform(), m_fixedBody->getPosition());

	m_world->lock();
	m_world->addConstraint(m_constraint);
	m_world->unlock();
	m_constraintAdded = true;
}

void WorldListenerDemo::removeConstraint()
{
	m_world->lock();
	m_world->removeConstraint(m_constraint);
	m_world->unlock();
	m_constraintAdded = false;
}


void WorldListenerDemo::addAction()
{	
	m_world->lock();
	m_hoverExampleAction->init( m_world->getGravity(), 9.0f);
	m_world->addAction(m_hoverExampleAction);
	m_world->unlock();
	m_actionAdded = true;
}

void WorldListenerDemo::removeAction()
{
	m_world->lock();
	m_world->removeAction(m_hoverExampleAction);
	m_world->unlock();
	m_actionAdded = false;
}

#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
"This demo illustrates how to dynamically add and remove constraints between two bodies during " \
"simulation. A collection of listeners are also added to the world so that we can be notified " \
"of various events that are triggered within the simulation. " \
"The scene consists of two boxes, one of which is dynamic and the other fixed. The dynamic box " \
"slowly moves across the screen until it is within a specified distance from the fixed box at " \
"which point a constraint is added between the two. The constraint is later removed when another " \
"positional based rule is invalidated. ";

HK_DECLARE_DEMO(WorldListenerDemo, HK_DEMO_TYPE_PRIME, "Using 'listeners' to receive callbacks from the world", helpString);

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
