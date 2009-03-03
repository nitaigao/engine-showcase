#ifndef __SCRIPTCOMPONENT_H
#define __SCRIPTCOMPONENT_H

#include "../System/ISystemComponent.hpp"

extern "C" 
{
#	include <lua.h>
}

class ScriptComponent : public ISystemComponent
{

public:

	ScriptComponent( lua_State* state )
		: _state( state )
	{

	};

	virtual ~ScriptComponent( ) { };

	inline SystemType GetType( ) { return ScriptSystemType; };
	void Initialize( SystemPropertyList properties );

	inline lua_State* GetState( ) { return _state; };

private:

	lua_State* _state;

};

#endif