/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   INetworkProvider.hpp
*  @date   2009/07/15
*/
#pragma once
#ifndef INETWORKPROVIDER_HPP
#define INETWORKPROVIDER_HPP

#include "Configuration/IConfiguration.hpp"

namespace Network
{
	/*! 
	 *  A communication provider to the Network
	 */
	class INetworkProvider
	{

	public:

		/*! Default Destructor
		 *
		 *  @return ()
		 */
		virtual ~INetworkProvider( ) { };

		virtual void Initialize( Configuration::IConfiguration* configuration ) = 0;

		virtual void Update( const float& deltaMilliseconds ) = 0;

		virtual void Release( ) = 0;

		virtual void Message( const std::string& message, AnyType::AnyTypeMap parameters ) = 0;

		virtual void PushMessage( const std::string& componentId, const std::string& message, AnyType::AnyTypeMap parameters ) = 0;
		
	};
};

#endif