#include "SystemManager.h"

using namespace State;

#include "../Management/Management.h"

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

void SystemManager::InitializeAllSystems( )
{
	for( ISystem::SystemTypeMap::iterator i = _systemsByType.begin( ); i != _systemsByType.end( ); ++i )
	{
		( *i ).second->Initialize( );
	}
}

void SystemManager::Update( const float& deltaMilliseconds )
{
	Management::GetInstrumentation( )->SetFPS( 1.0f / deltaMilliseconds );

	int loopCount = 0;
	int maxLoops = 10;

	float step = 1.0f / 100.0f;
	float accumulator = step + deltaMilliseconds;

	float logicStart = Management::GetPlatformManager( )->GetClock( ).GetTime( );

	while( accumulator > step && loopCount < maxLoops )
	{
		for( ISystem::SystemQueueMap::iterator i = _systemsByQueue.begin( ); i != _systemsByQueue.end( ); ++i )
		{
			if ( ( *i ).first == System::Queues::LOGIC )
			{
				( *i ).second->Update( step );
			}
		}

		accumulator -= step;
		loopCount++;
	}

	float logicEnd = Management::GetPlatformManager( )->GetClock( ).GetTime( );

	Management::GetInstrumentation( )->SetRoundTime( System::Queues::LOGIC, logicEnd - logicStart );

	float houseKeepingStart = Management::GetPlatformManager( )->GetClock( ).GetTime( );

	for( ISystem::SystemQueueMap::iterator i = _systemsByQueue.begin( ); i != _systemsByQueue.end( ); ++i )
	{
		if ( ( *i ).first == System::Queues::HOUSE )
		{
			( *i ).second->Update( deltaMilliseconds );
		}
	}

	float houseKeepingEnd = Management::GetPlatformManager( )->GetClock( ).GetTime( );

	Management::GetInstrumentation( )->SetRoundTime( System::Queues::HOUSE, houseKeepingEnd - houseKeepingStart );

	float presentationStart = Management::GetPlatformManager( )->GetClock( ).GetTime( );

	for( ISystem::SystemQueueMap::iterator i = _systemsByQueue.begin( ); i != _systemsByQueue.end( ); ++i )
	{
		if ( ( *i ).first == System::Queues::RENDER )
		{
			( *i ).second->Update( deltaMilliseconds );
		}
	}

	float presentationEnd = Management::GetPlatformManager( )->GetClock( ).GetTime( );

	Management::GetInstrumentation( )->SetRoundTime( System::Queues::RENDER, presentationEnd - presentationStart );
}

void SystemManager::Release( )
{
	for( ISystem::SystemTypeMap::const_reverse_iterator i = _systemsByType.rbegin( ); i != _systemsByType.rend( ); ++i )
	{
		( *i ).second->Release( );
		delete ( *i ).second;
	}

	_systemsByType.clear( );
	_systemsByQueue.clear( );
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