/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   SoundController.h
*  @date   2009/04/28
*/
#ifndef __SOUNDCONTROLLER_H
#define __SOUNDCONTROLLER_H

#include "ISoundController.hpp"

namespace Script
{
	/*!
	 *  An Interface to the Sound System for Script  
	 */
	class SoundController : public ISoundController
	{

	public:

		/*! Default Destructor
		 *
		 *  @return ()
		 */
		~SoundController( ) { };


		/*! Default Constructor
		 *
		 *  @return ()
		 */
		SoundController( )
		{

		}


		/*! Instructs the Sound System to play some music from the given event path
		*
		*  @param[in] const std::string & eventPath
		*  @return (void)
		*/
		void PlayMusic( const std::string& eventPath );

	private:

		SoundController( const SoundController & copy ) { };
		SoundController & operator = ( const SoundController & copy ) { return *this; };

	};
};

#endif