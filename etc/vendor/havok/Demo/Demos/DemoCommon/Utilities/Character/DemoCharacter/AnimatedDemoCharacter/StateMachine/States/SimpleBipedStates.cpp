/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/DemoCommon/Utilities/Character/DemoCharacter/AnimatedDemoCharacter/StateMachine/States/SimpleBipedStates.h>

void SimpleBipedWalkState::enterState( hkUint32 prevState, AnimationEventQueue* animMachine ) const
{
	AnimationEventQueue::AnimationCommand cmd;
	cmd.m_time = 0.0f; // immediately
	cmd.m_command = AnimationEventQueue::HK_EASE_IN;
	cmd.m_params.setEase.m_control = MOVE_CONTROL;
	cmd.m_params.setEase.m_easeTime = 0.1f;
	animMachine->addCommand( cmd );

	// sync walk and run
	cmd.m_command = AnimationEventQueue::HK_SET_LOCAL_TIME;
	cmd.m_params.setTime.m_control = WALK_CONTROL;
	cmd.m_params.setTime.m_localTime = 0.0f;
	animMachine->addCommand( cmd );

	cmd.m_params.setTime.m_control = RUN_CONTROL;
	cmd.m_params.setTime.m_localTime = m_walkRunSyncOffset;
	animMachine->addCommand( cmd );

}

void SimpleBipedWalkState::leaveState( hkUint32 nextState, AnimationEventQueue* animMachine ) const
{
	AnimationEventQueue::AnimationCommand cmd;
	cmd.m_time = 0.0f; // immediately
	cmd.m_command = AnimationEventQueue::HK_EASE_OUT;
	cmd.m_params.setEase.m_control = MOVE_CONTROL;


	switch (nextState)
	{
	case IN_AIR_STATE:
		{
			cmd.m_params.setEase.m_easeTime = 0.2f;
			break;
		}
	default:
		{
			cmd.m_params.setEase.m_easeTime = 0.2f;
			break;
		}
	}

	animMachine->addCommand( cmd );
}

void SimpleBipedWalkState::update( hkReal timestep, const SimpleBipedStateInput* input ) const
{
	// Check if we should fall
	if (!input->m_isSupported) 
	{
		input->m_context->setCurrentState(IN_AIR_STATE, input->m_animMachine);
	} 
	else if (!input->m_shouldWalk)
	{
		input->m_context->setCurrentState(STAND_STATE, input->m_animMachine);
	}
	else if (input->m_shouldJump)
	{
		input->m_context->setCurrentState(JUMP_STATE, input->m_animMachine);
	}
	else if (input->m_shouldDive)
	{
		input->m_context->setCurrentState(DIVE_STATE, input->m_animMachine);
	}
}


void SimpleBipedStandState::enterState( hkUint32 prevState, AnimationEventQueue* animMachine ) const 
{
	AnimationEventQueue::AnimationCommand cmd;
	cmd.m_time = 0.0f; // immediately
	cmd.m_command = AnimationEventQueue::HK_EASE_IN;
	cmd.m_params.setEase.m_control = IDLE_CONTROL;
	cmd.m_params.setEase.m_easeTime = 0.2f;
	animMachine->addCommand( cmd );
}

void SimpleBipedStandState::leaveState( hkUint32 nextState, AnimationEventQueue* animMachine ) const 
{
	AnimationEventQueue::AnimationCommand cmd;
	cmd.m_time = 0.0f; // immediately
	cmd.m_command = AnimationEventQueue::HK_EASE_OUT;
	cmd.m_params.setEase.m_control = IDLE_CONTROL;


	switch (nextState)
	{
	case WALK_STATE:
	case IN_AIR_STATE:
		{
			cmd.m_params.setEase.m_easeTime = 0.5f;
			break;
		}
	default:
		{
			cmd.m_params.setEase.m_easeTime = 0.1f;
			break;
		}
	}

	animMachine->addCommand( cmd );
}

void SimpleBipedStandState::update( hkReal timestep, const SimpleBipedStateInput* input ) const
{

	if (!input->m_isSupported)
	{
		input->m_context->setCurrentState(IN_AIR_STATE, input->m_animMachine);
	}
	else if (input->m_shouldWalk)
	{
		input->m_context->setCurrentState(WALK_STATE, input->m_animMachine);
	}
	else if (input->m_shouldJump)
	{
		input->m_context->setCurrentState(JUMP_STATE, input->m_animMachine);
	}
	else if (input->m_shouldDive)
	{
		input->m_context->setCurrentState(DIVE_STATE, input->m_animMachine);
	}
}

void SimpleBipedLandState::enterState( hkUint32 prevState, AnimationEventQueue* animMachine ) const 
{
	AnimationEventQueue::AnimationCommand cmd;
	cmd.m_time = 0.0f; // immediately

	cmd.m_command = AnimationEventQueue::HK_SET_PLAYBACK_SPEED;
	cmd.m_params.setSpeed.m_control = LAND_CONTROL;
	cmd.m_params.setSpeed.m_speed = 1.0f;
	animMachine->addCommand( cmd );

	cmd.m_command = AnimationEventQueue::HK_SET_LOCAL_TIME;
	cmd.m_params.setTime.m_control = LAND_CONTROL;
	cmd.m_params.setTime.m_localTime = 0.0f;
	animMachine->addCommand( cmd );

	cmd.m_command = AnimationEventQueue::HK_EASE_IN;
	cmd.m_params.setEase.m_control = LAND_CONTROL;
	cmd.m_params.setEase.m_easeTime = 0.05f;
	animMachine->addCommand( cmd );

	cmd.m_command = AnimationEventQueue::HK_SET_PLAYBACK_SPEED;
	cmd.m_time = 60.0f / 60.0f;	// Hold at end of animation
	cmd.m_params.setSpeed.m_control = LAND_CONTROL;
	cmd.m_params.setSpeed.m_speed = 0.0f;
	animMachine->addCommand( cmd );

}

void SimpleBipedLandState::leaveState( hkUint32 nextState, AnimationEventQueue* animMachine ) const 
{
	AnimationEventQueue::AnimationCommand cmd;
	cmd.m_time = 0.0f; // immediately
	cmd.m_command = AnimationEventQueue::HK_EASE_OUT;
	cmd.m_params.setEase.m_control = LAND_CONTROL;

	switch (nextState)
	{
	case STAND_STATE:
		{
			cmd.m_params.setEase.m_easeTime = 0.3f;
			break;
		}
	default:
		{
			cmd.m_params.setEase.m_easeTime = 0.3f;
			break;
		}
	}

	animMachine->addCommand( cmd );
}

void SimpleBipedLandState::update( hkReal timestep, const SimpleBipedStateInput* input ) const
{
	const hkReal timeInState = input->m_context->getCurrentTime() - input->m_context->getLastStateChange();

	if (timeInState >= 60.0f / 60.0f - 0.3f)
	{
		if (input->m_shouldWalk)
		{
			input->m_context->setCurrentState(WALK_STATE, input->m_animMachine);
		}
		else if (input->m_shouldJump)
		{
			input->m_context->setCurrentState(JUMP_STATE, input->m_animMachine);
		}
		else if (input->m_shouldDive)
		{
			input->m_context->setCurrentState(DIVE_STATE, input->m_animMachine);
		}
		else
		{
			input->m_context->setCurrentState(STAND_STATE, input->m_animMachine);
		}
	}
}


void SimpleBipedJumpState::enterState( hkUint32 prevState, AnimationEventQueue* animMachine ) const 
{
	AnimationEventQueue::AnimationCommand cmd;
	cmd.m_time = 0.0f; // immediately

	cmd.m_command = AnimationEventQueue::HK_SET_PLAYBACK_SPEED;
	cmd.m_params.setSpeed.m_control = JUMP_CONTROL;
	cmd.m_params.setSpeed.m_speed = 1.0f;
	animMachine->addCommand( cmd );

	cmd.m_command = AnimationEventQueue::HK_SET_LOCAL_TIME;
	cmd.m_params.setTime.m_control = JUMP_CONTROL;
	cmd.m_params.setTime.m_localTime = 0.0f;
	animMachine->addCommand( cmd );

	cmd.m_command = AnimationEventQueue::HK_EASE_IN;
	cmd.m_params.setEase.m_control = JUMP_CONTROL;
	cmd.m_params.setEase.m_easeTime = 0.1f;
	animMachine->addCommand( cmd );

	cmd.m_command = AnimationEventQueue::HK_SET_PLAYBACK_SPEED;
	cmd.m_time = 40.0f / 60.0f;	// Hold at peak of jump - this should be annotation driven (or similar)
	cmd.m_params.setSpeed.m_control = JUMP_CONTROL;
	cmd.m_params.setSpeed.m_speed = 0.0f;
	animMachine->addCommand( cmd );
}

void SimpleBipedJumpState::leaveState( hkUint32 nextState, AnimationEventQueue* animMachine ) const 
{
	AnimationEventQueue::AnimationCommand cmd;
	cmd.m_time = 0.0f; // immediately
	cmd.m_command = AnimationEventQueue::HK_EASE_OUT;
	cmd.m_params.setEase.m_control = JUMP_CONTROL;
	switch (nextState)
	{
	case IN_AIR_STATE:
		{
			cmd.m_params.setEase.m_easeTime = 0.5f;
			break;
		}
	default:
		{
			cmd.m_params.setEase.m_easeTime = 0.1f;
			break;
		}
	}
	animMachine->addCommand( cmd );
}

void SimpleBipedJumpState::update( hkReal timestep, const SimpleBipedStateInput* input ) const
{
	const hkReal timeInState = input->m_context->getCurrentTime() - input->m_context->getLastStateChange();

	if (timeInState >= 40.0f / 60.0f - 0.2f)
	{
		input->m_context->setCurrentState(IN_AIR_STATE, input->m_animMachine);
	}

}

void SimpleBipedInAirState::enterState( hkUint32 prevState, AnimationEventQueue* animMachine ) const 
{
	AnimationEventQueue::AnimationCommand cmd;
	cmd.m_time = 0.0f; // immediately
	cmd.m_command = AnimationEventQueue::HK_EASE_IN;
	cmd.m_params.setEase.m_control = IN_AIR_CONTROL;

	switch (prevState)
	{
	case JUMP_STATE:
		{
			cmd.m_params.setEase.m_easeTime = 0.5f;
			break;
		}
	default:
		{
			cmd.m_params.setEase.m_easeTime = 0.1f;
			break;
		}
	}

	animMachine->addCommand( cmd );
}

void SimpleBipedInAirState::leaveState( hkUint32 nextState, AnimationEventQueue* animMachine ) const 
{
	AnimationEventQueue::AnimationCommand cmd;
	cmd.m_time = 0.0f; // immediately
	cmd.m_command = AnimationEventQueue::HK_EASE_OUT;
	cmd.m_params.setEase.m_control = IN_AIR_CONTROL;

	switch (nextState)
	{
	case STAND_STATE:
		{
			cmd.m_params.setEase.m_easeTime = 0.5f;
			break;
		}
	default:
		{
			cmd.m_params.setEase.m_easeTime = 0.2f;
			break;
		}
	}

	animMachine->addCommand( cmd );
}

void SimpleBipedInAirState::update( hkReal timestep, const SimpleBipedStateInput* input ) const
{
	if (input->m_isSupported)
	{
		if (input->m_shouldWalk)
		{
			input->m_context->setCurrentState(WALK_STATE, input->m_animMachine);
		}
		else if (input->m_shouldJump)
		{
			input->m_context->setCurrentState(JUMP_STATE, input->m_animMachine);
		}
		else
		{
			input->m_context->setCurrentState(LAND_STATE, input->m_animMachine);
		}
	}
}

void SimpleBipedDiveState::enterState( hkUint32 prevState, AnimationEventQueue* animMachine ) const 
{
	AnimationEventQueue::AnimationCommand cmd;
	cmd.m_time = 0.0f; // immediately
	cmd.m_command = AnimationEventQueue::HK_SET_PLAYBACK_SPEED;
	cmd.m_params.setSpeed.m_control = DIVE_CONTROL;
	cmd.m_params.setSpeed.m_speed = 1.0f;
	animMachine->addCommand( cmd );

	cmd.m_command = AnimationEventQueue::HK_SET_LOCAL_TIME;
	cmd.m_params.setTime.m_control = DIVE_CONTROL;
	cmd.m_params.setTime.m_localTime = 0.0f;
	animMachine->addCommand( cmd );

	cmd.m_command = AnimationEventQueue::HK_EASE_IN;
	cmd.m_params.setEase.m_control = DIVE_CONTROL;
	cmd.m_params.setEase.m_easeTime = 0.05f;
	animMachine->addCommand( cmd );

}

void SimpleBipedDiveState::leaveState( hkUint32 nextState, AnimationEventQueue* animMachine ) const 
{
	AnimationEventQueue::AnimationCommand cmd;
	cmd.m_time = 0.0f; // immediately
	cmd.m_command = AnimationEventQueue::HK_EASE_OUT;
	cmd.m_params.setEase.m_control = DIVE_CONTROL;	
	cmd.m_params.setEase.m_easeTime = 0.3f;

	animMachine->addCommand( cmd );
}

void SimpleBipedDiveState::update( hkReal timestep, const SimpleBipedStateInput* input ) const
{
	const hkReal timeInState = input->m_context->getCurrentTime() - input->m_context->getLastStateChange();

	if (timeInState >= 150.0f / 60.0f - 0.3f)
	{
		if (!input->m_isSupported)
		{
			input->m_context->setCurrentState(IN_AIR_STATE, input->m_animMachine);
		}
		else if (input->m_shouldWalk)
		{
			input->m_context->setCurrentState(WALK_STATE, input->m_animMachine);
		}
		else if (input->m_shouldJump)
		{
			input->m_context->setCurrentState(JUMP_STATE, input->m_animMachine);
		}
		else if (!input->m_shouldWalk)
		{
			input->m_context->setCurrentState(STAND_STATE, input->m_animMachine);
		}
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
