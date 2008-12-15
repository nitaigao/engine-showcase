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

	luabind::globals( _luaState )[ "script" ] = this;
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