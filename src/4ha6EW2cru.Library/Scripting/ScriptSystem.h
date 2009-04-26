#ifndef __SCRIPTSYSTEM_H
#define __SCRIPTSYSTEM_H

#include "../System/ISystem.hpp"
#include "../Configuration/IConfiguration.hpp"

class ScriptSystem : public ISystem
{

public:

	virtual ~ScriptSystem( ) { };

	ScriptSystem( Configuration::IConfiguration* configuration )
		: _configuration( configuration )
	{
		
	}

	void Initialize( ) { };
	inline void Update( float deltaMilliseconds ) { };
	void Release( ) { };

	inline System::Types::Type GetType( ) { return System::Types::SCRIPT; };

	ISystemScene* CreateScene( );

	inline AnyValue::AnyValueMap GetProperties( ) { return _properties; };
	inline void SetProperty( const std::string& name, AnyValue value ) { };

private:

	AnyValue::AnyValueMap _properties;
	Configuration::IConfiguration* _configuration;

	ScriptSystem( const ScriptSystem & copy ) { };
	ScriptSystem & operator = ( const ScriptSystem & copy ) { return *this; };

};

#endif