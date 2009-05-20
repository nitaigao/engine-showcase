#include "ScriptSystemScene.h"

#include <luabind/table_policy.hpp>
#include <luabind/operator.hpp>
using namespace luabind;

#include "../Events/Event.h"
#include "../Events/EventData.hpp"
using namespace Events;

#include "../Logging/Logger.h"
using namespace Logging;

using namespace Maths;

#include "../Management/Management.h"
#include "../Exceptions/ScriptException.hpp"
#include "../Exceptions/OutOfRangeException.hpp"

#include "ScriptEvent.hpp"
#include "ScriptComponent.h"
#include "ScriptConfiguration.h"

namespace Script
{
	ScriptSystemScene::~ScriptSystemScene( )
	{
		delete m_eventHandlers;
		m_eventHandlers = 0;

		delete m_scriptConfiguration;
		m_scriptConfiguration = 0;
	}

	ScriptSystemScene::ScriptSystemScene( Configuration::IConfiguration* configuration )
	{
		m_scriptConfiguration = new ScriptConfiguration( configuration );
		m_state = lua_open( );
		m_eventHandlers = new EventHandlerList( );
	}

	ISystemComponent* ScriptSystemScene::CreateComponent( const std::string& name, const std::string& type )
	{
		int result = lua_checkstack( m_state, LUA_MINSTACK );

		if( !result )
		{
			OutOfRangeException e( "ScriptSystemScene::CreateComponent - Unable to grow the LUA stack to the required size" );
			Logger::Fatal( e.what( ) );
			throw e;
		}
		
		int top = lua_gettop( m_state ); 
		lua_getfield( m_state, LUA_REGISTRYINDEX, "Scripts" ); // top + 1 

		lua_State *childState = lua_newthread( m_state ); // top + 2 

		ScriptComponent* component = new ScriptComponent( name, childState );
		
		lua_newtable( m_state );  // a global table for this script 
		lua_newtable( m_state );  // meta table 
		
		lua_getfenv( m_state,top + 2 ); // that returns the global table (we are	going to protect) 
		lua_setfield( m_state, -2, "__index" ); // set global table as __index of the thread 
		lua_setmetatable( m_state, -2 );
		lua_setfenv( m_state, top + 2 );  // set environment of the new thread

		luabind::globals( childState )[ "script" ] = component;

		m_components[ name ] = component;
		
		component->SetAttribute( System::Attributes::Name, name );
		component->SetAttribute( System::Attributes::Type, System::Types::SCRIPT );
		component->SetAttribute( System::Attributes::Parent, this );

		return component;
	}

	void ScriptSystemScene::DestroyComponent( ISystemComponent* component )
	{
		m_components.erase( component->GetAttributes( )[ System::Attributes::Name ].GetValue< std::string >( ) );
		component->Destroy( );
		delete component;
		component = 0;
	}

	void ScriptSystemScene::Initialize( )
	{
		luaL_openlibs( m_state );
		luabind::open( m_state );

		module( m_state )
		[
			def( "print", &ScriptSystemScene::Print ),
			def( "quit", &ScriptSystemScene::Quit ),
			def( "loadLevel", &ScriptSystemScene::LoadLevel ),
			def( "endGame", &ScriptSystemScene::EndGame ),

			class_< ScriptConfiguration >( "Config" )
				.property( "isFullScreen", &ScriptConfiguration::IsFullScreen, &ScriptConfiguration::SetFullScreen )
				.property( "displayWidth", &ScriptConfiguration::GetDisplayWidth, &ScriptConfiguration::SetDisplayWidth )
				.property( "displayHeight", &ScriptConfiguration::GetDisplayHeight, &ScriptConfiguration::SetDisplayHeight )
				.property( "isConsole", &ScriptConfiguration::IsConsole, &ScriptConfiguration::SetConsole ),

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
					.def( "keyOutEvent", &SoundController::KeyOutEvent ),

			class_< MathVector3 >( "Vector" )
				.def( constructor< const float&, const float&, const float& >( ) )
				.def( self + MathVector3( ) ),

			class_< MathQuaternion >( "Quaternion" )
				.def( constructor< const float&, const float&, const float&, const float& >( ) )

		];

		luabind::globals( m_state )[ "Configuration" ] = m_scriptConfiguration;

		luabind::set_pcall_callback( &ScriptSystemScene::Script_PError );
		luabind::set_error_callback( &ScriptSystemScene::Script_Error );
		luabind::set_cast_failed_callback( &ScriptSystemScene::Script_CastError );
	}

	void ScriptSystemScene::Destroy()
	{
		lua_close( m_state );
		m_state = 0;
	}

	int ScriptSystemScene::Script_PError( lua_State* luaState )
	{
		std::stringstream errorMessage;

		lua_Debug d;
		int result = 0;
		result = lua_getstack( luaState, 0, &d );

		if ( result )
		{
			result = lua_getinfo( luaState, "Sln", &d );

			errorMessage << "Script Error: " << d.short_src << ":" << d.currentline;

			if ( d.name != 0 )
			{
				errorMessage << "(" << d.namewhat << " " << d.name << ")";
			}

			errorMessage << " ";
		}

		if ( lua_isstring( luaState, -1 ) )
		{
			std::string error = lua_tostring( luaState, -1 );
			lua_pop( luaState, 1 );

			errorMessage << error;
		}

		lua_pushstring( luaState, errorMessage.str( ).c_str( ) );
		Logger::Warn( errorMessage.str( ) );

		return 1;	
	}

	void ScriptSystemScene::Script_CastError( lua_State* luaState, LUABIND_TYPE_INFO typeInfo )
	{
		ScriptSystemScene::Script_PError( luaState );
	}

	void ScriptSystemScene::Script_Error( lua_State* luaState )
	{
		ScriptSystemScene::Script_PError( luaState );
	}

	void ScriptSystemScene::Print( const std::string& message )
	{
		Logger::Info( message );
	}

	void ScriptSystemScene::Quit( )
	{
		Management::GetInstance( )->GetEventManager( )->QueueEvent( new Event( GAME_QUIT ) );
	}

	void ScriptSystemScene::LoadLevel( const std::string& levelName )
	{
		IEventData* eventData = new LevelChangedEventData( levelName );
		Management::GetInstance( )->GetEventManager( )->QueueEvent( new Event( GAME_LEVEL_CHANGED, eventData ) );
	}

	void ScriptSystemScene::EndGame()
	{
		Management::GetInstance( )->GetEventManager( )->QueueEvent( new ScriptEvent( "GAME_ENDED" ) );
		Management::GetInstance( )->GetEventManager( )->QueueEvent( new Event( GAME_ENDED ) );
	}

	void ScriptSystemScene::Update( const float& deltaMilliseconds )
	{
		for( ScriptComponentList::iterator i = m_components.begin( ); i != m_components.end( ); ++i )
		{
			( *i ).second->Update( deltaMilliseconds );
		}
	}

	ISystemComponent* ScriptSystemScene::FindComponent( const std::string& name ) const
	{
		return ( *( m_components.find( name ) ) ).second;
	}
}