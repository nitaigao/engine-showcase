#include "PhysicsSystemCharacterComponent.h"

#include <Ogre.h>

#include <sstream>
#include "../Logging/Logger.h"

#include "../io/FileBuffer.hpp"
#include "../io/FileManager.h"

#include "../Geometry/GeometrySystemComponent.h"
#include "../Input/InputSystemComponent.h"

#include <Common/Base/System/hkBaseSystem.h>
#include <Common/Base/Memory/hkThreadMemory.h>
#include <Common/Base/Memory/Memory/Pool/hkPoolMemory.h>
#include <Common/Base/System/Error/hkDefaultError.h>
#include <Common/Base/System/Io/IStream/hkIStream.h>
#include <Common/Base/Monitor/hkMonitorStream.h>
#include <Common/Serialize/Packfile/Binary/hkBinaryPackfileReader.h>
#include <Common/Serialize/Util/hkRootLevelContainer.h>
#include <Physics/Dynamics/Entity/hkpRigidBody.h>
#include <Physics/Utilities/Serialize/hkpPhysicsData.h>
#include <Physics/Collide/Shape/Convex/Capsule/hkpCapsuleShape.h>

void PhysicsSystemCharacterComponent::Initialize( SystemPropertyList properties )
{
	hkpCharacterStateManager* characterManager = new hkpCharacterStateManager( );

	hkpCharacterState* state = new hkpCharacterStateOnGround( );
	characterManager->registerState( state, HK_CHARACTER_ON_GROUND );
	state->removeReference( );

	state = new hkpCharacterStateInAir( );
	characterManager->registerState( state, HK_CHARACTER_IN_AIR );
	state->removeReference( );

	state = new hkpCharacterStateJumping( );
	characterManager->registerState( state,	HK_CHARACTER_JUMPING );
	state->removeReference( );

	state = new hkpCharacterStateClimbing( );
	characterManager->registerState( state,	HK_CHARACTER_CLIMBING );
	state->removeReference( );

	_characterContext = new hkpCharacterContext( characterManager, HK_CHARACTER_ON_GROUND );
	characterManager->removeReference( );

	_characterContext->setCharacterType( hkpCharacterContext::HK_CHARACTER_RIGIDBODY );

	hkpCharacterRigidBodyCinfo characterInfo;
	characterInfo.m_mass = 100.0f;
	characterInfo.m_maxForce = 1000.0f;
	characterInfo.m_maxSlope = 70.0f * HK_REAL_DEG_TO_RAD;
	characterInfo.m_up = MathVector3::Up( ).AshkVector4( );
	characterInfo.m_shape = new hkpCapsuleShape( hkVector4( 0.0f, 0.4f, 0.0f ), hkVector4( 0.0f, -0.4f, 0.0f ), 0.6f );

	_characterBody = new hkpCharacterRigidBody( characterInfo );

	_body = _characterBody->getRigidBody( );
	_body->setUserData( _componentId );
	_scene->GetWorld( )->addEntity( _body );

	_characterInput.m_wantJump = false;
	_characterInput.m_atLadder = false;
	_characterInput.m_inputLR = 0.0f;
	_characterInput.m_inputUD = 0.0f;

	_characterInput.m_up = MathVector3::Up( ).AshkVector4( );
	_characterInput.m_forward = MathVector3::Forward( ).AshkVector4( );
	_characterInput.m_characterGravity = hkVector4( 0.0f, -16.0f, 0.0f );
}

PhysicsSystemCharacterComponent::~PhysicsSystemCharacterComponent()
{
	_characterBody->removeReference( );
	delete _characterContext;
	delete _previousGround;
}

void PhysicsSystemCharacterComponent::Update( float deltaMilliseconds )
{
	float mouseSmooth = 0.15f;

	_characterInput.m_stepInfo.m_deltaTime = deltaMilliseconds;
	_characterInput.m_stepInfo.m_invDeltaTime = 1.0f / deltaMilliseconds;

	_characterInput.m_velocity = _characterBody->getRigidBody( )->getLinearVelocity( );
	_characterInput.m_position = _characterBody->getRigidBody( )->getPosition( );

	hkQuaternion rotationDelta;
	rotationDelta.setAxisAngle( MathVector3::Up( ).AshkVector4( ), -_mouseXDelta * deltaMilliseconds * mouseSmooth );
	_mouseXDelta = 0;

	hkQuaternion rotation;
	rotation = _characterBody->getRigidBody( )->getRotation( );
	rotation.mul( rotationDelta );

	_characterBody->getRigidBody( )->setRotation( rotation );
	_characterInput.m_forward.set( 0.0f, 0.0f, 1.0f );
	_characterInput.m_forward.setRotatedDir( rotation, _characterInput.m_forward );

	hkpSurfaceInfo ground;
	_characterBody->checkSupport( _characterInput.m_stepInfo, ground );

	const int skipFramesInAir = 10; 

	if (_characterInput.m_wantJump)
	{
		_framesInAir = skipFramesInAir;
	}

	if ( ground.m_supportedState != hkpSurfaceInfo::SUPPORTED )
	{
		if (_framesInAir < skipFramesInAir)
		{
			_characterInput.m_isSupported = true;
			_characterInput.m_surfaceNormal = _previousGround->m_surfaceNormal;
			_characterInput.m_surfaceVelocity = _previousGround->m_surfaceVelocity;
			_characterInput.m_surfaceMotionType = _previousGround->m_surfaceMotionType;
		}
		else
		{
			_characterInput.m_isSupported = false;
			_characterInput.m_surfaceNormal = ground.m_surfaceNormal;
			_characterInput.m_surfaceVelocity = ground.m_surfaceVelocity;	
			_characterInput.m_surfaceMotionType = ground.m_surfaceMotionType;
		}			

		_framesInAir++;
	}
	else
	{
		_characterInput.m_isSupported = true;
		_characterInput.m_surfaceNormal = ground.m_surfaceNormal;
		_characterInput.m_surfaceVelocity = ground.m_surfaceVelocity;
		_characterInput.m_surfaceMotionType = ground.m_surfaceMotionType;

		_previousGround->set(ground);

		if (_framesInAir > skipFramesInAir)
		{
			_framesInAir = 0;
		}			
	}

	hkpCharacterOutput output;
	_characterContext->update( _characterInput, output );
	_characterBody->setLinearVelocity( output.m_velocity, deltaMilliseconds );

	_characterInput.m_inputLR = 0.0f;
	_characterInput.m_inputUD = 0.0f;
	_characterInput.m_wantJump = false;
}

void PhysicsSystemCharacterComponent::Observe( ISubject* subject, unsigned int systemChanges )
{
	PhysicsSystemComponent::Observe( subject, systemChanges );

	ISystemComponent* component = static_cast< ISystemComponent* >( subject );

	float walkSpeed = 1.0f;

	if ( component->GetType( ) == InputSystemType )
	{
		InputSystemComponent* inputComponent = static_cast< InputSystemComponent* >( component );

		if ( System::Changes::Input::Mouse_Moved & systemChanges )
		{
			_mouseXDelta = inputComponent->GetProperties( )[ "mouseXDelta" ].GetValue< int >( );
		}

		if( System::Changes::Input::Move_Forward & systemChanges )
		{
			_characterInput.m_inputUD = walkSpeed;
		}

		if( System::Changes::Input::Move_Backward & systemChanges )
		{
			_characterInput.m_inputUD = -walkSpeed;
		}

		if( System::Changes::Input::Strafe_Right & systemChanges )
		{
			_characterInput.m_inputLR = walkSpeed;
		}

		if( System::Changes::Input::Strafe_Left & systemChanges )
		{
			_characterInput.m_inputLR = -walkSpeed;
		}

		if( System::Changes::Input::Jump & systemChanges )
		{
			_characterInput.m_wantJump = true;
		}
	}
}