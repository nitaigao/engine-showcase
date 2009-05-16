/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   IRendererSystemComponent.hpp
*  @date   2009/04/27
*/
#ifndef IRENDERERSYSTEMCOMPONENT_HPP
#define IRENDERERSYSTEMCOMPONENT_HPP

#include "../System/ISystemComponent.hpp"

namespace Renderer
{
	/*!
	 *  A Render System Component 
	 */
	class IRendererSystemComponent : public ISystemComponent
	{

	public:

		/*! Default Destructor
		 *
		 *  @return ()
		 */
		virtual ~IRendererSystemComponent( ) { };


		/*! Plays the specified animation
		 *
		 *  @param[in] const std::string & animationName
		 *  @param[in] const bool & loopAnimation
		 *  @return (void)
		 */
		virtual void PlayAnimation( const std::string& animationName, const bool& loopAnimation ) = 0;

	};
};

#endif