/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   AnimationBlender.h
*  @date   2009/04/25
*/
#ifndef __ANIMATIONBLENDER_H
#define __ANIMATIONBLENDER_H

#include "IAnimationBlender.hpp"

#include <Ogre.h>

namespace Renderer
{
	/*! 
	 *  Blends two AnimationStates to give a smooth transition
	 */
	class AnimationBlender : public IAnimationBlender
	{

	public:

		/*! Default Destructor
		 *
		 *  @return ()
		 */
		~AnimationBlender( ) { };

		/*! Default Constructor
		 *
		 *  @param[in] Ogre::Entity * entity
		 *  @return ()
		 */
		AnimationBlender( Ogre::Entity* entity )
			: _entity( entity )
			, _sourceAnimationState( 0 )
			, _targetAnimationState( 0 )
			, _timeLeft( 0.0f )
			, _blendDuration( 0.0f )
		{

		}

		/*! Steps each AnimationState inside the Blender
		 *
		 *  @param[in] const float & deltaMilliseconds
		 *  @return (void)
		 */
		void Update( const float& deltaMilliseconds );


		/*! Blends the current AnimationState to the specified animation
		 *
		 *  @param[in] const std::string & animationName
		 *  @param[in] const float & blendDuration
		 *  @param[in] const bool & loopAnimation
		 *  @return (void)
		 */
		void Blend( const std::string& animationName, const float& blendDuration, const bool& loopAnimation );

	private:

		AnimationBlender( const AnimationBlender & copy ) { };
		AnimationBlender & operator = ( const AnimationBlender & copy ) { return *this; };

		Ogre::Entity* _entity;
		Ogre::AnimationState* _sourceAnimationState;
		Ogre::AnimationState* _targetAnimationState;

		float _timeLeft;
		float _blendDuration;

	};

	#endif
};