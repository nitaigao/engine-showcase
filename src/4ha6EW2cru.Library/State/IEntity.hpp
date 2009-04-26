#ifndef __IENTITY_H
#define __IENTITY_H

#include <vector>

#include "../System/ISystemComponent.hpp"

class IEntity
{

public:

	virtual ~IEntity( ) { };

	virtual const std::string& GetName( ) = 0;
	virtual void AddComponent( ISystemComponent* component ) = 0;
	virtual ISystemComponent* FindComponent( System::Types::Type systemType ) = 0;
	virtual SystemComponentList& GetComponents( ) = 0;

};

typedef std::vector< IEntity* > EntityList;

#endif