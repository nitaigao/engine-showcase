#include "SoundComponentSerializer.h"

using namespace ticpp;

#include "../../System/SystemTypeMapper.hpp"
using namespace System;

namespace Serialization
{
	ISystemComponent* SoundComponentSerializer::DeSerialize( const std::string entityName, ticpp::Element* componentElement, const ISystemScene::SystemSceneMap& systemScenes )
	{
		std::string system;
		componentElement->GetAttribute( System::Attributes::SystemType, &system );

		ISystemScene::SystemSceneMap::const_iterator systemScene = systemScenes.find( System::SystemTypeMapper::StringToType( system ) );

		std::string type;
		componentElement->GetAttribute( System::Attributes::ComponentType, &type );

		return ( *systemScene ).second->CreateComponent( entityName, type );
	}
}