#include "WorldEntity.h"

namespace State
{
	void WorldEntity::AddComponent( ISystemComponent* component )
	{
		component->SetId( m_id );
		component->AddObserver( this );

		m_components.push_back( component );
	}
	
	void WorldEntity::Observe( ISubject* subject, const unsigned int& systemChanges )
	{
		for( SystemComponentList::iterator i = m_components.begin( ); i != m_components.end( ); ++i )
		{		
			if ( ( *i )->GetRequestedChanges( ) & systemChanges && ( *i ) != subject )
			{
				( *i )->Observe( subject, systemChanges );
			}
		}
	}
	
	ISystemComponent* WorldEntity::FindComponent( const System::Types::Type& systemType ) const
	{
		for( SystemComponentList::const_iterator i = m_components.begin( ); i != m_components.end( ); ++i )
		{
			if ( systemType == ( *i )->GetType( ) )
			{
				return ( *i );
			}
		}
	
		return 0;
	}

	AnyValue WorldEntity::Message( const std::string& message, AnyValue::AnyValueMap parameters )
	{
		std::vector< AnyValue > results;

		for( SystemComponentList::const_iterator i = m_components.begin( ); i != m_components.end( ); ++i )
		{
			AnyValue result = ( *i )->Message( message, parameters );
			results.push_back( result );
		}

		return results;
	}
}