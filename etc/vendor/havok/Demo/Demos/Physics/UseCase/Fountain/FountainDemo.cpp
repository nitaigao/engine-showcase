/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/Physics/UseCase/Fountain/FountainDemo.h>

#include <Common/Base/Algorithm/PseudoRandom/hkPseudoRandomGenerator.h>

#include <Physics/Dynamics/Action/hkpUnaryAction.h>
#include <Physics/Dynamics/Phantom/hkpAabbPhantom.h>

#include <Physics/Dynamics/World/BroadPhaseBorder/hkpBroadPhaseBorder.h>

#include <Common/Base/Thread/Job/ThreadPool/Cpu/hkCpuJobThreadPool.h>

#include <Common/Base/Config/hkConfigThread.h>


	/// Called at the end of the hkpWorld::simulate call, Attention: this might change
	// For all bodies inside the phantom, apply upward force
void FountainDemo::postSimulationCallback( hkpWorld* )
{
	//
	//	move the phantom
	//
	{
		m_localTime += 0.03f;
		float si = hkMath::sin( m_localTime ) * 6.0f;
		float co = hkMath::cos( m_localTime ) * 6.0f;
		const hkReal s = 1.5f;
		hkAabb aabb;
		aabb.m_min = hkVector4( -s + si, -10.0f, -s + co);
		aabb.m_max = hkVector4(  s + si,  10.0f,  s + co ) ;
		m_phantom->setAabb( aabb );
	}
	//
	//	apply a force on all objects
	//
	for (int i = 0; i < m_phantom->getOverlappingCollidables().getSize(); i++ )
	{
		hkpCollidable* c = m_phantom->getOverlappingCollidables()[i];
		if ( c->getType() == hkpWorldObject::BROAD_PHASE_ENTITY )
		{
			hkVector4 force( 0.0f,70.0f,0.0f );
			hkVector4 impulse; impulse.setMul4( m_timestep, force );
			hkpRigidBody* rb = hkGetRigidBody(m_phantom->getOverlappingCollidables()[i]);
			if ( rb )
			{
				rb->applyLinearImpulse( impulse );
			}
		}
	}
}

	/// Called, when the world gets deleted
void FountainDemo::worldDeletedCallback( hkpWorld* world )
{
	world->removeWorldDeletionListener( this );
	world->removeWorldPostSimulationListener( this );
}


class FountainBroadphaseBorder : public hkpBroadPhaseBorder
{
	public:

		FountainBroadphaseBorder( hkpWorld* world );

		virtual void maxPositionExceededCallback( hkpEntity* body );

	private:
		hkPseudoRandomGenerator m_generator;
};


FountainBroadphaseBorder::FountainBroadphaseBorder( hkpWorld* world )
:	hkpBroadPhaseBorder( world ), m_generator(747)
{
}

//	 If a body moves outside the world size (broadphase), warp back to center
void FountainBroadphaseBorder::maxPositionExceededCallback( hkpEntity* entity )
{
	hkpRigidBody* body = static_cast<hkpRigidBody*>(entity);
	hkVector4 pos( m_generator.getRandRange( -3.0f, 3.0f ),3,m_generator.getRandRange( -3.0f, 3.0f ));
	body->setPositionAndRotationAsCriticalOperation( pos, body->getRotation() );

	hkVector4 vel( m_generator.getRandRange( -3.0f, 3.0f ), m_generator.getRandRange( -3.0f, 3.0f ), m_generator.getRandRange( -3.0f, 3.0f ) );
	body->setLinearVelocityAsCriticalOperation( vel );
}


FountainDemo::FountainDemo(hkDemoEnvironment* env)
:	hkDefaultPhysicsDemo(env), m_localTime(0.0f)
{
	// Disable warnings
	hkError::getInstance().setEnabled(0xf0de4355, false);	// 'Your m_collisionTolerance seems to be very big'
	hkError::getInstance().setEnabled(0x1983bb7, false);	// 'The stack size in hkMemory is insufficient...'
	hkError::getInstance().setEnabled(0x7dd65995, false);	// 'System has requested a heap allocation on stack overflow...'

	//
	// Setup the camera
	//
	{
		hkVector4 from(0.0f, 30.0f, 50.0f);
		hkVector4 to  (0.0f,  0.0f,  0.0f);
		hkVector4 up  (0.0f,  1.0f,  0.0f);
		setupDefaultCameras( env, from, to, up );

		forceShadowState(false);

	}

	{
		hkpWorldCinfo info;
		info.m_gravity.set(0.0f, -1.9f, 0.0f);
		info.setBroadPhaseWorldSize( 100.0f );
		info.setupSolverInfo(hkpWorldCinfo::SOLVER_TYPE_4ITERS_MEDIUM);
		info.m_broadPhaseBorderBehaviour = info.BROADPHASE_BORDER_DO_NOTHING;	// we do our own stuff
		m_world = new hkpWorld( info );
		m_world->lock();

		// Register broadphase border
		FountainBroadphaseBorder *border = new FountainBroadphaseBorder( m_world );
		m_world->setBroadPhaseBorder(border);
		border->removeReference();

		// Register ALL agents (though some may not be necessary)
		hkpAgentRegisterUtil::registerAllAgents(m_world->getCollisionDispatcher());

		setupGraphics();
	}


	//
	// Create the ground
	//
	{
		hkVector4 boxSize( 20.0f, .5f , 20.0f );
		hkpShape *boxShape = new hkpBoxShape( boxSize , 0 );

		hkpRigidBodyCinfo boxInfo;

		boxInfo.m_motionType = hkpMotion::MOTION_FIXED;
		boxInfo.m_shape = boxShape;
		boxInfo.m_friction = 0.0f;

		{
			boxInfo.m_rotation = hkQuaternion( hkVector4( 1.0f,0.0f,0.0f ), 0.5f );
			hkpRigidBody* boxRigidBody = new hkpRigidBody(boxInfo);
			m_world->addEntity( boxRigidBody );
			boxRigidBody->removeReference();
		}
		{
			boxInfo.m_rotation = hkQuaternion( hkVector4( 1.0f,0.0f,0.0f), -0.5f );
			hkpRigidBody* boxRigidBody = new hkpRigidBody(boxInfo);
			m_world->addEntity( boxRigidBody );
			boxRigidBody->removeReference();
		}
		{
			boxInfo.m_rotation = hkQuaternion( hkVector4( 0.0f,0.0f,1.0f ), -0.5f );
			hkpRigidBody* boxRigidBody = new hkpRigidBody(boxInfo);
			m_world->addEntity( boxRigidBody );
			boxRigidBody->removeReference();
		}
		{
			boxInfo.m_rotation = hkQuaternion( hkVector4( 0.0f,0.0f,1.0f ), 0.5f );
			hkpRigidBody* boxRigidBody = new hkpRigidBody(boxInfo);
			m_world->addEntity( boxRigidBody );
			boxRigidBody->removeReference();
		}

		boxShape->removeReference();
	}

	//
	// Create many falling rigid bodies
	//
	{
		// We create a single shape, and resuse it for all bodies.
		hkVector4 boxSize( .5f, .5f ,.5f );
		hkpShape *boxShape = new hkpBoxShape( boxSize , 0 );

		// Limit number of polygons as Havok renderer is not perfect!
		const int numBodies = 800;
		hkPseudoRandomGenerator generator(654);
		for ( int i = 0; i < numBodies; i++ )
		{
			hkpRigidBody* boxRigidBody;
			{

				hkpRigidBodyCinfo boxInfo;
				boxInfo.m_shape = boxShape;
				boxInfo.m_motionType = hkpMotion::MOTION_BOX_INERTIA;

				boxInfo.m_position.set( generator.getRandRange(-10.0f, 10.0f), 10.0f, generator.getRandRange(-10.0f, 10.0f) );

				boxInfo.m_mass = 1.0f;
				hkpInertiaTensorComputer::setShapeVolumeMassProperties(boxInfo.m_shape, boxInfo.m_mass, boxInfo);

				boxRigidBody = new hkpRigidBody(boxInfo);

				m_world->addEntity( boxRigidBody );
			}
			boxRigidBody->removeReference();
		}
		boxShape->removeReference();
	}

	//
	// add myself as a SimulationListener
	//
	m_world->addWorldDeletionListener( this );
	m_world->addWorldPostSimulationListener( this );


	//
	// add the phantom
	//
	{
		
		hkAabb info;
		info.m_min = hkVector4( -2.0f, -10.0f, -2.0f );
		info.m_max = hkVector4( 2.0f, 10.0f, 2.0f ) ;
		m_phantom = new hkpAabbPhantom( info );
		
		m_world->addPhantom( m_phantom );
	}

	m_world->unlock();
}

FountainDemo::~FountainDemo()
{
	m_world->lock();

	// Re-enable warnings
	hkError::getInstance().setEnabled(0xf0de4355, true);
	hkError::getInstance().setEnabled(0x1983bb7, true);
	hkError::getInstance().setEnabled(0x7dd65995, true);

	//	environment->m_drawWireframe = true;
	m_phantom->removeReference();
	m_phantom = HK_NULL;
	m_world->removeWorldDeletionListener( this );
	m_world->removeWorldPostSimulationListener( this );

	//m_jobThreadPool deleted in hkDefaultPhysicsDemo();

	m_world->unlock();
}



#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
"A demo which uses a phantom object to act as a fountain, applying upward forces to anything which " \
"falls inside it. " \
"Additionally, this demo is a SimulationListener, and when notified by the world that a body has " \
"moved outside the world limits, it warps it back to the center of the fountain phantom " \
"Initially, many bodies are interpenetrating, which creates an explosion-like effect";

HK_DECLARE_DEMO(FountainDemo, HK_DEMO_TYPE_PRIME, "Fountain", helpString);

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
