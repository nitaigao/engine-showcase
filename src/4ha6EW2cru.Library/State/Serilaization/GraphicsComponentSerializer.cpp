#include "GraphicsComponentSerializer.h"

#include <yaml.h>

namespace Serialization
{
	ISystemComponent* GraphicsComponentSerializer::DeSerialize( const std::string& entityName, const YAML::Node& componentNode, const SystemSceneMap&  systemScenes )
	{
		SystemSceneMap::const_iterator systemScene = systemScenes.find( System::Types::RENDER );

		std::string type;
		componentNode[ "type" ] >> type;

		ISystemComponent* systemComponent = ( *systemScene ).second->CreateComponent( entityName, ( type == "camera" ) ? type : "default" );

		std::string model;
		componentNode[ "model" ] >> model;

		systemComponent->SetAttribute( System::Attributes::Model, model );
		
		systemComponent->Initialize( );
	
		return systemComponent;
	}
}