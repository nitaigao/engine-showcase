/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   ServiceManager.h
*  @date   2009/04/25
*/
#ifndef __SERVICEMANAGER_H
#define __SERVICEMANAGER_H

#include <vector>
#include "IServiceManager.h"

/*! 
*  Manages Registered System Services
*/
class ServiceManager : public IServiceManager
{

public:

	/*! Unregisters all Services
	*
	*  @return ()
	*/
	virtual ~ServiceManager( ) { };


	/*! Default Constructor
	 *
	 *  @return ()
	 */
	ServiceManager( ) { };

	/*! Registers a Service
	*
	*  @param[in] IService * service
	*  @return (void)
	*/
	inline void RegisterService( IService* service ) { _services.push_back( service ); };

	/*! Finds a Registered Service by System::Types::Type
	*
	*  @param[in] System::Types::Type systemType
	*  @return (IService*)
	*/
	IService* FindService( System::Types::Type systemType ) const;


private:

	IService::ServiceList _services;

};

#endif