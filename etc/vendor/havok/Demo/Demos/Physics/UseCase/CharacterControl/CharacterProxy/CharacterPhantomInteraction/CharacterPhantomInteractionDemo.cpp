/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>

#include <Demos/Physics/UseCase/CharacterControl/CharacterProxy/CharacterPhantomInteraction/CharacterPhantomInteractionDemo.h>

#include <Common/Base/Monitor/hkMonitorStream.h>

#include <Physics/Utilities/CharacterControl/CharacterProxy/hkpCharacterProxy.h>
#include <Physics/Utilities/CharacterControl/StateMachine/hkpDefaultCharacterStates.h>

#include <Physics/Collide/Shape/Convex/Capsule/hkpCapsuleShape.h>
#include <Physics/Collide/Query/Collector/PointCollector/hkpClosestCdPointCollector.h>
#include <Physics/Collide/Query/Collector/PointCollector/hkpAllCdPointCollector.h>
#include <Physics/Collide/Query/CastUtil/hkpLinearCastInput.h>
#include <Physics/Collide/Shape/Compound/Collection/SimpleMesh/hkpSimpleMeshShape.h>
#include <Physics/Collide/Shape/Compound/Tree/Mopp/hkpMoppBvTreeShape.h>
#include <Physics/Collide/Shape/Compound/Tree/Mopp/hkpMoppUtility.h>
#include <Physics/Collide/Agent/hkpProcessCollisionOutput.h>
#include <Physics/Collide/Agent/hkpProcessCollisionInput.h>

#include <Physics/Dynamics/Phantom/hkpSimpleShapePhantom.h>
#include <Physics/Dynamics/Phantom/hkpAabbPhantom.h>

#include <Physics/ConstraintSolver/Simplex/hkpSimplexSolver.h>

#include <Demos/DemoCommon/Utilities/GameUtils/GameUtils.h>
#include <Demos/DemoCommon/Utilities/Character/CharacterUtils.h>
#include <Common/Base/Algorithm/PseudoRandom/hkPseudoRandomGenerator.h>

//#include <hkdemoframework/hkDemoFramework.h>

#include <Demos/DemoCommon/DemoFramework/hkTextDisplay.h>

#include <Graphics/Common/Input/Pad/hkgPad.h>
#include <Graphics/Common/Window/hkgWindow.h>

#include <Common/Visualize/hkDebugDisplay.h>

#define HK_OBJECT_IS_TRIGGER 7777

#define KEEP_DISTANCE 0.1f

static hkBool g_justHitTrigger;

class MyTiggerIdentifyingSimpleShapePhantom: public hkpSimpleShapePhantom
{

public:
	MyTiggerIdentifyingSimpleShapePhantom(const hkpShape* shape, const hkTransform& transform, hkUint32 m_collisionFilterInfo = 0)
		: hkpSimpleShapePhantom(shape, transform, m_collisionFilterInfo), m_hitCount(0)
	{
	}

	
		// Implement proper response to overlapping collidables: allow any collidable with a shape
	void addOverlappingCollidable( hkpCollidable* collidable)
	{
			// Pick out collidables which have been tagged as triggers.
			// Currently the only collidables you should get are those owned by a
			// hkpWorldObject (ie a phantom or entity), but its owner is a void* to avoid dependencies
			// of hkcollide2 library on hkdynamics2 library. So to safeguard the against the addition of 
			// "user-owned" collidables, we check the type here:				
		if((collidable->getType() == hkpRigidBody::BROAD_PHASE_ENTITY) 
			|| (collidable->getType() == hkpRigidBody::BROAD_PHASE_PHANTOM)) 
		{
			hkpWorldObject* wo = hkGetWorldObject (collidable);
			if(wo->hasProperty(HK_OBJECT_IS_TRIGGER))
			{
				hkcout << "\nJust Hit Trigger " << m_hitCount << ".";
				g_justHitTrigger = true;
				m_hitCount++;
				return;
			}
		}

	
			// This is the normal handling code - anything which has a shape gets passed on to 
			// the "collisionDetails" list used by the character controller for collision 
			// processing.
		if ( collidable->getShape() != HK_NULL )
		{
			hkpCollidableAccept accept = fireCollidableAdded( collidable );

			if ( accept == HK_COLLIDABLE_ACCEPT  )
			{
				CollisionDetail& det = m_collisionDetails.expandOne();
				det.m_collidable = collidable;
			}
		}
	}

private:
	hkUint32 m_hitCount;

};

CharacterPhantomInteractionDemo::CharacterPhantomInteractionDemo(hkDemoEnvironment* env)
:	hkDefaultPhysicsDemo(env, DEMO_FLAGS_NO_SERIALIZE)
{

	g_justHitTrigger = false;

	m_characterVelocity = 25.0f;

	m_wallDistance = 25.0f;	// Total coincidence that this is the same as velocity above!

	m_env = env;

	//
	// Setup the camera
	//
	{
		hkVector4 from(  0.0f, 20.0f, -80.0f);
		hkVector4 to  (  0.0f,  0.0f,   0.0f);
		hkVector4 up  (  0.0f,  1.0f,   0.0f);
		setupDefaultCameras( env, from, to, up );

		// disable back face culling
		setGraphicsState(HKG_ENABLED_CULLFACE, false);
	}

	//
	// Create the world
	//
	{
		hkpWorldCinfo info;
		info.setBroadPhaseWorldSize( 350.0f );  
		info.m_gravity.set(0,0,-9.8f);
		info.m_collisionTolerance = 0.1f;		
		m_world = new hkpWorld( info );
		m_world->lock();

		hkpAgentRegisterUtil::registerAllAgents(m_world->getCollisionDispatcher());

		setupGraphics();
	}

	
	////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////
	//////////////////////////// Add a 'floor' /////////////////////////
	////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////
	{
		hkpMoppBvTreeShape* moppShape = getTiledPlanarMoppBvTreeShape(60, 60.0f);
		
		///////////////////////////////////////////////////////


		// To illustrate using the shape, create a rigid body by first defining a template.
		hkpRigidBodyCinfo rigidBodyInfo;

		rigidBodyInfo.m_shape = moppShape;
		rigidBodyInfo.m_position.set(0.0f, 0.0f, 0.0f);
		rigidBodyInfo.m_angularDamping = 0.0f;
		rigidBodyInfo.m_linearDamping = 0.0f;

		// If we set this to true, the body is fixed, and no mass properties need to be computed.
		rigidBodyInfo.m_motionType = hkpMotion::MOTION_FIXED;
			
		 
		// Create a rigid body (using the template above).
		hkpRigidBody* rigidBody = new hkpRigidBody(rigidBodyInfo);

		// Remove references since the body now "owns" the Shapes.
		moppShape->removeReference();

		// Finally add body so we can see it, and remove reference since the world now "owns" it.
		m_world->addEntity(rigidBody);
		rigidBody->removeReference();

	}

	////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////
	//////////////////////////// Add a 'wall' //////////////////////////
	////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////
	{
		hkpMoppBvTreeShape* moppShape = getTiledPlanarMoppBvTreeShape(3, 10.0f);
		
		///////////////////////////////////////////////////////


		// To illustrate using the shape, create a rigid body by first defining a template.
		hkpRigidBodyCinfo rigidBodyInfo;

		rigidBodyInfo.m_shape = moppShape;
		rigidBodyInfo.m_position.set(m_wallDistance, 0.0f, 5.0f);
		hkVector4 axis(0,1,0);
		hkQuaternion rot(axis, HK_REAL_PI* 0.5f);
		rigidBodyInfo.m_rotation = rot;
		rigidBodyInfo.m_angularDamping = 0.0f;
		rigidBodyInfo.m_linearDamping = 0.0f;

		// If we set this to true, the body is fixed, and no mass properties need to be computed.
		rigidBodyInfo.m_motionType = hkpMotion::MOTION_FIXED;
			
		 
		// Create a rigid body (using the template above).
		hkpRigidBody* rigidBody = new hkpRigidBody(rigidBodyInfo);

		// Remove references since the body now "owns" the Shapes.
		moppShape->removeReference();

		// Finally add body so we can see it, and remove reference since the world now "owns" it.
		m_world->addEntity(rigidBody);
		rigidBody->removeReference();

		// Color the wall so we can see through it
		HK_SET_OBJECT_COLOR(static_cast<int>((hkUlong)rigidBody->getCollidable()), hkColor::rgbFromChars(255, 255, 255));

	}
	
	


	////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////
	/////// Add some rigid bodies and mark them as 'triggers' //////////
	////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////
	{

		hkpRigidBodyCinfo rci;
		hkVector4 axis(0,1,0);
		hkQuaternion rot(axis, 0.9f);
		rci.m_rotation = rot;
		rci.m_restitution = 1.0f;

		// Add a rigid body which we use to represent a trigger.
		{ 
			hkVector4 baseSize( 0.5f, 0.5f, 0.5f);
			rci.m_shape = new hkpBoxShape( baseSize, 0.0f );
			rci.m_position.set(10, 0, 7);

			rci.m_motionType = hkpMotion::MOTION_BOX_INERTIA;
			rci.m_centerOfMass.setZero4();
			rci.m_inertiaTensor.setDiagonal(.2f, .2f, .2f);
			rci.m_mass = 10.0f;

			hkpRigidBody* trigger = new hkpRigidBody(rci);
			rci.m_shape->removeReference();
			m_world->addEntity(trigger);

				// Now just set a user property on this object, and change its color to make it easy to see
			triggerifyWorldObject(trigger, hkColor::rgbFromChars(255, 0, 0));
			trigger->removeReference();
		} 
		

		// Add a rigid body which we use to represent a trigger.
		{ 
			hkVector4 baseSize( 0.5f, 0.5f, 2.5f);
			rci.m_shape = new hkpBoxShape( baseSize, 0.0f );
			rci.m_position.set(-10, 0, 3.5);
			
			rci.m_motionType = hkpMotion::MOTION_FIXED;
			hkpRigidBody* trigger = new hkpRigidBody(rci);
			rci.m_shape->removeReference();
			m_world->addEntity(trigger);

				// Now just set a user property on this boyo, and change its color to make it easy to see
			triggerifyWorldObject(trigger, hkColor::rgbFromChars(255, 0, 0));
			trigger->removeReference();
		} 
		

		// Add a rigid body which we use to represent a trigger.
		{ 
			hkVector4 baseSize( 1.0f, 0.5f, 5.0f);
			rci.m_shape = new hkpBoxShape( baseSize, 0.0f );

				// We calculate how far 'behind' the wall we can put the trigger such that the character controller
				// will still be able to 'hit' it, even though it's on the other side!
				// This will happen when the character does a linear cast forward through the wall.
			const hkReal distanceAtWhichWallAndTriggerAreSeparatedByExactlyTolerance = m_wallDistance + baseSize(0) + m_world->getCollisionInput()->getTolerance(); 
			const hkReal distCanMoveInOneFrame = m_characterVelocity * m_timestep;
			const hkReal extraCastLength = KEEP_DISTANCE + 0.1f;	// See hkpCharacterProxy::integrate
			const hkReal slightlyLessThanAllThat = distanceAtWhichWallAndTriggerAreSeparatedByExactlyTolerance + (distCanMoveInOneFrame + extraCastLength ) * 0.9f;

			rci.m_position.set(slightlyLessThanAllThat, 0, 5);
			rci.m_rotation.setIdentity();

			rci.m_motionType = hkpMotion::MOTION_FIXED;
		
			hkpRigidBody* trigger = new hkpRigidBody(rci);
			rci.m_shape->removeReference();
			m_world->addEntity(trigger);

				// Now just set a user property on this boyo, and change its color to make it easy to see
			triggerifyWorldObject(trigger, hkColor::rgbFromChars(255, 0, 0));
			trigger->removeReference();
		} 
	}


	
	////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////
	////////// Add some phantoms and mark them as 'triggers' ///////////
	////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////
	{
		// Add a phantom which we use to represent a trigger.
		{ 
			hkVector4 baseSize( 0.5f, 0.5f, 0.5f);
			hkVector4 position(0, -10, 3);
			hkAabb aabb;
			aabb.m_min.setSub4(position, baseSize);
			aabb.m_max.setAdd4(position, baseSize);

			hkpAabbPhantom* trigger = new hkpAabbPhantom(aabb);
			m_world->addPhantom(trigger);

			triggerifyWorldObject(trigger, 0);
			trigger->removeReference();
		} 
		

		// Add a phantom which we use to represent a trigger.
		{ 
			hkVector4 baseSize( 0.5f, 1.5f, 2.5f);
			hkVector4 position(0, 10, 3);
			hkAabb aabb;
			aabb.m_min.setSub4(position, baseSize);
			aabb.m_max.setAdd4(position, baseSize);

			hkpAabbPhantom* trigger = new hkpAabbPhantom(aabb);
			m_world->addPhantom(trigger);

			triggerifyWorldObject(trigger, 0);
			trigger->removeReference();	
		} 	
	}
	


	//
	//	Create a character proxy object
	//
	{
		// Construct a shape

		hkVector4 vertexA(0,0, .4f);
		hkVector4 vertexB(0,0,-.4f);		

		// Create a capsule to represent the character standing
		m_standShape = new hkpCapsuleShape(vertexA, vertexB, .6f);

		// Create a capsule to represent the character crouching
		vertexA.setZero4();
		m_crouchShape = new hkpCapsuleShape(vertexA, vertexB, .6f);


		// Construct a Shape Phantom
		hkTransform tx; tx.setIdentity();
		m_phantom = new MyTiggerIdentifyingSimpleShapePhantom( m_standShape, tx, hkpGroupFilter::calcFilterInfo(0,2) );
		
		// Add the phantom to the world
		m_world->addPhantom(m_phantom);
		m_phantom->removeReference();

		// Construct a character proxy
		hkpCharacterProxyCinfo cpci;
		cpci.m_position.set(0,0,5);
		cpci.m_staticFriction = 0.0f;
		cpci.m_dynamicFriction = 1.0f;
		cpci.m_up.setNeg4( m_world->getGravity() );
		cpci.m_up.normalize3();	
		cpci.m_keepDistance = KEEP_DISTANCE;
		cpci.m_userPlanes = 4;

		cpci.m_shapePhantom = m_phantom;
		m_characterProxy = new hkpCharacterProxy( cpci );
	}
	

	//
	// Create the Character state machine and context
	//
	{
		hkpCharacterState* state;
		hkpCharacterStateManager* manager = new hkpCharacterStateManager();

		hkpCharacterStateOnGround* gstate = new hkpCharacterStateOnGround();
		gstate->setSpeed(m_characterVelocity);
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
		manager->removeReference();
	}
	
	// Current camera angle about up
	m_currentAngle = 0.4f;
			// Snap mouse back to middle
	m_env->m_window->setMousePosition(m_env->m_window->getWidth() >> 1, m_env->m_window->getHeight() >> 1);


	m_strings.setSize(15);
	for(int i = 0; i < m_strings.getSize() - 1; i++)
	{
		m_strings[i] = hkString(".");
	}

	m_frameNumber = 0;

	m_world->unlock();
}

CharacterPhantomInteractionDemo::~CharacterPhantomInteractionDemo()
{
	m_world->lock();

	m_world->removePhantom(m_characterProxy->getShapePhantom());

	m_characterProxy->removeReference();

	m_standShape->removeReference();

	m_crouchShape->removeReference();

	m_world->unlock();

	delete m_characterContext;
}

/////////////////////////////////////////////////////////////////////

hkDemo::Result CharacterPhantomInteractionDemo::stepDemo()
{
	hkVector4 up;
	hkQuaternion orient;

	{
		m_world->lock();

		m_frameNumber++;

		//	Get user input data
		int m_upAxisIndex = 2;
		up.setZero4();
		up(m_upAxisIndex) = 1;

		hkReal posX = 0.f;
		hkReal posY = 0.f;
		{
			float deltaAngle = 0.f;
			CharacterUtils::getUserInputForCharacter(m_env, deltaAngle, posX, posY);
			m_currentAngle += deltaAngle;
			orient.setAxisAngle(up, m_currentAngle);
		}

		hkpCharacterInput input;
		hkpCharacterOutput output;
		{
			input.m_inputLR = posX;
			input.m_inputUD = posY;

			input.m_wantJump = m_env->m_window->getMouse().wasButtonPressed(HKG_MOUSE_LEFT_BUTTON)
				|| m_env->m_gamePad->wasButtonPressed(HKG_PAD_BUTTON_1);
			input.m_atLadder = false;

			input.m_up = up;
			input.m_forward.set(1,0,0);
			input.m_forward.setRotatedDir( orient, input.m_forward );

			input.m_stepInfo.m_deltaTime = m_timestep;
			input.m_stepInfo.m_invDeltaTime = 1.0f / m_timestep;
			input.m_characterGravity.set(0,0,-20);
			input.m_velocity = m_characterProxy->getLinearVelocity();
			input.m_position = m_characterProxy->getPosition();

			hkVector4 down;	down.setNeg4(up);
			hkpSurfaceInfo ground;
			m_characterProxy->checkSupport(down, ground);

			input.m_isSupported = ground.m_supportedState == hkpSurfaceInfo::SUPPORTED;
			input.m_surfaceNormal = ground.m_surfaceNormal;
			input.m_surfaceVelocity = ground.m_surfaceVelocity;

		}


		// Apply the character state machine
		{
			HK_TIMER_BEGIN( "update character state", HK_NULL );

			m_characterContext->update(input, output);

			HK_TIMER_END();
		}

		hkStepInfo si;
		si.m_deltaTime = m_timestep;
		si.m_invDeltaTime = 1.0f / m_timestep;

		//Apply the player character controller
		{
			HK_TIMER_BEGIN( "simulate character", HK_NULL );

			// Feed output from state machine into character proxy
			m_characterProxy->setLinearVelocity(output.m_velocity);

			//	hkpAllCdPointCollector castCollector;
			//	hkpAllCdPointCollector startPointCollector;
			m_characterProxy->integrate( si, m_world->getGravity() );//, castCollector, startPointCollector  );

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
			hkString::snprintf(buffer, 255, "State : %s", stateStr);
			m_env->m_textDisplay->outputText(buffer, 20, 100, 0xffffffff);



			int i;
			// Cycle
			if(m_frameNumber % 3 == 0)
			{
				for(i = 0; i < m_strings.getSize() - 1; i++)
				{
					m_strings[i] = m_strings[i+1];
				}
				m_strings[i] = ".";
			}

			if(g_justHitTrigger)
			{
				m_strings[m_strings.getSize() - 1] = "Just Hit Trigger";
			}

			g_justHitTrigger = false;

			for(i = 0; i < m_strings.getSize(); i++)
			{
				m_env->m_textDisplay->outputText(m_strings[i], 20, 120 + i *15, 0xffffffff);
			}
		}

		//
		// Handle crouching
		//
		{
			hkBool wantCrouch = (m_env->m_window->getMouse().getButtonState() & HKG_MOUSE_RIGHT_BUTTON)
				|| m_env->m_gamePad->isButtonPressed(HKG_PAD_BUTTON_2);
			hkBool isCrouching = m_phantom->getCollidable()->getShape() == m_crouchShape;

			// We want to stand
			if (isCrouching && !wantCrouch)
			{
				// Attempt to stand
				swapPhantomShape(m_standShape);
			}

			// We want to crouch
			if (!isCrouching && wantCrouch)
			{
				// Attempt to crouch
				swapPhantomShape(m_crouchShape);
			}
		}

		m_world->unlock();
	}

	// Step the world
	hkDefaultPhysicsDemo::stepDemo();

	{
		m_world->lock();

		// Camera Handling
		{
			const hkReal height = 1.7f;	
			hkVector4 forward;
			forward.set(1,0,0);
			forward.setRotatedDir( orient, forward );

			hkVector4 from, to;
			to = m_characterProxy->getPosition();
			to.addMul4(height, up);

			hkVector4 dir;
			dir.setMul4( height, up );
			dir.addMul4( -10.f, forward);

			from.setAdd4(to, dir);
			setupDefaultCameras(m_env, from, to, up, 1.0f);
		}

		m_world->unlock();
	}

	return hkDemo::DEMO_OK;
}


void CharacterPhantomInteractionDemo::triggerifyWorldObject(hkpWorldObject* wo, int color)
{
		// Add a property so we can identify this as a trigger
	hkpPropertyValue val(1);
	wo->addProperty(HK_OBJECT_IS_TRIGGER, val);


	// Color the trigger so we can see it clearly
	HK_SET_OBJECT_COLOR(static_cast<int>((hkUlong)wo->getCollidable()), color);
}

void CharacterPhantomInteractionDemo::swapPhantomShape( hkpShape* newShape )
{
	// Remember the current shape
	hkpShape* currentShape = const_cast<hkpShape*>(m_phantom->getCollidable()->getShape());

	// Swap to the new shape.
	// N.B. To be safe, we always remove the phantom from the world first, then change the shape,
	// then re-add, in order to refresh the cached agents in any hkCachingShapePhantoms which
	// may also be present in the world.
	// This also forces the display to be rebuilt, which is necessary for us to see the new shape!
	{
		// Note we do not have to add a reference before removing becasue we hold a hkpCharacterProxy
		// which has a reference to this phantom - hence removal from the world cannot cause this phantom to 
		// be accidentally deleted.
		m_world->removePhantom( m_phantom );
		m_phantom->setShape(newShape);
		m_world->addPhantom( m_phantom );
	}

	//
	// We use getClosestPoints to check for penetration
	//
	hkpClosestCdPointCollector collector;
	m_phantom->getClosestPoints( collector );

	// Allow a very slight tolerance (approx 1cm)

	if (collector.hasHit() && collector.getHit().m_contact.getDistance() < .01f)
	{
		// Switch the phantom back to our current shape.
		// N.B. To be safe, we always remove the phantom from the world first, then change the shape,
		// then re-add, in order to refresh the cached agents in any hkCachingShapePhantoms which
		// may also be present in the world.
		// This also forces the display to be rebuilt, which is necessary for us to see the new shape!	
		{
			// Note we do not have to add a reference before removing becasue we hold a hkpCharacterProxy
			// which has a reference to this phantom - hence removal from the world cannot cause this phantom to 
			// be accidentally deleted.
			m_world->removePhantom( m_phantom );
			m_phantom->setShape( currentShape );
			m_world->addPhantom( m_phantom );
		}
	}
}





hkpMoppBvTreeShape* CharacterPhantomInteractionDemo::getTiledPlanarMoppBvTreeShape(int sideDivisions, hkReal sideLength)
{
	sideDivisions++;
	//
	//	We use a simple mesh in our example, which copies all data.
	//  If you want to share graphics and physics, use the hkpMeshShape instead
	//
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
				t.m_weldingInfo = 0;

				t.m_a = corner; 
				t.m_b = corner + sideDivisions; 
				t.m_c = corner + 1;
				meshShape->m_triangles.pushBack( t );
				
				t.m_a = corner + 1 ; 
				t.m_b = corner + sideDivisions; 
				t.m_c = corner + sideDivisions + 1;
				meshShape->m_triangles.pushBack( t );
				corner++; 
			}
			corner++; 
		}
	}

		// We set the radius to 0.0f ONLY to ease visual calculations.
	meshShape->m_radius = 0.0f;

	hkpMoppCompilerInput mci;
	hkpMoppCode* code = hkpMoppUtility::buildCode( meshShape ,mci);

	hkpMoppBvTreeShape* moppShape = new hkpMoppBvTreeShape(meshShape, code);
	code->removeReference();
	meshShape->removeReference();

	return moppShape;
}


#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
"This is a test scene for one technique to implement 'phantom' trigger objects " \
"for the Havok Character Controller : Using the broadphase overlap callback addOverlappingCollidable().\n" \
"Rigid bodies which have been 'tagged' with a 'trigger' property are coloured red.\n" \
"Phantoms which have been 'tagged' with a 'trigger' property are yellow.\n" \
"We display some text for one frame when the triggers are 'hit. Please read class description for more details.'\n";

HK_DECLARE_DEMO(CharacterPhantomInteractionDemo, HK_DEMO_TYPE_PRIME, "Character-Phantom Interaction", helpString);

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
