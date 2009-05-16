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

		m_characterContext = new hkpCharacterContext( characterManager, HK_CHARACTER_ON_GROUND );
		characterManager->removeReference( );

		m_characterContext->setCharacterType( hkpCharacterContext::HK_CHARACTER_RIGIDBODY );

		hkpCharacterRigidBodyCinfo characterInfo;
		characterInfo.m_mass = 100.0f;
		characterInfo.m_maxForce = 1000.0f;
		characterInfo.m_maxSlope = 70.0f * HK_REAL_DEG_TO_RAD;
		characterInfo.m_up = MathVector3::Up( ).AshkVector4( );
		characterInfo.m_shape = new hkpCapsuleShape( hkVector4( 0.0f, 1.4f, 0.0f ), hkVector4( 0.0f, 0.35f, 0.0f ), 0.3f );
		characterInfo.m_collisionFilterInfo = hkpGroupFilter::calcFilterInfo( 1, 1 );

		m_characterBody = new hkpCharacterRigidBody( characterInfo );

		_body = m_characterBody->getRigidBody( );
		_body->setName( _name.c_str( ) );
		_scene->GetWorld( )->addEntity( _body );

		m_characterInput.m_wantJump = false;
		m_characterInput.m_atLadder = false;
		m_characterInput.m_inputLR = 0.0f;
		m_characterInput.m_inputUD = 0.0f;

		m_characterInput.m_up = MathVector3::Up( ).AshkVector4( );
		m_characterInput.m_characterGravity = hkVector4( 0.0f, -16.0f, 0.0f );
		m_characterInput.m_userData = true;
	}

	PhysicsSystemCharacterComponent::~PhysicsSystemCharacterComponent()
	{
		m_characterBody->removeReference( );
		delete m_characterContext;
		delete m_previousGround;
	}

	void PhysicsSystemCharacterComponent::Update( const float& deltaMilliseconds )
	{
		if ( m_characterInput.m_userData )
		{
			m_characterInput.m_inputUD = m_forwardBackward;
			m_characterInput.m_inputLR = m_leftRight;

			m_characterInput.m_stepInfo.m_deltaTime = deltaMilliseconds;
			m_characterInput.m_stepInfo.m_invDeltaTime = 1.0f / deltaMilliseconds;

			m_characterInput.m_velocity = m_characterBody->getRigidBody( )->getLinearVelocity( );
			m_characterInput.m_position = m_characterBody->getRigidBody( )->getPosition( );

			m_characterInput.m_forward = MathVector3::Forward( ).AshkVector4( );
			m_characterInput.m_forward.setRotatedDir( _body->getRotation( ), m_characterInput.m_forward );

			hkpSurfaceInfo ground;
			m_characterBody->checkSupport( m_characterInput.m_stepInfo, ground );

			const int skipFramesInAir = 10; 

			if (m_characterInput.m_wantJump)
			{
				m_framesInAir = skipFramesInAir;
			}

			if ( ground.m_supportedState != hkpSurfaceInfo::SUPPORTED )
			{
				if (m_framesInAir < skipFramesInAir)
				{
					m_characterInput.m_isSupported = true;
					m_characterInput.m_surfaceNormal = m_previousGround->m_surfaceNormal;
					m_characterInput.m_surfaceVelocity = m_previousGround->m_surfaceVelocity;
					m_characterInput.m_surfaceMotionType = m_previousGround->m_surfaceMotionType;
				}
				else
				{
					m_characterInput.m_isSupported = false;
					m_characterInput.m_surfaceNormal = ground.m_surfaceNormal;
					m_characterInput.m_surfaceVelocity = ground.m_surfaceVelocity;	
					m_characterInput.m_surfaceMotionType = ground.m_surfaceMotionType;
				}			

				m_framesInAir++;
			}
			else
			{
				m_characterInput.m_isSupported = true;
				m_characterInput.m_surfaceNormal = ground.m_surfaceNormal;
				m_characterInput.m_surfaceVelocity = ground.m_surfaceVelocity;
				m_characterInput.m_surfaceMotionType = ground.m_surfaceMotionType;

				m_previousGround->set(ground);

				if (m_framesInAir > skipFramesInAir)
				{
					m_framesInAir = 0;
				}			
			}

			hkpCharacterOutput output;
			m_characterContext->update( m_characterInput, output );
			m_characterBody->setLinearVelocity( output.m_velocity, deltaMilliseconds );

			this->PushChanges( 
				System::Changes::Geometry::Orientation |
				System::Changes::Geometry::Position
				);

			float stopSpeed = 0.0f;

			m_forwardBackward = stopSpeed;
			m_leftRight = stopSpeed;
			m_characterInput.m_inputLR = stopSpeed;
			m_characterInput.m_inputUD = stopSpeed;
			m_characterInput.m_wantJump = false;

			m_characterInput.m_userData = ( m_characterBody->getLinearVelocity( ).compareEqual4( hkVector4::getZero( ) ).anyIsSet( ) );
		}
	}

	void PhysicsSystemCharacterComponent::Observe( ISubject* subject, const unsigned int& systemChanges )
	{
		PhysicsSystemComponent::Observe( subject, systemChanges );

		ISystemComponent* component = static_cast< ISystemComponent* >( subject );

		float walkSpeed = 2.0f;
			
		if( System::Changes::Input::Move_Forward & systemChanges )
		{
			m_forwardBackward = -walkSpeed;
		}

		if( System::Changes::Input::Move_Backward & systemChanges )
		{
			m_forwardBackward = walkSpeed;
		}

		if( System::Changes::Input::Strafe_Right & systemChanges )
		{
			m_leftRight = -walkSpeed;
		}

		if( System::Changes::Input::Strafe_Left & systemChanges )
		{
			m_leftRight = walkSpeed;
		}

		if( System::Changes::Input::Jump & systemChanges )
		{
			m_characterInput.m_wantJump = true;
		}

		if ( m_forwardBackward || m_leftRight )
		{
			hkReal lengthSquared = m_forwardBackward * m_forwardBackward + m_leftRight * m_leftRight;

			if (lengthSquared > HK_REAL_MIN)
			{
				lengthSquared = hkMath::sqrt(lengthSquared);
				m_forwardBackward /= lengthSquared;
				m_leftRight /= lengthSquared;
			}
		}

		m_characterInput.m_userData = true;
	}
}