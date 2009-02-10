#ifndef __SCRIPTMANAGER_H
#define __SCRIPTMANAGER_H

extern "C" 
{
#	include <lua.h>
}

#include <luabind/luabind.hpp>
using namespace luabind;

#include "Script.h"
#include "../System/Configuration.h"

class ScriptManager
{

	typedef std::pair< EventType, object > EventHandler;
	typedef std::vector< EventHandler > EventHandlerList;

	typedef std::vector< lua_State* > ChildStateList;

public:

	ScriptManager( Configuration* configuration );
	~ScriptManager( );

	/*! Retrieves an instance of the ScriptManager Singleton */
	static ScriptManager* GetInstance( );
	
	/*! Releases all resources and the ScriptManager Singleton */
	void Release( );

	/*! Initializes the Script Management mechanism */
	static bool Initialize( Configuration* configuration );

	void Update( );

	/*! Loads a Script */
	lua_State* LoadScript( const std::string scriptPath );

	/*! Creates a new Script */
	Script* CreateScript( std::string scriptPath );

	/*! Removes a Script from the Master State and Destroys it */
	void DestroyScript( Script* script );

	void RegisterEvent( EventType eventType, object function );
	void UnRegisterEvent( EventType eventType, object function );

private:

	void OnEvent( const IEvent* event );

	/*! Registers Global Functions such as 'quit', 'print' */
	void RegisterScriptGlobals( lua_State* luaState );

	/*! Listens for Execute Commands */
	void CommandExecuted( const IEvent* event );

	static void FromLua_GameQuit( void );
	static void FromLua_LoadLevel( const std::string levelName );
	static void FromLua_Print( const std::string message );
	static int FromLua_ScriptError( lua_State* luaState );
	static void FromLua_BroadcastEvent( EventType eventType );
	static void FromLua_RegisterEvent( EventType eventType, object function );
	static void FromLua_UnRegisterEvent( EventType eventType, object function );

	lua_State* _masterState;
	ChildStateList _childStates;
	EventHandlerList* _eventHandlers;
	EventHandlerList* _eventHandlersPurge;
	Configuration* _configuration;

};

#endif