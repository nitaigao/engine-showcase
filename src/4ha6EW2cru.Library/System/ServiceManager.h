#ifndef __SERVICEMANAGER_H
#define __SERVICEMANAGER_H

#include <vector>
#include "IServiceManager.h"

class ServiceManager : public IServiceManager
{

public:

	virtual ~ServiceManager( ) { };

	ServiceManager( ) { };

	inline void RegisterService( IService* service ) { _services.push_back( service ); };

	IService* FindService( SystemType systemType ) const;


private:

	IService::ServiceList _services;

};

#endif