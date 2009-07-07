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
		
	};
};

#endif