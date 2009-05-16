/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   ISoundScene.hpp
*  @date   2009/04/27
*/
#ifndef ISOUNDSCENE_HPP
#define ISOUNDSCENE_HPP

#include "ISoundSystem.hpp"
#include "../System/ISystemScene.hpp"

namespace Sound
{
	/*!
	 *  A Sound Scene 
	 */
	class ISoundScene : public ISystemScene
	{

	public:

		/*! Default Destructor
		 *
		 *  @return ()
		 */
		virtual ~ISoundScene( ) { };

		virtual ISoundSystem* GetSoundSystem( ) = 0;

		

	};
};

#endif