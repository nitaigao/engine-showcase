/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   IUXSystemScene.hpp
*  @date   2009/04/26
*/
#ifndef __IUXSYSTEMSCENE_H
#define __IUXSYSTEMSCENE_H

#include "../System/ISystemScene.hpp"
#include <MyGUI.h>

namespace UX
{
	/*! 
	 *  A scene specific to the UX System
	 */
	class IUXSystemScene : public ISystemScene
	{

	public:

		/*! Default Destructor
		 *
		 *  @return ()
		 */
		virtual ~IUXSystemScene( ) { };


		/*! Returns the GUI System
		 *
		 *  @return (MyGUI::Gui*)
		 */
		virtual MyGUI::Gui* GetGui( ) const = 0;

	};
};

#endif