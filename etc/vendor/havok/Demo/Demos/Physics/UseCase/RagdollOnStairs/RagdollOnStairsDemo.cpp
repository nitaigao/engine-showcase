/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Physics/Dynamics/World/hkpPhysicsSystem.h>

#include <Demos/Physics/UseCase/RagdollOnStairs/RagdollOnStairsDemo.h>
#include <Common/Visualize/hkDebugDisplay.h>

#include <Graphics/Common/Input/Pad/hkgPad.h>
#include <Demos/DemoCommon/Utilities/GameUtils/GameUtils.h>

RagdollOnStairsDemo::RagdollOnStairsDemo(hkDemoEnvironment* env)
:	hkDefaultPhysicsDemo(env)
{
	{
		hkpWorldCinfo bob;
		bob.m_gravity.set( 0, 0, -9.81f );
		bob.setBroadPhaseWorldSize( 200.0f );
		m_world = new hkpWorld( bob );
		m_world->lock();

		// Register ALL agents (though some may not be necessary)
		hkpAgentRegisterUtil::registerAllAgents(m_world->getCollisionDispatcher());
	}

	//
	// Create a group filter
	//
	{
		hkpGroupFilter* filter = new hkpGroupFilter();
		hkpGroupFilterSetup::setupGroupFilter( filter );
		m_world->setCollisionFilter( filter );
		filter->removeReference();
	}
	
	hkVector4 position( 0, 0, 0 );

	//
	//	Create a ground box
	//
	{		

		hkVector4 size( 15, 15, 1 );

		hkpRigidBody* rb = GameUtils::createBox( size, 0, hkVector4::getZero() );
		rb->setCollisionFilterInfo( hkpGroupFilter::calcFilterInfo( hkpGroupFilterSetup::LAYER_STATIC ) );

		m_world->addEntity( rb );
		rb->removeReference();
	}

	//
	//	Create a staircase
	//
	{	
		const int numSteps = 10;
		for( int i = 0; i < numSteps; i++ )
		{
			hkVector4 size( 7, 0.5f, 0.5f );

			position(1) += size(1);
			position(2) += size(2);

			hkpRigidBody* rb = GameUtils::createBox( size, 0, position );
			rb->setCollisionFilterInfo( hkpGroupFilter::calcFilterInfo( hkpGroupFilterSetup::LAYER_STATIC ) );
			rb->getMaterial().setFriction( 0.25f );
			rb->getMaterial().setFriction( 0.75f );

			m_world->addEntity( rb );
			rb->removeReference();
		}
	}
	
	//
	// Create a ragdoll
	//
	{
		const	int	numRagdollTypes = GameUtils::RPT_COUNT;
		m_ragdollType = numRagdollTypes-1;
		position(1) -= 1.0f;

		const int		numRagdolls = 5;
		const hkReal	height = 2.0f;
		for( int i = 0; i < numRagdolls; i++ )
		{
			position(2) += height * 1.25f;

			hkQuaternion	rotation; rotation.setIdentity();

			hkpPhysicsSystem* ragdoll = GameUtils::createRagdoll( height, position, rotation, i + 1, (GameUtils::RagdollPartType)m_ragdollType );
			hkpInertiaTensorComputer::optimizeInertiasOfConstraintTree( ragdoll->getConstraints().begin(), ragdoll->getConstraints().getSize(), ragdoll->getRigidBodies()[0] );

			const hkArray<hkpRigidBody*>& rigidbodies = ragdoll->getRigidBodies();
			for( int iRB = 0; iRB < rigidbodies.getSize(); iRB++ )
			{
				hkpRigidBody* body = rigidbodies[iRB];
				body->setLinearDamping( 0.1f );
				body->getMaterial().setFriction(0.31f);				
			}
		
			m_world->addPhysicsSystem(ragdoll);
			m_ragdolls.pushBack( ragdoll );
		}
	}

	//
	// Setup the camera
	//
	{
		hkVector4 from( -10.0f, -10.0f, 2.5f );
		hkVector4 to  (   0.0f,   0.0f, 5.0f );
		hkVector4 up  (   0.0f,   0.0f, 1.0f );
		setupDefaultCameras( env, from, to, up );
		
		float ldir[] = { 0, 0.5f, -1.0f };
		setSoleDirectionLight(m_env, ldir, 0xffffffff );

		setupGraphics();
	}

	m_world->unlock();
}

RagdollOnStairsDemo::~RagdollOnStairsDemo()
{
	m_world->lock();

	for( int i = 0; i < m_ragdolls.getSize(); i++ )
	{
		m_ragdolls[i]->removeReference();
	}

	m_world->unlock();
}

hkDemo::Result RagdollOnStairsDemo::stepDemo()
{
	m_world->lock();

	const	int	numRagdollTypes = GameUtils::RPT_COUNT;
	hkBool		recreateRagdoll = false;

	const hkgPad* pad = m_env->m_gamePad;

	if( pad->wasButtonPressed( HKG_PAD_DPAD_UP ) )
	{
		recreateRagdoll = true;
	}

	if( pad->wasButtonPressed( HKG_PAD_DPAD_LEFT ) )
	{
		recreateRagdoll = true;
		m_ragdollType = (--m_ragdollType + numRagdollTypes) % numRagdollTypes;
	}

	if( pad->wasButtonPressed( HKG_PAD_DPAD_RIGHT ) )
	{
		recreateRagdoll = true;
		m_ragdollType = (++m_ragdollType + numRagdollTypes) % numRagdollTypes;
	}

	if( recreateRagdoll )
	{
		hkVector4 position( 0, 4.5f, 5 );
		const hkReal	height = 2.0f;

		for( int i = 0; i < m_ragdolls.getSize(); i++ )
		{
			m_world->removePhysicsSystem(m_ragdolls[i]);
			m_ragdolls[i]->removeReference();

			position(2) += height * 1.25f;

			hkQuaternion	rotation; rotation.setIdentity();

			///[createragdoll_text]
			/// The ragdolls in this demo are created using the createRagdoll function from the 'GameUtils'
			/// utility class. This function takes a height and constructs rigid bodies and constraints
			/// representing limbs and joints in proportion for a standard humanoid biped of that size.
			///
			hkpPhysicsSystem* ragdoll = GameUtils::createRagdoll( height, position, rotation, i + 1, (GameUtils::RagdollPartType)m_ragdollType );
			///>
			/// After the ragdoll is created it can be added to the world
			///
			m_world->addPhysicsSystem(ragdoll);
			///>
			/// Note that when removing a ragdoll from the world we do not need to 
			/// explicitly remove the constraints, only the rigid bodies. 
			/// The constraints will be removed from the world automatically when any 
			/// of their associated rigid bodies are removed.
			
			m_ragdolls[i] = ragdoll;
		}
	}

	m_world->unlock();

	return hkDefaultPhysicsDemo::stepDemo();
}



#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = "Some ragdolls falling down a stairs";

HK_DECLARE_DEMO(RagdollOnStairsDemo, HK_DEMO_TYPE_PHYSICS, helpString, helpString );

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
