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

	/*! Loads a Script into the current state */
	void Include( std::string scriptPath );

private:

	Script( );

	Script( const Script & copy ) { };
	Script & operator = ( const Script & copy ) { return *this; }

	lua_State* _parentState;
	lua_State* _luaState;
	EventHandlers _eventHandlers;
	FileBuffer* _fileBuffer;
	bool _isInitialized;

};

#endif