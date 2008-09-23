#include "Game.h"

#include "../Exceptions/AlreadyInitializedException.hpp"
#include "../Exceptions/UnInitializedException.hpp"

#include "../Common/Paths.hpp"
#include "../Logging/ConsoleAppender.h"
#include "../Logging/FileAppender.h"

void Game::Initialize( )
{
	if ( _isInitialized )
	{
		AlreadyInitializedException e ( "Game::Initialize - Attempted to Initialized when the game had already been Initialized" );
		Logger::GetInstance( )->Fatal( e.what( ) );
		throw e;
	}

	{	// Initialize all Singletons
		
		Logger::Initialize( );

		{
			ConsoleAppender* consoleAppender = static_cast< ConsoleAppender* >( AppenderFactory::CreateAppender( CONSOLEAPPENDER ) );
			Logger::GetInstance( )->AddAppender( consoleAppender );

			FileAppender* fileAppender = static_cast< FileAppender* >( AppenderFactory::CreateAppender( FILEAPPENDER ) );
			fileAppender->Initialize( Paths::GetLogFilePath( ) );
			Logger::GetInstance( )->AddAppender( fileAppender );

			Logger::GetInstance( )->Info( "Initializing Game" );
		}

		EventManager::Initialize( );

		{

			IAppender* eventAppender = AppenderFactory::CreateAppender( EVENTAPPENDER );
			Logger::GetInstance( )->AddAppender( eventAppender );

		}

		FileManager::Initialize( );

		FileManager::GetInstance( )->AddFileStore( Paths::GetViewPackagePath( ) );
	}

	{	// Initialize all Views

		_view = new HumanView( );
		int desktopWidth = GetSystemMetrics( SM_CXSCREEN );
		int desktopHeight = GetSystemMetrics( SM_CYSCREEN );
		_view->Initialize( desktopWidth, desktopHeight, false );
	}

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