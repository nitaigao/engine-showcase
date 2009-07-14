#include "AnimationComponentSerializer.h"

using namespace ticpp;

#include "../../System/SystemTypeMapper.hpp"
using namespace System;

namespace Serialization
{
	ISystemComponent* AnimationComponentSerializer::DeSerialize( const std::string entityName, ticpp::Element* componentElement, const ISystemScene::SystemSceneMap& systemScenes )
	{
		std::string system;
		componentElement->GetAttribute( System::Attributes::SystemType, &system );

		ISystemScene::SystemSceneMap::const_iterator systemScene = systemScenes.find( System::SystemTypeMapper::StringToType( system ) );

		std::string type;
		componentElement->GetAttribute( System::Attributes::ComponentType, &type );

		ISystemComponent* systemComponent = ( *systemScene ).second->CreateComponent( entityName, type );

		std::map< std::string, std::string > animations;

		ticpp::Element* attributesElement = componentElement->FirstChildElement( "attributes" );
		for( Iterator< Element > attribute = attributesElement->FirstChildElement( false ); attribute != attribute.end( ); attribute++ )
		{
			std::string key;
			attribute->GetAttribute( "key", &key );

			if ( key == "animation" )
			{
				std::string animationName;
				attribute->GetAttribute( "v1", &animationName );

				std::string animationPath;
				attribute->GetAttribute( "v2", &animationPath );

				animations.insert( std::make_pair( animationName, animationPath ) );
			}

			if ( key == System::Attributes::Animation::BindPose )
			{
				std::string animationPath;
				attribute->GetAttribute( "v1", &animationPath );

				systemComponent->SetAttribute( System::Attributes::Animation::BindPose, animationPath );
			}

			if ( key == System::Attributes::Animation::DefaultAnimation )
			{
				std::string animationPath;
				attribute->GetAttribute( "v1", &animationPath );

				systemComponent->SetAttribute( System::Attributes::Animation::DefaultAnimation, animationPath );
			}
		}

		systemComponent->SetAttribute( System::Attributes::Animation::Animations , animations );

		return systemComponent;
	}
}