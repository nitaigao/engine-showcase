/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/Physics/Api/Collide/Shapes/HeightField/TriSampledHeightField/TriSampledHeightFieldDemo.h>

//
// Need some shapes
//
#include <Physics/Collide/Shape/Convex/Box/hkpBoxShape.h>
#include <Physics/Collide/Shape/HeightField/SampledHeightField/hkpSampledHeightFieldShape.h>
#include <Physics/Collide/Shape/HeightField/SampledHeightField/hkpSampledHeightFieldBaseCinfo.h>

#include <Physics/Collide/Shape/HeightField/SampledHeightField/hkpSampledHeightFieldShape.h>
#include <Physics/Collide/Shape/HeightField/SampledHeightField/hkpSampledHeightFieldBaseCinfo.h>

#include <Physics/Collide/Shape/HeightField/TriSampledHeightField/hkpTriSampledHeightFieldCollection.h>
#include <Physics/Collide/Shape/HeightField/TriSampledHeightField/hkpTriSampledHeightFieldBvTreeShape.h>

#include <Demos/DemoCommon/DemoFramework/hkTextDisplay.h>


// A very simple procedural heightfield shape. See the sampledheightfieldapi demo for more details on the heightfield shape.
class SimpleSampledHeightFieldShape: public hkpSampledHeightFieldShape
{
	public:
		
		SimpleSampledHeightFieldShape( const hkpSampledHeightFieldBaseCinfo& ci ): hkpSampledHeightFieldShape(ci)
		{
		}

		HK_FORCE_INLINE hkReal getHeightAtImpl( int x, int z ) const
		{
			// Create a flat heightfield with a single bump
			if ( x == 3 )
			{
				return 3;
			}
			else
			{
				return 0;
			}
		}
		HK_FORCE_INLINE hkBool getTriangleFlipImpl() const
		{	
			return false;
		}

		virtual void collideSpheres( const CollideSpheresInput& input, SphereCollisionOutput* outputArray) const
		{
			hkSampledHeightFieldShape_collideSpheres(*this, input, outputArray);
		}
};

TriSampledHeightFieldDemo::TriSampledHeightFieldDemo(hkDemoEnvironment* env)
	:	hkDefaultPhysicsDemo(env, DEMO_FLAGS_NO_SERIALIZE)
{
	// Disable face culling
	setGraphicsState(HKG_ENABLED_CULLFACE, false);

	// Setup a camera in the right place to see our demo.
	{
		hkVector4 from( -3.7f, 5, 17.6f );
		hkVector4 to  (-1.5f, 1, 1.1f );
		hkVector4 up  (  0.0f, 1.0f, 0.0f);
		setupDefaultCameras(env, from, to, up);
	}

	{
		hkpWorldCinfo info;
		info.setBroadPhaseWorldSize( 100.0f );
		info.m_collisionTolerance = 0.03f;
		m_world = new hkpWorld(info);
		m_world->lock();

		setupGraphics();
	}

	hkpAgentRegisterUtil::registerAllAgents( m_world->getCollisionDispatcher() );

	//
	// Create two movable rigid bodies to fall on the two heightfields.
	//
	{
		hkVector4 halfExtents(1.f, .25f, 1.f );
		hkpShape* shape = new hkpBoxShape( halfExtents , 0 );

		for (int i = 0; i < 2; i++ )
		{
			hkpRigidBodyCinfo ci;
			ci.m_motionType = hkpMotion::MOTION_SPHERE_INERTIA;
			ci.m_shape = shape;
			ci.m_mass = 4.f;
			hkpMassProperties m;
			hkpInertiaTensorComputer::computeShapeVolumeMassProperties(shape,4, m);
			ci.m_inertiaTensor = m.m_inertiaTensor;
			ci.m_position.set( hkReal(i * 7) - 5 , 4, 3 );

			hkpRigidBody* body = new hkpRigidBody( ci );
			m_world->addEntity(body);
			body->removeReference();
		}
		shape->removeReference();
	}


	{
		// Create our heightfield

		hkpSampledHeightFieldBaseCinfo ci;
		ci.m_xRes = 7;
		ci.m_zRes = 7;
		SimpleSampledHeightFieldShape* heightFieldShape = new SimpleSampledHeightFieldShape( ci );

		{
			//
			// Create the first fixed rigid body, using the standard heightfield as the shape
			//

			hkpRigidBodyCinfo rci;
			rci.m_motionType = hkpMotion::MOTION_FIXED;
			rci.m_position.set(-8, 0, 0);
			rci.m_shape = heightFieldShape;
			rci.m_friction = 0.2f;

			hkpRigidBody* bodyA = new hkpRigidBody( rci );
			m_world->addEntity(bodyA);
			bodyA->removeReference();


			//
			// Create the second fixed rigid body, using the triSampledHeightfield
			//

			// Wrap the heightfield in a hkpTriSampledHeightFieldCollection:
			hkpTriSampledHeightFieldCollection* collection = new hkpTriSampledHeightFieldCollection( heightFieldShape );

			// Now wrap the hkpTriSampledHeightFieldCollection in a hkpTriSampledHeightFieldBvTreeShape
			hkpTriSampledHeightFieldBvTreeShape* bvTree = new hkpTriSampledHeightFieldBvTreeShape( collection );

			// Finally, assign the new hkpTriSampledHeightFieldBvTreeShape to be the rigid body's shape
			rci.m_shape = bvTree;

			rci.m_position.set(-1,0,0);
			hkpRigidBody* bodyB = new hkpRigidBody( rci );
			m_world->addEntity(bodyB);
			bodyB->removeReference();

			heightFieldShape->removeReference();
			collection->removeReference();
			bvTree->removeReference();

		}

		hkString left("Standard heightfield.");
		m_env->m_textDisplay->outputText3D(left, -7, -.2f, 7, 0xffffffff, 2000);

		hkString right("Triangle heightfield.");
		m_env->m_textDisplay->outputText3D(right, -1, -.2f, 7, 0xffffffff, 2000);
	}

	m_world->unlock();
}


TriSampledHeightFieldDemo::~TriSampledHeightFieldDemo()
{
	m_env->m_textDisplay->wipeText();
}

////////////////////////////////////////////////////////////////////
#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
"This demo shows the difference between the heightfield shape "\
"and the triangulated sampled heightfield shape. Both shapes use the same "\
"underlying data, however they are simulated differently. The heightfield "\
"shape simply collides against the vertices of the colliding object, "\
"while the triangulated heightfield shape performs full collision detection "\
"against the triangles formed by the heightfield.";

HK_DECLARE_DEMO(TriSampledHeightFieldDemo, HK_DEMO_TYPE_PRIME | HK_DEMO_TYPE_CRITICAL, "A heightfield simulated as a triangle mesh", helpString);

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
