#include "GeometryComponentSerializer.h"

using namespace ticpp;

#include "../../Maths/MathVector3.hpp"
#include "../../Maths/MathQuaternion.hpp"
using namespace Maths;

#include "../../System/SystemTypeMapper.hpp"
using namespace System;

namespace Serialization
{
	ISystemComponent* GeometryComponentSerializer::DeSerialize( const std::string entityName, ticpp::Element* componentElement, const ISystemScene::SystemSceneMap& systemScenes )
	{
		std::string system;
		componentElement->GetAttribute( System::Attributes::SystemType, &system );

		ISystemScene::SystemSceneMap::const_iterator systemScene = systemScenes.find( SystemTypeMapper::StringToType( system ) );

		std::string type;
		componentElement->GetAttribute( System::Attributes::ComponentType, &type );

		ISystemComponent* systemComponent = ( *systemScene ).second->CreateComponent( entityName, type );

		ticpp::Element* attributesElement = componentElement->FirstChildElement( "attributes" );
		for( Iterator< Element > attribute = attributesElement->FirstChildElement( false ); attribute != attribute.end( ); attribute++ )
		{
			std::string key;
			attribute->GetAttribute( "key", &key );

			float x, y, z, w;

			if ( key == System::Attributes::Position )
			{
				attribute->GetAttribute( "v1", &x );
				attribute->GetAttribute( "v2", &y );
				attribute->GetAttribute( "v3", &z );

				systemComponent->SetAttribute( System::Attributes::Position, MathVector3( x, y, z ) );
			}

			if ( key == System::Attributes::Scale )
			{
				attribute->GetAttribute( "v1", &x );
				attribute->GetAttribute( "v2", &y );
				attribute->GetAttribute( "v3", &z );

				systemComponent->SetAttribute( System::Attributes::Scale, MathVector3( x, y, z ) );
			}

			if ( key == System::Attributes::Orientation )
			{
				attribute->GetAttribute( "v1", &x );
				attribute->GetAttribute( "v2", &y );
				attribute->GetAttribute( "v3", &z );
				attribute->GetAttribute( "v4", &w );

				systemComponent->SetAttribute( System::Attributes::Orientation, MathQuaternion( x, y, z, w ) );
			}
		}
	
		return systemComponent;
	}
}
