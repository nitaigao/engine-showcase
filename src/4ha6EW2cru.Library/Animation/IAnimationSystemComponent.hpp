/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   IAnimationSystemComponent.hpp
*  @date   2009/04/25
*/
#ifndef IANIMATIONSYSTEMCOMPONENT_HPP
#define IANIMATIONSYSTEMCOMPONENT_HPP

#include "../System/ISystemComponent.hpp"

namespace Animation
{
	/*! 
	 *  An Animation System Component
	 */
	class IAnimationSystemComponent : public ISystemComponent
	{

	public:

		/*! Default Destructor
		 *
		 *  @return ()
		 */
		virtual ~IAnimationSystemComponent( ) { };
		
	};
};

#endif