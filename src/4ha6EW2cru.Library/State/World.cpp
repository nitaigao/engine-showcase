#include "World.h"

#include "../System/ISystem.hpp"
#include "../System/Management.h"

#include "Entity.h"

World::~World()
{
	for ( SystemSceneMap::iterator i = _systemScenes.begin( ); i != _systemScenes.end( ); ++i )
	{
		delete ( *i ).second;
	}
}

IEntity* World::CreateEntity( const std::string& name )
{
	IEntity* entity = new Entity( name );
	_entities.push_back( entity );
	return entity;
}

float _stepAccumulator;

void World::Update( float deltaMilliseconds )
{
	float logicStep = 1.0f / 100.0f;
	_stepAccumulator += deltaMilliseconds;

	while( _stepAccumulator >= logicStep )
	{
		for( SystemSceneMap::iterator i = _systemScenes.begin( ); i != _systemScenes.end( ); ++i )
		{
			if( ( *i ).second->GetType( ) != RenderSystemType )
			{
				( *i ).second->Update( logicStep );
			}
		}

		_stepAccumulator -= logicStep;
	}

	for( SystemSceneMap::iterator i = _systemScenes.begin( ); i != _systemScenes.end( ); ++i )
	{
		if( ( *i ).second->GetType( ) == RenderSystemType )
		{
			( *i ).second->Update( deltaMilliseconds );
		}
	}
}

void World::Clear( )
{
	for ( EntityList::iterator e = _entities.begin( ); e != _entities.end( ); ++e )
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

IEntity* World::FindEntity( const std::string& name )
{
	for ( EntityList::iterator e = _entities.begin( ); e != _entities.end( ); ++e )
	{
		if ( ( *e )->GetName( ) == name )
		{
			return ( *e );
		}
	}
}