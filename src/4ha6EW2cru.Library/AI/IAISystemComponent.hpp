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


		/*! Returns the Script Component for the AI Component
		 *
		 *  @return (IScriptComponent*)
		 */
		virtual Script::IScriptComponent* GetScriptComponent( ) = 0;

	};
};

#endif