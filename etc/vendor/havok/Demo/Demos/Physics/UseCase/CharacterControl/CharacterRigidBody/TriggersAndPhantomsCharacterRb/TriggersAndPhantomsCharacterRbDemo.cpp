/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>

#include <Demos/Physics/UseCase/CharacterControl/CharacterRigidBody/TriggersAndPhantomsCharacterRb/TriggersAndPhantomsCharacterRbDemo.h>
#include <Physics/Utilities/CharacterControl/CharacterRigidBody/hkpCharacterRigidBody.h>

#include <Physics/Utilities/CharacterControl/StateMachine/hkpDefaultCharacterStates.h>

// Used to generate a convex hull for convexVerticesShape
#include <Common/Internal/ConvexHull/hkGeometryUtility.h>

// Used for character controller code
#include <Physics/Dynamics/Phantom/hkpSimpleShapePhantom.h>
#include <Physics/Collide/Shape/Convex/Capsule/hkpCapsuleShape.h>
#include <Physics/ConstraintSolver/Simplex/hkpSimplexSolver.h>

#include <Physics/Collide/Query/Collector/PointCollector/hkpClosestCdPointCollector.h>

// Used for graphics and I/O
#include <Demos/DemoCommon/DemoFramework/hkTextDisplay.h>
#include <Demos/DemoCommon/Utilities/GameUtils/GameUtils.h>
#include <Demos/DemoCommon/Utilities/Character/CharacterUtils.h>

#include <Common/Visualize/hkDebugDisplay.h>
#include <Common/Base/Monitor/hkMonitorStream.h>
#include <Common/Base/Container/LocalArray/hkLocalArray.h>

 // Used to create the Mopp 'code' object
#include <Physics/Collide/Shape/Compound/Collection/SimpleMesh/hkpSimpleMeshShape.h>
#include <Physics/Collide/Shape/Compound/Tree/Mopp/hkpMoppUtility.h>

#include <Physics/Dynamics/Collide/hkpCollisionListener.h>

#include <Physics/Internal/Collide/Agent3/Machine/Nn/hkpAgentNnTrack.h>

#define HK_OBJECT_IS_TRIGGER 7777

#define RIGIDBODY_COLOR hkColor::DARKGREEN
#define RIGIDBODY_TRIGGER_COLOR hkColor::LIME

#define PHANTOM_COLOR hkColor::DARKORCHID
#define PHANTOM_TRIGGER_COLOR hkColor::PINK

#define CHARACTER_COLOR hkColor::rgbFromChars(255, 0, 0, 150)
#define CHARACTER_TRIGGER_COLOR hkColor::rgbFromChars(255, 165, 0, 150)

static const hkVector4 UP(0,0,1);


// A utility function to get a list of bodies hit with a trigger property set.  For each trigger object collected the 
// first contact point between that object and the characterRb is also returned, in the hitContacts array.
// This functionality could be achieved via a listener but this version is more SPU friendly, as listeners are not fired on SPUs by default
void getRbTriggerInfo( hkpRigidBody* characterRb, hkArray<hkpRigidBody*>& triggerObjectsHit, hkArray<hkContactPoint>& hitContacts )
{
	hkpLinkedCollidable* coll = characterRb->getLinkedCollidable();
	for ( int i = 0; i < coll->m_collisionEntries.getSize(); ++i )
	{
		hkpRigidBody* rb = hkGetRigidBody( coll->m_collisionEntries[i].m_partner );
		if ( rb != HK_NULL && rb->hasProperty(HK_OBJECT_IS_TRIGGER) ) 
		{
			if ( coll->m_collisionEntries[i].m_agentEntry->m_contactMgr->m_type == hkpContactMgr::TYPE_SIMPLE_CONSTRAINT_CONTACT_MGR )
			{
				hkpSimpleConstraintContactMgr* mgr = (hkpSimpleConstraintContactMgr*)(coll->m_collisionEntries[i].m_agentEntry->m_contactMgr);
				if (mgr->m_contactConstraintData.getNumContactPoints() > 0)
				{
					hkContactPoint* contactPoints = mgr->m_contactConstraintData.m_atom->getContactPoints();

					triggerObjectsHit.pushBack( rb );
					hitContacts.pushBack(contactPoints[0]);
				}
			}
		}
	}
}


class MyPhantomForCharacterRbListener: public hkReferencedObject, public hkpPhantomOverlapListener
{
public:

	HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DEMO );

	MyPhantomForCharacterRbListener()
		: m_overlapped(false), m_overlappedTrigger(false) 
	{
	}

	~MyPhantomForCharacterRbListener()
	{
	}

	// hkpPhantom interface implementation
	virtual void collidableAddedCallback(   const hkpCollidableAddedEvent& event )
	{
		hkpRigidBody* rb = hkGetRigidBody( event.m_collidable );

		if (rb != HK_NULL)
		{
			// detect only character rigid body
			if (rb->getMotionType() == hkpMotion::MOTION_CHARACTER)
			{
				m_overlapped = true;
				
				if ( event.m_phantom->hasProperty(HK_OBJECT_IS_TRIGGER) )
				{
					m_overlappedTrigger = true;
				}							
				
			}
		}
	}
	
	// hkpPhantom interface implementation
	virtual void collidableRemovedCallback( const hkpCollidableRemovedEvent& event )
	{
		hkpRigidBody* rb = hkGetRigidBody(event.m_collidable);

		if (rb != HK_NULL)
		{
			// detect only character rigid body
			if (rb->getMotionType() == hkpMotion::MOTION_CHARACTER)
			{
				m_overlapped = false;

				if ( event.m_phantom->hasProperty(HK_OBJECT_IS_TRIGGER) )
				{
					m_overlappedTrigger = false;
				}								
			}
		}
	}

public:

	hkBool	m_overlapped;
	hkBool	m_overlappedTrigger;

};



TriggersAndPhantomsCharacterRbDemo::TriggersAndPhantomsCharacterRbDemo(hkDemoEnvironment* env)
:	hkDefaultPhysicsDemo(env), m_generator(747)
{
	
	// Create the world
	{
		hkpWorldCinfo info;
		info.setBroadPhaseWorldSize( 500.0f );  
		info.m_gravity.set(0,0,-9.8f);
		info.m_collisionTolerance = 0.01f;		
		info.m_contactPointGeneration = hkpWorldCinfo::CONTACT_POINT_ACCEPT_ALWAYS;
		m_world = new hkpWorld( info );
		m_world->lock();

		hkpAgentRegisterUtil::registerAllAgents(m_world->getCollisionDispatcher());

		setGraphicsState(HKG_ENABLED_CULLFACE, false);

		setupGraphics();
	}

	// Setup collision layers 
	{
		// Replace filter
		hkpGroupFilter* groupFilter = new hkpGroupFilter();

		// We disable collisions between different layers to determine what behaviour we want
		groupFilter->disableCollisionsBetween(LAYER_PHANTOMS, LAYER_PHANTOMS);
		groupFilter->disableCollisionsBetween(LAYER_PHANTOMS, LAYER_RIGIDBODIES);
		groupFilter->disableCollisionsBetween(LAYER_PHANTOMS, LAYER_GROUND);

		m_world->setCollisionFilter( groupFilter, true);
		groupFilter->removeReference();
	}

	
	// Add a floor
	{
		hkVector4 halfExtents(30.0f, 30.0f, 1.0f);
		hkpBoxShape* shape = new hkpBoxShape(halfExtents);
		
		hkpRigidBodyCinfo rigidBodyInfo;

		rigidBodyInfo.m_shape = shape;
		rigidBodyInfo.m_position.set(0.0f, 0.0f, 0.0f);
		rigidBodyInfo.m_angularDamping = 0.0f;
		rigidBodyInfo.m_linearDamping = 0.0f;

		rigidBodyInfo.m_collisionFilterInfo = hkpGroupFilter::calcFilterInfo(LAYER_GROUND,0 );

		// If we set this to true, the body is fixed, and no mass properties need to be computed.
		rigidBodyInfo.m_motionType = hkpMotion::MOTION_FIXED;
			
		 
		// Create a rigid body (using the template above).
		hkpRigidBody* rigidBody = new hkpRigidBody(rigidBodyInfo);

		// Remove references since the body now "owns" the Shapes.
		shape->removeReference();

		// Finally add body so we can see it, and remove reference since the world now "owns" it.
		m_world->addEntity(rigidBody);
		rigidBody->removeReference();

	}

	// Create a character rigid body
	{
	
		// Construct a shape
		hkVector4 vertexA(0,0, .4f);
		hkVector4 vertexB(0,0,-.4f);		

		// Create a capsule to represent the character standing
		m_standShape = new hkpCapsuleShape(vertexA, vertexB, .6f);

		// Construct a character rigid body
		hkpCharacterRigidBodyCinfo info;
		info.m_mass = 100.0f;
		info.m_shape = m_standShape;
		info.m_friction = 0.0f;
		info.m_vdbColor = hkColor::rgbFromChars( 240, 0, 0, 150 );  // transparent red
		info.m_maxSlope = HK_REAL_PI / 3.0f;
		info.m_collisionFilterInfo = hkpGroupFilter::calcFilterInfo( LAYER_CHARACTER,0 );

		info.m_maxForce = 1000.0f;
		info.m_up = UP;
		info.m_position.set(0, 0, 5);


		m_characterRigidBody = new hkpCharacterRigidBody( info );
		m_world->addEntity( m_characterRigidBody->getRigidBody() );
	}
	
	// Create the Character state machine and context
	{
		hkpCharacterState* state;
		hkpCharacterStateManager* manager = new hkpCharacterStateManager();

		hkpCharacterStateOnGround* gstate = new hkpCharacterStateOnGround();
		manager->registerState( gstate,	HK_CHARACTER_ON_GROUND);
		gstate->removeReference();

		state = new hkpCharacterStateInAir();
		manager->registerState( state,	HK_CHARACTER_IN_AIR);
		state->removeReference();

		state = new hkpCharacterStateJumping();
		manager->registerState( state,	HK_CHARACTER_JUMPING);
		state->removeReference();

		state = new hkpCharacterStateClimbing();
		manager->registerState( state,	HK_CHARACTER_CLIMBING);
		state->removeReference();

		m_characterContext = new hkpCharacterContext(manager, HK_CHARACTER_ON_GROUND);
		m_characterContext->setCharacterType(hkpCharacterContext::HK_CHARACTER_RIGIDBODY);
		manager->removeReference();
	}

	// Add interaction rigid bodies and phantoms
	{
		// Add some rigid bodies and mark them as 'triggers'
		for(int i = 0; i < 15; i++)
		{
			hkpRigidBodyCinfo rci;

			rci.m_shape = getRandomShape();

			// Place at random position and random rotation
			rci.m_position = getRandomPositionAwayFromCharacter(15.0f, 15.0f, 7.0f);
			hkVector4 axis(m_generator.getRandRange(-1.0f, 1.0f), m_generator.getRandRange(-1.0f, 1.0f), m_generator.getRandRange(-1.0f, 1.0f));
			axis.normalize3();
			rci.m_rotation.setAxisAngle(axis, m_generator.getRandRange(0.0f, HK_REAL_PI));

			rci.m_motionType = hkpMotion::MOTION_BOX_INERTIA;

			rci.m_linearDamping = 1.0f;	// Don't want them rolling away
			rci.m_angularDamping = 1.0f;

			const hkReal mass = 100.0f;
			hkpMassProperties massProperties;
			hkpInertiaTensorComputer::computeShapeVolumeMassProperties(rci.m_shape, mass, massProperties);
			rci.m_inertiaTensor = massProperties.m_inertiaTensor;
			rci.m_centerOfMass = massProperties.m_centerOfMass;
			rci.m_mass = massProperties.m_mass;	

			rci.m_collisionFilterInfo = hkpGroupFilter::calcFilterInfo( LAYER_RIGIDBODIES,0 );


			hkpRigidBody* trigger = new hkpRigidBody(rci);
			rci.m_shape->removeReference();
			m_world->addEntity(trigger);

			if(m_generator.getRandRange(0.0f, 1.0f) < 0.5f)
			{
				// Set a user property on this object, and change its color to make it easy to see
				triggerifyWorldObject(trigger, RIGIDBODY_TRIGGER_COLOR);
			}
			else
			{
				HK_SET_OBJECT_COLOR(static_cast<int>((hkUlong)trigger->getCollidable()), RIGIDBODY_COLOR );
			}
		
			trigger->removeReference();
		
		}

		// Add some phantoms and mark half of them as 'triggers'
		for(int i = 0; i < 15; i++)
		{
			hkpShape* shape = getRandomShape();

			// Place at random position and random rotation
			hkTransform randomTx; 
			randomTx.setTranslation( getRandomPositionAwayFromCharacter(15.0f, 0.0f, 15.0f));
			hkVector4 axis(m_generator.getRandRange(-1.0f, 1.0f), m_generator.getRandRange(-1.0f, 1.0f), m_generator.getRandRange(-1.0f, 1.0f));
			axis.normalize3();
			hkQuaternion rot(axis, m_generator.getRandRange(0.0f, HK_REAL_PI));
			randomTx.setRotation(rot);
			
			hkpSimpleShapePhantom* trigger = new hkpSimpleShapePhantom( shape, randomTx, hkpGroupFilter::calcFilterInfo( LAYER_PHANTOMS,0 ) );
			m_phantoms.pushBack(trigger);
			shape->removeReference();
			
			// Add phantom overlap listeners
			MyPhantomForCharacterRbListener* pl = new MyPhantomForCharacterRbListener();
			m_phantomListeners.pushBack(pl);
			
			trigger->addPhantomOverlapListener(m_phantomListeners[i]);
			
			m_world->addPhantom(trigger);

			// Some of them set as trigger
			if(m_generator.getRandRange(0.0f, 1.0f) < 0.5f)
			{
				// Now just set a user property on this boyo, and change its color to make it easy to see
				triggerifyWorldObject(trigger, PHANTOM_TRIGGER_COLOR);
			}
			else
			{
				HK_SET_OBJECT_COLOR(static_cast<int>((hkUlong)trigger->getCollidable()), PHANTOM_COLOR );
			}
		
			trigger->removeReference();
		}

	}

	// Current camera angle about up
	m_currentAngle = 0.4f;
	// Snap mouse back to middle
	m_env->m_window->setMousePosition(m_env->m_window->getWidth() >> 1, m_env->m_window->getHeight() >> 1);

	m_world->unlock();
}

void TriggersAndPhantomsCharacterRbDemo::removeAndReaddObject(hkpWorldObject* wo)
{
	wo->addReference();
	hkpRigidBody* body = hkGetRigidBody(wo->getCollidable());
	
	// It's a rigid body or phantom
	if(body)
	{
		//It's a rigid body, first remove from world
		m_world->removeEntity(body);

		// Place at random positon, random rotation
		hkVector4 newpos = getRandomPositionAwayFromCharacter(15.0f, 15.0f, 7.0f);
		body->setPosition(newpos);
		hkVector4 axis(m_generator.getRandRange(-1.0f, 1.0f), m_generator.getRandRange(-1.0f, 1.0f), m_generator.getRandRange(-1.0f, 1.0f));
		axis.normalize3();
		hkQuaternion rot(axis, m_generator.getRandRange(0.0f, HK_REAL_PI));
		body->setRotation(rot);

		body->setAngularVelocity(hkVector4::getZero());
		body->setLinearVelocity(hkVector4::getZero());

		m_world->addEntity(body);

		// Color the rigid body with correspoding color
		HK_SET_OBJECT_COLOR(static_cast<int>((hkUlong)wo->getCollidable()), RIGIDBODY_TRIGGER_COLOR);

	}
	else
	{
		// It's phantom, assume a simple shape phantom 
		hkpSimpleShapePhantom* phantom = static_cast<hkpSimpleShapePhantom*> (hkGetPhantom(wo->getCollidable()));

		// Place at random position, random rotation
		hkVector4 newpos = getRandomPositionAwayFromCharacter(15.0f, 0.0f, 15.0f);
		hkVector4 axis(m_generator.getRandRange(-1.0f, 1.0f), m_generator.getRandRange(-1.0f, 1.0f), m_generator.getRandRange(-1.0f, 1.0f));
		axis.normalize3();
		hkQuaternion rot(axis, m_generator.getRandRange(0.0f, HK_REAL_PI));
		phantom->setPosition(newpos);
		hkTransform t(rot, newpos);
		phantom->setTransform(t);

		m_world->removePhantom(phantom);

		m_world->addPhantom(phantom);

		// Color the phantom with corresponding color
		HK_SET_OBJECT_COLOR(static_cast<int>((hkUlong)wo->getCollidable()), PHANTOM_TRIGGER_COLOR);

	}

	wo->removeReference();

}

TriggersAndPhantomsCharacterRbDemo::~TriggersAndPhantomsCharacterRbDemo()
{
	m_world->lock();

	m_characterRigidBody->removeReference();

	// Remove and delete all phantom listeners
	for (int i=0; i < m_phantoms.getSize(); i++)
	{
		m_phantoms[i]->removePhantomOverlapListener(m_phantomListeners[i]);		
	}
	
	for (int i=0; i < m_phantomListeners.getSize(); i++)
	{
		m_phantomListeners[i]->removeReference();		
	}

	m_standShape->removeReference();
	
	m_world->unlock();

	delete m_characterContext;
}

hkDemo::Result TriggersAndPhantomsCharacterRbDemo::stepDemo()
{
	hkpCharacterInput input;

	{
		m_world->lock();

		//	Get user input data
		hkQuaternion orient;
		
		hkReal posX = 0.f;
		hkReal posY = 0.f;
		{
			float deltaAngle = 0.f;
			CharacterUtils::getUserInputForCharacter(m_env, deltaAngle, posX, posY);
			m_currentAngle += deltaAngle;
			orient.setAxisAngle(UP, m_currentAngle);
		}

		hkpCharacterOutput output;
		{
			input.m_inputLR = posX;
			input.m_inputUD = posY;

			input.m_wantJump = m_env->m_window->getMouse().wasButtonPressed(HKG_MOUSE_LEFT_BUTTON)
				|| m_env->m_gamePad->wasButtonPressed(HKG_PAD_BUTTON_1);
			input.m_atLadder = false;

			input.m_up = UP;
			input.m_forward.set(1,0,0);
			input.m_forward.setRotatedDir( orient, input.m_forward );

			hkStepInfo stepInfo;
			stepInfo.m_deltaTime = m_timestep;
			stepInfo.m_invDeltaTime = 1.0f/m_timestep;
			
			input.m_stepInfo = stepInfo;
			input.m_characterGravity.set(0,0,-16);
			input.m_velocity = m_characterRigidBody->getLinearVelocity();
			input.m_position = m_characterRigidBody->getPosition();

			hkpSurfaceInfo ground;
			m_characterRigidBody->checkSupport(stepInfo,ground);

			input.m_isSupported = (ground.m_supportedState == hkpSurfaceInfo::SUPPORTED);
			input.m_surfaceNormal = ground.m_surfaceNormal;
			input.m_surfaceVelocity = ground.m_surfaceVelocity;

		}

		// Apply the character state machine
		{
			HK_TIMER_BEGIN( "update character state", HK_NULL );

			m_characterContext->update(input, output);

			HK_TIMER_END();
		}

		// Display state
		{
			hkpCharacterStateType state = m_characterContext->getState();
			char * stateStr;

			switch (state)
			{
			case HK_CHARACTER_ON_GROUND:
				stateStr = "On Ground";	break;
			case HK_CHARACTER_JUMPING:
				stateStr = "Jumping"; break;
			case HK_CHARACTER_IN_AIR:
				stateStr = "In Air"; break;
			case HK_CHARACTER_CLIMBING:
				stateStr = "Climbing"; break;
			default:
				stateStr = "Other";	break;
			}
			char buffer[255];
			const int& h = m_env->m_window->getHeight();
			hkString::snprintf(buffer, 255, "State : %s", stateStr);
			m_env->m_textDisplay->outputText(buffer, 20, h-40, 0xffffffff);
		}
		
		//Apply the player character controller
		{
			HK_TIMER_BEGIN( "simulate character", HK_NULL );

			// Feed output velocity from state machine into character rigid body
			m_characterRigidBody->setLinearVelocity(output.m_velocity, m_timestep);

			HK_TIMER_END();
		}

		
		// Here we deal with any 'triggers' we may have encountered.
		{

			// Build list of triggered objects
			hkLocalArray<hkpRigidBody*> triggerObjectsHit(20);
			hkLocalArray<hkContactPoint> hitContacts(20); 
			getRbTriggerInfo( m_characterRigidBody->getRigidBody(), triggerObjectsHit, hitContacts );

			// We'll decide now which objects we'd like to consider as being 'hit'
			
			// We'll assume that any body we picked up using the start collector which has a negative distance 
			// was penetrating us during that integrate  - we need to check these here because they aren't picked
			// up during the linear casts.
			for(int i = 0; i < triggerObjectsHit.getSize(); i++)
			{
				if( hitContacts[i].getDistance() < 0.1f)
				{
					removeAndReaddObject( triggerObjectsHit[i]);
				}
			}

			hkBool inPhantom = false;
			
			// Loop over all phantoms listeners (each phantom has its own listener)
			for(int i=0; i<m_phantomListeners.getSize();i++)
			{
				// Check status of each listener
				if ( m_phantomListeners[i]->m_overlapped )
				{
					inPhantom = true;

					if (m_phantomListeners[i]->m_overlappedTrigger)
					{
						// Get closest point					
						hkpClosestCdPointCollector collector;
						m_phantoms[i]->getClosestPoints( collector );

						if (collector.hasHit())
						{ 
							if (collector.getHit().m_contact.getDistance() < 0.1f )
							{
								removeAndReaddObject(m_phantoms[i]);
							}
								
						}
					}
				}

			}
			
			// Recolor character to orange if object is overlaped with phantom
			hkUint32 color = (inPhantom) ? CHARACTER_TRIGGER_COLOR : CHARACTER_COLOR;
			HK_SET_OBJECT_COLOR(hkUlong(m_characterRigidBody->getRigidBody()->getCollidable()), color);
			
	
		}

		m_world->unlock();
	}

	// Step world
	{
		hkDefaultPhysicsDemo::stepDemo();
	}
	
	{
		m_world->lock();

		// Camera Handling
		{
			const hkReal height = 1.7f;

			hkVector4 from, to;
			to = m_characterRigidBody->getRigidBody()->getPosition();
			to.addMul4(height, UP);

			hkVector4 dir;
			dir.setMul4( height, UP );
			dir.addMul4( -8.f, input.m_forward);

			from.setAdd4(to, dir);
			setupDefaultCameras(m_env, from, to, UP, 1.0f);
		}

		m_world->unlock();
	}

	return hkDemo::DEMO_OK;
}


void TriggersAndPhantomsCharacterRbDemo::triggerifyWorldObject(hkpWorldObject* wo, int color)
{
	// Add a property so we can identify this as a trigger
	hkpPropertyValue val(1);
	wo->addProperty(HK_OBJECT_IS_TRIGGER, val);

	// Color the trigger so we can see it corresponding color
	HK_SET_OBJECT_COLOR(static_cast<int>((hkUlong)wo->getCollidable()), color);
}

hkpMoppBvTreeShape* TriggersAndPhantomsCharacterRbDemo::getTiledPlanarMoppBvTreeShape(int sideDivisions, hkReal sideLength, hkReal radius)
{
	sideDivisions++;
	
	//	We use a simple mesh in our example, which copies all data.
	//  If you want to share graphics and physics, use the hkpMeshShape instead
	hkpSimpleMeshShape* meshShape = new hkpSimpleMeshShape;

	{
		for(int i = 0; i < sideDivisions; i++)
		{
			for (int j = 0; j < sideDivisions; j++ )
			{
				hkVector4 vertex ( i * 1.0f - (sideDivisions-1) * 0.5f, j * 1.0f - (sideDivisions-1) * 0.5f, 0.0f );
				vertex.mul4(sideLength/(sideDivisions-1));
				meshShape->m_vertices.pushBack( vertex );
			}
		}
	}

	{
		int corner = 0;
		for(int i = 0; i < sideDivisions - 1; i++)
		{
			for (int j = 0; j < sideDivisions - 1; j++ )
			{
				hkpSimpleMeshShape::Triangle t;
				t.m_a = corner; t.m_b = corner + 1; t.m_c = corner + sideDivisions; t.m_weldingInfo = 0;
				meshShape->m_triangles.pushBack( t );
				t.m_a = corner+1; t.m_b = corner + sideDivisions; t.m_c = corner + sideDivisions + 1;
				meshShape->m_triangles.pushBack( t );
				corner++; 
			}
			corner++; 
		}
	}

	meshShape->m_radius = radius;

	hkpMoppCompilerInput mci;
	hkpMoppCode* code = hkpMoppUtility::buildCode( meshShape ,mci);

	hkpMoppBvTreeShape* moppShape = new hkpMoppBvTreeShape(meshShape, code);
	code->removeReference();
	meshShape->removeReference();

	return moppShape;
}

hkpShape* TriggersAndPhantomsCharacterRbDemo::getRandomShape()
{
	int shapeType = (int) (m_generator.getRandRange(0,1) * 6);
	switch(shapeType)
	{
		// Create MOPP body
		case 0:
		{
			//hkpMoppBvTreeShape* shape = getTiledPlanarMoppBvTreeShape(2, 4.0f);
			//return(shape);
		}

		// Create ConvexVertices body
		case 1:
		{
			// Data specific to this shape.
			int numVertices = 4;

				// 16 = 4 (size of "each float group", 3 for x,y,z, 1 for padding) * 4 (size of float)
			int stride = 16;

			float vertices[] = { // 4 vertices plus padding
				-2.0f, 1.0f, 1.0f, 0.0f,  // v0
				 1.0f, 2.0f, 0.0f, 0.0f,  // v1
				 0.0f, 0.0f, 5.0f, 0.0f,  // v2
				 1.0f, -1.0f, 0.0f, 0.0f  // v3
			};
			

			hkpConvexVerticesShape* shape;
			hkArray<hkVector4> planeEquations;
			hkGeometry geom;
			{
				hkStridedVertices stridedVerts;
				{
					stridedVerts.m_numVertices = numVertices;
					stridedVerts.m_striding = stride;
					stridedVerts.m_vertices = vertices;
				}

				hkGeometryUtility::createConvexGeometry( stridedVerts, geom, planeEquations );

				{
					stridedVerts.m_numVertices = geom.m_vertices.getSize();
					stridedVerts.m_striding = sizeof(hkVector4);
					stridedVerts.m_vertices = &(geom.m_vertices[0](0));
				}

				shape = new hkpConvexVerticesShape(stridedVerts, planeEquations);
			}

			return(shape);
		}

		// Create Box body
		case 2:
		{
			// Data specific to this shape.
			hkVector4 halfExtents = hkVector4(m_generator.getRandRange(0.5f, 2.0f), m_generator.getRandRange(0.5f, 4.0f), m_generator.getRandRange(0.5f, 2.0f));
			hkpBoxShape* shape = new hkpBoxShape(halfExtents );
			return(shape);
		}

		// Create Sphere body
		case 3:
		{
			hkReal radius = m_generator.getRandRange(1.5f, 4.0f);
			hkpConvexShape* shape = new hkpSphereShape(radius);
			return(shape);
		}

		// Create Triangle body
		case 4:
		{
			hkVector4 a(-m_generator.getRandRange(0.5f, 2.0f), -1.5f,  0.0f);
			hkVector4 b(m_generator.getRandRange(0.5f, 2.0f), -1.5f,  0.0f);
			hkVector4 c(0.0f,  3.5f,  0.0f);

			hkpTriangleShape* shape = new hkpTriangleShape(a, b, c);
			shape->setRadius(0.3f);	// Make fat
			return(shape);
		}

		// Create Capsule body
		case 5:
		{
			hkVector4 a(-1.5f, -2.5f,  0.0f);
			hkVector4 b(1.5f, -2.5f,  0.0f);

			hkpCapsuleShape* shape = new hkpCapsuleShape(a, b, 0.7f);
			return(shape);
		}

	}	// End case

	HK_ASSERT2(0x0, 0, "Failed to build shape.");
	return HK_NULL;

}


hkVector4 TriggersAndPhantomsCharacterRbDemo::getRandomPositionAwayFromCharacter(hkReal planerange, hkReal heightrange, hkReal mindist)
{
	hkVector4 pos;
	do
	{
		pos.set(m_generator.getRandRange(-planerange, planerange), m_generator.getRandRange(-planerange, planerange), m_generator.getRandRange(2.0f, heightrange + 2.0f));
		pos.sub4(m_characterRigidBody->getRigidBody()->getPosition());
	}while(pos.length3() < mindist);

	pos.add4(m_characterRigidBody->getRigidBody()->getPosition());
	return pos;
}


#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
" This is a test scene for one technique to implement phantom and rigid body trigger objects " \
" and their various interactions during collisions with the character rigid body controller using the custom listeners. " \
" Normal rigid bodies are coloured dark green. Rigid bodies  which have been 'tagged' with a 'trigger' property are coloured lime. " \
" Normal phantoms are colored dark orchid. Phantoms which have been 'tagged' with a 'trigger' property are pink. " \
" We remove and re-add when the triggers are 'hit'. We also change the color of character to orange when entering the phantom. " \
" Please read class description for more details.\n " \
" Controls:" \
"  Mouse / Analog Stick to turn \n" \
"  \224 / \225 Forward/Back \n" \
"  LMB / \221 to Jump \n";

HK_DECLARE_DEMO(TriggersAndPhantomsCharacterRbDemo, HK_DEMO_TYPE_PRIME, "Character 'picking up' objects marked as triggers.", helpString);

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
