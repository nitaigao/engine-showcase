#include "ScriptSystem.h"

#include "ScriptSystemScene.h"
#include "ScriptComponent.h"

#include "Management/Management.h"

#include <luabind/luabind.hpp>
#include <luabind/table_policy.hpp>
#include <luabind/operator.hpp>
using namespace luabind;

#include "SoundFacade.h"
#include "InstrumentationFacade.h"
#include "AnimationFacade.h"
#include "NetworkFacade.h"

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
		m_scene = new ScriptSystemScene( m_configuration );
		m_scene->Initialize( );
		return m_scene;
	}

	AnyType::AnyTypeMap ScriptSystem::Execute( const std::string& actionName, AnyType::AnyTypeMap& parameters )
	{
		AnyType::AnyTypeMap results;

		if( actionName == "loadScript" )
		{
			ISystemComponent* component = m_auxScene->CreateComponent( parameters[ System::Attributes::Name ].As< std::string >( ), "default" );
			component->SetAttribute( System::Parameters::ScriptPath, parameters[ System::Parameters::ScriptPath ].As< std::string >( ) );
			component->Initialize( );
			results[ "component" ] = component;
		}

		if ( actionName == "unloadComponent" )
		{
			ISystemComponent* component = m_auxScene->FindComponent( parameters[ "name" ].As< std::string >( ) );
			m_auxScene->DestroyComponent( component );
		}

		if ( actionName == "getMasterState" )
		{
			results[ "masterState" ] = m_auxScene->GetState( );
		}

		if ( actionName == System::Messages::RegisterScriptFunctions )
		{
			lua_State* luaState = parameters[ System::Parameters::ScriptState ].As< lua_State* >( );
			
			module( luaState )
			[
				def( "print", &ScriptSystemScene::Print ),
				def( "quit", &ScriptSystemScene::Quit ),
				def( "loadLevel", &ScriptSystemScene::LoadLevel ),
				def( "endGame", &ScriptSystemScene::EndGame ),

				class_< ScriptConfiguration >( "Config" )
					.property( "isFullScreen", &ScriptConfiguration::IsFullScreen, &ScriptConfiguration::SetFullScreen )
					.property( "displayWidth", &ScriptConfiguration::GetDisplayWidth, &ScriptConfiguration::SetDisplayWidth )
					.property( "displayHeight", &ScriptConfiguration::GetDisplayHeight, &ScriptConfiguration::SetDisplayHeight )
					.property( "isConsole", &ScriptConfiguration::IsConsole, &ScriptConfiguration::SetConsole )
					.property( "isInvertY", &ScriptConfiguration::IsInvertY, &ScriptConfiguration::SetInvertY )
					.property( "isSmoothMouse", &ScriptConfiguration::IsSmoothMouse, &ScriptConfiguration::SetSmoothMouse )
					.property( "mouseSmoothAmount", &ScriptConfiguration::GetMouseSmoothAmount, &ScriptConfiguration::SetMouseSmoothAmount )
					.property( "sfxVolume", &ScriptConfiguration::GetSFXVolume, &ScriptConfiguration::SetSFXVolume )
					.property( "musicVolume", &ScriptConfiguration::GetMusicVolume, &ScriptConfiguration::SetMusicVolume ),

				class_< ScriptComponent >( "ScriptComponent" )
					.def( "include", &ScriptComponent::IncludeScript )
					.def( "registerEventHandler", &ScriptComponent::RegisterEvent )
					.def( "registerUpdateHandler", &ScriptComponent::RegisterUpdate )
					.def( "unregisterEventHandler", &ScriptComponent::UnRegisterEvent )
					.def( "unregisterUpdateHandler", &ScriptComponent::UnRegisterUpdate )
					.def( "getName", &ScriptComponent::GetName )
					.def( "getLookAt", &ScriptComponent::GetLookAt )
					.def( "getPosition", &ScriptComponent::GetPosition )
					.def( "getTime", &ScriptComponent::GetTime )
					.def( "executeString", &ScriptComponent::ExecuteString )
					.def( "rayQuery", &ScriptComponent::RayQuery, copy_table( result ) )
					.def( "broadcastEvent", ( void ( ScriptComponent::* ) ( const std::string& ) ) &ScriptComponent::BroadcastEvent )
					.def( "broadcastEvent", ( void ( ScriptComponent::* ) ( const std::string&, const std::string& ) ) &ScriptComponent::BroadcastEvent )
					.def( "broadcastEvent", ( void ( ScriptComponent::* ) ( const std::string&, const int& ) ) &ScriptComponent::BroadcastEvent )
					.def( "broadcastEvent", ( void ( ScriptComponent::* ) ( const std::string&, const std::string&, const std::string& ) ) &ScriptComponent::BroadcastEvent )
					.def( "broadcastEvent", ( void ( ScriptComponent::* ) ( const std::string&, const std::string&, const int& ) ) &ScriptComponent::BroadcastEvent )
					.def( "broadcastEvent", ( void ( ScriptComponent::* ) ( const std::string&, const int&, const int& ) ) &ScriptComponent::BroadcastEvent )
					.def( "broadcastEvent", ( void ( ScriptComponent::* ) ( const std::string&, const int&, const std::string& ) ) &ScriptComponent::BroadcastEvent )
					
			];

			SoundFacade::RegisterFunctions( luaState );
			InstrumentationFacade::RegisterFunctions( luaState );
			AnimationFacade::RegisterFunctions( luaState );
			NetworkFacade::RegisterFunctions( luaState );
		}

		return results;
	}

	void ScriptSystem::Initialize( Configuration::IConfiguration* configuration )
	{
		m_configuration = configuration;
		m_auxScene = static_cast< IScriptSystemScene* >( this->CreateScene( ) );  
	}

	void ScriptSystem::Update( const float& deltaMilliseconds )
	{
		m_auxScene->Update( deltaMilliseconds );
		m_scene->Update( deltaMilliseconds );
	}

	void ScriptSystem::Message( const std::string& message, AnyType::AnyTypeMap parameters )
	{
		if ( message == System::Messages::RegisterService )
		{
			Management::Get( )->GetServiceManager( )->RegisterService( this );
		}
	}
}