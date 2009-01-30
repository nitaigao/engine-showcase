#ifndef __SCRIPTMANAGER_H
#define __SCRIPTMANAGER_H

extern "C" 
{
#	include <lua.h>
}

#include "Script.h"

class ScriptManager
{

public:

	ScriptManager::ScriptManager( );
	ScriptManager::~ScriptManager( );

	/*! Retrieves an instance of the ScriptManager Singleton */
	static ScriptManager* GetInstance( );

	/*! Releases all resources and the ScriptManager Singleton */
	void Release( );

	/*! Initializes the Script Management mechanism */
	static bool Initialize( );

	/*! Updates the ScriptManager and it's Children */
	//void Update( );

	/*! Creates a new Script */
	Script* CreateScript( std::string scriptPath );

	/*! Removes a Script from the Master State and Destroys it */
	void DestroyScript( Script* script );

private:

	/*! Registers Global Functions such as 'quit', 'print' */
	void RegisterScriptGlobals( lua_State* luaState );

	/*! Listens for Execute Commands */
	void CommandExecuted( const IEvent* event );

	static void FromLua_GameQuit( void );
	static void FromLua_LoadLevel( const std::string levelName );
	static void FromLua_Print( const std::string message );
	static int FromLua_ScriptError( lua_State* luaState );

	lua_State* _masterState;

};

#endif