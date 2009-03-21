#ifndef __PHYSICSSYSTEMRIGIDBODYCOMPONENT_H
#define __PHYSICSSYSTEMRIGIDBODYCOMPONENT_H

#include "PhysicsSystemComponent.h"

class PhysicsSystemRigidBodyComponent : public PhysicsSystemComponent
{

public:

	PhysicsSystemCharacterComponent( const std::string& name, HavokPhysicsSystemScene* scene )
		: _name ( name )
		, _scene( scene )
		, _loadedData( 0 )
	{

	}

private:


};

#endif