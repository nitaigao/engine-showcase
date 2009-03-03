#ifndef __Entity_H
#define __Entity_H

class Entity : public IEntity
{

public:

	virtual ~Entity( ) { };

	Entity( const std::string& name )
		: _name( name )
	{

	}

	inline const std::string& GetName( ) { return _name; };
	inline void AddComponent( ISystemComponent* component ) { _components.push_back( component ); };
	inline ComponentList& GetComponents( ) { return _components; };

private:

	std::string _name;
	ComponentList _components;

};

#endif