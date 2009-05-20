/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


#include <Demos/demos.h>
#include <Demos/Physics/Api/Collide/AsynchronousQueries/ShapeQuery/ShapeQueryDemo.h>

#include <Common/Base/Algorithm/PseudoRandom/hkPseudoRandomGenerator.h>

#include <Common/Base/Monitor/hkMonitorStream.h>

#include <Common/Base/Math/Vector/hkVector4Util.h>

// We will need these shapes
#include <Physics/Collide/Shape/Convex/Triangle/hkpTriangleShape.h>
#include <Physics/Collide/Shape/Convex/Capsule/hkpCapsuleShape.h>

#include <Physics/Collide/Agent/hkpCollisionAgentConfig.h>
#include <Physics/Collide/Agent/hkpProcessCollisionInput.h>

#include <Physics/Collide/Dispatch/hkpCollisionDispatcher.h>

#include <Physics/Collide/Query/Collector/PointCollector/hkpClosestCdPointCollector.h>
#include <Physics/Collide/Query/Collector/PointCollector/hkpAllCdPointCollector.h>
#include <Physics/Collide/Query/Collector/BodyPairCollector/hkpAllCdBodyPairCollector.h>
#include <Physics/Collide/Query/Collector/RayCollector/hkpClosestRayHitCollector.h>

#include <Physics/Collide/Query/CastUtil/hkpLinearCastInput.h>
#include <Physics/Collide/Query/CastUtil/hkpWorldRayCastInput.h>



// Needed to create planes of convex hull for hkpConvexVerticesShape
#include <Common/Base/hkBase.h>
#include <Common/Internal/ConvexHull/hkGeometryUtility.h>

#include <Physics/Dynamics/Phantom/hkpAabbPhantom.h>

#include <Physics/Dynamics/Phantom/hkpSimpleShapePhantom.h>
#include <Physics/Dynamics/Phantom/hkpCachingShapePhantom.h>

// We need to display the results of the raycast to confirm correctness.
#include <Common/Visualize/hkDebugDisplay.h>
#include <Physics/Utilities/VisualDebugger/Viewer/hkpShapeDisplayBuilder.h>
#include <Physics/Utilities/VisualDebugger/Viewer/Dynamics/hkpPhantomDisplayViewer.h>
#include <Common/Visualize/Shape/hkDisplayGeometry.h>

// Needed for legend
#include <Graphics/Common/Window/hkgWindow.h>
#include <Demos/DemoCommon/DemoFramework/hkTextDisplay.h>


// Number of query repetitions (for better timing info)
#if defined(HK_DEBUG)  || defined( HK_PS2 )
#	define NUM_ITER 1
#else
#	define NUM_ITER 10
#endif

// Macro to allow to separate timers for the different query types
#define MY_TIMER_BEGIN(index)	switch(index)	{		\
					case 0:		HK_TIMER_BEGIN("SPHERE", HK_NULL); break;	\
					case 1:		HK_TIMER_BEGIN("CAPSULE", HK_NULL); break;	\
					case 2:		HK_TIMER_BEGIN("BOX", HK_NULL); break;		\
					case 3:		HK_TIMER_BEGIN("CONVEX", HK_NULL); break;	\
					default:	HK_TIMER_BEGIN("UNKNOWN", HK_NULL); break;	};

#define MY_TIMER_END HK_TIMER_END



// Definitions of variants
struct ShapeQueryVariant
{
	const char*	m_name;
	const char* m_details;
};

static const ShapeQueryVariant g_variants[] =
{
{ "Raycasts", "Raycasts which use an AABB phantom for frame coherence (orange rays) perform quicker than world raycasts (green rays) for 'short' distances. Check the timings to illustrate." },
{ "Linear Casts", "Shapes are projected down onto the populated landscape, and points of collision are highlighted. Check the timings to compare query strategies." },
{ "Closest Points", "All closest points and normals of shapes within the (short) collision detection tolerance are highlighted. Check the timings to compare query strategies." },
{ "Penetrations", "Shapes and landscape triangles which penetrate the moving shapes are highlighted. Check the timings to compare query strategies." }
};


// Forward declarations



ShapeQueryDemo::ShapeQueryDemo(hkDemoEnvironment* env)
	:	hkDefaultPhysicsDemo(env), m_time(0)
{

	// Setup the camera.
	{
		hkVector4 from(0.0f, 10.0f, 30.0f);
		hkVector4 to  (0.0f, 0.0f, 0.0f);
		hkVector4 up  (0.0f, 1.0f, 0.0f);
		setupDefaultCameras(env, from, to, up);
	}

	// Create the world.
	{
		hkpWorldCinfo info;
		
		info.m_gravity.set(0.0f, -9.81f, 0.0f);	
		info.setBroadPhaseWorldSize( 100.0f );
		info.m_collisionTolerance = .001f;
		m_world = new hkpWorld(info);
		m_world->lock();

		setupGraphics();
	}

	//	Register collision agents
	hkpAgentRegisterUtil::registerAllAgents( m_world->getCollisionDispatcher() );

	
	//
	// Create the underlying landscape
	//
	hkAabb aabb;
	{
		hkVector4 scaling(1.0f, 0.3f, 1.0f );
		m_groundShapeFactory.setScaling( scaling );
		m_groundShapeFactory.setTriangleRadius( 0.0f );

		hkpMoppBvTreeShape* groundShape = m_groundShapeFactory.createMoppShape();
		{
			hkpRigidBodyCinfo groundInfo;
			groundInfo.m_shape = groundShape;
			groundInfo.m_motionType = hkpMotion::MOTION_FIXED;
			hkpRigidBody* b = new hkpRigidBody(groundInfo);
			
			b->getCollidable()->getShape()->getAabb( b->getTransform(), 0.0f, aabb );

			m_world->addEntity(b);
			b->removeReference();
		}
		groundShape->removeReference();
	}

	//	Create a selection of shapes
	hkArray<hkpShape*> shapes;
	createShapes( shapes );

	// We would like to ensure that each object is placed on the surface of the landscape such
	// that it is not penetrating and is resting on the surface. To do this we make use of world 
	// linear casting which will allow us to shape cast our objects against the landscape.
	// Using the results of this cast we can easily position our objects in the desired manner.
	{
		hkPseudoRandomGenerator random(501);
		int xRes = 8;
		int yRes = 8;
		for (int i = 0; i < xRes * yRes; i ++ )
		{
			// create a fixed rigid body info with a random orientation
			hkpRigidBodyCinfo ci;
			ci.m_shape = shapes[ random.getRandChar( shapes.getSize() ) ];
			ci.m_motionType = hkpMotion::MOTION_FIXED;
			random.getRandomRotation( ci.m_rotation );

			// place it above the plane
			ci.m_position.set(  (1.8f * aabb.m_max(0)) * (-.5f + (i%xRes)/ float(xRes)), 
								aabb.m_max(1) + 10.0f,
								(1.8f * aabb.m_max(2)) * (-.5f + (i/xRes)/ float(yRes))  );
			
			// create the rigid body
			hkpRigidBody* b = new hkpRigidBody(ci);


			// To perform the linear casting we use a hkpLinearCastInput structure and
			// set the end position of the cast (the start position is given by the
			// location of our object). We then create a hkpClosestCdPointCollector
			// to gather the results of the cast and then ask the world to perform
			// the cast:
			{
				hkpLinearCastInput li;
				li.m_to.set( ci.m_position(0), aabb.m_min(1), ci.m_position(2) );

				hkpClosestCdPointCollector hitCollector;
				m_world->linearCast( b->getCollidable(), li, hitCollector );

				//	Check for hits
				if ( hitCollector.hasHit() )
				{
					hkVector4 position; position.setInterpolate4( ci.m_position, li.m_to, hitCollector.getHitContact().getDistance() );
					b->setPosition( position );
				}
			}

			m_world->addEntity( b );
			m_fixedSmallBodies.pushBack( b );
		}
	}

	hkPseudoRandomGenerator generator(3245);

	// Create query objects and phantoms from our shapes
	if (m_variantId == 0)
	{
		// Only AABB phantoms are needed for the raycast variant
		buildAabbPhantoms( m_world, 10, m_rayCastPhantoms, generator );
	}
	else
	{
		// Create query objects and phantoms from shapes
		buildQueryObects( shapes, m_queryObjects );
		buildSimplePhantoms( m_world, shapes, m_simplePhantoms, generator );
		buildCachingPhantoms( m_world, shapes, m_cachingPhantoms, generator );
	}


	//	Remove shape references
	{
		for (int i = 0; i < shapes.getSize(); i++)
		{ 
			shapes[i]->removeReference();
			shapes[i] = 0;
		}
	}

	m_world->unlock();
}


ShapeQueryDemo::~ShapeQueryDemo()
{
	m_world->lock();

	while( m_fixedSmallBodies.getSize() )
	{
		m_fixedSmallBodies[0]->removeReference();
		m_fixedSmallBodies.removeAt(0);
	}

	{
		for ( int i = 0; i < m_queryObjects.getSize(); i++ )
		{
			QueryObject& qo = m_queryObjects[i];
			hkDebugDisplay::getInstance().removeGeometry( (hkUlong)qo.m_collidable, 0, 0 );
			delete qo.m_transform;
			delete qo.m_collidable;
		}
	}

	{
		for ( int i = 0; i < m_rayCastPhantoms.getSize(); i++)
		{
			m_rayCastPhantoms[i]->removeReference();
		}
	}
	{
		for ( int i = 0; i < m_simplePhantoms.getSize(); i++)
		{
			m_simplePhantoms[i]->removeReference();
		}
	}
	{
		for ( int i = 0; i < m_cachingPhantoms.getSize(); i++)
		{
			m_cachingPhantoms[i]->removeReference();
		}
	}

	m_world->unlock();
}




/////////////////////////////////////////////////////////////////
//	Raycasts
/////////////////////////////////////////////////////////////////

// Note that we make several calls to casting functions in this demo to
// better measure algorithmic quality. We first compare raycasts in the
// broadphase world against raycasts using AABB phantoms for frame coherency.

void ShapeQueryDemo::worldRayCast( hkpWorld* world, hkReal time, hkBool useCollector )
{
	const int N_RAY_CASTS = 10;

	hkpWorldRayCastInput  rayInputs[N_RAY_CASTS];
	hkpWorldRayCastOutput rayOutputs[N_RAY_CASTS];
	hkpClosestRayHitCollector rayCollector[ N_RAY_CASTS ];

	// We may also choose whether or not we wish to use a collector or output structure to
	// gather the results of the query (hkpWorld::castRay has two overloaded methods).

	hkVector4 displacement( 0.0f, -30.0f, 0.0f );

	//	Perform all queries in one go (do not interleave with examining the results,
	//  as this is bad for code and data cache.
	{
		for ( int i = 0; i < N_RAY_CASTS; i++ )
		{
			hkpWorldRayCastInput& in = rayInputs[i];

			// create a new position: all objects move in a circle
			hkReal t = time + (HK_REAL_PI * 2 * i) / N_RAY_CASTS;
			hkVector4 pos( hkMath::sin( t ) * 8.0f, 10.0f, hkMath::cos( t ) * 8.0f );

			//	Set our position in our input structure
			in.m_from = pos;
			in.m_to.setAdd4( pos, displacement );

			//
			// Query for intersecting objects. monitor the timing.
			//
			for (int k = 0; k < NUM_ITER ; k++ )
			{
				HK_TIMER_BEGIN("raycast", HK_NULL);
				if ( useCollector )
				{
					hkpClosestRayHitCollector& collector = rayCollector[i];
					collector.reset();
					world->castRay( in, collector );
				}
				else
				{
					hkpWorldRayCastOutput& out = rayOutputs[i];
					out.reset();
					world->castRay( in, out );
				}
				HK_TIMER_END( );
			}
		}
	}

	// Once we've cast our rays all that remains to do is display them!
	// We can do this by using both the input structure (to find the
	// start position) and the output structure (which in the case
	// of using collectors uses the collector's internal
	// hkpWorldRayCastOutput structure) to find the end point.
	// We plot N_RAY_CAST casts, one for each iteration, in a GREEN colour.
	{
		for ( int i = 0; i < N_RAY_CASTS; i++ )
		{
			hkpWorldRayCastInput& in = rayInputs[i];

			const hkpWorldRayCastOutput* out;
			if ( useCollector )
			{
				 out = &rayCollector[i].getHit();
			}
			else
			{
				out = &rayOutputs[i];
			}

			//	Display the ray
			{
				hkVector4 hitPoint;	hitPoint.setInterpolate4( in.m_from, in.m_to, out->m_hitFraction );
				HK_DISPLAY_LINE(in.m_from, hitPoint, hkColor::GREEN);
			}

			//	Display the result
			if ( out->hasHit() )
			{
				displayRayHit( world, in, *out );
			}
		}
	}
}

// Raycasting for phantoms operates in a similar manner with one small adjustment and that is that we
// must move our phantom so that it encapsulates the ray. We can do this as follows:

void ShapeQueryDemo::phantomRayCast( hkpWorld* world, hkReal time, hkArray<hkpAabbPhantom*>& phantoms, hkBool useCollector )
{
	const int N_RAY_CASTS = 10;

	hkpWorldRayCastInput  rayInputs[N_RAY_CASTS];
	hkpWorldRayCastOutput rayOutputs[N_RAY_CASTS];
	hkpClosestRayHitCollector rayCollector[ N_RAY_CASTS ];

	hkVector4 displacement( 0.0f, -30.0f, 0.0f );

	//	Perform all queries in one go (do not interleave with examining the results,
	//  as this is bad for code and data cache.
	{
		for ( int i = 0; i < N_RAY_CASTS; i++ )
		{
			hkpWorldRayCastInput& in = rayInputs[i];

			// create a new position: all objects move in a circle
			hkReal t = time + (HK_REAL_PI * 2 * i) / N_RAY_CASTS;
			hkVector4 pos( hkMath::sin( t ) * 12.0f, 10.0f, hkMath::cos( t ) * 12.0f );

			//	Set our position in our input structure
			in.m_from = pos;
			in.m_to.setAdd4( pos, displacement );


			//
			// Query for intersecting objects
			//
			for (int k = 0; k < NUM_ITER ; k++ )
			{
				HK_TIMER_BEGIN("raycast", HK_NULL);

				//	Move our phantom so it encapsulates our ray
				hkpAabbPhantom* phantom = phantoms[i];
				{
					hkAabb aabb;
					aabb.m_min.setMin4( in.m_to, in.m_from );
					aabb.m_max.setMax4( in.m_to, in.m_from );
					phantom->setAabb( aabb );
				}

				if ( useCollector )
				{
					hkpClosestRayHitCollector& collector = rayCollector[i];
					collector.reset();
					phantom->castRay( in, collector );
				}
				else
				{
					hkpWorldRayCastOutput& out = rayOutputs[i];
					out.reset();
					phantom->castRay( in, out );
				}
				HK_TIMER_END( );
			}
		}
	}

	//
	//	Batch display the results
	//	NOTE that phantom displays have been forced off for this variant.
	//
	{
		for ( int i = 0; i < N_RAY_CASTS; i++ )
		{
			hkpWorldRayCastInput& in = rayInputs[i];

			const hkpWorldRayCastOutput* out;
			if ( useCollector )
			{
				 out = &rayCollector[i].getHit();
				 hkVector4 off(.1f,0,0);
				 in.m_from.add4( off );
				 in.m_to.add4( off );
			}
			else
			{
				out = &rayOutputs[i];
			}
			
			//	Display the ray
			{
				hkVector4 hitPoint;	hitPoint.setInterpolate4( in.m_from, in.m_to, out->m_hitFraction );
				HK_DISPLAY_LINE(in.m_from, hitPoint, hkColor::rgbFromChars( 240, 200, 0, 200 ));
			}

			//	Display the result
			if ( out->hasHit() )
			{
				displayRayHit( world, in, *out );
			}
		}
	}
}




/////////////////////////////////////////////////////////////////
//	Linear casts
/////////////////////////////////////////////////////////////////

// The next casting operation we'll be trying is a linear or shape cast. Once again
// we perform several casts in a batch to obtain a truer measure of the algorithmic
// quality. Since we're using shapes to cast this time we'll be using the collection
// of 'QueryObjects' we constructed earlier. If you recall each 'QueryObject' has a
// hkpCollidable* and hkMotionState*.

void ShapeQueryDemo::worldLinearCast( hkpWorld* world, hkReal time, hkArray<QueryObject>& queryObjects )
{
	// For this cast we use a hkpClosestCdPointCollector to gather the results:
	hkpClosestCdPointCollector collector[10];

	// Since we're not too concerned with perfect accuracy, we can set the early out
	// distance to give the algorithm a chance to exit more quickly:
	world->getCollisionInput()->m_config->m_iterativeLinearCastEarlyOutDistance = 0.1f;
	
	// Cast direction
	hkVector4 displacement( 0.0f, -30.0f, 0.0f );

	// Perform all queries in one go (do not interleave with examining the results,
	// as this is bad for code and data cache.

	// The core loop is very similar to the one we described for the world ray cast,
	// with several casts being performed forming a circular pattern. The only
	// real difference this time is that we get the shape to cast (and the 'from'
	// position) from our collection of 'QueryObjects'.
	{

		for ( int i = 0; i < queryObjects.getSize(); i++ )
		{
			QueryObject& qo = queryObjects[i];

			// Create a new position: all objects move in a circle
			hkReal t = time + HK_REAL_PI * 2 * i / queryObjects.getSize();
			hkVector4 pos( hkMath::sin( t ) * 10.0f, 10.0f, hkMath::cos( t ) * 10.0f );

			// Set our position in our motion state
			qo.m_transform->setTranslation(pos);

			//
			// Query for intersecting objects
			//
			for (int k = 0; k < NUM_ITER ; k++ )
			{
				MY_TIMER_BEGIN(i);
				{
					// Input
					hkpLinearCastInput input;
					input.m_to.setAdd4( pos, displacement );

					// Output
					hkpClosestCdPointCollector& coll = collector[i];
					coll.reset();

					// Call the engine
					world->linearCast( qo.m_collidable, input, coll );
				}
				MY_TIMER_END();
			}
		}
	}

	//
	//	Batch examine the results
	//

	// All that remains to do now is to display the results and update the position of our 'QueryObjects' (QO). We use the updateGeometry(...)
	// method from hkDebugDisplay to do the QO updates and once again we draw everything in GREEN:
	{
		for ( int i = 0; i < queryObjects.getSize(); i++ )
		{
			QueryObject& qo = queryObjects[i];

			// update our display
			if ( collector[i].hasHit() )
			{
				// move our position to the hit and draw a line along the cast direction
				hkVector4& pos = qo.m_transform->getTranslation();
				hkVector4 to; to.setAdd4( pos, displacement );
				hkVector4 newPos; newPos.setInterpolate4( pos, to, collector[i].getHitContact().getDistance() );
				HK_DISPLAY_LINE(pos, newPos, hkColor::GREEN);

				// Update our QO
				qo.m_transform->setTranslation( newPos );
				hkDebugDisplay::getInstance().updateGeometry( qo.m_collidable->getTransform(), (hkUlong)qo.m_collidable, 0);

				// call a function to display the details
				displayRootCdPoint( world, collector[i].getHit() );
			}
		}
	}
}

// Linear casting with phantoms can be achieved in much the same manner as the above.
// However this time we perform the casting operation using the setPositionAndLinearCast(...) method:
template <class T>
static void phantomLinearCast( ShapeQueryDemo* demo, hkArray<T*>& phantoms, hkReal offset )
{
	hkpClosestCdPointCollector collector[10];

	//	We are not really interested in the perfect accuracy, so we give the
	//  algorithm a chance for early outs
	demo->m_world->getCollisionInput()->m_config->m_iterativeLinearCastEarlyOutDistance = 0.1f;

	//	Cast direction
	hkVector4 displacement( 0.0f, -30.0f, 0.0f );

	//	Perform all queries in one go (do not interleave with examining the results,
	//  as this is bad for code and data cache.
	{
		for ( int i = 0; i < phantoms.getSize(); i++ )
		{
			// create a new position: all objects move in a circle
			hkReal t = demo->m_time + HK_REAL_PI * 2 * i / phantoms.getSize() + offset;
			hkVector4 pos( hkMath::sin( t ) * 12.0f, 10.0f, hkMath::cos( t ) * 10.0f );

			//
			// Query for intersecting objects
			//
			for (int k = 0; k < NUM_ITER ; k++ )
			{
				MY_TIMER_BEGIN(i);
				{
					// input
					hkpLinearCastInput input;
					input.m_to.setAdd4( pos, displacement );

					// output
					hkpClosestCdPointCollector& coll = collector[i];
					coll.reset();

					//	Call the engine
					phantoms[i]->setPositionAndLinearCast( pos, input, coll, HK_NULL );

				}
				MY_TIMER_END();
			}
		}
	}

	// As usual for phantoms we need only concern ourselves with updating the display with the results of the
	// linear cast. In this case we draw everything in ORANGE.
	{
		for ( int i = 0; i < phantoms.getSize(); i++ )
		{
			// update our display
			if ( collector[i].hasHit() )
			{
				// move our position to the hit and draw a line along the cast direction
				const hkVector4& pos = phantoms[i]->getTransform().getTranslation();
				hkVector4 to; to.setAdd4( pos, displacement );
				hkVector4 newPos; newPos.setInterpolate4( pos, to, collector[i].getHitContact().getDistance() );
				
				HK_DISPLAY_LINE(pos, newPos, hkColor::rgbFromChars( 240, 200, 0, 200 ));
				phantoms[i]->setPosition( newPos );

				// call a function to display the details
				demo->displayRootCdPoint( demo->m_world, collector[i].getHit() );
			}
		}
	}
}


/////////////////////////////////////////////////////////////////
//	Penetration queries
/////////////////////////////////////////////////////////////////

// This casting query will return a list of all collidables that overlap with our 'QueryObject' (QO).
// Since we would like to know all bodies that overlap we need a collector to keep a complete
// list of collidables and not just, for example, the closest. A collector that will do this is the hkpAllCdBodyPairCollector,
// and again we need several of them, one for each iteration:

void ShapeQueryDemo::worldGetPenetrations( hkpWorld* world, hkReal time, hkArray<QueryObject>& queryObjects )
{
	hkpAllCdBodyPairCollector collector[10];

	// Next we batch query the system (that is ask for the penetrations N times before processing the results).
	// This follows our usual core loop:
	for ( int i = 0; i < queryObjects.getSize(); i++ )
	{
		QueryObject& qo = queryObjects[i];

		// create a new position: all objects move in a circle
		hkReal t = time + HK_REAL_PI * 2 * i / queryObjects.getSize();
		hkVector4 pos( hkMath::sin( t ) * 10.0f, 0.0f, hkMath::cos( t ) * 10.0f );
		qo.m_transform->setTranslation(pos);

		//
		// Query for intersecting objects
		//
		for (int j = 0; j < NUM_ITER; j++ )
		{
			MY_TIMER_BEGIN(i);
			collector[i].reset();
			world->getPenetrations( qo.m_collidable, *world->getCollisionInput(), collector[i] );
			MY_TIMER_END();
		}
	}

	// Next up is the display update, first we highlight any overlapping triangles or objects:
	{
		for ( int i = 0; i < queryObjects.getSize(); i++ )
		{
			// iterate over each individual hit
			for (int j = 0; j < collector[i].getHits().getSize(); j++ )
			{
				displayRootCdBody( world, collector[i].getHits()[j].m_rootCollidableB, collector[i].getHits()[j].m_shapeKeyB );
			}
		}
	}

	// Followed by a geometry update for our QO:
	{
		for ( int i = 0; i < queryObjects.getSize(); i++ )
		{
			QueryObject& qo = queryObjects[i];
			hkDebugDisplay::getInstance().updateGeometry( qo.m_collidable->getTransform(), (hkUlong)qo.m_collidable, 0);
		}
	}
}


// The code for phantomGetPenetrations(...) is pretty much identical to the above, but instead of using QO we
// use our phantoms. The only significant difference is that we do not have to make any display updates
// for the phantoms as the system will take care of this automatically. For the sake of completeness the core
// loop is presented below:

template <class T>
static void phantomGetPenetrations( ShapeQueryDemo* demo, hkArray<T*>& phantoms, hkReal offset )
{
	hkpAllCdBodyPairCollector collector[10];

	//	Perform all queries in one go (do not interleave with examining the results,
	//  as this is bad for code and data cache.
	{
		for ( int i = 0; i < phantoms.getSize(); i++ )
		{
			// create a new position: all objects move in a circle
			hkReal t = demo->m_time + HK_REAL_PI * 2 * i / phantoms.getSize() + offset;
			hkVector4 pos( hkMath::sin( t ) * 10.0f, 0.0f, hkMath::cos( t ) * 10.0f );

			phantoms[i]->setPosition(pos);

			// Query for intersecting objects
			for (int j = 0; j < NUM_ITER; j++ )
			{
				MY_TIMER_BEGIN(i);
				collector[i].reset();
				phantoms[i]->getPenetrations( collector[i] );
				MY_TIMER_END();
			}
		}
	}


	//	Batch display our hits
	{
		for ( int i = 0; i < phantoms.getSize(); i++ )
		{
			// iterate over each individual hit
			for (int j = 0; j < collector[i].getHits().getSize(); j++ )
			{
				demo->displayRootCdBody( demo->m_world, collector[i].getHits()[j].m_rootCollidableB, collector[i].getHits()[j].m_shapeKeyB );
			}
		}
	}

	// Finally no need to update the phantoms, as the graphics engine picks up phantoms automatically.
}




/////////////////////////////////////////////////////////////////
//	Closest point queries
/////////////////////////////////////////////////////////////////

// This function is very similar to the previous one and again we'll be using the hkpAllCdPointCollector to
// gather the results of our query.

void ShapeQueryDemo::worldGetClosestPoints( hkpWorld* world, hkReal time, hkArray<QueryObject>& queryObjects )
{
	hkpAllCdPointCollector collector[10];

	// We setup the usual loop and call the getClosestPoints(...) method:
	{
		for ( int i = 0; i < queryObjects.getSize(); i++ )
		{
			QueryObject& qo = queryObjects[i];

			// create a new position: all objects move in a circle
			hkReal t = time + HK_REAL_PI * 2 * i / queryObjects.getSize();
			hkVector4 pos( hkMath::sin( t ) * 10.0f, 0.0f, hkMath::cos( t ) * 10.0f );
			qo.m_transform->setTranslation(pos);

			// Query for intersecting objects
			for (int j = 0; j < NUM_ITER; j++ )
			{
				MY_TIMER_BEGIN(i);
				collector[i].reset();
				world->getClosestPoints( qo.m_collidable, *world->getCollisionInput(), collector[i] );
				MY_TIMER_END();
			}
		}
	}

	// Updating our display is carried out in the usual manner:
	{
		for ( int i = 0; i < queryObjects.getSize(); i++ )
		{
			// iterate over each individual hit
			for (int j = 0; j < collector[i].getHits().getSize(); j++ )
			{
				displayRootCdPoint( world, collector[i].getHits()[j] );
			}
		}
	}

	{
		for ( int i = 0; i < queryObjects.getSize(); i++ )
		{
			QueryObject& qo = queryObjects[i];
			hkDebugDisplay::getInstance().updateGeometry( qo.m_collidable->getTransform(), (hkUlong)qo.m_collidable, 0);
		}
	}
}

// The good news is that the phantom version of the code is very similar! With the lack of geometry updates being
// the only real exception. The inner loop is given below:

template <class T>
static void phantomGetClosestPoints( ShapeQueryDemo* demo, hkArray<T*>& phantoms, hkReal offset )
{
	hkpAllCdPointCollector collector[10];

	//	Perform all queries in one go (do not interleave with examining the results,
	//  as this is bad for code and data cache.
	{
		for ( int i = 0; i < phantoms.getSize(); i++ )
		{
			// create a new position: all objects move in a circle
			hkReal t = demo->m_time + HK_REAL_PI * 2 * i / phantoms.getSize() + offset;
			hkVector4 pos( hkMath::sin( t ) * 10.0f, 0.0f, hkMath::cos( t ) * 10.0f );
			phantoms[i]->setPosition(pos);

			// Query for intersecting objects
			for (int j = 0; j < NUM_ITER; j++ )
			{
				MY_TIMER_BEGIN(i);
				collector[i].reset();
				phantoms[i]->getClosestPoints( collector[i] );
				MY_TIMER_END();
			}
		}
	}

	//	Batch update our display
	{
		for ( int i = 0; i < phantoms.getSize(); i++ )
		{
			// iterate over each individual hit
			for (int j = 0; j < collector[i].getHits().getSize(); j++ )
			{
				demo->displayRootCdPoint( demo->m_world, collector[i].getHits()[j] );
			}
		}
	}
}




/////////////////////////////////////////////////////////////////
//	Step demo
/////////////////////////////////////////////////////////////////

hkDemo::Result ShapeQueryDemo::stepDemo()
{
	m_world->lock();

	//	Reset all object colors
	{
		for (int i = 0; i < m_fixedSmallBodies.getSize(); i++)
		{
			HK_SET_OBJECT_COLOR((hkUlong)m_fixedSmallBodies[i]->getCollidable(), hkColor::rgbFromChars(70,70,70));
		}
	}

	//
	//	Perform queries for the selected variant
	//
	switch (m_variantId)
	{

	// Raycast
	case 0:
		{
			HK_MONITOR_PUSH_DIR("hkpWorld::rayCastWithCollector");
			worldRayCast( m_world, m_time, true );
			HK_MONITOR_POP_DIR();

			HK_MONITOR_PUSH_DIR("hkpWorld::rayCast");
			worldRayCast( m_world, m_time, false );
			HK_MONITOR_POP_DIR();
		}
		{
			HK_MONITOR_PUSH_DIR("hkpPhantom::rayCastWithCollector");
			phantomRayCast( m_world, m_time, m_rayCastPhantoms, true );
			HK_MONITOR_POP_DIR();

			HK_MONITOR_PUSH_DIR("hkpPhantom::rayCast");
			phantomRayCast( m_world, m_time, m_rayCastPhantoms, false );
			HK_MONITOR_POP_DIR();
		}
		break;

	// Linear cast
	case 1:	
		{
			HK_MONITOR_PUSH_DIR("linearCast (world)");
			worldLinearCast( m_world, m_time, m_queryObjects );
			HK_MONITOR_POP_DIR();
		}
		{
			HK_MONITOR_PUSH_DIR("linearCast (simplePhantom)");
			phantomLinearCast<hkpSimpleShapePhantom>( this, m_simplePhantoms, HK_REAL_PI*0.16f );
			HK_MONITOR_POP_DIR();
		}
		{
			HK_MONITOR_PUSH_DIR("linearCast (cachingPhantom)");
			phantomLinearCast<hkpCachingShapePhantom>( this, m_cachingPhantoms, HK_REAL_PI*0.33f );
			HK_MONITOR_POP_DIR();
		}
		break;

	// Closest point
	case 2:
		{
			HK_MONITOR_PUSH_DIR("getClosestPoints (world)");
			worldGetClosestPoints( m_world, m_time, m_queryObjects );
			HK_MONITOR_POP_DIR();
		}
		{
			HK_MONITOR_PUSH_DIR("getClosestPoints (simplePhantom)");
			phantomGetClosestPoints<hkpSimpleShapePhantom>( this, m_simplePhantoms, HK_REAL_PI*0.16f );
			HK_MONITOR_POP_DIR();
		}
		{
			HK_MONITOR_PUSH_DIR("getClosestPoints (cachingPhantom)");
			phantomGetClosestPoints<hkpCachingShapePhantom>( this, m_cachingPhantoms, HK_REAL_PI*0.33f );
			HK_MONITOR_POP_DIR();
		}
		break;

	// Penetrations
	case 3:
		{
			HK_MONITOR_PUSH_DIR("getPenetrations (world)");
			worldGetPenetrations( m_world, m_time, m_queryObjects );
			HK_MONITOR_POP_DIR();
		}
		{
			HK_MONITOR_PUSH_DIR("getPenetrations (simplePhantom)");
			phantomGetPenetrations<hkpSimpleShapePhantom>(this, m_simplePhantoms, HK_REAL_PI*0.16f );
			HK_MONITOR_POP_DIR();
		}
		{
			HK_MONITOR_PUSH_DIR("getPenetrations (cachingPhantom)");
			phantomGetPenetrations<hkpCachingShapePhantom>(this, m_cachingPhantoms, HK_REAL_PI*0.33f );
			HK_MONITOR_POP_DIR();
		}
		break;

	// Unknown variant
	default:
		HK_ASSERT2(0xf7420c20, false, "Unknown shape query variant");
		break;
	}
	

	// Legend
	if (m_variantId != 0)
	{
		const int h = m_env->m_window->getHeight();
		m_env->m_textDisplay->outputText("Green: World queries", 20, h-70, (hkUint32)hkColor::GREEN, 1);
		m_env->m_textDisplay->outputText("Blue: Simple phantom queries", 20, h-50, (hkUint32)hkColor::BLUE, 1);
		m_env->m_textDisplay->outputText("Red: Caching phantom queries", 20, h-30, (hkUint32)hkColor::RED, 1);
	}

	m_time += 0.005f;

	m_world->unlock();

	return hkDefaultPhysicsDemo::stepDemo();
}





/////////////////////////////////////////////////////////////////
//	Misc.
/////////////////////////////////////////////////////////////////


// Override normal physics demo context
void ShapeQueryDemo::setupContexts(hkArray<hkProcessContext*>& contexts)
{
	// Setup the normal context
	hkDefaultPhysicsDemo::setupContexts(contexts);

	// Remove any phantom display viewers for the *raycast* variant.
	// They confuse the picture.
	if (m_variantId == 0)
	{
		while (1)
		{
			int i = m_debugViewerNames.indexOf(hkpPhantomDisplayViewer::getName());
			if(i == -1) break;
			m_debugViewerNames.removeAt(i);
		}
	}
}

// Before we discuss the various display methods found in this demo we'll mention why there
// are a lack of phantom display methods. The reason is simply that the engine will automatically
// display phantoms by default so there is no need to explicitly draw them! Simple as that!
//
// The main display method is called displayRootCdBody(...). It will highlight any object that is resting
// on the landscape in a light shade of grey and outline any triangles in the landscape MOPP that are
// touched. It decides whether to highlight the object or outline triangles based on the shape key
// passed to it:

void ShapeQueryDemo::displayRootCdBody( hkpWorld* world, const hkpCollidable* collidable, hkpShapeKey key)
{
	hkpShapeCollection::ShapeBuffer shapeBuffer;
	
	//	Check, whether we have a triangle from the landscape or a single object
	if ( key == HK_INVALID_SHAPE_KEY )
	{
		// now we have a single object as we are not part of a hkpShapeCollection
		HK_SET_OBJECT_COLOR((hkUlong)collidable, hkColor::rgbFromChars(160,160,160));
	}
	else
	{
		// now we need to get our triangle.
		// Attention: The next lines make certain assumptions about how the landscape is constructed:
		//   1. The landscape is a single hkMoppShape with wraps a hkpShapeCollection, which
		//      produces only triangles.
		//   2. All hkShapeCollections are wrapped with a hkpBvTreeShape

		HK_ASSERT(0x3e93321f,  world->getCollisionDispatcher()->hasAlternateType( collidable->getShape()->getType(), HK_SHAPE_BV_TREE ) );

		const hkpBvTreeShape* bvTreeShape = static_cast<const hkpBvTreeShape*>(collidable->getShape());

		hkpShapeKey triangleId = key;

		const hkpShape* childShape = bvTreeShape->getContainer()->getChildShape( triangleId, shapeBuffer );

		HK_ASSERT(0x23f67112,  childShape->getType() == HK_SHAPE_TRIANGLE );
		const hkpTriangleShape* triangle = static_cast<const hkpTriangleShape*>( childShape );

		//	Draw the border of the triangle
		HK_DISPLAY_LINE(triangle->getVertex(0), triangle->getVertex(1), hkColor::WHITE);
		HK_DISPLAY_LINE(triangle->getVertex(2), triangle->getVertex(1), hkColor::WHITE);
		HK_DISPLAY_LINE(triangle->getVertex(0), triangle->getVertex(2), hkColor::WHITE);

		if ( (const void*)childShape != (const void*)shapeBuffer && shapeBuffer[10] == 0 )
		{
			HK_ASSERT(0x20b8765d, 0);
		}
	}
}


// Given below are two small helper functions that make use of the previous display methods
// described to highlight the affected triangles or objects and either draw the normal
// or the ray itself in YELLOW.

void ShapeQueryDemo::displayRootCdPoint( hkpWorld* world, const hkpRootCdPoint& cp )
{
	displayRootCdBody( world, cp.m_rootCollidableB, cp.m_shapeKeyB );

	// Display contact point
	HK_DISPLAY_ARROW( cp.m_contact.getPosition(), cp.m_contact.getNormal(), hkColor::YELLOW );
}

void ShapeQueryDemo::displayRayHit( hkpWorld* world, const hkpWorldRayCastInput& in, const hkpWorldRayCastOutput& out )
{
	// Display hit shape
	// This demo has a hardcoded knowledge of the shape hierarchy. It could be more generalized
	// by using the hkpShapeContainer interface.
	int key = out.m_shapeKeys[0];
	for(int i = 0; out.m_shapeKeys[i] != HK_INVALID_SHAPE_KEY; ++i )
	{
		key = out.m_shapeKeys[i];
	}
	displayRootCdBody( world, out.m_rootCollidable, key );

	// Display contact point
	{
		hkVector4 pos; pos.setInterpolate4( in.m_from, in.m_to, out.m_hitFraction );
		HK_DISPLAY_ARROW( pos, out.m_normal, hkColor::YELLOW);
	}
}




// Although you may not have noticed it yet, this demo does not add any of the objects
// we use to query the system into the world, and so we must build ourselves display
// geometry so that we may visualise them. This is one of the key features of Havok 2.2;
// the ability to perform various casting operations without the need to have dynamic
// bodies in the scene.
// It should be noted that normally we could just construct the shape and the motionstate
// as variables on the stack when we wish to make queries, but since we need to update our graphics
// we must use persistent objects in this case.
// To create our display geometry we use the hkpShapeDisplayBuilder
// class and supply it with the required data as follows:

void ShapeQueryDemo::buildQueryObects( hkArray<hkpShape*>& shapes, hkArray<QueryObject>& objects )
{
	hkpShapeDisplayBuilder::hkpShapeDisplayBuilderEnvironment env;
	hkpShapeDisplayBuilder builder(env);

	for (int i = 0; i < shapes.getSize(); i++)
	{
		QueryObject qo;
		qo.m_transform = new hkTransform();
		qo.m_transform->setIdentity();

		qo.m_collidable = new hkpCollidable( shapes[i], qo.m_transform );
		objects.pushBack( qo );

		hkArray<hkDisplayGeometry*> displayGeometries;

		builder.buildDisplayGeometries( shapes[i], displayGeometries );
		hkDebugDisplay::getInstance().addGeometry( displayGeometries, hkTransform::getIdentity(), (hkUlong)qo.m_collidable, 0, 0 );

		while( displayGeometries.getSize() )
		{
			delete displayGeometries[0];
			displayGeometries.removeAt(0);
		}

		// Set green color
		HK_SET_OBJECT_COLOR((hkUlong)qo.m_collidable, hkColor::rgbFromChars(0,255,0,120));
	}
}

void ShapeQueryDemo::buildAabbPhantoms( hkpWorld* world, int numPhantoms, hkArray<hkpAabbPhantom*>& phantoms, hkPseudoRandomGenerator& generator )
{
	for (int i = 0; i < numPhantoms; i++)
	{
		hkAabb aabb;
		// create a random position. This is important, otherwise all the phantoms will start in the
		// center of the level and will overlap, causing an N squared worst case problem
		aabb.m_min.set( i * 10.f, 10.f, generator.getRandRange( -20.f, 20.f ));
		aabb.m_max = aabb.m_min;

		hkpAabbPhantom* phantom = new hkpAabbPhantom( aabb );

		phantoms.pushBack( phantom );
		world->addPhantom( phantom );
	}
}

void ShapeQueryDemo::buildSimplePhantoms( hkpWorld* world, hkArray<hkpShape*>& shapes, hkArray<hkpSimpleShapePhantom*>& phantoms, hkPseudoRandomGenerator& generator )
{
	for (int i = 0; i < shapes.getSize(); i++)
	{
		// create a random position. This is important, otherwise all the phantoms will start in the
		// center of the level and will overlap, causing an N squared worst case problem
		hkTransform t;
		t.getRotation().setIdentity();
		t.getTranslation().set( i * 10.f, 10.f, generator.getRandRange( -20.f, 20.f ));

		hkpSimpleShapePhantom* phantom = new hkpSimpleShapePhantom( shapes[i], t );

		phantoms.pushBack( phantom );
		world->addPhantom( phantom );
		
		// Set blue color
		HK_SET_OBJECT_COLOR((hkUlong)phantom->getCollidable( ), hkColor::rgbFromChars(0,0,255,120));
	}
}

void ShapeQueryDemo::buildCachingPhantoms( hkpWorld* world, hkArray<hkpShape*>& shapes, hkArray<hkpCachingShapePhantom*>& phantoms, hkPseudoRandomGenerator& generator )
{
	for (int i = 0; i < shapes.getSize(); i++)
	{
		// create a random position. This is important, otherwise all the phantoms will start in the
		// center of the level and will overlap, causing an N squared worst case problem
		hkTransform t;
		t.getRotation().setIdentity();
		t.getTranslation().set( i * 10.f, 10.f, generator.getRandRange( -20.f, 20.f ));

		hkpCachingShapePhantom* phantom = new hkpCachingShapePhantom( shapes[i], t );

		phantoms.pushBack( phantom );
		world->addPhantom( phantom );
	
		// Set red color
		HK_SET_OBJECT_COLOR((hkUlong)phantom->getCollidable( ), hkColor::rgbFromChars(255,0,0,120));
	}
}


void ShapeQueryDemo::createShapes(hkArray<hkpShape*>&	shapesOut)
{
	const hkReal s = 1.25f;

	// Create Sphere body
	{
		hkpConvexShape* shape = new hkpSphereShape(s);
		shapesOut.pushBack(shape );
	}

	// Create Capsule body
	{
		hkVector4 a(-s * .5f,  0.f,   0.f);
		hkVector4 b( s * .5f,  0.f,   0.f);
		hkpCapsuleShape* shape = new hkpCapsuleShape(a, b, s);
		shapesOut.pushBack(shape );
	}

	// Create Box body
	{
		hkVector4 halfExtents; halfExtents.setAll3( s );
		hkpBoxShape* shape = new hkpBoxShape(halfExtents, 0 );
		shapesOut.pushBack(shape );
	}
	

	// Create ConvexVertices body
	{
		int numVertices = 12;

			// 3 for x,y,z (size of float)
		int stride = 3 * sizeof(float);
		const hkReal t = s * 0.7f;
		const hkReal o = 0.0f;

		float vertices[] = {	 // 4 vertices 
			 -s,  o, -s,
			 -s,  o,  s,
			  s,  o, -s,
			  s,  o,  s,

			  o,  s, -t,
			  o,  s,  t,
			  t,  s,  o,
			 -t,  s,  o, 
			  o, -s, -t,
			  o, -s,  t,
			  t, -s,  o,
			 -t, -s,  o, 
		};
		
		hkpConvexVerticesShape* shape;
		hkArray<hkVector4> planeEquations;
		hkGeometry geom;
		{
			hkStridedVertices stridedVerts;
			{
				stridedVerts.m_numVertices = numVertices;
				stridedVerts.m_striding = stride;
				stridedVerts.m_vertices = vertices;
			}

			hkGeometryUtility::createConvexGeometry( stridedVerts, geom, planeEquations );

			{
				stridedVerts.m_numVertices = geom.m_vertices.getSize();
				stridedVerts.m_striding = sizeof(hkVector4);
				stridedVerts.m_vertices = &(geom.m_vertices[0](0));
			}

			shape = new hkpConvexVerticesShape(stridedVerts, planeEquations);
		}

		shapesOut.pushBack( shape );
	}
}


#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif


static const char helpString[] = "How to asynchronously query the collision detection system.";

HK_DECLARE_DEMO_VARIANT_USING_STRUCT( ShapeQueryDemo, HK_DEMO_TYPE_PRIME, ShapeQueryVariant, g_variants, helpString ); 

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
