/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


#ifndef HK_AsynchronuousSpecialEffectsThreadDemo_H
#define HK_AsynchronuousSpecialEffectsThreadDemo_H

#include <Demos/DemoCommon/DemoFramework/hkDefaultPhysicsDemo.h>
#include <Common/Base/System/Stopwatch/hkStopwatch.h>
#include <Common/Base/Thread/CriticalSection/hkCriticalSection.h>
#include <Common/Base/Thread/Semaphore/hkSemaphore.h>
#include <Common/Base/Thread/Thread/hkThread.h>


//
// NOTE: This demo was moved to tests for 4.0, because it does several things we dont fully support
// - Synchronizing the Tank (needs to be a utility)
// - Rendering from the transforms of the special effects world as it is updated (our read/write locks are there to prevent this kind of thing)
//


	/// This demo is similar to SpecialEffectsPhysicsDemo<br>
	/// However we do not synchronize the two threads but leave them running
	/// totally asynchronously.
	/// This has several consequences:
	///   - If the CPU spent in the special effects is less than 1/60,
	///     than the demo will behave as the synchronous version.
	///   - If the special effects CPU consumption exceeds 100%, than the
	///     special effects physics will fall behind. In this case we get
	///     two simulations which behave like a network physics.
	///   - This means the special effects physics can run a few frames behind.
	///     So we have to predict the current positions by extrapolating the
	///     transforms of the rigid bodies.
	///   - In order to allow the special effects physics to catch up, we slow
	///     down the time in the special effects worlds. This has the effect
	///     of slow motion and reduced gravity, but the visual artifacts might
	///     be less serious than dropping frames.
	///   - As we already run the physics and graphics independently, we can further
	///     reduce the special effects physics frequency to 30Hz, allowing for
	///     twice as much special effects physics compared to running the physics at 60Hz.
	///     In this very case we have to increase the inertia tensor of small
	///     bodies in order to keep the simulation stable.
	///   - As this approach allows the special effects physics to exceed the 100%
	///     CPU budget, we can use this feature to compensate for CPU spikes in the AI,
	///     Sound and Graphics engine. The idea is that if the graphics spikes, we 
	///     spent less CPU on the special effects physics.
class AsynchronuousSpecialEffectsThreadDemo : public hkDefaultPhysicsDemo
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO);

		AsynchronuousSpecialEffectsThreadDemo(hkDemoEnvironment* environment);

		~AsynchronuousSpecialEffectsThreadDemo();

		Result stepDemo(); 

	public:

		struct TankBodies
		{
			hkpRigidBody* m_skiL;
			hkpRigidBody* m_skiR;
			hkpRigidBody* m_body;
			hkpRigidBody* m_tower;
			hkpRigidBody* m_gun;
		};

		struct TankTransforms
		{
			hkTransform m_skiL;
			hkTransform m_skiR;
			hkTransform m_body;
			hkTransform m_tower;
			hkTransform m_gun;
		};

			/// Message from the game engine to the Special Effects (se) physics thread.
			/// All write and read access to the structure must be locked using m_section
		struct Game2SePhysicsInfo
		{
			hkCriticalSection m_section;

				/// If the physics is quicker than the graphics engine
				/// it set m_physicsWaiting to true and waits for m_startPhysics
            hkBool		m_physicsWaiting;
			hkSemaphore m_startPhysics;

				/// The relative time difference between physics and game
			hkReal m_gameTimeMinusSeTime;

				/// The real time factor of the special effect physics
			hkReal m_seTimeFactor;

				/// The time step of the physics
			hkReal m_physicsDeltaTime;

				/// the target frame delta time
			hkReal m_frameDeltaTime;

				/// The transforms of the game physics objects
			TankTransforms m_tankTransforms;

				/// The time in special effects physics when the above transforms are valid
			hkTime m_tankTransformsTime;

				/// Bullet info
			hkUint32 m_remainingBullets;

			hkTransform m_bulletDirection;

			Game2SePhysicsInfo(): m_section(4000), m_startPhysics(0,10){}
		};

			/// The information sent from the special effects physics back to the game engine
		struct Se2GamePhysicsInfo
		{
				// The time we need to extrapolate our special effects physics to
			hkTime m_frameTimeSePhysics;

				// The physics steps per second
			hkReal m_physicsFrequency;

				// The physics CPU load
			hkReal m_physicsCpuLoad;
		};


	public:
		// 
		// objects in game engine hkpWorld
		//
		class TankModel* m_tank;

		//
		//	special effects world
		//
		hkpWorld* m_specialEffectsWorld;
		hkStopwatch m_stopWatch;
		TankBodies m_proxyTank;

		// Communication to/from the special effects physics
		Game2SePhysicsInfo m_game2Se;
		Se2GamePhysicsInfo m_se2game;

			// Averaged fps variable
		hkReal m_fps;


		//
		//	Variables for synchronization of the game-play and physics threads
		//
	public:

		hkThread m_physicsThread;

		class hkSemaphore* m_physicsInitiate;
		class hkSemaphore* m_physicsFinished;

			// main thread -> physics thread communication:
			// A signal from the main thread to the physics thread to terminate
			// Terminating the thread from the outside would leave all resources unfreed
		hkBool m_terminatePhysicsThread;

			// physics thread -> main thread communication:
			// A pointer to the timer data of the physics thread, we just have to append
			// that to the timer data of the main thread once the physics is finished
		//hkCriticalSection m_monitorSection;
		//char m_monitorStreamBegin[200000];
		//char* m_monitorStreamEnd;

		//
		// World setup
		//

		static void HK_CALL createWalls( hkpWorld* world, const hkVector4& pos );

		hkpShape* createGround();

		//
		// Clone/copy bodies between worlds
		//

			// clone all bodies of the tank
		void cloneTankBodies( class TankModel* tank, hkpWorld* world, TankBodies& proxyTank );

			// get the position and orientation 
		void exportTank( TankModel* tank, hkTime currentTime, TankTransforms& tout  );
			
			// keyframe all bodies of the tank into the special effects world
		static void keyframeProxyTank( TankTransforms& tank, hkpWorld* world, hkReal dt, TankBodies& proxyTank );

	protected:

		//
		// Manual display handling
		//

		virtual void setupContexts(hkArray<hkProcessContext*>& contexts);

		class hkpShapeDisplayViewer* m_shapeDisplayViewer;
};

#endif

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
