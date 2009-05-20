/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef GDC_STATE_H
#define GDC_STATE_H

#include <Demos/DemoCommon/Utilities/GameUtils/hkMap.h>
// Protocol for animation playback

// A data protocol on top of the common functionality offered by hkaAnimatedSkeleton and controls.
// Schedules data commands and send them to the animated skeleton.
// The transitions in the state machine each have hard coded lists of these commands
class GdcAnimationMachine : public hkReferencedObject
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO);

		 // Instantiate for each animated skeleton
		GdcAnimationMachine( class hkaAnimatedSkeleton* m_animatedSkeleton );

		~GdcAnimationMachine();

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

	protected:

		void process( const AnimationCommand& command );

		hkReal m_currentTime;

		hkaAnimatedSkeleton* m_animatedSkeleton;

		hkArray<AnimationCommand> m_eventQueue;
};


struct GdcStateInput
{
		/// Whether this character should walk as soon as possible
	hkBool m_shouldWalk;

		/// Whether this character should jump as soon as possible
	hkBool m_shouldJump;

		/// Whether this character should die as soon as possible
	hkBool m_shouldDie;

		/// Whether this character should get up as soon as possible
	hkBool m_shouldGetUp;

		/// Whether this character should dive
	hkBool m_shouldDive;

		/// Whether the character is supported or not
	hkBool m_isSupported;

	class GdcAnimationMachine*	m_animMachine;
	class GdcStateContext*		m_context;
	class Gdc2005DemoOptions*	m_options;
};

class GdcState : public hkReferencedObject
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO);

			/// Return the state type
		virtual hkUint32 getId() const = 0;

			/// Transition to a new state. Returns true if the transition is valid.
		virtual void enterState( hkUint32 prevState, GdcAnimationMachine* machine ) const = 0;

			/// Transition from this state. Returns true if the transition is valid.
		virtual void leaveState( hkUint32 nextState, GdcAnimationMachine* machine ) const = 0;

			/// Update this state
		virtual void update( hkReal timestep, const GdcStateInput* input ) const = 0;
};

class GdcStateManager : public hkReferencedObject
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO);

			/// Removes references to all registered states
		~GdcStateManager();

			/// Registers a state for a given id. This adds a reference to the registered
			/// If a state already exists for this id then the reference to the existing state
			/// is removed.
		void registerState( hkUint32 stateId, GdcState* m_state );

			/// returns the state registered for the given id
			/// If no state has been registered this returns HK_NULL
		GdcState* getState( hkUint32 stateId ) const;

	private:

		hkMap<hkUint32, GdcState*> m_registeredStates;
};

/// The context holds the current state of the state machine and is the interface that handles all state machine requests.
class GdcStateContext : public hkReferencedObject
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO);

			/// Initializes the state machine context
			/// This adds a reference to the state manager
		GdcStateContext(const GdcStateManager* manager);

			/// Removes the reference from the state manager
		~GdcStateContext();

			/// Returns the current state
		hkUint32 getCurrentState() const;

			/// Causes a state transition. This also calls the leaveState and enterState methods
			/// for the appropriate states
		void setCurrentState(hkUint32 state, GdcAnimationMachine* machine );

			/// Updates the state machine using the given input
			/// The output structure in initialised before being passed to the state
		void update(hkReal m_timestep, const GdcStateInput* input);

			// Time control
		hkReal getCurrentTime() const;

			// Time control
		hkReal getLastStateChange() const;

	private:

		const GdcStateManager* m_stateManager;

		hkUint32 m_currentStateId;

		hkReal m_currentTime;

		hkReal m_lastStateChange;
};

#endif // GDC_STATE_H

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
