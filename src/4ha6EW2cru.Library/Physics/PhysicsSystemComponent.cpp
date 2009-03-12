#include "PhysicsSystemComponent.h"

#include "../io/FileBuffer.hpp"
#include "../io/FileManager.h"

#include "../Geometry/GeometrySystemComponent.h"

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
	for ( SystemPropertyList::iterator i = properties.begin( ); i != properties.end( ); ++i )
	{
		if ( ( *i ).GetName( ) == "body" )
		{
			std::string bodyPath = ( *i ).GetValue< std::string >( );
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
	GeometrySystemComponent* geometrySystemComponent = static_cast< GeometrySystemComponent* >( subject );

	_body->setPositionAndRotation( 
		geometrySystemComponent->GetPosition( ).AshkVector4( ),	
		geometrySystemComponent->GetOrientation( ).Normalize( ).AshkQuaternion( )
		);
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