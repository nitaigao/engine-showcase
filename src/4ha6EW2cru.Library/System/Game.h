#ifndef __Game_H
#define __Game_H

#include "../Logging/Logger.h"
#include "../Events/EventManager.h"
#include "../IO/FileManager.h"

#include "../Logging/AppenderFactory.h"

#include "../Events/EventListener.h"
#include  "../Events/Event.h"

#include "../View/HumanView.h"

/*!
	Represents the Operating System Entry Point of the Game
*/
class Game
{

public:

	Game( )
		: _loopCount( 0 )
		, _isQuitting( false )
		, _view( 0 )
	{ };
	
	/*! Initializes the Game and all Subsystems */
	bool Initialize( );

	/*! Entry point to start the main Game Loop */
	void StartLoop( bool loopOnce );

	/*! Releases all of the Game Subsystems */
	void Release( );

private:

	/*! Recieves the GAME_QUIT Event and starts the Quit Process */
	void OnGameQuit( const IEvent* event );

	int _loopCount;
	bool _isQuitting;

	HumanView* _view;

};

#endif

// EOF