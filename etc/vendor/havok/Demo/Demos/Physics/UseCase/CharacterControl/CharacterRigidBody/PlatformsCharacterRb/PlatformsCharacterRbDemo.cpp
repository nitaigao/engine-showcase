/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Common/Base/Container/LocalArray/hkLocalArray.h>

#include <Demos/Physics/UseCase/CharacterControl/CharacterRigidBody/PlatformsCharacterRb/PlatformsCharacterRbDemo.h>

// Used for
// character rigid body
#include <Physics/Utilities/CharacterControl/CharacterRigidBody/hkpCharacterRigidBody.h>
// character proxy
#include <Physics/Utilities/CharacterControl/CharacterProxy/hkpCharacterProxy.h>
// state machine
#include <Physics/Utilities/CharacterControl/StateMachine/hkpDefaultCharacterStates.h>
// capsule shape
#include <Physics/Collide/Shape/Convex/Capsule/hkpCapsuleShape.h>

// Used for serialization
#include <Common/Base/Reflection/hkClass.h>
#include <Common/Serialize/Util/hkLoader.h>
#include <Common/Serialize/Util/hkRootLevelContainer.h>
#include <Common/Base/Reflection/Registry/hkTypeInfoRegistry.h>
#include <Physics/Utilities/Serialize/hkpPhysicsData.h>
#include <Common/Base/Container/LocalArray/hkLocalBuffer.h>

// Scene Data
#include <Common/SceneData/Scene/hkxScene.h>

// Camera handling
#include <Common/Visualize/hkDebugDisplay.h>
#include <Graphics/Bridge/DisplayHandler/hkgDisplayHandler.h>
#include <Graphics/Bridge/SceneData/hkgSceneDataConverter.h>
#include <Graphics/Common/DisplayObject/hkgDisplayObject.h>
// Transparency raycasting
#include <Physics/Dynamics/Collide/hkpCollisionListener.h>
#include <Physics/Collide/Query/CastUtil/hkpWorldRayCastInput.h>
#include <Physics/Collide/Query/CastUtil/hkpWorldRayCastOutput.h>
#include <Physics/Collide/Query/Collector/RayCollector/hkpAllRayHitCollector.h>
// Common
#include <Demos/DemoCommon/Utilities/Asset/hkAssetManagementUtil.h>
#include <Demos/DemoCommon/Utilities/GameUtils/GameUtils.h>
#include <Demos/DemoCommon/Utilities/Character/CharacterUtils.h>
#include <Demos/DemoCommon/DemoFramework/hkTextDisplay.h>

// Level colors
#define NORMAL_GRAY				hkColor::rgbFromChars( 192, 192, 192, 255 )
#define TRANSPARENT_GREY		hkColor::rgbFromChars( 192, 192, 192, 125 )

static const hkVector4 UP (0,0,1);

PlatformsCharacterRbDemo::PlatformsCharacterRbDemo(hkDemoEnvironment* env)
:	hkDefaultPhysicsDemo(env)
{

	// Setup the graphics
	{
		// Disable back face culling
		setGraphicsState(HKG_ENABLED_CULLFACE, false);

		// don't really want shadows as makes it too dark
		forceShadowState(false);

		setupLights(m_env); // so that the extra lights are added

		// allow color change on precreated objects
		m_env->m_displayHandler->setAllowColorChangeOnPrecreated(true);
	}

	// Create the world
	{
		hkpWorldCinfo info;
		info.setBroadPhaseWorldSize( 350.0f );
		info.m_gravity.set(0,0,-9.8f);
		info.m_collisionTolerance = 0.01f;
		m_world = new hkpWorld( info );
		m_world->lock();

		hkpAgentRegisterUtil::registerAllAgents(m_world->getCollisionDispatcher());

		setupGraphics();
	}

	// Load the level
	{
		m_loader = new hkLoader();

		hkString assetFile = hkAssetManagementUtil::getFilePath("Resources/Physics/levels/test_platform.hkx");
		hkRootLevelContainer* container = m_loader->load( assetFile.cString() );
		HK_ASSERT2(0x27343437, container != HK_NULL , "Could not load asset");
		hkxScene* scene = reinterpret_cast<hkxScene*>( container->findObjectByType( hkxSceneClass.getName() ));

		HK_ASSERT2(0x27343635, scene, "No scene loaded");
		env->m_sceneConverter->convert( scene, hkgAssetConverter::CONVERT_ALL );

		hkpPhysicsData* physics = reinterpret_cast<hkpPhysicsData*>( container->findObjectByType( hkpPhysicsDataClass.getName() ));
		HK_ASSERT2(0x27343635, physics, "No physics loaded");

		// Physics
		if (physics)
		{
			const hkArray<hkpPhysicsSystem*>& psys = physics->getPhysicsSystems();

			// Tie the two together
			for (int i=0; i<psys.getSize(); i++)
			{
				hkpPhysicsSystem* system = psys[i];

				// Change the layer of the rigid bodies
				for (int rb=0; rb < system->getRigidBodies().getSize(); rb++)
				{
					const hkUlong id = hkUlong(system->getRigidBodies()[rb]->getCollidable());
					HK_SET_OBJECT_COLOR(id,NORMAL_GRAY);
					m_objectIds.pushBack(id);
				}

				// Associate the display and physics (by name)
				if (scene)
				{
					addPrecreatedDisplayObjectsByName( psys[i]->getRigidBodies(), scene );
				}

				// add the lot to the world
				m_world->addPhysicsSystem(system);
			}
		}
	}

	// Add horizontal keyframed platform
	{

		hkpShape* platform = new hkpBoxShape(hkVector4(1.5,2.5,0.25));

		hkpRigidBodyCinfo rbci;
		rbci.m_shape = platform;
		rbci.m_motionType = hkpMotion::MOTION_KEYFRAMED;
		rbci.m_position.set(2.5f, 0.0f, 0.25f);
		rbci.m_friction = 1.0f;
		rbci.m_collisionFilterInfo = hkpGroupFilter::calcFilterInfo(hkpGroupFilter::calcFilterInfo(1));

		m_horPlatform = new hkpRigidBody(rbci);

		platform->removeReference();
		m_world->addEntity(m_horPlatform);
		m_horPlatform->removeReference();

	}

	// Add vertical keyframed platform
	{
		hkpShape* platform = new hkpBoxShape(hkVector4(1.5,2.5,0.25));

		hkpRigidBodyCinfo rbci;
		rbci.m_shape = platform;
		rbci.m_motionType = hkpMotion::MOTION_KEYFRAMED;
		rbci.m_position.set(-3.5f, 0.0f, 3.25f);
		rbci.m_friction = 1.0f;
		rbci.m_collisionFilterInfo = hkpGroupFilter::calcFilterInfo(hkpGroupFilter::calcFilterInfo(1));

		m_verPlatform = new hkpRigidBody(rbci);

		platform->removeReference();
		m_world->addEntity(m_verPlatform);
		m_verPlatform->removeReference();
	}

	//	Create a character rigid body
	{
		// Create a capsule to represent the character standing
		hkVector4 vertexA(0,0, 0.4f);
		hkVector4 vertexB(0,0,-0.4f);
	
		m_standShape = new hkpCapsuleShape(vertexA, vertexB, .6f);

		// Construct a character rigid body
		hkpCharacterRigidBodyCinfo info;
		info.m_mass = 100.0f;
		info.m_shape = m_standShape;

		info.m_maxForce = 1000.0f;
		info.m_up = UP;
		info.m_position.set(0.0f, 5.0f, 1.5f);
		info.m_maxSlope = HK_REAL_PI/2.0f;

		m_characterRigidBody = new hkpCharacterRigidBody( info );
		m_world->addEntity( m_characterRigidBody->getRigidBody() );

	}
	
	// Create the character state machine
	{
		hkpCharacterState* state;
		hkpCharacterStateManager* manager = new hkpCharacterStateManager();

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

		m_characterContext = new hkpCharacterContext(manager, HK_CHARACTER_ON_GROUND);
		m_characterContext->setCharacterType(hkpCharacterContext::HK_CHARACTER_RIGIDBODY);
		manager->removeReference();
	}

	// Set colors of platforms
	HK_SET_OBJECT_COLOR(hkUlong(m_verPlatform->getCollidable()),hkColor::BLUE);
	HK_SET_OBJECT_COLOR(hkUlong(m_horPlatform->getCollidable()),hkColor::GREEN);

	// Set global time
	m_time = 0.0f;

	// Initialize hkpSurfaceInfo for previous ground 
	m_previousGround = new hkpSurfaceInfo();
	m_framesInAir = 0;

	// Current camera angle about up
	m_currentAngle = HK_REAL_PI * 0.5f;

	m_world->unlock();
}

PlatformsCharacterRbDemo::~PlatformsCharacterRbDemo()
{
	m_world->markForWrite();

	m_characterRigidBody->removeReference();

	m_standShape->removeReference();
	
	delete m_characterContext;

	delete m_previousGround;

	// need to delete world before we wipe the loader
	if(m_world)
	{
		m_world->removeReference();
		m_world = HK_NULL;
	}

	delete m_loader;
}

/////////////////////////////////////////////////////////////////////

hkDemo::Result PlatformsCharacterRbDemo::stepDemo()
{
	// Process inputs, calculate state, set new velocity
	{
		m_world->lock();

		// update total run time
		m_time += m_timestep;

		//	Get user input data
		hkReal posX = 0.f;
		hkReal posY = 0.f;
		{
			float deltaAngle = 0.f;
			CharacterUtils::getUserInputForCharacter(m_env, deltaAngle, posX, posY);
			m_currentAngle += deltaAngle;
			m_currentOrient.setAxisAngle(UP, m_currentAngle);
		}

		hkpCharacterInput input;
		hkpCharacterOutput output;
		{
			input.m_inputLR = posX;
			input.m_inputUD = posY;

			input.m_wantJump =  m_env->m_window->getMouse().wasButtonPressed(HKG_MOUSE_LEFT_BUTTON)
				|| m_env->m_gamePad->wasButtonPressed(HKG_PAD_BUTTON_1);
			input.m_atLadder = false;

			input.m_up = UP;
			input.m_forward.set(1,0,0);
			input.m_forward.setRotatedDir( m_currentOrient, input.m_forward );

			hkStepInfo stepInfo;
			stepInfo.m_deltaTime = m_timestep;
			stepInfo.m_invDeltaTime = 1.0f/m_timestep;
			stepInfo.m_endTime = m_time;

			input.m_stepInfo = stepInfo;
			input.m_characterGravity.set(0,0,-16);
			input.m_velocity = m_characterRigidBody->getLinearVelocity();
			input.m_position = m_characterRigidBody->getPosition();

			hkpSurfaceInfo ground;
			m_characterRigidBody->checkSupport(stepInfo,ground);

			input.m_isSupported = (ground.m_supportedState == hkpSurfaceInfo::SUPPORTED);
			input.m_surfaceNormal = ground.m_surfaceNormal;
			input.m_surfaceVelocity = ground.m_surfaceVelocity;
			input.m_surfaceMotionType = ground.m_surfaceMotionType;
			
		}

		// Apply the character state machine
		{
			HK_TIMER_BEGIN( "update character state", HK_NULL );

			m_characterContext->update(input, output);

			HK_TIMER_END();
		}

		//Apply the player character controller
		{
			HK_TIMER_BEGIN( "simulate character", HK_NULL );

			// Feed output velocity from state machine into character rigid body
			m_characterRigidBody->setLinearVelocity(output.m_velocity, m_timestep);

			HK_TIMER_END();
		}

		// Display state
		{
			hkpCharacterStateType state = m_characterContext->getState();
			char * stateStr;

			switch (state)
			{
			case HK_CHARACTER_ON_GROUND:
				stateStr = "On Ground";	break;
			case HK_CHARACTER_JUMPING:
				stateStr = "Jumping"; break;
			case HK_CHARACTER_IN_AIR:
				stateStr = "In Air"; break;
			case HK_CHARACTER_CLIMBING:
				stateStr = "Climbing"; break;
			default:
				stateStr = "Other";	break;
			}
			char buffer[255];
			hkString::snprintf(buffer, 255, "State : %s", stateStr);
			m_env->m_textDisplay->outputText(buffer, 20, 270, 0xffffffff);
		}

		m_world->unlock();
	}

	// Step the world
	{
		hkDefaultPhysicsDemo::stepDemo();
	}
	
	// Move the platforms
	{
		m_world->lock();

		moveHorizontalPlatform();

		moveVerticalPlatform();

		m_world->unlock();
	}

	// Transparent camera handling
	{
		m_world->lock();

		cameraHandling();

		m_world->unlock();
	}

	return hkDemo::DEMO_OK;
}

void PlatformsCharacterRbDemo::moveHorizontalPlatform()
{
	const hkReal velAplitude = 3.0f; 
	const hkReal velOmega = 1.0f;

	hkVector4 newVelocity; newVelocity = m_horPlatform->getLinearVelocity();
	newVelocity(0) = velAplitude*cos(velOmega*m_time);
	m_horPlatform->setLinearVelocity(newVelocity);
}

void PlatformsCharacterRbDemo::moveVerticalPlatform()
{
	const hkReal velAplitude = 3.0f; 
	const hkReal velOmega = 1.0f;

	hkVector4 newVelocity; newVelocity = m_verPlatform->getLinearVelocity();
	newVelocity(2) = velAplitude*cos(velOmega*m_time);
	m_verPlatform->setLinearVelocity(newVelocity);
}

void PlatformsCharacterRbDemo::cameraHandling()
{
	const hkReal height = .7f;
	hkVector4 forward;
	forward.set(1,0,0);
	forward.setRotatedDir( m_currentOrient, forward );

	hkVector4 from, to;
	to = m_characterRigidBody->getPosition();
	to.addMul4(height, UP );

	hkVector4 dir;
	dir.setMul4( height, UP );
	dir.addMul4( -8.f, forward);

	from.setAdd4(to, dir);

	// Make object in line of sight transparent
	{
		// Cast down to landscape to get an accurate position
		hkpWorldRayCastInput raycastIn;

		// Reverse direction for collision detection
		raycastIn.m_from = to;
		raycastIn.m_to = from;
		raycastIn.m_filterInfo = hkpGroupFilter::calcFilterInfo(0);

		hkpAllRayHitCollector collector;

		m_world->castRay( raycastIn, collector);

		hkLocalArray<hkUlong>	transp(5);

		// Loop over all collected objects
		for(int i=0; i < collector.getHits().getSize();i++)
		{
			hkpWorldRayCastOutput raycastOut = collector.getHits()[i];

			transp.pushBack(hkUlong(raycastOut.m_rootCollidable));
		}

		// loop over display ids
		for(int i=0; i < m_objectIds.getSize();i++)
		{
			if(transp.indexOf(m_objectIds[i]) != -1)
			{
				HK_SET_OBJECT_COLOR(m_objectIds[i], TRANSPARENT_GREY);
			}
			else
			{
				HK_SET_OBJECT_COLOR(m_objectIds[i],NORMAL_GRAY);
			}
		}

	}

	setupDefaultCameras(m_env, from , to, UP, 1.0f);
}

#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
"This demo shows an interaction of the character rigid body " \
"with vertical and horizontal moving keyframed platform. " \
"Controls:" \
"  Mouse / Analog Stick to turn \n" \
"  \224 / \225 Forward/Back \n" \
"  LMB / \221 to Jump \n"; 
 


HK_DECLARE_DEMO(PlatformsCharacterRbDemo, HK_DEMO_TYPE_PRIME, "CharacterTest", helpString);


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
