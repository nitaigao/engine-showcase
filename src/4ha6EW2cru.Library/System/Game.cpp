#include "Game.h"

#include "../Exceptions/AlreadyInitializedException.hpp"
#include "../Exceptions/UnInitializedException.hpp"

#include "Configuration.h""

#include "../Common/Paths.hpp"
#include "../Logging/ConsoleAppender.h"
#include "../Logging/FileAppender.h"
#include "../Logging/EventAppender.h"

#include "../Scripting/ScriptManager.h"

void Game::Initialize( )
{
	if ( _isInitialized )
	{
		AlreadyInitializedException e ( "Game::Initialize - Attempted to Initialized when the game had already been Initialized" );
		Logger::GetInstance( )->Fatal( e.what( ) );
		throw e;
	}

	// -- Startup Sequence -- //
		
	// Bring up the basic Logging System

	Logger::Initialize( );

	{
		ConsoleAppender* consoleAppender = new ConsoleAppender( );
		Logger::GetInstance( )->AddAppender( consoleAppender );

		FileAppender* fileAppender = new FileAppender( Paths::GetLogFilePath( ) );
		fileAppender->Initialize(  );
		Logger::GetInstance( )->AddAppender( fileAppender );

		Logger::GetInstance( )->Info( "Initializing Game" );
	}

	// Bring up the File System

	FileManager::Initialize( );

	// Bring up the Events System

	EventManager::Initialize( );

	// Load the game configuration

	Configuration* config = Configuration::Load( "config/game.cfg" );

	// Setup the Logging System

	LogLevel logLevel = static_cast< LogLevel >( config->FindConfigItemInt( "log_level" ) );
	Logger::GetInstance( )->SetLogLevel( logLevel );

	IAppender* eventAppender = new EventAppender( EventManager::GetInstance( ) );
	Logger::GetInstance( )->AddAppender( eventAppender );

	// Bring up the Script System

	ScriptManager::Initialize( );

	// Initialize all Views

	int isFullScreen = config->FindConfigItemInt( "display_isfullscreen" );
	_view = new HumanView( );

	if ( !isFullScreen )
	{
		_view->Initialize( 
			config->FindConfigItemInt( "display_windowed_width" ), 
			config->FindConfigItemInt( "display_windowed_height" ), 
			config->FindConfigItemInt( "display_windowed_depth" ), 
			isFullScreen 
			);			
	}
	else
	{
		_view->Initialize( 
			config->FindConfigItemInt( "display_fullscreen_width" ), 
			config->FindConfigItemInt( "display_fullscreen_height" ), 
			config->FindConfigItemInt( "display_fullscreen_depth" ), 
			isFullScreen 
			);
	}

	// Create core dependencies

	EventManager::GetInstance( )->AddEventListener( GAME_QUIT, this, &Game::OnGameQuit );
	EventManager::GetInstance( )->QueueEvent( new Event( GAME_INITIALIZED ) );

	// Cleanup and return

	delete config;
	_isInitialized = true;
}

void Game::StartLoop( bool loopOnce )
{
	if ( !_isInitialized )
	{
		UnInitializedException e( "Game::StartLoop - Cannot Start the Loop when not Initialized" );
		Logger::GetInstance( )->Fatal( e.what( ) );
		throw e;
	}

	MSG msg;

	while( !_isQuitting )
	{
		if ( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}

		_view->Update( );
		_view->Render( );

		EventManager::GetInstance( )->Update( );

		if ( loopOnce )
		{
			_isQuitting = true;
		}
	}
}

void Game::Release( )
{
	if ( !_isInitialized )
	{
		UnInitializedException e( "Game::Release - Cannot Release when not Initialized" );
		Logger::GetInstance( )->Fatal( e.what( ) );
		throw e;
	}

	delete _view;

	{	// Release all Singletons

		ScriptManager::GetInstance( )->Release( );
		FileManager::GetInstance( )->Release( );
		EventManager::GetInstance( )->Release( );
		Logger::GetInstance( )->Release( );
	}
}

void Game::OnGameQuit( const IEvent* event )
{
	_isQuitting = true;
	EventManager::GetInstance( )->RemoveEventListener( GAME_QUIT, this, &Game::OnGameQuit );
}

// EOF