#include "PhysicsSystemComponent.h"

#include <Ogre.h>

#include <sstream>
#include "../Logging/Logger.h"

#include "../Management/Management.h"

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

#include "../IO/ResourceManager.h"
using namespace Resources;

void PhysicsSystemComponent::Initialize( AnyValue::AnyValueMap properties )
{
	std::string bodyPath = properties[ "body" ].GetValue< std::string >( );

	IResource* resource = Management::GetInstance( )->GetResourceManager( )->GetResource( bodyPath );

	hkIstream istreamFromMemory( resource->GetFileBuffer( )->fileBytes, resource->GetFileBuffer( )->fileLength );
	hkStreamReader* streamReader = istreamFromMemory.getStreamReader( );

	hkBinaryPackfileReader reader;
	reader.loadEntireFile( streamReader );

	_loadedData = reader.getPackfileData( );
	_loadedData->addReference();

	hkRootLevelContainer* container = static_cast< hkRootLevelContainer* >( reader.getContents( "hkRootLevelContainer" ) );
	hkpPhysicsData* physicsData = static_cast< hkpPhysicsData* >( container->findObjectByType( "hkpPhysicsData" ) );

	_body = physicsData->findRigidBodyByName( _name.c_str( ) );
	_body->setUserData( _componentId );
	_scene->GetWorld( )->addEntity( _body );
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

	if ( systemChanges & System::Changes::Geometry::All )
	{
		_body->setPositionAndRotation( 
			component->GetPosition( ).AshkVector4( ),	
			component->GetOrientation( ).AshkQuaternion( )
		);
	}
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

MathQuaternion PhysicsSystemComponent::GetOrientation( )
{
	hkQuaternion physicsRotation = _body->getRotation( );

	MathQuaternion rotation(
		physicsRotation( 0 ),
		physicsRotation( 1 ),
		physicsRotation( 2 ),
		physicsRotation( 3 )
		);

	return rotation;
}