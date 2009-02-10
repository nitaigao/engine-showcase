#include "Game.h"

#include <mmsystem.h> 

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

	_configuration = new Configuration( );

	// Setup the Logging System

	LogLevel logLevel = static_cast< LogLevel >( _configuration->GetLoggingLevel( ) );
	Logger::GetInstance( )->SetLogLevel( logLevel );

	IAppender* eventAppender = new EventAppender( EventManager::GetInstance( ) );
	Logger::GetInstance( )->AddAppender( eventAppender );

	// Bring up the Script System

	ScriptManager::Initialize( _configuration );

	// Initialize all Views

	_view = new HumanView( _configuration );
	_view->Initialize( );

	// Create core dependencies

	EventManager::GetInstance( )->AddEventListener( GAME_QUIT, this, &Game::OnGameQuit );
	EventManager::GetInstance( )->QueueEvent( new Event( GAME_INITIALIZED ) );

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

	float deltaMilliseconds = 0.0f;
	DWORD endFrameTime = 0;
	DWORD startFrameTime = 0;

	while( !_isQuitting )
	{
		deltaMilliseconds = ( endFrameTime - startFrameTime ) / 1000.0f;
		startFrameTime = timeGetTime( );

		if ( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}

		_view->Update( deltaMilliseconds );
		_view->Render( );

		EventManager::GetInstance( )->Update( );

		if ( loopOnce )
		{
			_isQuitting = true;
		}

		endFrameTime = timeGetTime( );
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
	delete _configuration;

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