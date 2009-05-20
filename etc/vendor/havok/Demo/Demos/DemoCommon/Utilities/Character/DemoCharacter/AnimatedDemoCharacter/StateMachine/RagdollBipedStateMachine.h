/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef RAGDOLL_BIPED_STATE_H
#define RAGDOLL_BIPED_STATE_H

#include <Demos/DemoCommon/Utilities/Character/DemoCharacter/AnimatedDemoCharacter/StateMachine/SimpleBipedStateMachine.h>

enum RagdollBipedStateTypes 
{
	DYING_STATE = SIMPLE_BIPED_MAX_STATE,
	DEAD_STATE,
	GETTING_UP_STATE,
	RAGDOLL_BIPED_MAX_STATE
};

enum RagdollBipedAnimControls
{
	DYING_CONTROL = SIMPLE_BIPED_MAX_CONTROL,
	GET_UP_CONTROL,
	DEATH_CONTROL,
	RAGDOLL_BIPED_MAX_CONTROL
};

struct RagdollBipedStateInput : public SimpleBipedStateInput
{
	/// Whether this character should die as soon as possible
	hkBool m_shouldDie; 

	/// Whether this character should get up as soon as possible
	hkBool m_shouldGetUp; 

	/// Whether the character is done getting up
	hkBool m_doneGettingUp;

	class RagdollBipedStateContext* m_ragdollContext;
};


class RagdollBipedState : public hkReferencedObject
{
public:

	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO);

	/// Return the state type
	virtual hkUint32 getId() const = 0;

	/// Transition to a new state. Returns true if the transition is valid.
	virtual void enterState( hkUint32 prevState, AnimationEventQueue* machine ) const = 0;

	/// Transition from this state. Returns true if the transition is valid.
	virtual void leaveState( hkUint32 nextState, AnimationEventQueue* machine ) const = 0;

	/// Update this state
	virtual void update( hkReal timestep, const RagdollBipedStateInput* input ) const = 0;
};

class RagdollBipedStateManager : public hkReferencedObject
{
	public:
		
		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO);

			/// Removes references to all registered states
		~RagdollBipedStateManager();

			/// Registers a state for a given id. This adds a reference to the registered
			/// If a state already exists for this id then the reference to the existing state
			/// is removed.
		void registerState( hkUint32 stateId, RagdollBipedState* m_state );

			/// returns the state registered for the given id
			/// If no state has been registered this returns HK_NULL
		RagdollBipedState* getState( hkUint32 stateId ) const;

	private:

		hkMap<hkUint32, RagdollBipedState*> m_registeredStates;
};

/// The context holds the current state of the state machine and is the interface that handles all state machine requests.
//class RagdollBipedStateContext : public hkReferencedObject
//{
//public:
//
//	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO);
//
//	/// Initializes the state machine context
//	/// This adds a reference to the state manager
//	RagdollBipedStateContext(const RagdollBipedStateManager* manager);
//
//	/// Updates the state machine using the given input
//	/// The output structure in initialised before being passed to the state
//	void update(hkReal m_timestep, const RagdollBipedStateInput* input);
//
//};

class RagdollBipedStateContext : public hkReferencedObject
{
public:

	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO);

	/// Initializes the state machine context
	/// This adds a reference to the state manager
	RagdollBipedStateContext(const RagdollBipedStateManager* manager);

	/// Removes the reference from the state manager
	~RagdollBipedStateContext();

	/// Returns the current state
	hkUint32 getCurrentState() const;

	/// Causes a state transition. This also calls the leaveState and enterState methods
	/// for the appropriate states
	void setCurrentState(hkUint32 state, AnimationEventQueue* machine );

	/// Updates the state machine using the given input
	/// The output structure in initialised before being passed to the state
	void update(hkReal m_timestep, const RagdollBipedStateInput* input);

	// Time control
	hkReal getCurrentTime() const;

	// Time control
	hkReal getLastStateChange() const;

private:

	const RagdollBipedStateManager* m_stateManager;

	hkUint32 m_currentStateId;

	hkReal m_currentTime;

	hkReal m_lastStateChange;
};

#endif // RAGDOLL_BIPED_STATE_H

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
