/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   INetworkFacade.h
*  @date   2009/07/06
*/
#pragma once
#ifndef INETWORKFACADE_H
#define INETWORKFACADE_H

#include "IScriptFacade.hpp"

namespace Script
{
	/*! 
	 *  A Facade Interface to interface with the Network System
	 */
	class INetworkFacade : public IScriptFacade
	{

	public:

		/*! Default Destructor
		 *
		 *  @return ()
		 */
		virtual ~INetworkFacade( ) { };
		
	};
};

#endif