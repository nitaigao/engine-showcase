#include "NetworkSystemScene.h"

#include "NetworkSystemComponent.h"

namespace Network
{

	ISystemComponent* NetworkSystemScene::CreateComponent( const std::string& name, const std::string& type )
	{
		ISystemComponent* component = new NetworkSystemComponent( );
		component->SetAttribute( System::Attributes::Name, name );
		component->SetAttribute( System::Attributes::SystemType, System::Types::NETWORK );
		component->SetAttribute( System::Attributes::Parent, this );

		m_components.insert( std::make_pair( name, component ) );

		return component;
	}

	void NetworkSystemScene::DestroyComponent( ISystemComponent* component )
	{
		m_components.erase( component->GetAttributes( )[ System::Attributes::Name ].As< std::string >( ) );

		delete component;
		component = 0;
	}

	void NetworkSystemScene::MessageComponent( const std::string componentId, const System::Message& message, AnyType::AnyTypeMap parameters )
	{
		static_cast< INetworkSystemComponent* >( m_components[ componentId ] )->MessageFromNetwork( message, parameters );
	}
}