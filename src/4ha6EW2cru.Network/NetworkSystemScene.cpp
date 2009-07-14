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
		return component;
	}

	void NetworkSystemScene::DestroyComponent( ISystemComponent* component )
	{
		delete component;
		component = 0;
	}
}