#include "ScriptSystemScene.h"


#include "../Events/IEvent.hpp"
#include "../Events/Event.h"

#include "ScriptEvent.hpp"
#include "ScriptComponent.h"
#include "ScriptConfiguration.h"

#include "../Logging/Logger.h"
#include "../IO/FileManager.h"
#include "../System/Management.h"
#include "../Exceptions/ScriptException.hpp"
#include <luabind/iterator_policy.hpp>

ScriptSystemScene::~ScriptSystemScene( )
{
	Management::GetInstance( )->GetEventManager( )->RemoveEventListener( ALL_EVENTS, this, &ScriptSystemScene::OnEvent );

	for( ScriptObjectList::iterator i = _scriptObjects.begin( ); i != _scriptObjects.end( ); ++i )
	{
		delete ( *i );
	}

	delete _eventHandlers;
	delete _scriptConfiguration;

	lua_close( _state );
	_state = 0;
}

ScriptSystemScene::ScriptSystemScene( ISystem* scriptSystem, Configuration* configuration )
{
	_scriptConfiguration = new ScriptConfiguration( configuration );
	_state = lua_open( );
	_eventHandlers = new EventHandlerList( );
	_system = scriptSystem;
}

ISystemComponent* ScriptSystemScene::CreateComponent( const std::string& name, const std::string& type )
{
	lua_State* childState = lua_newthread( _state );

	ScriptComponent* scriptComponent = new ScriptComponent( name, childState );

	_scriptObjects.push_back( scriptComponent );

	return scriptComponent;
}

void ScriptSystemScene::DestroyComponent( ISystemComponent* component )
{

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
			.property( "availableVideoModes", &ScriptConfiguration::GetAvailableVideoModes, &ScriptConfiguration::SetAvailableVideoModes, return_stl_iterator )
			.property( "isConsole", &ScriptConfiguration::IsConsole, &ScriptConfiguration::SetConsole ),

		class_< std::vector< std::string > >( "StringList" ),

		class_< EventType >( "EventType" )
			.enum_( "constants" )
			[
				value( "INPUT_KEY_UP", INPUT_KEY_UP ),
				value( "INPUT_MOUSE_RELEASED", INPUT_MOUSE_RELEASED ),

				value( "GRAPHICS_SETTINGS_CHANGED", GRAPHICS_SETTINGS_CHANGED ),

				value( "UI_TITLE_SCREEN", UI_TITLE_SCREEN ),
				value( "UI_MAIN_MENU", UI_MAIN_MENU ),
				value( "UI_PAUSE_MENU", UI_PAUSE_MENU ),
				value( "UI_CLEAR", UI_CLEAR ),
				value( "UI_OPTIONS", UI_OPTIONS ),
				value( "UI_CONSOLE", UI_CONSOLE ),
				value( "LOG_MESSAGE_APPENDED", LOG_MESSAGE_APPENDED ),

				value( "GAME_INITIALIZED", GAME_INITIALIZED )
			]
	];

	luabind::globals( _state )[ "Configuration" ] = _scriptConfiguration;
	luabind::set_pcall_callback( &ScriptSystemScene::Script_PError );
	luabind::set_error_callback( &ScriptSystemScene::Script_Error );
	luabind::set_cast_failed_callback( &ScriptSystemScene::Script_CastError );

	Management::GetInstance( )->GetEventManager( )->AddEventListener( ALL_EVENTS, this, &ScriptSystemScene::OnEvent );
}

void ScriptSystemScene::RegisterEvent( EventType eventType, object function )
{
	EventHandler eventHandler( eventType, function );
	_eventHandlers->push_back( eventHandler );
}

void ScriptSystemScene::UnRegisterEvent( EventType eventType, object function )
{
	for( EventHandlerList::iterator i = _eventHandlers->begin( ); i != _eventHandlers->end( ); ++i )
	{
		if ( ( *i ).first == eventType && ( *i ).second == function )
		{
			_eventHandlers->erase( i );
			return;
		}
	}
}

void ScriptSystemScene::BroadcastEvent( EventType eventType )
{
	Management::GetInstance( )->GetEventManager( )->QueueEvent( new Event( eventType ) );
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

	result = lua_getstack( luaState, 0, &d );
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
	Logger::GetInstance( )->Fatal( errorMessage.str( ) );

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

void ScriptSystemScene::Print( const std::string message )
{
	Logger::GetInstance( )->Info( message );
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

void ScriptSystemScene::ExecuteString( const std::string input )
{
	luaL_dostring( _state, input.c_str( ) );
}

void ScriptSystemScene::EndGame()
{
	Management::GetInstance( )->GetEventManager( )->QueueEvent( new ScriptEvent( "GAME_ENDED" ) );
	Management::GetInstance( )->GetEventManager( )->QueueEvent( new Event( GAME_ENDED ) );
}