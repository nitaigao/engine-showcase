/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   IScriptFacade.hpp
*  @date   2009/05/15
*/
#ifndef ISCRIPTFACADE_HPP
#define ISCRIPTFACADE_HPP

#include <deque>

namespace Script
{
	/*!
	 *  A Controller For Interfacing with different components from a Script 
	 */
	class IScriptFacade
	{

	public:

		typedef std::deque< IScriptFacade* > ScriptControllerList;

		/*! Default Destructor
		 *
		 *  @return ()
		 */
		virtual ~IScriptFacade( ) { };

		

	};
};

#endif