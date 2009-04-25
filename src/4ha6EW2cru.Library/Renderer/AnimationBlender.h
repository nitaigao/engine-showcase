#ifndef __ANIMATIONBLENDER_H
#define __ANIMATIONBLENDER_H

#include "IAnimationBlender.hpp"

#include <Ogre.h>

class AnimationBlender : public IAnimationBlender
{

public:

	AnimationBlender( Ogre::Entity* entity )
		: _entity( entity )
		, _sourceAnimationState( 0 )
		, _targetAnimationState( 0 )
		, _timeLeft( 0.0f )
		, _blendDuration( 0.0f )
	{

	}

	void Update( float deltaMilliseconds );

	void Blend( const std::string animationName, float blendDuration, bool loopAnimation );

private:

	Ogre::Entity* _entity;
	Ogre::AnimationState* _sourceAnimationState;
	Ogre::AnimationState* _targetAnimationState;

	float _timeLeft;
	float _blendDuration;

};

#endif