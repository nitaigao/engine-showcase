#include "ScriptComponent.h"

#include "../Logging/Logger.h"
#include "../Exceptions/ScriptException.hpp"
#include "../IO/FileManager.h"

extern "C" 
{
#	include <lualib.h>
#	include <lauxlib.h>
}

void ScriptComponent::Initialize( SystemPropertyList properties )
{
	for ( SystemPropertyList::iterator i = properties.begin( ); i != properties.end( ); ++i )
	{
		if ( ( *i ).first == "scriptPath" )
		{
			FileManager* fileManager = new FileManager( );
			FileBuffer* scriptBuffer = fileManager->GetFile( ( *i ).second.GetValue< std::string >( ) );
			delete fileManager;

			int result = luaL_loadbuffer( _state, scriptBuffer->fileBytes, scriptBuffer->fileLength, scriptBuffer->filePath.c_str( ) );

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
		}
	}
}