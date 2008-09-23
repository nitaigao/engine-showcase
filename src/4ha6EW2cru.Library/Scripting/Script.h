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
using namespace luabind;

/*!
	An interface for loading LUA scripts
*/
class Script
{

	typedef std::map< EventType, object > EventHandlers;
	
public:

	~Script( );

	/*! Initializes the Script from a FileBuffer */
	void Initialize( );

	/*! Creates the Script from a FileBuffer */
	static Script* CreateFromFileBuffer( FileBuffer* fileBuffer );

	/*! Allows the specified function to be called from C++ */
	void CallFunction( const std::string functionName );

	/*! Returns the Script state */
	lua_State* GetState( ) const;
	
private:

	Script( )
		: _luaState( 0 )
		, _fileBuffer( 0 )
		, _isInitialized( false )
	{ 

	};

	Script( const Script & copy ) { };
	Script & operator = ( const Script & copy ) { return *this; };

	/*! Receives addEventListener request from LUA and forwards to the Script Instance */
	static void FromLua_AddEventListener( Script* script, EventType eventType, object handlerFunction );

	/*! Receives an Event from the Event Manager and forwards to the LUA Script */
	void ToLua_EventHandler( const IEvent* event );

	/*! Adds an EventListener to the System and maps it to an Lua Function */
	void AddEventListener( const EventType eventType, const object handlerFunction );

	/*! Send a message to the Logger */
	static void LogMessage( const std::string message );

	lua_State* _luaState;
	EventHandlers _eventHandlers;
	FileBuffer* _fileBuffer;
	bool _isInitialized;

};

#endif