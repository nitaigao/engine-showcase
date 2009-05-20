/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef MY_STATE_MACHINE_H
#define MY_STATE_MACHINE_H

#include <Common/Base/Types/Physics/hkStepInfo.h>

class hkaAnimatedSkeleton;
class hkaDefaultAnimationControl;
class hkpCharacterProxy;
class hkaRagdollInstance;
class hkaSkeletonMapper;
class hkaSkeleton;

enum MyStateType
{
	// default states
	MY_STATE_STANDING = 0 ,
	MY_STATE_WALKING,
	MY_STATE_IN_AIR,	
	MY_STATE_DYING,			// Used by ragdolldeath demo
	MY_STATE_DEAD,			// Used by ragdolldeath demo
	MY_STATE_GETTING_UP,	// Used by ragdolldeath demo

	MY_STATE_TOTAL_COUNT
};


// CONTEXT : Contains all persistent state information across updates on the state machine
// (contains per-instance information)

struct MyStateMachineContext
{
public:

	// Current State
	MyStateType m_currentState;

	// Previous State
	MyStateType m_previousState;

	// Animated Skeleton associated with this character
	hkaAnimatedSkeleton*	m_animatedSkeleton;

	// Different animations controls in the character
	hkaDefaultAnimationControl* m_walkControl;
	hkaDefaultAnimationControl* m_idleControl;
	hkaDefaultAnimationControl* m_protectControl;

	// Orientation (forward)
	hkReal m_currentAngle;

	// Physical world
	hkpWorld* m_world;
 
	// Physical proxy associated with this character
	hkpCharacterProxy* m_characterProxy;

	// Physical ragdoll associated with this character
	// and mappings to and from this representation
	// (Used by ragdoll demos)
	hkaRagdollInstance* m_ragdollInstance;
	hkaSkeletonMapper* m_girlToRagdollMapper;
	hkaSkeletonMapper* m_ragdollToGirlMapper;

	// An artificial force applied on dying
	hkVector4 m_launchImpulse;

	MyStateMachineContext()
	{
		m_currentState = m_previousState = MY_STATE_STANDING;
		m_animatedSkeleton = HK_NULL;
		m_walkControl =  HK_NULL;
		m_idleControl = HK_NULL;
		m_world = HK_NULL;
		m_characterProxy = HK_NULL;
		m_ragdollInstance = HK_NULL;
		m_girlToRagdollMapper = HK_NULL;
		m_ragdollToGirlMapper = HK_NULL;
		m_currentAngle = 0;
		m_launchImpulse.setZero4();

	}

	// Removes references
	~MyStateMachineContext();

};


// INPUT : Input in to the state machine (other than the current context)
struct MyStateMachineInput
{
	/// Input X range -1 to 1 (left / right) 
	hkReal				m_inputLR;	

	/// Input Y range -1 to 1 (back / forward)
	hkReal				m_inputDU;	

	/// Whether this character should die as soon as possible
	hkBool m_shouldDie;

	/// Whether this character should get up as soon as possible
	hkBool m_shouldGetUp;

	/// Set this to the timestep between calls to the state machine
	hkStepInfo			m_stepInfo;

	/// Gravity applied in certain states if unsupported.
	hkVector4			m_characterGravity;	

	MyStateMachineInput()
	{
		m_inputLR = m_inputDU = 0.0f;
		m_shouldDie = false;
		m_shouldGetUp = false;
		m_stepInfo.m_deltaTime = -1.0f; // Force the user to set this
		m_stepInfo.m_invDeltaTime = -1.0f; // Force the user to set this
		m_characterGravity.setZero4();
	}

};


// OUTPUT : Output from the state machine (other than changes to the context)
struct MyStateMachineOutput
{
	// Motion resulting during the animation
	hkQsTransform m_desiredMotion;

	// Desired additional velocity
	hkVector4 m_additionalVelocity;


	// Should we disable the character proxy ?
	// (Used by ragdoll demo)
	hkBool m_disableProxy;

	// Should we render the girl based on ragdoll
	hkBool m_renderRagdoll;

	// Whether the ragdoll is in a bad state (through landscape and we're trying to fix it)
	hkBool m_badRagdoll;


	MyStateMachineOutput ()
	{
		m_desiredMotion.setIdentity();
		m_additionalVelocity.setZero4();

		m_disableProxy = false;
		m_renderRagdoll = false;
		m_badRagdoll = false;
	}

};

class MyState;


class MyStateMachine: public hkReferencedObject
{
public:

	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO);

	/// Initializes the array of states to return HK_NULL for all state types
	MyStateMachine();

	// Removes references to all registered states
	~MyStateMachine();

	/// Registers a state for a given state type. This adds a reference to the registered
	/// If a state already exists for this type then the reference to the existing state
	/// is removed.
	void registerState(MyStateType stateType, MyState* state );

	/// returns the state registered for the given type
	/// If no state has been registered this returns HK_NULL
	MyState* getState (MyStateType stateType) const;

	/// Updates the state machine using the given input
	/// The output structure in initialized before being passed to the state
	void update(MyStateMachineContext& context, const MyStateMachineInput& input, MyStateMachineOutput& output);

private:

	MyState* m_registeredStates[MY_STATE_TOTAL_COUNT];

};

#endif //MY_STATE_MACHINE_H

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
