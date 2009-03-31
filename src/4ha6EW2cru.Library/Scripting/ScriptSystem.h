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

	ISystemScene* CreateScene( );

	inline SystemType GetSystemType( ) { return ScriptSystemType; };

	inline PropertyMap GetProperties( ) { return _properties; };
	inline void SetProperties( PropertyMap properties ) { };

private:

	PropertyMap _properties;
	Configuration* _configuration;

	ScriptSystem( const ScriptSystem & copy ) { };
	ScriptSystem & operator = ( const ScriptSystem & copy ) { return *this; };

};

#endif