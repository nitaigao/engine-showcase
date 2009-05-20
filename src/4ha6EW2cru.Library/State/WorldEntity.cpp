#include "WorldEntity.h"

namespace State
{
	void WorldEntity::AddComponent( ISystemComponent* component )
	{
		m_components.push_back( component );

		component->AddObserver( this );
		component->Message( System::Messages::AddedToComponent, AnyValue::AnyValueKeyMap( ) );
	}

	AnyValue WorldEntity::Message( const unsigned int& messageId, AnyValue::AnyValueKeyMap parameters )
	{
		std::vector< AnyValue > results;

		for( SystemComponentList::const_iterator i = m_components.begin( ); i != m_components.end( ); ++i )
		{
			AnyValue result = ( *i )->Message( messageId, parameters );
			results.push_back( result );
		}

		ObserverMap::iterator observers = m_observers.find( messageId );

		while( observers != m_observers.end( ) )
		{
			AnyValue result = ( *observers ).second->Message( messageId, parameters );
			results.push_back( result );
			++observers;
		}

		return results;
	}
}