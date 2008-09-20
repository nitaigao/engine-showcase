#include "Script.h"

#include "../Logging/Logger.h"
#include "../Events/EventListener.h"
#include "../Events/EventManager.h"

#include "../Exceptions/UnInitializedException.hpp"
#include "../Exceptions/ScriptException.hpp"
#include "../Exceptions/AlreadyInitializedException.hpp"
#include "../Exceptions/OutOfRangeException.hpp"

Script::~Script( )
{
	delete _fileBuffer;

	for( EventHandlers::iterator i = _eventHandlers.begin( ); i != _eventHandlers.end( ); ++i )
	{
		EventManager::GetInstance( )->RemoveEventListener( ( *i ).first, this, &Script::ToLua_EventHandler );
	}

	_eventHandlers.clear( );

	if ( _luaState != 0 )
	{
		lua_close( _luaState );
		_luaState = 0;
	}
}

Script* Script::CreateFromFileBuffer( FileBuffer* fileBuffer )
{
	if ( 0 == fileBuffer )
	{
		NullReferenceException nullE( "Script::CreateFromFileBuffer - The given FileBuffer is NULL" );
		Logger::GetInstance( )->Fatal( nullE.what( ) );
		throw nullE;
	}

	if ( fileBuffer->fileBytes == 0 || fileBuffer->fileSize == 0 )
	{
		delete fileBuffer;

		UnInitializedException unInitE( "Script::CreateFromFileBuffer - Cannot Initialize from an Empty File Buffer" );
		Logger::GetInstance( )->Fatal( unInitE.what( ) );
		throw unInitE;
	}

	Script* script = new Script( );
	script->_fileBuffer = fileBuffer;
	return script;
}

void Script::Initialize( )
{
	if ( _isInitialized )
	{
		AlreadyInitializedException aiE( "Script::Initialize - Script has already been Initialized" );
		Logger::GetInstance( )->Fatal( aiE.what( ) );
		throw aiE;
	}

	_luaState = lua_open( );
	luabind::open( _luaState );

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

	lua_pcall( _luaState, 0, 0, 0 );

	module( _luaState )
	[
		def( "print",  &Script::LogMessage ),
		def( "addEventListener", &Script::FromLua_AddEventListener ),

		class_< Script >( "Script" ),

		class_< EventType >( "EventType" )
			.enum_( "constants" )
			[
				value( "TEST_EVENT", TEST_EVENT ),
				value( "KEY_UP", KEY_UP )
			]
	];

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

	try
	{
		call_function< int >( _luaState, functionName.c_str( ), this );
	}
	catch( luabind::error& error )
	{
		object error_msg( from_stack( error.state( ), -1) );

		std::stringstream errorMessage;
		errorMessage << "Script Error: " << error.what( ) << ": " << error_msg;
		Logger::GetInstance( )->Fatal( errorMessage.str( ) );	

		throw ScriptException( errorMessage.str( ) );
	}
}

lua_State* Script::GetState( ) const
{
	 if ( !_isInitialized )
	 {
		 UnInitializedException e( "Script::GetState -Script is not Initialized" );
		 Logger::GetInstance( )->Fatal( e.what( ) );
		 throw e;
	 }

	 return _luaState;
}

void Script::LogMessage( const std::string message )
{
	Logger::GetInstance( )->Info( message );
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

void Script::ToLua_EventHandler( const IEvent* event )
{
	const IEvent* e = event;
	
	try
	{
		_eventHandlers[ event->GetEventType( ) ]( this );
	}
	catch( luabind::error& error )
	{
		object error_msg( from_stack( error.state( ), -1) );

		std::stringstream errorMessage;
		errorMessage << "Script Handle Event Error: " << error.what( ) << ": " << error_msg;
		Logger::GetInstance( )->Fatal( errorMessage.str( ) );	

		throw std::exception( errorMessage.str( ).c_str( ) );
	}
}