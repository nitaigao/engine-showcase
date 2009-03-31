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
	virtual void AddSystemScene( ISystemScene* systemScene ) = 0;
	virtual const SystemSceneMap& GetSystemScenes( ) = 0;
	virtual void Update( float deltaMilliseconds ) = 0;
	virtual void Clear( ) = 0;
};

typedef std::map< std::string, IWorld* > WorldList;

#endif