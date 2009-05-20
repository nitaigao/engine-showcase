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

#include <Demos/Physics/Test/Performance/SimplifiedTois/SimplifiedToisDemo.h>
#include <Demos/DemoCommon/Utilities/GameUtils/GameUtils.h>

#include <Physics/Dynamics/World/hkpPhysicsSystem.h>

#include <Physics/Collide/Dispatch/hkpCollisionDispatcher.h>

#include <Common/Base/Monitor/Spu/hkSpuMonitorCache.h>
#include <Common/Base/Monitor/hkMonitorStream.h>

#include <Physics/Dynamics/Constraint/Bilateral/Ragdoll/hkpRagdollConstraintData.h>
#include <Common/Base/Thread/JobQueue/hkJobQueue.h>

#include <Common/Base/Thread/Job/ThreadPool/Cpu/hkCpuJobThreadPool.h>




SimplifiedToisDemo::SimplifiedToisDemo(hkDemoEnvironment* env)
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
		info.m_simulationType = hkpWorldCinfo::SIMULATION_TYPE_CONTINUOUS;
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

		hkpRigidBody* ground = GameUtils::createBox(hkVector4(50.0f, 0.05f, 50.0f), 0.0f, hkVector4(0.0f, -0.05f, 0.0f), 0.0f);
		m_world->addEntity(ground);
		ground->removeReference();

		hkVector4 position(-2.0f, 20.0f, -2.0f);
		hkVector4 velocity(0.0f, -5.0f, 0.0f);

		for (int x = 0; x < 3; x++)
		{
			for (int y = 0; y < 3; y++)
			{
				hkVector4 offsetX(2.0f, 0.0f, 0.0f);
				hkVector4 offsetY(0.0f, 0.0f, 2.0f);

				hkVector4 thisPosition; 
				thisPosition.setAddMul4(position, offsetX, hkReal(x));
				thisPosition.addMul4(hkReal(y), offsetY);
				createRagdoll(m_world, thisPosition, velocity, m_ragdolls);
			}
		}

		m_world->unlock();
	}

}


hkDemo::Result SimplifiedToisDemo::stepDemo()
{
	static int count = 0;
	if (++count > 200)
	{
		count = 0;
		m_world->markForWrite();
		hkBool& useSimplifiedTois = m_world->getCollisionDispatcher()->getCollisionQualityInfo(hkpCollisionDispatcher::COLLISION_QUALITY_SIMPLIFIED_TOI)->m_useSimpleToiHandling;
		useSimplifiedTois = !useSimplifiedTois;
		respawnRagdolls();
		m_world->unmarkForWrite();
	}


	return hkDefaultPhysicsDemo::stepDemo();
}


SimplifiedToisDemo::~SimplifiedToisDemo( )
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

void SimplifiedToisDemo::respawnRagdolls()
{
	m_world->lock();

	for (int r =0 ; r < m_ragdolls.getSize(); r++ )
	{
		hkpPhysicsSystem* ragdoll = m_ragdolls[r];

		hkVector4 offset; offset.set(0.0f, 20.0f, 0.0f);
		hkVector4 velocity; velocity.set(0.0f, -5.0f, 0.0f);
		for (int b = 0; b < ragdoll->getRigidBodies().getSize(); b++ )
		{
			hkpRigidBody* rb = ragdoll->getRigidBodies()[b];
			hkVector4 pos; pos.setAdd4( rb->getPosition(), offset );
			rb->setPosition( pos );
			rb->setLinearVelocity(velocity);
			rb->setAngularVelocity( hkVector4::getZero() );
		}

		// This is illegal and will trigger asserts.
		// Simplified_toi-quality object, cannot link to toi-priority constraints.
		//for (int c = 0; c < ragdoll->getConstraints().getSize(); c++)
		//{
		//	hkpConstraintInstance* constraint = ragdoll->getConstraints()[c];
		//	constraint->m_priority = hkpConstraintInstance::PRIORITY_TOI;
		//}
	}
	m_world->unlock();

}




//
// Create a grid of ragdolls
//

void SimplifiedToisDemo::createRagdoll( hkpWorld* world, const hkVector4& position, const hkVector4& velocity, hkArray<hkpPhysicsSystem*>&ragdollsOut)
{
	static int systemGroup = 2;
	hkReal ragdollHeight = 2.00f;

	hkpPhysicsSystem* ragdoll;
	{
		hkQuaternion	rotation; rotation.setIdentity();
		rotation.setAxisAngle( hkTransform::getIdentity().getColumn(0), HK_REAL_PI  * -0.5f );
		ragdoll = GameUtils::createRagdoll( ragdollHeight, position, rotation, systemGroup, GameUtils::RPT_CAPSULE );
	}

	for ( int i = 0; i < ragdoll->getRigidBodies().getSize(); i++)
	{
		hkpRigidBody* rb = ragdoll->getRigidBodies()[i];
		rb->setCollisionFilterInfo( hkpGroupFilter::calcFilterInfo( 1, systemGroup ) );
		rb->setLinearDamping( 0.0f );
		rb->setQualityType(HK_COLLIDABLE_QUALITY_DEBRIS_SIMPLE_TOI);
		rb->setLinearVelocity(velocity);
	}

	systemGroup++;

	world->addPhysicsSystem(ragdoll);
	ragdollsOut.pushBack(ragdoll);
}



#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

HK_DECLARE_DEMO( SimplifiedToisDemo, HK_DEMO_TYPE_PHYSICS , "Simplified Tois", "Ragdolls on landscape using Normal Tois and SimplifiedTois" );

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
