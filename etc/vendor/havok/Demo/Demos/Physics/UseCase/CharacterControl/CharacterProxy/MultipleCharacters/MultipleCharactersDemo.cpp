/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/Physics/UseCase/CharacterControl/CharacterProxy/MultipleCharacters/MultipleCharactersDemo.h>
#include <Demos/DemoCommon/Utilities/GameUtils/Landscape/LandscapeRepository.h>

#include <Common/Base/Monitor/hkMonitorStream.h>
#include <Common/Base/Container/LocalArray/hkLocalArray.h>

#include <Physics/Collide/Shape/Convex/Capsule/hkpCapsuleShape.h>

#include <Physics/Dynamics/Phantom/hkpSimpleShapePhantom.h>

#include <Physics/Utilities/CharacterControl/CharacterProxy/hkpCharacterProxy.h>
#include <Physics/Utilities/CharacterControl/StateMachine/hkpDefaultCharacterStates.h>

struct MultipleCharactersVariant
{
	const char*	m_name;
	const char*	m_landscapeType;
	const char* m_details;
};

static const MultipleCharactersVariant g_variants[] = 
{
	{ "100 Characters on 4800-Triangle Landscape", "simpleFlatLandOneMetreTris", "CharacterTest" },
	{ "100 Characters on 79000-Triangle Landscape", "defaultFlatLandOneMetreTris", "CharacterTest" },
};



MultipleCharactersDemo::MultipleCharactersDemo(hkDemoEnvironment* env)
:	hkDefaultPhysicsDemo(env)
{
	const MultipleCharactersVariant& variant = g_variants[m_variantId];

	//
	// Setup the camera
	//
	{
		hkVector4 from(  0.0f, 20.0f, -80.0f);
		hkVector4 to  (  0.0f,  0.0f,   0.0f);
		hkVector4 up  (  0.0f,  1.0f,   0.0f);
		setupDefaultCameras( env, from, to, up );
		
		forceShadowState(false);

	}

	//
	// Create the world
	//
	{
		hkpWorldCinfo info;
		info.setBroadPhaseWorldSize( 350.0f );  
		info.m_gravity.set(0, -9.8f, 0);
		info.m_collisionTolerance = 0.1f;
		m_world = new hkpWorld( info );
		m_world->lock();

		hkpAgentRegisterUtil::registerAllAgents(m_world->getCollisionDispatcher());

		setupGraphics();
	}


	//
	// Create the ground using the landscape repository
	//
	{
		int landscapeIndex = LandscapeRepository::getInstance().getLandscapeIndexByName(variant.m_landscapeType);
		m_landscapeContainer = LandscapeRepository::getInstance().getLandscape(landscapeIndex);

		hkpRigidBodyCinfo groundInfo;
		groundInfo.m_motionType = hkpMotion::MOTION_FIXED;
		groundInfo.m_shape = m_landscapeContainer->m_shape;

		hkpRigidBody* groundbody = new hkpRigidBody(groundInfo);

		m_world->addEntity(groundbody)->removeReference();

		setupDefaultCameras( env, m_landscapeContainer->m_cameraFrom, m_landscapeContainer->m_cameraTo, hkVector4(0,1,0) ); 
	}

	AabbSpawnUtil spawnUtil( m_landscapeContainer->m_spawnVolumes );



	//
	//	Create character proxy objects
	//

	const int numCharacters = 100;

	m_characterProxy.setSize( numCharacters );
	{
		hkVector4 top(0, .4f, 0);
		hkVector4 bottom(0,-.4f, 0);		

		// Create a capsule to represent the character standing
		hkpShape* characterShape = new hkpCapsuleShape(top, bottom, .6f);

		for (int i=0; i < numCharacters; i++)
		{

			hkpShapePhantom* phantom = new hkpSimpleShapePhantom( characterShape, hkTransform::getIdentity() );

			// Add the phantom to the world
			m_world->addPhantom(phantom);
			phantom->removeReference();


			// Construct a character proxy
			hkpCharacterProxyCinfo cpci;
			spawnUtil.getNewSpawnPosition( hkVector4(1, 2, 1), cpci.m_position );

			cpci.m_up.setNeg4( m_world->getGravity() );
			cpci.m_up.normalize3();

			cpci.m_shapePhantom = phantom;
			m_characterProxy[i] = new hkpCharacterProxy( cpci );

		}
		characterShape->removeReference();
	}
	
	//
	// Create the Character state machine and context
	//
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
	m_characterContext.setSize(numCharacters);
	{
		for (int i=0; i < numCharacters; i++)
		{
			m_characterContext[i] = new hkpCharacterContext(manager, HK_CHARACTER_ON_GROUND);
		}
		manager->removeReference();
	}
	
	//Initialised the round robin counter
	m_tick = 0;

	m_world->unlock();
}

MultipleCharactersDemo::~MultipleCharactersDemo()
{
	m_world->lock();

	for (int i=0; i < m_characterProxy.getSize(); i++)
	{
		m_characterProxy[i]->removeReference();

		delete m_characterContext[i];
	}

	m_world->unlock();
}


hkDemo::Result MultipleCharactersDemo::stepDemo()
{
	HK_TIMER_BEGIN( "Whole simulation step", HK_NULL );

	m_world->lock();

	hkVector4 up;
	up.setNeg4( m_world->getGravity() );
	up.normalize3();
	int numCharacters = m_characterProxy.getSize();

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

			input[i].m_stepInfo.m_deltaTime = m_timestep;
			input[i].m_stepInfo.m_invDeltaTime = 1.0f / (m_timestep);
			input[i].m_characterGravity = m_world->getGravity();
			input[i].m_velocity = m_characterProxy[i]->getLinearVelocity();
			input[i].m_position = m_characterProxy[i]->getPosition();

			hkVector4 down;	down.setNeg4(up);
			hkpSurfaceInfo ground;
			m_characterProxy[i]->checkSupport(down, ground);

			input[i].m_isSupported = ground.m_supportedState == hkpSurfaceInfo::SUPPORTED;
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
			m_characterContext[i]->update(input[i], output[i]);
		}

		HK_TIMER_END();

	}

	//Apply the character controllers
	{
		HK_TIMER_BEGIN( "simulate character", HK_NULL );

		for (int i=0; i < numCharacters; i++)
		{
			hkStepInfo csi;
			csi.m_deltaTime = m_timestep;
			csi.m_invDeltaTime = 1.0f / m_timestep;

			// Feed output from state machine into character proxy
			m_characterProxy[i]->setLinearVelocity(output[i].m_velocity);
			m_characterProxy[i]->integrate( csi, m_world->getGravity() );

			// If the character has fallen off the world we reposition them
			hkVector4 pos = m_characterProxy[i]->getPosition();
			if (pos(1) < -10.0f)
			{
				pos.setZero4();
				pos(1) = 5;
				m_characterProxy[i]->setPosition(pos);
			}
		}

		HK_TIMER_END();
	}

	m_world->unlock();

	// Step the world
	hkDefaultPhysicsDemo::stepDemo();

	HK_TIMER_END();

	return hkDemo::DEMO_OK;
}

#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
"A demo used to test the performance of multiple characters";

HK_DECLARE_DEMO_VARIANT(MultipleCharactersDemo, HK_DEMO_TYPE_PRIME | HK_DEMO_TYPE_CRITICAL | HK_DEMO_TYPE_STATS, g_variants[0].m_name, 0, helpString, g_variants[0].m_details);
HK_DECLARE_DEMO_VARIANT(MultipleCharactersDemo, HK_DEMO_TYPE_PRIME                         | HK_DEMO_TYPE_STATS, g_variants[1].m_name, 1, helpString, g_variants[1].m_details);

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
