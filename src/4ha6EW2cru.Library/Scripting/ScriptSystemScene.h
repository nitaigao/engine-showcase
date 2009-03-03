#ifndef __SCRIPTSYSTEMSCENE_H
#define __SCRIPTSYSTEMSCENE_H

#include <vector>

#include "ScriptConfiguration.h"
#include "../System/ISystem.hpp"
#include "../System/ISystemComponent.hpp"
#include "../System/Configuration.h"
#include "../IO/IFileManager.hpp"
#include "../Events/IEvent.hpp"
#include "../Events/EventType.hpp"

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

	ISystemComponent* CreateComponent( const std::string& name );
	void DestroyComponent( ISystemComponent* component );;
	inline SystemType GetType( ) { return ScriptSystemType; };

	void Initialize( );

	void RegisterEvent( EventType eventType, object function );
	void UnRegisterEvent( EventType eventType, object function );
	void BroadcastEvent( EventType eventType );

private:

	static void FromLua_Print( const std::string message );
	static int FromLua_ScriptError( lua_State* luaState );

	void OnEvent( const IEvent* event );

	ISystem* _system;
	Configuration* _configuration;
	ScriptConfiguration* _scriptConfiguration;
	IFileManager* _fileManager;

	lua_State* _state;
	ScriptObjectList _scriptObjects;
	EventHandlerList* _eventHandlers;

};

#endif