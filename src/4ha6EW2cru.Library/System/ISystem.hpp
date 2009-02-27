#ifndef __ISYSTEM_H
#define __ISYSTEM_H

#include <string>
#include <map>

#include "SystemType.hpp"
#include "Property.hpp"
#include "ISystemObject.hpp"

class ISystem
{

protected:

	typedef std::map< std::string, Property > PropertyMap;

public:

	virtual ~ISystem( ) { };

	virtual void Initialize( ) = 0;
	virtual void Update( float deltaMilliseconds ) = 0;
	virtual void Release( ) = 0;

	virtual SystemType GetSystemType( ) = 0;
	virtual ISystemObject* CreateObject( const std::string& name, SystemType systemType ) = 0;

	virtual PropertyMap GetProperties( ) = 0;

};

#endif