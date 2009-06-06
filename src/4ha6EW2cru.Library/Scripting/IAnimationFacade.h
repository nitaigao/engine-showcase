/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   IAnimationFacade.h
*  @date   2009/06/03
*/
#ifndef IANIMATIONFACADE_H
#define IANIMATIONFACADE_H

#include "IScriptFacade.hpp"

namespace Script
{
	/*! 
	 *  A Facade interface to the Animation System
	 */
	class IAnimationFacade : public IScriptFacade
	{

	public:

		/*! Default Destructor
		 *
		 *  @return ()
		 */
		virtual ~IAnimationFacade( ) { };
		
	};
};

#endif