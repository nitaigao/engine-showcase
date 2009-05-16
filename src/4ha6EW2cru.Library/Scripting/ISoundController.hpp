/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   ISFXController.hpp
*  @date   2009/04/28
*/
#ifndef ISOUNDCONTROLLER_HPP
#define ISOUNDCONTROLLER_HPP

#include <string>

#include "IScriptController.hpp"

namespace Script
{
	/*!
	 *  An Interface to the Sound System for Script 
	 */
	class ISoundController : public IScriptController
	{

	public:

		/*! Default Destructor
		 *
		 *  @return ()
		 */
		virtual ~ISoundController( ) { };

		
		/*! Instructs the Sound System to play some music from the given event path
		 *
		 *  @param[in] const std::string & eventPath
		 *  @return (void)
		 */
		virtual void PlayMusic( const std::string& eventPath ) = 0;

	};
};

#endif