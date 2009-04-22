#ifndef __ISERVICEMANAGER_H
#define __ISERVICEMANAGER_H

#include "IService.hpp"

class IServiceManager
{

public:

	virtual ~IServiceManager( ) { };

	virtual void RegisterService( IService* service ) = 0;
	virtual IService* FindService( SystemType systemType ) const = 0;

};

#endif