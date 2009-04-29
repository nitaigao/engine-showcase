/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   ISFXController.hpp
*  @date   2009/04/28
*/
#ifndef __ISOUNDCONTROLLER_HPP
#define __ISOUNDCONTROLLER_HPP

#include <string>

namespace Script
{
	/*!
	 *  An Interface to the Sound System for Script 
	 */
	class ISoundController
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