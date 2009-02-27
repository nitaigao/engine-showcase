#ifndef __SCRIPTOBJECT_H
#define __SCRIPTOBJECT_H

#include "../System/ISystemObject.hpp"

extern "C" 
{
#	include <lua.h>
#	include <lualib.h>
}

class ScriptObject : public ISystemObject
{

public:

	ScriptObject( lua_State* state )
		: _state( state )
	{

	};

	virtual ~ScriptObject( ) { };

	SystemType GetSystemType( ) { return ScriptSystemType; };

	lua_State* GetState( ) { return _state; };

private:

	lua_State* _state;

};

#endif