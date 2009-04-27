/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   IScriptSystem.hpp
*  @date   2009/04/27
*/
#ifndef __ISCRIPTSYSTEM_HPP
#define __ISCRIPTSYSTEM_HPP

#include "../System/ISystem.hpp"

namespace Script
{
	/*!
	 *  The Scripting System 
	 */
	class IScriptSystem : public ISystem
	{

	public:

		/*! Default Destructor
		 *
		 *  @return ()
		 */
		virtual ~IScriptSystem( ) { };

	};
};

#endif