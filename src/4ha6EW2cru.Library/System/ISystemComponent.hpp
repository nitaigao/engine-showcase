#ifndef __ISYSTEMENTITY_H
#define __ISYSTEMENTITY_H

#include "SystemType.hpp"
#include "SystemProperty.hpp"

class ISystemComponent
{

public:

	virtual ~ISystemComponent( ) { };

	virtual SystemType GetType( ) = 0;
	virtual void Initialize( SystemPropertyList properties ) = 0;

};

typedef std::vector< ISystemComponent* > SystemComponentList;


#endif