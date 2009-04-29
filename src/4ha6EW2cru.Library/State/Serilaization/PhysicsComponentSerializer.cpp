#include "PhysicsComponentSerializer.h"

#include <yaml.h>

namespace Serialization
{
	ISystemComponent* PhysicsComponentSerializer::DeSerialize( const std::string& entityName, const YAML::Node& componentNode, const ISystemScene::SystemSceneMap& systemScenes )
	{
		AnyValue::AnyValueMap properties;
	
		for( YAML::Iterator componentProperty = componentNode.begin( ); componentProperty != componentNode.end( ); ++componentProperty ) 
		{
			std::string propertyKey, propertyValue;
	
			componentProperty.first( ) >> propertyKey;
			componentProperty.second( ) >> propertyValue;
	
			properties.insert( std::make_pair( propertyKey, propertyValue ) );
		}
	
		SystemSceneMap::const_iterator systemScene = systemScenes.find( System::Types::PHYSICS );
	
		std::string type = properties[ "type" ].GetValue< std::string >( );
	
		ISystemComponent* systemComponent = ( *systemScene ).second->CreateComponent( entityName, ( type == "character" ) ? type : "default" );
		systemComponent->Initialize( properties );
	
		return systemComponent;
	}
}

