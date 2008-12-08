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

	module( _masterState )
	[
		def( "quit", &ScriptManager::FromLua_GameQuit )
	];

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

	module( script->GetState( ) )
	[

		def( "print", &ScriptManager::FromLua_Print )

	];

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