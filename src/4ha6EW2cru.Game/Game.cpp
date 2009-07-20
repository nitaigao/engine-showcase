#include "Game.h"

#include "System/Memory.cxx"

#include "Exceptions/AlreadyInitializedException.hpp"
#include "Exceptions/UnInitializedException.hpp"

#include "Logging/Logger.h"
using namespace Logging;

#include "Management/Management.h"

#include "Configuration/Configuration.h"
#include "Configuration/ConfigurationTypes.hpp"
using namespace Configuration;

#include "State/World.h"
using namespace State;

#include "State/Serilaization/XMLSerializer.h"
using namespace Serialization;

#include "Events/Event.h"
#include "Events/EventData.hpp"
#include "Events/ScriptEvent.hpp"
using namespace Events;

void Game::Initialize( )
{
	Logger::Initialize( );
	Logger::Get( )->SetLogLevel( Logging::LEVEL_WARN );

	Management::Initialize( );

	if ( m_isInitialized )
	{
		AlreadyInitializedException e ( "Game::Initialize - Attempted to Initialized when the game had already been Initialized" );
		Logger::Get( )->Fatal( e.what( ) );
		throw e;
	}

	// -- Set Configuration Defaults 

	m_configuration = ClientConfiguration::Load( "game.cfg" );
	m_configuration->SetDefault( ConfigSections::Developer, ConfigItems::Developer::Console, false );
	m_configuration->SetDefault( ConfigSections::Logging, ConfigItems::Logging::LogLevel, static_cast< int >( LEVEL_FATAL ) );

#ifdef _DEBUG
	m_configuration->SetDefault( ConfigSections::Logging, ConfigItems::Logging::LogLevel, static_cast< int >( LEVEL_ALL ) );
#endif // _DEBUG

	LogLevel logLevel = static_cast< LogLevel >( m_configuration->Find( ConfigSections::Logging, ConfigItems::Logging::LogLevel ).As< int >( ) );
	Logger::Get( )->SetLogLevel( logLevel );

	// -- Initialize All Systems

	ISystemManager* systemManager = Management::Get( )->GetSystemManager( );
	AnyType::AnyTypeMap programOptions = Management::Get( )->GetPlatformManager( )->GetProgramOptions( );

	// -- Server

	ISystem* geometrySystem = systemManager->LoadSystem( "4ha6EW2cru.Geometry.dll" );
	systemManager->RegisterSystem( System::Queues::HOUSE, geometrySystem );

	ISystem* scriptSystem = systemManager->LoadSystem( "4ha6EW2cru.Script.dll" );
	systemManager->RegisterSystem( System::Queues::HOUSE, scriptSystem );

	ISystem* networkSystem = systemManager->LoadSystem( "4ha6EW2cru.Network.dll" );
	systemManager->RegisterSystem( System::Queues::HOUSE, networkSystem );

	if ( programOptions.find( System::Options::DedicatedServer ) != programOptions.end( ) )
	{
		ISystem* physicsSystem = systemManager->LoadSystem( "4ha6EW2cru.Physics.dll" );
		systemManager->RegisterSystem( System::Queues::LOGIC, physicsSystem );

		//ISystem* aiSystem = systemManager->LoadSystem( "4ha6EW2cru.AI.dll" );
		//systemManager->RegisterSystem( System::Queues::HOUSE, aiSystem );

		systemManager->GetSystem( System::Types::NETWORK )->SetAttribute( System::Attributes::Network::IsServer, true );
		Management::Get( )->GetPlatformManager( )->CreateConsoleWindow( );
	}
	else
	{
		ISystem* rendererSystem = systemManager->LoadSystem( "4ha6EW2cru.Renderer.dll" );
		systemManager->RegisterSystem( System::Queues::HOUSE, rendererSystem );

		ISystem* animationSystem = systemManager->LoadSystem( "4ha6EW2cru.Animation.dll" );
		systemManager->RegisterSystem( System::Queues::HOUSE, animationSystem );

		ISystem* inputSystem = systemManager->LoadSystem( "4ha6EW2cru.Input.dll" );
		systemManager->RegisterSystem( System::Queues::HOUSE, inputSystem );

		ISystem* uxSystem = systemManager->LoadSystem( "4ha6EW2cru.UX.dll" );
		systemManager->RegisterSystem( System::Queues::HOUSE, uxSystem );

		ISystem* soundSystem = systemManager->LoadSystem( "4ha6EW2cru.Sound.dll" );
		systemManager->RegisterSystem( System::Queues::HOUSE, soundSystem );
	}

	systemManager->InitializeAllSystems( m_configuration );

	// -- Setup the World and World Loader

	m_world = systemManager->CreateWorld( );
	m_worldLoader = new Serialization::XMLSerializer( m_world );

	// -- Register Events

	Management::Get( )->GetEventManager( )->AddEventListener( GAME_QUIT, this, &Game::OnGameQuit );
	Management::Get( )->GetEventManager( )->AddEventListener( GAME_LEVEL_CHANGED, this, &Game::OnGameLevelChanged ); 
	Management::Get( )->GetEventManager( )->AddEventListener( GAME_ENDED, this, &Game::OnGameEnded );
	Management::Get( )->GetEventManager( )->QueueEvent( new ScriptEvent( "GAME_INITIALIZED" ) );

	if ( programOptions.find( System::Options::LevelName ) != programOptions.end( ) )
	{
		std::string levelName = programOptions[ System::Options::LevelName ].As< std::string >( );
		LevelChangedEventData* eventData = new LevelChangedEventData( levelName );
		Management::Get( )->GetEventManager( )->QueueEvent( new Event( GAME_LEVEL_CHANGED, eventData ) );
	}

	m_isInitialized = true;
}

Game::~Game( )
{
	delete m_configuration;
}

void Game::Update( )
{
	assert( m_isInitialized && "Game::StartLoop - Cannot Start the Loop when not Initialized" );

	float deltaMilliseconds = Management::Get( )->GetPlatformManager( )->GetClock( ).GetDeltaMilliseconds( );

	m_worldLoader->Update( deltaMilliseconds );
	Management::Get( )->Update( deltaMilliseconds );
}

void Game::Release( )
{
	if ( !m_isInitialized )
	{
		UnInitializedException e( "Game::Release - Cannot Release when not Initialized" );
		Logger::Get( )->Fatal( e.what( ) );
		throw e;
	}

	Management::Get( )->GetEventManager( )->RemoveEventListener( GAME_QUIT, this, &Game::OnGameQuit );
	Management::Get( )->GetEventManager( )->RemoveEventListener( GAME_LEVEL_CHANGED, this, &Game::OnGameLevelChanged ); 

	m_world->Clear( );

	delete m_worldLoader;
	delete m_world;

	Management::Release( );
	Logger::Release( );
}

void Game::OnGameQuit( const IEvent* event )
{
	m_world->Destroy( );

	m_isQuitting = true;
}

void Game::OnGameLevelChanged( const IEvent* event )
{
	this->OnGameEnded( event );

	LevelChangedEventData* eventData = static_cast< LevelChangedEventData* >( event->GetEventData( ) );

	std::stringstream levelPath;
	levelPath << "/data/levels/" << eventData->GetLevelName( ) << ".xml";
	
	m_worldLoader->Load( levelPath.str( ) );

	Management::Get( )->GetInstrumentation( )->SetLevelName( eventData->GetLevelName( ) );
}

void Game::OnGameEnded( const IEvent* event )
{
	m_world->Clear( );
}

// EOF