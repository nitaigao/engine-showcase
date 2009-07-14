#include "PhysicsComponentSerializer.h"

using namespace ticpp;

#include "../../System/SystemTypeMapper.hpp"
using namespace System;

namespace Serialization
{
	ISystemComponent* PhysicsComponentSerializer::DeSerialize( const std::string entityName, ticpp::Element* componentElement, const ISystemScene::SystemSceneMap& systemScenes )
	{
		std::string system;
		componentElement->GetAttribute( System::Attributes::SystemType, &system );

		ISystemScene::SystemSceneMap::const_iterator systemScene = systemScenes.find( System::SystemTypeMapper::StringToType( system ) );

		std::string type;
		componentElement->GetAttribute( System::Attributes::ComponentType, &type );

		ISystemComponent* systemComponent = ( *systemScene ).second->CreateComponent( entityName, type );

		ticpp::Element* attributesElement = componentElement->FirstChildElement( "attributes" );
		for( Iterator< Element > attribute = attributesElement->FirstChildElement( false ); attribute != attribute.end( ); attribute++ )
		{
			std::string key;
			attribute->GetAttribute( "key", &key );

			if ( key == System::Parameters::Body )
			{
				std::string bodyPath;
				attribute->GetAttribute( "v1", &bodyPath );

				systemComponent->SetAttribute( System::Parameters::Body, bodyPath );
			}
		}
	
		return systemComponent;
	}
}

