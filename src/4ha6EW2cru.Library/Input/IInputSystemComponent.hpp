/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   IInputSystemComponent.hpp
*  @date   2009/05/15
*/
#ifndef __IINPUTSYSTEMCOMPONENT_HPP
#define __IINPUTSYSTEMCOMPONENT_HPP

#include "../System/ISystemComponent.hpp"

#include <OIS/OISKeyboard.h>
#include <OIS/OISMouse.h>

namespace Input
{
	/*!
	 *  An Input System Component 
	 */
	class IInputSystemComponent : public ISystemComponent
	{

	public:

		/*! Default Destructor
		 *
		 *  @return ()
		 */
		virtual ~IInputSystemComponent( ) { };


		/*! Called by the Scene when a mouse button has been released
		 *
		 *  @param[in] const OIS::MouseEvent & arg
		 *  @param[in] OIS::MouseButtonID id
		 *  @return (void)
		 */
		virtual void MouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id ) = 0;

	};
};

#endif