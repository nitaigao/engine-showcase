/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_SLIDINGWORLDDEMO_H
#define HK_SLIDINGWORLDDEMO_H

#include <Demos/DemoCommon/DemoFramework/hkDefaultPhysicsDemo.h>

class hkpBroadPhase;

	/// This demo shows a "sliding" world/broadphase. See the .cpp for more details.
class SlidingWorldDemo : public hkDefaultPhysicsDemo
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO);
		
		SlidingWorldDemo(hkDemoEnvironment* env);

		~SlidingWorldDemo();

		void makeFakeInput();
		Result stepDemo();

		enum SlidingWorldDemoMode
		{
			AUTOMATIC_SHIFT,
			MANUAL_SHIFT,
			SLIDINGWORLD_MAX_MODE 
		};

			/// If the demo variant is moving/recentering the broadphase around the world, do that.
		void recenterBroadPhaseVariant(const hkVector4& requestedShift, hkArray<hkpBroadPhaseHandle*>& objectsEnteringBroadphaseBorder, hkVector4& effectiveShift);

			/// If the demo variant is changing (shifting) the coordinate system, do that.
		void shiftCoordinateSystemVariant(const hkVector4& requestedShift, hkArray<hkpBroadPhaseHandle*>& objectsEnteringBroadphaseBorder, hkVector4& effectiveShift);

			/// Helper method to tidy up those bodies leaving the broadphase as a result of moving it. 
			/// Note that we *might* also need this when changing the coordinate system, but it is less likely:
			/// In this demo we do, but normally when we change the coordinate system, we expect to have constructed a 
			/// broadphase large enough to encompass the game world (so changing the coordinate system moves things "more inside" 
			/// the broadphase!).
		void removeBodiesLeavingBroadphaseFromSimulation(const hkArray<hkpBroadPhaseHandle*>& objectsEnteringBroadphaseBorder);

			/// Helper to add new bodies newly 'inside' simulation/broadphase as a result of moving it. 
			/// Similarly here we don't usually expect to need this function when changing the coordinate space, though in this
			/// demo we do.
		void addBodiesNewlyInsideSimulationAreaToSimulation();


	protected:		

			/// We need to update the AABBs for all objects in the world after recentering the broadphase.
		void recalcAabbsAfterBroadphaseRecenter( hkpWorld* world );

			/// This is the core method for coordinate changing.
			/// Translate all objects in the world silently (without informing dependent objects or raising callbacks).
			///    - no calls to updateAabb
			///    - (TODO) no tim updates (m_oldCenterOfMassInWorld(3) reset to 0)
		static void HK_CALL shiftAllGameObjectDataSilently(hkpWorld* world, const hkVector4& shiftDistance );

			/// The result of hkBroadphase::shiftBroadPhase() or hkBroadphase::shiftAllObjects() may have duplicates in it, so this 
			/// just removes them.
		static void HK_CALL removeDuplicatesFromArray(hkArray<hkpBroadPhaseHandle*>& objectsEnteringBroadphaseBorder );

			/// Handle the demo keys
		void handleKeys(hkBool& doShift, hkVector4& newCenter); 

	private:

		static void drawAabb(const hkVector4& m_minExtent, const hkVector4& m_maxExtent, int color);
		static void displayCurrentBroadPhaseAabb(hkpWorld* world, int color);

			/// For illustration purposes, draw something to illustrate the location of the game objects which 
			/// are not currently in simulation (outside the broadphase, hence removed from the world)
		void drawUnsimulatedBodies();

		hkReal m_time;		
		hkUint32 m_ticks;
		hkVector4 m_centers[8];
		hkVector4 m_currentCenter;

		SlidingWorldDemoMode m_currentMode;

		int m_delayBetweenAutomaticShifts;

		hkArray<hkpRigidBody*> m_boxes;

};

#endif	// HK_SLIDINGWORLDDEMO_H

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
