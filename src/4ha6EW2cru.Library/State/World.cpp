#include "World.h"

#include "../System/ISystem.hpp"
#include "../System/Management.h"

#include "Entity.h"

World::~World()
{
	this->Clear( );

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

void World::Update( float deltaMilliseconds )
{
	for( SystemSceneMap::iterator i = _systemScenes.begin( ); i != _systemScenes.end( ); ++i )
	{
		( *i ).second->Update( deltaMilliseconds );
	}
}

void World::Clear( )
{
	ISystem::PropertyMap renderSystemProperties;
	SystemProperty cameraProperty = SystemProperty( "camera", std::string( "default" ) );
	renderSystemProperties.insert( std::make_pair( cameraProperty.GetName( ), cameraProperty ) );
	Management::GetInstance( )->GetSystemManager( )->GetSystem( RenderSystemType )->SetProperties( renderSystemProperties );

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