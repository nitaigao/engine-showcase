#include "InputComponentSerializer.h"

#include <yaml.h>

namespace Serialization
{
	ISystemComponent* InputComponentSerializer::DeSerialize( const std::string& entityName, const YAML::Node& componentNode, const ISystemScene::SystemSceneMap& systemScenes )
	{
		AnyValue::AnyValueMap properties;

		for( YAML::Iterator componentProperty = componentNode.begin( ); componentProperty != componentNode.end( ); ++componentProperty ) 
		{
			std::string propertyKey, propertyValue;

			componentProperty.first( ) >> propertyKey;
			componentProperty.second( ) >> propertyValue;

			properties.insert( std::make_pair( propertyKey, propertyValue ) );
		}

		std::string type = properties[ "type" ].GetValue< std::string >( );

		SystemSceneMap::const_iterator systemScene = systemScenes.find( System::Types::INPUT );

		ISystemComponent* systemComponent = ( *systemScene ).second->CreateComponent( entityName, type );
		systemComponent->Initialize( properties );

		return systemComponent;
	}
}