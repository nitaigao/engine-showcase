#include "Game.h"

#include "../Common/Paths.hpp"
#include "../Logging/ConsoleAppender.h"
#include "../Logging/FileAppender.h"

bool Game::Initialize( )
{
	{	// Initialize all Singletons
		
		if ( !Logger::Initialize( ) )
		{
			return false;
		}

		{
			ConsoleAppender* consoleAppender = static_cast< ConsoleAppender* >( AppenderFactory::CreateAppender( CONSOLEAPPENDER ) );
			Logger::GetInstance( )->AddAppender( consoleAppender );
	
			FileAppender* fileAppender = static_cast< FileAppender* >( AppenderFactory::CreateAppender( FILEAPPENDER ) );
			fileAppender->Initialize( Paths::GetLogFilePath( ) );
			Logger::GetInstance( )->AddAppender( fileAppender );

			Logger::GetInstance( )->Info( "Initializing Game" );
		}

		if ( !EventManager::Initialize( ) )
		{
			return false;
		}

		if ( !FileManager::Initialize( ) )
		{
			return false;
		}

		FileManager::GetInstance( )->AddFileStore( Paths::GetViewPackagePath( ) );
	}

	{	// Initialize all Views

		_view = new HumanView( );

		if ( !_view->Initialize( ) )
		{
			return false;
		}
	}

	EventManager::GetInstance( )->AddEventListener( GAME_QUIT, this, &Game::OnGameQuit );
	EventManager::GetInstance( )->QueueEvent( new Event( GAME_INITIALIZED ) );

	return true;
}

void Game::StartLoop( bool loopOnce )
{
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
	Logger::GetInstance( )->Info( "Releasing the Game" );

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