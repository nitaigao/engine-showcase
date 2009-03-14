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

	virtual ~ScriptComponent( ) { };

	ScriptComponent( const std::string& name, lua_State* state )
		: _name( name )
		, _state( state )
	{

	};

	void Initialize( SystemPropertyList properties );

	void AddObserver( IObserver* observer ) { };
	void PushChanges( unsigned int systemChanges ) { };
	
	inline unsigned int GetRequestedChanges( ) { return 0; };
	void Observe( ISubject* subject, unsigned int systemChanges ) { };

	inline const std::string& GetName( ) { return _name; };
	inline SystemType GetType( ) { return ScriptSystemType; };

	inline lua_State* GetState( ) { return _state; };

private:

	lua_State* _state;
	std::string _name;

	ScriptComponent( const ScriptComponent & copy ) { };
	ScriptComponent & operator = ( const ScriptComponent & copy ) { return *this; };

};

#endif