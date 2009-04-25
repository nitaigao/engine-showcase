#ifndef __SCRIPTSYSTEM_H
#define __SCRIPTSYSTEM_H

#include "../System/ISystem.hpp"
#include "../System/Configuration.h"

class ScriptSystem : public ISystem
{

public:

	virtual ~ScriptSystem( ) { };

	ScriptSystem( Configuration* configuration )
		: _configuration( configuration )
	{
		
	}

	void Initialize( ) { };
	inline void Update( float deltaMilliseconds ) { };
	void Release( ) { };

	inline SystemType GetType( ) { return ScriptSystemType; };

	ISystemScene* CreateScene( );

	inline AnyValueMap GetProperties( ) { return _properties; };
	inline void SetProperty( const std::string& name, AnyValue value ) { };

private:

	AnyValueMap _properties;
	Configuration* _configuration;

	ScriptSystem( const ScriptSystem & copy ) { };
	ScriptSystem & operator = ( const ScriptSystem & copy ) { return *this; };

};

#endif