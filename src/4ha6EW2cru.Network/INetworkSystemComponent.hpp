/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   INetworkSystemComponent.hpp
*  @date   2009/07/06
*/
#pragma once
#ifndef INETWORKSYSTEMCOMPONENT_HPP
#define INETWORKSYSTEMCOMPONENT_HPP

#include "System/ISystemComponent.hpp"

namespace Network
{
	/*! 
	 *  A Network System Component
	 */
	class INetworkSystemComponent : public ISystemComponent
	{

	public:

		/*! Default Destructor
		 *
		 *  @return ()
		 */
		virtual ~INetworkSystemComponent( ) { };


		/*! Receives Messages from an inbound network connection
		*
		* @param[in] const std::string & message
		* @param[in] AnyType::AnyTypeMap parameters
		* @return ( void )
		*/
		virtual void MessageFromNetwork( const std::string& message, AnyType::AnyTypeMap parameters ) = 0;
		
	};
};

#endif