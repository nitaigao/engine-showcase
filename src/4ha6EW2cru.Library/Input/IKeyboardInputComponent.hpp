/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   IKeyboardInputComponent.hpp
*  @date   2009/05/09
*/
#ifndef __IKEYBOARDINPUTCOMPONENT_HPP
#define __IKEYBOARDINPUTCOMPONENT_HPP

#include "../System/ISystemComponent.hpp"

namespace Input
{
	/*!
	 *  A Keyboard Input Component 
	 */
	class IKeyboardInputComponent : public ISystemComponent
	{

	public:

		/*! Default Destructor
		 *
		 *  @return ()
		 */
		virtual ~IKeyboardInputComponent( ) { };



	};
};

#endif