#ifndef __AISYSTEM_H
#define __AISYSTEM_H

#include "../System/ISystem.hpp"

class AISystem : public ISystem
{

public:

	virtual ~AISystem( ) { };

	inline void Initialize( ) { };
	inline void Update( float deltaMilliseconds ) { };
	inline void Release( ) { };

	inline SystemType GetType( ) { return AISystemType; };

	ISystemScene* CreateScene( );

	inline AnyValueMap GetProperties( ) { return _properties; };
	inline void SetProperty( const std::string& name, AnyValue value ) { };

private:

	AnyValueMap _properties;
};

#endif