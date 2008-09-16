#include "Script.h"

#include "../Logging/Logger.h"
#include "../Events/EventListener.h"
#include "../Events/EventManager.h"
#include "../Exceptions/ScriptException.hpp"

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
	Script* script = new Script( );
	script->_fileBuffer = fileBuffer;
	return script;
}

bool Script::Initialize( )
{
	bool result = false;

	if ( _fileBuffer != 0 )
	{
		if ( _fileBuffer->fileBytes != 0 )
		{
			std::stringstream loadingMessage;
			loadingMessage << "Loading Script: " << _fileBuffer->filePath;
			Logger::GetInstance( )->Fatal( loadingMessage.str( ) );

			_luaState = lua_open( );
			luabind::open( _luaState );

			luaL_loadbuffer( _luaState, _fileBuffer->fileBytes, _fileBuffer->fileSize, _fileBuffer->filePath.c_str( ) );
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

			result = true;
		}
		else
		{
			Logger::GetInstance( )->Fatal( "Cannot intitialize script, fileBuffer is has no bytes!" );
		}
	}
	else
	{
		Logger::GetInstance( )->Fatal( "Cannot intitialize script, fileBuffer is NULL" );
	}

	return result;
}

void Script::CallFunction( const std::string functionName )
{
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