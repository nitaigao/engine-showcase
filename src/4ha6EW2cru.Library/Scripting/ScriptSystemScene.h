#ifndef __SCRIPTSYSTEMSCENE_H
#define __SCRIPTSYSTEMSCENE_H

#include <vector>

#include "ScriptConfiguration.h"

#include "../IO/IFileManager.hpp"
#include "../System/Configuration.h"
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

class ScriptSystemScene : public ISystemScene
{
	typedef std::pair< EventType, luabind::object > EventHandler;
	typedef std::vector< EventHandler > EventHandlerList;
	typedef std::vector< ISystemComponent* > ScriptComponentList;

public:

	virtual ~ScriptSystemScene( );

	ScriptSystemScene( Configuration* configuration );

	ISystemComponent* CreateComponent( const std::string& name, const std::string& type );
	void DestroyComponent( ISystemComponent* component );;
	inline SystemType GetType( ) { return ScriptSystemType; };
	inline void Update( float deltaMilliseconds ) { };

	void Initialize( );

	void RegisterEvent( EventType eventType, luabind::object function );
	void UnRegisterEvent( EventType eventType, luabind::object function );
	void BroadcastEvent( EventType eventType );

	/*! Returns the Master LUA State for the Scene */
	lua_State* GetState( ) const { return _state; };

	/*! Executes the specified string */
	void ExecuteString( const std::string& input );

	/*! Prints the specified message to the console */
	static void Print( const std::string& message );

	/*! Quits the game */
	static void Quit( );

	/*! Loads the specified level */
	static void LoadLevel( const std::string& levelName );

	/*! Ends the current Game */
	static void EndGame( );

	/*! Gets called when an error occurs inside a running script */
	static int Script_PError( lua_State* luaState );

	/*! Gets called when an error occurs inside a script */
	static void Script_Error( lua_State* luaState );

	/*! Gets called when a casting error occurs inside a script */
	static void Script_CastError( lua_State* luaState, LUABIND_TYPE_INFO typeInfo );


private:

	void OnEvent( const IEvent* event );

	ScriptConfiguration* _scriptConfiguration;

	lua_State* _state;
	ScriptComponentList _components;
	EventHandlerList* _eventHandlers;

	ScriptSystemScene( ) { };
	ScriptSystemScene( const ScriptSystemScene & copy ) { };
	ScriptSystemScene & operator = ( const ScriptSystemScene & copy ) { return *this; };

};

#endif