/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   IInputSystemComponent.hpp
*  @date   2009/05/15
*/
#ifndef __IINPUTSYSTEMCOMPONENT_HPP
#define __IINPUTSYSTEMCOMPONENT_HPP

#include "System/ISystemComponent.hpp"

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

		typedef std::deque< IInputSystemComponent* > InputSystemComponentList;

		/*! Default Destructor
		 *
		 *  @return ()
		 */
		virtual ~IInputSystemComponent( ) { };


		/*! Called by the Scene when the user presses a mouse button
		*
		* @param[in] const OIS::MouseEvent & arg
		* @param[in] OIS::MouseButtonID id
		* @return ( void )
		*/
		virtual void MousePressed( const OIS::MouseEvent& arg, OIS::MouseButtonID id ) = 0;


		/*! Called by the Scene when a mouse button has been released
		 *
		 *  @param[in] const OIS::MouseEvent & arg
		 *  @param[in] OIS::MouseButtonID id
		 *  @return (void)
		 */
		virtual void MouseReleased( const OIS::MouseEvent& arg, OIS::MouseButtonID id ) = 0;


		/*! Called the the Scene when the user moves the mouse
		*
		* @param[in] const OIS::MouseEvent & arg
		* @return ( void )
		*/
		virtual void MouseMoved( const OIS::MouseEvent &arg ) = 0;


		/*! Called by the Scene when a key is released
		*
		* @param[in] const KeyEvent & arg
		* @return ( bool )
		*/
		virtual void KeyReleased( const OIS::KeyEvent &arg ) = 0;


		/*! Called by the Scene when a key is pressed
		*
		* @param[in] const OIS::KeyEvent & arg
		* @return ( void )
		*/
		virtual void KeyPressed( const OIS::KeyEvent &arg ) = 0;

	};
};

#endif