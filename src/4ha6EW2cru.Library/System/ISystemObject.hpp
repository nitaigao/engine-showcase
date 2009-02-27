#ifndef __ISYSTEMOBJECT_H
#define __ISYSTEMOBJECT_H

#include "SystemType.hpp"

class ISystemObject
{

public:

	virtual ~ISystemObject( ) { };

	virtual SystemType GetSystemType( ) = 0;

};

#endif