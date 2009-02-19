#include "ScriptManager.h"

#include <luabind/luabind.hpp>
#include <luabind/iterator_policy.hpp>
#include <luabind/copy_policy.hpp>
using namespace luabind;

#include "../Logging/Logger.h"
#include "../Events/EventManager.h"
#include "../IO/FileManager.h"

#include "../Events/Event.h"
#include "../Events/EventData.hpp"

#include "../Exceptions/AlreadyInitializedException.hpp"
#include "../Exceptions/UnInitializedException.hpp"
#include "../Exceptions/NullReferenceException.hpp"
#include "../Exceptions/ScriptException.hpp"

static ScriptManager* g_ScriptManagerInstance = 0;

ScriptManager::ScriptManager( Configuration* configuration )
{
	_configuration = configuration;
	_masterState = lua_open( );
	luaL_openlibs( _masterState );
	luabind::open( _masterState );

	_eventHandlers = new EventHandlerList( );

	module( _masterState )
	[
		def( "quit", &ScriptManager::FromLua_GameQuit ),
		def( "print", &ScriptManager::FromLua_Print ),
		def( "registerEvent", &ScriptManager::FromLua_RegisterEvent ),
		def( "unregisterEvent", &ScriptManager::FromLua_UnRegisterEvent ),
		def( "broadcastEvent", &ScriptManager::FromLua_BroadcastEvent ),
		def( "loadLevel", &ScriptManager::FromLua_LoadLevel ),

		class_< KeyEventData >( "KeyEventData" )
			.def( "getKeyCode", &KeyEventData::GetKeyCode )
			.def( "getKeyText", &KeyEventData::GetKeyText ),

		class_< Script >( "Script" )
			.def( "include", &Script::Include ),

		class_< AppenderEventData >( "AppenderEventData" )
			.def( "getMessage", &AppenderEventData::GetMessage ),

		class_< EventType >( "EventType" )
			.enum_( "constants" )
			[
				value( "TEST_EVENT", TEST_EVENT ),
				value( "SCRIPT_COMMAND_EXECUTED", SCRIPT_COMMAND_EXECUTED ),
				value( "LOG_MESSAGE_APPENDED", LOG_MESSAGE_APPENDED ),

				value( "INPUT_KEY_UP", INPUT_KEY_UP ),
				value( "INPUT_MOUSE_RELEASED", INPUT_MOUSE_RELEASED ),

				value( "UI_TITLE_SCREEN", UI_TITLE_SCREEN ),
				value( "UI_MAIN_MENU", UI_MAIN_MENU ),
				value( "UI_CLEAR", UI_CLEAR ),
				value( "UI_OPTIONS", UI_OPTIONS ),

				value( "GRAPHICS_SETTINGS_CHANGED", GRAPHICS_SETTINGS_CHANGED ),

				value( "GAME_INITIALIZED", GAME_INITIALIZED )
			],

		class_< Configuration >( "Config" )
			.property( "isFullScreen", &Configuration::IsFullScreen, &Configuration::SetFullScreen )
			.property( "displayWidth", &Configuration::GetDisplayWidth, &Configuration::SetDisplayWidth )
			.property( "displayHeight", &Configuration::GetDisplayHeight, &Configuration::SetDisplayHeight )
			.property( "availableVideoModes", &Configuration::GetAvailableVideoModes, &Configuration::SetAvailableVideoModes, return_stl_iterator )

		//class_< std::vector< std::string > >( "StringVector" )
	];

	luabind::globals( _masterState )[ "Configuration" ] = _configuration;

	luabind::set_pcall_callback( &ScriptManager::FromLua_ScriptError );

	EventManager::GetInstance( )->AddEventListener( ALL_EVENTS, this, &ScriptManager::OnEvent );
}

ScriptManager::~ScriptManager( )
{
	delete _eventHandlers;

	lua_close( _masterState );
	_masterState = 0;
}

ScriptManager* ScriptManager::GetInstance( )
{
	if ( 0 == g_ScriptManagerInstance )
	{
		UnInitializedException e( "ScriptManager::GetInstance - ScriptManager has not been Initialized" );
		Logger::GetInstance( )->Fatal( e.what( ) );
		throw e;
	}

	return g_ScriptManagerInstance;
}

void ScriptManager::Release( )
{
	Logger::GetInstance( )->Info( "ScriptManager::Release - Releasing Event Manager" );

	delete g_ScriptManagerInstance;
	g_ScriptManagerInstance = 0;
}

bool ScriptManager::Initialize( Configuration* configuration )
{
	Logger::GetInstance( )->Info( "ScriptManager::Initialize - Initializing ScriptManager" );

	if ( g_ScriptManagerInstance != 0 )
	{
		AlreadyInitializedException e( "ScriptManager::Initialize - ScriptManager has already been Initialized" );
		Logger::GetInstance( )->Fatal( e.what ( ) );
		throw e;
	}

	g_ScriptManagerInstance = new ScriptManager( configuration );

	return true;	
}

lua_State* ScriptManager::LoadScript( const std::string scriptPath )
{
	FileBuffer* scriptBuffer = FileManager::GetInstance( )->GetFile( scriptPath );
	
	lua_State* childState = lua_newthread( _masterState );
	
	int result = luaL_loadbuffer( childState, scriptBuffer->fileBytes, scriptBuffer->fileLength, scriptBuffer->filePath.c_str( ) );

	if ( LUA_ERRSYNTAX == result )
	{
		ScriptException syntaxE( "Script::Initialize - There is a syntax error within the Script" );
		Logger::GetInstance( )->Fatal( syntaxE.what( ) );
		throw syntaxE;
	}

	if ( LUA_ERRMEM == result )
	{
		ScriptException memE( "Script::Initialize - There is memory allocation error within the Script" );
		Logger::GetInstance( )->Fatal( memE.what( ) );
		throw memE;
	}

	delete scriptBuffer;

	_childStates.push_back( childState );
	
	return childState;
}

Script* ScriptManager::CreateScript( std::string scriptPath )
{
	FileBuffer* scriptBuffer = FileManager::GetInstance( )->GetFile( scriptPath );
	return new Script( _masterState, scriptBuffer );
}

void ScriptManager::DestroyScript( Script* script )
{
	if ( 0 == script )
	{
		NullReferenceException e( "ScriptManager::DestroyScript - The given Script is NULL" );
		Logger::GetInstance( )->Fatal( e.what( ) );
		throw e;
	}

	delete script;
	script = 0;
}

void ScriptManager::RegisterEvent( EventType eventType, object function )
{
	EventHandler eventHandler( eventType, function );
	_eventHandlers->push_back( eventHandler );
}

void ScriptManager::UnRegisterEvent( EventType eventType, object function )
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

void ScriptManager::CommandExecuted( const IEvent* event )
{
	ScriptCommandEventData* eventData = static_cast< ScriptCommandEventData* >( event->GetEventData( ) );

	int result = luaL_dostring( _masterState, eventData->GetCommand( ).c_str( ) );

	if( 0 != result )
	{
		std::string error = lua_tostring( _masterState, -1 );
		Logger::GetInstance( )->Warn( error );
	}
}

void ScriptManager::FromLua_Print( const std::string message )
{
	Logger::GetInstance( )->Info( message );
}

void ScriptManager::FromLua_LoadLevel( const std::string levelName )
{
	LevelChangedEventData* eventData = new LevelChangedEventData( levelName );
	EventManager::GetInstance( )->QueueEvent( new Event( GAME_LEVEL_CHANGED, eventData ) );
}

void ScriptManager::FromLua_BroadcastEvent( EventType eventType )
{
	Event* event = new Event( eventType );
	EventManager::GetInstance( )->QueueEvent( event );
}

int ScriptManager::FromLua_ScriptError( lua_State* luaState )
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
	//throw e; -- otherwise the console throws errors
	
	return 1;	
}

void ScriptManager::FromLua_GameQuit( void )
{
	EventManager::GetInstance( )->QueueEvent( new Event( GAME_QUIT ) );
}

void ScriptManager::FromLua_RegisterEvent( EventType eventType, object function )
{
	ScriptManager::GetInstance( )->RegisterEvent( eventType, function );
}

void ScriptManager::FromLua_UnRegisterEvent( EventType eventType, object function )
{
	ScriptManager::GetInstance( )->UnRegisterEvent( eventType, function );
}

void ScriptManager::OnEvent( const IEvent* event )
{
	EventHandlerList handlers( *_eventHandlers );

	for ( EventHandlerList::iterator i = handlers.begin( ); i != handlers.end( ); ++i )
	{
		if ( ( *i ).first == event->GetEventType( ) )
		{
			call_function< void >( ( *i ).second );
		}
	}
}