#ifndef __ISYSTEMWORLD_H
#define __ISYSTEMWORLD_H

#include <string>
#include <map>
#include <vector>

#include "ISystemComponent.hpp"

class ISystemScene
{

public:

	virtual ~ISystemScene( ) { };

	virtual void Initialize( ) = 0;
	virtual void Update( float deltaMilliseconds ) = 0;

	virtual SystemType GetType( ) = 0;

	virtual ISystemComponent* CreateComponent( const std::string& name, const std::string& type ) = 0;
	virtual void DestroyComponent( ISystemComponent* component ) = 0;
};

typedef std::map< SystemType, ISystemScene* > SystemSceneMap;
typedef std::vector< ISystemScene* > SystemSceneList;


#endif