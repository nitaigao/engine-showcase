/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#include <Demos/demos.h>
#include <Demos/DemoCommon/Utilities/Character/DemoCharacter/AnimatedDemoCharacter/StateMachine/SimpleBipedStateMachine.h>

#include <Animation/Animation/Playback/hkaAnimatedSkeleton.h>
#include <Animation/Animation/Playback/Control/Default/hkaDefaultAnimationControl.h>


AnimationEventQueue::AnimationEventQueue( class hkaAnimatedSkeleton* animatedSkeleton )
:  m_currentTime(0.0f), m_animatedSkeleton( animatedSkeleton )
{
	m_animatedSkeleton->addReference();
}

AnimationEventQueue::~AnimationEventQueue( )
{
	m_animatedSkeleton->removeReference();
}

void AnimationEventQueue::addCommand( const AnimationCommand& command )
{
	// Set the global time for the command
	AnimationEventQueue::AnimationCommand gCmd = command;
	gCmd.m_time += getCurrentTime();
	
	int idx = 0;
	while( (idx < m_eventQueue.getSize()) && (gCmd.m_time >= m_eventQueue[idx].m_time) )
	{
		idx++;
	}

	if (idx == m_eventQueue.getSize())
	{
		m_eventQueue.pushBack( gCmd );
	}
	else
	{
		m_eventQueue.insertAt(idx, gCmd);
	}
}

void AnimationEventQueue::process( const AnimationCommand& command )
{
	// This method shows that controls should go somewhere else to be managed.
	// And they should have a data interface 

	switch ( command.m_command )
	{
	case HK_EASE_IN:
		{
			hkaDefaultAnimationControl* control = (hkaDefaultAnimationControl*) m_animatedSkeleton->getAnimationControl( command.m_params.setEase.m_control );
			control->easeIn( command.m_params.setEase.m_easeTime );
		}
		break;
	case HK_EASE_OUT:
		{
			hkaDefaultAnimationControl* control = (hkaDefaultAnimationControl*) m_animatedSkeleton->getAnimationControl( command.m_params.setEase.m_control );
			control->easeOut( command.m_params.setEase.m_easeTime  );
		}
		break;
	case HK_SET_WEIGHT:
		{
			hkaDefaultAnimationControl* control = (hkaDefaultAnimationControl*) m_animatedSkeleton->getAnimationControl( command.m_params.setWeight.m_control );
			control->setMasterWeight( command.m_params.setWeight.m_weight );
		}
		break;
	case HK_SET_LOCAL_TIME:
		{
			hkaDefaultAnimationControl* control = (hkaDefaultAnimationControl*) m_animatedSkeleton->getAnimationControl( command.m_params.setTime.m_control );
			control->setLocalTime( command.m_params.setTime.m_localTime );
		}
		break;
	case HK_SET_PLAYBACK_SPEED:
		{
			hkaDefaultAnimationControl* control = (hkaDefaultAnimationControl*) m_animatedSkeleton->getAnimationControl( command.m_params.setSpeed.m_control );
			control->setPlaybackSpeed( command.m_params.setSpeed.m_speed );
		}
		break;
	default:
		break;
	}
}

void AnimationEventQueue::update( hkReal timestep )
{
	const hkReal endTime = m_currentTime + timestep;

	while ((m_eventQueue.getSize()) && (m_eventQueue[0].m_time <= endTime))
	{
		process( m_eventQueue[0] );
		// Update internal time (in case we later use it during processing)
		m_currentTime = hkMath::max2(m_currentTime, m_eventQueue[0].m_time);
		m_eventQueue.removeAtAndCopy( 0 );
	}	

	m_currentTime = endTime;
}

void AnimationEventQueue::flushQueue()
{
	m_eventQueue.clear();
}

hkReal AnimationEventQueue::getCurrentTime() const
{
	return m_currentTime;
}

void AnimationEventQueue::resetTime(hkReal newTime)
{
	hkReal diffTime = newTime - m_currentTime;
	for (int i=0; i< m_eventQueue.getSize(); i++)
	{
		m_eventQueue[i].m_time += diffTime;
	}

	m_currentTime = newTime;
}

// State Manager
SimpleBipedStateManager::~SimpleBipedStateManager()
{
	hkMap<hkUint32, SimpleBipedState*>::iterator itr = m_registeredStates.begin();
	while (itr < m_registeredStates.end())
	{
		itr->second->removeReference();
		itr++;
	}
}

void SimpleBipedStateManager::registerState( hkUint32 stateId, SimpleBipedState* state )
{
	state->addReference();

	if (m_registeredStates.find( stateId ))
	{
		m_registeredStates[ stateId ]->removeReference();
	}

	m_registeredStates[stateId] = state;
}

SimpleBipedState* SimpleBipedStateManager::getState( hkUint32 stateId ) const
{
	return m_registeredStates.get(stateId, HK_NULL);
}

// State machine
SimpleBipedStateContext::SimpleBipedStateContext(const SimpleBipedStateManager* manager)
	: m_stateManager(manager), m_currentStateId(0xffffffff), m_currentTime(0.0f), m_lastStateChange(0.0f)
{
	m_stateManager->addReference();
}
	
SimpleBipedStateContext::~SimpleBipedStateContext()
{
	m_stateManager->removeReference();
}

hkUint32 SimpleBipedStateContext::getCurrentState() const
{
	return m_currentStateId;
}

void SimpleBipedStateContext::setCurrentState(hkUint32 state, AnimationEventQueue* machine )
{
	if (m_currentStateId != state)
	{
		hkUint32 oldStateId = m_currentStateId;
		m_currentStateId = 0xffffffff; // Invalidate state id during transition

		SimpleBipedState* oldState = m_stateManager->getState( oldStateId );
		if (oldState)
		{
			oldState->leaveState( state, machine );
		}

		SimpleBipedState* newState = m_stateManager->getState( state );
		if (newState)
		{
			newState->enterState( oldStateId, machine );
		}
		m_currentStateId = state;
		m_lastStateChange = m_currentTime;
	}
}

void SimpleBipedStateContext::update(hkReal timestep, const SimpleBipedStateInput* input )
{
	SimpleBipedState* currentState = m_stateManager->getState( m_currentStateId );
	if (currentState)
	{
		currentState->update( timestep, input );
	}
	m_currentTime += timestep;
}

// Time control
hkReal SimpleBipedStateContext::getCurrentTime() const
{
	return m_currentTime;
}

// Time control
hkReal SimpleBipedStateContext::getLastStateChange() const
{
	return m_lastStateChange;
}

// Walk to jump with Sync.

// hkReal timeLeft = m_walkControl->getPeriod() - m_walkControl->getLocalTime();
// HK_ANIM_EVENT( NOW,				EASE_OUT,				WALK_CONTROL, timeLeft );
// HK_ANIM_EVENT( NOW,				SET_LOCAL_TIME,			JUMP_CONTROL, 0.0f );
// HK_ANIM_EVENT( NOW,				SET_PLAYBACK_SPEED,		JUMP_CONTROL, 0.0f );
// HK_ANIM_EVENT( NOW,				EASE_IN,				JUMP_CONTROL, timeLeft );
// HK_ANIM_EVENT( NOW,				CHANGE_STATE,			WALK_TO_JUMP );
// HK_ANIM_EVENT( NOW + timeLeft,	SET_PLAYBACK_SPEED,		JUMP_CONTROL, 1.0f );
// HK_ANIM_EVENT( NOW + timeLeft,	CHANGE_STATE,			JUMP );

// Straight transition

// HK_ANIM_EVENT( NOW,		EASE_OUT,	  WALK_CONTROL, 0.1f )
// HK_ANIM_EVENT( NOW,		EASE_IN,	  FALL_CONTROL, 0.1f )
// HK_ANIM_EVENT( NOW+0.1f, CHANGE_STATE, FALLING )

// Flushing command pipe

// HK_ANIM_EVENT( NOW, FLUSH );

// Wait for annotation event before going to idle

// hkReal eventTime = m_walkControl->getAnimation->getAnnotations()[0].m_time - m_walkControl->getLocalTime();
// hkReal loopedEventTime = (eventTime < 0.0f) ? eventTime + m_walkControl->getPeriod() : eventTime;
// HK_ANIM_EVENT(NOW, CHANGE_STATE, WAITING);
// HK_ANIM_EVENT(NOW + loopedEventTime, CHANGE_STATE, IDLE);

// Overlay a wave anim for 2 secs
// HK_ANIM_EVENT(NOW, EASE_IN, WAVE_CONTROL, 0.1f);
// HK_ANIM_EVENT(NOW+2, EASE_OUT, WAVE_CONTROL, 0.1f);

/*
* Havok SDK - NO SOURCE PC DOWNLOAD, BUILD(#20090216)
* 
* Confidential Information of Havok.  (C) Copyright 1999-2009
* Telekinesys Research Limited t/a Havok. All Rights Reserved. The Havok
* Logo, and the Havok buzzsaw logo are trademarks of Havok.  Title, ownership
* rights, and intellectual property rights in the Havok software remain in
* Havok and/or its suppliers.
* 
* Use of this software for evaluation purposes is subject to and indicates
* acceptance of the End User licence Agreement for this product. A copy of
* the license is included with this software and is also available at www.havok.com/tryhavok.
* 
*/
