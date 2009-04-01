#ifndef __SCRIPTSYSTEMSCENE_H
#define __SCRIPTSYSTEMSCENE_H

#include <vector>

#include "../IO/IFileManager.hpp"
#include "../System/Configuration.h"
#include "ScriptConfiguration.h"
#include "../System/ISystem.hpp"
#include "../System/ISystemComponent.hpp"

#include "../Events/IEvent.hpp"
#include "../Events/EventType.hpp"
#include "../Events/EventData.hpp"

extern "C" 
{
#	include <lua.h>
#	include <lualib.h>
}

#include <luabind/luabind.hpp>
using namespace luabind;

class ScriptSystemScene : public ISystemScene
{
	typedef std::pair< EventType, object > EventHandler;
	typedef std::vector< EventHandler > EventHandlerList;
	typedef std::vector< ISystemComponent* > ScriptObjectList;

public:

	virtual ~ScriptSystemScene( );

	ScriptSystemScene( ISystem* scriptSystem, Configuration* configuration );

	ISystemComponent* CreateComponent( const std::string& name, const std::string& type );
	void DestroyComponent( ISystemComponent* component );;
	inline SystemType GetType( ) { return ScriptSystemType; };
	inline void Update( float deltaMilliseconds ) { };

	void Initialize( );

	void RegisterEvent( EventType eventType, object function );
	void UnRegisterEvent( EventType eventType, object function );
	void BroadcastEvent( EventType eventType );

	/*! Executes the specified string */
	void ExecuteString( const std::string input );

private:

	/*! -- Script Helpers -- */

	/*! Prints the specified message to the console */
	static void Print( const std::string message );

	/*! Gets called when an error occurs inside a running script */
	static int Script_PError( lua_State* luaState );

	/*! Gets called when an error occurs inside a script */
	static void Script_Error( lua_State* luaState );

	/*! Gets called when a casting error occurs inside a script */
	static void Script_CastError( lua_State* luaState, LUABIND_TYPE_INFO typeInfo );

	/*! Quits the game */
	static void Quit( );

	/*! Loads the specified level */
	static void LoadLevel( const std::string& levelName );

	/*! Ends the current Game */
	static void EndGame( );

	void OnEvent( const IEvent* event );

	ISystem* _system;
	Configuration* _configuration;
	ScriptConfiguration* _scriptConfiguration;

	lua_State* _state;
	ScriptObjectList _scriptObjects;
	EventHandlerList* _eventHandlers;

	ScriptSystemScene( ) { };
	ScriptSystemScene( const ScriptSystemScene & copy ) { };
	ScriptSystemScene & operator = ( const ScriptSystemScene & copy ) { return *this; };

};

#endif