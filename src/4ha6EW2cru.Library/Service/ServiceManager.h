/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   ServiceManager.h
*  @date   2009/04/25
*/
#pragma once
#ifndef SERVICEMANAGER_H
#define SERVICEMANAGER_H

#include <vector>
#include "IServiceManager.h"

namespace Services
{
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
		~ServiceManager( ) { };


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
		inline void RegisterService( IService* service ) { m_services.push_back( service ); };


		/*! Messages all Registered Services
		*
		* @param[in] const std::string & message
		* @param[in] AnyType::AnyTypeMap parameters
		* @return ( void )
		*/
		void MessageAll( const std::string& message, AnyType::AnyTypeMap parameters );


		/*! Finds a Registered Service by System::Types::Type
		*
		*  @param[in] System::Types::Type systemType
		*  @return (IService*)
		*/
		IService* FindService( System::Types::Type systemType ) const;


	private:

		IService::ServiceList m_services;

	};
};

#endif