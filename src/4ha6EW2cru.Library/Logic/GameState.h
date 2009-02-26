#ifndef __GAMESTATE_H
#define __GAMESTATE_H

#include "IGameState.hpp"

#include "../Events/IEvent.hpp"

#include "ILevelLoader.hpp"

class GameState : public IGameState
{

public:

	GameState( ILevelLoader* levelLoader, IWorld* world )
		: _levelLoader( levelLoader )
		, _world( world )
	{

	}

	~GameState( );

	void OnLevelChanged( const IEvent* event );

private:

	ILevelLoader* _levelLoader;
	IWorld* _world;
};

#endif