/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef SIMPLE_BIPED_STATE_H
#define SIMPLE_BIPED_STATE_H

#include <Demos/DemoCommon/Utilities/GameUtils/hkMap.h>
// Protocol for animation playback

// A data protocol on top of the common functionality offered by hkaAnimatedSkeleton and controls.
// Schedules data commands and send them to the animated skeleton.
// The transitions in the state machine each have hard coded lists of these commands
class AnimationEventQueue : public hkReferencedObject
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO);

		 // Instantiate for each animated skeleton
		AnimationEventQueue( class hkaAnimatedSkeleton* m_animatedSkeleton );

		~AnimationEventQueue();

		enum AnimationCommandType
		{
			// No operation
			HK_NOP = 0,

			// Weight Control
			HK_EASE_IN,
			HK_EASE_OUT,
			HK_SET_WEIGHT,

			// Time Control
			HK_SET_LOCAL_TIME,

			// Speed Control
			HK_SET_PLAYBACK_SPEED,

			// Max
			HK_MAX_CMD = 0xffffffff
		};

		struct EaseParams
		{
			int m_control;
			hkReal m_easeTime;
		};

		struct SetWeightParams
		{
			int m_control;
			hkReal m_weight;
		};

		struct SetTimeParams
		{
			int m_control;
			hkReal m_localTime;
		};

		struct SetSpeedParams
		{
			int m_control;
			hkReal m_speed;
		};

		struct AnimationCommand
		{
			hkReal  m_time;

			hkEnum<AnimationCommandType,hkInt8> m_command;

			union {
				EaseParams		setEase;
				SetWeightParams	setWeight;
				SetTimeParams	setTime;
				SetSpeedParams	setSpeed;
			} m_params;

		};

			/// Adds a command to the queue
		void addCommand( const AnimationCommand& command );	

			/// Advance machine to the given time.
		void update( hkReal timestep );

			/// Flush all commands from the queue
		void flushQueue();

			/// Access the internal clock
		hkReal getCurrentTime() const;

			/// Reset the internal clock
		void resetTime(hkReal newTime = 0.0f);

		const hkaAnimatedSkeleton* getAnimatedSkeleton() { return m_animatedSkeleton; }

	protected:

		void process( const AnimationCommand& command );

		hkReal m_currentTime;

		hkaAnimatedSkeleton* m_animatedSkeleton;

		hkArray<AnimationCommand> m_eventQueue;
};

enum SimpleBipedStateTypes
{
	WALK_STATE = 0,
	STAND_STATE,
	JUMP_STATE,
	IN_AIR_STATE,
	LAND_STATE,
	DIVE_STATE,
	SIMPLE_BIPED_MAX_STATE
};

enum SimpleBipedAnimControls
{
	MOVE_CONTROL,
	IDLE_CONTROL,
	JUMP_CONTROL,
	IN_AIR_CONTROL,
	LAND_CONTROL,
	WALK_CONTROL,
	RUN_CONTROL,
	DIVE_CONTROL,
	SIMPLE_BIPED_MAX_CONTROL
};

struct SimpleBipedStateInput
{
		/// Whether this character should walk as soon as possible
	hkBool m_shouldWalk;

		/// Whether this character should jump as soon as possible
	hkBool m_shouldJump; 

		/// Whether this character should dive
	hkBool m_shouldDive; 

		/// Whether the character is supported or not
	hkBool m_isSupported;

	class AnimationEventQueue*	m_animMachine;
	class SimpleBipedStateContext*		m_context;
};


class SimpleBipedState : public hkReferencedObject
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
		virtual void update( hkReal timestep, const SimpleBipedStateInput* input ) const {}
};

class SimpleBipedStateManager : public hkReferencedObject
{
	public:
		
		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO);

			/// Removes references to all registered states
		~SimpleBipedStateManager();

			/// Registers a state for a given id. This adds a reference to the registered
			/// If a state already exists for this id then the reference to the existing state
			/// is removed.
		void registerState( hkUint32 stateId, SimpleBipedState* m_state );

			/// returns the state registered for the given id
			/// If no state has been registered this returns HK_NULL
		SimpleBipedState* getState( hkUint32 stateId ) const;

	private:

		hkMap<hkUint32, SimpleBipedState*> m_registeredStates;
};


/// The context holds the current state of the state machine and is the interface that handles all state machine requests.
class SimpleBipedStateContext : public hkReferencedObject
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO);

			/// Initializes the state machine context
			/// This adds a reference to the state manager
		SimpleBipedStateContext(const SimpleBipedStateManager* manager);

			/// Removes the reference from the state manager
		~SimpleBipedStateContext();

			/// Returns the current state
		hkUint32 getCurrentState() const;

			/// Causes a state transition. This also calls the leaveState and enterState methods
			/// for the appropriate states
		void setCurrentState(hkUint32 state, AnimationEventQueue* machine );

			/// Updates the state machine using the given input
			/// The output structure in initialised before being passed to the state
		void update(hkReal m_timestep, const SimpleBipedStateInput* input);

			// Time control
		hkReal getCurrentTime() const;

			// Time control
		hkReal getLastStateChange() const;

	//public:

		const SimpleBipedStateManager* m_stateManager;

		hkUint32 m_currentStateId;

		hkReal m_currentTime;

		hkReal m_lastStateChange;
};


#endif // SIMPLE_BIPED_STATE_H

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
