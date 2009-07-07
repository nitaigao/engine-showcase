/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   IAnimationSystemScene.hpp
*  @date   2009/04/25
*/
#ifndef IANIMATIONSYSTEMSCENE_HPP
#define IANIMATIONSYSTEMSCENE_HPP

#include "System/ISystemScene.hpp"

namespace Animation
{
	/*! 
	 *  The Animation System Scene
	 */
	class IAnimationSystemScene : public ISystemScene
	{

	public:

		/*! Default Destructor
		 *
		 *  @return ()
		 */
		virtual ~IAnimationSystemScene( ) { };
		
	};
};

#endif