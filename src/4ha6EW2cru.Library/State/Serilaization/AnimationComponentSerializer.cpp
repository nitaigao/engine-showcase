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
		std::map< std::string, std::string > animations;

		for( YAML::Iterator componentProperty = componentNode.begin( ); componentProperty != componentNode.end( ); ++componentProperty ) 
		{
			if ( componentProperty.second().GetType( ) == YAML::CT_MAP )
			{
				for( YAML::Iterator animationNode = componentProperty.second( ).begin( ); animationNode != componentProperty.second( ).end( ); ++animationNode ) 
				{
					std::string animationName, animationPath;

					animationNode.first( ) >> animationName;
					animationNode.second( ) >> animationPath;

					animations.insert( std::make_pair( animationName, animationPath ) );
				}
			}
			else
			{
				std::string propertyKey, propertyValue;

				componentProperty.first( ) >> propertyKey;
				componentProperty.second( ) >> propertyValue;

				systemComponent->SetAttribute( propertyKey, propertyValue );
			}
		}


		systemComponent->SetAttribute( System::Attributes::Animation::Animations , animations );
		systemComponent->Initialize( );

		return systemComponent;
	}
}