/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   IAISystemComponent.hpp
*  @date   2009/04/27
*/
#ifndef IAISYSTEMCOMPONENT_HPP
#define IAISYSTEMCOMPONENT_HPP

#include "System/ISystemComponent.hpp"

namespace AI
{
	/*! 
	 *  An AI System Component
	 */
	class IAISystemComponent : public ISystemComponent
	{

	public:

		typedef std::deque< IAISystemComponent* > AISystemComponentList;

		/*! Default Destructor
		 *
		 *  @return ()
		 */
		virtual ~IAISystemComponent( ) { };

	};
};

#endif