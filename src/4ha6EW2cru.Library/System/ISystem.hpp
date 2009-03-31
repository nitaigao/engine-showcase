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

public:

	typedef std::map< std::string, SystemProperty > PropertyMap;

	virtual ~ISystem( ) { };

	virtual void Initialize( ) = 0;
	virtual void Update( float deltaMilliseconds ) = 0;
	virtual void Release( ) = 0;

	virtual ISystemScene* CreateScene( ) = 0;

	virtual SystemType GetSystemType( ) = 0;
	virtual PropertyMap GetProperties( ) = 0;
	virtual void SetProperties( PropertyMap systemProperties ) = 0;

};

typedef std::deque< ISystem* > SystemList;

#endif