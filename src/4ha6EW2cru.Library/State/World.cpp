#include "World.h"

#include "../System/ISystem.hpp"
#include "../Management/Management.h"

#include "WorldEntity.h"

namespace State
{
	World::~World()
	{
		for ( SystemSceneMap::reverse_iterator i = m_systemScenes.rbegin( ); i != m_systemScenes.rend( ); ++i )
		{
			( *i ).second->Destroy( );
			delete ( *i ).second;
		}
	}
	
	IWorldEntity* World::CreateEntity( const std::string& name )
	{
		IWorldEntity* entity = new WorldEntity( name, m_lastEntityId++ );
		m_entities.push_back( entity );
		return entity;
	}
	
	void World::Update( const float& deltaMilliseconds )
	{
		float logicStep = 1.0f / 100.0f;
		m_stepAccumulator += deltaMilliseconds;
	
		while( m_stepAccumulator >= logicStep )
		{
			for( SystemSceneMap::iterator i = m_systemScenes.begin( ); i != m_systemScenes.end( ); ++i )
			{
				if ( 
					( *i ).second->GetType( ) != System::Types::AI && 
					( *i ).second->GetType( ) != System::Types::RENDER
					) 
				{
					( *i ).second->Update( logicStep );
				}
			}
	
			m_stepAccumulator -= logicStep;
		}
	
		for( SystemSceneMap::iterator i = m_systemScenes.begin( ); i != m_systemScenes.end( ); ++i )
		{
			if( 
				( *i ).second->GetType( ) == System::Types::INPUT ||
				( *i ).second->GetType( ) == System::Types::AI ||
				( *i ).second->GetType( ) == System::Types::RENDER
				)
			{
				( *i ).second->Update( deltaMilliseconds );
			}
		}
	}
	
	void World::Clear( )
	{
		for ( IWorldEntity::WorldEntityList::iterator e = m_entities.begin( ); e != m_entities.end( ); ++e )
		{
			SystemComponentList components = ( *e )->GetComponents( );
	
			for( SystemComponentList::iterator c = components.begin( ); c != components.end( ); ++c )
			{
				m_systemScenes[ ( *c )->GetType( ) ]->DestroyComponent( ( *c ) );
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