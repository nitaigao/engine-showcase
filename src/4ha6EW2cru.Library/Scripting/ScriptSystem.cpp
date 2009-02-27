#include "ScriptSystem.h"

#include "../Logging/Logger.h"
#include "../IO/FileManager.h"

#include "../Exceptions/ScriptException.hpp"

ScriptSystem::ScriptSystem( IConfiguration* configuration )
{
	this->Constructor( configuration, new FileManager( ) );
}

ScriptSystem::ScriptSystem( IConfiguration* configuration, IFileManager* fileManager )
{
	this->Constructor( configuration, fileManager );
}

ScriptSystem::~ScriptSystem( )
{
	for( ScriptObjectList::iterator i = _scriptObjects.begin( ); i != _scriptObjects.end( ); ++i )
	{
		delete ( *i );
	}

	delete _fileManager;

	lua_close( _state );
	_state = 0;
}

void ScriptSystem::Initialize( )
{
	luaL_openlibs( _state );
	luabind::open( _state );
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