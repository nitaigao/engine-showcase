#ifndef __PHYSICSSYSTEMCHARACTERCOMPONENT_H
#define __PHYSICSSYSTEMCHARACTERCOMPONENT_H

#include "PhysicsSystemComponent.h"

class PhysicsSystemCharacterComponent : public PhysicsSystemComponent
{

public:

	PhysicsSystemCharacterComponent( const std::string& name, HavokPhysicsSystemScene* scene )
		: PhysicsSystemComponent( name, scene )
	{

	};

	void Initialize( SystemPropertyList properties );


};

#endif