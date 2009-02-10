#include "Script.h"

#include "../Events/EventType.hpp"
#include "../Events/EventData.hpp"
#include "../Events/EventListener.h"
#include "../Events/EventManager.h"

#include "../Exceptions/ScriptException.hpp"
#include "../Exceptions/AlreadyInitializedException.hpp"

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

	if ( fileBuffer->fileBytes == 0 || fileBuffer->fileLength == 0 )
	{
		delete fileBuffer;

		UnInitializedException unInitE( "Script::Script - Cannot Create from an Empty File Buffer" );
		Logger::GetInstance( )->Fatal( unInitE.what( ) );
		throw unInitE;
	}

	_luaState = _parentState;
}

Script::~Script( )
{
	delete _fileBuffer;
}

void Script::Initialize( )
{
	if ( _isInitialized )
	{
		AlreadyInitializedException aiE( "Script::Initialize - Script has already been Initialized" );
		Logger::GetInstance( )->Fatal( aiE.what( ) );
		throw aiE;
	}

	int result = luaL_loadbuffer( _luaState, _fileBuffer->fileBytes, _fileBuffer->fileLength, _fileBuffer->filePath.c_str( ) );

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

	_isInitialized = true;
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

	if ( scriptBuffer->fileBytes == 0 || scriptBuffer->fileLength == 0 )
	{
		delete scriptBuffer;

		UnInitializedException unInitE( "Script::Include - Cannot Create from an Empty File Buffer" );
		Logger::GetInstance( )->Fatal( unInitE.what( ) );
		throw unInitE;
	}

	int result = luaL_loadbuffer( _luaState, scriptBuffer->fileBytes, scriptBuffer->fileLength, scriptBuffer->filePath.c_str( ) );
	delete scriptBuffer;

	lua_pcall( _luaState, 0, 0, 0 );
}