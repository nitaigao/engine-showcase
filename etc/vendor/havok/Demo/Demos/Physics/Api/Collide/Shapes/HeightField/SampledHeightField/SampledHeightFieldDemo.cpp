/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/Physics/Api/Collide/Shapes/HeightField/SampledHeightField/SampledHeightFieldDemo.h>


//
// Need some shapes
//
#include <Physics/Collide/Shape/Convex/Box/hkpBoxShape.h>
#include <Physics/Collide/Shape/HeightField/SampledHeightField/hkpSampledHeightFieldShape.h>
#include <Physics/Collide/Shape/HeightField/SampledHeightField/hkpSampledHeightFieldBaseCinfo.h>
#include <Common/Base/Types/Geometry/Aabb/hkAabb.h>


// This tutorial shows:
// - How to subclass the sampled heightfield interface and implement a heightfield.
// - How to add the proper serialization interface to the subclassed heightfield


// The sampled heightfield shape implements collideSpheres such that it assumes that the 
// heights are evenly spaced in a grid along X and Z, with Y up.
// Note that collideSpheres is overridden and forwards to a templated function in hkpSampledHeightFieldShape.
// We do this so that the getHeightAt and getTriangleFlip  can be inlined.

class MySampledHeightFieldShape: public hkpSampledHeightFieldShape
{
	public:
		

		MySampledHeightFieldShape( const hkpSampledHeightFieldBaseCinfo& ci, hkUint16* data )
			:	hkpSampledHeightFieldShape(ci),
				m_data(data)
		{
		}

		// Generate a rough terrain
		HK_FORCE_INLINE hkReal getHeightAtImpl( int x, int z ) const
		{
			// Lookup data and return a float
			// We scale the data artifically by 5 to make it look interesting
			return 5.0f * hkReal(m_data[x * m_zRes + z]) / hkReal( hkUint16(-1) );
		}

		//	This should return true if the two triangles share the edge p00-p11
		// otherwise it should return false if the triangles share the edge p01-p10
		HK_FORCE_INLINE hkBool getTriangleFlipImpl() const
		{	
			return false;
		}

		virtual void collideSpheres( const CollideSpheresInput& input, SphereCollisionOutput* outputArray) const
		{
			hkSampledHeightFieldShape_collideSpheres(*this, input, outputArray);
		}

	private:

		hkUint16* m_data;
};


// We can see that the getHeightAt function is very straight forward and 
// could easily do a displacement map lookup from your render data etc.
// The Triangle flip specifies if given a quad of 4 points [00,01,11,10], does
// the triangle diagonal span 00-11 (true), or 01,10 (false). This is so
// that the collision detection and the display are using the same convention.
// Now that we have a class to use, lets see how easy it is to make one.



SampledHeightFieldDemo::SampledHeightFieldDemo(hkDemoEnvironment* env)
	:	hkDefaultPhysicsDemo(env, DEMO_FLAGS_NO_SERIALIZE)
{

	// Then we setup a camera in the right place to see our demo.
	{
		hkVector4 from( -11.5f, 10.f, 4.f );
		hkVector4 to  ( 0.5f, 4.f, 0.f );
		hkVector4 up  (  0.0f, 1.0f, 0.0f);
		setupDefaultCameras(env, from, to, up);
	}

	// We create a world the same as always, and let the demo initialize the graphics.
	{
		hkpWorldCinfo info;
		info.setBroadPhaseWorldSize( 100.0f );
		info.m_collisionTolerance = 0.03f;
		m_world = new hkpWorld(info);
		m_world->lock();

		setupGraphics();
	}

	// As we want collision detection, we must register the 
	// agents for the shapes we are going to use. Here we will
	// just use the hkpAgentRegisterUtil to register the common ones
	// and that includes the heightfields.
	hkpAgentRegisterUtil::registerAllAgents( m_world->getCollisionDispatcher() );

	// If we are to test a heightfield we want to drop objects on it, so here
	// we will create stacks of boxes, 64 bodies in total.
	{
		hkVector4 halfExtents(.25f, .25f, .25f );
		hkpShape* shape = new hkpBoxShape( halfExtents , 0 );

		for (int i = 0; i < 64; i++ )
		{
			hkpRigidBodyCinfo ci;
			ci.m_motionType = hkpMotion::MOTION_SPHERE_INERTIA;
			ci.m_shape = shape;
			ci.m_mass = 4.f;
			ci.m_inertiaTensor.setDiagonal( .3f,.3f,.3f );
			ci.m_position.set( hkReal(i&3)-1.0f, hkReal((i>>2)&3)+5.0f, hkReal((i>>4)&3)-1.0f );

			hkpRigidBody* body = new hkpRigidBody( ci );
			m_world->addEntity(body);
			body->removeReference();
		}
		shape->removeReference();
	}

	// Here we create an array of shorts that our heightfield will lookup
	// This array could easily have been loaded from a texture.
	const int xRes = 64;
	const int zRes = 64;
	m_heightData = hkAllocate<hkUint16>(xRes * zRes, HK_MEMORY_CLASS_DEMO);
	{
		for (int x = 0; x < xRes; x++)
		{
			for (int z = 0; z < zRes; z++)
			{
				hkReal dx,dz,height = 0;
				int octave = 1;
				// Add togther a few sine and cose waves
				for (int i=0; i< 3; i++)
				{
					dx = hkReal(x * octave) / xRes;
					dz = hkReal(z * octave) / zRes;

					height +=  (5 - (i * 2)) * hkMath::cos(dx * HK_REAL_PI) * hkMath::sin(dz * HK_REAL_PI);
					octave *= 4;
				}

				// Convert to 16 bit 
				m_heightData[x*zRes + z] = static_cast<hkUint16> ( hkUint16(-1) * (height + 10.0f) / 20.0f );
			}
		}
	}

	// Finally, we want our new heightfield. It will be a fixed body with 
	// the shape that we have defined at the start. Our heightfield above 
	// can cope with any resolution, but we will say that it is a 64x64
	// grid.
	{
		hkpSampledHeightFieldBaseCinfo ci;
		ci.m_xRes = xRes;
		ci.m_zRes = zRes;

		MySampledHeightFieldShape* heightFieldShape = new MySampledHeightFieldShape( ci , m_heightData);

		// Now that we have a shape all we need is the fixed body to represent it in the 
		// the simulation. Standard rigid body setup.
		{
			hkpRigidBodyCinfo rci;
			rci.m_motionType = hkpMotion::MOTION_FIXED;
			rci.m_position.setMul4( -0.5f, heightFieldShape->m_extents ); // center the heightfield
			rci.m_shape = heightFieldShape;
			rci.m_friction = 0.2f;

			hkpRigidBody* body = new hkpRigidBody( rci );

			m_world->addEntity(body);
			body->removeReference();
		}
	
		// Just need to remove the reference we hold to the shape, and that is it.
		heightFieldShape->removeReference();
	}

	m_world->unlock();
}


SampledHeightFieldDemo::~SampledHeightFieldDemo()
{
	hkDeallocate<hkUint16>( m_heightData );
}


hkDemo::Result SampledHeightFieldDemo::stepDemo()
{
	return hkDefaultPhysicsDemo::stepDemo();
}



////////////////////////////////////////////////////////////////////
#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
"This demo shows how to implement your own heightfield shape. " \
"The heightfield is built from an array of 16 bit height values. " \
"You should use this demo as a starting point when integrating your " \
"own heightfields.";

HK_DECLARE_DEMO(SampledHeightFieldDemo, HK_DEMO_TYPE_PRIME, "A regularly sampled heightfield", helpString);

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
