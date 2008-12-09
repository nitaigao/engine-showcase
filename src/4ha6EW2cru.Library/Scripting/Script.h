#ifndef __SCRIPT_H
#define __SCRIPT_H

#include <map>

extern "C" 
{
#	include <lua.h>
}

#include "../Events/EventType.hpp"
#include "../Events/IEvent.hpp"
#include "../IO/FileBuffer.hpp"

#include <luabind/luabind.hpp>
#include <luabind/adopt_policy.hpp>
#include <luabind/return_reference_to_policy.hpp>
using namespace luabind;

/*!
	An interface for loading LUA scripts
*/
class Script
{

	typedef std::map< EventType, object > EventHandlers;
	
public:

	Script( lua_State* parentState, FileBuffer* fileBuffer );
	~Script( );

	/*! Initializes the Script from a FileBuffer */
	void Initialize( );

	/*! Allows the specified function to be called from C++ */
	void CallFunction( const std::string functionName );

	/*! Returns the Script state */
	inline lua_State* GetState( ) const { return _luaState; };
	
	/*! Receives addEventListener request from LUA and forwards to the Script Instance */
	//static void FromLua_AddEventListener( Script* script, EventType eventType, object handlerFunction );

	/*! Recieves a removeEventListener request from LUA and forwards it to the Script Instance */
	//static void FromLua_RemoveEventListener( Script* script, EventType eventType, object handlerFunction );

	/*! Loads a Script into the current state */
	void Include( std::string scriptPath );

private:

	Script( );

	Script( const Script & copy ) { };
	Script & operator = ( const Script & copy ) { return *this; };

	/*! Receives an Event from the Event Manager and forwards to the LUA Script */
	//void ToLua_EventHandler( const IEvent* event );

	/*! Is called when an error occurs within the Script */
	static int FromLua_ScriptError( lua_State* luaState );

	/*! Adds an EventListener to the System and maps it to an Lua Function */
	//void AddEventListener( const EventType eventType, const object handlerFunction );

	/*! Removes an EventListener mapping from the Script */
	//void RemoveEventListener( const EventType eventType, const object handlerFunction );

	/*! Send a message to the Logger */
	//static void LogMessage( const std::string message );

	lua_State* _parentState;
	lua_State* _luaState;
	EventHandlers _eventHandlers;
	FileBuffer* _fileBuffer;
	bool _isInitialized;

};

#endif