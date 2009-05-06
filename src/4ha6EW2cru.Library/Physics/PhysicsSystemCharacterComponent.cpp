#include "PhysicsSystemCharacterComponent.h"

using namespace Maths;

#include "../Geometry/GeometrySystemComponent.h"
#include "../Input/InputSystemComponent.h"

#include <Physics/Dynamics/Entity/hkpRigidBody.h>
#include <Physics/Collide/Shape/Convex/Capsule/hkpCapsuleShape.h>

namespace Physics
{
	void PhysicsSystemCharacterComponent::Initialize( AnyValue::AnyValueMap& properties )
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
		characterInfo.m_shape = new hkpCapsuleShape( hkVector4( 0.0f, 1.0f, 0.0f ), hkVector4( 0.0f, -1.0f, 0.0f ), 0.6f );

		_characterBody = new hkpCharacterRigidBody( characterInfo );

		_body = _characterBody->getRigidBody( );
		_body->setName( _name.c_str( ) );
		_scene->GetWorld( )->addEntity( _body );

		_characterInput.m_wantJump = false;
		_characterInput.m_atLadder = false;
		_characterInput.m_inputLR = 0.0f;
		_characterInput.m_inputUD = 0.0f;

		_characterInput.m_up = MathVector3::Up( ).AshkVector4( );
		_characterInput.m_forward = MathVector3::Forward( ).AshkVector4( );
		_characterInput.m_characterGravity = hkVector4( 0.0f, -16.0f, 0.0f );
		_characterInput.m_userData = false;
	}

	PhysicsSystemCharacterComponent::~PhysicsSystemCharacterComponent()
	{
		_characterBody->removeReference( );
		delete _characterContext;
		delete _previousGround;
	}

	void PhysicsSystemCharacterComponent::Update( const float& deltaMilliseconds )
	{
		if ( _characterInput.m_userData )
		{
			_characterInput.m_inputUD = _forwardBackward;
			_characterInput.m_inputLR = _leftRight;

			_characterInput.m_stepInfo.m_deltaTime = deltaMilliseconds;
			_characterInput.m_stepInfo.m_invDeltaTime = 1.0f / deltaMilliseconds;

			_characterInput.m_velocity = _characterBody->getRigidBody( )->getLinearVelocity( );
			_characterInput.m_position = _characterBody->getRigidBody( )->getPosition( );

			_characterInput.m_forward.set( 0.0f, 0.0f, 1.0f );
			_characterInput.m_forward.setRotatedDir( _characterBody->getRigidBody( )->getRotation( ), _characterInput.m_forward );

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

			this->PushChanges( 
				System::Changes::Geometry::Orientation |
				System::Changes::Geometry::Position
				);

			float stopSpeed = 0.0f;

			_forwardBackward = stopSpeed;
			_leftRight = stopSpeed;
			_characterInput.m_inputLR = stopSpeed;
			_characterInput.m_inputUD = stopSpeed;
			_characterInput.m_wantJump = false;

			_characterInput.m_userData = ( _characterBody->getLinearVelocity( ).compareEqual4( hkVector4::getZero( ) ).anyIsSet( ) );
		}
	}

	void PhysicsSystemCharacterComponent::Observe( ISubject* subject, const unsigned int& systemChanges )
	{
		PhysicsSystemComponent::Observe( subject, systemChanges );

		ISystemComponent* component = static_cast< ISystemComponent* >( subject );

		float walkSpeed = 2.0f;
			
		if( System::Changes::Input::Move_Forward & systemChanges )
		{
			_forwardBackward = walkSpeed;
		}

		if( System::Changes::Input::Move_Backward & systemChanges )
		{
			_forwardBackward = -walkSpeed;
		}

		if( System::Changes::Input::Strafe_Right & systemChanges )
		{
			_leftRight = walkSpeed;
		}

		if( System::Changes::Input::Strafe_Left & systemChanges )
		{
			_leftRight = -walkSpeed;
		}

		if( System::Changes::Input::Jump & systemChanges )
		{
			_characterInput.m_wantJump = true;
		}

		if ( _forwardBackward || _leftRight )
		{
			hkReal lengthSquared = _forwardBackward * _forwardBackward + _leftRight * _leftRight;

			if (lengthSquared > HK_REAL_MIN)
			{
				lengthSquared = hkMath::sqrt(lengthSquared);
				_forwardBackward /= lengthSquared;
				_leftRight /= lengthSquared;
			}
		}

		_characterInput.m_userData = true;
	}
}