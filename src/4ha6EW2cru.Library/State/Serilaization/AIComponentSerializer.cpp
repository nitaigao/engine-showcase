#include "AIComponentSerializer.h"

#include <yaml.h>

namespace Serialization
{
	ISystemComponent* AIComponentSerializer::Deserialize( const std::string& entityName, const YAML::Node& componentNode, const ISystemScene::SystemSceneMap& systemScenes )
	{ 
		AnyValue::AnyValueMap properties;
	
		for( YAML::Iterator componentProperty = componentNode.begin( ); componentProperty != componentNode.end( ); ++componentProperty ) 
		{
			std::string propertyKey, propertyValue;
	
			componentProperty.first( ) >> propertyKey;
			componentProperty.second( ) >> propertyValue;
	
			properties.insert( std::make_pair( propertyKey, propertyValue ) );
		}
	
		SystemSceneMap::const_iterator systemScene = systemScenes.find( System::Types::AI );
	
		ISystemComponent* systemComponent = ( *systemScene ).second->CreateComponent( entityName, "default" );
		systemComponent->Initialize( properties );
	
		return systemComponent;
	}
}