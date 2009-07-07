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
#include "Geometry/GeometrySystem.h"
#include "Physics/HavokPhysicsSystem.h"
#include "AI/AISystem.h"
#include "UX/UXSystem.h"
#include "Sound/SoundSystem.h"
#include "Animation/AnimationSystem.h"
//#include "NetworkSystem.h"

#include "Events/Event.h"
#include "Events/EventData.hpp"
using namespace Events;

#include "Renderer/Color.hpp"
#include "Scripting/ScriptEvent.hpp"
using namespace Script;

#include "System/SystemExports.hpp"

void Game::Initialize( )
{
	AnyType::AnyTypeMap programOptions = Management::GetPlatformManager( )->GetProgramOptions( );

	if ( m_isInitialized )
	{
		AlreadyInitializedException e ( "Game::Initialize - Attempted to Initialized when the game had already been Initialized" );
		Logger::Fatal( e.what( ) );
		throw e;
	}

#ifdef _DEBUG
	Logger::SetLogLevel( Logging::LEVEL_ALL );
#endif // _DEBUG

	// -- Initialize All Singletons

	Management::Initialize( );

	// -- Set Configuration Defaults 

	m_configuration = ClientConfiguration::Load( "config/game.cfg" );
	m_configuration->SetDefault( "Developer", "console", false );
	m_configuration->SetDefault( "Logging", "level", static_cast< int >( LEVEL_FATAL ) );

	LogLevel logLevel = static_cast< LogLevel >( m_configuration->Find( "Logging", "level" ).As< int >( ) );
	Logger::SetLogLevel( logLevel );

	// -- Initialize All Systems

	ISystemManager* systemManager = Management::GetSystemManager( );

	// -- Server

	systemManager->RegisterSystem( System::Queues::HOUSE, new Geometry::GeometrySystem( ) );
	systemManager->RegisterSystem( System::Queues::LOGIC, new Physics::HavokPhysicsSystem( ) );
	systemManager->RegisterSystem( System::Queues::HOUSE, new Script::ScriptSystem( m_configuration ) );
	systemManager->RegisterSystem( System::Queues::HOUSE, new AI::AISystem( ) );

	HMODULE library = LoadLibraryA( "4ha6EW2cru.Network.dll" );

	if ( library )
	{
		CreateSystemFunction createSystem = reinterpret_cast< CreateSystemFunction >( GetProcAddress( library, "CreateSystem" ) );
		systemManager->RegisterSystem( System::Queues::HOUSE, createSystem( m_configuration ) );
	}

	if ( programOptions.find( System::Options::DedicatedServer ) != programOptions.end( ) )
	{
		systemManager->GetSystem( System::Types::NETWORK )->SetAttribute( System::Attributes::Network::IsServer, true );
		Management::GetPlatformManager( )->CreateConsoleWindow( );
	}
	else
	{
		systemManager->RegisterSystem( System::Queues::RENDER, new Renderer::RendererSystem( m_configuration ) );
		systemManager->RegisterSystem( System::Queues::HOUSE, new Animation::AnimationSystem( ) );
		systemManager->RegisterSystem( System::Queues::HOUSE, new Input::InputSystem( m_configuration ) );
		systemManager->RegisterSystem( System::Queues::HOUSE, new UX::UXSystem( ) );
		systemManager->RegisterSystem( System::Queues::HOUSE, new Sound::SoundSystem( m_configuration ) );

		systemManager->GetSystem( System::Types::NETWORK )->SetAttribute( System::Attributes::Network::IsServer, false );
	}

	systemManager->InitializeAllSystems( );

	// -- Setup the World and World Loader

	m_world = systemManager->CreateWorld( );
	m_worldLoader = new Serialization::XMLSerializer( m_world );

	// -- Register Events

	Management::GetEventManager( )->AddEventListener( GAME_QUIT, this, &Game::OnGameQuit );
	Management::GetEventManager( )->AddEventListener( GAME_LEVEL_CHANGED, this, &Game::OnGameLevelChanged ); 
	Management::GetEventManager( )->AddEventListener( GAME_ENDED, this, &Game::OnGameEnded );
	Management::GetEventManager( )->QueueEvent( new ScriptEvent( "GAME_INITIALIZED" ) );

	if ( programOptions.find( System::Options::LevelName ) != programOptions.end( ) )
	{
		std::string levelName = programOptions[ System::Options::LevelName ].As< std::string >( );
		LevelChangedEventData* eventData = new LevelChangedEventData( levelName );
		Management::GetEventManager( )->QueueEvent( new Event( GAME_LEVEL_CHANGED, eventData ) );
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

	float deltaMilliseconds = Management::GetPlatformManager( )->GetClock( ).GetDeltaMilliseconds( );

	m_worldLoader->Update( deltaMilliseconds );
	Management::Update( deltaMilliseconds );
}

void Game::Release( )
{
	if ( !m_isInitialized )
	{
		UnInitializedException e( "Game::Release - Cannot Release when not Initialized" );
		Logger::Fatal( e.what( ) );
		throw e;
	}

	Management::GetEventManager( )->RemoveEventListener( GAME_QUIT, this, &Game::OnGameQuit );
	Management::GetEventManager( )->RemoveEventListener( GAME_LEVEL_CHANGED, this, &Game::OnGameLevelChanged ); 

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