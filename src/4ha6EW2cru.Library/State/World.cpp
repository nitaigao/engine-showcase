#include "World.h"

#include "../System/ISystem.hpp"
#include "../Management/Management.h"

#include "WorldEntity.h"

namespace State
{
	World::~World()
	{
		for ( SystemSceneMap::reverse_iterator i = _systemScenes.rbegin( ); i != _systemScenes.rend( ); ++i )
		{
			delete ( *i ).second;
		}
	}
	
	IWorldEntity* World::CreateEntity( const std::string& name )
	{
		IWorldEntity* entity = new WorldEntity( name );
		_entities.push_back( entity );
		return entity;
	}
	
	float _stepAccumulator;
	
	void World::Update( const float& deltaMilliseconds )
	{
		float logicStep = 1.0f / 100.0f;
		_stepAccumulator += deltaMilliseconds;
	
		while( _stepAccumulator >= logicStep )
		{
			for( SystemSceneMap::iterator i = _systemScenes.begin( ); i != _systemScenes.end( ); ++i )
			{
				if( ( *i ).second->GetType( ) != System::Types::RENDER )
				{
					( *i ).second->Update( logicStep );
				}
			}
	
			_stepAccumulator -= logicStep;
		}
	
		for( SystemSceneMap::iterator i = _systemScenes.begin( ); i != _systemScenes.end( ); ++i )
		{
			if( ( *i ).second->GetType( ) == System::Types::RENDER )
			{
				( *i ).second->Update( deltaMilliseconds );
			}
		}
	}
	
	void World::Clear( )
	{
		for ( IWorldEntity::WorldEntityList::iterator e = _entities.begin( ); e != _entities.end( ); ++e )
		{
			SystemComponentList components = ( *e )->GetComponents( );
	
			for( SystemComponentList::iterator c = components.begin( ); c != components.end( ); ++c )
			{
				_systemScenes[ ( *c )->GetType( ) ]->DestroyComponent( ( *c ) );
			}
	
			delete ( *e );
		}
	
		_entities.clear( );
	}
	
	IWorldEntity* World::FindEntity( const std::string& name )
	{
		for ( IWorldEntity::WorldEntityList::iterator e = _entities.begin( ); e != _entities.end( ); ++e )
		{
			if ( ( *e )->GetName( ) == name )
			{
				return ( *e );
			}
		}
	
		return 0;
	}
}