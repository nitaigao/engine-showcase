#include "InputComponentSerializer.h"

#include <yaml.h>

namespace Serialization
{
	ISystemComponent* InputComponentSerializer::DeSerialize( const std::string& entityName, const YAML::Node& componentNode, const ISystemScene::SystemSceneMap& systemScenes )
	{
		SystemSceneMap::const_iterator systemScene = systemScenes.find( System::Types::INPUT );

		ISystemComponent* systemComponent = ( *systemScene ).second->CreateComponent( entityName, "default" );
		systemComponent->Initialize( );

		return systemComponent;
	}
}