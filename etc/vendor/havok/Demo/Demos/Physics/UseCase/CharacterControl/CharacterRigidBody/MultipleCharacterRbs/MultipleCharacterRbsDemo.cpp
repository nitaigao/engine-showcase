/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Common/Base/Monitor/hkMonitorStream.h>
#include <Common/Base/Container/LocalArray/hkLocalArray.h>
#include <Demos/Physics/UseCase/CharacterControl/CharacterRigidBody/MultipleCharacterRbs/MultipleCharacterRbsDemo.h>
#include <Demos/DemoCommon/Utilities/GameUtils/Landscape/LandscapeRepository.h>

#include <Physics/Utilities/CharacterControl/CharacterRigidBody/hkpCharacterRigidBody.h>
#include <Physics/Utilities/CharacterControl/StateMachine/hkpDefaultCharacterStates.h>
#include <Physics/Collide/Shape/Convex/Capsule/hkpCapsuleShape.h>

#include <Common/Base/Types/Color/hkColor.h>

static const hkVector4 UP(0,1,0);

MultipleCharacterRbsDemo::MultipleCharacterRbsDemo(hkDemoEnvironment* env)
:	hkDefaultPhysicsDemo(env)
{

	// Setup the graphics
	{
		forceShadowState(false);
	}

	// Create the world
	{
		hkpWorldCinfo info;
		info.setBroadPhaseWorldSize( 350.0f );  
		info.m_gravity.set(0, -9.8f, 0);
		info.m_contactPointGeneration = hkpWorldCinfo::CONTACT_POINT_ACCEPT_ALWAYS;
		info.m_collisionTolerance = 0.1f; // faster, but better 0.01f for accuracy
		m_world = new hkpWorld( info );
		m_world->lock();

		hkpAgentRegisterUtil::registerAllAgents(m_world->getCollisionDispatcher());

		setupGraphics();
	}

	// Create the ground using the landscape repository
	{
		int landscapeIndex = LandscapeRepository::getInstance().getLandscapeIndexByName("simpleFlatLandOneMetreTris");
		m_landscapeContainer = LandscapeRepository::getInstance().getLandscape(landscapeIndex);

		hkpRigidBodyCinfo groundInfo;
		groundInfo.m_motionType = hkpMotion::MOTION_FIXED;
		groundInfo.m_shape = m_landscapeContainer->m_shape;

		hkpRigidBody* groundbody = new hkpRigidBody(groundInfo);

		m_world->addEntity(groundbody)->removeReference();

		setupDefaultCameras( env, m_landscapeContainer->m_cameraFrom, m_landscapeContainer->m_cameraTo, hkVector4(0,1,0) );
		
	}

	AabbSpawnUtil spawnUtil( m_landscapeContainer->m_spawnVolumes );
	

	//	Create character rigid bodies 

	const int numCharacters = 100;

	{
		m_characterRigidBodies.setSize( numCharacters );

		// Create a capsule to represent the character standing
		hkVector4 top(0, .4f, 0);
		hkVector4 bottom(0,-.4f, 0);		

		hkpShape* characterShape = new hkpCapsuleShape(top, bottom, .6f);

		// Construct characters
		for (int i=0; i < numCharacters; i++)
		{

			// Construct a character rigid body
			hkpCharacterRigidBodyCinfo info;
			info.m_mass = 100.0f;
			info.m_shape = characterShape;

			info.m_maxForce = 2000.0f;
			info.m_up = UP;
			spawnUtil.getNewSpawnPosition( hkVector4(1, 2, 1), info.m_position );

			info.m_maxSlope = 90.0f * HK_REAL_DEG_TO_RAD;

			m_characterRigidBodies[i] = new hkpCharacterRigidBody( info );
			m_world->addEntity( m_characterRigidBodies[i]->getRigidBody() );

		}
		characterShape->removeReference();
	}
	
	// Create the Character state machine and context
	hkpCharacterStateManager* manager;
	{
		hkpCharacterState* state;
		manager = new hkpCharacterStateManager();

		state = new hkpCharacterStateOnGround();
		manager->registerState( state,	HK_CHARACTER_ON_GROUND);
		state->removeReference();

		state = new hkpCharacterStateInAir();
		manager->registerState( state,	HK_CHARACTER_IN_AIR);
		state->removeReference();

		state = new hkpCharacterStateJumping();
		manager->registerState( state,	HK_CHARACTER_JUMPING);
		state->removeReference();

		state = new hkpCharacterStateClimbing();
		manager->registerState( state,	HK_CHARACTER_CLIMBING);
		state->removeReference();

	}

	// Create a context for each character
	{
		m_characterContexts.setSize(numCharacters);

		for (int i=0; i < numCharacters; i++)
		{
			m_characterContexts[i] = new hkpCharacterContext(manager, HK_CHARACTER_ON_GROUND);
			m_characterContexts[i]->setCharacterType(hkpCharacterContext::HK_CHARACTER_RIGIDBODY);
		}
		manager->removeReference();
	}
	
	//Initialized the round robin counter
	m_tick = 0;

	m_world->unlock();
}

MultipleCharacterRbsDemo::~MultipleCharacterRbsDemo()
{
	m_world->lock();

	for (int i=0; i < m_characterRigidBodies.getSize(); i++)
	{
		m_characterRigidBodies[i]->removeReference();

		delete m_characterContexts[i];
	}

	m_world->unlock();
}


hkDemo::Result MultipleCharacterRbsDemo::stepDemo()
{
	HK_TIMER_BEGIN( "simulate multiply characters", HK_NULL );

	m_world->lock();

	hkVector4 up;
	up.setNeg4( m_world->getGravity() );
	up.normalize3();
	int numCharacters = m_characterRigidBodies.getSize();

	m_tick++;

	hkLocalArray<hkpCharacterInput> input( numCharacters ) ;
	input.setSize(numCharacters);

	// Fill in the state machine input structure
	{
		for (int i=0; i < numCharacters; i++)
		{
			input[i].m_atLadder = false;
			input[i].m_up = up;

			// Steer the characters
			hkReal time = hkReal(m_tick) / 60.0f;
			input[i].m_inputLR = hkMath::sin(time + i);
			input[i].m_inputUD = hkMath::cos(time + i);
			input[i].m_wantJump = false;
			input[i].m_forward.set(1,0,0);

			hkStepInfo stepInfo;
			stepInfo.m_deltaTime = m_timestep;
			stepInfo.m_invDeltaTime = 1.0f/m_timestep;
			
			input[i].m_stepInfo = stepInfo;

			input[i].m_characterGravity = m_world->getGravity();
			input[i].m_velocity = m_characterRigidBodies[i]->getLinearVelocity();
			input[i].m_position = m_characterRigidBodies[i]->getPosition();

			hkpSurfaceInfo ground;
			m_characterRigidBodies[i]->checkSupport(stepInfo, ground);

			input[i].m_isSupported = (ground.m_supportedState == hkpSurfaceInfo::SUPPORTED);
			input[i].m_surfaceNormal = ground.m_surfaceNormal;
			input[i].m_surfaceVelocity = ground.m_surfaceVelocity;	
		}
	}


	hkLocalArray<hkpCharacterOutput> output( numCharacters);
	output.setSize(numCharacters);

	// Apply the character state machine
	{
		HK_TIMER_BEGIN( "update character state", HK_NULL );

		for (int i=0; i < numCharacters; i++)
		{
			m_characterContexts[i]->update(input[i], output[i]);
		}

		HK_TIMER_END();

	}

	//Apply the character controllers
	{
		HK_TIMER_BEGIN( "simulate character", HK_NULL );

		for (int i=0; i < numCharacters; i++)
		{

			// Feed the output velocity from state machine into character rigid body
			m_characterRigidBodies[i]->setLinearVelocity(output[i].m_velocity, m_timestep);
			
			// If the character has fallen off the world we reposition them
			hkVector4 pos = m_characterRigidBodies[i]->getRigidBody()->getPosition();
			if (pos(1) < -10.0f)
			{
				pos.setZero4();
				pos(1) = 5;
				m_characterRigidBodies[i]->getRigidBody()->setPosition(pos);
			}

		}

		HK_TIMER_END();

		m_world->unlock();
	}

	// Step the world
	{
		hkDefaultPhysicsDemo::stepDemo();
	}

	HK_TIMER_END();

	return hkDemo::DEMO_OK;
}

#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
"A demo used to test the performance of multiple character rigid bodies";

HK_DECLARE_DEMO(MultipleCharacterRbsDemo, HK_DEMO_TYPE_PRIME, "CharacterTest", helpString);

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
