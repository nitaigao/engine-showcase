/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   IFPSMouseInputComponent.hpp
*  @date   2009/05/09
*/
#ifndef __IFPSMOUSEINPUTCOMPONENT_HPP
#define __IFPSMOUSEINPUTCOMPONENT_HPP

#include "../System/ISystemComponent.hpp"

namespace Input
{
	/*!
	 *  A First Person Input Mouse Component 
	 */
	class IMouseInputComponent : public ISystemComponent
	{

	public:

		/*! Default Destructor
		 *
		 *  @return ()
		 */
		virtual ~IMouseInputComponent( ) { };

		

	};
};

#endif