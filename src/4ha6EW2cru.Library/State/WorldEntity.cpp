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

		for ( ObserverMap::iterator i = m_observers.begin( ); i != m_observers.end( ); ++i )
		{
			if ( ( *i ).first == System::Messages::All_Messages || ( *i ).first == message )
			{
				( *i ).second->Message( message, parameters );
			}
		}

		return results;
	}

	void WorldEntity::Initialize( )
	{
		this->Message( System::Messages::PreInitialize, AnyType::AnyTypeMap( ) );

		for( ISystemComponent::SystemComponentList::const_iterator i = m_components.begin( ); i != m_components.end( ); ++i )
		{
			( *i )->Initialize( ); 
		}

		this->Message( System::Messages::PostInitialize, AnyType::AnyTypeMap( ) );
	}
}