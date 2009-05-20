/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


#include <Demos/demos.h>
#include <Physics/Collide/Query/Collector/RayCollector/hkpClosestRayHitCollector.h>
#include <Demos/Physics/Api/Collide/RayCasting/OptimizedWorldRayCast/OptimizedWorldRaycastDemo.h>

#include <Common/Base/Monitor/hkMonitorStream.h>

#include <Physics/Internal/Collide/BroadPhase/hkpBroadPhase.h>
#include <Physics/Collide/Query/CastUtil/hkpWorldRayCastInput.h>
#include <Physics/Collide/Query/CastUtil/hkpWorldRayCastOutput.h>
#include <Physics/Collide/Query/CastUtil/hkpWorldRayCaster.h>


// We need to create a phantom
#include <Physics/Dynamics/Phantom/hkpAabbPhantom.h>

// We need to display the results of the raycast to confirm correctness.
#include <Common/Visualize/hkDebugDisplay.h>
//#include <hkdemoframework/hkDemoFramework.h>

#include <Common/Base/Algorithm/PseudoRandom/hkPseudoRandomGenerator.h>

OptimizedWorldRaycastDemo::OptimizedWorldRaycastDemo(hkDemoEnvironment* env)
:	hkDefaultPhysicsDemo(env)
{
	{
		m_numRigidBodies = int(m_env->m_cpuMhz);
		m_numBroadphaseMarkers = 64;
		m_rayLength = 5;
		m_worldSizeX = 2.0f * hkMath::sqrt(hkReal(m_env->m_cpuMhz));
		m_worldSizeY = 3;
		m_worldSizeZ = m_worldSizeX;
	}
	//
	// Setup the camera.
	//
	{
		hkVector4 from(30.0f, 8.0f, 25.0f);
		hkVector4 to  ( 4.0f, 0.0f, -3.0f);
		hkVector4 up  ( 0.0f, 1.0f,  0.0f);
		setupDefaultCameras(env, from, to, up);

		// Demo is slow graphicaly as it without shadows
		forceShadowState(false);

	}

	//
	// Create the world.
	//
	{
		hkpWorldCinfo info;
		
		// Set gravity to zero so body floats.
		info.m_gravity.setZero4();

		// make the world big enough to hold all our objects
		// make y larger so that our raycasts stay within the world aabb
		info.m_broadPhaseWorldAabb.m_max.set( m_worldSizeX + 10, 10*m_worldSizeY + 10, m_worldSizeZ + 10 );
		info.m_broadPhaseWorldAabb.m_min.setNeg4( info.m_broadPhaseWorldAabb.m_max );

		// Subdivide the broadphase space into equal sections along the x-axis
		// NOTE: Disabling this until the marker crash issue is fixed.
		//info.m_broadPhaseNumMarkers = m_numBroadphaseMarkers;

		
		m_world = new hkpWorld(info);
		m_world->lock();

		setupGraphics();
	}

	// register all agents(however, we put all objects into the some group,
	// so no collision will be detected
	hkpAgentRegisterUtil::registerAllAgents( m_world->getCollisionDispatcher() );

	// Add a collision filter to the world to allow the bodies interpenetrate
	{
		hkpGroupFilter* filter = new hkpGroupFilter();
		filter->disableCollisionsBetween( hkpGroupFilterSetup::LAYER_DEBRIS, hkpGroupFilterSetup::LAYER_DEBRIS );
		m_world->setCollisionFilter( filter );
		filter->removeReference();
	}

	//
	// Set the simulation time to 0
	//
	m_time = 0;


	//
	//	Create our phantom at our origin.
	//  This is a bad hack, we should really create our phantom at it's final position,
	//  but let's keep the demo simple.
	//  If you actually create many phantoms all at the origin, you get a massive CPU spike
	//  as every phantom will collide with every other phantom.
	//
	{
		hkAabb aabb;
		aabb.m_min.setZero4();
		aabb.m_max.setZero4();
		m_phantom = new hkpAabbPhantom( aabb );
		m_world->addPhantom( m_phantom );

		m_phantomUseCache = new hkpAabbPhantom( aabb );
		m_world->addPhantom( m_phantomUseCache );
	}


	//
	// Create some bodies (reuse the ShapeRaycastApi demo)
	//
	createBodies();

	m_world->unlock();
}

OptimizedWorldRaycastDemo::~OptimizedWorldRaycastDemo()
{
	m_world->lock();
	m_phantom->removeReference();
	m_phantomUseCache->removeReference();
	m_world->unlock();
}

hkDemo::Result OptimizedWorldRaycastDemo::stepDemo()
{
	m_world->lock();

	m_time += m_timestep;

	//
	//	Create a number of ray directions
	//
	const int NUM_DIRECTIONS = 8; const hkReal increment = 2.0f;
	//const int NUM_DIRECTIONS = 125; const hkReal increment = 0.5f;

	hkVector4 rayDirections[NUM_DIRECTIONS];
	{
		int i = 0;
		for (hkReal x = -1.0f; x <= 1.0f; x += increment)
		{
			for (hkReal y = -1.0f; y <= 1.0f; y += increment)
			{
				for (hkReal z = -1.0f; z <= 1.0f; z+= increment)
				{
					rayDirections[i++].set(x,y,z);
				}
			}
		}
	}

	
	hkReal angle = 0.5f * m_time;
	
	// In these first chapter we simply call the hkpWorld castRay version with no further optimizations
	{
		angle += HK_REAL_PI * 0.3f;

		hkReal xPos = m_rayLength * 3.0f * hkMath::sin(angle);
		hkReal zPos = m_rayLength * 3.0f * hkMath::cos(angle);

		hkpWorldRayCastInput      inputs[NUM_DIRECTIONS];
		hkpClosestRayHitCollector collectors[NUM_DIRECTIONS];

		HK_TIMER_BEGIN("NotOptimized", HK_NULL);
		{
			for (int i = 0; i < NUM_DIRECTIONS; i++)
			{
				inputs[i].m_from.set( xPos, 0, zPos);
				inputs[i].m_to.setAddMul4(inputs[i].m_from, rayDirections[i], 1.f*m_rayLength);
				m_world->castRay(inputs[i], collectors[i]);
			}
		}
		HK_TIMER_END();
		displayHits( inputs, NUM_DIRECTIONS, collectors, hkColor::RED);
	}
	
	// This second set of raycasts (yellow) is using a the group functionality of the hkpWorldRayCaster.
	// Internally it builds an the aabb cache. This is an optimization for broadphase raycasts. 
	// Note that we need to pass in array of collectors to the hkpWorldRayCaster.
	// As the hkpWorldRayCaster has no clue about the size of each collector, we have to pass in the size.
	// Interesting if we use a size of 0, than all raycast will report to the same collector.
	{
		angle += HK_REAL_PI * 0.3f;

		hkReal xPos = m_rayLength * 3.0f * hkMath::sin(angle);
		hkReal zPos = m_rayLength * 3.0f * hkMath::cos(angle);

		hkpWorldRayCastInput inputs[NUM_DIRECTIONS];
		hkpClosestRayHitCollector collectors[NUM_DIRECTIONS];

		//
		//	Cast Rays
		//
		HK_TIMER_BEGIN("CastRayGroup", HK_NULL);
		{
			for (int i = 0; i < NUM_DIRECTIONS; i++)
			{
				inputs[i].m_from.set( xPos, 0, zPos);
				inputs[i].m_to.setAddMul4(inputs[i].m_from, rayDirections[i], 1.f*m_rayLength);

			}
			hkpWorldRayCaster rayCaster;
			rayCaster.castRayGroup( *m_world->getBroadPhase(), inputs, NUM_DIRECTIONS, m_world->getCollisionFilter(), collectors, sizeof(collectors[0]) ); 
		}
		HK_TIMER_END();
		displayHits( inputs, NUM_DIRECTIONS, collectors, hkColor::CYAN);
	}
	
	// This set of raycasts (yellow) is using a the hkpBroadPhaseAabbCache.
	// So by building the aabb cache. This is an optimization for broadphase raycasts. 
	// The idea is that the aabb cache is actually a reduced broadphase, just storing
	// objects inside the aabb. Therefore using this cache can speed up rayCasts significantly.
	// Unfortunately we cannot use our simple hkpWorld::castRay() function, but have to use
	// a small helper class hkpWorldRayCaster (which is actually used by the hkpWorld::castRay()).
	{
		angle += HK_REAL_PI * 0.3f;

		hkReal xPos = m_rayLength * 3.0f * hkMath::sin(angle);
		hkReal zPos = m_rayLength * 3.0f * hkMath::cos(angle);

		hkpWorldRayCastInput inputs[NUM_DIRECTIONS];
		hkpClosestRayHitCollector collectors[NUM_DIRECTIONS];

		//
		//	Calc Cache
		//
		HK_TIMER_BEGIN_LIST("UseAabbCache", "CalcCache");
		hkpBroadPhaseAabbCache* cache;
		int                    cacheSize;
		{	
			hkAabb aabb;
			aabb.m_min.set( xPos - m_rayLength, -m_rayLength, zPos-m_rayLength);
			aabb.m_max.set( xPos + m_rayLength, +m_rayLength, zPos+m_rayLength);

			cacheSize = m_world->getBroadPhase()->getAabbCacheSize();
			cache     = reinterpret_cast<hkpBroadPhaseAabbCache*>(hkAllocateStack<char>(cacheSize));
			
			m_world->getBroadPhase()->calcAabbCache( aabb, cache );
		}

		//
		//	Cast Rays
		//
		HK_TIMER_SPLIT_LIST("CastRays");
		{
			for (int i = 0; i < NUM_DIRECTIONS; i++)
			{
				inputs[i].m_from.set( xPos, 0, zPos);
				inputs[i].m_to.setAddMul4(inputs[i].m_from, rayDirections[i], 1.f*m_rayLength);

				hkpWorldRayCaster rayCaster;
				rayCaster.castRay( *m_world->getBroadPhase(), inputs[i], m_world->getCollisionFilter(), cache, collectors[i] ); 
			}
		}
		HK_TIMER_END_LIST();
		hkDeallocateStack<char>( (char*)cache);
		displayHits( inputs, NUM_DIRECTIONS, collectors, hkColor::YELLOW);
	}
	
	
	// This third set of raycasts (blue) is using a the hkpBroadPhaseAabbCache and
	// also makes use of the fact that many rays starting at the same position can
	// be handled specially.
	// Unfortunately we cannot use our simple hkpWorld::castRay() function, but have to use
	// a small helper class hkpWorldRayCaster (which is actually used by the hkpWorld::castRay()).
	{
		angle += HK_REAL_PI * 0.3f;

		hkReal xPos = m_rayLength * 3.0f * hkMath::sin(angle);
		hkReal zPos = m_rayLength * 3.0f * hkMath::cos(angle);

		hkpWorldRayCastInput      inputs[NUM_DIRECTIONS];
		hkpClosestRayHitCollector collectors[NUM_DIRECTIONS];

		//
		//	Calc Cache
		//
		HK_TIMER_BEGIN_LIST("CacheSameStart", "CalcCache");
		hkpBroadPhaseAabbCache* cache;
		int                    cacheSize;
		{	
			hkAabb aabb;
			aabb.m_min.set( xPos - m_rayLength, -m_rayLength, zPos-m_rayLength);
			aabb.m_max.set( xPos + m_rayLength, +m_rayLength, zPos+m_rayLength);

			cacheSize = m_world->getBroadPhase()->getAabbCacheSize();
			cache = reinterpret_cast<hkpBroadPhaseAabbCache*>(hkAllocateStack<char>(cacheSize));
			
			m_world->getBroadPhase()->calcAabbCache( aabb, cache );
		}
		
		//
		//	Cast Rays
		//
		HK_TIMER_SPLIT_LIST("CastRays");
		{
			for (int i = 0; i < NUM_DIRECTIONS; i++)
			{
				inputs[i].m_from.set( xPos, 0, zPos);
				inputs[i].m_to.setAddMul4(inputs[i].m_from, rayDirections[i], 1.f*m_rayLength);
			}
			hkpWorldRayCaster rayCaster;
			rayCaster.castRaysFromSinglePoint( *m_world->getBroadPhase(), inputs, NUM_DIRECTIONS, m_world->getCollisionFilter(), cache, collectors, hkSizeOf( collectors[0] ) ); 
		}
		HK_TIMER_END_LIST();
		hkDeallocateStack<char>( (char*)cache);
		displayHits( inputs, NUM_DIRECTIONS, collectors, hkColor::BLUE);
	}

	// This 4th set of raycasts (green) is using a the AabbPhantom
	// A phantom is like a persistent aabb cache, so it only makes sense if our aabb shows some
	// framecoherency. 
	// Unfortunetaly the current phantom implementations simply casts the ray against all objects
	// overlapping with the phantom. Therefor, if too many objects are intersecting the phantoms
	// aabb, performance could get bad.
	{
		angle += HK_REAL_PI * 0.3f;
		hkReal xPos = m_rayLength * 3.0f * hkMath::sin(angle);
		hkReal zPos = m_rayLength * 3.0f * hkMath::cos(angle);

		hkpWorldRayCastInput inputs[NUM_DIRECTIONS];
		hkpClosestRayHitCollector collectors[NUM_DIRECTIONS];

		HK_TIMER_BEGIN_LIST("AabbPhantom", "MovePhantom");
		{	
			hkAabb aabb;
			aabb.m_min.set( xPos - m_rayLength, -m_rayLength, zPos-m_rayLength);
			aabb.m_max.set( xPos + m_rayLength, +m_rayLength, zPos+m_rayLength);

			m_phantom->setAabb( aabb );
		}
		
		HK_TIMER_SPLIT_LIST("CastRays");

		{
			for (int i = 0; i < NUM_DIRECTIONS; i++)
			{
				inputs[i].m_from.set( xPos, 0, zPos);
				inputs[i].m_to.setAddMul4(inputs[i].m_from, rayDirections[i], 1.f*m_rayLength);
				m_phantom->castRay(inputs[i], collectors[i]);
			}
		}

		HK_TIMER_END_LIST();
		displayHits( inputs, NUM_DIRECTIONS, collectors, hkColor::GREEN);
	}

	// fastest (purple), combine all optimizations above
	// We are using a phantom to set up the broadphase cache. This avoids the problem of the hkpPhantom::castRay()
	{
		angle += HK_REAL_PI * 0.3f;
		hkReal xPos = m_rayLength * 3.0f * hkMath::sin(angle);
		hkReal zPos = m_rayLength * 3.0f * hkMath::cos(angle);

		hkpWorldRayCastInput inputs[NUM_DIRECTIONS];
		hkpClosestRayHitCollector collectors[NUM_DIRECTIONS];

		hkpBroadPhaseAabbCache* cache;
		int                    cacheSize;
		HK_TIMER_BEGIN_LIST("AllOpt.", "MovePhantomAndDoCache");
		{	
			hkAabb aabb;
			aabb.m_min.set( xPos - m_rayLength, -m_rayLength, zPos-m_rayLength);
			aabb.m_max.set( xPos + m_rayLength, +m_rayLength, zPos+m_rayLength);

			m_phantomUseCache->setAabb( aabb );

			cacheSize = m_world->getBroadPhase()->getAabbCacheSize();
			cache = reinterpret_cast<hkpBroadPhaseAabbCache*>(hkAllocateStack<char>(cacheSize));
			
			m_world->getBroadPhase()->calcAabbCache( m_phantomUseCache->getOverlappingCollidables(), cache );
		}

		HK_TIMER_SPLIT_LIST("CastRays");

		{
			for (int i = 0; i < NUM_DIRECTIONS; i++)
			{
				inputs[i].m_from.set( xPos, 0, zPos);
				inputs[i].m_to.setAddMul4(inputs[i].m_from, rayDirections[i], 1.f*m_rayLength);
			}
			hkpWorldRayCaster rayCaster;
			rayCaster.castRaysFromSinglePoint( *m_world->getBroadPhase(), inputs, NUM_DIRECTIONS, m_world->getCollisionFilter(), cache, collectors, hkSizeOf( collectors[0] ) ); 
		}

		HK_TIMER_END_LIST();
		hkDeallocateStack<char>( (char*)cache);
		displayHits( inputs, NUM_DIRECTIONS, collectors, hkColor::PURPLE);
	}

	m_world->unlock();

	return hkDefaultPhysicsDemo::stepDemo();
}

void OptimizedWorldRaycastDemo::displayHit( const hkpWorldRayCastInput& input, hkpClosestRayHitCollector& collector, int color) const
{
	// To visualize the raycast we make use of a macro defined in "hkDebugDisplay.h" called HK_DISPLAY_LINE.
	// The macro takes three parameters: a start point, an end point and the line colour.
	// If a hit is found we display a RED line from the raycast start point to the point of intersection and mark that
	// point with a small RED cross. The intersection point is calculated using: startWorld + (result.m_mindist * endWorld).
	//
	// If no hit is found we simply display a GREY line between the raycast start and end points.

	if( collector.hasHit() )
	{
		hkVector4 intersectionPointWorld;
		intersectionPointWorld.setInterpolate4( input.m_from, input.m_to, collector.getHit().m_hitFraction );
		HK_DISPLAY_LINE( input.m_from, intersectionPointWorld, color);
		HK_DISPLAY_ARROW( intersectionPointWorld, collector.getHit().m_normal, color);
	}
	else
	{
		// Otherwise draw full length
		HK_DISPLAY_LINE(input.m_from, input.m_to, color);
	}
}

void OptimizedWorldRaycastDemo::displayHits( const hkpWorldRayCastInput* inputs, int numInputs, hkpClosestRayHitCollector* collectors, int color) const
{
	for (int i = 0; i < numInputs; i++)
	{
		displayHit(inputs[i], collectors[i],color);
	}
}


void OptimizedWorldRaycastDemo::createBodies()
{
	hkpRigidBodyCinfo rigidBodyInfo;
	rigidBodyInfo.m_collisionFilterInfo = hkpGroupFilter::calcFilterInfo(1, 1);
	hkPseudoRandomGenerator rand(100);

	hkArray<hkpEntity*> bodyArray;
	bodyArray.reserve(m_numRigidBodies);

	hkpShape* shape;
	{
		hkVector4 halfExtents(0.5f, 0.5f, 0.5f);
		shape = new hkpBoxShape( halfExtents, 0.0f );
	}

	for( int i = 0; i < m_numRigidBodies; i++)
	{
		// All bodies created below are movable
		rigidBodyInfo.m_motionType = hkpMotion::MOTION_SPHERE_INERTIA;

		// A collection of many rigid bodies is randomly created using a hkpBoxShape
		rigidBodyInfo.m_shape = shape;

		// As usual we fill out the hkpRigidBodyCinfo 'blueprint' for the rigidbody, with the code above specifying
		// the necessary information for the 'm_shape' member. To create a hkpConvexVerticesShape we need a set of vertices and
		// we must generate a set of plane equations from these points. As you can see from the code this is all performed 
		// prior to instantiating the shape.

		// Fake Inertia tensor for simplicity, assume it's a unit cube
		{
			hkReal mass = 10.0f;
			hkReal d = mass * 0.5f;
			rigidBodyInfo.m_inertiaTensor.setDiagonal( d,d,d );
			rigidBodyInfo.m_mass = mass;			
		}	

		// The object is then assigned a random position, orientation and angular velocity and added to the world:

		rigidBodyInfo.m_position.set(	rand.getRandRange(-1.f*m_worldSizeX, 1.f*m_worldSizeX),
										rand.getRandRange(-1.f*m_worldSizeY, 1.f*m_worldSizeY),
										rand.getRandRange(-1.f*m_worldSizeZ, 1.f*m_worldSizeZ));
		rand.getRandomRotation( rigidBodyInfo.m_rotation );

		rigidBodyInfo.m_collisionFilterInfo = hkpGroupFilterSetup::LAYER_DEBRIS;

		hkpRigidBody* rigidBody = new hkpRigidBody(rigidBodyInfo);

		// Give them an intial velocity
		hkVector4 angularVel(rand.getRandRange(-1.0f, 1.0f), rand.getRandRange(-1.0f, 1.0f), rand.getRandRange(-1.0f, 1.0f));
		rigidBody->setAngularVelocity(angularVel);
		rigidBody->setAngularDamping(0.0f);

		bodyArray.pushBack( rigidBody );

		// There is no gravity vector for this world and so the bodies will appear to float in space.
	}
	shape->removeReference();

	// Batch add all bodies to the system and defragment the broadphase
	m_world->addEntityBatch( bodyArray.begin(), bodyArray.getSize() );
	m_world->getBroadPhase()->defragment();
		
	//
	//	Remove all references to bodies. They are now referenced by m_world
	//
	{
		for ( int i = 0; i < bodyArray.getSize(); i++ )
		{
			bodyArray[i]->removeReference();
		}
	}
}


#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
"A demo which shows how to use raycast with the broadphase more efficiently. The current implementation of " \
"the broadphase is exceptional for normal physics use but somehow poor for static queries like world ray casts " \
"this demo shows different techniques to speed up the broadphase significantly. Check the timers to see for yourself."
;

HK_DECLARE_DEMO(OptimizedWorldRaycastDemo, HK_DEMO_TYPE_PRIME | HK_DEMO_TYPE_CRITICAL | HK_DEMO_TYPE_STATS, "Using world raycasts in a more optimized way", helpString);

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
