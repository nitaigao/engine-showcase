#include "ScriptComponentSerializer.h"

#include <yaml.h>
#include "../../Scripting/IScriptComponent.hpp"
using namespace Script;

namespace Serialization
{
	ISystemComponent* ScriptComponentSerializer::DeSerialize( const std::string& entityName, const YAML::Node& componentNode, const ISystemScene::SystemSceneMap& systemScenes )
	{
		ISystemScene::SystemSceneMap::const_iterator systemScene = systemScenes.find( System::Types::SCRIPT );

		ISystemComponent* systemComponent = ( *systemScene ).second->CreateComponent( entityName, "default" );
		IScriptComponent* scriptComponent = static_cast< IScriptComponent* >( systemComponent );
		
		std::string scriptPath;
		componentNode[ "scriptPath" ] >> scriptPath;

		systemComponent->SetAttribute( System::Parameters::ScriptPath, scriptPath );
		scriptComponent->Initialize( );
		scriptComponent->RunScript( );
	
		return scriptComponent;
	}
}