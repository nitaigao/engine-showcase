/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/ShowCase/Gdc2005/StateMachine/States/GdcLandState.h>

void GdcLandState::enterState( hkUint32 prevState, GdcAnimationMachine* animMachine ) const
{
	GdcAnimationMachine::AnimationCommand cmd;
	cmd.m_time = 0.0f; // immediately

	cmd.m_command = GdcAnimationMachine::HK_SET_PLAYBACK_SPEED;
	cmd.m_params.setSpeed.m_control = GDC_LAND_CONTROL;
	cmd.m_params.setSpeed.m_speed = 1.0f;
	animMachine->addCommand( cmd );

	cmd.m_command = GdcAnimationMachine::HK_SET_LOCAL_TIME;
	cmd.m_params.setTime.m_control = GDC_LAND_CONTROL;
	cmd.m_params.setTime.m_localTime = 0.0f;
	animMachine->addCommand( cmd );

	cmd.m_command = GdcAnimationMachine::HK_EASE_IN;
	cmd.m_params.setEase.m_control = GDC_LAND_CONTROL;
	cmd.m_params.setEase.m_easeTime = 0.05f;
	animMachine->addCommand( cmd );

	cmd.m_command = GdcAnimationMachine::HK_SET_PLAYBACK_SPEED;
	cmd.m_time = 60.0f / 60.0f;	// Hold at end of animation
	cmd.m_params.setSpeed.m_control = GDC_LAND_CONTROL;
	cmd.m_params.setSpeed.m_speed = 0.0f;
	animMachine->addCommand( cmd );

}

void GdcLandState::leaveState( hkUint32 nextState, GdcAnimationMachine* animMachine ) const
{
	GdcAnimationMachine::AnimationCommand cmd;
	cmd.m_time = 0.0f; // immediately
	cmd.m_command = GdcAnimationMachine::HK_EASE_OUT;
	cmd.m_params.setEase.m_control = GDC_LAND_CONTROL;

	switch (nextState)
	{
	case GDC_STAND_STATE:
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

void GdcLandState::update( hkReal timestep, const GdcStateInput* input ) const
{
	if (input->m_shouldDie)
	{
		input->m_context->setCurrentState(GDC_DYING_STATE, input->m_animMachine);
	}

	const hkReal timeInState = input->m_context->getCurrentTime() - input->m_context->getLastStateChange();

	if (timeInState >= 60.0f / 60.0f - 0.3f)
	{
 		if (input->m_shouldWalk)
		{
			input->m_context->setCurrentState(GDC_WALK_STATE, input->m_animMachine);
		}
		else if (input->m_shouldJump)
		{
			input->m_context->setCurrentState(GDC_JUMP_STATE, input->m_animMachine);
		}
		else if (input->m_shouldDive)
		{
			input->m_context->setCurrentState(GDC_DIVE_STATE, input->m_animMachine);
		}
		else
		{
			input->m_context->setCurrentState(GDC_STAND_STATE, input->m_animMachine);
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
