#include "ScriptSystem.h"

#include "ScriptSystemScene.h"
#include "ScriptComponent.h"

#include "../Management/Management.h"

namespace Script
{
	ISystemScene* ScriptSystem::CreateScene( )
	{
		_scene = new ScriptSystemScene( _configuration );
		_scene->Initialize( );
		return _scene;
	}

	AnyValue::AnyValueMap ScriptSystem::Execute( const std::string& actionName, AnyValue::AnyValueMap& parameters )
	{
		AnyValue::AnyValueMap results;

		if( actionName == "loadScript" )
		{
			ISystemComponent* systemComponent = _scene->CreateComponent( parameters[ "name" ].GetValue< std::string >( ), "default" );
			IScriptComponent* scriptComponent = static_cast< IScriptComponent* >( systemComponent );
			scriptComponent->Initialize( parameters );
			results[ "state" ] = scriptComponent->GetState( );
		}

		return results;
	}

	void ScriptSystem::Initialize()
	{
		Management::GetInstance( )->GetServiceManager( )->RegisterService( this );
	}

	void ScriptSystem::Update( const float& deltaMilliseconds )
	{
		_scene->Update( deltaMilliseconds );
	}
}