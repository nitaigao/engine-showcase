#include "AnimationBlender.h"

#include <Ogre.h>
using namespace Ogre;

void AnimationBlender::Blend( const std::string animationName, float blendDuration )
{
	AnimationState* newTargetAnimationState = _entity->getAnimationState( animationName );

	if( _timeLeft > 0 )
	{
		if ( newTargetAnimationState == _targetAnimationState )
		{

		}
		else if ( newTargetAnimationState == _sourceAnimationState )
		{
			_sourceAnimationState = _targetAnimationState;
			_targetAnimationState = newTargetAnimationState;
			_timeLeft = _blendDuration - _timeLeft;
		}
		else
		{
			if ( _timeLeft < _blendDuration * 0.5 )
			{
				_targetAnimationState->setEnabled( true );
				_targetAnimationState->setWeight( 0 );
			}
			else
			{
				_sourceAnimationState->setEnabled( false );
				_sourceAnimationState->setWeight( 0 );
				_sourceAnimationState = _targetAnimationState;
			}

			_targetAnimationState = newTargetAnimationState;
			_targetAnimationState->setEnabled( true );
			_targetAnimationState->setWeight( 1.0 - _timeLeft / _blendDuration );
			_targetAnimationState->setTimePosition( 0 );
		}
	}
	else
	{
		if( _sourceAnimationState == 0 )
		{
			_sourceAnimationState = newTargetAnimationState;
		}

		_timeLeft = _blendDuration = blendDuration;
		_targetAnimationState = newTargetAnimationState;
		_targetAnimationState->setEnabled( true );
		_targetAnimationState->setWeight( 0 );
		_targetAnimationState->setTimePosition( 0 );
	}
}

void AnimationBlender::Update( float deltaMilliseconds )
{
	if ( _sourceAnimationState != 0 )
	{
		if ( _timeLeft > 0 )
		{
			_timeLeft -= deltaMilliseconds;

			if ( _timeLeft < 0 )
			{
				_sourceAnimationState->setEnabled( false );
				_sourceAnimationState->setWeight( 0 );
				_sourceAnimationState = _targetAnimationState;
				_sourceAnimationState->setEnabled( true );
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

		if ( _sourceAnimationState->getTimePosition( ) >= _sourceAnimationState->getLength( ) )
		{
			_blendComplete = true;
		}
		else
		{
			_blendComplete = false;
		}

		_sourceAnimationState->addTime( deltaMilliseconds );
		_sourceAnimationState->setLoop( true );
	}
}