/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/Physics/UseCase/PlanetGravity/PlanetGravityDemo.h>
#include <Demos/Physics/UseCase/PlanetGravity/LaunchPadListener.h>
#include <Demos/Physics/UseCase/PlanetGravity/SimpleGravityPhantom.h>
#include <Demos/Physics/UseCase/PlanetGravity/PhantomTrackAction.h>
#include <Demos/Physics/UseCase/PlanetGravity/TeleporterPhantomCallbackShape.h>

#include <Common/Base/Monitor/hkMonitorStream.h>
#include <Common/Base/System/Io/IStream/hkIStream.h>
#include <Common/Visualize/hkDebugDisplay.h>
#include <Demos/DemoCommon/Utilities/Character/CharacterUtils.h>
#include <Physics/Collide/Query/CastUtil/hkpWorldRayCastInput.h>
#include <Physics/Collide/Query/Collector/RayCollector/hkpAllRayHitCollector.h>
#include <Physics/Collide/Query/Collector/RayCollector/hkpClosestRayHitCollector.h>
#include <Physics/Collide/Shape/Convex/Capsule/hkpCapsuleShape.h>
#include <Physics/Collide/Shape/Misc/Bv/hkpBvShape.h>
#include <Physics/Dynamics/Constraint/Bilateral/PointToPath/hkpLinearParametricCurve.h>
#include <Physics/Dynamics/Constraint/Bilateral/LimitedHinge/hkpLimitedHingeConstraintData.h>
#include <Physics/Utilities/Serialize/hkpHavokSnapshot.h>
#include <Physics/Utilities/Serialize/hkpPhysicsData.h>
#include <Physics/Utilities/CharacterControl/CharacterRigidBody/hkpCharacterRigidBody.h>
#include <Physics/Utilities/CharacterControl/StateMachine/hkpDefaultCharacterStates.h>

#include <Graphics/Common/DisplayWorld/hkgDisplayWorld.h>
#include <Graphics/Common/Light/hkgLightManager.h>

hkpCharacterRigidBody* PlanetGravityDemo::m_characterRigidBody;
hkVector4 PlanetGravityDemo::m_worldUp;
hkVector4 PlanetGravityDemo::m_characterForward;

hkpWorld* PlanetGravityDemo::loadWorld( const char* path, hkpPhysicsData** physicsData, hkPackfileReader::AllocatedData** memData )
{
	hkIstream infile( path );
	HK_ASSERT( 0x215d080c, infile.isOk() );

	*physicsData = hkpHavokSnapshot::load( infile.getStreamReader(), memData );

	HK_ASSERT( 0, *physicsData != HK_NULL );

	// Ensure non-multithreaded simulation for non-multithreaded platforms

	hkpWorld* world = (*physicsData)->createWorld();
	world->setGravity( hkVector4::getZero() );

	//	Create a character rigid body object
	{
		// Construct a shape
		hkVector4 vertexA( 0.0f,  0.4f, 0.0f );
		hkVector4 vertexB( 0.0f, -0.4f, 0.0f );

		// Create a capsule to represent the character standing
		m_standShape = new hkpCapsuleShape( vertexA, vertexB, 0.6f );

		// Create a capsule to represent the character crouching
		// Note that we create the smaller capsule with the base at the same position as the larger capsule.		
		m_crouchShape = new hkpCapsuleShape( hkVector4::getZero(), vertexB, 0.6f );

		// Construct a character rigid body
		hkpCharacterRigidBodyCinfo info;
		info.m_mass = 80.0f;
		info.m_shape = m_standShape;

		info.m_maxForce = 8000.0f;
		info.m_position.set( 30.f, 0.f, 30.f );
		info.m_maxSlope = 45.0f * HK_REAL_DEG_TO_RAD;
		info.m_friction = 0.25f;
		m_characterRigidBody = new hkpCharacterRigidBody( info );
		world->addEntity( m_characterRigidBody->getRigidBody() );
		m_characterRigidBody->getRigidBody()->setRestitution( 0.0f );
	}

	// Create the Character state machine and context
	{
		hkpCharacterState* state;
		hkpCharacterStateManager* manager = new hkpCharacterStateManager();

		state = new hkpCharacterStateOnGround();
		manager->registerState( state,	HK_CHARACTER_ON_GROUND );

		static_cast<hkpCharacterStateOnGround*>( manager->getState( HK_CHARACTER_ON_GROUND ) )->setDisableHorizontalProjection( true );
		state->removeReference();

		state = new hkpCharacterStateInAir();
		manager->registerState( state,	HK_CHARACTER_IN_AIR );
		state->removeReference();

		state = new hkpCharacterStateJumping();
		manager->registerState( state,	HK_CHARACTER_JUMPING );
		state->removeReference();

		m_characterContext = new hkpCharacterContext( manager, HK_CHARACTER_IN_AIR );
		manager->removeReference();

		// Set character type
		m_characterContext->setCharacterType( hkpCharacterContext::HK_CHARACTER_RIGIDBODY );
	}

	return world;
}

// This function casts a set of small rays along a curved path.
// Leaving numSamplePoints as 0 will use however many points defined originally in the curve.
hkReal PlanetGravityDemo::castCurvedRay( hkpClosestRayHitCollector& output, const hkpLinearParametricCurve& curve, int numSamplePoints )
{
	hkReal t_min, t_max;
	t_min = curve.getStart();
	t_max = curve.getEnd();

	if( numSamplePoints == 0 )
	{
		numSamplePoints = curve.m_points.getSize();
	}

	// Sample the curve and do a cast at each sample point
	for( int i = 0; i < numSamplePoints - 1; i++ )
	{
		hkpWorldRayCastInput ray;

		hkReal difference = ( t_max - t_min );
		hkReal parametricValue1 = t_min + ( difference * ( i / static_cast<hkReal>( numSamplePoints ) ) );
		hkReal parametricValue2 = t_min + ( difference * ( (i + 1.0f) / static_cast<hkReal>( numSamplePoints ) ) );

		curve.getPoint( parametricValue1, ray.m_from );
		curve.getPoint( parametricValue2, ray.m_to );

		HK_DISPLAY_LINE( ray.m_from, ray.m_to, hkColor::RED );

		m_world->castRay( ray, output );
		if( output.hasHit() )
		{
			return parametricValue1 + ( ( parametricValue2 - parametricValue1 ) * output.getHit().m_hitFraction );
		}

		output.reset();
	}
	return -1.0f;
}

hkDemo::Result PlanetGravityDemo::stepDemo()
{
	// Update lighting
	{
		// Update the light source to be at the camera
		float position[3];
		m_cameraPosition.store3( position );
		m_flashLight->setPosition( position );

		// Update the light direction to be pointing toward the character controller rigid body
		hkVector4 directionVector;
		directionVector.setSub4( m_cameraPosition, m_characterRigidBody->getPosition() );
		directionVector.mul4( -1.0f );
		directionVector.normalize3();

		float direction[3];
		directionVector.store3( direction );
		m_flashLight->setDirection( direction );
	}

	// Detach the camera from the character when P is pressed.
	if( m_env->m_window->getKeyboard().wasKeyPressed('P') )
	{
		m_detachedCamera = !m_detachedCamera;
	}

	// Update turrets
	for( int i = 0; i < m_turrets.getSize(); i++ )
	{
		Turret& turret = m_turrets[i];
		turret.cooldown -= m_timestep;
		
		// Make the turret spin
		turret.hinge->setMotorTargetAngle( turret.hinge->getMotorTargetAngle() + ( m_timestep / 5.f ) );

		// Bail out if the turret is "hot"
		if( turret.cooldown > 0.0f )
		{
			continue;
		}

		// Generate a curved raycast and shoot the ray
		// This has to be done every time-step as it's in world-space
		{
			const hkReal radius = 14.8f;
			hkRotation rot;
			hkVector4 offset;
			hkVector4 turretDown;
			
			rot.set( turret.turretRigidBody->getRotation() );
			offset = turret.turretRigidBody->getPosition();
			turretDown.setMul4( -1.0f, rot.getColumn(2) );

			hkpLinearParametricCurve myCurve;
			// Move the ray's source a little up so it's coming from the center of the barrel
			hkTransform localTransform( hkQuaternion::getIdentity(), hkVector4( 0.0f, 0.0f, 0.7f ) );

			// Create a curve of 20 points
			for( int j = 0; j < 20; j++ )
			{
				hkReal angle = HK_REAL_PI * static_cast<hkReal>(j) / 15.0f;

				hkVector4 newPoint( radius * 2.0f * sin( angle ),
									0.0f,
									radius * 2.0f * cos( angle ) );

				newPoint.setTransformedPos( localTransform, newPoint );
				newPoint.setTransformedPos( turret.turretRigidBody->getTransform(), newPoint );
				newPoint.addMul4( radius * 2.0f, turretDown );

				myCurve.addPoint( newPoint );
			}

			// We only need the closest hit (as our lasers can't pass through objects)
			//  so hkpClosestRayHitCollector is used.
			hkpClosestRayHitCollector raycastOutput;
			hkReal t = castCurvedRay( raycastOutput, myCurve, 20 );

			// Apply a large force to the closest rb we hit, along the tangent at the colliding point
			if( raycastOutput.hasHit() )
			{
				hkpRigidBody* hitRb = hkGetRigidBody( raycastOutput.getHit().m_rootCollidable );

				if( hitRb->getMotionType() != hkpMotion::MOTION_FIXED )
				{
					hkVector4 tangent;
					myCurve.getTangent( t, tangent );
					tangent.mul4( 15000.0f );

					applyScaledLinearImpulse( hitRb, tangent );
					turret.cooldown = 3.0f;
				}
			}
		}
	}

	m_world->markForWrite();

	// Update the character context
	m_characterRigidBody->m_up = m_worldUp;
	hkReal posX = 0.0f;
	hkReal posY = 0.0f;
	if( !m_detachedCamera )
	{
		float deltaAngle;
		CharacterUtils::getUserInputForCharacter( m_env, deltaAngle, posX, posY );

		if( ( ( hkMath::fabs( posX ) < HK_REAL_MAX ) && ( hkMath::fabs( posY ) < HK_REAL_MAX ) ) && ( posX || posY ) )
		{
			// find new orientation in local space
			hkVector4 newForward( -posY, 0.0f, -posX );
			hkVector4 absoluteForward( 1.0f, 0.0f, 0.0f );
			hkReal characterAngle = hkMath::acos( absoluteForward.dot3( newForward ) );

			// Calculate cross product to get sign of rotation.
			hkVector4 crossProduct;
			crossProduct.setCross( absoluteForward, newForward );

			if( crossProduct(1) < 0.0f )
			{
				characterAngle *= -1.0f;
			}

			// Rotate the character's rigid body to face in the direction it's moving
			hkRotation newRotation;
			newRotation.setAxisAngle( m_worldUp, characterAngle );
 			m_characterForward.setRotatedDir( newRotation, m_cameraForward );
 			m_characterForward.normalize3();
		}

		// Rotate the camera's forward vector based on world up vector and mouse movement
		if( deltaAngle != 0.0f && m_characterRigidBody->getRigidBody()->getRotation().hasValidAxis() )
		{
			hkRotation newRotation;
			newRotation.setAxisAngle( m_worldUp, deltaAngle );
 			m_cameraForward.setRotatedDir( newRotation, m_cameraForward );
 			m_cameraForward.normalize3();
		}
	}

	HK_TIMER_BEGIN( "set character state", HK_NULL );
	hkpCharacterInput input;
	hkpCharacterOutput output;
	{
		input.m_atLadder = false;
		input.m_inputLR = posX;
		input.m_inputUD = posY;

		if( m_detachedCamera )
		{
			input.m_wantJump = false;
		}
		else
		{
			input.m_wantJump = m_env->m_window->getMouse().wasButtonPressed( HKG_MOUSE_LEFT_BUTTON )
							|| m_env->m_gamePad->wasButtonPressed( HKG_PAD_BUTTON_1 );
		}

		// Check that we have a valid rotation. Probably won't for the first couple of frames.
		if( !( m_characterRigidBody->getRigidBody()->getRotation().hasValidAxis() ) )
		{
			input.m_up = hkVector4( 0.0f, 0.0f, 1.0f );
			input.m_forward = m_cameraForward;
		}
		else
		{
			input.m_up = m_worldUp;

			// Recalculate m_forward so it's perpendicular to m_worldUp
			hkVector4 newRot;
			newRot.setCross( m_cameraForward, m_worldUp );
			m_cameraForward.setCross( m_worldUp, newRot );

			// Display character's current heading
			hkRotation characterRotation;
			characterRotation.set( m_characterRigidBody->getRigidBody()->getRotation() );
			HK_DISPLAY_ARROW( m_characterRigidBody->getPosition(), characterRotation.getColumn(0), hkColor::LIMEGREEN );

			input.m_forward = m_cameraForward;
		}

		hkStepInfo stepInfo;
		stepInfo.m_deltaTime = m_timestep;
		stepInfo.m_invDeltaTime = 1.0f / m_timestep;

		input.m_stepInfo = stepInfo;
		input.m_characterGravity.setMul4( -20.0f, m_worldUp );

		input.m_velocity = m_characterRigidBody->getRigidBody()->getLinearVelocity();
		input.m_position = m_characterRigidBody->getRigidBody()->getPosition();
		{
			hkpSurfaceInfo ground;
			m_characterRigidBody->checkSupport( stepInfo, ground );

			// Avoid accidental state changes (Smooth movement on stairs)
			// During transition supported->unsupported continue to return N-frames hkpSurfaceInfo data from previous supported state
			{
				// Number of frames to skip (continue with previous hkpSurfaceInfo data)
				const int skipFramesInAir = 6;

				if( input.m_wantJump )
				{
					m_framesInAir = skipFramesInAir;
				}

				hkpSurfaceInfo* currInfo;
				if( ground.m_supportedState != hkpSurfaceInfo::SUPPORTED )
				{
					if( m_framesInAir < skipFramesInAir )
					{
						input.m_isSupported = true;
						currInfo = m_previousGround;
					}
					else
					{
						input.m_isSupported = false;
						currInfo = &ground;
					}

					m_framesInAir++;
				}
				else
				{
					input.m_isSupported = true;
					currInfo = &ground;

					m_previousGround->set( ground );

					// reset old number of frames
					if( m_framesInAir > skipFramesInAir )
					{
						m_framesInAir = 0;
					}			
				}

				input.m_surfaceNormal = currInfo->m_surfaceNormal;
				input.m_surfaceVelocity = currInfo->m_surfaceVelocity;
				input.m_surfaceMotionType = currInfo->m_surfaceMotionType;
			}
		}

		HK_TIMER_END();
	}

	// Apply the character state machine
	{
		HK_TIMER_BEGIN( "update character state", HK_NULL );

		m_characterContext->update( input, output );

		HK_TIMER_END();
	}

	//Apply the player character controller
	{
		HK_TIMER_BEGIN( "simulate character", HK_NULL );

		// Set output velocity from state machine into character rigid body
		m_characterRigidBody->setLinearVelocity( output.m_velocity, m_timestep );

		HK_TIMER_END();

		m_world->unmarkForWrite();
	}

	// Rotate the character
	{
		hkRotation newOrientation;
		newOrientation.getColumn(0) = m_characterForward;
		newOrientation.getColumn(1) = m_worldUp;
		newOrientation.getColumn(2).setCross( newOrientation.getColumn(0), newOrientation.getColumn(1) );
		newOrientation.renormalize();
		
		reorientCharacter( newOrientation );
	}

	// Step the world
	hkDefaultPhysicsDemo::stepDemo();

	// Display state
	{
		hkpCharacterStateType state = m_characterContext->getState();
		char* stateStr;

		switch( state )
		{
			case HK_CHARACTER_ON_GROUND:
			{
				stateStr = "On Ground";
				break;
			}
			case HK_CHARACTER_JUMPING:
			{
				stateStr = "Jumping";
				break;
			}
			case HK_CHARACTER_IN_AIR:
			{
				stateStr = "In Air";
				break;
			}
			default:
			{
				stateStr = "Other";
				break;
			}
		}

		char buffer[255];
		hkString::snprintf( buffer, 255, "State : %s", stateStr );
		m_env->m_textDisplay->outputText( buffer, 20.f, 270.f, 0xffffffff );
	}

	//
	// Handle crouching (only for capsule)
	//
	if( !m_detachedCamera )
	{
		m_world->markForWrite();
		hkBool wantCrouch = ( m_env->m_window->getMouse().getButtonState() & HKG_MOUSE_RIGHT_BUTTON )
						 || ( m_env->m_gamePad->getButtonState() & HKG_PAD_BUTTON_2 );

		hkBool isCrouching = ( m_characterRigidBody->getRigidBody()->getCollidable()->getShape() == m_crouchShape );


		// We want to stand
		if( isCrouching && !wantCrouch )
		{
			m_characterRigidBody->getRigidBody()->setShape( m_standShape );
		}

		// We want to crouch
		else if( !isCrouching && wantCrouch )
		{
			m_characterRigidBody->getRigidBody()->setShape( m_crouchShape );
		}

		m_world->unmarkForWrite();
	}

	// Transparent camera handling
	if( !m_detachedCamera )
	{
		m_world->markForWrite();
		handleCamera();
		m_world->unmarkForWrite();
	}

	return hkDemo::DEMO_OK;
}

void PlanetGravityDemo::handleCamera()
{
	const hkReal distanceToCamera = 20.0f;
	const hkReal slack = 1.15f;
	const hkReal gain = 6.0f;
	const hkVector4& cameraTarget = m_characterRigidBody->getPosition();

	// A trial camera direction vector. This may not be the final vector.
	hkVector4 trialDirection;
	trialDirection.setMul4( distanceToCamera, m_worldUp );

	// The trial position of the camera based on the trial vector
	hkVector4 cameraSource;
	cameraSource.setAdd4( cameraTarget, trialDirection );

	// Find an interpolated position between the current camera position and trial position.
	{
		hkVector4 cameraDifference;
		cameraDifference.setSub4( cameraSource, m_cameraPosition );
		cameraDifference.mul4( m_timestep * gain * 4.0f );
		
		hkVector4 possibleCameraPosition( m_cameraPosition );
		possibleCameraPosition.add4( cameraDifference );

		hkVector4 actualDirection;
		actualDirection.setSub4( possibleCameraPosition, cameraTarget );

		// If the Move the cameraSource based on the newly calculated direction if we're too far
		if( trialDirection.dot3(actualDirection) > ( ( distanceToCamera * distanceToCamera ) / slack ) )
		{
			cameraSource = possibleCameraPosition;
		}
	}

	// Run a second iteration of interpolation
	{
		hkVector4 cameraDifference;
		cameraDifference.setSub4( cameraSource, m_cameraPosition );
		cameraDifference.mul4( m_timestep * gain / 4.0f );

		hkVector4 originalCameraVector;
		originalCameraVector.setSub4( m_cameraPosition, cameraTarget );

		hkVector4 normalizedOriginalCameraVector = originalCameraVector;
		normalizedOriginalCameraVector.normalize3();

		// cameraAngle is the dot product so it's actually cos(angle)
		hkReal cameraAngle = normalizedOriginalCameraVector.dot3( m_worldUp );

		// Move the camera if it's too far to the side, too far, or too close
		if( cameraAngle < 0.83f || originalCameraVector.length3() > distanceToCamera * slack ||
			originalCameraVector.length3() < distanceToCamera / slack )
		{
			m_cameraPosition.add4( cameraDifference );
		}
	}

	// Make object in line of sight transparent

	//{
	//	// Cast down to landscape to get an accurate position
	//	hkpWorldRayCastInput raycastIn;

	//	// Reverse direction for collision detection
	//	raycastIn.m_from = cameraTarget;
	//	raycastIn.m_to = m_cameraPosition;
	//	raycastIn.m_filterInfo = hkpGroupFilter::calcFilterInfo(0);

	//	hkpClosestRayHitCollector collector;

	//	m_world->castRay( raycastIn, collector);

	//	if( collector.hasHit())
	//	{
	//		hkpWorldRayCastOutput raycastOut = collector.getHit();
	//		hkVector4 rayVector;
	//		rayVector.setInterpolate4(raycastIn.m_from, raycastIn.m_to, raycastOut.m_hitFraction);
	//		HK_DISPLAY_ARROW(raycastIn.m_from, rayVector, hkColor::RED);

	//		cameraSource.setAdd4(cameraTarget, rayVector);
	//		m_cameraPosition = rayVector;
	//	}
	//}
	

	// Calculate the target camera up vector
	hkVector4 cameraUpDifference;
	cameraUpDifference.setSub4( m_cameraForward, m_cameraUp );

	// Interpolate a position so smooth the movement out
	cameraUpDifference.mul4( m_timestep * gain );
	m_cameraUp.add4( cameraUpDifference );

	setupDefaultCameras( m_env, m_cameraPosition, cameraTarget, m_cameraUp, 1.0f );
}

hkBool PlanetGravityDemo::reorientCharacter( const hkRotation& rotation )
{
	const hkReal gain = 0.5f;

	const hkQuaternion& currentOrient = m_characterRigidBody->getRigidBody()->getRotation();

	hkQuaternion desiredOrient;
	desiredOrient.set( rotation );

	hkVector4 angle;
	hkVector4 angularVelocity;
	currentOrient.estimateAngleTo( desiredOrient, angle );
	angularVelocity.setMul4( gain / m_timestep, angle );

	m_characterRigidBody->getRigidBody()->markForWrite();
	m_characterRigidBody->setAngularVelocity( angularVelocity );
	m_characterRigidBody->getRigidBody()->unmarkForWrite();

	return true;
}

PlanetGravityDemo::PlanetGravityDemo( hkDemoEnvironment* env )
: hkDefaultPhysicsDemo(env)
{
	hkString filename; // We have a different binary file depending on the compiler and platform
	filename.printf( "Resources/Physics/Levels/planetgravity_L%d%d%d%d.hkx",
					 hkStructureLayout::HostLayoutRules.m_bytesInPointer,
					 hkStructureLayout::HostLayoutRules.m_littleEndian ? 1 : 0,
					 hkStructureLayout::HostLayoutRules.m_reusePaddingOptimization? 1 : 0,
					 hkStructureLayout::HostLayoutRules.m_emptyBaseClassOptimization? 1 : 0 );
	hkIstream infile( filename.cString() );
	HK_ASSERT( 0x215d080c, infile.isOk() );

	hkpWorld::IgnoreForceMultithreadedSimulation ignoreForceMultithreaded;

	// Disable warning: 'm_contactRestingVelocity not set, setting it to REAL_MAX, so that the new collision restitution code will be disabled'
	hkError::getInstance().setEnabled( 0xf03243ed, false );

	// Load the startup scene
	{
		m_worldUp.set( 0.0f, 0.0f, 1.0f );
		m_characterForward.set( 1.0f, 0.0f, 0.0f );

		// Initialize hkpSurfaceInfo for storing the old ground info
		m_previousGround = new hkpSurfaceInfo();
		m_framesInAir = 0;

		m_cameraForward.set( 1.0f, 0.0f, 0.0f );
		m_cameraUp = m_cameraForward;
		m_cameraPosition.set( 20.0f, 1.0f, 35.0f );

		m_detachedCamera = false;

		// Load the world.
		m_world = loadWorld( filename.cString(), &m_physicsData, &m_loadedData );
		m_world->markForWrite();

		setupDefaultCameras( env, m_cameraPosition, m_characterRigidBody->getPosition(), m_worldUp );

		// Setup graphics
		setupGraphics();
		forceShadowState(false); // Disable shadows
		setupSkyBox(env);

		{
			removeLights( m_env );
			float v[] = { 0.941f, 0.941f, 0.784f };
			m_flashLight = hkgLight::create();
			m_flashLight->setType( HKG_LIGHT_DIRECTIONAL );
			m_flashLight->setDiffuse( v );
			v[0] = 0;
			v[1] = 1;
			v[2] = -0.5;
			m_flashLight->setDirection( v );
			v[0] = 0;
			v[1] = -1000;
			v[2] = 0;
			m_flashLight->setPosition( v );
			m_flashLight->setDesiredEnabledState( true );
			env->m_displayWorld->getLightManager()->addLight( m_flashLight );
			env->m_displayWorld->getLightManager()->computeActiveSet( HKG_VEC3_ZERO );
		}

		// Set up the collision filter
		{
			hkpGroupFilter* filter = new hkpGroupFilter();
			filter->disableCollisionsBetween(1, 1);
			m_world->setCollisionFilter(filter);
			filter->removeReference();
		}

		// Go through all loaded rigid bodies
		for( int i = 0; i < m_physicsData->getPhysicsSystems().getSize(); i++ )
		{
			const hkArray<hkpRigidBody*>& bodies = m_physicsData->getPhysicsSystems()[i]->getRigidBodies();
			for( int j = 0; j < bodies.getSize(); j++ )
			{
				hkString rbName( bodies[j]->getName() );

				// If the rb is a planet (name is "planet*")
				if( rbName.beginsWith( "planet" ) )
				{
					// If the body is a representation of a gravitational field (name: "*GravField"),
					//  remove it from the simulation.
					if( rbName.endsWith( "GravField" ) )
					{
						m_world->removeEntity( bodies[j] );
					}
					// Otherwise, it's actually a planet.
					else
					{
						hkAabb currentAabb;
						const hkpCollidable* hullCollidable = HK_NULL;

						// Find the planet's gravity field
						hkpRigidBody* planetRigidBody = bodies[j];
						hkString gravFieldRbName;
						gravFieldRbName.printf( "%sGravField", rbName.cString() );
						hkpRigidBody* gravFieldRigidBody = m_physicsData->findRigidBodyByName( gravFieldRbName.cString() );

						// If there's a GravField rigid body, then grab its collidable to be used for gravity calculation.
						if( gravFieldRigidBody )
						{
							hullCollidable = gravFieldRigidBody->getCollidable();
							gravFieldRigidBody->getCollidable()->getShape()->getAabb( gravFieldRigidBody->getTransform(), 0.0f, currentAabb );
						}
						else
						{
							planetRigidBody->getCollidable()->getShape()->getAabb( planetRigidBody->getTransform(), 0.0f, currentAabb );
						}

						// Scale up the planet's gravity field's AABB so it goes beyond the planet
						hkVector4 extents;
						extents.setSub4( currentAabb.m_max, currentAabb.m_min );
						hkInt32 majorAxis = extents.getMajorAxis();
						hkReal maxExtent = extents( majorAxis );
						maxExtent *= 0.4f;

						// Scale the AABB's extents
						hkVector4 extension;
						extension.setAll( maxExtent );
						currentAabb.m_max.add4( extension );
						currentAabb.m_min.sub4( extension );

						// Attach a gravity phantom to the planet so it can catch objects which come close
						SimpleGravityPhantom* gravityAabbPhantom = new SimpleGravityPhantom( planetRigidBody, currentAabb, hullCollidable );
						m_world->addPhantom( gravityAabbPhantom );
						gravityAabbPhantom->removeReference();

						// Add a tracking action to the phantom so it follows the planet. This allows support for non-fixed motion type planets
						if (planetRigidBody->getMotion()->getType() != hkpMotion::MOTION_FIXED)
						{
							PhantomTrackAction* trackAction = new PhantomTrackAction( planetRigidBody, gravityAabbPhantom );
							m_world->addAction( trackAction );
							trackAction->removeReference();
						}
					}
				}
				// if the rigid body is a launchpad (name: "launchPadSource*")
				else if( rbName.beginsWith("launchPadSource" ) )
				{
					hkString targetName;

					// Find launchpad "target" (used to calculate trajectory when launching)
					targetName.printf( "launchPadTarget%s", rbName.substr( hkString::strLen("launchPadSource") ).cString() );
					hkpRigidBody* target = m_physicsData->findRigidBodyByName( targetName.cString() );
					HK_ASSERT2( 0x0, target, "All launchPadSource rigid bodies must have associated launchPadTargets." );

					// Add a collision listener to the launchpad so it can apply forces to colliding rbs
					LaunchPadListener* launchPadListener = new LaunchPadListener( target->getPosition() );
					bodies[j]->addCollisionListener( launchPadListener );
					bodies[j]->setMotionType( hkpMotion::MOTION_FIXED );

					HK_SET_OBJECT_COLOR( reinterpret_cast<hkUlong>( bodies[j]->getCollidable() ), hkColor::RED );

					m_world->removeEntity( target );
				}
				// A "basic" launchpad just applies a force in the direction of the collision normal
				else if( rbName.beginsWith( "launchPadBasic" ) )
				{
					LaunchPadListener* launchPadListener = new LaunchPadListener( bodies[j]->getMass() );
					bodies[j]->addCollisionListener( launchPadListener );
					bodies[j]->setMotionType( hkpMotion::MOTION_FIXED );

					HK_SET_OBJECT_COLOR( reinterpret_cast<hkUlong>( bodies[j]->getCollidable() ), hkColor::RED );
				}
				else if( rbName.beginsWith( "teleporterSource" ) )
				{
					hkString targetName;

					// Find the teleportation destination of the teleporter
					targetName.printf( "teleporterTarget%s", rbName.substr( hkString::strLen("teleporterSource") ).cString() );
					hkpRigidBody* target = m_physicsData->findRigidBodyByName( targetName.cString() );
					HK_ASSERT2( 0, target, "All teleporterSource rigid bodies must have associated teleporterTargets." );

					// Replace the rb with a callback shape phantom. Colliding rbs will be teleported to the destination.
					TeleporterPhantomCallbackShape* phantomCallbackShape = new TeleporterPhantomCallbackShape( target->getTransform() );
					hkpBvShape* phantom = new hkpBvShape( bodies[j]->getCollidable()->getShape(), phantomCallbackShape );
					phantomCallbackShape->removeReference();
					bodies[j]->getCollidable()->getShape()->removeReference();
					bodies[j]->setShape( phantom );
					phantom->removeReference();

					m_world->removeEntity( target );
				}
				else if( rbName.beginsWith( "TurretTop" ) )
				{
					// Create a place to store state information for this turret.
					Turret& turret = m_turrets.expandOne();
					turret.constraint = bodies[j]->getConstraint(0);
					turret.hinge = static_cast<hkpLimitedHingeConstraintData*>( const_cast<hkpConstraintData*>( turret.constraint->getData() ) );
					turret.turretRigidBody = bodies[j];
					turret.cooldown = 0.0f;

					// Allow the hinge to spin infinitely and start the motor up
					turret.hinge->disableLimits();
					turret.hinge->setMotorActive( turret.constraint, true );

					// Do not allow the turret's simulation island deactivate.
					//  If it does, it will stop spinning.
					turret.turretRigidBody->setDeactivator( hkpRigidBodyDeactivator::DEACTIVATOR_NEVER );
				}

				// Update collision filter so that needless CollColl3 agents are not created.
				// For example, turrets  and geometry marked as "static" (such as the swing)
				//  should never collide with a planet, nor each other.
				if(  ( rbName.beginsWith( "planet" ) && !rbName.endsWith( "GravField" ) )
					|| rbName.beginsWith( "Turret" )
					|| rbName.endsWith( "_static" ) )
				{
					bodies[j]->setCollisionFilterInfo( hkpGroupFilter::calcFilterInfo( 1 ) );

					// Destroy or create agents (according to new quality type). This also removes Toi events.
					m_world->updateCollisionFilterOnEntity(bodies[j], HK_UPDATE_FILTER_ON_ENTITY_FULL_CHECK, HK_UPDATE_COLLECTION_FILTER_PROCESS_SHAPE_COLLECTIONS);
				}
			}
		}

		m_world->unmarkForWrite();
	}
}

void PlanetGravityDemo::cleanupWorld()
{
	m_world->markForWrite();
	m_world->removeReference();
	m_world = HK_NULL;

	m_physicsData->removeReference();
	m_loadedData->removeReference();
}

PlanetGravityDemo::~PlanetGravityDemo()
{
	m_world->markForWrite();
	m_characterRigidBody->removeReference();

	m_standShape->removeReference();
	m_crouchShape->removeReference();

	delete m_characterContext;
	delete m_previousGround;

	m_world->unmarkForWrite();
	m_flashLight->release();

	cleanupWorld();
}

// Apply a force which is scaled depending on whether or not it's our character controller.
// The character controller's special gravity implementation means that it needs a greater
//  force to get the same effect.
void PlanetGravityDemo::applyScaledLinearImpulse( hkpRigidBody* rb, hkVector4& impulse )
{
	if( rb != m_characterRigidBody->getRigidBody() )
	{
		impulse.mul4( 0.3f );
	}

	rb->applyLinearImpulse( impulse );
}

#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = "This demo shows how to use the rigid body character controller and actions to create worlds with local gravity.\n"  \
								 "Use \224 \225 \226 \227 to control the character.\n" \
								 "Left click = Jump   /   Right click = Crouch";

HK_DECLARE_DEMO( PlanetGravityDemo, HK_DEMO_TYPE_PHYSICS, "Shows how to create local gravity fields.", helpString );

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
