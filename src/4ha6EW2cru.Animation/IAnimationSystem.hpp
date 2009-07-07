/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   IAnimationSystem.hpp
*  @date   2009/04/25
*/
#ifndef IANIMATIONSYSTEM_HPP
#define IANIMATIONSYSTEM_HPP

#include "System/ISystem.hpp"

namespace Animation
{
	/*! 
	 *  The Animation System
	 */
	class IAnimationSystem : public ISystem
	{

	public:

		/*! Default Destructor
		 *
		 *  @return ()
		 */
		virtual ~IAnimationSystem( ) { };
		
	};
};

#endif