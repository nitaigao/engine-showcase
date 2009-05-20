/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/Animation/Api/MotionExtraction/CharacterController/StateMachine/MyInAirState.h>

#include <Animation/Animation/Playback/hkaAnimatedSkeleton.h>
#include <Animation/Animation/Playback/Control/Default/hkaDefaultAnimationControl.h>

#include <Physics/Utilities/CharacterControl/CharacterProxy/hkpCharacterProxy.h>

// Process the input - causes state transitions and actions
void MyInAirState::update(MyStateMachineContext& context, const MyStateMachineInput& input, MyStateMachineOutput& output )
{
	const hkVector4 up(0.0f,0.0f,1.0f);
	const hkVector4 down(0.0f, 0.0f, -1.0f);

	if (context.m_previousState != MY_STATE_IN_AIR)
	{
		// Ease in the IN AIR ANIMATION 
		// Use idle for now
		context.m_idleControl->easeIn(1.0f);
	}


	// Check if we are now supported or not
	hkpSurfaceInfo ground;
	context.m_characterProxy->checkSupport(down, ground);
	if (ground.m_supportedState == hkpSurfaceInfo::SUPPORTED)
	{
		// We are now standing
		context.m_idleControl->easeOut(1.0f);
		context.m_currentState = MY_STATE_STANDING;
	}

	// Should I die? (see ragdolldeath demo)
	if (input.m_shouldDie)
	{
		context.m_idleControl->easeOut(1.0f);
		context.m_currentState = MY_STATE_DYING;
	}

	output.m_additionalVelocity = context.m_characterProxy->getLinearVelocity();

	// Add in some gravity 
	output.m_additionalVelocity.addMul4( input.m_stepInfo.m_deltaTime.val(), input.m_characterGravity );

	// Changes to context
	context.m_previousState = MY_STATE_IN_AIR;
	context.m_animatedSkeleton->stepDeltaTime(input.m_stepInfo.m_deltaTime);

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
