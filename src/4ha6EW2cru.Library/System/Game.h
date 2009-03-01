#ifndef __Game_H
#define __Game_H

#include <vector>

#include "ISystem.hpp"
#include "Configuration.h"

#include "../Events/IEvent.hpp"

/*!
	Represents the Operating System Entry Point of the Game
*/
class Game
{

	typedef std::vector< ISystem* > SystemList;

public:

	Game( )
		: _isQuitting( false )
		, _isInitialized( false )
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

	bool _isQuitting;
	bool _isInitialized;

	SystemList _systems;
	Configuration* _configuration;

};

#endif

// EOF