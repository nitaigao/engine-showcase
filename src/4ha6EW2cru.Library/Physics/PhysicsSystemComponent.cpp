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
#include <Common/Serialize/Version/hkVersionUtil.h>

namespace Physics
{
	PhysicsSystemComponent::~PhysicsSystemComponent( )
	{
		std::string name = _name;
		if ( m_body != 0 )
		{
			_scene->GetWorld( )->removeEntity( m_body );
		}

		if ( m_loadedData != 0 )
		{
			m_loadedData->removeReference( );
		}
	}

	void PhysicsSystemComponent::Initialize( )
	{
		std::string bodyPath = m_attributes[ System::Attributes::Body ].GetValue< std::string >( );

		IResource* resource = Management::GetInstance( )->GetResourceManager( )->GetResource( bodyPath );

		hkIstream istreamFromMemory( resource->GetFileBuffer( )->fileBytes, resource->GetFileBuffer( )->fileLength );
		hkStreamReader* streamReader = istreamFromMemory.getStreamReader( );

		hkBinaryPackfileReader reader;
		reader.loadEntireFile( streamReader );

		hkVersionUtil::updateToCurrentVersion( reader, hkVersionRegistry::getInstance() );

		m_loadedData = reader.getPackfileData( );
		m_loadedData->addReference();

		hkRootLevelContainer* container = static_cast< hkRootLevelContainer* >( reader.getContents( "hkRootLevelContainer" ) );
		hkpPhysicsData* physicsData = static_cast< hkpPhysicsData* >( container->findObjectByType( "hkpPhysicsData" ) );

		m_body = physicsData->findRigidBodyByName( _name.c_str( ) );
		_scene->GetWorld( )->addEntity( m_body );
	}

	AnyValue PhysicsSystemComponent::PushMessage( const unsigned int& messageId, AnyValue::AnyValueKeyMap parameters )
	{
		return m_observer->Message( messageId, parameters );
	}

	AnyValue PhysicsSystemComponent::Message( const unsigned int& messageId, AnyValue::AnyValueKeyMap parameters )
	{
		if ( messageId & System::Messages::SetPosition )
		{
			m_body->setPosition( parameters[ System::Attributes::Position ].GetValue< MathVector3 >( ).AshkVector4( ) );
		}

		if ( messageId & System::Messages::SetOrientation )
		{
			m_body->setRotation( parameters[ System::Attributes::Orientation ].GetValue< MathQuaternion >( ).AshkQuaternion( ) );
		}

		return AnyValue( );
	}
}