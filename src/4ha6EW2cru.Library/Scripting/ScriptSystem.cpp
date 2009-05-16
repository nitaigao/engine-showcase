#include "ScriptSystem.h"

#include "ScriptSystemScene.h"
#include "ScriptComponent.h"

#include "../Management/Management.h"

namespace Script
{
	ScriptSystem::~ScriptSystem()
	{
		if ( m_auxScene != 0 )
		{
			delete m_auxScene;
			m_auxScene = 0;
		}
	}

	void ScriptSystem::Release()
	{
		if ( m_auxScene != 0 )
		{
			m_auxScene->Destroy( );
		}
	}

	ISystemScene* ScriptSystem::CreateScene( )
	{
		ScriptSystemScene* scene = new ScriptSystemScene( m_configuration );
		scene->Initialize( );
		return scene;
	}

	AnyValue::AnyValueMap ScriptSystem::Execute( const std::string& actionName, AnyValue::AnyValueMap& parameters )
	{
		AnyValue::AnyValueMap results;

		if( actionName == "loadScript" )
		{
			ISystemComponent* systemComponent = m_auxScene->CreateComponent( parameters[ "name" ].GetValue< std::string >( ), "default" );
			IScriptComponent* scriptComponent = static_cast< IScriptComponent* >( systemComponent );
			scriptComponent->Initialize( parameters );
			results[ "state" ] = scriptComponent->GetState( );
		}

		if ( actionName == "unloadComponent" )
		{
			ISystemComponent* component = m_auxScene->FindComponent( parameters[ "name" ].GetValue< std::string >( ) );
			m_auxScene->DestroyComponent( component );
		}

		if ( actionName == "getMasterState" )
		{
			results[ "masterState" ] = m_auxScene->GetState( );
		}

		return results;
	}

	void ScriptSystem::Initialize()
	{
		Management::GetInstance( )->GetServiceManager( )->RegisterService( this );

		m_auxScene = static_cast< IScriptSystemScene* >( this->CreateScene( ) );  
	}

	void ScriptSystem::Update( const float& deltaMilliseconds )
	{
		m_auxScene->Update( deltaMilliseconds );
	}
}