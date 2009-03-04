#ifndef __Entity_H
#define __Entity_H

#include "IEntity.hpp"
#include "../System/ISystemComponent.hpp"

class Entity : public IEntity, public IObserver
{

public:

	virtual ~Entity( ) { };

	Entity( const std::string& name )
		: _name( name )
	{

	}

	inline const std::string& GetName( ) { return _name; };

	void AddComponent( ISystemComponent* component );
	void Observe( ISubject* subject );
	inline SystemComponentList& GetComponents( ) { return _components; };

private:

	std::string _name;
	SystemComponentList _components;

};

#endif