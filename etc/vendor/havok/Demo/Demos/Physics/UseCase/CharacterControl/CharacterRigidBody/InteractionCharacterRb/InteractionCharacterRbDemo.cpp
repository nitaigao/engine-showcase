/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>

#include <Demos/Physics/UseCase/CharacterControl/CharacterRigidBody/InteractionCharacterRb/InteractionCharacterRbDemo.h>
#include <Physics/Utilities/CharacterControl/CharacterRigidBody/hkpCharacterRigidBody.h>
#include <Physics/Utilities/CharacterControl/StateMachine/hkpDefaultCharacterStates.h>

// Used for character controller code
#include <Physics/Collide/Shape/Convex/Capsule/hkpCapsuleShape.h> 
#include <Physics/Dynamics/Constraint/Bilateral/Hinge/hkpHingeConstraintData.h>

// Used for graphics and I/O
#include <Common/Base/Monitor/hkMonitorStream.h>
#include <Common/Base/Container/LocalArray/hkLocalArray.h>
#include <Common/Visualize/hkDebugDisplay.h>
#include <Demos/DemoCommon/Utilities/GameUtils/GameUtils.h>
#include <Demos/DemoCommon/Utilities/Character/CharacterUtils.h>
#include <Common/Base/Algorithm/PseudoRandom/hkPseudoRandomGenerator.h>
#include <Demos/DemoCommon/DemoFramework/hkTextDisplay.h>

// Used for transparent camera
#include <Physics/Dynamics/Collide/hkpCollisionListener.h>
#include <Physics/Collide/Query/CastUtil/hkpWorldRayCastInput.h>
#include <Physics/Collide/Query/CastUtil/hkpWorldRayCastOutput.h>
#include <Physics/Collide/Query/Collector/RayCollector/hkpAllRayHitCollector.h>

// Level colors 
#define NORMAL_GRAY				hkColor::rgbFromChars( 192, 192, 192, 255 )
#define TRANSPARENT_GREY		hkColor::rgbFromChars( 192, 192, 192, 125 )

static const hkVector4 UP(0,0,1);

InteractionCharacterRbDemo::InteractionCharacterRbDemo(hkDemoEnvironment* env)
:	hkDefaultPhysicsDemo(env) 
{

	// Create the world
	{
		hkpWorldCinfo info;
		info.setupSolverInfo( hkpWorldCinfo::SOLVER_TYPE_4ITERS_MEDIUM ); // default solver
		info.m_contactPointGeneration = hkpWorldCinfo::CONTACT_POINT_ACCEPT_ALWAYS;
		info.setBroadPhaseWorldSize( 350.0f );  
		info.m_gravity.set(0,0,-9.8f);
		info.m_collisionTolerance = 0.01f;
		
		m_world = new hkpWorld( info );
		m_world->lock();

		hkpAgentRegisterUtil::registerAllAgents(m_world->getCollisionDispatcher());

		setupGraphics();
	}

	// Create the floor
	{
		hkpShape* box = new hkpBoxShape(hkVector4(100,100,1));

		hkpRigidBodyCinfo ci;
		ci.m_shape = box;
		ci.m_motionType = hkpMotion::MOTION_FIXED;
		ci.m_friction = 1.0f;
		ci.m_collisionFilterInfo = hkpGroupFilter::calcFilterInfo(hkpGroupFilter::calcFilterInfo(1));

		hkpRigidBody* entity = new hkpRigidBody(ci);
		box->removeReference();
		m_world->addEntity(entity);
		entity->removeReference();
	}

	hkReal roomSide = 20;
	
	// Create walls
	{
		hkpShape* box = new hkpBoxShape(hkVector4(1,roomSide ,3));

		hkpRigidBodyCinfo ci;
		ci.m_shape = box;
		ci.m_motionType = hkpMotion::MOTION_FIXED;
		ci.m_collisionFilterInfo = hkpGroupFilter::calcFilterInfo(hkpGroupFilter::calcFilterInfo(1));
		{
			ci.m_position.set(roomSide,0,3);
			hkpRigidBody* entity = new hkpRigidBody(ci);
			m_world->addEntity(entity);
			
			// for wall transparency
			const hkUlong id = hkUlong(entity ->getCollidable());
			HK_SET_OBJECT_COLOR(id,NORMAL_GRAY);
			m_objectIds.pushBack(id);

			entity->removeReference();

		}
		{
			ci.m_position.set(-roomSide, 0, 3);
			hkpRigidBody* entity = new hkpRigidBody(ci);
			m_world->addEntity(entity);
			
			// for wall transparency
			const hkUlong id = hkUlong(entity ->getCollidable());
			HK_SET_OBJECT_COLOR(id,NORMAL_GRAY);
			m_objectIds.pushBack(id);

			entity->removeReference();
		}

		ci.m_rotation.setAxisAngle(hkVector4(0,0,1), HK_REAL_PI * .5f);

		{
			ci.m_position.set(0, roomSide, 3);
			hkpRigidBody* entity = new hkpRigidBody(ci);
			m_world->addEntity(entity);
			
			// for wall transparency
			const hkUlong id = hkUlong(entity ->getCollidable());
			HK_SET_OBJECT_COLOR(id,NORMAL_GRAY);
			m_objectIds.pushBack(id);

			entity->removeReference();
		}
		{
			ci.m_position.set(0, -roomSide, 3);
			hkpRigidBody* entity = new hkpRigidBody(ci);
			m_world->addEntity(entity);
			
			// for wall transparency
			const hkUlong id = hkUlong(entity ->getCollidable());
			HK_SET_OBJECT_COLOR(id,NORMAL_GRAY);
			m_objectIds.pushBack(id);

			entity->removeReference();
		}
		box->removeReference();
	}


	// Create some objects for pushing around

	hkReal objectSeparation = 4;

	hkPseudoRandomGenerator random(501);
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			hkVector4 size(random.getRandReal01(), random.getRandReal01(), random.getRandReal01());
			hkVector4 minSize( 1,1,1 );
			size.add4( minSize );

			hkVector4 position((hkReal)i*objectSeparation - 10, (hkReal)j*objectSeparation - 10, 2);

			hkReal volume = size(0) * size(1) * size(2);
			hkReal mass = 0;
			int color = 0;
			switch (i % 4)
			{
				// Very Heavy
				case 0:
					mass = 300;
					color = hkColor::BLUE;
					break;
				// Heavy
				case 1:
					mass = 100;
					color = 0xffcc9933;
					break;
				// Light
				case 2:
					mass = 50;
					color = 0xff660000;
					break;
				// Very Light
				case 3:
					mass = 5;
					color = hkColor::WHITE;
					break;
			}

			hkReal scaledMass = volume * mass;

			hkpRigidBody* body;
			switch(j % 4)
			{
				case 0:
					body = GameUtils::createBox(size, scaledMass, position);
					break;
				case 1:
					body = GameUtils::createCapsuleFromBox(size, scaledMass , position);
					break;
				case 2:
					body = GameUtils::createRandomConvexGeometricFromBox(size, scaledMass , position, 10, &random);
					break;
				default:
					hkReal radius = size(0)/2.0f;
					body = GameUtils::createSphere(radius, scaledMass * HK_REAL_PI / 6, position);
					break;

			}
			m_rigidBodies.pushBack(body);

			m_world->addEntity(body);
			HK_SET_OBJECT_COLOR((hkUlong)body->getCollidable(), color);

			body->removeReference();
		}
	}

	// Create a seesaw
	{
		hkVector4 halfSize(hkVector4(5,2,.05f));
		hkpShape* box = new hkpBoxShape(halfSize);

		hkpRigidBodyCinfo ci;
		ci.m_shape = box;
		ci.m_motionType = hkpMotion::MOTION_BOX_INERTIA;
		ci.m_position.set(10,10,3);
		ci.m_collisionFilterInfo = hkpGroupFilter::calcFilterInfo(hkpGroupFilter::calcFilterInfo(3));

		ci.m_mass = 25.0f;
		hkpMassProperties massProperties;
		hkpInertiaTensorComputer::computeBoxVolumeMassProperties(halfSize, ci.m_mass, massProperties);
		ci.m_inertiaTensor = massProperties.m_inertiaTensor;
		ci.m_centerOfMass = massProperties.m_centerOfMass;	

		ci.m_inertiaTensor.mul(2);
		ci.m_restitution = 0;

		hkpRigidBody* entity = new hkpRigidBody(ci);
		box->removeReference();
		m_world->addEntity(entity);
		entity->removeReference();

		hkpHingeConstraintData* hc = new hkpHingeConstraintData();
		{
			// Set the pivot
			hkVector4 pivot;		
			pivot.setAdd4(ci.m_position, halfSize);

			hkVector4 axis(0.0f, 1.0f, 0.0f);

			// Create constraint
			hc->setInWorldSpace(entity->getTransform(), hkTransform::getIdentity(), ci.m_position, axis);		

			hkpConstraintInstance* constraint = new hkpConstraintInstance(entity, m_world->getFixedRigidBody(), hc);
			m_world->addConstraint( constraint); 
			constraint->removeReference();

			hc->removeReference();  
		}

	}

	
	//	Create character rigid body
	{

		// Construct a shape
		hkVector4 top(0,0, .4f);
		hkVector4 bottom(0,0,-.4f);		

		// Create a capsule to represent the character standing
		hkpShape* standCapsule = new hkpCapsuleShape(top, bottom, .6f);


		// Construct a character rigid body
		hkpCharacterRigidBodyCinfo info;
		info.m_mass = 100.0f;
		info.m_shape = standCapsule;

		info.m_maxForce = 2000.0f;
		info.m_up = UP;
		info.m_position.set(9,9,6);


		m_characterRigidBody = new hkpCharacterRigidBody( info );
		m_world->addEntity( m_characterRigidBody->getRigidBody() );


		standCapsule->removeReference();

	}

	// Create the Character state machine and context
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
		manager->removeReference();
		
		// Set character type and smaller velocity output filter parameters 
		// (defaults are: gain = 1, maxVelocity = 20, maxAcceleration = 625 )
		m_characterContext->setCharacterType(hkpCharacterContext::HK_CHARACTER_RIGIDBODY);
		m_characterContext->setFilterParameters(0.9f,15.0f,400.0f);
	}
	
	// Current camera angle about UP
	m_currentAngle = HK_REAL_PI;
	m_env->m_window->setMousePosition(m_env->m_window->getWidth() >> 1, m_env->m_window->getHeight() >> 1);

	// Init actual time
	m_time = 0.0f;

	// Initialize hkpSurfaceInfo for previous ground info keeping
	m_previousGround = new hkpSurfaceInfo();
	m_framesInAir = 0;

	m_world->unlock();
}

InteractionCharacterRbDemo::~InteractionCharacterRbDemo()
{
	m_world->lock();

	m_characterRigidBody->removeReference();
	m_characterContext->removeReference();

	delete m_previousGround;

	m_world->unlock();	
}

void InteractionCharacterRbDemo::makeFakeInput()
{
	// go forward
	m_env->m_gamePad->forceButtonPressed(HKG_PAD_DPAD_UP);

	if ( (m_physicsStepCounter % 3) == 0)
	{
		// try to fire a box
		m_env->m_gamePad->forceButtonPressed(HKG_PAD_BUTTON_2);
	}

	if ( (m_physicsStepCounter % 15) == 0)
	{
		// jump
		m_env->m_gamePad->forceButtonPressed(HKG_PAD_BUTTON_1);
	}

}

hkDemo::Result InteractionCharacterRbDemo::stepDemo()
{
	// Update actual time
	m_time += m_timestep;

	// Do character
	{
		m_world->lock();

		hkpCharacterInput input;
		hkpCharacterOutput output;

		// Display masses of interaction rigid bodies
		for (int i = 0; i < m_rigidBodies.getSize(); ++i)
		{
			const hkVector4& pos = m_rigidBodies[i]->getPosition();
			hkString str; str.printf("%.1f", m_rigidBodies[i]->getMass() );
			m_env->m_textDisplay->outputText3D(str, pos(0), pos(1), pos(2) + 2 );
		}

		//	Get user input data
		hkReal posX = 0.f;
		hkReal posY = 0.f;
		{
			float deltaAngle = 0.f;
			CharacterUtils::getUserInputForCharacter(m_env, deltaAngle, posX, posY);
			m_currentAngle += deltaAngle;
			m_currentOrient.setAxisAngle(UP, m_currentAngle);
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
			input.m_forward.setRotatedDir( m_currentOrient, input.m_forward );

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

			// Avoid accidental state changes (Smooth movement on stairs)
			// During transition supported->unsupported continue to return N-frames hkpSurfaceInfo data from previous supported state
			{

				// Number of frames to skip (continue with previous hkpSurfaceInfo data)
				const int skipFramesInAir = 3;

				if (input.m_wantJump)
				{
					m_framesInAir = skipFramesInAir;
				}

				if ( ground.m_supportedState != hkpSurfaceInfo::SUPPORTED )
				{
					if (m_framesInAir < skipFramesInAir)
					{
						input.m_isSupported = true;
						input.m_surfaceNormal = m_previousGround->m_surfaceNormal;
						input.m_surfaceVelocity = m_previousGround->m_surfaceVelocity;
						input.m_surfaceMotionType = m_previousGround->m_surfaceMotionType;
					}
					else
					{
						input.m_isSupported = false;
						input.m_surfaceNormal = ground.m_surfaceNormal;
						input.m_surfaceVelocity = ground.m_surfaceVelocity;	
						input.m_surfaceMotionType = ground.m_surfaceMotionType;
					}			

					m_framesInAir++;

				}
				else
				{
					input.m_isSupported = true;
					input.m_surfaceNormal = ground.m_surfaceNormal;
					input.m_surfaceVelocity = ground.m_surfaceVelocity;
					input.m_surfaceMotionType = ground.m_surfaceMotionType;

					m_previousGround->set(ground);


					// Reset framesInAir
					if (m_framesInAir > skipFramesInAir)
					{
						m_framesInAir = 0;
					}			

				}
			}
		}

		// Apply the character state machine
		{
			HK_TIMER_BEGIN( "update character state", HK_NULL );

			m_characterContext->update(input, output);			

			HK_TIMER_END();
		}

		// Apply the player character controller
		{
			HK_TIMER_BEGIN( "simulate character", HK_NULL );

			// Set output velocity from state machine into character rigid body
			m_characterRigidBody->setLinearVelocity(output.m_velocity, m_timestep);
			
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

		// Fire box
		{
	
			static int count = 0;
			count++;
			static hkReal sideLength = .5f;
			hkBool fireBoxState = (m_env->m_window->getMouse().getButtonState() & HKG_MOUSE_RIGHT_BUTTON)
				|| m_env->m_gamePad->isButtonPressed(HKG_PAD_BUTTON_2);

			if ( fireBoxState && (count%10 == 0) )
			{
				hkpShape* box = new hkpBoxShape(hkVector4(sideLength, sideLength, sideLength));

				hkpRigidBodyCinfo ci;
				ci.m_shape = box;
				ci.m_motionType = hkpMotion::MOTION_BOX_INERTIA;
				ci.m_position = m_characterRigidBody->getRigidBody()->getPosition();

				hkVector4 forward;
				forward.set(0,4,1);
				forward.setRotatedDir( m_currentOrient, forward );
				ci.m_position.add4(forward);
				ci.m_mass = sideLength * sideLength * sideLength * 1000;
				ci.m_inertiaTensor.mul(ci.m_mass);

				ci.m_linearVelocity.setNeg4(forward);
				ci.m_linearVelocity.mul4(10);

				hkpRigidBody* entity = new hkpRigidBody(ci);
				box->removeReference();
				m_world->addEntity(entity);
				entity->removeReference();

			}
		}

		m_world->unlock();
	}

	// Step the world
	{
		hkDefaultPhysicsDemo::stepDemo();
	}
	
	// Camera Handling
	{
		m_world->lock();

		cameraHandling();
		
		m_world->unlock();
	}

	return hkDemo::DEMO_OK;
}

void InteractionCharacterRbDemo::cameraHandling()
{
	const hkReal height = 1.7f;
	hkVector4 forward;
	forward.set(1,0,0);
	forward.setRotatedDir( m_currentOrient, forward );

	hkVector4 from, to;
	to = m_characterRigidBody->getRigidBody()->getPosition();
	to.addMul4(height, UP );

	hkVector4 dir;
	dir.setMul4( height, UP );
	dir.addMul4( -8.f, forward);

	from.setAdd4(to, dir);

	// Make object in line of sight transparent
	{
		// Cast down to landscape to get an accurate position
		hkpWorldRayCastInput raycastIn;

		// Reverse direction for collision detection
		raycastIn.m_from = to;
		raycastIn.m_to = from;
		raycastIn.m_filterInfo = hkpGroupFilter::calcFilterInfo(0);

		hkpAllRayHitCollector collector;

		m_world->castRay( raycastIn, collector);

		// 
		hkLocalArray<hkUlong>	transp(5);

		// Loop over all collected objects
		for(int i=0; i < collector.getHits().getSize();i++)
		{
			hkpWorldRayCastOutput raycastOut = collector.getHits()[i];

			transp.pushBack(hkUlong(raycastOut.m_rootCollidable));
		}

		// Loop over all objects display ids
		for(int i=0; i < m_objectIds.getSize();i++)
		{
			if(transp.indexOf(m_objectIds[i]) != -1)
			{
				HK_SET_OBJECT_COLOR(m_objectIds[i], TRANSPARENT_GREY);
			}
			else
			{
				HK_SET_OBJECT_COLOR(m_objectIds[i],NORMAL_GRAY);
			}
		}

	}

	setupDefaultCameras(m_env, from , to, UP, 1.0f);
}



#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
" This demo shows interaction with objects of different sizes and weights. " \
" The weight of each rigid body is displayed above the rigid body" \
" Specific objects are the seesaw and fired boxes\n"
" Controls:" \
"  Mouse / Analog Stick to turn \n" \
"  \224 / \225 Forward/Back \n" \
"  RMB / \222 to Fire a box at the character. \n" \
"  LMB / \221 to Jump.";

HK_DECLARE_DEMO(InteractionCharacterRbDemo, HK_DEMO_TYPE_PRIME | HK_DEMO_TYPE_CRITICAL, "Character/Dynamic Objects Interaction Demo", helpString);


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
