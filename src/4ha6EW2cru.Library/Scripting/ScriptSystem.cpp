#include "ScriptSystem.h"

#include "ScriptSystemScene.h"
#include "ScriptComponent.h"

#include "../Management/Management.h"

#include <luabind/luabind.hpp>
#include <luabind/table_policy.hpp>
#include <luabind/operator.hpp>
using namespace luabind;

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

	AnyType::AnyTypeMap ScriptSystem::Execute( const std::string& actionName, AnyType::AnyTypeMap& parameters )
	{
		AnyType::AnyTypeMap results;

		if( actionName == "loadScript" )
		{
			ISystemComponent* systemComponent = m_auxScene->CreateComponent( parameters[ "name" ].As< std::string >( ), "default" );
			IScriptComponent* scriptComponent = static_cast< IScriptComponent* >( systemComponent );
			scriptComponent->SetAttribute( System::Attributes::ScriptPath, parameters[ "scriptPath" ].As< std::string >( ) );
			scriptComponent->Initialize( );
			results[ "state" ] = scriptComponent->GetState( );
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
			module( parameters[ System::Attributes::ScriptState ].As< lua_State* >( ) )
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
					.def( "playAnimation", &ScriptComponent::PlayAnimation )
					.def( "broadcastEvent", ( void ( ScriptComponent::* ) ( const std::string& ) ) &ScriptComponent::BroadcastEvent )
					.def( "broadcastEvent", ( void ( ScriptComponent::* ) ( const std::string&, const std::string& ) ) &ScriptComponent::BroadcastEvent )
					.def( "broadcastEvent", ( void ( ScriptComponent::* ) ( const std::string&, const int& ) ) &ScriptComponent::BroadcastEvent )
					.def( "broadcastEvent", ( void ( ScriptComponent::* ) ( const std::string&, const std::string&, const std::string& ) ) &ScriptComponent::BroadcastEvent )
					.def( "broadcastEvent", ( void ( ScriptComponent::* ) ( const std::string&, const std::string&, const int& ) ) &ScriptComponent::BroadcastEvent )
					.def( "broadcastEvent", ( void ( ScriptComponent::* ) ( const std::string&, const int&, const int& ) ) &ScriptComponent::BroadcastEvent )
					.def( "broadcastEvent", ( void ( ScriptComponent::* ) ( const std::string&, const int&, const std::string& ) ) &ScriptComponent::BroadcastEvent ),

				class_< SoundController >( "SoundController" )
					.def( "triggerEvent", &SoundController::TriggerEvent )
					.def( "keyOutEvent", &SoundController::KeyOutEvent )
			];
		}

		return results;
	}

	void ScriptSystem::Initialize()
	{
		m_auxScene = static_cast< IScriptSystemScene* >( this->CreateScene( ) );  
	}

	void ScriptSystem::Update( const float& deltaMilliseconds )
	{
		m_auxScene->Update( deltaMilliseconds );
	}

	void ScriptSystem::Message( const std::string& message, AnyType::AnyTypeMap parameters )
	{
		if ( message == System::Messages::RegisterService )
		{
			Management::GetInstance( )->GetServiceManager( )->RegisterService( this );
		}
	}
}