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

	ISystemManager* systemManager = Management::GetInstance( )->GetSystemManager( );
	systemManager->RegisterSystem( new Geometry::GeometrySystem( ) );
	systemManager->RegisterSystem( new Renderer::RendererSystem( m_configuration ) );
	systemManager->RegisterSystem( new Physics::HavokPhysicsSystem( ) );
	systemManager->RegisterSystem( new Input::InputSystem( m_configuration ) );
	systemManager->RegisterSystem( new Script::ScriptSystem( m_configuration ) );
	systemManager->RegisterSystem( new UX::UXSystem( ) );
	systemManager->RegisterSystem( new Sound::SoundSystem( m_configuration ) );
	systemManager->RegisterSystem( new AI::AISystem( ) );
	systemManager->InitializeAllSystems( );

	// -- Setup the World and World Loader

	m_world = systemManager->CreateWorld( );
	m_worldLoader = new Serialization::WorldSerializer( m_world );

	// -- Register Events

	Management::GetInstance( )->GetEventManager( )->AddEventListener( GAME_QUIT, this, &Game::OnGameQuit );
	Management::GetInstance( )->GetEventManager( )->AddEventListener( GAME_LEVEL_CHANGED, this, &Game::OnGameLevelChanged ); 
	Management::GetInstance( )->GetEventManager( )->AddEventListener( GAME_ENDED, this, &Game::OnGameEnded );
	Management::GetInstance( )->GetEventManager( )->QueueEvent( new ScriptEvent( "GAME_INITIALIZED" ) );

	AnyType::AnyTypeMap programOptions = Management::GetInstance( )->GetPlatformManager( )->GetProgramOptions( );
	
	if ( programOptions.find( System::Options::LevelName ) != programOptions.end( ) )
	{
		std::string levelName = programOptions[ System::Options::LevelName ].As< std::string >( );
		LevelChangedEventData* eventData = new LevelChangedEventData( levelName );
		Management::GetInstance( )->GetEventManager( )->QueueEvent( new Event( GAME_LEVEL_CHANGED, eventData ) );
	}

	m_isInitialized = true;
}

Game::~Game( )
{
	delete m_configuration;
}

void Game::Update( const float& deltaMilliseconds )
{
	if ( !m_isInitialized )
	{
		UnInitializedException e( "Game::StartLoop - Cannot Start the Loop when not Initialized" );
		Logger::Fatal( e.what( ) );
		throw e;
	}

	m_worldLoader->Update( deltaMilliseconds );

	if ( m_worldLoader->IsFinishedLoading( ) )
	{
		m_world->Update( deltaMilliseconds );
	}

	Management::GetInstance( )->Update( deltaMilliseconds );
}

void Game::Release( )
{
	if ( !m_isInitialized )
	{
		UnInitializedException e( "Game::Release - Cannot Release when not Initialized" );
		Logger::Fatal( e.what( ) );
		throw e;
	}

	Management::GetInstance( )->GetEventManager( )->RemoveEventListener( GAME_QUIT, this, &Game::OnGameQuit );
	Management::GetInstance( )->GetEventManager( )->RemoveEventListener( GAME_LEVEL_CHANGED, this, &Game::OnGameLevelChanged ); 

	m_world->Clear( );

	delete m_worldLoader;
	delete m_world;

	Management::GetInstance( )->Release( );
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
	levelPath << "/data/levels/" << eventData->GetLevelName( ) << ".yaml";
	
	m_worldLoader->Load( levelPath.str( ) );
}

void Game::OnGameEnded( const IEvent* event )
{
	m_world->Clear( );

	ISystem* renderSystem = Management::GetInstance( )->GetSystemManager( )->GetSystem( System::Types::RENDER );

	renderSystem->SetAttribute( "activeCamera", "default" );
	renderSystem->SetAttribute( "backgroundColor", Renderer::Color( 0.0f, 0.0f, 0.0f ) );
}

// EOF