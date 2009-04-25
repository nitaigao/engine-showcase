#ifndef __ISYSTEM_H
#define __ISYSTEM_H

#include <string>
#include <deque>
#include <map>

#include "SystemType.hpp"
#include "SystemProperty.hpp"
#include "ISystemScene.hpp"

class ISystem
{

protected:

public:

	virtual ~ISystem( ) { };

	virtual void Initialize( ) = 0;
	virtual void Update( float deltaMilliseconds ) = 0;
	virtual void Release( ) = 0;

	virtual SystemType GetType( ) = 0;

	virtual ISystemScene* CreateScene( ) = 0;

	virtual AnyValue::AnyValueMap GetProperties( ) = 0;
	virtual void SetProperty( const std::string& name, AnyValue value ) = 0;

};

typedef std::deque< ISystem* > SystemList;

#endif