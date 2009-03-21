#include "Entity.h"

void Entity::AddComponent( ISystemComponent* component )
{
	component->AddObserver( this );
	_components.push_back( component );
}

void Entity::Observe( ISubject* subject, unsigned int systemChanges )
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