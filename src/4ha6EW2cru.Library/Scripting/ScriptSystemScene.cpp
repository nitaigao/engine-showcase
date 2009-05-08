#include "ScriptSystemScene.h"

#include <luabind/table_policy.hpp>
using namespace luabind;

#include "../Events/Event.h"
#include "../Events/EventData.hpp"
using namespace Events;

#include "../Logging/Logger.h"
using namespace Logging;

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
		delete _eventHandlers;
		_eventHandlers = 0;

		delete _soundController;
		_soundController = 0;

		delete _scriptConfiguration;
		_scriptConfiguration = 0;
	}

	ScriptSystemScene::ScriptSystemScene( Configuration::IConfiguration* configuration )
	{
		_scriptConfiguration = new ScriptConfiguration( configuration );
		_soundController = new SoundController( );
		_state = lua_open( );
		_eventHandlers = new EventHandlerList( );
	}

	ISystemComponent* ScriptSystemScene::CreateComponent( const std::string& name, const std::string& type )
	{
		int result = lua_checkstack( _state, LUA_MINSTACK );

		if( !result )
		{
			OutOfRangeException e( "ScriptSystemScene::CreateComponent - Unable to grow the LUA stack to the required size" );
			Logger::Fatal( e.what( ) );
			throw e;
		}
		
		int top = lua_gettop( _state ); 
		lua_getfield( _state, LUA_REGISTRYINDEX, "Scripts" ); // top + 1 

		lua_State *childState = lua_newthread( _state ); // top + 2 

		ScriptComponent* component = new ScriptComponent( name, childState );
		
		lua_newtable( _state );  // a global table for this script 
		lua_newtable( _state );  // meta table 
		
		lua_getfenv( _state,top + 2 ); // that returns the global table (we are	going to protect) 
		lua_setfield( _state, -2, "__index" ); // set global table as __index of the thread 
		lua_setmetatable( _state, -2 );
		lua_setfenv( _state, top + 2 );  // set environment of the new thread

		luabind::globals( childState )[ "script" ] = component;

		_components[ name ] = component;

		return component;
	}

	void ScriptSystemScene::DestroyComponent( ISystemComponent* component )
	{
		_components.erase( component->GetName( ) );
		component->Destroy( );
		delete component;
		component = 0;
	}

	void ScriptSystemScene::Initialize( )
	{
		luaL_openlibs( _state );
		luabind::open( _state );

		module( _state )
		[
			def( "print", &ScriptSystemScene::Print ),
			def( "quit", &ScriptSystemScene::Quit ),
			def( "loadLevel", &ScriptSystemScene::LoadLevel ),
			def( "endGame", &ScriptSystemScene::EndGame ),

			class_< SoundController >( "SoundController" )
				.def( "playMusic", &SoundController::PlayMusic ),

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
				.def( "getId", &ScriptComponent::GetId )
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
				.def( "broadcastEvent", ( void ( ScriptComponent::* ) ( const std::string&, const int&, const std::string& ) ) &ScriptComponent::BroadcastEvent )
		];

		luabind::globals( _state )[ "Configuration" ] = _scriptConfiguration;
		luabind::globals( _state )[ "sfx" ] = _soundController;

		luabind::set_pcall_callback( &ScriptSystemScene::Script_PError );
		luabind::set_error_callback( &ScriptSystemScene::Script_Error );
		luabind::set_cast_failed_callback( &ScriptSystemScene::Script_CastError );
	}

	void ScriptSystemScene::Destroy()
	{
		lua_close( _state );
		_state = 0;
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
		for( ScriptComponentList::iterator i = _components.begin( ); i != _components.end( ); ++i )
		{
			( *i ).second->Update( deltaMilliseconds );
		}
	}

	ISystemComponent* ScriptSystemScene::FindComponent( const std::string& name ) const
	{
		return ( *( _components.find( name ) ) ).second;
	}
}