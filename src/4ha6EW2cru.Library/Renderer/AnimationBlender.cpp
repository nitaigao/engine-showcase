#include "AnimationBlender.h"

#include <Ogre.h>
using namespace Ogre;

namespace Renderer
{
	void AnimationBlender::Blend( const std::string& animationName, const float& blendDuration, const bool& loopAnimation )
	{
		AnimationState* newTargetAnimationState = _entity->getAnimationState( animationName );

		if ( newTargetAnimationState == _sourceAnimationState )
		{
			if ( _sourceAnimationState->hasEnded( ) )
			{
				_sourceAnimationState->setTimePosition( 0 );
			}
		}
		else if ( newTargetAnimationState == _targetAnimationState )
		{

		}
		else
		{
			if( _sourceAnimationState == 0 )
			{
				_sourceAnimationState = newTargetAnimationState;
				_sourceAnimationState->setEnabled( true );
				_sourceAnimationState->setLoop( false );
				_sourceAnimationState->setTimePosition( 0 );
				_sourceAnimationState->setWeight( 1 );
			}
			else
			{
				if ( _targetAnimationState != 0 )
				{
					_targetAnimationState->setTimePosition( 0 );
					_targetAnimationState->setEnabled( false );
				}

				_timeLeft = _blendDuration = blendDuration;
				_targetAnimationState = newTargetAnimationState;
				_targetAnimationState->setEnabled( true );
				_targetAnimationState->setLoop( false );
				_targetAnimationState->setTimePosition( 0 );
				_targetAnimationState->setWeight( 1.0 - _timeLeft / _blendDuration );
			}
		}
	}

	void AnimationBlender::Update( const float& deltaMilliseconds )
	{
		if( _sourceAnimationState != 0 )
		{
			if ( _timeLeft > 0 )
			{
				_timeLeft -= deltaMilliseconds;

				if ( _timeLeft <= 0 )
				{
					_sourceAnimationState->setTimePosition( 0 );
					_sourceAnimationState->setEnabled( false );
					_sourceAnimationState = _targetAnimationState;
					_sourceAnimationState->setWeight( 1 );
					_targetAnimationState = 0;
				}
				else
				{
					_sourceAnimationState->setWeight( _timeLeft / _blendDuration );

					_targetAnimationState->setWeight( 1.0 - _timeLeft / _blendDuration );
					_targetAnimationState->addTime( deltaMilliseconds );
				}
			}

			_sourceAnimationState->addTime( deltaMilliseconds );
		}
	}
}