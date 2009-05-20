/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/Physics/UseCase/CharacterControl/CharacterRigidBody/PushedCharacterRbs/PushedCharacterRbsDemo.h>
#include <Physics/Utilities/CharacterControl/CharacterRigidBody/hkpCharacterRigidBody.h>

#include <Physics/Collide/Shape/Convex/Capsule/hkpCapsuleShape.h> 
#include <Physics/ConstraintSolver/Simplex/hkpSimplexSolver.h>
#include <Physics/Utilities/CharacterControl/StateMachine/hkpDefaultCharacterStates.h>

#include <Demos/DemoCommon/Utilities/GameUtils/GameUtils.h>
#include <Demos/DemoCommon/Utilities/Character/CharacterUtils.h>
#include <Demos/DemoCommon/DemoFramework/hkTextDisplay.h>

#include <Common/Visualize/hkDebugDisplay.h>
#include <Common/Base/Monitor/hkMonitorStream.h>
#include <Common/Base/Container/LocalArray/hkLocalArray.h>

// Attached phantom shape includes
#include <Physics/Collide/Shape/Misc/Bv/hkpBvShape.h>
#include <Physics/Collide/Shape/Misc/PhantomCallback/hkpPhantomCallbackShape.h>
#include <Physics/Collide/Shape/Compound/Collection/List/hkpListShape.h>

#define	NORMAL_CHARACTER	hkColor::rgbFromChars( 250, 140, 0, 150 )
#define	DETECTED_CHARACTER	hkColor::rgbFromChars( 199 , 21, 133, 150 ) 

static const hkVector4 UP(0,0,1);

// Simple 'AI' controller calculates key inputs (LR and UD) to ensure movement 
// of the character to the desired position. Two versions implemented. 
// "Power" of character is independent or proportional to a distance to the desired position.
static void goToPointController( const hkVector4& desiredPosition, hkpRigidBody* rigidBody, hkReal& inputLROut, hkReal& inputUDOut )
{
	// gain helps to avoid vibration around desired position
	const hkReal gain = 0.9f; 

	hkVector4 normalWS; 
	normalWS.setSub4( desiredPosition, rigidBody->getPosition()); 
	
	hkReal distance; distance = normalWS.length3();

	// if character retreats from desired position 
	if ( distance > 0.01f)
	{
		normalWS.normalize3();
		
		hkVector4 normalMS; 
		normalMS.setRotatedDir(rigidBody->getRotation(),normalWS); 
		//normalMS.setRotatedInverseDir(rigidBody->getRotation(),normalWS); 
		normalMS.normalize3();
		HK_DISPLAY_ARROW(rigidBody->getPosition(),normalMS,hkColor::YELLOW);

		// 
		// Controllers rangE
		//
		//		  -1	 
		//		  up
		// 1 left    right -1
		//		 down	
		//         1
		//

		// Approach A (distance dependent)
		//inputLROut = gain*distance*normalMS(1); 
		//inputLROut = hkMath::clamp(inputLROut,-1.0f,1.0f);
		
		//inputUDOut = -gain*distance*normalMS(0); 
		//inputUDOut = hkMath::clamp(inputUDOut,-1.0f,1.0f);

		// Approach B (distance independent)
		inputLROut = gain*normalMS(1); 
		inputUDOut = -gain*normalMS(0); 	
		
	}
	else
	{
		inputLROut = 0.0f;
		inputUDOut = 0.0f;
	}

	

}

// Custom implementation of hkpPhantomCallbackShape.
// All centres around our custom implementations of the two pure virtual methods in the base class, 
// namely phantomEnterEvent(...) and phantomLeaveEvent(...). These methods notify us of an entry or exit 
// event for the phantom volume and most importantly provide a reference to the collidable that has penetrated the phantom.
// Both methods simply color the characters. 

class MyAttachedPhantom: public hkpPhantomCallbackShape
{
public:

	MyAttachedPhantom(){ }

	// hkpPhantom interface implementation (Enter Event)
	virtual void phantomEnterEvent( const hkpCollidable* collidableA, const hkpCollidable* collidableB, const hkpCollisionInput& env )
	{
		// the color can only be changed once the entity has been added to the world
		hkpRigidBody* owner = hkGetRigidBody(collidableB);

		// Change the color of the entring character 
		// the "Collidables" here are "faked" so it's necessary to get the owner first in order
		// to get the "real" collidable! Don't change the color to fixed or keyframed!
		if (! owner->isFixedOrKeyframed())
		{
			HK_SET_OBJECT_COLOR((hkUlong)owner->getCollidable(), DETECTED_CHARACTER);
		}
		
	}

	// hkpPhantom interface implementation (Leave Event)
	virtual void phantomLeaveEvent( const hkpCollidable* collidableA, const hkpCollidable* collidableB )
	{
		// the color can only be changed once the entity has been added to the world
		hkpRigidBody* owner = hkGetRigidBody(collidableB);

		// Change the color of the leaving character (to original one)
		// the "Collidables" here are "faked" so it's necessary to get the owner first in order
		// to get the "real" collidable! Don't change the color to fixed or keyframed!
		if (! owner->isFixedOrKeyframed())
		{
			HK_SET_OBJECT_COLOR((hkUlong)owner->getCollidable(), NORMAL_CHARACTER);
		}	

	}

};


PushedCharacterRbsDemo::PushedCharacterRbsDemo(hkDemoEnvironment* env)
:	hkDefaultPhysicsDemo(env)
{
	
	// Create the world
	{
		hkpWorldCinfo info;
		info.setupSolverInfo( hkpWorldCinfo::SOLVER_TYPE_4ITERS_MEDIUM );
		info.setBroadPhaseWorldSize( 350.0f );  
		info.m_gravity.set(0,0,-9.8f);
		info.m_contactPointGeneration = hkpWorldCinfo::CONTACT_POINT_ACCEPT_ALWAYS;
		info.m_collisionTolerance = 0.01f;
		
		m_world = new hkpWorld( info );
		m_world->lock();

		hkpAgentRegisterUtil::registerAllAgents(m_world->getCollisionDispatcher());

		setupGraphics();
	}


	// Create the floor
	{
		hkpShape* box = new hkpBoxShape(hkVector4(20,20,1));

		hkpRigidBodyCinfo ci;
		ci.m_shape = box;
		ci.m_position.set(0,0,-1.0f);
		ci.m_motionType = hkpMotion::MOTION_FIXED;
		ci.m_friction = 0.0f;
		ci.m_collisionFilterInfo = hkpGroupFilter::calcFilterInfo(hkpGroupFilter::calcFilterInfo(0));

		hkpRigidBody* entity = new hkpRigidBody(ci);
		box->removeReference();
		m_world->addEntity(entity);
		entity->removeReference();
	}

	//	Create main controlled character with attached hkPhantomCallback shape	
	{
		
		// Construct a shapes
		hkpShape* shapes[2];

		hkVector4 top(0,0, .4f);
		hkVector4 bottom(0,0,-.4f);		

		// Create a capsule to represent the character standing
		shapes[0] = new hkpCapsuleShape(top, bottom, .6f);

		// Create a triangle to represent attached phantom
		{
			hkVector4 v0( 0.0f, 0.0f, -0.9f );
			hkVector4 v1( 5.0f, -2.5f, -0.9f );
			hkVector4 v2( 5.0f, 2.5f, -0.9f );

			hkpTriangleShape* triangle = new hkpTriangleShape( v0, v1, v2 );
			MyAttachedPhantom* attachedPhantom = new MyAttachedPhantom();
			shapes[1] = new hkpBvShape( triangle, attachedPhantom );

			attachedPhantom->removeReference();
			triangle->removeReference();
		}

		hkpListShape* characterShape = new hkpListShape( shapes, 2 );

		shapes[0]->removeReference();
		shapes[1]->removeReference();

		// Construct a character rigid body
		hkpCharacterRigidBodyCinfo info;
		info.m_mass = 100.0f;
		info.m_shape = characterShape;

		info.m_maxForce = 1000.0f;
		info.m_up = UP;
		info.m_position.set(10.0f,-5.0f,1.0f);
		

		m_characterRigidBody = new hkpCharacterRigidBody( info );
		m_world->addEntity( m_characterRigidBody->getRigidBody() );

		characterShape->removeReference();

	}

	// Create the main character state machine and context
	{
		hkpCharacterState* state;
		hkpCharacterStateManager* manager = new hkpCharacterStateManager();

		state = new hkpCharacterStateOnGround();
		manager->registerState( state,	HK_CHARACTER_ON_GROUND);
		state->removeReference();

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


	// Create characters for interaction 
	
	const int numCharacters = 7;

	m_characterRigidBodies.setSize( numCharacters );
	m_desiredPositions.setSize( numCharacters );
	{
		hkVector4 top(0, 0, 0.4f);
		hkVector4 bottom(0, 0, -0.4f);		

		// Create a capsule to represent the character standing
		hkpShape* characterShape = new hkpCapsuleShape(top, bottom, .6f);

		for (int i=0; i < numCharacters; i++)
		{
			// Construct a character rigid body
			hkpCharacterRigidBodyCinfo info;
			info.m_shape = characterShape;
			info.m_maxForce = 1000.0f;
			info.m_up = UP;
			info.m_vdbColor = NORMAL_CHARACTER;  // transparent red
			

			hkVector4 position;
			
			switch (i)
			{
				case 0:
					// Static separated light character (no gotoPoint controller)
					info.m_position.set(10.0f,0.0f,1.0f);
					info.m_mass = 50.0f;
					info.m_maxForce = 1000.0f;
					break;
				case 1:
					// Static separated normal character
					info.m_position.set(5.0f,0.0f,1.0f);
					info.m_mass = 100.0f;
					info.m_maxForce = 1000.0f;
					break;
				case 2:
					// Static separated heavy character
					info.m_position.set(0.0f,0.0f,1.0f);
					info.m_mass = 200.0f;
					info.m_maxForce = 1000.0f;
					break;
				case 3:
					// Static normal character in group of 3
					info.m_position.set(-5.0f,0.0f,1.0f);
					info.m_mass = 100.0f;
					info.m_maxForce = 1000.0f;
					break;
				case 4:
					// Static normal characters in group of 3
					info.m_position.set(-5.0,1.2f,1.0f);
					info.m_mass = 100.0f;
					info.m_maxForce = 1000.0f;
					break;
				case 5:
					// Static normal characters in group of 3
					info.m_position.set(-5.0,-1.2f,1.0f);
					info.m_mass = 100.0f;
					info.m_maxForce = 1000.0f;
					break;					
				case 6:
					// Dynamic normal characters in group
					info.m_position.set(-10.0,0.0f,1.0f);
					info.m_mass = 100.0f;
					info.m_maxForce = 1000.0f;
					break;
			}


			// Store desired(init) positions
			m_desiredPositions[i] = info.m_position;
			m_desiredPositions[i](2)-=0.9f; // stay 0.1 m above floor - dist
			
			m_characterRigidBodies[i] = new hkpCharacterRigidBody( info );
			m_world->addEntity( m_characterRigidBodies[i]->getRigidBody() );

			// Reset to new color
			m_characterRigidBodies[i]->getRigidBody()->removeProperty(HK_PROPERTY_DEBUG_DISPLAY_COLOR);
			m_characterRigidBodies[i]->getRigidBody()->addProperty(HK_PROPERTY_DEBUG_DISPLAY_COLOR, NORMAL_CHARACTER); 
			
		}

		characterShape->removeReference();
	
		// Create character state machine
		{
			hkpCharacterStateManager* manager;
			manager = new hkpCharacterStateManager();

			hkpCharacterState* state;
			
			state = new hkpCharacterStateOnGround();
			manager->registerState( state,	HK_CHARACTER_ON_GROUND);
			state->removeReference();

			state = new hkpCharacterStateInAir();
			manager->registerState( state,	HK_CHARACTER_IN_AIR);
			state->removeReference();

			state = new hkpCharacterStateJumping();
			manager->registerState( state,	HK_CHARACTER_JUMPING);
			state->removeReference();

			state = new hkpCharacterStateClimbing();
			manager->registerState( state,	HK_CHARACTER_CLIMBING);
			state->removeReference();

			// Create a context for each character
			m_characterContexts.setSize(numCharacters);
			
			for (int i=0; i < numCharacters; i++)
			{
				m_characterContexts[i] = new hkpCharacterContext(manager, HK_CHARACTER_ON_GROUND);
				m_characterContexts[i]->setCharacterType(hkpCharacterContext::HK_CHARACTER_RIGIDBODY);
			}
			manager->removeReference();			
		}
	}
		
	// Current camera angle about up
	m_currentAngle = 0.0f;
	m_env->m_window->setMousePosition(m_env->m_window->getWidth() >> 1, m_env->m_window->getHeight() >> 1);

	// Init actual time
	m_time = 0.0f;

	m_world->unlock();
}

PushedCharacterRbsDemo::~PushedCharacterRbsDemo()
{
	m_world->lock();

	m_characterRigidBody->removeReference();
	m_characterContext->removeReference();

	for (int i=0; i < m_characterRigidBodies.getSize(); i++)
	{
		m_characterRigidBodies[i]->removeReference();

		delete m_characterContexts[i];
	}

	m_world->unlock();	
}


/////////////////////////////////////////////////////////////////////

void PushedCharacterRbsDemo::makeFakeInput()
{
	// go left
	m_env->m_gamePad->forceButtonPressed(HKG_PAD_DPAD_LEFT);

	if ( (m_physicsStepCounter % 50) == 0)
	{
		// jump
		m_env->m_gamePad->forceButtonPressed(HKG_PAD_BUTTON_1);
	}

}

hkDemo::Result PushedCharacterRbsDemo::stepDemo()
{

	// Update actual time
	m_time += m_timestep;
		
	// Do user controlled character
	hkpCharacterInput input;
	hkpCharacterOutput output;
	{
		m_world->lock();

		hkQuaternion orient;
		hkReal posX = 0.f;
		hkReal posY = 0.f;
		{
			hkReal deltaAngle = 0.f;
			CharacterUtils::getUserInputForCharacter(m_env, deltaAngle, posX, posY);
			m_currentAngle += deltaAngle;
			orient.setAxisAngle(UP, m_currentAngle);
		}

		// Prepare input for character
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
			stepInfo.m_endTime = m_time;

			input.m_stepInfo = stepInfo;
			input.m_characterGravity.set(0,0,-16);
			input.m_velocity = m_characterRigidBody->getRigidBody()->getLinearVelocity();
			input.m_position = m_characterRigidBody->getRigidBody()->getPosition();

			hkpSurfaceInfo ground;
			m_characterRigidBody->checkSupport(stepInfo, ground);

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

		//Apply the player character controller
		{
			HK_TIMER_BEGIN( "simulate character", HK_NULL );

			// Feed output velocity from state machine into character rigid body
			m_characterRigidBody->setLinearVelocity(output.m_velocity, m_timestep);

			// Rotate character to ensure rotation of attached detection phantom 
			// Set directly orientation. Possible, because character shape is symmetric stand capsule
			// Much better approach is implemented in asymmetric character demo
			m_characterRigidBody->getRigidBody()->setRotation(orient);
			
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
			m_env->m_textDisplay->outputText(buffer, 10, 10, 0xffffffff);
		}

	}

	// Do all "AI" interaction characters
	{
		doInteractionCharacterRbs();
	}

	m_world->unlock();

	// Step the world
	{
		hkDefaultPhysicsDemo::stepDemo();
	}

	{
		m_world->lock();

		// Camera Handling
		{
			const hkReal height = 2.0f;
			
			hkVector4 from, to;
			to = m_characterRigidBody->getPosition();
			to.addMul4(height, UP);

			hkVector4 dir;
			dir.setMul4( 3*height, UP);
			dir.addMul4( -8.f, input.m_forward);

			from.setAdd4(to, dir);
			setupDefaultCameras(m_env, from, to, UP , 1.0f);
		}

		m_world->unlock();
	}

	return hkDemo::DEMO_OK;
}

void PushedCharacterRbsDemo::doInteractionCharacterRbs()
{
	int numCharacters = m_characterRigidBodies.getSize();

	hkLocalArray<hkpCharacterInput> input( numCharacters ) ; input.setSize(numCharacters);

	// Fill in the state machine input structure
	{
		for (int i=0; i < numCharacters; i++)
		{
			input[i].m_atLadder = false;
			input[i].m_up = UP;

			// Calculate steering of the characters to follow desiredPosition
			switch (i)
			{
				case 0:
					{
						// no-controller applied
						input[i].m_inputLR = 0.0f;
						input[i].m_inputUD = 0.0f;	
						// display init position
						HK_DISPLAY_STAR(m_desiredPositions[i], 0.1f ,hkColor::LIGHTBLUE);
					} break;

				case 1:
				case 2:
				case 3:
				case 4:
				case 5:
					{
					  // applied "go to point" controller		
					  goToPointController( m_desiredPositions[i], m_characterRigidBodies[i]->getRigidBody(), input[i].m_inputLR, input[i].m_inputUD );
					  // display desired position	
					  HK_DISPLAY_STAR(m_desiredPositions[i], 0.1f ,hkColor::LIGHTBLUE);

					} break;

				case 6:
					{
						const hkReal amplitude = 5.0f;
						hkVector4 desiredPosition; desiredPosition = m_desiredPositions[i]; desiredPosition(1) += amplitude*hkMath::sin(1.0f*m_time);
						// display desired position
						HK_DISPLAY_STAR(desiredPosition,0.1f,hkColor::LIGHTBLUE);

						// display movement line
						hkVector4 start; start = m_desiredPositions[i]; start(1) += amplitude; 
						hkVector4 end; end = m_desiredPositions[i]; end(1) -= amplitude; 

						HK_DISPLAY_LINE(start,end,hkColor::LIGHTBLUE);

						// applied "go to point" controller	
						goToPointController( desiredPosition, m_characterRigidBodies[i]->getRigidBody(), input[i].m_inputLR, input[i].m_inputUD );											
						
					} break;
			}

			input[i].m_wantJump = false;
			input[i].m_forward.set(1,0,0);

			hkStepInfo stepInfo;
			stepInfo.m_deltaTime = m_timestep;
			stepInfo.m_invDeltaTime = 1.0f/m_timestep;
			stepInfo.m_endTime = m_time;

			input[i].m_stepInfo = stepInfo;

			input[i].m_characterGravity = m_world->getGravity();
			input[i].m_velocity = m_characterRigidBodies[i]->getRigidBody()->getLinearVelocity();
			input[i].m_position = m_characterRigidBodies[i]->getRigidBody()->getPosition();

			hkpSurfaceInfo ground;
			m_characterRigidBodies[i]->checkSupport(stepInfo, ground);

			input[i].m_isSupported = (ground.m_supportedState == hkpSurfaceInfo::SUPPORTED);
			input[i].m_surfaceNormal = ground.m_surfaceNormal;
			input[i].m_surfaceVelocity = ground.m_surfaceVelocity;				
		}
	}


	hkLocalArray<hkpCharacterOutput> output( numCharacters);
	output.setSize(numCharacters);

	// Apply the character state machine
	{
		HK_TIMER_BEGIN( "update character state", HK_NULL );

		for (int i=0; i < numCharacters; i++)
		{
			m_characterContexts[i]->update(input[i], output[i]);
		}

		HK_TIMER_END();

	}

	//Apply the character controllers
	{
		HK_TIMER_BEGIN( "simulate character", HK_NULL );

		for (int i=0; i < numCharacters; i++)
		{
			m_characterRigidBodies[i]->setLinearVelocity(output[i].m_velocity, m_timestep);		
		}

		HK_TIMER_END();
	}
}

#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
"This demo shows an interaction of the character rigid body " \
"with another characters in various situations: 'free' character, 'AI' controlled characters, "  \
" group of characters and moving character. Attached phantom (triangle shape) detects potential interacting characters and colors them! "\
" Controls:" \
"  Mouse / Analog Stick to turn \n" \
"  \224 / \225 Forward/Back \n" \
"  LMB / \221 to Jump \n"; 

HK_DECLARE_DEMO(PushedCharacterRbsDemo, HK_DEMO_TYPE_PRIME | HK_DEMO_TYPE_CRITICAL, "Character/Character Interaction Demo", helpString);


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
