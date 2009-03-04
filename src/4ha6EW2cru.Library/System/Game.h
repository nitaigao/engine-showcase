#ifndef __Game_H
#define __Game_H

#include <vector>

#include "ISystem.hpp"
#include "Configuration.h"

#include "../Events/IEvent.hpp"
#include "../State/IWorld.hpp"

/*!
	Represents the Operating System Entry Point of the Game
*/
class Game
{

public:

	Game( )
		: _isQuitting( false )
		, _isInitialized( false )
		, _world( 0 )
	{

	}

	~Game( );
	
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

	bool _isQuitting;
	bool _isInitialized;

	Configuration* _configuration;
	IWorld* _world;

};

#endif

// EOF