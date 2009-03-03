#ifndef __IWORLD_H
#define __IWORLD_H

#include <string>
#include <map>

#include "IEntity.hpp"
#include "../System/ISystem.hpp"

class IWorld
{

public:

	virtual ~IWorld( ) { };

	virtual IEntity* CreateEntity( const std::string& name ) = 0;
	virtual void RegisterSystem( ISystem* system ) = 0;
	virtual const SystemSceneList& GetSystemScenes( ) = 0;

};

typedef std::map< std::string, IWorld* > WorldList;

#endif