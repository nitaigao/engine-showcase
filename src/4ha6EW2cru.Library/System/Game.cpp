#include "Game.h"

#include "../Events/Event.h"
#include "../Logging/Logger.h"

#include "../Scripting/ScriptSystem.h"
#include "../Input/InputSystem.h"
#include "../Graphics/OgreRenderSystem.h"

#include "../Exceptions/AlreadyInitializedException.hpp"
#include "../Exceptions/UnInitializedException.hpp"

#include "../Logging/ConsoleAppender.h"

#include "Management.h"

void Game::Initialize( )
{
	if ( _isInitialized )
	{
		AlreadyInitializedException e ( "Game::Initialize - Attempted to Initialized when the game had already been Initialized" );
		Logger::GetInstance( )->Fatal( e.what( ) );
		throw e;
	}

	// -- Startup Sequence -- //

	// -- Intitialize All Singletons

	Logger::Initialize( );
	ConsoleAppender* consoleAppender = new ConsoleAppender( );
	Logger::GetInstance( )->AddAppender( consoleAppender );
	Logger::GetInstance( )->Info( "Initializing Game" );

	Management::Initialize( );

	// -- Intialize All Systems

	Management::GetInstance( )->GetSystemManager( )->AddSystem( new ScriptSystem( _configuration ) );
	Management::GetInstance( )->GetSystemManager( )->AddSystem( new OgreRenderSystem( _configuration ) );
	Management::GetInstance( )->GetSystemManager( )->AddSystem( new InputSystem( _configuration ) );

	Management::GetInstance( )->GetSystemManager( )->InitializeAllSystems( );

	// -- Register Events

	Management::GetInstance( )->GetEventManager( )->AddEventListener( GAME_QUIT, this, &Game::OnGameQuit );
	Management::GetInstance( )->GetEventManager( )->QueueEvent( new Event( GAME_INITIALIZED ) );

	_isInitialized = true;
}

void Game::Update( float deltaMilliseconds )
{
	if ( !_isInitialized )
	{
		UnInitializedException e( "Game::StartLoop - Cannot Start the Loop when not Initialized" );
		Logger::GetInstance( )->Fatal( e.what( ) );
		throw e;
	}

	Management::GetInstance( )->Update( deltaMilliseconds );
}

void Game::Release( )
{
	if ( !_isInitialized )
	{
		UnInitializedException e( "Game::Release - Cannot Release when not Initialized" );
		Logger::GetInstance( )->Fatal( e.what( ) );
		throw e;
	}

	Management::GetInstance( )->Release( );
	
	Logger::GetInstance( )->Release( );
}

void Game::OnGameQuit( const IEvent* event )
{
	_isQuitting = true;
	Management::GetInstance( )->GetEventManager( )->RemoveEventListener( GAME_QUIT, this, &Game::OnGameQuit );
}

// EOF