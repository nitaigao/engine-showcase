#ifndef __SCRIPTSYSTEM_H
#define __SCRIPTSYSTEM_H

#include "../System/ISystem.hpp"
#include "../System/Configuration.h"

class ScriptSystem : public ISystem
{

public:

	ScriptSystem( Configuration* configuration )
		: _configuration( configuration )
	{
		
	}

	virtual ~ScriptSystem( ) { };

	void Initialize( );
	void Update( float deltaMilliseconds ) { };
	void Release( ) { };

	ISystemScene* CreateScene( );

	inline SystemType GetSystemType( ) { return ScriptSystemType; };
	inline PropertyMap GetProperties( ) { return _properties; };

private:

	ScriptSystem( const ScriptSystem & copy ) { };
	ScriptSystem & operator = ( const ScriptSystem & copy ) { return *this; };

	PropertyMap _properties;
	Configuration* _configuration;

};

#endif