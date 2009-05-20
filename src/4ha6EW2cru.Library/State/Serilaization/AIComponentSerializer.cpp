#include "AIComponentSerializer.h"

#include <yaml.h>

namespace Serialization
{
	ISystemComponent* AIComponentSerializer::DeSerialize( const std::string& entityName, const YAML::Node& componentNode, const ISystemScene::SystemSceneMap& systemScenes )
	{ 
		SystemSceneMap::const_iterator systemScene = systemScenes.find( System::Types::AI );

		ISystemComponent* systemComponent = ( *systemScene ).second->CreateComponent( entityName, "default" );

		std::string scriptPath;
		componentNode[ "scriptPath" ] >> scriptPath;

		systemComponent->SetAttribute( System::Attributes::ScriptPath, scriptPath );
		systemComponent->Initialize( );

		return systemComponent;
	}
}