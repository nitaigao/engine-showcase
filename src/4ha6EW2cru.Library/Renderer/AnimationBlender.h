/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   src\4ha6EW2cru.Library\Renderer\AnimationBlender.h
*  @date   2009/04/25
*/
#ifndef __ANIMATIONBLENDER_H
#define __ANIMATIONBLENDER_H

#include "IAnimationBlender.hpp"

#include <Ogre.h>

/*! 
 *  Blends two AnimationStates to give a smooth transition
 */
class AnimationBlender : public IAnimationBlender
{

public:

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
	 *  @param[in] float deltaMilliseconds
	 *  @return (void)
	 */
	void Update( float deltaMilliseconds );


	/*! Blends the current AnimationState to the specified animation
	 *
	 *  @param[in] const std::string animationName
	 *  @param[in] float blendDuration
	 *  @param[in] bool loopAnimation
	 *  @return (void)
	 */
	void Blend( const std::string animationName, float blendDuration, bool loopAnimation );

private:

	Ogre::Entity* _entity;
	Ogre::AnimationState* _sourceAnimationState;
	Ogre::AnimationState* _targetAnimationState;

	float _timeLeft;
	float _blendDuration;

};

#endif