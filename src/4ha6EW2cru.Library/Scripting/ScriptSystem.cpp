#include "ScriptSystem.h"

#include "ScriptSystemScene.h"
#include "ScriptComponent.h"

#include "../Management/Management.h"

namespace Script
{
	ScriptSystem::~ScriptSystem()
	{
		if ( _auxScene != 0 )
		{
			delete _auxScene;
			_auxScene = 0;
		}
	}

	void ScriptSystem::Release()
	{
		if ( _auxScene != 0 )
		{
			_auxScene->Destroy( );
		}
	}

	ISystemScene* ScriptSystem::CreateScene( )
	{
		ScriptSystemScene* scene = new ScriptSystemScene( _configuration );
		scene->Initialize( );
		return scene;
	}

	AnyValue::AnyValueMap ScriptSystem::Execute( const std::string& actionName, AnyValue::AnyValueMap& parameters )
	{
		AnyValue::AnyValueMap results;

		if( actionName == "loadScript" )
		{
			ISystemComponent* systemComponent = _auxScene->CreateComponent( parameters[ "name" ].GetValue< std::string >( ), "default" );
			IScriptComponent* scriptComponent = static_cast< IScriptComponent* >( systemComponent );
			scriptComponent->Initialize( parameters );
			results[ "state" ] = scriptComponent->GetState( );
		}

		if ( actionName == "unloadComponent" )
		{
			ISystemComponent* component = _auxScene->FindComponent( parameters[ "name" ].GetValue< std::string >( ) );
			_auxScene->DestroyComponent( component );
		}

		if ( actionName == "getMasterState" )
		{
			results[ "masterState" ] = _auxScene->GetState( );
		}

		return results;
	}

	void ScriptSystem::Initialize()
	{
		Management::GetInstance( )->GetServiceManager( )->RegisterService( this );

		_auxScene = static_cast< IScriptSystemScene* >( this->CreateScene( ) );  
	}

	void ScriptSystem::Update( const float& deltaMilliseconds )
	{
		_auxScene->Update( deltaMilliseconds );
	}
}