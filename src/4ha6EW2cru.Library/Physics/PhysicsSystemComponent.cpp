#include "PhysicsSystemComponent.h"

using namespace Maths;

#include "../IO/IResource.hpp"
using namespace Resources;

#include "../Management/Management.h"

#include <Common/Base/System/Io/IStream/hkIStream.h>
#include <Common/Serialize/Packfile/Binary/hkBinaryPackfileReader.h>
#include <Common/Serialize/Util/hkRootLevelContainer.h>
#include <Physics/Dynamics/Entity/hkpRigidBody.h>
#include <Physics/Utilities/Serialize/hkpPhysicsData.h>

namespace Physics
{
	void PhysicsSystemComponent::Initialize( AnyValue::AnyValueMap& properties )
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

	void PhysicsSystemComponent::PushChanges( const unsigned int& systemChanges )
	{
		_observer->Observe( this, systemChanges );
	}

	void PhysicsSystemComponent::Observe( ISubject* subject, const unsigned int& systemChanges )
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
}