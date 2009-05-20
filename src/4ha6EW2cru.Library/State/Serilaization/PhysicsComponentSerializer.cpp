#include "PhysicsComponentSerializer.h"

#include <yaml.h>

namespace Serialization
{
	ISystemComponent* PhysicsComponentSerializer::DeSerialize( const std::string& entityName, const YAML::Node& componentNode, const ISystemScene::SystemSceneMap& systemScenes )
	{
		SystemSceneMap::const_iterator systemScene = systemScenes.find( System::Types::PHYSICS );

		std::string type;
		componentNode[ "type" ] >> type;

		ISystemComponent* systemComponent = ( *systemScene ).second->CreateComponent( entityName, ( type == "character" ) ? type : "default" );

		std::string body;
		componentNode[ "body" ] >> body;

		systemComponent->SetAttribute( System::Attributes::Body, body );

		systemComponent->Initialize( );
	
		return systemComponent;
	}
}

