/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>

#include <Demos/DemoCommon/Utilities/Character/CharacterUtils.h>
#include <Physics/Utilities/Weapons/hkpFirstPersonGun.h>

#include <Physics/Dynamics/Entity/hkpRigidBody.h>
#include <Physics/Utilities/CharacterControl/CharacterRigidBody/hkpCharacterRigidBody.h>
#include <Physics/Utilities/CharacterControl/StateMachine/hkpDefaultCharacterStates.h>

#include <Physics/Collide/Filter/Group/hkpGroupFilter.h>
#include <Physics/Collide/Shape/Convex/Capsule/hkpCapsuleShape.h>
#include <Physics/Collide/Shape/Convex/Sphere/hkpSphereShape.h>
#include <Physics/Collide/Query/CastUtil/hkpWorldRayCastInput.h>
#include <Physics/Collide/Query/CastUtil/hkpWorldRayCastOutput.h>
#include <Physics/Collide/Query/Collector/RayCollector/hkpAllRayHitCollector.h>
#include <Physics/Collide/Query/CastUtil/hkpLinearCastInput.h>
#include <Physics/Collide/Query/Collector/PointCollector/hkpClosestCdPointCollector.h>

#include <Physics/Dynamics/World/hkpWorld.h>

#include <Common/Visualize/hkDebugDisplay.h>
#include <Graphics/Common/Camera/hkgCamera.h>
#include <Graphics/Common/DisplayObject/hkgDisplayObject.h>
#include <Graphics/Bridge/DisplayHandler/hkgDisplayHandler.h>


	#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
	#undef _WIN32_WINNT
	#define _WIN32_WINNT 0x0500	  // win2000
	#undef _WIN32_WINDOWS 
	#define _WIN32_WINDOWS 0x0500 // win2000
	#undef WINVER	
	#define WINVER 0x0500	      // win2000

	#include <windows.h>


static void HK_CALL _deadzone(float& stickX, float& stickY)
{
	hkReal INPUT_DEADZONE = 0.15f; // PS2 style .. large

	const float deadzoneScale = 1.0f/(1.0f-INPUT_DEADZONE);
	stickX = ( stickX < -INPUT_DEADZONE) ? (stickX + INPUT_DEADZONE) * deadzoneScale : (
		( stickX > INPUT_DEADZONE ) ? (stickX - INPUT_DEADZONE) * deadzoneScale : 0);
	stickY = ( stickY < -INPUT_DEADZONE) ? (stickY + INPUT_DEADZONE) * deadzoneScale : (
		( stickY > INPUT_DEADZONE ) ? (stickY - INPUT_DEADZONE) * deadzoneScale : 0);
}


void HK_CALL CharacterUtils::getUserInputForCharacter( hkDemoEnvironment* env, float& deltaAngle, float& posX, float& posY )
{
	// Only run this function if the window is in focus, because it grabs the mouse
	if ( GetForegroundWindow() != env->m_window->getPlatformHandle() )
		return;

	// Mouse
	deltaAngle = 0.f;
	posX = 0.f;
	posY = 0.f;
	const hkgPad& pad = *(env->m_gamePad);

	bool haveProperGamePad = env->m_window->hasGamePads() && !(env->m_options->m_forceKeyboardGamepad);

	// No gamepad (win32 and PLAYSTATION(R)3 default at the mo)
	if (!haveProperGamePad)
	{
		const hkReal MOUSE_SENSITIVITY = 1.0f;
		int mouseX = env->m_window->getMouse().getPosX();
		int halfWindowWidth = env->m_window->getWidth() >> 1;

		{
			hkReal inc  = hkReal(halfWindowWidth - mouseX) / halfWindowWidth;
			inc = (inc < -1.0f) ? -1.0f : inc;
			inc = (inc >  1.0f) ?  1.0f : inc;
			deltaAngle = 3.4f * inc * MOUSE_SENSITIVITY;
		}


		// Snap mouse back to middle
		env->m_window->setMousePosition(env->m_window->getWidth() >> 1, env->m_window->getHeight() >> 1);

		if (env->m_window->getKeyboard().getKeyState( 'A'))
			posX=1.f;
		if (env->m_window->getKeyboard().getKeyState( 'D'))
			posX=-1.f;
		if (env->m_window->getKeyboard().getKeyState( 'W'))
			posY=-1.f;
		if (env->m_window->getKeyboard().getKeyState( 'S'))
			posY=1.f;

	}
	else if (pad.isValid())
	{
		if ( (pad.getButtonState() & HKG_PAD_BUTTON_L1) == 0)
		{
			const hkReal PAD_SENSITIVITY = 0.05f;
			float stickX = pad.getStickPosX(1);
			float stickY = pad.getStickPosY(1);
			_deadzone(stickX, stickY);
			deltaAngle = 3.4f * -stickX * PAD_SENSITIVITY;
			posY = -stickY;
		}

		{
			float stickZ = pad.getStickPosX(0);
			float stickZZ = 0;
			_deadzone(stickZ, stickZZ);
			posX = -stickZ; // strafe on stick0
		}
	}

	// dpad == dir keys on PC
	{
		if ((pad.getButtonState() & HKG_PAD_DPAD_UP) != 0)
			posY=-1.f;
		if ((pad.getButtonState() & HKG_PAD_DPAD_LEFT) != 0)
			posX=1.f;
		if ((pad.getButtonState() & HKG_PAD_DPAD_RIGHT) != 0)
			posX=-1.f;
		if ((pad.getButtonState() & HKG_PAD_DPAD_DOWN) != 0)
			posY=1.f;
	}

	// Normalize the movement vector
	hkReal lenSqd = posY * posY + posX * posX;
	if (lenSqd > HK_REAL_MIN)
	{
		lenSqd = hkMath::sqrt(lenSqd);
		posY /= lenSqd;
		posX /= lenSqd;
	}
}

void HK_CALL CharacterUtils::getUserInputForCharacter( hkDemoEnvironment* env, const Sensivity& sensivity, float& deltaAngle, float& deltaElevation, float& posX, float& posY )
{
	// Mouse
	deltaAngle = 0.f;
	deltaElevation = 0.f;
	posX = 0.f;
	posY = 0.f;

	// Only run this function if the window is in focus, because it grabs the mouse
	if ( GetForegroundWindow() != env->m_window->getPlatformHandle() )
	{
		return;
	}

	const hkgPad& pad = *(env->m_gamePad);

	if (pad.isValid())
	{
		if ( (pad.getButtonState() & HKG_PAD_BUTTON_L1) == 0)
		{
			float stickX = pad.getStickPosX(1);
			float stickY = pad.getStickPosY(1);
			_deadzone(stickX, stickY);
			posX = -stickX; // strafe on stick1
			posY = -stickY;
		}

		{
			const hkReal PAD_SENSITIVITY = 0.02f;
			float stickX = pad.getStickPosX(0);
			float stickY = pad.getStickPosY(0);
			_deadzone(stickX, stickY);
			deltaAngle		= 3.4f * -stickX * PAD_SENSITIVITY * sensivity.m_padX;
			deltaElevation	= 3.4f * -stickY * PAD_SENSITIVITY * sensivity.m_padY;
		}

		// dpad == dir keys on PC
		{
			if ((pad.getButtonState() & HKG_PAD_DPAD_UP) != 0)
				posY=-1.f;
			if ((pad.getButtonState() & HKG_PAD_DPAD_LEFT) != 0)
				posX=1.f;
			if ((pad.getButtonState() & HKG_PAD_DPAD_RIGHT) != 0)
				posX=-1.f;
			if ((pad.getButtonState() & HKG_PAD_DPAD_DOWN) != 0)
				posY=1.f;
		}

	}

	// No gamepad (win32 and PLAYSTATION(R)3 default at the mo)
	if( env->m_window->getMouse().isValid() )
	{
		int mouseX = env->m_window->getMouse().getPosX();
		int halfWindowWidth = env->m_window->getWidth() >> 1;

		{
			hkReal inc  = hkReal(halfWindowWidth - mouseX) / halfWindowWidth;
			inc = (inc < -1.0f) ? -1.0f : inc;
			inc = (inc >  1.0f) ?  1.0f : inc;
			if ( inc != 0 )
				deltaAngle = 3.4f * inc * sensivity.m_mouseY;
		}

		int mouseY = env->m_window->getMouse().getPosY() + 1; // XXX not sure why I have to add 1
		int halfWindowHeight = env->m_window->getHeight() >> 1;
		{
			hkReal inc  = hkReal(halfWindowHeight - mouseY) / halfWindowHeight;
			inc = (inc < -1.0f) ? -1.0f : inc;
			inc = (inc >  1.0f) ?  1.0f : inc;
			if ( inc != 0.0f )
			{
				deltaElevation = 3.4f * inc * sensivity.m_mouseX;
			}
		}

		// Snap mouse back to middle
		env->m_window->setMousePosition(env->m_window->getWidth() >> 1, env->m_window->getHeight() >> 1);

		if (env->m_window->getKeyboard().getKeyState( 'A'))
			posX=1.f;
		if (env->m_window->getKeyboard().getKeyState( 'D'))
			posX=-1.f;
		if (env->m_window->getKeyboard().getKeyState( 'W'))
			posY=-1.f;
		if (env->m_window->getKeyboard().getKeyState( 'S'))
			posY=1.f;
		}


	// Normalize the movement vector
	hkReal lenSqd = posY * posY + posX * posX;
	if (lenSqd > 1.0f)
	{
		lenSqd = hkMath::sqrt(lenSqd);
		posY /= lenSqd;
		posX /= lenSqd;
	}
}

// Compute the blend params that will produce the desired velocity
void CharacterUtils::computeBlendParams( hkReal desiredVel, hkReal walkVel, hkReal runVel, hkReal walkDur, hkReal runDur, hkReal& blend, hkReal& walkSpeed, hkReal& runSpeed )
{
	// Analytical solution of blending aproximation
	// Solution is second root of quadratic equation 

	const hkReal runWalkRatio = runDur / walkDur;

	const hkReal wVratio = walkVel*runWalkRatio;
	const hkReal rVratio = runVel*runWalkRatio;
	const hkReal rVratio2 = rVratio*runWalkRatio;
	const hkReal dVratio = desiredVel*runWalkRatio;

	blend = (-2.0f*wVratio+walkVel+rVratio2-sqrt(walkVel*walkVel-2.0f*walkVel*rVratio2+runVel*rVratio2*runWalkRatio*runWalkRatio+4.0f*(-walkVel*dVratio+wVratio*dVratio+rVratio2*desiredVel-rVratio2*dVratio)))/(2.0f*(walkVel-wVratio-rVratio+rVratio2));

	blend = hkMath::clamp(blend, 0.f, 1.f);
	runSpeed  = (1.0f-blend) * runWalkRatio + blend;
	walkSpeed = blend * (1.0f / runWalkRatio) + (1.0f - blend);	

}


hkpCharacterContext* HK_CALL CharacterUtils::createDefaultCharacterContext()
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

	hkpCharacterContext* characterProxyContext = new hkpCharacterContext(manager, HK_CHARACTER_ON_GROUND);
	manager->removeReference();
	return characterProxyContext;
}



void HK_CALL CharacterUtils::displayCharacterState(hkpCharacterContext* context, hkDemoEnvironment* env )
{

	// Rigid body controller state
	hkpCharacterStateType stateRBC = context->getState();
	char * stateStrRBC;

	switch (stateRBC)
	{
	case HK_CHARACTER_ON_GROUND:
		stateStrRBC = "On Ground";	break;
	case HK_CHARACTER_JUMPING:
		stateStrRBC = "Jumping"; break;
	case HK_CHARACTER_IN_AIR:
		stateStrRBC = "In Air"; break;
	case HK_CHARACTER_CLIMBING:
		stateStrRBC = "Climbing"; break;
	default:
		stateStrRBC = "Other";	break;
	}

	char buffer[255];
	hkString::snprintf(buffer, 255, "State: %s", stateStrRBC);
	env->m_textDisplay->outputText(buffer, 20, 450, 0xffffffff);
}


void HK_CALL CharacterUtils::setCharacterInput( const hkpCharacterRigidBody* characterRb, hkpCharacterInput& input, const hkVector4& gravity )
{
	//
	// Copy some values from the character controller
	//
	input.m_velocity = characterRb->getLinearVelocity();
	input.m_position = characterRb->getPosition();
	input.m_up = characterRb->m_up;
	input.m_characterGravity = gravity;

	//
	// Call get support and set the relevant values in the character input
	//

	hkpSurfaceInfo ground;
	// XXX this parameter should not be needed for check support
	hkStepInfo stepInfo;
	stepInfo.m_deltaTime = 1.f / 60.f;
	stepInfo.m_invDeltaTime = 60.f;
	characterRb->checkSupport( stepInfo, ground);

	input.m_isSupported = (ground.m_supportedState == hkpSurfaceInfo::SUPPORTED);
	input.m_surfaceNormal = ground.m_surfaceNormal;
	input.m_surfaceVelocity = ground.m_surfaceVelocity;
	input.m_surfaceMotionType = ground.m_surfaceMotionType;

}



SimpleCharacter::SimpleCharacter( const hkVector4& position, const hkVector4& up )
{
	createDefaultCharacterController( position, up );

	m_characterRbContext = CharacterUtils::createDefaultCharacterContext();
	HK_ASSERT( 0, up.isNormalized3() );
	m_currentAngle = HK_REAL_PI * 0.5f; 
	m_currentElevation = 0;
	m_gravity.setMul4(-16, up);
}


SimpleCharacter::SimpleCharacter( hkpCharacterRigidBody* characterRb, hkpCharacterContext* context )
{ 
	m_characterRb = characterRb; 
	m_characterRbContext = context;

	m_currentAngle = HK_REAL_PI * 0.5f; 
	m_currentElevation = 0;

	characterRb->addReference();
	context->addReference();

	m_gravity.setMul4(-16, characterRb->m_up);
	//m_hasUserControl = false;
}

void SimpleCharacter::createDefaultCharacterController( const hkVector4& position, const hkVector4& up, hkReal capsuleHeight, hkReal capsuleRadius )
{
	//  Create character
	{
		hkReal h = capsuleHeight * 0.5f - capsuleRadius;
		hkVector4 vertexA; vertexA.setMul4( h, up );
		hkVector4 vertexB; vertexB.setMul4( -h, up);
		hkpShape* characterShape = new hkpCapsuleShape(vertexA, vertexB, capsuleRadius);

		// Construct a character rigid body
		hkpCharacterRigidBodyCinfo info;
		info.m_shape = characterShape;
		info.m_position = position;
		info.m_up = up;

		m_characterRb = new hkpCharacterRigidBody( info );
		characterShape->removeReference();
	}
}

SimpleCharacter::~SimpleCharacter()
{	
	m_characterRb->removeReference();
	m_characterRbContext->removeReference();
}

void SimpleCharacter::update( hkReal timestep, const CharacterControls& controls, bool showForwardDirection )
{
	hkpCharacterInput input;
	hkpCharacterOutput output;

	input.m_atLadder = false;
	input.m_stepInfo.m_deltaTime = timestep;
	input.m_stepInfo.m_invDeltaTime = 1.0f / timestep;

	input.m_forward = controls.m_forward;
	input.m_inputLR = controls.m_straffeLeftRight;
	input.m_inputUD = controls.m_forwardBack;
	input.m_wantJump = controls.m_wantJump;

	CharacterUtils::setCharacterInput( m_characterRb, input, m_gravity );

	m_characterRbContext->update(input, output);		

	m_characterRb->setLinearVelocity(output.m_velocity, timestep);

	if (showForwardDirection)
	{
		hkVector4 start; start.setAdd4( m_characterRb->getPosition(), m_characterRb->m_up );
		hkVector4 end; end = start; end.addMul4( 1.f, controls.m_forward );
		HK_DISPLAY_LINE( start, end, hkColor::BLUE );
	}
}



FirstPersonCharacterCinfo::FirstPersonCharacterCinfo()
{
	m_position.set(0, 3, 0);
	m_direction.set(1,0,0);
	m_up.set(0,1,0);
	m_gravityStrength = 16;
	m_capsuleHeight = 2.0f;
	m_capsuleRadius = 0.6f;
	m_eyeHeight	= 0.7f;

	m_invertUpDown = false;
	m_verticalSensitivity = .1f;
	m_horizontalSensitivity = .1f;
	m_sensivityPadX = 0.2f;
	m_sensivityPadY = 0.2f;
	m_maxUpDownAngle = HK_REAL_PI / 6.f;
	m_canDetachFromCharacter = true;
	m_forwardBackwardSpeedModifier = 1.0f;
	m_leftRightSpeedModifier = 1.0f;

	m_numFramesPerShot = 5;
	m_characterRb = HK_NULL;
	m_context = HK_NULL;
}


FirstPersonCharacter::FirstPersonCharacter( FirstPersonCharacterCinfo& info, hkDemoEnvironment* env, hkpWorld* world  )
{
	if ( info.m_characterRb == HK_NULL )
	{
		createDefaultCharacterController( info.m_position, info.m_up, info.m_capsuleHeight, info.m_capsuleRadius );
	}
	else
	{
		m_characterRb->addReference();
	}

	if (info.m_context == HK_NULL)
	{
		m_characterRbContext = CharacterUtils::createDefaultCharacterContext();
	}
	else
	{
		m_characterRbContext->addReference();
	}

	HK_ASSERT( 0, info.m_up.isNormalized3() );
	m_currentAngle = HK_REAL_PI * 0.5f; 
	m_currentElevation = 0;
	info.m_direction.normalize3();

	{
		hkReal dot = info.m_up.dot3(info.m_direction);
		m_currentElevation = HK_REAL_PI / 2 - hkMath::acos(dot);
		if (m_currentElevation > info.m_maxUpDownAngle) m_currentElevation = info.m_maxUpDownAngle;
		if (m_currentElevation < -info.m_maxUpDownAngle) m_currentElevation = -info.m_maxUpDownAngle;

		info.m_direction.addMul4( -dot, info.m_up);
	}
	{
		if (info.m_direction.isOk3())
		{
			info.m_direction.normalize3();
			hkVector4 forward; forward.set(1,0,0);
			hkReal dot = forward.dot3(info.m_direction);
			m_currentAngle = hkMath::acos(dot);
		}
		else
		{
			m_currentAngle = 0;
		}

	}
	updateCamera( world, env->m_window );

	m_gravity.setMul4(-info.m_gravityStrength, info.m_up);
	m_gunCounter = 0;
	m_gunCounterRmb = 0;

	m_hasUserControl = true;
	env->m_mousePickingEnabled = false;
	m_makeOccludingObjectsTransparent = true;// Only used when view is behind character (currently code is disabled)

	m_invertUpDown = info.m_invertUpDown;
	m_verticalSensitivity = info.m_verticalSensitivity;
	m_horizontalSensitivity = info.m_horizontalSensitivity;
	m_sensivityPadX = info.m_sensivityPadX;
	m_sensivityPadY = info.m_sensivityPadY;
	m_eyeHeight = info.m_eyeHeight;

	m_maxUpDownAngle = info.m_maxUpDownAngle;
	m_canDetachFromCharacter = info.m_canDetachFromCharacter;
	m_numFramesPerShot = info.m_numFramesPerShot;
	m_forwardBackwardSpeedModifier = info.m_forwardBackwardSpeedModifier;
	m_leftRightSpeedModifier = info.m_leftRightSpeedModifier;

	m_currentGun = HK_NULL;

	m_env = env;
	m_world = world;
}

FirstPersonCharacter::~FirstPersonCharacter()
{
	if (m_currentGun)
	{
		m_currentGun->quitGun( m_world );
		m_currentGun->removeReference();
	}
}

void FirstPersonCharacter::getForwardDir( hkVector4& forward )
{
	forward.set(1,0,0);
	hkQuaternion currentOrient;
	currentOrient.setAxisAngle(m_characterRb->m_up, m_currentAngle);
	forward.setRotatedDir( currentOrient, forward );

	hkVector4 rotateAxis;
	rotateAxis.setCross(forward, m_characterRb->m_up);
	hkQuaternion upOrient;
	upOrient.setAxisAngle( rotateAxis, m_currentElevation );
	forward.setRotatedDir( upOrient, forward );
}

void FirstPersonCharacter::setForwardDir( const hkVector4& forward )
{
	hkReal sinUp = forward.dot3( m_characterRb->m_up );
	hkVector4 remaining = forward; 
	remaining.subMul4( sinUp, m_characterRb->m_up );
	hkReal cosUp = remaining.length3();
	m_currentElevation = hkMath::atan2fApproximation( sinUp, cosUp );

	hkVector4 X; X.set(1,0,0);
	hkVector4 R; R.setCross( m_characterRb->m_up, X );
	hkReal sinX = X.dot3(forward);
	hkReal cosX = R.dot3(forward);

	m_currentAngle = hkMath::atan2fApproximation( cosX, sinX );
}


hkpFirstPersonGun* FirstPersonCharacter::setGun( hkpFirstPersonGun* gun )
{
	if (gun)
	{
		gun->addReference();
	}
	if (m_currentGun)
	{
		m_currentGun->quitGun( m_world );
		m_currentGun->removeReference();
	}
	m_currentGun = gun;
	if (gun)
	{
		gun->initGun( m_world );
	}
	return gun;
}

void FirstPersonCharacter::update( hkReal timestep )
{
	if ( m_hasUserControl )
	{
		m_world->lock();
		CharacterControls controls;
		if ( m_hasUserControl )
		{
			setInputFromUserControls( m_env, controls );
		}
		else
		{
			controls.m_forward.setZero4();
			controls.m_forwardBack = 0;
			controls.m_straffeLeftRight = 0;
			controls.m_wantJump = false;
			controls.m_fire = false;
			controls.m_fireRmb = false;
		}

		if ( controls.m_fire && m_currentGun )
		{
			hkTransform viewTransform;
			this->getViewTransform( viewTransform );
			m_currentGun->fireGun( m_world, viewTransform );
		}

		SimpleCharacter::update( timestep, controls, false );
		updateCamera( m_world, m_env->m_window );

		if (m_currentGun)
		{
			hkpRigidBody* body = m_characterRb->m_character;
			hkTransform viewTransform; getViewTransform(viewTransform);
			m_currentGun->stepGun( timestep, m_world, body, viewTransform, controls.m_fire, controls.m_fireRmb );
		}
		m_world->unlock();
	}


	const hkgKeyboard& keyBoard = m_env->m_window->getKeyboard();
	if (keyBoard.wasKeyPressed('P') || keyBoard.wasKeyPressed('O') )
	{
		m_hasUserControl = !m_hasUserControl;
		if (m_hasUserControl)
		{
			// Snap mouse back to middle
			m_env->m_window->setMousePosition(m_env->m_window->getWidth() >> 1, m_env->m_window->getHeight() >> 1);
			m_env->m_mousePickingEnabled = false;

			// position rigid body at camera position
			hkgViewport* viewPort = m_env->m_window->getViewport(0);
			hkgCamera* c = viewPort->getCamera();

			hkVector4 position; position.setZero4();
			c->getFrom( &position(0) );
			hkVector4 to; to.setZero4();
			c->getTo( &to(0) );
			hkVector4 dir; dir.setSub4( to, position );
			dir.normalize3();

			{
				position.subMul4( m_eyeHeight, m_characterRb->m_up);
				hkpRigidBody* body = m_characterRb->m_character;
				body->setPosition( position );
				body->setLinearVelocity( hkVector4::getZero() );
			}
			setForwardDir( dir );
			if ( keyBoard.wasKeyPressed('O') )
			{
				m_gravity.setZero4();
			}
			else
			{
				m_gravity.setMul4( -16, m_characterRb->m_up );
			}
		}
		else
		{
			m_env->m_mousePickingEnabled = true;
		}
	}
}

void FirstPersonCharacter::setInputFromUserControls( hkDemoEnvironment* env, CharacterControls& controls )
{
	// Get user input data
	hkReal deltaAngle;
	hkReal deltaElevation;

	CharacterUtils::Sensivity sensivity;
	{
		sensivity.m_mouseX = m_horizontalSensitivity;
		sensivity.m_mouseY = m_verticalSensitivity;
		sensivity.m_padX = m_sensivityPadX;
		sensivity.m_padY = m_sensivityPadY;
	}

	CharacterUtils::getUserInputForCharacter(env, sensivity, deltaAngle, deltaElevation, controls.m_straffeLeftRight, controls.m_forwardBack );

	controls.m_straffeLeftRight *= m_leftRightSpeedModifier;
	controls.m_forwardBack      *= m_forwardBackwardSpeedModifier;

	if (!m_invertUpDown) 
	{
		deltaElevation = -deltaElevation;
	}

	m_currentAngle += deltaAngle;
	m_currentElevation += deltaElevation;
	
	if (m_currentElevation < -m_maxUpDownAngle) m_currentElevation = -m_maxUpDownAngle;
	if (m_currentElevation > m_maxUpDownAngle)	m_currentElevation = m_maxUpDownAngle;

	hkQuaternion currentOrient; currentOrient.setAxisAngle(m_characterRb->m_up, m_currentAngle);
 
	controls.m_wantJump = env->m_window->getKeyboard().wasKeyPressed( HKG_VKEY_SPACE );
	controls.m_wantJump == controls.m_wantJump || (env->m_gamePad->getButtonState() & HKG_PAD_BUTTON_1) != 0;

	controls.m_forward.set(1,0,0);
	controls.m_forward.setRotatedDir( currentOrient, controls.m_forward );

	controls.m_fire = false;
	if ( (env->m_window->getMouse().getButtonState() & HKG_MOUSE_LEFT_BUTTON) ||
		 (env->m_gamePad->getButtonState() & HKG_PAD_BUTTON_R1) )
	{
		if ( m_gunCounter-- < 0 )
		{
			m_gunCounter = 10;
			controls.m_fire = true;
		}
	}
	else
	{
		m_gunCounter = 0;
	}

	controls.m_fireRmb = false;
	if ( (env->m_window->getMouse().getButtonState() & HKG_MOUSE_RIGHT_BUTTON) ||
		 (env->m_gamePad->getButtonState() & HKG_PAD_BUTTON_R2) )
	{
		if ( m_gunCounterRmb-- < 0 )
		{
			m_gunCounterRmb = 20;
			controls.m_fireRmb = true;
		}
	}
	else
	{
		m_gunCounterRmb = 0;
	}
}

//// Objects colors
#define NORMAL_GRAY				hkColor::rgbFromChars( 192, 192, 192, 255 )
#define TRANSPARENT_GREY		hkColor::rgbFromChars( 192, 192, 192, 125 )

void FirstPersonCharacter::updateCamera( hkpWorld* world, hkgWindow* window )
{
	const hkReal height = m_eyeHeight; 

	hkVector4 from;
	from = m_characterRb->getPosition();
	from.addMul4(height, m_characterRb->m_up );

	hkVector4 forward;
	getForwardDir( forward );

	hkVector4 to;
	to.setAddMul4( from, forward, 4 );

/*
	hkVector4 to;
	to = m_characterRb->getPosition();
	to.addMul4(height, m_characterRb->m_up );


	hkVector4 dir;
	dir.setMul4( height, m_characterRb->m_up );
	dir.addMul4( -4.f, forward);

	hkVector4 from;
	from.setAdd4(to, dir);
*/
	// Make object in the line of sight transparent
	if (0)//(m_makeOccludingObjectsTransparent)
	{
		// Cast down to landscape to get an accurate position
		hkpWorldRayCastInput raycastIn;

		// Reverse direction for raycasting
		raycastIn.m_from = to;
		raycastIn.m_to = from;
		raycastIn.m_filterInfo = hkpGroupFilter::calcFilterInfo(0);

		hkpAllRayHitCollector collector;

		world->castRay( raycastIn, collector);

		for (int i = 0; i < m_transparentObjects.getSize(); ++i )
		{
			HK_SET_OBJECT_COLOR(m_transparentObjects[i], NORMAL_GRAY );
		}
		m_transparentObjects.clear();

		// Loop over all collected objects
		for(int i=0; i < collector.getHits().getSize();i++)
		{				
			hkpWorldRayCastOutput raycastOut = collector.getHits()[i];
			hkpRigidBody* rb = hkGetRigidBody(raycastOut.m_rootCollidable);
			if (rb && rb->isFixedOrKeyframed())
			{
				HK_SET_OBJECT_COLOR((hkUlong)raycastOut.m_rootCollidable, TRANSPARENT_GREY );

				m_transparentObjects.pushBack((hkUlong)raycastOut.m_rootCollidable);
			}
		}
	}

	window->getContext()->lock();
	for(int i = 0; i < window->getNumViewports(); ++i)
	{
		hkgViewport* viewPort = window->getViewport(i);
		hkgCamera* c = viewPort->getCamera();

		float upN[3];
		hkgVec3Copy(upN, &m_characterRb->m_up(0));
		hkgVec3Normalize(upN);
		// set up camera
		c->setFrom(&from(0));
		c->setTo(&to(0));
		c->setUp(upN);
		c->orthogonalize();
		c->computeModelView();
		c->computeProjection();
		//c->setHandednessMode(HKG_CAMERA_HANDEDNESS_MODE( rightHanded ? HKG_CAMERA_HANDEDNESS_RIGHT : HKG_CAMERA_HANDEDNESS_LEFT) );
		viewPort->setFlyModeUp( upN );
	}
	window->getContext()->unlock();
}

void FirstPersonCharacter::reset(hkpWorld* world)
{
	if (m_currentGun) m_currentGun->reset(world);
}

void FirstPersonCharacter::getViewTransform(hkTransform& viewTransformOut)
{
	const hkReal height = .7f; // XXX

	viewTransformOut.getTranslation().setAddMul4( m_characterRb->m_character->getPosition(), m_characterRb->m_up, height );

	// get rotation
	hkVector4 x; getForwardDir(x); x.normalize3();
	hkVector4 y; y = m_characterRb->m_up; //y.normalize3();
	hkVector4 z; z.setCross(x, y); z.normalize3();
	             y.setCross(z, x); y.normalize3();
	viewTransformOut.getRotation().setCols(x, y, z);
}

hkpRigidBody* FirstPersonCharacter::getRigidBody() 
{
	return m_characterRb->m_character; 
}



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
