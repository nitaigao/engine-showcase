#include "Script.h"

#include "../Logging/Logger.h"

#include "../Events/EventType.hpp"
#include "../Events/EventData.hpp"
#include "../Events/EventListener.h"
#include "../Events/EventManager.h"

#include "../Exceptions/UnInitializedException.hpp"
#include "../Exceptions/ScriptException.hpp"
#include "../Exceptions/AlreadyInitializedException.hpp"
#include "../Exceptions/OutOfRangeException.hpp"

#include "../IO/FileBuffer.hpp"
#include "../IO/FileManager.h"

Script::Script( lua_State* parentState, FileBuffer* fileBuffer ) : _parentState( parentState ), _fileBuffer( fileBuffer ), _isInitialized( false )
{
	if ( 0 == fileBuffer )
	{
		NullReferenceException nullE( "Script::Script - The given FileBuffer is NULL" );
		Logger::GetInstance( )->Fatal( nullE.what( ) );
		throw nullE;
	}

	if ( fileBuffer->fileBytes == 0 || fileBuffer->fileSize == 0 )
	{
		delete fileBuffer;

		UnInitializedException unInitE( "Script::Script - Cannot Create from an Empty File Buffer" );
		Logger::GetInstance( )->Fatal( unInitE.what( ) );
		throw unInitE;
	}

	_luaState = lua_open( );
	luabind::open( _luaState );
}

Script::~Script( )
{
	delete _fileBuffer;

	for( EventHandlers::iterator i = _eventHandlers.begin( ); i != _eventHandlers.end( ); ++i )
	{
		EventManager::GetInstance( )->RemoveEventListener( ( *i ).first, this, &Script::ToLua_EventHandler );
	}

	_eventHandlers.clear( );

	lua_close( _luaState );
	_luaState = 0;
}

void Script::Initialize( )
{
	if ( _isInitialized )
	{
		AlreadyInitializedException aiE( "Script::Initialize - Script has already been Initialized" );
		Logger::GetInstance( )->Fatal( aiE.what( ) );
		throw aiE;
	}

	int result = luaL_loadbuffer( _luaState, _fileBuffer->fileBytes, _fileBuffer->fileSize, _fileBuffer->filePath.c_str( ) );

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

	module( _luaState )
	[
		//def( "print",  &Script::LogMessage ),
		def( "addEventListener", &Script::FromLua_AddEventListener ),
		def( "removeEventListiner", &Script::FromLua_RemoveEventListener ),

		class_< Script >( "Script" )
			.def( "include", &Script::Include )
			.def( "addEventListener", &Script::AddEventListener )
			.def( "removeEventListener", &Script::RemoveEventListener ),

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

	luabind::globals( _luaState )[ "script" ] = this;
	luabind::set_pcall_callback( &Script::FromLua_ScriptError );
	lua_pcall( _luaState, 0, 0, 0 );

	_isInitialized = true;
}

void Script::CallFunction( const std::string functionName )
{
	if ( !_isInitialized )
	{
		UnInitializedException unE( "Script::CallFunction - Script is not Initialized" );
		Logger::GetInstance( )->Fatal( unE.what( ) );
		throw unE;
	}

	if( functionName.empty( ) )
	{
		OutOfRangeException outE( "Script::CallFunction - The given function name is empty" );
		Logger::GetInstance( )->Fatal( outE.what( ) );
		throw outE;
	}
	
	call_function< int >( _luaState, functionName.c_str( ), this );
}

void Script::FromLua_AddEventListener( Script* script, EventType eventType, object handlerFunction )
{
	script->AddEventListener( eventType, handlerFunction );
}

void Script::AddEventListener( const EventType eventType, object handlerFunction )
{
	_eventHandlers.insert( std::pair< EventType, object >( eventType, handlerFunction ) );

	EventManager::GetInstance( )->AddEventListener( eventType, this, &Script::ToLua_EventHandler );
}

void Script::FromLua_RemoveEventListener( Script* script, EventType eventType, object handlerFunction )
{
	script->RemoveEventListener( eventType, handlerFunction );
}

void Script::RemoveEventListener( const EventType eventType, const object handlerFunction )
{
	std::pair< 
			EventHandlers::iterator, 
			EventHandlers::iterator
		> handlers = _eventHandlers.equal_range( eventType ); 

	for( EventHandlers::iterator i = handlers.first; i != handlers.second; ++i )
	{
		EventManager::GetInstance( )->RemoveEventListener( eventType, this, &Script::ToLua_EventHandler );
		_eventHandlers.erase( i );
		return;
	}
}

void Script::ToLua_EventHandler( const IEvent* event )
{
	object eventHandler = _eventHandlers[ event->GetEventType( ) ];

	switch( event->GetEventType( ) )
	{
		
	case LOG_MESSAGE_APPENDED:

		call_function< void >( eventHandler, static_cast< AppenderEventData* >( event->GetEventData( ) ) );

		break;

	default:

		call_function< void >( eventHandler );

		break;
	}
}

int Script::FromLua_ScriptError( lua_State* luaState )
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
	//throw e;
	
	return 1;	
}

void Script::Include( std::string scriptPath )
{
	FileBuffer* scriptBuffer = FileManager::GetInstance( )->GetFile( scriptPath );

	if ( 0 == scriptBuffer )
	{
		NullReferenceException nullE( "Script::Include - The given FileBuffer is NULL" );
		Logger::GetInstance( )->Fatal( nullE.what( ) );
		throw nullE;
	}

	if ( scriptBuffer->fileBytes == 0 || scriptBuffer->fileSize == 0 )
	{
		delete scriptBuffer;

		UnInitializedException unInitE( "Script::Include - Cannot Create from an Empty File Buffer" );
		Logger::GetInstance( )->Fatal( unInitE.what( ) );
		throw unInitE;
	}

	int result = luaL_loadbuffer( _luaState, scriptBuffer->fileBytes, scriptBuffer->fileSize, scriptBuffer->filePath.c_str( ) );
	delete scriptBuffer;

	lua_pcall( _luaState, 0, 0, 0 );
}