/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Common/Base/Memory/Memory/hkMemory.h>
#include <Physics/Collide/Shape/HeightField/SampledHeightField/hkpSampledHeightFieldShape.h>

#include <Physics/Collide/Query/CastUtil/hkpWorldRayCastInput.h>
#include <Physics/Collide/Query/CastUtil/hkpWorldRayCastOutput.h>

#include <Demos/Physics/Test/Performance/SlidingRagdolls/SlidingRagdollsDemo.h>
#include <Demos/DemoCommon/Utilities/GameUtils/GameUtils.h>

#include <Physics/Dynamics/World/hkpPhysicsSystem.h>

#include <Common/Base/Monitor/Spu/hkSpuMonitorCache.h>
#include <Common/Base/Monitor/hkMonitorStream.h>

#include <Physics/Dynamics/Constraint/Bilateral/Ragdoll/hkpRagdollConstraintData.h>
#include <Common/Base/Thread/JobQueue/hkJobQueue.h>

#include <Common/Base/Thread/Job/ThreadPool/Cpu/hkCpuJobThreadPool.h>




SlidingRagdollsDemo::SlidingRagdollsDemo(hkDemoEnvironment* env)
:	hkDefaultPhysicsDemo(env)
{

	//
	// Setup the camera.
	//
	{
		hkVector4 from(20.0f, 5.0f, 20.0f);
		hkVector4 to(0.0f, 0.0f, 0.0f);
		hkVector4 up(0.0f, 1.0f, 0.0f);
		setupDefaultCameras( m_env, from, to, up );
	}

	//
	// Create the world.
	// 
	{
		hkpWorldCinfo info;
		info.m_collisionTolerance = 0.01f;
		info.m_gravity.set(0.0f, -9.8f, 0.0f);
		info.setBroadPhaseWorldSize(250.0f) ;
		info.setupSolverInfo(hkpWorldCinfo::SOLVER_TYPE_2ITERS_MEDIUM);

		m_world = new hkpWorld( info );
		m_world->lock();

		// Register all agents.
		hkpAgentRegisterUtil::registerAllAgents(m_world->getCollisionDispatcher());

		setupGraphics();
		m_world->unlock();
	}


	//
	// Setup the scene: a set of ragdolls falling down a sloping heightfield
	//
	{
		m_world->lock();

		//
		// Create a collision filter, to filter collisions between ragdoll limbs
		//
		{
			hkpGroupFilter* filter = new hkpGroupFilter();
			m_world->setCollisionFilter( filter );
			filter->removeReference();
		}

		// Build the landscape and add it to m_world.
		buildLandscape();
#if defined(HK_PLATFORM_PSP) || defined(HK_DEBUG)
		createRagdollGrid( m_world, 2,2, 8.0f, 8.0f, m_ragdolls );
#else
		createRagdollGrid( m_world, 5,5, 8.0f, 8.0f, m_ragdolls );
#endif

		m_world->unlock();
	}

}


hkDemo::Result SlidingRagdollsDemo::stepDemo()
{
	// check all ragdolls for falling of the world
	respawnRagdollsIfNecessary();

	return hkDefaultPhysicsDemo::stepDemo();
}


SlidingRagdollsDemo::~SlidingRagdollsDemo( )
{
	// Release the underlying geometry data.
	// Note: normally this data is owned by the graphics system (not in our case though).

	m_world->lock();
	for (int r=0; r < m_ragdolls.getSize(); ++r)
	{
		m_ragdolls[r]->removeReference();
	}
	m_world->unlock();
}


//
// Helper functions and classes to setup the physical demo
//

void SlidingRagdollsDemo::respawnRagdollsIfNecessary()
{
	m_world->lock();

	for (int r =0 ; r < m_ragdolls.getSize(); r++ )
	{
		hkpPhysicsSystem* ragdoll = m_ragdolls[r];
		hkpRigidBody* body = ragdoll->getRigidBodies()[0];
		if ( body->getPosition()(1) < -30.0f )
		{
			hkVector4 offset;
			hkReal oldX = body->getPosition()(0);
			offset(0) = hkMath::clamp( oldX, -60.0f, 60.0f ) - body->getPosition()(0);
			offset(1) = 40.0f;
			offset(2) = -60.0f - body->getPosition()(2);
			offset(3) = 0.0f;

			for (int b = 0; b < ragdoll->getRigidBodies().getSize(); b++ )
			{
				hkpRigidBody* rb = ragdoll->getRigidBodies()[b];
				hkVector4 pos; pos.setAdd4( rb->getPosition(), offset );
				rb->setPosition( pos );
				rb->setAngularVelocity( hkVector4::getZero() );
				rb->setLinearVelocity( hkVector4::getZero() );
			}
		}
	}
	m_world->unlock();

}

class SlidingRagdollsDemoSampledHeightFieldShape: public hkpSampledHeightFieldShape
{
public:
	SlidingRagdollsDemoSampledHeightFieldShape( const hkpSampledHeightFieldBaseCinfo& ci )
		: hkpSampledHeightFieldShape(ci)
	{
	}

	HK_FORCE_INLINE hkReal getHeightAtImpl( int x, int y ) const
	{
		HK_ASSERT(0x31f89011,  x>=0 && x < m_xRes );
		HK_ASSERT(0x4c32b1bc,  y>=0 && y < m_zRes );

		const int x_scale = 8;
		const int y_scale = 4;
		x = x&(x_scale-1);
		y = y&(y_scale-1);
		hkReal sx = x * (2.0f / x_scale );
		hkReal sy = y * (2.0f / y_scale );
		if ( x > x_scale/2 )		{			sx = 2.0f - sx;			}
		if ( y > y_scale/2 )		{			sy = 2.0f - sy;			}

		return 0.3f * sy;
	}

	/// this should return true if the two triangles share the edge p00-p11
	///      otherwise it should return false if the triangles share the edge p01-p10
	HK_FORCE_INLINE hkBool getTriangleFlipImpl() const
	{
		return false;
	}

	virtual void collideSpheres( const CollideSpheresInput& input, SphereCollisionOutput* outputArray) const
	{
		hkSampledHeightFieldShape_collideSpheres(*this, input, outputArray);
	}
};

void SlidingRagdollsDemo::buildLandscape()
{
	//
	// Create the ground
	//
	{
		hkpRigidBodyCinfo groundInfo;

		//
		//	Set the if condition to 0 if you want to test the heightfield
		//
		{
			hkpSampledHeightFieldBaseCinfo ci;
			ci.m_xRes = 64;
			ci.m_zRes = 64;
			ci.m_scale.set( 2.0f, 2.0f, 2.0f );
			groundInfo.m_shape = new SlidingRagdollsDemoSampledHeightFieldShape( ci );
			groundInfo.m_position.set(-0.5f * ci.m_xRes * ci.m_scale(0), -0.0f, -0.5f * ci.m_zRes * ci.m_scale(2));
		}

		{
			groundInfo.m_motionType = hkpMotion::MOTION_FIXED;
			groundInfo.m_friction = 2.0f;
			groundInfo.m_rotation.setAxisAngle( hkTransform::getIdentity().getColumn(0), 0.25f );
			groundInfo.m_friction = 0.0f;
			hkpRigidBody* groundbody = new hkpRigidBody(groundInfo);

			m_world->addEntity(groundbody);
			groundbody->removeReference();
		}

		groundInfo.m_shape->removeReference();
	}
}



//
// Create a grid of ragdolls
//

void SlidingRagdollsDemo::createRagdollGrid( hkpWorld* world, int x_size, int y_size, hkReal xStep, hkReal yStep, hkArray<hkpPhysicsSystem*>&	ragdollsOut)
{
	int systemGroup = 2;
	hkReal ragdollHeight = 2.00f;

	hkpPhysicsSystem* templateRagdoll;
	{
		hkQuaternion	rotation; rotation.setIdentity();
		rotation.setAxisAngle( hkTransform::getIdentity().getColumn(0), HK_REAL_PI  * -0.5f );
		hkVector4 position; position.setZero4();
		templateRagdoll = GameUtils::createRagdoll( ragdollHeight, position, rotation, systemGroup, GameUtils::RPT_CAPSULE );
	}

	for( int x = 0; x < x_size; x++ )
	{
		for( int y = 0; y < y_size; y++ )
		{
			hkVector4 position;
			// do a raycast to place the ragdoll
			{
				hkpWorldRayCastInput ray;
				ray.m_from.set( (x - 0.5f * x_size) * xStep,  100, (y - 0.5f * y_size) * yStep );
				ray.m_to = ray.m_from;
				ray.m_to(1) -= 200.0f;

				hkpWorldRayCastOutput result;
				world->castRay( ray, result );
				position.setInterpolate4( ray.m_from, ray.m_to, result.m_hitFraction );
				position(1) += ragdollHeight* 0.5f;
			}

			hkpPhysicsSystem* ragdoll = templateRagdoll->clone();
			{
				for ( int i = 0; i < ragdoll->getRigidBodies().getSize(); i++)
				{
					hkpRigidBody* rb = ragdoll->getRigidBodies()[i];
					rb->setCollisionFilterInfo( hkpGroupFilter::calcFilterInfo( 1, systemGroup ) );
					rb->setLinearDamping( 0.1f );
					hkVector4 pos; pos.setAdd4( rb->getPosition(), position );
					rb->setPosition( pos );
				}
			}
			systemGroup++;


			world->addPhysicsSystem(ragdoll);
			ragdollsOut.pushBack(ragdoll);
		}
	}
	templateRagdoll->removeReference();
}


#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

HK_DECLARE_DEMO( SlidingRagdollsDemo, HK_DEMO_TYPE_PHYSICS | HK_DEMO_TYPE_CRITICAL, "Sliding ragdolls", "Lots of ragdolls on a landscape" );

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
