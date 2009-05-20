/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

// This demo is an optimised version of the WorldRaycastDemo.
// As the current 3Axis-sweep-and-prune is not optimal for ray casts there are a number of
// strategies to speed things up. These include:
//    - Creating am hkpAabbPhantom around the raycast. This is by far the best choice if your ray is short
//      and is persistent over frames. (using the green rays)
//    - Using an hkpBroadPhaseAabbCache, which prefilters the broadphase to a sub volume. Within this
//      volume raycasts can be performed at a much better CPU. (using yellow rays)
//    - If we have many rays starting at the same position, we can combine those rays into a single cast.
//      Also we can combine this technique with using an hkpBroadPhaseAabbCache. (using blue rays)
//    - And we still have the unoptimized version (using red rays)
// This demo will create 4 sets of 8 short raycasts, each using one of the above techniques.
// It is recommended that you start playing around with the parameters like world size,
// number of objects and objects size. 
// Ideally in the end the scene will match your game in respect to object number and object density. 
// All objects in the scene are 1*1*1 meter cubes.

#ifndef HK_OptimizedWorldRaycastDemo_H
#define HK_OptimizedWorldRaycastDemo_H


#include <Demos/DemoCommon/DemoFramework/hkDefaultPhysicsDemo.h>

class hkpRigidBody;
class hkpAabbPhantom;


class OptimizedWorldRaycastDemo : public hkDefaultPhysicsDemo
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO);

		OptimizedWorldRaycastDemo(hkDemoEnvironment* env);

		~OptimizedWorldRaycastDemo();

		Result stepDemo(); 

	protected:
		// Helper methods
		void createBodies();

		void displayHit( const hkpWorldRayCastInput& input, hkpClosestRayHitCollector& collector, int color) const;
		void displayHits( const hkpWorldRayCastInput* inputs, int numInputs, hkpClosestRayHitCollector* collectors, int color) const;

	protected:
		
		hkpAabbPhantom *m_phantom;
		hkpAabbPhantom *m_phantomUseCache;
		hkReal m_time;
		int m_numRigidBodies;
		int m_numBroadphaseMarkers;
		hkReal m_rayLength;
		hkReal m_worldSizeX;
		hkReal m_worldSizeY;
		hkReal m_worldSizeZ;
};

#endif	// HK_OptimizedWorldRaycastDemo_H

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
