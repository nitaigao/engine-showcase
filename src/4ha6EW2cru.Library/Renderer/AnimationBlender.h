/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   AnimationBlender.h
*  @date   2009/04/25
*/
#ifndef ANIMATIONBLENDER_H
#define ANIMATIONBLENDER_H

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
			: m_entity( entity )
			, m_sourceAnimationState( 0 )
			, m_targetAnimationState( 0 )
			, m_timeLeft( 0.0f )
			, m_blendDuration( 0.0f )
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

		Ogre::Entity* m_entity;
		Ogre::AnimationState* m_sourceAnimationState;
		Ogre::AnimationState* m_targetAnimationState;

		float m_timeLeft;
		float m_blendDuration;

	};

	#endif
};