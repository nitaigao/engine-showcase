/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   IAnimationBlender.hpp
*  @date   2009/04/26
*/
#ifndef __IANIMATIONBLENDER_H
#define __IANIMATIONBLENDER_H

#include <string>

namespace Renderer
{
	/*! 
	*  Blends two AnimationStates to give a smooth transition
	*/
	class IAnimationBlender
	{

	public:

		/*! Default Destructor
		 *
		 *  @return ()
		 */
		virtual ~IAnimationBlender( ) { };


		/*! Steps each AnimationState inside the Blender
		*
		*  @param[in] const float & deltaMilliseconds
		*  @return (void)
		*/
		virtual void Update( const float& deltaMilliseconds ) = 0;


		/*! Blends the current AnimationState to the specified animation
		 *
		 *  @param[in] const std::string & animationName
		 *  @param[in] const float & blendDuration
		 *  @param[in] const bool & loopAnimation
		 *  @return (void)
		 */
		virtual void Blend( const std::string& animationName, const float& blendDuration, const bool& loopAnimation ) = 0;

	};
};


#endif