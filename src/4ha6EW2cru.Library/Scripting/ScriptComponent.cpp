#include "ScriptComponent.h"

#include "../Logging/Logger.h"
#include "../Exceptions/ScriptException.hpp"
#include "../System/Management.h"

extern "C" 
{
#	include <lualib.h>
#	include <lauxlib.h>
}

void ScriptComponent::Initialize( AnyValueMap properties )
{
	for ( AnyValueMap::iterator i = properties.begin( ); i != properties.end( ); ++i )
	{
		if ( ( *i ).first == "scriptPath" )
		{
			std::string scriptPath = ( *i ).second.GetValue< std::string >( );
			FileBuffer* scriptBuffer = Management::GetInstance( )->GetFileManager( )->GetFile( scriptPath, false );

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