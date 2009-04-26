/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   src\4ha6EW2cru.Library\System\IServiceManager.h
*  @date   2009/04/25
*/
#ifndef __ISERVICEMANAGER_H
#define __ISERVICEMANAGER_H

#include "IService.hpp"

/*! 
 *  Manages Registered System Services
 */
class IServiceManager
{

public:

	/*! Unregisters all Services
	 *
	 *  @return ()
	 */
	virtual ~IServiceManager( ) { };


	/*! Registers a Service
	 *
	 *  @param[in] IService * service
	 *  @return (void)
	 */
	virtual void RegisterService( IService* service ) = 0;


	/*! Finds a Registered Service by System::Types::Type
	 *
	 *  @param[in] System::Types::Type systemType
	 *  @return (IService*)
	 */
	virtual IService* FindService( System::Types::Type systemType ) const = 0;

};

#endif