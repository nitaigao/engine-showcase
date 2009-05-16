#include "AnimationBlender.h"

#include <Ogre.h>
using namespace Ogre;

namespace Renderer
{
	void AnimationBlender::Blend( const std::string& animationName, const float& blendDuration, const bool& loopAnimation )
	{
		AnimationState* newTargetAnimationState = m_entity->getAnimationState( animationName );

		if ( newTargetAnimationState == m_sourceAnimationState )
		{
			if ( m_sourceAnimationState->hasEnded( ) )
			{
				m_sourceAnimationState->setTimePosition( 0 );
			}
		}
		else if ( newTargetAnimationState == m_targetAnimationState )
		{

		}
		else
		{
			if( m_sourceAnimationState == 0 )
			{
				m_sourceAnimationState = newTargetAnimationState;
				m_sourceAnimationState->setEnabled( true );
				m_sourceAnimationState->setLoop( loopAnimation );
				m_sourceAnimationState->setTimePosition( 0 );
				m_sourceAnimationState->setWeight( 1 );
			}
			else
			{
				if ( m_targetAnimationState != 0 )
				{
					m_targetAnimationState->setTimePosition( 0 );
					m_targetAnimationState->setEnabled( false );
				}

				m_timeLeft = m_blendDuration = blendDuration;
				m_targetAnimationState = newTargetAnimationState;
				m_targetAnimationState->setEnabled( true );
				m_targetAnimationState->setLoop( loopAnimation );
				m_targetAnimationState->setTimePosition( 0 );
				m_targetAnimationState->setWeight( 1.0 - m_timeLeft / m_blendDuration );
			}
		}
	}

	void AnimationBlender::Update( const float& deltaMilliseconds )
	{
		if( m_sourceAnimationState != 0 )
		{
			if ( m_timeLeft > 0 )
			{
				m_timeLeft -= deltaMilliseconds;

				if ( m_timeLeft <= 0 )
				{
					m_sourceAnimationState->setTimePosition( 0 );
					m_sourceAnimationState->setEnabled( false );
					m_sourceAnimationState->setLoop( false );
					m_sourceAnimationState = m_targetAnimationState;
					m_sourceAnimationState->setWeight( 1 );
					m_targetAnimationState = 0;
				}
				else
				{
					m_sourceAnimationState->setWeight( m_timeLeft / m_blendDuration );

					m_targetAnimationState->setWeight( 1.0 - m_timeLeft / m_blendDuration );
					m_targetAnimationState->addTime( deltaMilliseconds );
				}
			}

			m_sourceAnimationState->addTime( deltaMilliseconds );
		}
	}
}