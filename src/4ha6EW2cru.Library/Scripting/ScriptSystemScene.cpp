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

#include "ScriptEvent.hpp"
#include "ScriptComponent.h"
#include "ScriptConfiguration.h"

namespace Script
{
	ScriptSystemScene::~ScriptSystemScene( )
	{
		Management::GetInstance( )->GetEventManager( )->RemoveEventListener( ALL_EVENTS, this, &ScriptSystemScene::OnEvent );

		for( ScriptComponentList::iterator i = _components.begin( ); i != _components.end( ); ++i )
		{
			delete ( *i );
		}

		delete _eventHandlers;
		_eventHandlers = 0;

		delete _scriptConfiguration;
		_scriptConfiguration = 0;

		lua_close( _state );
		_state = 0;
	}

	ScriptSystemScene::ScriptSystemScene( Configuration::IConfiguration* configuration )
	{
		_scriptConfiguration = new ScriptConfiguration( configuration );
		_state = lua_open( );
		_eventHandlers = new EventHandlerList( );
	}

	ISystemComponent* ScriptSystemScene::CreateComponent( const std::string& name, const std::string& type )
	{
		int top = lua_gettop( _state ); 
		lua_getfield( _state, LUA_REGISTRYINDEX, "Scripts" ); // top + 1 

		lua_State *childState = lua_newthread( _state ); // top + 2 

		ScriptComponent* component = new ScriptComponent( name, childState );
		
		lua_newtable( _state );  // a global table for this script 
		lua_newtable( _state );  // meta table 
		
		lua_getfenv( _state,top + 2 ); // that returns the global table (we are	going to protect) 
		lua_setfield( _state, -2, "__index" ); // set global table as __index of the thread 
		lua_setmetatable( _state, -2 );
		lua_setfenv( _state,top + 2 );  // set environment of the new thread

		luabind::globals( childState )[ "Script" ] = component;

		_components.push_back( component );

		return component;
	}

	void ScriptSystemScene::DestroyComponent( ISystemComponent* component )
	{
		for( ScriptComponentList::iterator i = _components.begin( ); i != _components.end( ); ++i )
		{
			if ( ( *i )->GetName( ) == component->GetName( ) )
			{
				_components.erase( i );
				component->Destroy( );
				delete component;
				component = 0;
				return;
			}
		}
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
				.def( "getName", &ScriptComponent::GetName )
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
		luabind::set_pcall_callback( &ScriptSystemScene::Script_PError );
		luabind::set_error_callback( &ScriptSystemScene::Script_Error );
		luabind::set_cast_failed_callback( &ScriptSystemScene::Script_CastError );

		Management::GetInstance( )->GetEventManager( )->AddEventListener( ALL_EVENTS, this, &ScriptSystemScene::OnEvent );
	}

	void ScriptSystemScene::OnEvent( const IEvent* event )
	{
		EventHandlerList handlers( *_eventHandlers );

		for ( EventHandlerList::iterator i = handlers.begin( ); i != handlers.end( ); ++i )
		{
			if ( ( *i ).first == event->GetEventType( ) )
			{
				EventType eventType = event->GetEventType( );
				if ( event->GetEventType( ) == ALL_EVENTS )
				{
					ScriptEvent* scriptEvent = ( ScriptEvent* ) event;
					call_function< void >( ( *i ).second, scriptEvent->GetEventName( ), scriptEvent->GetValue1( ), scriptEvent->GetValue2( ) );
				}
				else
				{
					call_function< void >( ( *i ).second );
				}
			}
		}
	}

	int ScriptSystemScene::Script_PError( lua_State* luaState )
	{
		lua_Debug d;

		int result = 0;

		result = lua_getstack( luaState, 1, &d );
		result = lua_getinfo( luaState, "Sln", &d );

		std::string error = lua_tostring( luaState, -1 );
		lua_pop( luaState, 1 );

		std::stringstream errorMessage;
		errorMessage << "Script Error: " << d.short_src << ":" << d.currentline;

		if ( d.name != 0 )
		{
			errorMessage << "(" << d.namewhat << " " << d.name << ")";
		}

		errorMessage << " " << error;

		lua_pushstring( luaState, errorMessage.str( ).c_str( ) );

		ScriptException e( errorMessage.str( ) );
		Logger::Fatal( errorMessage.str( ) );

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
			( *i )->Update( deltaMilliseconds );
		}
	}
}