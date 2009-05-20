/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Common/Base/Monitor/hkMonitorStream.h>

#include <Demos/Physics/UseCase/CharacterControl/CharacterProxy/CharacterInteraction/CharacterInteractionDemo.h>
#include <Physics/Utilities/CharacterControl/CharacterProxy/hkpCharacterProxy.h>
#include <Physics/Utilities/CharacterControl/StateMachine/hkpDefaultCharacterStates.h>

#include <Demos/DemoCommon/Utilities/GameUtils/GameUtils.h>
#include <Demos/DemoCommon/Utilities/Character/CharacterUtils.h>
#include <Common/Base/Algorithm/PseudoRandom/hkPseudoRandomGenerator.h>
#include <Demos/DemoCommon/DemoFramework/hkTextDisplay.h>

#include <Graphics/Common/Input/Pad/hkgPad.h>
#include <Graphics/Common/Window/hkgWindow.h>
#include <Physics/Collide/Query/Collector/PointCollector/hkpClosestCdPointCollector.h>
#include <Physics/Collide/Shape/Convex/Capsule/hkpCapsuleShape.h> 
#include <Physics/Dynamics/Phantom/hkpSimpleShapePhantom.h>

#include <Physics/Dynamics/Constraint/Bilateral/Hinge/hkpHingeConstraintData.h>

#include <Physics/Utilities/Constraint/Keyframe/hkpKeyFrameUtility.h>

#include <Common/Visualize/hkDebugDisplay.h>


#include <Demos/DemoCommon/DemoFramework/hkTextDisplay.h>
#include <Physics/ConstraintSolver/Simplex/hkpSimplexSolver.h>

// This listener can be used to prevent objects from moving the character at all.
// It should only be used if the charater strength has been set to REAL_MAX.
class ZeroPlanesCharacterInteractionListener : public hkReferencedObject, public hkpCharacterProxyListener
{
	public:
		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO);

		ZeroPlanesCharacterInteractionListener(const hkVector4& up)
		{
			m_up = up;
		}

		// In this callback we examine the constraints passed to the simplex.
		// WARNING: This only works when the character is not in a moving environment, as the velocities
		// are zeroed. 
		void processConstraintsCallback( const hkArray<hkpRootCdPoint>& manifold, hkpSimplexSolverInput& input ) 
		{
			// Do not move the character for dynamic bodies.
			int i;
			for (i=0; i < manifold.getSize(); i++)
			{
				// For each constraint, check if it comes from a dynamic body.
				// If it does, zero the constraint velocity.

				// Get the rigid body.
				hkpRigidBody* rigidBody = hkGetRigidBody(manifold[i].m_rootCollidableB);

				// Make sure we got a rigid body, not another phantom.
				if (rigidBody && !rigidBody->isFixedOrKeyframed())
				{
					hkpSurfaceConstraintInfo& surface = input.m_constraints[i];
					surface.m_velocity.setZero4();
				}
			}
			// The remaining planes are vertical planes which have been added in to prevent character movement up
			// slopes which are too steep.
			// Unfortunately we do not have a corresponging manifold point, so we just set all these velocities to
			// the velocity of the character.
			while (i < input.m_numConstraints)
			{
				hkpSurfaceConstraintInfo& surface = input.m_constraints[i];
				surface.m_velocity.setZero4();
				i++;
			}
		}

		hkVector4 m_up;
};


CharacterInteractionDemo::CharacterInteractionDemo(hkDemoEnvironment* env)
:	hkDefaultPhysicsDemo(env)
{
	//
	// Create the world
	//
	{
		hkpWorldCinfo info;
		info.setupSolverInfo( hkpWorldCinfo::SOLVER_TYPE_4ITERS_MEDIUM );
		info.setBroadPhaseWorldSize( 350.0f );  
		info.m_gravity.set(0,0,-9.8f);
		info.m_collisionTolerance = 0.1f;
		
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
		ci.m_collisionFilterInfo = hkpGroupFilter::calcFilterInfo(hkpGroupFilter::calcFilterInfo(1));

		hkpRigidBody* entity = new hkpRigidBody(ci);
		box->removeReference();
		m_world->addEntity(entity);
		entity->removeReference();
	}

	hkReal roomSide = 20;
	
	// Create Walls
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
			entity->removeReference();

		}
		{
			ci.m_position.set(-roomSide, 0, 3);
			hkpRigidBody* entity = new hkpRigidBody(ci);
			m_world->addEntity(entity);
			entity->removeReference();
		}


		ci.m_rotation.setAxisAngle(hkVector4(0,0,1), HK_REAL_PI * .5f);

		{
			ci.m_position.set(0, roomSide, 3);
			hkpRigidBody* entity = new hkpRigidBody(ci);
			m_world->addEntity(entity);
			entity->removeReference();
		}
		{
			ci.m_position.set(0, -roomSide, 3);
			hkpRigidBody* entity = new hkpRigidBody(ci);
			m_world->addEntity(entity);
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
				case 0:
					// Very Heavy
					mass = 300;
					color = hkColor::BLUE;
					break;
				case 1:
					// Heavy
					mass = 100;
					color = 0xffcc9933;
					break;
				case 2:
					// Very Light
					mass = 50;
					color = 0xff660000;
					break;
				case 3:
					// Very Light
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

	// Create a see-saw
	{
		hkVector4 halfSize(hkVector4(5,2,.05f));
		hkpShape* box = new hkpBoxShape(halfSize);

		hkpRigidBodyCinfo ci;
		ci.m_shape = box;
		ci.m_motionType = hkpMotion::MOTION_BOX_INERTIA;
		ci.m_position.set(10,10,3);
		ci.m_collisionFilterInfo = hkpGroupFilter::calcFilterInfo(hkpGroupFilter::calcFilterInfo(3));

		ci.m_mass = 50;
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

	//
	//	Create character controller
	//
	{
		// Construct a shape

		hkVector4 top(0,0, .4f);
		hkVector4 bottom(0,0,-.4f);		

		// Create a capsule to represent the character standing
		hkpShape* standCapsule = new hkpCapsuleShape(top, bottom, .6f);

		// Construct a Shape Phantom
		hkpShapePhantom* phantom = new hkpSimpleShapePhantom( standCapsule, hkTransform::getIdentity(), hkpGroupFilter::calcFilterInfo(0,2) );
		standCapsule->removeReference();

		// Add the phantom to the world
		m_world->addPhantom(phantom);
		phantom->removeReference();

		// Construct a character proxy
		hkpCharacterProxyCinfo cpci;
		cpci.m_position.set(9, 9, 6);
		cpci.m_staticFriction = 0.0f;
		cpci.m_dynamicFriction = 1.0f;
		cpci.m_up.set(0,0,1);
		cpci.m_userPlanes = 4;
		cpci.m_keepDistance = .05f;
		cpci.m_maxSlope = HK_REAL_PI / 3.f;

		//
		// This value will affect how much the character pushes down on objects it stands on. 
		//

		cpci.m_characterMass = 100;

		//
		// This value will affect how much the character is able to push other objects around.
		//

		cpci.m_characterStrength = 5000;

		cpci.m_shapePhantom = phantom;
		m_characterProxy = new hkpCharacterProxy( cpci );
	}


	// This custom listener will stop the character from being moved by any dynamic object. It should only
	// Be enabled if the character strength is REAL_MAX.
	m_listener = HK_NULL;
	//m_listener = new ZeroPlanesCharacterInteractionListener( m_characterProxy->m_up );
	//m_characterProxy->addCharacterProxyListener(m_listener);


	//
	// Create the Character state machine and context
	//
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
	}
	
	// Current camera angle about up
	m_currentAngle = 3.81f;
	m_env->m_window->setMousePosition(m_env->m_window->getWidth() >> 1, m_env->m_window->getHeight() >> 1);

	m_time = 0;

	m_world->unlock();
}

CharacterInteractionDemo::~CharacterInteractionDemo()
{
	m_world->lock();

	m_world->removePhantom(m_characterProxy->getShapePhantom());
	m_characterProxy->removeReference();

	m_world->unlock();

	delete m_listener;

	delete m_characterContext;
}

void CharacterInteractionDemo::makeFakeInput()
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

/////////////////////////////////////////////////////////////////////

hkDemo::Result CharacterInteractionDemo::stepDemo()
{

	m_time += m_timestep;

	hkpCharacterInput input;

	{
		m_world->lock();

		// Display masses of rigid bodies
		for (int i = 0; i < m_rigidBodies.getSize(); ++i)
		{
			const hkVector4& pos = m_rigidBodies[i]->getPosition();
			hkString str; str.printf("%.1f", m_rigidBodies[i]->getMass() );
			m_env->m_textDisplay->outputText3D(str, pos(0), pos(1), pos(2) + 2 );
		}

		//	Get user input data
		int m_upAxisIndex = 2;
		hkVector4 up; up.setZero4();
		up(m_upAxisIndex) = 1;

		hkQuaternion orient;
		hkReal posX = 0.f;
		hkReal posY = 0.f;
		{
			float deltaAngle = 0.f;
			CharacterUtils::getUserInputForCharacter(m_env, deltaAngle, posX, posY);
			m_currentAngle += deltaAngle;
			orient.setAxisAngle(up, m_currentAngle);
		}

		// Prepare input for character
		hkpCharacterOutput output;
		{

			input.m_inputLR = posX;
			input.m_inputUD = posY;

			input.m_wantJump = m_env->m_window->getMouse().wasButtonPressed(HKG_MOUSE_LEFT_BUTTON)
				|| m_env->m_gamePad->wasButtonPressed(HKG_PAD_BUTTON_1);
			input.m_atLadder = false;

			input.m_up = m_characterProxy->m_up;
			input.m_forward.set(1,0,0);
			input.m_forward.setRotatedDir( orient, input.m_forward );

			input.m_stepInfo.m_deltaTime = m_timestep;
			input.m_stepInfo.m_invDeltaTime = 1.0f / m_timestep;
			input.m_stepInfo.m_endTime = m_time;
			input.m_characterGravity.set(0,0,-16);
			input.m_velocity = m_characterProxy->getLinearVelocity();
			input.m_position = m_characterProxy->getPosition();

			hkVector4 down;	down.setNeg4(m_characterProxy->m_up);
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


		//Apply the player character controller
		{
			HK_TIMER_BEGIN( "simulate character", HK_NULL );

			// Feed output from state machine into character proxy
			m_characterProxy->setLinearVelocity(output.m_velocity);

			hkStepInfo si;
			si.m_deltaTime = m_timestep;
			si.m_invDeltaTime = 1.0f/m_timestep;

			m_characterProxy->integrate( si, m_world->getGravity() );

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

		// fire box

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
			ci.m_position = m_characterProxy->getPosition();

			hkVector4 forward;
			forward.set(0,4,1);
			forward.setRotatedDir( orient, forward );
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

		m_world->unlock();
	}

	// Step the world
	hkDefaultPhysicsDemo::stepDemo();

	{
		m_world->lock();

		// Camera Handling
		{
			const hkReal height = 1.7f;

			hkVector4 from, to;
			to = m_characterProxy->getPosition();
			to.addMul4(height, m_characterProxy->m_up);

			hkVector4 dir;
			dir.setMul4( height, m_characterProxy->m_up );
			dir.addMul4( -8.f, input.m_forward);

			from.setAdd4(to, dir);
			setupDefaultCameras(m_env, from, to, m_characterProxy->m_up, 1.0f);
		}

		m_world->unlock();
	}

	return hkDemo::DEMO_OK;
}



#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
"\nThis is our dynamic test scene for the Havok character controller " \
"This demo shows interaction with objects of different sizes and weights." \
"The weight of each rigid body is displayed above the rigid body" \
"Controls:" \
"  Mouse / Analog Stick to turn \n" \
"  \224 / \225 Forward/Back \n" \
"  RMB / \222 to Fire a box at the character. \n" \
"  LMB / \221 to Jump.";

HK_DECLARE_DEMO(CharacterInteractionDemo, HK_DEMO_TYPE_PRIME | HK_DEMO_TYPE_CRITICAL, "Character / Object Interaction Demo", helpString);


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
