/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


#include <Demos/demos.h>
#include <Demos/Physics/Api/Collide/RayCasting/RayCastCallback/RaycastCallbackDemo.h>


// Need some shapes
#include <Physics/Collide/Shape/Convex/Box/hkpBoxShape.h>
#include <Physics/Collide/Shape/Convex/Sphere/hkpSphereShape.h>

// Need to get broadPhase from world
#include <Physics/Internal/Collide/BroadPhase/hkpBroadPhase.h>


// We need to display the results of the raycast to confirm correctness.
#include <Common/Visualize/hkDebugDisplay.h>

#include <Physics/Collide/Shape/Query/hkpShapeRayCastInput.h>
#include <Physics/Collide/Query/CastUtil/hkpWorldRayCastOutput.h>



RaycastCallbackDemo::RaycastCallbackDemo(hkDemoEnvironment* env)
	:	hkDefaultPhysicsDemo(env),
		m_time(2.0f)
{

	//
	// Setup the camera.
	//
	{
		hkVector4 from(-20.0f, 25.0f, 20.0f);
		hkVector4 to  (-5.0f, 0.0f, 0.0f);
		hkVector4 up  (0.0f, 1.0f, 0.0f);
		setupDefaultCameras(env, from, to, up);
	}

	//
	// Create the world.
	//
	{
		hkpWorldCinfo info;

		// Set gravity to zero so body floats.
		info.m_gravity.set(0.0f, 0.0f, 0.0f);
		info.setBroadPhaseWorldSize( 100.0f );
		m_world = new hkpWorld(info);
		m_world->lock();

		setupGraphics();
	}
	createBodies();

	m_world->unlock();
}

RaycastCallbackDemo::MyFilteredRayCastCallback::MyFilteredRayCastCallback( const hkpWorldRayCastInput& input, hkpWorldRayCastOutput* output)
{
	m_input = input;
	m_output = output;
}

hkDemo::Result RaycastCallbackDemo::stepDemo()
{
	m_world->lock();

	// At the beginning of each frame all of the objects are initially set to GREY
	// making use of a macro defined in "hkDebugDisplay.h" called HK_SET_OBJECT_COLOR
	// which provides an easy way to change the colour of any given collidable.

	for (int i = 0; i < m_bodies.getSize(); i++)
	{
		HK_SET_OBJECT_COLOR((hkUlong)m_bodies[i]->getCollidable(), hkColor::rgbFromChars(128,128,128));
	}

	// We then cast a ray from a fixed start point to a moving end point (achieved by
	// calling a cosine function with varying parameter) with both points specified in
	// World Space.

	hkpWorldRayCastInput ray;
	{
		m_time += m_timestep / 1.0f;
		const hkReal y = 8.0f * hkMath::cos(m_time);
		ray.m_from.set(-20.f, 0.0f, -2.0f);
		ray.m_to.set( 10, y, y);
	}

	hkpWorldRayCastOutput output;
	{
		MyFilteredRayCastCallback rayCallback( ray, &output );

		hkpBroadPhase* broadPhase = m_world->getBroadPhase();

		hkpBroadPhase::hkpCastRayInput rayInput;
		rayInput.m_from = ray.m_from;
		rayInput.m_toBase = &ray.m_to;
		rayInput.m_aabbCacheInfo = HK_NULL;

		broadPhase->castRay(rayInput, &rayCallback, hkSizeOf(MyFilteredRayCastCallback)  );
	}

	// To visualise the raycast we make use of a macro defined in "hkDebugDisplay.h" called HK_DISPLAY_LINE.
	// The macro takes three parameters: a start point, an end point and the line colour.
	// If a hit is found we display a RED line from the raycast start point to the point of intersection and mark that
	// point with a small RED cross. The intersection point is calculated using: startWorld + (result.m_mindist * endWorld).
	// We also set the object to display in RED, once again making use of the HK_SET_OBJECT_COLOR macro.
	//
	// If no hit is found we simply display a GREY line between the raycast start and end points.

	if ( output.hasHit() )
	{
			hkVector4 intersectionPointWorld;
			intersectionPointWorld.setInterpolate4( ray.m_from, ray.m_to, output.m_hitFraction );
			HK_DISPLAY_LINE( ray.m_from, intersectionPointWorld, hkColor::RED);
			HK_SET_OBJECT_COLOR((hkUlong)output.m_rootCollidable, hkColor::RED);
	}
	else
	{
		// Otherwise draw as WHITE
		HK_DISPLAY_LINE(ray.m_from, ray.m_to, hkColor::WHITE);
	}

	m_world->unlock();

	return hkDefaultPhysicsDemo::stepDemo();
}


// This demo uses a 'grid' of spheres and boxes. These are created in the usual fashion by filling in the
// details for the various members of the hkpRigidBodyCinfo structure.

void RaycastCallbackDemo::createBodies()
{

	//
	// create shapes
	//
	hkpSphereShape* sphereShape;
	hkpBoxShape* boxShape;
	{
		hkVector4 halfExtents = hkVector4(1.0f, 1.0f, 1.0f);
		boxShape = new hkpBoxShape(halfExtents, 0 );

		hkReal radius = 1.0f;
		sphereShape = new hkpSphereShape(radius);
	}

	int count = 0;

	hkpRigidBodyCinfo rigidBodyInfo;
	rigidBodyInfo.m_motionType = hkpMotion::MOTION_FIXED;

	for(int i = -2; i < 2; i++)
	{
		for(int j = -2; j < 2; j++)
		{
			for(int k = -2; k < 2; k++)
			{
				rigidBodyInfo.m_position.set( i * 3.0f, j * 3.0f, k * 3.0f);

				if(i == -2 || count%2)
				{
					rigidBodyInfo.m_shape = boxShape;
				}
				else
				{
					rigidBodyInfo.m_shape = sphereShape;
				}

					hkpRigidBody* rigidBody = new hkpRigidBody(rigidBodyInfo);
					m_world->addEntity(rigidBody);
					m_bodies.pushBack(rigidBody);

				count++;
			}
			count++;
		}
		count++;
	}

	sphereShape->removeReference();
	boxShape->removeReference();

}


// In order to filter the shapes we derive a myFilteredRayCastCallback class from the hkRayCastCallback class
// and implement the addBroadPhaseHandle(...) method for it. Our custom myFilteredRayCastCallback::addBroadPhaseHandle(...)
// method will be called from broadPhase::castRay and so provides a simple way to filter our shapes.

// The code for this method is presented below:

hkReal RaycastCallbackDemo::MyFilteredRayCastCallback::addBroadPhaseHandle(const hkpBroadPhaseHandle* broadphaseHandle, int castIndex)
{
	const hkpCollidable* col = static_cast<hkpCollidable*>( static_cast<const hkpTypedBroadPhaseHandle*>(broadphaseHandle)->getOwner() );
	const hkpShape* shape = col->getShape();

	if (shape)
	{
		// Only test the ray against the primitive shape if it
		// is a sphere

		// The hkpBroadPhase::castRay function will call this function whenether it comes across
		// an overlapping AABB. Any broadPhase
		// entry that contains a shape other than sphere is ignored. When a sphere
		// is encountered then a test against the actual shape is performed.

		HK_SET_OBJECT_COLOR((hkUlong)col, hkColor::YELLOW);

		if(shape->getType() == HK_SHAPE_SPHERE )
		{
			hkpShapeRayCastInput sinput;
			const hkTransform& trans = col->getTransform();

			// transform the ray into local space
			sinput.m_from.setTransformedInversePos(trans, m_input.m_from);
			sinput.m_to.setTransformedInversePos(  trans, m_input.m_to);

			// subshape filtering turned off
			sinput.m_rayShapeCollectionFilter = HK_NULL;

			if (shape->castRay(sinput, *m_output))
			{
				// transform the normal back into worldspace
				m_output->m_rootCollidable = col;
				m_output->m_normal.setRotatedDir( trans.getRotation(), m_output->m_normal );
			}
		}
	}

	// return the current hitFraction, this will allow the broadphase to do early outs for
	// objects more distant than the current hit
	return m_output->m_hitFraction;
}


// The method is passed a hkpBroadPhaseHandle* which is cast into a hkpCollidable* from which we may obtain the hkpShape for that collidable.
// If this method has been called it means that the ray has intersected with the AABB for that shape and so it is coloured YELLOW.
// A simple test is performed to see if this shape is of the type HK_SHAPE_SPHERE and if so we proceed to do a shape raycast to check for actual
// intersection with the sphere. Note the transformation of the start and end points from World Space to Local Space for that shape prior to
// performing the shape raycast.


RaycastCallbackDemo::~RaycastCallbackDemo()
{
	m_world->lock();

	int i;
	for (i=0;i<m_bodies.getSize();i++)
	{
		m_bodies[i]->removeReference();
	}

	m_world->unlock();
}


#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
"This demo illustrates the use of the raycast callback mechanism " \
"to filter ray casts between two shape types.  " \
"When the ray, drawn in GREY, intersects the broadphase AABB of a given shape the display color of  " \
"the shape is changed from GREY to YELLOW. The callback then checks to see if the AABB intersected belongs " \
"to a shape of type HK_SHAPE_SPHERE and if it does an additional shape raycast is performed to see " \
"if the sphere itself is intersected. If it is, then both the ray and the sphere are displayed in RED " \
"and the ray is clipped to the intersection point. ";

HK_DECLARE_DEMO(RaycastCallbackDemo, HK_DEMO_TYPE_PRIME, "Using user raycast callbacks (filters) to customise ray tests\n\nAll broadphase objects hit are coloured yellow, however\n only spheres are tested for exact intersections (coloured red)", helpString);

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
