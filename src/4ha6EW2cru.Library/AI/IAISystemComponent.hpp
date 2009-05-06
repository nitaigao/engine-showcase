/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   IAISystemComponent.hpp
*  @date   2009/04/27
*/
#ifndef __IAISYSTEMCOMPONENT_HPP
#define __IAISYSTEMCOMPONENT_HPP

#include "../System/ISystemComponent.hpp"
#include "../Scripting/IScriptComponent.hpp"

namespace AI
{
	/*! 
	 *  An AI System Component
	 */
	class IAISystemComponent : public ISystemComponent
	{

	public:

		/*! Default Destructor
		 *
		 *  @return ()
		 */
		virtual ~IAISystemComponent( ) { };


		/*! Returns the frame number that this ai will perform its logic update
		 *
		 *  @return (int)
		 */
		virtual int GetFrameAssignment( ) const = 0;

	};
};

#endif