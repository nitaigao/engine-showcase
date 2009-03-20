#include "PhysicsSystemComponent.h"

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

hkpSurfaceInfo* m_previousGround;
int m_framesInAir;

void PhysicsSystemComponent::Initialize( SystemPropertyList properties )
{
	std::string type = properties[ "type" ].GetValue< std::string >( );

	if ( type == "character" )
	{
		m_previousGround = new hkpSurfaceInfo( );
		m_framesInAir = 0;

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
		characterInfo.m_shape = new hkpCapsuleShape( hkVector4( 0.0f, 0.0f, 0.4f ), hkVector4( 0.0f, 0.0f, -0.4f ), 0.6f );
		characterInfo.m_maxForce = 1000.0f;

		_characterBody = new hkpCharacterRigidBody( characterInfo );
		_scene->GetWorld( )->addEntity( _characterBody->getRigidBody( ) );
	}
	else // -- its a standard rigid body
	{
		std::string bodyPath = properties[ "body" ].GetValue< std::string >( );
		FileBuffer* bodyBuffer = FileManager::GetInstance( )->GetFile( bodyPath );

		hkIstream istreamFromMemory( bodyBuffer->fileBytes, bodyBuffer->fileLength );
		hkStreamReader* streamReader = istreamFromMemory.getStreamReader( );

		hkBinaryPackfileReader reader;
		reader.loadEntireFile( streamReader );

		_loadedData = reader.getPackfileData( );
		_loadedData->addReference();

		hkRootLevelContainer* container = static_cast< hkRootLevelContainer* >( reader.getContents( "hkRootLevelContainer" ) );
		hkpPhysicsData* physicsData = static_cast< hkpPhysicsData* >( container->findObjectByType("hkpPhysicsData") );

		_body = physicsData->findRigidBodyByName( _name.c_str( ) );
		_scene->GetWorld( )->addEntity( _body );

		delete bodyBuffer;
	}
}

PhysicsSystemComponent::~PhysicsSystemComponent()
{
	if ( _body != 0 )
	{
		_scene->GetWorld( )->removeEntity( _body );
		_loadedData->removeReference( );
	}
}

void PhysicsSystemComponent::PushChanges( unsigned int systemChanges )
{
	_observer->Observe( this, systemChanges );
}

void PhysicsSystemComponent::Observe( ISubject* subject, unsigned int systemChanges )
{
	ISystemComponent* component = static_cast< ISystemComponent* >( subject );

	if ( component->GetType( ) == GeometrySystemType )
	{
		GeometrySystemComponent* geometrySystemComponent = static_cast< GeometrySystemComponent* >( subject );

		Ogre::Quaternion rotationAmount( Ogre::Degree( 90 ), Ogre::Vector3( 1.0f, 0.0f, 0.0f ) );
		Ogre::Quaternion rotationResult = geometrySystemComponent->GetOrientation( ).AsOgreQuaternion( ) * rotationAmount;

		MathQuaternion correctRotation(
			rotationResult.x,
			rotationResult.y,
			rotationResult.z,
			rotationResult.w
			);

		hkpRigidBody* rigidBody = ( _characterContext != 0 ) ? _characterBody->getRigidBody( ) : _body;

		rigidBody->setPositionAndRotation( 
			geometrySystemComponent->GetPosition( ).AshkVector4( ),	
			correctRotation.AshkQuaternion( )
		);
	};
}

MathVector3 PhysicsSystemComponent::GetPosition()
{
	hkVector4 position = ( _characterBody != 0 ) ? _characterBody->getRigidBody( )->getPosition( ) : _body->getPosition( );
	
	return MathVector3( 
		position( 0 ),
		position( 1 ),
		position( 2 )
		);
}

MathQuaternion PhysicsSystemComponent::GetRotation()
{
	hkQuaternion rotation = ( _characterContext != 0 ) ? _characterBody->getRigidBody( )->getRotation( ) : _body->getRotation( );

	MathQuaternion oldRotation(
		rotation( 0 ),
		rotation( 1 ),
		rotation( 2 ),
		rotation( 3 )
		);

	Ogre::Quaternion rotationAmount( Ogre::Degree( -90 ), Ogre::Vector3( 1.0f, 0.0f, 0.0f ) );
	Ogre::Quaternion rotationResult = oldRotation.AsOgreQuaternion( ) * rotationAmount;

	MathQuaternion correctRotation(
		rotationResult.x,
		rotationResult.y,
		rotationResult.z,
		rotationResult.w
		);

	return correctRotation;
	
}

void PhysicsSystemComponent::Update( float deltaMilliseconds )
{
	if( _characterContext != 0 )
	{
		hkpCharacterStateType state = _characterContext->getState( );

		std::stringstream outputMessage;

		switch (state)
		{
		case HK_CHARACTER_ON_GROUND:
			outputMessage << "On Ground";	break;
		case HK_CHARACTER_JUMPING:
			outputMessage << "Jumping"; break;
		case HK_CHARACTER_IN_AIR:
			outputMessage << "In Air"; break;
		case HK_CHARACTER_CLIMBING:
			outputMessage << "Climbing"; break;
		default:
			outputMessage << "Other";	break;
		}

		Logger::GetInstance( )->Debug( outputMessage.str( ) );

		hkpCharacterInput input;

		input.m_wantJump = false;
		input.m_atLadder = false;

		input.m_inputLR = 0.0f;
		input.m_inputUD = 0.0f;

		input.m_up = hkVector4( 0.0f, 1.0f, 0.0f );
		input.m_forward = hkVector4( 0.0f, 0.0f, 1.0f );

		hkQuaternion orientation;
		orientation.setAxisAngle( hkVector4( 0.0f, 1.0f, 0.0f ), 0.0f );

		input.m_forward.setRotatedDir( orientation, input.m_up );

		hkStepInfo stepInfo;
		stepInfo.m_deltaTime = deltaMilliseconds;
		stepInfo.m_invDeltaTime = 1.0f / deltaMilliseconds;

		input.m_stepInfo = stepInfo;

		input.m_characterGravity.set( 0.0f, -16.0f, 0.0f ); //_scene->GetWorld( )->getGravity( );

		input.m_velocity = _characterBody->getRigidBody( )->getLinearVelocity( );
		input.m_position = _characterBody->getRigidBody( )->getPosition( );

		hkpSurfaceInfo ground;
		_characterBody->checkSupport( input.m_stepInfo, ground );

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

			if (m_framesInAir > skipFramesInAir)
			{
				m_framesInAir = 0;
			}			
		}

		hkpCharacterOutput output;
		_characterContext->update( input, output );
		
		hkVector4 currentVel; 
		currentVel = _characterBody->getRigidBody()->getLinearVelocity();

		hkVector4 currentAcc; 
		currentAcc.setSub4(output.m_velocity, currentVel);
		currentAcc.mul4(1/deltaMilliseconds);

		_characterBody->setLinearAccelerationToMassModifier(currentAcc);
		_characterBody->setLinearVelocity( output.m_velocity, deltaMilliseconds );
	}
}