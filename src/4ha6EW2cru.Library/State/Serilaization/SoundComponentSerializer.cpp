#include "SoundComponentSerializer.h"

namespace Serialization
{
	ISystemComponent* Serialization::SoundComponentSerializer::DeSerialize( const std::string& entityName, const YAML::Node& componentNode, const ISystemScene::SystemSceneMap& systemScenes )
	{
		ISystemScene::SystemSceneMap::const_iterator systemScene = systemScenes.find( System::Types::SOUND );

		ISystemComponent* systemComponent = ( *systemScene ).second->CreateComponent( entityName, "default" );
		systemComponent->Initialize( );

		return systemComponent;
	}
}