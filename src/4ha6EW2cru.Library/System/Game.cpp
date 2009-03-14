#include "Game.h"

#include "../Events/Event.h"
#include "../Events/EventData.hpp"
#include "../Logging/Logger.h"
#include "../Logging/ConsoleAppender.h"

#include "Management.h"
#include "Configuration.h"
#include "../State/World.h"
#include "../State/WorldLoader.h"

#include "../Scripting/ScriptSystem.h"
#include "../Input/InputSystem.h"
#include "../Graphics/OgreRenderSystem.h"
#include "../Geometry/GeometrySystem.h"
#include "../Physics/HavokPhysicsSystem.h"

#include "../Exceptions/AlreadyInitializedException.hpp"
#include "../Exceptions/UnInitializedException.hpp"

void Game::Initialize( )
{
	if ( _isInitialized )
	{
		AlreadyInitializedException e ( "Game::Initialize - Attempted to Initialized when the game had already been Initialized" );
		Logger::GetInstance( )->Fatal( e.what( ) );
		throw e;
	}

	// -- Intitialize All Singletons

	Logger::Initialize( );
	ConsoleAppender* consoleAppender = new ConsoleAppender( );
	Logger::GetInstance( )->AddAppender( consoleAppender );
	Logger::GetInstance( )->Info( "Initializing Game" );

	_configuration = Configuration::Load( "config/game.cfg" );

	Management::Initialize( );

	// -- Intialize All Systems

	ISystemManager* systemManager = Management::GetInstance( )->GetSystemManager( );
	systemManager->AddSystem( new GeometrySystem( ) );
	systemManager->AddSystem( new HavokPhysicsSystem( ) );
	systemManager->AddSystem( new ScriptSystem( _configuration ) );
	systemManager->AddSystem( new OgreRenderSystem( _configuration ) );
	systemManager->AddSystem( new InputSystem( _configuration ) );
	systemManager->InitializeAllSystems( );

	// -- Setup the World

	_world = new World( );

	SystemList systemList = systemManager->GetAllSystems( );
	
	for( SystemList::iterator i = systemList.begin( ); i != systemList.end( ); ++i )
	{
		_world->AddSystemScene( ( *i )->CreateScene( ) );
	}

	// -- Register Events

	Management::GetInstance( )->GetEventManager( )->AddEventListener( GAME_QUIT, this, &Game::OnGameQuit );
	Management::GetInstance( )->GetEventManager( )->AddEventListener( GAME_LEVEL_CHANGED, this, &Game::OnGameLevelChanged ); 
	Management::GetInstance( )->GetEventManager( )->QueueEvent( new Event( GAME_INITIALIZED ) );

	_isInitialized = true;
}

Game::~Game( )
{
	delete _configuration;
}

void Game::Update( float deltaMilliseconds )
{
	if ( !_isInitialized )
	{
		UnInitializedException e( "Game::StartLoop - Cannot Start the Loop when not Initialized" );
		Logger::GetInstance( )->Fatal( e.what( ) );
		throw e;
	}

	_world->Update( deltaMilliseconds );
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

	Management::GetInstance( )->GetEventManager( )->RemoveEventListener( GAME_QUIT, this, &Game::OnGameQuit );
	Management::GetInstance( )->GetEventManager( )->RemoveEventListener( GAME_LEVEL_CHANGED, this, &Game::OnGameLevelChanged ); 

	delete _world;

	Management::GetInstance( )->Release( );
	
	Logger::GetInstance( )->Release( );
}

void Game::OnGameQuit( const IEvent* event )
{
	_isQuitting = true;
}

void Game::OnGameLevelChanged( const IEvent* event )
{
	LevelChangedEventData* eventData = static_cast< LevelChangedEventData* >( event->GetEventData( ) );

	FileManager fileManager;
	fileManager.MountFileStore( "../game/data/levels.bad", "data/" );
	WorldLoader loader( _world, &fileManager );

	std::stringstream levelPath;
	levelPath << "/data/levels/" << eventData->GetLevelName( ) << ".yaml";

	loader.Load( levelPath.str( ) );
}

// EOF