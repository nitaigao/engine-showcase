#ifndef __IENTITY_H
#define __IENTITY_H

#include <vector>

#include "../System/ISystemComponent.hpp"

class IEntity
{

protected:

	typedef std::vector< ISystemComponent* > ComponentList;

public:

	virtual ~IEntity( ) { };

	virtual const std::string& GetName( ) = 0;
	virtual void AddComponent( ISystemComponent* component ) = 0;
	virtual ComponentList& GetComponents( ) = 0;

};

typedef std::vector< IEntity* > EntityList;

#endif