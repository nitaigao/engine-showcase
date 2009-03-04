#include "Entity.h"

void Entity::AddComponent( ISystemComponent* component )
{
	component->AddObserver( this );
	_components.push_back( component );
}

void Entity::Observe( ISubject* subject )
{
	for( SystemComponentList::iterator i = _components.begin( ); i != _components.end( ); ++i )
	{
		( *i )->Observe( subject );
	}
}