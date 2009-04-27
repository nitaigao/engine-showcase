#include "WorldEntity.h"

namespace State
{
	void WorldEntity::AddComponent( ISystemComponent* component )
	{
		component->AddObserver( this );
		_components.push_back( component );
	}
	
	void WorldEntity::Observe( ISubject* subject, const unsigned int& systemChanges )
	{
		for( SystemComponentList::iterator i = _components.begin( ); i != _components.end( ); ++i )
		{
			ISystemComponent* component = ( *i );
			
			if ( component->GetRequestedChanges( ) & systemChanges && component != subject )
			{
				( *i )->Observe( subject, systemChanges );
			}
		}
	}
	
	ISystemComponent* WorldEntity::FindComponent( const System::Types::Type& systemType ) const
	{
		for( SystemComponentList::const_iterator i = _components.begin( ); i != _components.end( ); ++i )
		{
			if ( systemType == ( *i )->GetType( ) )
			{
				return ( *i );
			}
		}
	
		return 0;
	}
}