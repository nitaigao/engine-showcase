/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   ISoundSystem.hpp
*  @date   2009/04/27
*/
#ifndef __ISOUNDSYSTEM_HPP
#define __ISOUNDSYSTEM_HPP

#include "../System/ISystem.hpp"

namespace Sound
{
	/*!
	 *  The Sound System 
	 */
	class ISoundSystem : public ISystem
	{

	public:

		/*! Default Destructor
		 *
		 *  @return ()
		 */
		virtual ~ISoundSystem( ) { };

	};
};

#endif