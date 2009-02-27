#ifndef __SCRIPTSYSTEM_H
#define __SCRIPTSYSTEM_H

#include "../System/ISystem.hpp"
#include "ScriptObject.h"

#include "../IO/IFileManager.hpp"
#include "../System/IConfiguration.hpp"

#include "../Events/IEvent.hpp"
#include "../Events/EventType.hpp"

extern "C" 
{
#	include <lua.h>
#	include <lualib.h>
}

#include <luabind/luabind.hpp>
using namespace luabind;

class ScriptSystem : public ISystem
{
	typedef std::pair< EventType, object > EventHandler;
	typedef std::vector< EventHandler > EventHandlerList;
	typedef std::vector< ScriptObject* > ScriptObjectList;

public:

	ScriptSystem( IConfiguration* configuration );
	ScriptSystem( IConfiguration* configuration, IFileManager* fileManager );

	virtual ~ScriptSystem( );

	void Initialize( );
	void Update( float deltaMilliseconds ) { };
	void Release( ) { };

	inline SystemType GetSystemType( ) { return ScriptSystemType; };

	inline PropertyMap GetProperties( ) { return _properties; };

	ISystemObject* CreateObject( const std::string& name, SystemType systemType );

	void RegisterEvent( EventType eventType, object function );

	void UnRegisterEvent( EventType eventType, object function );

private:

	ScriptSystem( const ScriptSystem & copy ) { };
	ScriptSystem & operator = ( const ScriptSystem & copy ) { return *this; };

	void Constructor( IConfiguration* configuration, IFileManager* fileManager );

	static void FromLua_Quit( void );
	static void FromLua_Print( const std::string message );
	static int FromLua_ScriptError( lua_State* luaState );
	static void FromLua_RegisterEvent( EventType eventType, object function );
	static void FromLua_UnRegisterEvent( EventType eventType, object function );
	static void FromLua_BroadcastEvent( EventType eventType );

	void OnEvent( const IEvent* event );

	PropertyMap _properties;
	IConfiguration* _configuration;
	IFileManager* _fileManager;

	lua_State* _state;
	ScriptObjectList _scriptObjects;
	EventHandlerList* _eventHandlers;

};

#endif