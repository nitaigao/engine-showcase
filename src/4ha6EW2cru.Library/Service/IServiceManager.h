/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   IServiceManager.h
*  @date   2009/04/25
*/
#pragma once
#ifndef ISERVICEMANAGER_H
#define ISERVICEMANAGER_H

#include "IService.hpp"

namespace Services
{
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


		/*! Messages all Registered Services
		*
		* @param[in] const std::string & message
		* @param[in] AnyType::AnyTypeMap parameters
		* @return ( void )
		*/
		virtual void MessageAll( const std::string& message, AnyType::AnyTypeMap parameters ) = 0;


		/*! Finds a Registered Service by System::Types::Type
		 *
		 *  @param[in] System::Types::Type systemType
		 *  @return (IService*)
		 */
		virtual IService* FindService( System::Types::Type systemType ) const = 0;

	};
};

#endif