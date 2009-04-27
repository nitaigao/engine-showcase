/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   IAISystemScene.hpp
*  @date   2009/04/27
*/

#ifndef __IAISYSTEMSCENE_H
#define __IAISYSTEMSCENE_H

#include "../System/ISystemScene.hpp"

namespace AI
{
	/*! 
	 *  An AI System Scene
	 */
	class IAISystemScene : public ISystemScene
	{

	public:

		/*! Default Destructor
		 *
		 *  @return ()
		 */
		virtual ~IAISystemScene( ) { };

	};
};

#endif