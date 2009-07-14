#include "SystemManager.h"

using namespace State;
using namespace Configuration;

#include "../Logging/Logger.h"
using namespace Logging;

#include "../Management/Management.h"

#include "SystemExports.hpp"
#include "../Exceptions/FileNotFoundException.hpp"

void SystemManager::RegisterSystem( const System::Queues::Queue& systemQueue, ISystem* system )
{
	_systemsByQueue.insert( std::make_pair( systemQueue, system ) );
	_systemsByType.insert( std::make_pair( system->GetType( ), system ) );

	system->Message( System::Messages::RegisterService, AnyType::AnyTypeMap( ) );
}

ISystem* SystemManager::GetSystem( const System::Types::Type& systemType ) const
{
	return ( *( _systemsByType.find( systemType ) ) ).second;
}

void SystemManager::InitializeAllSystems( IConfiguration* configuration )
{
	for( ISystem::SystemTypeMap::iterator i = _systemsByType.begin( ); i != _systemsByType.end( ); ++i )
	{
		( *i ).second->Initialize( configuration );
	}
}

void SystemManager::Update( const float& deltaMilliseconds )
{
	Management::Get( )->GetInstrumentation( )->SetFPS( 1.0f / deltaMilliseconds );

	int loopCount = 0;
	int maxLoops = 10;

	float step = 1.0f / 100.0f;
	m_accumulator += deltaMilliseconds;

	float logicStart = Management::Get( )->GetPlatformManager( )->GetClock( ).GetTime( );

	while( m_accumulator >= deltaMilliseconds && loopCount < maxLoops )
	{
		for( ISystem::SystemQueueMap::iterator i = _systemsByQueue.begin( ); i != _systemsByQueue.end( ); ++i )
		{
			if ( ( *i ).first == System::Queues::LOGIC )
			{
				( *i ).second->Update( step );
			}
		}

		m_accumulator -= step;
		loopCount++;
	}

	float logicEnd = Management::Get( )->GetPlatformManager( )->GetClock( ).GetTime( );

	Management::Get( )->GetInstrumentation( )->SetRoundTime( System::Queues::LOGIC, logicEnd - logicStart );

	float houseKeepingStart = Management::Get( )->GetPlatformManager( )->GetClock( ).GetTime( );

	for( ISystem::SystemQueueMap::iterator i = _systemsByQueue.begin( ); i != _systemsByQueue.end( ); ++i )
	{
		if ( ( *i ).first == System::Queues::HOUSE )
		{
			( *i ).second->Update( deltaMilliseconds );
		}
	}

	float houseKeepingEnd = Management::Get( )->GetPlatformManager( )->GetClock( ).GetTime( );

	Management::Get( )->GetInstrumentation( )->SetRoundTime( System::Queues::HOUSE, houseKeepingEnd - houseKeepingStart );

	float presentationStart = Management::Get( )->GetPlatformManager( )->GetClock( ).GetTime( );

	for( ISystem::SystemQueueMap::iterator i = _systemsByQueue.begin( ); i != _systemsByQueue.end( ); ++i )
	{
		if ( ( *i ).first == System::Queues::RENDER )
		{
			( *i ).second->Update( deltaMilliseconds );
		}
	}

	float presentationEnd = Management::Get( )->GetPlatformManager( )->GetClock( ).GetTime( );

	Management::Get( )->GetInstrumentation( )->SetRoundTime( System::Queues::RENDER, presentationEnd - presentationStart );
}

void SystemManager::Release( )
{
	for( ISystem::SystemTypeMap::const_reverse_iterator i = _systemsByType.rbegin( ); i != _systemsByType.rend( ); ++i )
	{
		( *i ).second->Release( );

		HMODULE library = m_systemLibraries[ ( *i ).second ];

		DestroySystemFunction destroySystem = reinterpret_cast< DestroySystemFunction >( GetProcAddress( library, "DestroySystem" ) );
		destroySystem( ( *i ).second );
	}

	_systemsByType.clear( );
	_systemsByQueue.clear( );

	m_systemLibraries.clear( );
}

IWorld* SystemManager::CreateWorld()
{
	IWorld* world = new World( );

	for( ISystem::SystemTypeMap::iterator i = _systemsByType.begin( ); i != _systemsByType.end( ); ++i )
	{
		world->AddSystemScene( ( *i ).second->CreateScene( ) );
	}

	return world;
}

bool SystemManager::HasSystem( const System::Types::Type& systemType ) const
{
	return ( _systemsByType.find( systemType ) != _systemsByType.end( ) );
}

ISystem* SystemManager::LoadSystem( const std::string& systemPath )
{
	HMODULE library = LoadLibrary( systemPath.c_str( ) );

	if ( library == NULL )
	{
		FileNotFoundException e( "SystemManager::LoadSystem - Unable to load the given System dll" );
		Logger::Get( )->Fatal( e.what( ) );
		throw e;
	}

	DWORD error = GetLastError( );

	InitializeSystemFunction initializeSystem = reinterpret_cast< InitializeSystemFunction >( GetProcAddress( library, "Initialize" ) );
	initializeSystem( Management::Get( ), Logger::Get( ) );


	CreateSystemFunction createSystem = reinterpret_cast< CreateSystemFunction >( GetProcAddress( library, "CreateSystem" ) );
	ISystem* system = createSystem( );

	m_systemLibraries.insert( std::make_pair( system, library ) );

	return system;
}