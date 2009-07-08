#include "AnimationBlender.h"

#include <sstream>

#include "Logging/Logger.h"
using namespace Logging;

namespace Animation
{
	void AnimationBlender::Blend( const std::string& animationName )
	{
		if ( m_animationControllers.find( animationName ) == m_animationControllers.end( ) )
		{
			std::stringstream logMessage;
			logMessage << "AnimationBlender::Blend: " << animationName << " doesn't exist";
			Logger::Get( )->Warn( logMessage.str( ) );
		}
		else
		{
			AnimationControllerList::iterator result = m_blendAnimations.find( animationName );

			if ( result == m_blendAnimations.end( ) )
			{
				m_blendAnimations.insert( *m_animationControllers.find( animationName ) );
			}

			m_unblendAnimations.erase( animationName );
		}
	}

	void AnimationBlender::UnBlend( const std::string& animationName )
	{
		if ( m_animationControllers.find( animationName ) == m_animationControllers.end( ) )
		{
			std::stringstream logMessage;
			logMessage << "AnimationBlender::UnBlend: " << animationName << " doesn't exist";
			Logger::Get( )->Warn( logMessage.str( ) );
		}
		else
		{
			AnimationControllerList::iterator result = m_unblendAnimations.find( animationName );

			if ( result == m_unblendAnimations.end( ) )
			{
				m_unblendAnimations.insert( *m_animationControllers.find( animationName ) );
			}

			m_blendAnimations.erase( animationName );
		}
	}

	void AnimationBlender::Update( const float& deltaMilliseconds )
	{
		for( AnimationControllerList::iterator i = m_blendAnimations.begin( ); i != m_blendAnimations.end( ); )
		{
			( *i ).second->setMasterWeight( ( *i ).second->getMasterWeight( ) + 0.1f );

			if ( ( *i ).second->getMasterWeight( ) >= 1.0f )
			{
				i = m_blendAnimations.erase( i );
			}
			else
			{
				++i;
			}
		}

		for( AnimationControllerList::iterator i = m_unblendAnimations.begin( ); i != m_unblendAnimations.end( ); )
		{
			( *i ).second->setMasterWeight( ( *i ).second->getMasterWeight( ) - 0.1f );

			if ( ( *i ).second->getMasterWeight( ) <= 0.0f )
			{
				i = m_unblendAnimations.erase( i );
			}
			else
			{
				++i;
			}
		}
	}
}
