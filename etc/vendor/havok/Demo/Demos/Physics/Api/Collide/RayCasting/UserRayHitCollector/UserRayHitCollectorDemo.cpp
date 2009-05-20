/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/Physics/Api/Collide/RayCasting/UserRayHitCollector/UserRayHitCollectorDemo.h>

/// Need some shapes
#include <Physics/Collide/Shape/Convex/Box/hkpBoxShape.h>
#include <Physics/Collide/Shape/Query/hkpRayHitCollector.h>

/// We need to display the results of the raycast to confirm correctness.
#include <Common/Visualize/hkDebugDisplay.h>

#include <Physics/Collide/Shape/Query/hkpShapeRayCastInput.h>
#include <Physics/Collide/Query/CastUtil/hkpWorldRayCastOutput.h>


UserRayHitCollectorDemo::UserRayHitCollectorDemo(hkDemoEnvironment* env)
	:	hkDefaultPhysicsDemo(env), 
		m_time(0.0f)
{

	//	
	// Setup the camera.
	//
	{
		hkVector4 from(-8.0f, 25.0f, 20.0f);
		hkVector4 to  (0.0f, 0.0f, 0.0f);
		hkVector4 up  (0.0f, 1.0f, 0.0f);
		setupDefaultCameras(env, from, to, up);
	}



	//
	// Create the world.
	//
	{
		hkpWorldCinfo info;
		info.setBroadPhaseWorldSize( 100.0f );

		m_world = new hkpWorld(info);
		m_world->lock();

		setupGraphics();
	}

	//
	//	Create a row of colored rigid bodies
	//
	{
		hkVector4 halfExtents(.5f, .5f, .5f );
		hkpShape* shape = new hkpBoxShape( halfExtents , 0 );

		int colors[4] = { hkColor::RED, hkColor::GREEN, hkColor::BLUE, hkColor::YELLOW };
		for (int i = 0; i < 4; i++ )
		{
			hkpRigidBodyCinfo ci;
			ci.m_motionType = hkpMotion::MOTION_FIXED;
			ci.m_shape = shape;
			ci.m_collisionFilterInfo = hkpGroupFilter::calcFilterInfo(i+1);
			ci.m_position.set( i * 3.0f, 0.f, 0.0f);

			hkpRigidBody* body = new hkpRigidBody( ci );

			body->addProperty( COLOR_PROPERTY_ID, colors[i] );
			m_world->addEntity(body);
			body->removeReference();

			// show the objects in nice transparent colors
			colors[i] &= ~hkColor::rgbFromChars( 0, 0, 0 );
			colors[i] |= hkColor::rgbFromChars( 0, 0, 0, 120 );
			HK_SET_OBJECT_COLOR((hkUlong)body->getCollidable(), colors[i]);
		}
		shape->removeReference();
	}

	m_world->unlock();
}

UserRayHitCollectorDemo::~UserRayHitCollectorDemo()
{
}

class UserRayHitCollector: public hkpRayHitCollector
{
	public:
		UserRayHitCollector( hkpWorldRayCastInput& ray ): m_ray(ray), m_lastHitFraction(0), m_lastColor( hkColor::WHITE ) {;}

	protected:
		//
		//	This collector draws a collered line from the last hit to the new hit.
		//  Nicely, the broadphase raycaster will sort all the objects, so our ray
		//  will hit in the correct order. However relying on the broadphase sorting
		//  does not work, if big objects, like landscape are involved.
		//
		virtual void addRayHit( const hkpCdBody& cdBody, const hkpShapeRayCastCollectorOutput& hitInfo )
		{
			//
			//	Display the current hit from the last intersection point
			//
			hkVector4 lastHit;	lastHit.setInterpolate4( m_ray.m_from, m_ray.m_to, m_lastHitFraction );
			hkVector4 hitPoint;	hitPoint.setInterpolate4( m_ray.m_from, m_ray.m_to, hitInfo.m_hitFraction );
			m_lastHitFraction = hitInfo.m_hitFraction;
			
			hkpWorldObject *object = static_cast<hkpWorldObject*>( cdBody.getRootCollidable()->getOwner() );

			HK_DISPLAY_LINE( lastHit, hitPoint, m_lastColor);
			m_lastColor = object->getProperty( UserRayHitCollectorDemo::COLOR_PROPERTY_ID ).getInt();
		}
	public:
		void displayFinalRay( )
		{
			hkVector4 lastHit;	lastHit.setInterpolate4( m_ray.m_from, m_ray.m_to, m_lastHitFraction );
			HK_DISPLAY_LINE( lastHit, m_ray.m_to, m_lastColor);
		}

	protected:
		hkpWorldRayCastInput m_ray;
		hkReal				m_lastHitFraction;
		int			m_lastColor;
};

hkDemo::Result UserRayHitCollectorDemo::stepDemo()
{
	//
	// shoot two times four colored rays.
	//  1. each ray should only hit the object with the same color
	//  2. each ray should only hit the triangles (which are at the same x-coord as the object)
	//
	m_time += m_timestep;

	{
		//
		//	Setup the ray coordinates
		//
		hkpWorldRayCastInput ray;
		{
			ray.m_from.set(-4.f, 0.f, 0.0f );
			ray.m_to.set ( 25.f, 0.f, 2.5f * hkMath::cos(m_time * 0.5f));
		}

		//
		//	Do the raycaster
		//
		{
			UserRayHitCollector collector(ray);
			m_world->lock();
			m_world->castRay( ray, collector );
			m_world->unlock();
			collector.displayFinalRay();
		}
	}

	return hkDefaultPhysicsDemo::stepDemo();
}



////////////////////////////////////////////////////////////////////
#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
"This demo illustrates the use of implementing a custom hkpRayHitCollector. " \
"The collector changes the ray color whenever it hits an object.";

HK_DECLARE_DEMO(UserRayHitCollectorDemo, HK_DEMO_TYPE_PRIME, "Raycast filtering using a hit collector", helpString);

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
