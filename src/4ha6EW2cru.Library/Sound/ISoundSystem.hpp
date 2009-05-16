/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   ISoundSystem.hpp
*  @date   2009/04/27
*/
#ifndef ISOUNDSYSTEM_HPP
#define ISOUNDSYSTEM_HPP

#include "../System/ISystem.hpp"

#include <fmod.hpp>
#include <fmod_event.hpp>

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

		/*! Returns the FMOD Event System
		*
		*  @return (FMOD::EventSystem*)
		*/
		virtual FMOD::EventSystem* GetEventSystem( ) const = 0;

	};
};

#endif