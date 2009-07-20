/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   IInputSystem.hpp
*  @date   2009/04/26
*/
#ifndef IINPUTSYSTEM_H
#define IINPUTSYSTEM_H

#include "System/ISystem.hpp"

#include "InputMessageBinding.hpp"

#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>

namespace Input
{
	/*! 
	 *  Interface to the Input System
	 */
	class IInputSystem : public ISystem
	{

	public:

		/*! Default Destructor
		*
		* @return (  )
		*/
		virtual ~IInputSystem( ) { };

		
		/*! Returns the Bindings setup in the config file
		*
		* @return ( AnyType::AnyTypeMap )
		*/
		virtual InputMessageBinding::InputMessageBindingList GetBindings( ) = 0;


		/*! Returns the Keyboard controller
		*
		* @return ( OIS::Keyboard* )
		*/
		virtual OIS::Keyboard* GetKeyboard( ) = 0;


		/*! Returns the Mouse controller
		*
		* @return ( OIS::Mouse* )
		*/
		virtual OIS::Mouse* GetMouse( ) = 0;

	};
};

#endif
