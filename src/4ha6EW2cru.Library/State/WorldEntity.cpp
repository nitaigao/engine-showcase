#include "WorldEntity.h"

namespace State
{
	void WorldEntity::AddComponent( ISystemComponent* component )
	{
		m_components.push_back( component );

		component->AddObserver( this );
		component->Message( System::Messages::AddedToComponent, AnyType::AnyTypeMap( ) );
	}

	AnyType WorldEntity::Message( const System::Message& message, AnyType::AnyTypeMap parameters )
	{
		std::vector< AnyType > results;

		for( ISystemComponent::SystemComponentList::const_iterator i = m_components.begin( ); i != m_components.end( ); ++i )
		{
			AnyType result = ( *i )->Message( message, parameters );
			results.push_back( result );
		}

		ObserverMap::iterator observers = m_observers.find( message );

		while( observers != m_observers.end( ) )
		{
			AnyType result = ( *observers ).second->Message( message, parameters );
			results.push_back( result );
			++observers;
		}

		return results;
	}
}