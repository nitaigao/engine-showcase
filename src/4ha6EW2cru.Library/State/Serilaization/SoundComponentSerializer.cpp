#include "SoundComponentSerializer.h"

namespace Serialization
{
	ISystemComponent* Serialization::SoundComponentSerializer::DeSerialize( const std::string& entityName, const YAML::Node& componentNode, const ISystemScene::SystemSceneMap& systemScenes )
	{
		AnyValue::AnyValueMap attributes;

		for( YAML::Iterator componentProperty = componentNode.begin( ); componentProperty != componentNode.end( ); ++componentProperty ) 
		{
			std::string propertyKey, propertyValue;

			componentProperty.first( ) >> propertyKey;
			componentProperty.second( ) >> propertyValue;

			attributes.insert( std::make_pair( propertyKey, propertyValue ) );
		}

		std::string type = attributes[ "type" ].GetValue< std::string >( );

		SystemSceneMap::const_iterator systemScene = systemScenes.find( System::Types::SOUND );

		ISystemComponent* systemComponent = ( *systemScene ).second->CreateComponent( entityName, type );
		systemComponent->Initialize( attributes );

		return systemComponent;
	}
}