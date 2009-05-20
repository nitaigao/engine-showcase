/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef RAGDOLL_BIPED_STATES_H
#define RAGDOLL_BIPED_STATES_H

#include <Demos/DemoCommon/Utilities/Character/DemoCharacter/AnimatedDemoCharacter/StateMachine/RagdollBipedStateMachine.h>

class RagdollBipedWrapperState : public RagdollBipedState
{
public:

	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO);

	RagdollBipedWrapperState( SimpleBipedState* childState ) : m_childState( childState ) { m_childState->addReference(); }

	virtual ~RagdollBipedWrapperState() { m_childState-> removeReference(); }

		/// Return the state type
	virtual hkUint32 getId() const { return m_childState->getId(); }

	/// Transition to a new state. Returns true if the transition is valid.
	virtual void enterState( hkUint32 prevState, AnimationEventQueue* machine ) const; //{ m_childState->enterState(prevState, machine); }

	/// Transition from this state. Returns true if the transition is valid.
	virtual void leaveState( hkUint32 nextState, AnimationEventQueue* machine ) const;// { m_childState->leaveState(nextState, machine); }

		/// Update this state
	virtual void update( hkReal timestep, const struct RagdollBipedStateInput* input ) const;

	SimpleBipedState* m_childState;

};

class RagdollBipedDeadState : public RagdollBipedState
{
public:

	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO);

	/// Return the state type
	virtual hkUint32 getId() const { return DEAD_STATE; }

	/// Transition to this state.
	virtual void enterState( hkUint32 prevState, class AnimationEventQueue* animMachine ) const;

	/// Transition from this state.
	virtual void leaveState( hkUint32 nextState, class AnimationEventQueue* animMachine ) const;

	/// Update this state
	virtual void update( hkReal timestep, const struct RagdollBipedStateInput* input ) const;

};

class RagdollBipedDyingState : public RagdollBipedState
{
public:

	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO);

	/// Return the state type
	virtual hkUint32 getId() const { return DYING_STATE; }

	/// Transition to this state.
	virtual void enterState( hkUint32 prevState, class AnimationEventQueue* animMachine ) const;

	/// Transition from this state.
	virtual void leaveState( hkUint32 nextState, class AnimationEventQueue* animMachine ) const;

	/// Update this state
	virtual void update( hkReal timestep, const struct RagdollBipedStateInput* input ) const;
};


class RagdollBipedGettingUpState : public RagdollBipedState
{
public:

	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO);

	/// Return the state type
	virtual hkUint32 getId() const { return GETTING_UP_STATE; }

	/// Transition to this state.
	virtual void enterState( hkUint32 prevState, class AnimationEventQueue* animMachine ) const;

	/// Transition from this state.
	virtual void leaveState( hkUint32 nextState, class AnimationEventQueue* animMachine ) const;

	/// Update this state
	virtual void update( hkReal timestep, const struct RagdollBipedStateInput* input ) const;

};


#endif // RAGDOLL_BIPED_STATES_H

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
