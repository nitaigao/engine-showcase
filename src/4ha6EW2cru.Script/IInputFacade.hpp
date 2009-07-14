/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   IInputFacade.hpp
*  @date   2009/07/14
*/
#pragma once
#ifndef IINPUTFACADE_HPP
#define IINPUTFACADE_HPP

#include "IScriptFacade.hpp"

namespace Script
{
	/*! 
	 *  The Facade interface to communicate with the Input System
	 */
	class IInputFacade : public IScriptFacade
	{

	public:

		/*! Default Destructor
		 *
		 *  @return ()
		 */
		virtual ~IInputFacade( ) { };
		
	};
};

#endif