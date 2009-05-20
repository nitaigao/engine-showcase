/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/Physics/Api/Collide/Shapes/HeightField/UserHeightField/UserHeightFieldDemo.h>

//
// Need some shapes
//
#include <Physics/Collide/Shape/Convex/Box/hkpBoxShape.h>
#include <Common/Base/Types/Geometry/Aabb/hkAabb.h>

#include <Common/Visualize/hkDebugDisplay.h>
#include <Physics/Utilities/VisualDebugger/Viewer/hkpShapeDisplayBuilder.h>
#include <Common/Visualize/Shape/hkDisplayGeometry.h>
#include <Common/Visualize/Shape/hkDisplaySphere.h>

// This tutorial shows:
// - How to subclass the heightfield interface and implement a custom collision detection with it.
// - How to use a heightfield shape for more that the default 2D grids style landscapes (here we make a hollow sphere).

// Most people think of heightfields as a 2D displacement map that can be used to create a 
// 3D evenly tessellated landscape for instance. This is what the Heightfield shape is 
// intended to support, but heightfields in Havok can be thought of as any shape that 
// can report a distance to a collection of spheres. Those spheres are the fast approximation
// of the object colliding with the shape. It is like that for very fast performance with 
// the traditional displacement map case, but we can exploit that to implement all variety 
// of shapes, in this case a concave, hollow sphere, acting like a dishwasher.

// Here we subclass the HeightFieldShape directly, as opposed to through the SampledHeightFieldShape.
// In that way we don't inherit the assumption that the heightfield is just heights at given x and z coordinates.


HollowSphereShape::HollowSphereShape( hkReal radius ): hkpHeightFieldShape(HK_SHAPE_HEIGHT_FIELD), m_radius(radius) {}

		// This is the main method that we must implement. It will take in a list of 
		// spheres and report back the distance to the surface for each of the spheres.
		// In our case the surface is a sphere itself so we have a very simple computation.
		// If the distance is too much (multiple of the radius), we can just report a large number
		// and not bother to work out the normal. As our surface is always pointing toward the 
		// origin in this example (a hollow sphere) the normal is just the position of the 
		// colliding sphere, negated. The radius is the length of that normal subtracted from the 
		// radius of out hollow sphere.
void HollowSphereShape::collideSpheres( const CollideSpheresInput& input, SphereCollisionOutput* outputArray) const 
{
	const hkSphere*        in = input.m_spheres;
	SphereCollisionOutput* out = outputArray;

	const hkReal sradius = m_radius - input.m_tolerance;
	const hkReal sradius2 = sradius * sradius;
	
	for (int i = input.m_numSpheres-1; i>=0; i--)
	{
		hkReal dist2 = in->getPosition().lengthSquared3();
		//
		//	Check if we are close enough to our surface
		//
		if ( dist2 < sradius2 )
		{
			//
			//	We are far away, just report a big distance
			//
			out[0](3) = m_radius;
		}
		else
		{
			//
			//	Now we are close, so report normal and distance
			//
			hkVector4 normal = in->getPosition();
			const hkReal dist = normal.normalizeWithLength3();
			out->setNeg4( normal );
			out[0](3) = m_radius - dist;
		}
		in++;
		out++;
	}
}

// The the AABB around this sphere

// We just need to implement the rest of the virtual methods passed
// on by the heightfield shape. The two most important are the AABB
// computation in world space, which in our case is trivial, and the 
// shape type identification, which is the normal SHAPE_HEIGHT_FIELD.
void HollowSphereShape::getAabbImpl( const hkTransform& localToWorld, hkReal tolerance, hkAabb& out  ) const
{
	hkVector4 radius4; radius4.setAll( m_radius );
	out.m_min.setSub4( localToWorld.getTranslation(), radius4 );
	out.m_max.setAdd4( localToWorld.getTranslation(), radius4 );
}

hkpShapeType HollowSphereShape::getType() const { return HK_SHAPE_HEIGHT_FIELD; }

// The rest of the virtual methods we will not implement
// for this demo as they are not used, and they are the 
// cast methods used by the ray and linear cast algorithms.

// We do not care about implementing this function in this demo
hkBool HollowSphereShape::castRayImpl( const hkpShapeRayCastInput& input, hkpShapeRayCastOutput& results) const{ return false; }

// We do not care about implementing this function in this demo
void HollowSphereShape::castRayWithCollectorImpl( const hkpShapeRayCastInput& input, const hkpCdBody& cdBody, hkpRayHitCollector& collector ) const {}

// We do not care about implementing this function in this demo
void HollowSphereShape::castSphere( const hkpSphereCastInput& input, const hkpCdBody& cdBody, hkpRayHitCollector& collector ) const {}



// Now that we have a class that represents the shape 
// we want to simulate, we need to setup a demo to test it out.
// Here we subclass the default demo as usual in the demo framework

UserHeightFieldDemo::UserHeightFieldDemo(hkDemoEnvironment* env)
	:	hkDefaultPhysicsDemo(env, DEMO_FLAGS_NO_SERIALIZE)
{

	// We set up a camera so that the demo starts off looking 
	// at the point of interest.
	{
		hkVector4 from(-8.0f, 10.0f, 6.0f);
		hkVector4 to  (0.0f, 0.0f, 0.0f);
		hkVector4 up  (0.0f, 1.0f, 0.0f);
		setupDefaultCameras(env, from, to, up);
	}

	// We create our world in the standard way.
	{
		hkpWorldCinfo info;
		info.setBroadPhaseWorldSize( 100.0f );
		m_world = new hkpWorld(info);
		m_world->lock();

		setupGraphics();
	}

	// We need to register the agents that we will use.
	// Heightfields are part of the standard agents
	// registered using the hkpAgentRegisterUtil, so we
	// will just use that for convenience.
	hkpAgentRegisterUtil::registerAllAgents( m_world->getCollisionDispatcher() );

	// To get make sure our fancy hollow sphere heightfield works, we
	// need to collide some objects with it. Here we just create a heap 
	// of 64 boxes in some stacks somewhere inside our sphere.
	{
		hkVector4 halfExtents; halfExtents.setAll3(.25f);
		hkpShape* shape = new hkpBoxShape( halfExtents , 0 );

		for (int i = 0; i < 64; i++ )
		{
			hkpRigidBodyCinfo ci;
			ci.m_motionType = hkpMotion::MOTION_SPHERE_INERTIA;
			ci.m_shape = shape;
			ci.m_mass = 4.f;
			ci.m_inertiaTensor.setDiagonal( 1.f,1.f,1.f );
			ci.m_position.set( hkReal(i&3)-1.0f, hkReal((i>>2)&3)-1.0f, hkReal((i>>4)&3)-1.0f );

			hkpRigidBody* body = new hkpRigidBody( ci );
			m_world->addEntity(body);
			body->removeReference();
		}
		shape->removeReference();
	}
	
	// Now we make the actual hollow sphere shape, and we choose a radius of 3 here
	{
		hkpShape* heightFieldShape = new HollowSphereShape( 3.0f );
		// We need a rigid body to hold the shape, and we also want it
		// to be a bit more interesting than just sitting there in space,
		// so we will keyframe it in a circular motion by just setting its
		// motion type hkpMotion::MOTION_KEYFRAMED and giving it an angular velocity.
		// To make it more dishwasher-like we will give it a very high friction
		// value so that the boxes inside get dragged around easily.
		{
			hkpRigidBodyCinfo ci;
			ci.m_motionType = hkpMotion::MOTION_KEYFRAMED;
			ci.m_shape = heightFieldShape;
			ci.m_friction = 2.0f;

			m_rigidBody = new hkpRigidBody( ci );

			hkVector4 angVel( 0,0,1);
			m_rigidBody->setAngularVelocity( angVel );

			m_world->addEntity(m_rigidBody);
		}

		// Then we just want to cleanup the last reference we have to
		// the shape we created, and that is it, demo complete.
		heightFieldShape->removeReference();

		// We'll now build geometry for a sphere that will represent the heightfield 
		// (but will not be part of the simulation, i.e. display only)
		{

			// We use a slightly larger radius to compensate for the error in the polygon sphere approximation
			m_sphereDisp = new hkDisplaySphere(hkSphere(hkVector4::getZero(), 3.1f), 10, 10);

			hkArray<hkDisplayGeometry*> displayGeometries;

			displayGeometries.pushBack(m_sphereDisp);
			hkDebugDisplay::getInstance().addGeometry( displayGeometries, hkTransform::getIdentity(), (hkUlong)m_sphereDisp, 0 , 0);
			HK_SET_OBJECT_COLOR((hkUlong)m_sphereDisp, hkColor::rgbFromChars(255, 255, 255, 100));
		}
	}

	m_world->unlock();
}

UserHeightFieldDemo::~UserHeightFieldDemo()
{
	m_world->lock();

	// Tidy up the geometry
	hkDebugDisplay::getInstance().removeGeometry( (hkUlong)m_sphereDisp, 0, 0);

	delete m_sphereDisp;

	m_rigidBody->removeReference();

	m_world->unlock();
}


hkDemo::Result UserHeightFieldDemo::stepDemo()
{
	// We must update our display representation for the heightfield 'manually' as it is not in the simulation

	// Update the transform for the display representation from the heightfield and update the display
	{
		hkDebugDisplay::getInstance().updateGeometry( m_rigidBody->getTransform(), (hkUlong)m_sphereDisp, 0);
	}

	return hkDefaultPhysicsDemo::stepDemo();
}



////////////////////////////////////////////////////////////////////
#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
"This demo shows how to implement your own procedural heightfield shape. " \
"To keep things simple, our implementation is a simple hollow " \
"sphere filled with boxes. The heightfield is keyframed to rotate. ";

HK_DECLARE_DEMO( UserHeightFieldDemo, HK_DEMO_TYPE_PRIME, "A custom procedural heightfield shape", helpString );

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
