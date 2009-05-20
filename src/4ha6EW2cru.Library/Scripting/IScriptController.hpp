/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   IScriptController.hpp
*  @date   2009/05/15
*/
#ifndef __ISCRIPTCONTROLLER_HPP
#define __ISCRIPTCONTROLLER_HPP

#include <deque>

namespace Script
{
	/*!
	 *  A Controller For Interfacing with different components from a Script 
	 */
	class IScriptController
	{

	public:

		typedef std::deque< IScriptController* > ScriptControllerList;

		/*! Default Destructor
		 *
		 *  @return ()
		 */
		virtual ~IScriptController( ) { };

		

	};
};

#endif