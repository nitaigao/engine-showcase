#include "ScriptSystem.h"

#include "../Logging/Logger.h"
#include "../IO/FileManager.h"

#include "../System/Management.h"

#include "../Exceptions/ScriptException.hpp"

static ScriptSystem* g_scriptSystem = 0;

ScriptSystem::ScriptSystem( IConfiguration* configuration )
{
	this->Constructor( configuration, new FileManager( ) );
}

ScriptSystem::ScriptSystem( IConfiguration* configuration, IFileManager* fileManager )
{
	this->Constructor( configuration, fileManager );
}

void ScriptSystem::Constructor( IConfiguration* configuration, IFileManager* fileManager )
{
	g_scriptSystem = this;
	_fileManager = fileManager;

	_configuration = configuration;
	_state = lua_open( );
	_eventHandlers = new EventHandlerList( );
}

ScriptSystem::~ScriptSystem( )
{
	Management::GetInstance( )->GetEventManager( )->RemoveEventListener( ALL_EVENTS, this, &ScriptSystem::OnEvent );

	for( ScriptObjectList::iterator i = _scriptObjects.begin( ); i != _scriptObjects.end( ); ++i )
	{
		delete ( *i );
	}

	delete _eventHandlers;
	delete _fileManager;

	lua_close( _state );
	_state = 0;
}

void ScriptSystem::Initialize( )
{
	luaL_openlibs( _state );
	luabind::open( _state );

	module( _state )
	[
		def( "print", &ScriptSystem::FromLua_Print ),
		def( "registerEvent", &ScriptSystem::FromLua_RegisterEvent ),
		def( "unregisterEvent", &ScriptSystem::FromLua_UnRegisterEvent ),

		class_< EventType >( "EventType" )
			.enum_( "constants" )
			[
				value( "INPUT_KEY_UP", INPUT_KEY_UP ),
				value( "INPUT_MOUSE_RELEASED", INPUT_MOUSE_RELEASED ),

				value( "UI_TITLE_SCREEN", UI_TITLE_SCREEN ),
				value( "UI_MAIN_MENU", UI_MAIN_MENU ),
				value( "UI_CLEAR", UI_CLEAR ),
				value( "UI_OPTIONS", UI_OPTIONS ),

				value( "GAME_INITIALIZED", GAME_INITIALIZED )
			]
	];

	luabind::set_pcall_callback( &ScriptSystem::FromLua_ScriptError );

	Management::GetInstance( )->GetEventManager( )->AddEventListener( ALL_EVENTS, this, &ScriptSystem::OnEvent );
}

ISystemObject* ScriptSystem::CreateObject( const std::string& name, SystemType systemType )
{
	FileBuffer* scriptBuffer = _fileManager->GetFile( name );

	lua_State* childState = lua_newthread( _state );

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

	ScriptObject* scriptObject = new ScriptObject( childState );
	_scriptObjects.push_back( scriptObject );

	return scriptObject;
}

void ScriptSystem::RegisterEvent( EventType eventType, object function )
{
	EventHandler eventHandler( eventType, function );
	_eventHandlers->push_back( eventHandler );
}

void ScriptSystem::UnRegisterEvent( EventType eventType, object function )
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

void ScriptSystem::OnEvent( const IEvent* event )
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

int ScriptSystem::FromLua_ScriptError( lua_State* luaState )
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

void ScriptSystem::FromLua_Print( const std::string message )
{
	Logger::GetInstance( )->Info( message );
}

void ScriptSystem::FromLua_RegisterEvent( EventType eventType, object function )
{
	g_scriptSystem->RegisterEvent( eventType, function );
}

void ScriptSystem::FromLua_UnRegisterEvent( EventType eventType, object function )
{
	g_scriptSystem->UnRegisterEvent( eventType, function );
}