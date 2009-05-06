#include "SystemManager.h"

using namespace State;

SystemManager::~SystemManager()
{
	for( ISystem::SystemMap::reverse_iterator i = _systems.rbegin( ); i != _systems.rend( ); ++i )
	{
		delete ( *i ).second;
	}

	_systems.clear( );
}

void SystemManager::RegisterSystem( ISystem* system )
{
	_systems[ system->GetType( ) ] = system;
}

ISystem* SystemManager::GetSystem( const System::Types::Type& systemType ) const
{
	return ( *( _systems.find( systemType ) ) ).second;
}

void SystemManager::InitializeAllSystems()
{
	for( ISystem::SystemMap::iterator i = _systems.begin( ); i != _systems.end( ); ++i )
	{
		( *i ).second->Initialize( );
	}
}

void SystemManager::Update( const float& deltaMilliseconds )
{
	for( ISystem::SystemMap::iterator i = _systems.begin( ); i != _systems.end( ); ++i )
	{
		( *i ).second->Update( deltaMilliseconds );
	}
}

void SystemManager::Release()
{
	for( ISystem::SystemMap::reverse_iterator i = _systems.rbegin( ); i != _systems.rend( ); ++i )
	{
		( *i ).second->Release( );
	}
}

IWorld* SystemManager::CreateWorld()
{
	IWorld* world = new World( );

	for( ISystem::SystemMap::iterator i = _systems.begin( ); i != _systems.end( ); ++i )
	{
		world->AddSystemScene( ( *i ).second->CreateScene( ) );
	}

	return world;
}

bool SystemManager::HasSystem( const System::Types::Type& systemType ) const
{
	return ( _systems.find( systemType ) != _systems.end( ) );
}