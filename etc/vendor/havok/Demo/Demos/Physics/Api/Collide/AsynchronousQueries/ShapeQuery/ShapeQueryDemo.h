/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

// This demo illustrates how to query the collision detection system asynchronously.
// It introduces the use of collector classes for both ray and linear (shape) casting.
// We will be querying both phantoms and the world for:
// - results of a raycast into the world,
// - results of a linear shape cast into the world,
// - all objects intersecting with a query object,
// - the closest points to an object in the world.

#ifndef HK_ShapeQueryDemo_H
#define HK_ShapeQueryDemo_H

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


class ShapeQueryDemo : public hkDefaultPhysicsDemo
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO);

		ShapeQueryDemo(hkDemoEnvironment* env);

		~ShapeQueryDemo();

		Result stepDemo(); 

	protected:
		// This demo uses 'QueryObjects' which are simply structures that
		// contain a hkpCollidable* and a hkMotionState*. These are used
		// during the world query calls and for display purposes.
		struct QueryObject
		{
			hkpCollidable* m_collidable;
			hkTransform*  m_transform;
		};

		
		//
		//	Shape query examples
		//
		
		// World queries for: ray cast; linear cast; penetrations; closest points
		static void HK_CALL worldRayCast( hkpWorld* world, hkReal time, hkBool useCollector = false );
		static void HK_CALL worldLinearCast( hkpWorld* world, hkReal time, hkArray<QueryObject>& queryObjects );
		static void HK_CALL worldGetPenetrations( hkpWorld* world, hkReal time, hkArray<QueryObject>& queryObjects );
		static void HK_CALL worldGetClosestPoints( hkpWorld* world, hkReal time, hkArray<QueryObject>& queryObjects );

		// Phantom queries for: Ray cast; ...
		static void HK_CALL phantomRayCast( hkpWorld* world, hkReal time, hkArray<hkpAabbPhantom*>& phantoms, hkBool useCollector = false );

		// We want to use templated member functions of the following form,
		// but some compilers dont like this. Instead, alternative templated
		// forms of these functions are statically declared in the cpp file.
		//static void HK_CALL phantomLinearCast( hkpWorld* world, hkReal time, hkArray<T*>& phantoms, hkReal offset=0.0f );
		//static void HK_CALL phantomGetPenetrations( hkpWorld* world, hkReal time, hkArray<T*>& phantoms, hkReal offset=0.0f );
		//static void HK_CALL phantomGetClosestPoints( hkpWorld* world, hkReal time, hkArray<T*>& phantoms, hkReal offset=0.0f );
		

		//
		//	Display stuff
		//

		// Override normal physics demo context
		void setupContexts(hkArray<hkProcessContext*>& contexts);

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
	
		// build some persistent phantoms for fast collision queries. We create AABB phantoms
		// for the raycast, and one simple phantom and one cached phantom for each "query object".
		static void HK_CALL buildAabbPhantoms( hkpWorld* world, int numPhantoms, hkArray<hkpAabbPhantom*>& phantoms, hkPseudoRandomGenerator& generator );
		static void HK_CALL buildSimplePhantoms( hkpWorld* world, hkArray<hkpShape*>& shapes, hkArray<hkpSimpleShapePhantom*>& phantoms, hkPseudoRandomGenerator& generator );
		static void HK_CALL buildCachingPhantoms( hkpWorld* world, hkArray<hkpShape*>& shapes, hkArray<hkpCachingShapePhantom*>& phantoms, hkPseudoRandomGenerator& generator );
		

		// a list of fixed bodies, used to reset the colors
		hkArray<hkpRigidBody*>	m_fixedSmallBodies;

		// a list of collidable/transform pair, just used to display the query,
		// no need to keep that data persistent in a game.
		hkArray<QueryObject>	m_queryObjects;

		// a list of persistent phantoms, which are used speed up collision detection
		// queries by using frame coherency. These phantoms have to be persistent
		hkArray<hkpAabbPhantom*>	    	m_rayCastPhantoms;
		hkArray<hkpSimpleShapePhantom*>	m_simplePhantoms;
		hkArray<hkpCachingShapePhantom*>	m_cachingPhantoms;

		// Landscape utility
		FlatLand						m_groundShapeFactory;

	public:
		hkReal m_time;
};

#endif	// HK_ShapeQueryDemo_H

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
