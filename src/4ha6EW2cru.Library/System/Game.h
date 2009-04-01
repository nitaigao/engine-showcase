#ifndef __Game_H
#define __Game_H

#include <vector>

#include "ISystem.hpp"
#include "Configuration.h"

#include "../Events/IEvent.hpp"
#include "../State/IWorld.hpp"
#include "../State/IWorldLoader.hpp"

/*!
	Represents the Operating System Entry Point of the Game
*/
class Game
{

public:

	virtual ~Game( );

	Game( )
		: _isQuitting( false )
		, _isInitialized( false )
		, _world( 0 )
		, _worldLoader( 0 )
	{

	}
	
	/*! Initializes the Game and all Subsystems */
	void Initialize( );

	void Update( float deltaMilliseconds );

	/*! Releases all of the Game Subsystems */
	void Release( );

	/*! Returns whether or not the Game is ready to Quit */
	inline bool IsQuitting( ) { return _isQuitting; };

private:

	/*! Receives the GAME_QUIT Event and starts the Quit Process */
	void OnGameQuit( const IEvent* event );

	/*! Receives the GAME_LEVEL_CHANGED and starts the WorldLoader */
	void OnGameLevelChanged( const IEvent* event );

	/*! Receives the GAME_ENDED Event and stops the current Game */
	void OnGameEnded( const IEvent* event );

	bool _isQuitting;
	bool _isInitialized;

	Configuration* _configuration;
	IWorldLoader* _worldLoader;
	IWorld* _world;

	Game( const Game & copy ) { };
	Game & operator = ( const Game & copy ) { return *this; };

};

#endif

// EOF