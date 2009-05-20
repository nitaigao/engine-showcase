/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_WorldLinearCastMultithreadedDemo_H
#define HK_WorldLinearCastMultithreadedDemo_H

#include <Demos/DemoCommon/DemoFramework/hkDefaultPhysicsDemo.h>
#include <Demos/DemoCommon/Utilities/GameUtils/Landscape/FlatLand.h>

class hkpRigidBody;
class hkpMoppBvTreeShape;
class hkpShapePhantom;
class hkpSimpleShapePhantom;
class hkpCachingShapePhantom;
class hkpAabbPhantom;
struct hkpRootCdPoint;
class hkPseudoRandomGenerator;


class WorldLinearCastMultithreadedDemo : public hkDefaultPhysicsDemo
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO);

	public:

		 WorldLinearCastMultithreadedDemo(hkDemoEnvironment* env);
		~WorldLinearCastMultithreadedDemo();

		Result stepDemo(); 

	protected:

		//
		// A QueryObject is a virtual object, consisting of a collidable and a transform, that is never added to the physics system.
		//
		struct QueryObject
		{
			hkpCollidable* m_collidable;
			hkTransform*  m_transform;
		};

		
		//
		//	Shape query examples
		//
		
		// Display one ray hit
		static void HK_CALL displayRayHit( hkpWorld* world, const hkpWorldRayCastInput& in, const hkpWorldRayCastOutput& out );

	public:
		// Display one hit by this collector
		static void HK_CALL displayRootCdPoint( hkpWorld* world, const hkpRootCdPoint& hit );

		// Highlight a object found by the collision detector
		static void HK_CALL displayRootCdBody( hkpWorld* world, const hkpCollidable* collidable, hkpShapeKey key);


	protected:
		//
		//	Build stuff
		//

		// Create some shapes
		static void createShapes(hkArray<hkpShape*>&	bodiesOut);

		// build some persistent query objects. Note: we only need these persistence
		// because of the graphics engine. Do not create these objects on the heap in
		// your game.
		static void HK_CALL buildQueryObects( hkArray<hkpShape*>& shapesIn, hkArray<QueryObject>& objects );
	
		hkArray<hkpRigidBody*>	m_rocksOnTheFloor;

		// a list of collidable/transform pair, just used to display the query,
		// no need to keep that data persistent in a game.
		hkArray<QueryObject>	m_queryObjects;

		// Landscape utility
		FlatLand				m_groundShapeFactory;

	public:

		hkReal m_time;

	private:

		HK_ALIGN128( hkSemaphoreBusyWait	m_semaphore );

};

#endif	// HK_WorldLinearCastMultithreadedDemo_H

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
