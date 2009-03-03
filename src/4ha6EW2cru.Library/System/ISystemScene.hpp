#ifndef __ISYSTEMWORLD_H
#define __ISYSTEMWORLD_H

#include <string>

#include "ISystemComponent.hpp"

class ISystemScene
{

public:

	virtual ~ISystemScene( ) { };

	virtual ISystemComponent* CreateComponent( const std::string& name ) = 0;
	virtual void DestroyComponent( ISystemComponent* component ) = 0;
	virtual SystemType GetType( ) = 0;

};

typedef std::map< SystemType, ISystemScene* > SystemSceneList;

#endif