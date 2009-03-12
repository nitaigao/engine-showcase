#ifndef __PHYSICSSYSTEMCOMPONENT_H
#define __PHYSICSSYSTEMCOMPONENT_H

#include "../System/ISystemComponent.hpp"
#include "HavokPhysicsSystemScene.h"

class PhysicsSystemComponent : public ISystemComponent
{

public:

	virtual ~PhysicsSystemComponent( ) { };

	PhysicsSystemComponent( const std::string& name, HavokPhysicsSystemScene* scene )
		: _name ( name )
		, _scene( scene )
	{

	}

	void Initialize( SystemPropertyList properties );
	void AddObserver( IObserver* observer ) { };

	void Observe( ISubject* subject ) { };
	void PushChanges( unsigned int systemChanges ) { };

	const std::string& GetName( ) { return _name; };
	SystemType GetType( ) { return PhysicsSystemType; };

private:

	std::string _name;
	HavokPhysicsSystemScene* _scene;

};

#endif