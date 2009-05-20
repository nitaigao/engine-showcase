/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#include <Demos/demos.h>
#include <Demos/DemoCommon/Utilities/Character/DemoCharacter/AnimatedDemoCharacter/StateMachine/SimpleBipedStateMachine.h>
#include <Demos/DemoCommon/Utilities/Character/DemoCharacter/AnimatedDemoCharacter/StateMachine/RagdollBipedStateMachine.h>

#include <Animation/Animation/Playback/hkaAnimatedSkeleton.h>
#include <Animation/Animation/Playback/Control/Default/hkaDefaultAnimationControl.h>


// State Manager
RagdollBipedStateManager::~RagdollBipedStateManager()
{
	hkMap<hkUint32, RagdollBipedState*>::iterator itr = m_registeredStates.begin();
	while (itr < m_registeredStates.end())
	{
		itr->second->removeReference();
		itr++;
	}
}

void RagdollBipedStateManager::registerState( hkUint32 stateId, RagdollBipedState* state )
{
	state->addReference();

	if (m_registeredStates.find( stateId ))
	{
		m_registeredStates[ stateId ]->removeReference();
	}

	m_registeredStates[stateId] = state;
}

RagdollBipedState* RagdollBipedStateManager::getState( hkUint32 stateId ) const
{
	return m_registeredStates.get(stateId, HK_NULL);
}

// State machine
RagdollBipedStateContext::RagdollBipedStateContext(const RagdollBipedStateManager* manager)
	: m_stateManager(manager), m_currentStateId(0xffffffff), m_currentTime(0.0f), m_lastStateChange(0.0f)
{
	m_stateManager->addReference();
}
	
RagdollBipedStateContext::~RagdollBipedStateContext()
{
	m_stateManager->removeReference();
}

hkUint32 RagdollBipedStateContext::getCurrentState() const
{
	return m_currentStateId;
}

void RagdollBipedStateContext::setCurrentState(hkUint32 state, AnimationEventQueue* machine )
{
	if (m_currentStateId != state)
	{
		hkUint32 oldStateId = m_currentStateId;
		m_currentStateId = 0xffffffff; // Invalidate state id during transition

		RagdollBipedState* oldState = m_stateManager->getState( oldStateId );
		if (oldState)
		{
			oldState->leaveState( state, machine );
		}

		RagdollBipedState* newState = m_stateManager->getState( state );
		if (newState)
		{
			newState->enterState( oldStateId, machine );
		}
		m_currentStateId = state;
		m_lastStateChange = m_currentTime;
	}
}

void RagdollBipedStateContext::update(hkReal timestep, const RagdollBipedStateInput* input )
{
	RagdollBipedState* currentState = m_stateManager->getState( m_currentStateId );
	if (currentState)
	{
		currentState->update( timestep, input );
	}
	m_currentTime += timestep;
}

// Time control
hkReal RagdollBipedStateContext::getCurrentTime() const
{
	return m_currentTime;
}

// Time control
hkReal RagdollBipedStateContext::getLastStateChange() const
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
