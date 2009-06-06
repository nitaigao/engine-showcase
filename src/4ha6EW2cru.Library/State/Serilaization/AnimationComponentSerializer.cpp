#include "AnimationComponentSerializer.h"

#include <yaml.h>
#include "../../System/SystemTypeMapper.hpp"
using namespace System;

namespace Serialization
{
	ISystemComponent* AnimationComponentSerializer::DeSerialize( const std::string& entityName, const YAML::Node& componentNode, const ISystemScene::SystemSceneMap& systemScenes )
	{
		std::string system;
		componentNode[ "system" ] >> system;

		ISystemScene::SystemSceneMap::const_iterator systemScene = systemScenes.find( SystemTypeMapper::StringToType( system ) );

		std::string type;
		componentNode[ "type" ] >> type;

		ISystemComponent* systemComponent = ( *systemScene ).second->CreateComponent( entityName, type );

		for( YAML::Iterator componentProperty = componentNode.begin( ); componentProperty != componentNode.end( ); ++componentProperty ) 
		{
			std::string propertyKey, propertyValue;

			componentProperty.first( ) >> propertyKey;
			componentProperty.second( ) >> propertyValue;

			systemComponent->SetAttribute( propertyKey, propertyValue );
		}

		systemComponent->Initialize( );

		return systemComponent;
	}
}