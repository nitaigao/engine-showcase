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

void PhysicsSystemComponent::Initialize( SystemPropertyList properties )
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

PhysicsSystemComponent::~PhysicsSystemComponent()
{
	std::string name = _name;
	if ( _body != 0 )
	{
		_scene->GetWorld( )->removeEntity( _body );
	}

	if ( _loadedData != 0 )
	{
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

		_body->setPositionAndRotation( 
			geometrySystemComponent->GetPosition( ).AshkVector4( ),	
			correctRotation.AshkQuaternion( )
		);
	};
}

MathVector3 PhysicsSystemComponent::GetPosition()
{
	hkVector4 position = _body->getPosition( );
	
	return MathVector3( 
		position( 0 ),
		position( 1 ),
		position( 2 )
		);
}

MathQuaternion PhysicsSystemComponent::GetRotation()
{
	hkQuaternion rotation = _body->getRotation( );

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