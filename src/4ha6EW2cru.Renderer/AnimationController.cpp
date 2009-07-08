#include "AnimationController.h"

namespace Renderer
{
	void AnimationController::SetWeight( const float& weight )
	{
		for( AnimationStateList::iterator i = m_animationStates.begin( ); i != m_animationStates.end( ); ++i )
		{
			( *i )->setWeight( weight );
		}
	}

	void AnimationController::SetLoop( const bool& loop )
	{
		for( AnimationStateList::iterator i = m_animationStates.begin( ); i != m_animationStates.end( ); ++i )
		{
			( *i )->setLoop( loop );
		}
	}

	void AnimationController::SetEnabled( const bool& enabled )
	{
		for( AnimationStateList::iterator i = m_animationStates.begin( ); i != m_animationStates.end( ); ++i )
		{
			( *i )->setEnabled( enabled );
		}
	}

	void AnimationController::Update( const float& deltaMilliseconds )
	{
		for( AnimationStateList::iterator i = m_animationStates.begin( ); i != m_animationStates.end( ); ++i )
		{
			( *i )->addTime( deltaMilliseconds );
		}
	}

	void AnimationController::SetTime( const float& time )
	{
		for( AnimationStateList::iterator i = m_animationStates.begin( ); i != m_animationStates.end( ); ++i )
		{
			( *i )->setTimePosition( time );
		}
	}
}