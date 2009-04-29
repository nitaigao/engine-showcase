#include "Game.h"

#include "Exceptions/AlreadyInitializedException.hpp"
#include "Exceptions/UnInitializedException.hpp"

#include "Logging/Logger.h"
using namespace Logging;

#include "Management/Management.h"

#include "Configuration/Configuration.h"
using namespace Configuration;

#include "State/World.h"
using namespace State;

#include "State/Serilaization/WorldSerializer.h"
using namespace Serialization;

#include "Scripting/ScriptSystem.h"
#include "Input/InputSystem.h"
#include "Renderer/RendererSystem.h"
#include "Geometry/GeometrySystem.h"
#include "Physics/HavokPhysicsSystem.h"
#include "AI/AISystem.h"
#include "UX/UXSystem.h"
#include "Sound/SoundSystem.h"

#include "Events/Event.h"
#include "Events/EventData.hpp"
using namespace Events;

#include "Renderer/Color.hpp"
#include "Scripting/ScriptEvent.hpp"
using namespace Script;

void Game::Initialize( )
{
	if ( _isInitialized )
	{
		AlreadyInitializedException e ( "Game::Initialize - Attempted to Initialized when the game had already been Initialized" );
		Logger::Fatal( e.what( ) );
		throw e;
	}

	// -- Initialize All Singletons

	Management::Initialize( );

	// -- Set Configuration Defaults 

	_configuration = ClientConfiguration::Load( "config/game.cfg" );
	_configuration->SetDefault( "Developer", "console", false );
	_configuration->SetDefault( "Logging", "level", static_cast< int >( LEVEL_FATAL ) );

	LogLevel logLevel = static_cast< LogLevel >( _configuration->Find( "Logging", "level" ).GetValue< int >( ) );
	Logger::SetLogLevel( logLevel );

	// -- Initialize All Systems

	ISystemManager* systemManager = Management::GetInstance( )->GetSystemManager( );
	systemManager->RegisterSystem( new Geometry::GeometrySystem( ) );
	systemManager->RegisterSystem( new Renderer::RendererSystem( _configuration ) );
	systemManager->RegisterSystem( new Physics::HavokPhysicsSystem( ) );
	systemManager->RegisterSystem( new Input::InputSystem( _configuration ) );
	systemManager->RegisterSystem( new Script::ScriptSystem( _configuration ) );
	systemManager->RegisterSystem( new AI::AISystem( ) );
	systemManager->RegisterSystem( new UX::UXSystem( ) );
	systemManager->RegisterSystem( new Sound::SoundSystem( ) );
	systemManager->InitializeAllSystems( );

	// -- Setup the World and World Loader

	_world = systemManager->CreateWorld( );
	_worldLoader = new Serialization::WorldSerializer( _world );

	// -- Register Events

	Management::GetInstance( )->GetEventManager( )->AddEventListener( GAME_QUIT, this, &Game::OnGameQuit );
	Management::GetInstance( )->GetEventManager( )->AddEventListener( GAME_LEVEL_CHANGED, this, &Game::OnGameLevelChanged ); 
	Management::GetInstance( )->GetEventManager( )->AddEventListener( GAME_ENDED, this, &Game::OnGameEnded );
	Management::GetInstance( )->GetEventManager( )->QueueEvent( new ScriptEvent( "GAME_INITIALIZED" ) );

	_isInitialized = true;
}

Game::~Game( )
{
	delete _configuration;
}

void Game::Update( const float& deltaMilliseconds )
{
	if ( !_isInitialized )
	{
		UnInitializedException e( "Game::StartLoop - Cannot Start the Loop when not Initialized" );
		Logger::Fatal( e.what( ) );
		throw e;
	}

	_worldLoader->Update( deltaMilliseconds );
	_world->Update( deltaMilliseconds );
	Management::GetInstance( )->Update( deltaMilliseconds );
}

void Game::Release( )
{
	if ( !_isInitialized )
	{
		UnInitializedException e( "Game::Release - Cannot Release when not Initialized" );
		Logger::Fatal( e.what( ) );
		throw e;
	}

	Management::GetInstance( )->GetEventManager( )->RemoveEventListener( GAME_QUIT, this, &Game::OnGameQuit );
	Management::GetInstance( )->GetEventManager( )->RemoveEventListener( GAME_LEVEL_CHANGED, this, &Game::OnGameLevelChanged ); 

	_world->Clear( );

	delete _worldLoader;
	delete _world;

	Management::GetInstance( )->Release( );
}

void Game::OnGameQuit( const IEvent* event )
{
	_isQuitting = true;
}

void Game::OnGameLevelChanged( const IEvent* event )
{
	this->OnGameEnded( event );

	LevelChangedEventData* eventData = static_cast< LevelChangedEventData* >( event->GetEventData( ) );

	std::stringstream levelPath;
	levelPath << "/data/levels/" << eventData->GetLevelName( ) << ".yaml";
	
	_worldLoader->Load( levelPath.str( ) );
}

void Game::OnGameEnded( const IEvent* event )
{
	_world->Clear( );

	ISystem* renderSystem = Management::GetInstance( )->GetSystemManager( )->GetSystem( System::Types::RENDER );

	renderSystem->SetProperty( "activeCamera", "default" );
	//renderSystem->SetProperty( "ambientColor", Renderer::Color( 0.0f, 0.0f, 0.0f ) );
	//renderSystem->SetProperty( "backgroundColor", Renderer::Color( 0.0f, 0.0f, 0.0f ) );
	renderSystem->SetProperty( "skyBox", "" );
}

// EOF