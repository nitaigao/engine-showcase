/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/DemoCommon/Utilities/Character/DemoCharacter/AnimatedDemoCharacter/StateMachine/States/RagdollBipedStates.h>

#include <Animation/Animation/Playback/hkaAnimatedSkeleton.h>
#include <Animation/Animation/Playback/Control/Default/hkaDefaultAnimationControl.h>


class RagdollDemoCharacter;

/// Update this state
void RagdollBipedWrapperState::update( hkReal timestep, const struct RagdollBipedStateInput* input ) const
{
	if (input->m_shouldDie)
	{
		input->m_ragdollContext->setCurrentState(DYING_STATE, input->m_animMachine);
//		input->m_ragdollCharacter->addRagdollToWorld();
	}
	else if (input->m_shouldGetUp)
	{
		input->m_ragdollContext->setCurrentState(GETTING_UP_STATE, input->m_animMachine);
	}
	else
	{
		int oldStateId = input->m_context->getCurrentState();
		m_childState->update( timestep, input );
		int newStateId = input->m_context->getCurrentState();
		if (oldStateId != newStateId)
		{
			// state changed
			input->m_ragdollContext->setCurrentState(newStateId, input->m_animMachine);
		}
	}
}

void RagdollBipedWrapperState::enterState( hkUint32 prevState, AnimationEventQueue* machine ) const
{
	m_childState->enterState(prevState, machine);
}

/// Transition from this state. Returns true if the transition is valid.
void RagdollBipedWrapperState::leaveState( hkUint32 nextState, AnimationEventQueue* machine ) const
{ 
	m_childState->leaveState(nextState, machine);
}



void RagdollBipedDeadState::enterState( hkUint32 prevState, AnimationEventQueue* animMachine ) const 
{
	AnimationEventQueue::AnimationCommand cmd;
	cmd.m_time = 0.0f; // immediately
	cmd.m_command = AnimationEventQueue::HK_EASE_IN;
	cmd.m_params.setEase.m_control = DEATH_CONTROL;
	cmd.m_params.setEase.m_easeTime = 0.5f;
	animMachine->addCommand( cmd );
}

void RagdollBipedDeadState::leaveState( hkUint32 nextState, AnimationEventQueue* animMachine ) const 
{
	AnimationEventQueue::AnimationCommand cmd;
	cmd.m_time = 0.0f; // immediately
	cmd.m_command = AnimationEventQueue::HK_EASE_OUT;
	cmd.m_params.setEase.m_control = DEATH_CONTROL;
	cmd.m_params.setEase.m_easeTime = 0.05f;
	animMachine->addCommand( cmd );
}

void RagdollBipedDeadState::update( hkReal timestep, const RagdollBipedStateInput* input ) const
{
	if (input->m_shouldGetUp)
	{
		input->m_ragdollContext->setCurrentState(GETTING_UP_STATE, input->m_animMachine);
//		input->m_ragdollCharacter->removeRagdollFromWorld();
	}
}


void RagdollBipedDyingState::enterState( hkUint32 prevState, AnimationEventQueue* animMachine ) const 
{
	AnimationEventQueue::AnimationCommand cmd;
	cmd.m_command = AnimationEventQueue::HK_SET_PLAYBACK_SPEED;
	cmd.m_params.setSpeed.m_control = DYING_CONTROL;
	cmd.m_params.setSpeed.m_speed = 1.0f;
	animMachine->addCommand( cmd );

	cmd.m_command = AnimationEventQueue::HK_SET_LOCAL_TIME;
	cmd.m_params.setTime.m_control = DYING_CONTROL;
	cmd.m_params.setTime.m_localTime = 0.0f;
	animMachine->addCommand( cmd );

	cmd.m_command = AnimationEventQueue::HK_EASE_IN;
	cmd.m_params.setEase.m_control = DYING_CONTROL;
	cmd.m_params.setEase.m_easeTime = 0.2f;
	animMachine->addCommand( cmd );

	cmd.m_command = AnimationEventQueue::HK_SET_PLAYBACK_SPEED;
	cmd.m_time = 96.0f / 60.0f;
	cmd.m_params.setSpeed.m_control = DYING_CONTROL;
	cmd.m_params.setSpeed.m_speed = 0.0f;
	animMachine->addCommand( cmd );
}

void RagdollBipedDyingState::leaveState( hkUint32 nextState, AnimationEventQueue* animMachine ) const 
{
	AnimationEventQueue::AnimationCommand cmd;
	cmd.m_time = 0.0f; // immediately
	cmd.m_command = AnimationEventQueue::HK_EASE_OUT;
	cmd.m_params.setEase.m_control = DYING_CONTROL;
	cmd.m_params.setEase.m_easeTime = 0.5f;
	animMachine->addCommand( cmd );
}

void RagdollBipedDyingState::update( hkReal timestep, const RagdollBipedStateInput* input ) const
{
	const hkReal timeInState = input->m_ragdollContext->getCurrentTime() - input->m_ragdollContext->getLastStateChange();

	if ( timeInState >= 96.0f / 60.0f - 0.5f )
	{
		input->m_ragdollContext->setCurrentState(DEAD_STATE, input->m_animMachine);
	}
}


void RagdollBipedGettingUpState::enterState( hkUint32 prevState, AnimationEventQueue* animMachine ) const 
{

	AnimationEventQueue::AnimationCommand cmd;
	cmd.m_time = 0.0f;

	cmd.m_command = AnimationEventQueue::HK_EASE_IN;
	cmd.m_params.setEase.m_control = GET_UP_CONTROL;
	cmd.m_params.setEase.m_easeTime = 0.0f;
	animMachine->addCommand( cmd );

	cmd.m_command = AnimationEventQueue::HK_SET_PLAYBACK_SPEED;
	cmd.m_params.setSpeed.m_control = GET_UP_CONTROL;
	cmd.m_params.setSpeed.m_speed = 1.0f;
	animMachine->addCommand( cmd );

}

void RagdollBipedGettingUpState::leaveState( hkUint32 nextState, AnimationEventQueue* animMachine ) const 
{
	AnimationEventQueue::AnimationCommand cmd;
	cmd.m_time = 0.0f; // immediately
	cmd.m_command = AnimationEventQueue::HK_EASE_OUT;
	cmd.m_params.setEase.m_control = GET_UP_CONTROL;
	cmd.m_params.setEase.m_easeTime = 0.3f;
	animMachine->addCommand( cmd );
}

void RagdollBipedGettingUpState::update( hkReal timestep, const RagdollBipedStateInput* input ) const
{
	const hkaAnimatedSkeleton* skel = input->m_animMachine->getAnimatedSkeleton();
	hkaDefaultAnimationControl* getUpControl = (hkaDefaultAnimationControl*)skel->getAnimationControl( GET_UP_CONTROL );
	
	if (input->m_shouldDie)
	{
		input->m_ragdollContext->setCurrentState(DYING_STATE, input->m_animMachine);
	}
	else if (getUpControl->getAnimationBinding()->m_animation->m_duration - getUpControl->getLocalTime() < 0.3f )
	{
		// Automatically transition to standing when the get-up animation is almost done.
		input->m_ragdollContext->setCurrentState(STAND_STATE, input->m_animMachine);

		// Stop the get-up animation from looping
		AnimationEventQueue::AnimationCommand cmd;
		cmd.m_command = AnimationEventQueue::HK_SET_PLAYBACK_SPEED;
		cmd.m_params.setSpeed.m_control = GET_UP_CONTROL;
		cmd.m_params.setSpeed.m_speed = 0.0f;
		input->m_animMachine->addCommand( cmd );
	}
	
}

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
