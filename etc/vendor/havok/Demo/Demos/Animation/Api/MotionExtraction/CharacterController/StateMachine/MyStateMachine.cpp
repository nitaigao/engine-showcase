/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/Animation/Api/MotionExtraction/CharacterController/StateMachine/MyStateMachine.h>
#include <Demos/Animation/Api/MotionExtraction/CharacterController/StateMachine/MyState.h>

#include <Animation/Animation/Playback/Control/Default/hkaDefaultAnimationControl.h>
#include <Animation/Animation/Playback/hkaAnimatedSkeleton.h>
#include <Animation/Animation/Mapper/hkaSkeletonMapper.h>

#include <Physics/Utilities/CharacterControl/CharacterProxy/hkpCharacterProxy.h>
#include <Animation/Ragdoll/Instance/hkaRagdollInstance.h>


MyStateMachineContext::~MyStateMachineContext()
{

}



/// Initialises the array of states to return HK_NULL for all state types
MyStateMachine::MyStateMachine()
{
	for (int i=0; i < MY_STATE_TOTAL_COUNT; i++)
	{
		m_registeredStates[i] = HK_NULL;
	}

}

// Removes references to all registered states
MyStateMachine::~MyStateMachine()
{
	for (int i=0; i < MY_STATE_TOTAL_COUNT; i++)
	{
		if (m_registeredStates[i] != HK_NULL)
		{
			m_registeredStates[i]->removeReference();
			m_registeredStates[i] = HK_NULL;
		}
	}
}

/// Registers a state for a given state type. This adds a reference to the registered
/// If a state already exists for this type then the reference to the existing state
/// is removed.
void MyStateMachine::registerState(MyStateType stateType, MyState* state)
{
	state->addReference();

	MyState* oldState = m_registeredStates[stateType];

	if (oldState != HK_NULL)
	{
		oldState->removeReference();
	}

	m_registeredStates[stateType] = state; 
}

/// returns the state registered for the given type
/// If no state has been registered this returns HK_NULL
MyState* MyStateMachine::getState (MyStateType stateType) const
{
	return m_registeredStates[stateType]; 
}


/// Updates the state machine using the given input
/// The output structure in initialised before being passed to the state
void MyStateMachine::update(MyStateMachineContext& context, const MyStateMachineInput& input, MyStateMachineOutput& output)
{
	getState(context.m_currentState)->update(context, input, output);
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
