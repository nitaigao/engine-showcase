#include "AIComponentSerializer.h"

using namespace ticpp;

#include "../../System/SystemTypeMapper.hpp"
using namespace System;

namespace Serialization
{
	ISystemComponent* AIComponentSerializer::DeSerialize( const std::string entityName, ticpp::Element* componentElement, const ISystemScene::SystemSceneMap& systemScenes )
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

			if ( key == System::Parameters::ScriptPath )
			{
				std::string scriptPath;
				attribute->GetAttribute( "v1", &scriptPath );

				systemComponent->SetAttribute( System::Parameters::ScriptPath, scriptPath );
			}

			if ( key == System::Parameters::NavigationMesh )
			{
				std::string navMesh;
				attribute->GetAttribute( "v1", &navMesh );

				systemComponent->SetAttribute( System::Parameters::NavigationMesh, navMesh );
			}
		}

		systemComponent->Initialize( );

		return systemComponent;
	}
}