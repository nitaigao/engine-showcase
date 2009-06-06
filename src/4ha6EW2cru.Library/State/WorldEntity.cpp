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
		AnyType::AnyTypeKeyMap results;

		for( ISystemComponent::SystemComponentList::const_iterator i = m_components.begin( ); i != m_components.end( ); ++i )
		{
			AnyType result = ( *i )->Message( message, parameters );
			System::Types::Type systemType = ( *i )->GetAttributes( )[ System::Attributes::SystemType ].As< System::Types::Type >( );
			results.insert( std::make_pair( systemType, result ) );
		}

		ObserverMap::iterator observers = m_observers.find( message );

		while( observers != m_observers.end( ) )
		{
			( *observers ).second->Message( message, parameters );
			++observers;
		}

		return results;
	}
}