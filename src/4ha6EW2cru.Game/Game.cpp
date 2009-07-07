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

#include "State/Serilaization/XMLSerializer.h"
using namespace Serialization;

#include "Scripting/ScriptSystem.h"
#include "Input/InputSystem.h"
#include "Renderer/RendererSystem.h"
#include "Physics/HavokPhysicsSystem.h"
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
	Management::Initialize( );

	AnyType::AnyTypeMap programOptions = Management::Get( )->GetPlatformManager( )->GetProgramOptions( );

	if ( m_isInitialized )
	{
		AlreadyInitializedException e ( "Game::Initialize - Attempted to Initialized when the game had already been Initialized" );
		Logger::Fatal( e.what( ) );
		throw e;
	}

#ifdef _DEBUG
	Logger::SetLogLevel( Logging::LEVEL_ALL );
#endif // _DEBUG

	// -- Set Configuration Defaults 

	m_configuration = ClientConfiguration::Load( "config/game.cfg" );
	m_configuration->SetDefault( "Developer", "console", false );
	m_configuration->SetDefault( "Logging", "level", static_cast< int >( LEVEL_FATAL ) );

	LogLevel logLevel = static_cast< LogLevel >( m_configuration->Find( "Logging", "level" ).As< int >( ) );
	Logger::SetLogLevel( logLevel );

	// -- Initialize All Systems

	ISystemManager* systemManager = Management::Get( )->GetSystemManager( );

	// -- Server

	ISystem* geometrySystem = systemManager->LoadSystem( "4ha6EW2cru.Geometry.dll" );
	systemManager->RegisterSystem( System::Queues::HOUSE, geometrySystem );

	systemManager->RegisterSystem( System::Queues::LOGIC, new Physics::HavokPhysicsSystem( ) );
	systemManager->RegisterSystem( System::Queues::HOUSE, new Script::ScriptSystem( ) );

	ISystem* networkSystem = systemManager->LoadSystem( "4ha6EW2cru.Network.dll" );
	systemManager->RegisterSystem( System::Queues::HOUSE, networkSystem );

	ISystem* aiSystem = systemManager->LoadSystem( "4ha6EW2cru.AI.dll" );
	systemManager->RegisterSystem( System::Queues::HOUSE, aiSystem );

	if ( programOptions.find( System::Options::DedicatedServer ) != programOptions.end( ) )
	{
		systemManager->GetSystem( System::Types::NETWORK )->SetAttribute( System::Attributes::Network::IsServer, true );
		Management::Get( )->GetPlatformManager( )->CreateConsoleWindow( );
	}
	else
	{
		systemManager->RegisterSystem( System::Queues::RENDER, new Renderer::RendererSystem( ) );

		ISystem* animationSystem = systemManager->LoadSystem( "4ha6EW2cru.Animation.dll" );
		systemManager->RegisterSystem( System::Queues::HOUSE, animationSystem );

		systemManager->RegisterSystem( System::Queues::HOUSE, new Input::InputSystem( ) );
		systemManager->RegisterSystem( System::Queues::HOUSE, new UX::UXSystem( ) );
		systemManager->RegisterSystem( System::Queues::HOUSE, new Sound::SoundSystem( ) );

		systemManager->GetSystem( System::Types::NETWORK )->SetAttribute( System::Attributes::Network::IsServer, false );
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
		Logger::Fatal( e.what( ) );
		throw e;
	}

	Management::Get( )->GetEventManager( )->RemoveEventListener( GAME_QUIT, this, &Game::OnGameQuit );
	Management::Get( )->GetEventManager( )->RemoveEventListener( GAME_LEVEL_CHANGED, this, &Game::OnGameLevelChanged ); 

	m_world->Clear( );

	delete m_worldLoader;
	delete m_world;

	Management::Release( );
}

void Game::OnGameQuit( const IEvent* event )
{
	m_isQuitting = true;
}

void Game::OnGameLevelChanged( const IEvent* event )
{
	this->OnGameEnded( event );

	LevelChangedEventData* eventData = static_cast< LevelChangedEventData* >( event->GetEventData( ) );

	std::stringstream levelPath;
	levelPath << "/data/levels/" << eventData->GetLevelName( ) << ".xml";
	
	m_worldLoader->Load( levelPath.str( ) );
}

void Game::OnGameEnded( const IEvent* event )
{
	m_world->Clear( );
}

// EOF