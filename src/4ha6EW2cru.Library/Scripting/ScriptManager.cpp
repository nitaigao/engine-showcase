#include "ScriptManager.h"

#include <luabind/luabind.hpp>
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

ScriptManager::ScriptManager( )
{
	_masterState = lua_open( );
	luabind::open( _masterState );

	this->RegisterScriptGlobals( _masterState );

	luabind::set_pcall_callback( &ScriptManager::FromLua_ScriptError );

	EventManager::GetInstance( )->AddEventListener( SCRIPT_COMMAND_EXECUTED, this, &ScriptManager::CommandExecuted );
}

void ScriptManager::FromLua_GameQuit( void )
{
	EventManager::GetInstance( )->QueueEvent( new Event( GAME_QUIT ) );
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

ScriptManager::~ScriptManager( )
{
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

bool ScriptManager::Initialize( )
{
	Logger::GetInstance( )->Info( "ScriptManager::Initialize - Initializing ScriptManager" );

	if ( g_ScriptManagerInstance != 0 )
	{
		AlreadyInitializedException e( "ScriptManager::Initialize - ScriptManager has already been Initialized" );
		Logger::GetInstance( )->Fatal( e.what ( ) );
		throw e;
	}

	g_ScriptManagerInstance = new ScriptManager( );

	return true;	
}

Script* ScriptManager::CreateScript( std::string scriptPath )
{
	FileBuffer* scriptBuffer = FileManager::GetInstance( )->GetFile( scriptPath );
	Script* script = new Script( _masterState, scriptBuffer );

	this->RegisterScriptGlobals( script->GetState( ) );

	return script;
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

void ScriptManager::FromLua_Print( const std::string message )
{
	Logger::GetInstance( )->Info( message );
}

void ScriptManager::FromLua_LoadLevel( const std::string levelName )
{
	LevelChangedEventData* eventData = new LevelChangedEventData( levelName );
	EventManager::GetInstance( )->QueueEvent( new Event( GAME_LEVEL_CHANGED, eventData ) );
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

void ScriptManager::RegisterScriptGlobals( lua_State* luaState )
{
	module( luaState )
	[
		def( "quit", &ScriptManager::FromLua_GameQuit ),
		def( "print", &ScriptManager::FromLua_Print ),
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
				value( "INPUT_KEY_UP", INPUT_KEY_UP ),
				value( "LOG_MESSAGE_APPENDED", LOG_MESSAGE_APPENDED )
			]

	];
}