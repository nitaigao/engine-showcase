#ifndef __WORLD_H
#define __WORLD_H

#include <vector>

#include "IWorld.hpp"

class World : public IWorld
{

public:

	~World( );

	World( )
	{

	}

	IEntity* CreateEntity( const std::string& name );

	void RegisterSystem( ISystem* system );

	const SystemSceneList& GetSystemScenes( ) { return _systemScenes; }

private:

	std::string _name;
	EntityList _entities;
	SystemSceneList _systemScenes;

};

#endif