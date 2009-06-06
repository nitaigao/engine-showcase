#include "AnimationNode.h"

#include "../Logging/Logger.h"
using namespace Logging;

namespace Animation
{
	AnimationNode::~AnimationNode()
	{
		for( IAnimationNode::AnimationNodeList::iterator i = m_children.begin( ); i != m_children.end( ); ++i )
		{
			delete ( *i );
		}
	}

	void AnimationNode::AddNode( IAnimationNode* node )
	{
		for( IAnimationNode::AnimationNodeList::iterator i = m_children.begin( ); i != m_children.end( ); ++i )
		{
			if ( ( *i )->GetAnimationName( ).compare( node->GetAnimationName( ) ) == 0 )
			{
				delete node;
				return;
			}
		}

		m_children.empty( );

		m_children.push_front( node );

		this->SetWeight( m_targetWeight );
	}

	void AnimationNode::SetWeight( const float& weight )
	{
		m_targetWeight = weight;
		m_timeLeft = m_blendDuration;

		float index = 0.0f;

		for( IAnimationNode::AnimationNodeList::iterator i = m_children.begin( ); i != m_children.end( ); ++i )
		{
			float childWeight = weight / ( ++index * index * index ); 
			( *i )->SetWeight( childWeight );
		}
	}

	IAnimationNode* AnimationNode::RemoveNode( const std::string& animationName )
	{
		if ( m_animationController )
		{
			if ( m_animationController->GetName( ) == animationName )
			{
				return this;
			}
		}

		IAnimationNode* node = 0;

		for( IAnimationNode::AnimationNodeList::iterator i = m_children.begin( ); i != m_children.end( ); ++i )
		{
			node = ( *i )->RemoveNode( animationName );

			if ( node )
			{
				m_children.erase( i );
				this->SetWeight( m_targetWeight );
				break;
			}
		}

		return node;
	}

	void AnimationNode::Update( const float& deltaMilliseconds )
	{
		if ( m_animationController != 0 )
		{
			m_animationController->Update( deltaMilliseconds );

			if ( m_timeLeft > 0.0f )
			{
				m_timeLeft -= deltaMilliseconds;

				if ( m_timeLeft <= 0.0f )
				{
					m_animationController->SetWeight( m_targetWeight );
					m_timeLeft = 0.0f;
				}
				else
				{
					float percentTime = ( m_timeLeft / m_blendDuration ) * 100.0f;
					float newWeight = ( m_targetWeight * percentTime ) / 100.0f;
					m_animationController->SetWeight( newWeight );
				}
			}
		}

		for( IAnimationNode::AnimationNodeList::iterator i = m_children.begin( ); i != m_children.end( ); ++i )
		{
			( *i )->Update( deltaMilliseconds );
		}
	}
}