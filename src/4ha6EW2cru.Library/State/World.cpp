#include "World.h"

#include "../System/ISystem.hpp"
#include "../Management/Management.h"

#include "WorldEntity.h"

namespace State
{
	World::~World()
	{
		for ( ISystemScene::SystemSceneMap::reverse_iterator i = m_systemScenes.rbegin( ); i != m_systemScenes.rend( ); ++i )
		{
			( *i ).second->Destroy( );
			delete ( *i ).second;
		}
	}
	
	IWorldEntity* World::CreateEntity( const std::string& name )
	{
		IWorldEntity* entity = new WorldEntity( name );
		m_entities.push_back( entity );
		return entity;
	}
	
	void World::Update( const float& deltaMilliseconds )
	{
		for( ISystemScene::SystemSceneMap::iterator i = m_systemScenes.begin( ); i != m_systemScenes.end( ); ++i )
		{
			( *i ).second->Update( deltaMilliseconds );
		}
	}
	
	void World::Clear( )
	{
		for ( IWorldEntity::WorldEntityList::iterator e = m_entities.begin( ); e != m_entities.end( ); ++e )
		{
			ISystemComponent::SystemComponentList components = ( *e )->GetComponents( );
	
			for( ISystemComponent::SystemComponentList::iterator c = components.begin( ); c != components.end( ); ++c )
			{
				m_systemScenes[ ( *c )->GetAttributes( )[ System::Attributes::SystemType ].As< System::Types::Type >( ) ]->DestroyComponent( ( *c ) );
			}
	
			delete ( *e );
		}
	
		m_entities.clear( );
	}
	
	IWorldEntity* World::FindEntity( const std::string& name )
	{
		for ( IWorldEntity::WorldEntityList::iterator e = m_entities.begin( ); e != m_entities.end( ); ++e )
		{
			if ( ( *e )->GetName( ) == name )
			{
				return ( *e );
			}
		}
	
		return 0;
	}
}