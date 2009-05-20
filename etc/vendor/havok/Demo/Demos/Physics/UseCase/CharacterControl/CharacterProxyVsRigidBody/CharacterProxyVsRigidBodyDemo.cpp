/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Common/Base/Container/LocalArray/hkLocalArray.h>

#include <Demos/Physics/UseCase/CharacterControl/CharacterProxyVsRigidBody/CharacterProxyVsRigidBodyDemo.h>

// Character rigid body
#include <Physics/Utilities/CharacterControl/CharacterRigidBody/hkpCharacterRigidBody.h>
// Character proxy
#include <Physics/Utilities/CharacterControl/CharacterProxy/hkpCharacterProxy.h>
// State machine
#include <Physics/Utilities/CharacterControl/StateMachine/hkpDefaultCharacterStates.h>

// Serialization
#include <Common/Base/Reflection/hkClass.h>
#include <Common/Serialize/Util/hkLoader.h>
#include <Common/Serialize/Util/hkRootLevelContainer.h>
#include <Common/Base/Reflection/Registry/hkTypeInfoRegistry.h>
#include <Physics/Utilities/Serialize/hkpPhysicsData.h>
#include <Common/Base/Container/LocalArray/hkLocalBuffer.h>

// Scene Data
#include <Common/SceneData/Scene/hkxScene.h>

#include <Physics/Collide/Shape/Convex/Capsule/hkpCapsuleShape.h>
#include <Physics/Dynamics/Phantom/hkpSimpleShapePhantom.h>
#include <Physics/Collide/Query/Collector/PointCollector/hkpClosestCdPointCollector.h>
#include <Physics/Collide/Query/Collector/PointCollector/hkpAllCdPointCollector.h>
#include <Physics/ConstraintSolver/Simplex/hkpSimplexSolver.h>
#include <Physics/Collide/Query/CastUtil/hkpLinearCastInput.h>

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


#include <Physics/Internal/Collide/Agent3/Machine/Nn/hkpAgentNnTrack.h>


// Ladder id
#define HK_OBJECT_IS_LADDER 1234
// Objects colors
#define NORMAL_GRAY				hkColor::rgbFromChars( 192, 192, 192, 255 )
#define TRANSPARENT_GREY		hkColor::rgbFromChars( 192, 192, 192, 125 )

// Up direction
static const hkVector4 UP (0,0,1);

// This custom listener  can be used to detect character proxy collision with ladder.
// The listener is inherited from hkpCharacterProxyListener.
class hkCharacterProxyForLadderListener : public hkReferencedObject, public hkpCharacterProxyListener
{
public:
	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO);

	hkCharacterProxyForLadderListener( ) 
		: m_atLadder(false)
	{}

	// Called after a contact point is created. 
	void contactPointAddedCallback(const hkpRootCdPoint& point)
	{
		hkpRigidBody* body = hkGetRigidBody(point.m_rootCollidableB);

		if ( body->hasProperty(HK_OBJECT_IS_LADDER) )
		{
			m_atLadder = true;
			m_ladderNorm = point.m_contact.getNormal();
			body->getPointVelocity(point.m_contact.getPosition(), m_ladderVelocity);
		}			
	}

	// Called before a contact point gets removed.
	void contactPointRemovedCallback(const hkpRootCdPoint& point)
	{
		hkpRigidBody* body = hkGetRigidBody(point.m_rootCollidableB);

		if ( body->hasProperty(HK_OBJECT_IS_LADDER) )
		{
			m_atLadder = false;
		}
	}

public:

	hkBool m_atLadder;

	hkVector4 m_ladderNorm;

	hkVector4 m_ladderVelocity;

};

CharacterProxyVsRigidBodyDemo::CharacterProxyVsRigidBodyDemo(hkDemoEnvironment* env)
:	hkDefaultPhysicsDemo(env)
{

	// Setup the camera
	{
		// disable back face culling
		setGraphicsState(HKG_ENABLED_CULLFACE, false);

		// don't really want shadows as makes it too dark
		forceShadowState(false);

		setupLights(m_env); // so that the extra lights are added

		// Allow to change object color after creation
		m_env->m_displayHandler->setAllowColorChangeOnPrecreated(true);

	}

	// Create the world
	{
		hkpWorldCinfo info;
		info.setBroadPhaseWorldSize( 350.0f );
		info.m_gravity.set(0,0,-9.8f);
		// NOTE: This small collision tolerance slightly improves the characters abilitiy to climb stairs smoothly.
		// Increase it to .1 to observe the effect
		info.m_collisionTolerance = 0.01f;

		m_world = new hkpWorld( info );
		m_world->lock();

		hkpAgentRegisterUtil::registerAllAgents(m_world->getCollisionDispatcher());

		setupGraphics();
	}

	// Setup layer collision
	{
		// Replace filter
		hkpGroupFilter* groupFilter = new hkpGroupFilter();

		// We disable collisions between characters
		groupFilter->disableCollisionsBetween(LAYER_CHARACTER_PROXY, LAYER_CHARACTER_RIGIDBODY);
		m_world->setCollisionFilter( groupFilter, true);
		groupFilter->removeReference();
	}

	// Load the level (scene)
	{
		m_loader = new hkLoader();

		hkString assetFile = hkAssetManagementUtil::getFilePath("Resources/Physics/levels/twosided_test_level.hkx");
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
					hkpRigidBody* body = system->getRigidBodies()[rb];
					body->setCollisionFilterInfo(hkpGroupFilter::calcFilterInfo( CharacterProxyVsRigidBodyDemo::LAYER_LANDSCAPE,0 ));
					const hkUlong id = hkUlong(body->getCollidable());
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
	
	// Add a ladder
	{
		hkVector4 baseSize( 1.0f, 0.5f, 3.6f);

		hkpRigidBodyCinfo rci;
		rci.m_shape = new hkpBoxShape( baseSize );
		rci.m_position.set(32.14f,-13.0 ,-4.5);
		rci.m_motionType = hkpMotion::MOTION_FIXED;
		hkpRigidBody* ladder = new hkpRigidBody(rci);
		rci.m_shape->removeReference();
		m_world->addEntity(ladder)->removeReference();

		// Add a property so we can identify this as a ladder
		hkpPropertyValue val(1);
		ladder->addProperty(HK_OBJECT_IS_LADDER, val);

		// Color the ladder so we can see it clearly
		HK_SET_OBJECT_COLOR((hkUlong)ladder->getCollidable(), 0x7f1f3f1f);
	}

	//  Create character shapes
	{
		// Construct a shape
		hkVector4 vertexA(0,0, 0.4f);
		hkVector4 vertexB(0,0,-0.4f);		

		// Create a capsule to represent the character standing
		m_standShape = new hkpCapsuleShape(vertexA, vertexB, .6f);

		// Create a capsule to represent the character crouching
		// Note that we create the smaller capsule with the base at the same position as the larger capsule.
		// This means we can simply swap the shapes without having to reposition the character proxy,
		// and if the character is standing on the ground, it will still be on the ground.
		vertexA.setZero4();
		m_crouchShape = new hkpCapsuleShape(vertexA, vertexB, .6f);
		
		// Load newShape (convex hull) for character rigid body
		{
			hkString assetFile = hkAssetManagementUtil::getFilePath("Resources/Physics/levels/character_new_shape.hkx");
			hkRootLevelContainer* container = m_loader->load( assetFile.cString() );
			HK_ASSERT2(0x27343437, container != HK_NULL , "Could not load asset");

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

					// only one shape
					hkpRigidBody* body = system->getRigidBodies()[0];
					m_newShape = const_cast<hkpShape*>(body->getCollidable()->getShape());
				}
			}
		}

	}

	//	Create a character proxy object
	{
		// Construct a shape phantom
		m_phantom = new hkpSimpleShapePhantom( m_standShape, hkTransform::getIdentity(), hkpGroupFilter::calcFilterInfo( CharacterProxyVsRigidBodyDemo::LAYER_CHARACTER_PROXY,0 ) );

		// Add the phantom to the world
		m_world->addPhantom(m_phantom);
		m_phantom->removeReference();

		// Construct a character proxy
		hkpCharacterProxyCinfo cpci;
		cpci.m_position.set(10.0f, 0.0f, -6.0f);
		cpci.m_staticFriction = 0.0f;
		cpci.m_dynamicFriction = 1.0f;
		cpci.m_up.setNeg4( m_world->getGravity() );
		cpci.m_up.normalize3();	
		cpci.m_userPlanes = 4;
		cpci.m_maxSlope = 65.0f * HK_REAL_DEG_TO_RAD;

		cpci.m_shapePhantom = m_phantom;
		m_characterProxy = new hkpCharacterProxy( cpci );

		// Add in a custom ladder listener
		{
			m_listenerProxy = new hkCharacterProxyForLadderListener();
			m_characterProxy->addCharacterProxyListener(m_listenerProxy);
		}
	}
	
	// Create the character proxy state machine and context
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

		m_characterProxyContext = new hkpCharacterContext(manager, HK_CHARACTER_ON_GROUND);
		manager->removeReference();
	}

	//	Create a character rigid body object
	{

		// Construct a character rigid body
		hkpCharacterRigidBodyCinfo info;
		info.m_mass = 100.0f;
		info.m_shape = m_standShape;
		info.m_friction = 0.0f;
		// NOTE: this allows smooth behaviour at all stairs in scene
		info.m_maxSlope = 65.0f * HK_REAL_DEG_TO_RAD;
		info.m_collisionFilterInfo = hkpGroupFilter::calcFilterInfo( CharacterProxyVsRigidBodyDemo::LAYER_CHARACTER_RIGIDBODY,0 );

		info.m_maxForce = 1000.0f;
		info.m_up = UP;
		info.m_position.set(10.0f, 0.0f, -6.0f);


		m_characterRigidBody = new hkpCharacterRigidBody( info );
		m_world->addEntity( m_characterRigidBody->getRigidBody() );

	}
	
	// Create the character rigid body state machine and context
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

		m_characterRigidBodyContext = new hkpCharacterContext(manager, HK_CHARACTER_ON_GROUND);
		m_characterRigidBodyContext->setCharacterType(hkpCharacterContext::HK_CHARACTER_RIGIDBODY);
		manager->removeReference();
	}

	// Initialize hkpSurfaceInfo for previous ground info holding
	m_previousGround = new hkpSurfaceInfo();
	m_framesInAir = 0;

	// Current controller active
	// 0 - both active
	// 1 - classical active
	// 2 - rigid body controller
	m_controllerSwitch = 0;

	// Current camera angle about up
	m_currentAngle = HK_REAL_PI * 0.5f;

	m_world->unlock();

}

CharacterProxyVsRigidBodyDemo::~CharacterProxyVsRigidBodyDemo()
{
	m_world->markForWrite();

	m_characterRigidBody->removeReference();

	m_characterProxy->removeCharacterProxyListener(m_listenerProxy);
	m_listenerProxy->removeReference();
	m_characterProxy->removeReference();
	
	m_standShape->removeReference();
	m_crouchShape->removeReference();

	delete m_characterRigidBodyContext;
	delete m_characterProxyContext;

	delete m_previousGround;


	// need to delete world (delete refs to bodies about to disappear)
	m_world->removeReference();
	m_world = HK_NULL;
	
	delete m_loader;
}

/////////////////////////////////////////////////////////////////////

hkDemo::Result CharacterProxyVsRigidBodyDemo::stepDemo()
{

	// Process inputs
	{
		const hkgPad* pad = m_env->m_gamePad;

		// Switch between controllers
		if ( pad->wasButtonPressed(HKG_PAD_BUTTON_1) )
		{
			if (++m_controllerSwitch > 2) 
			{
				m_controllerSwitch = 0; 
			}
		}

		// Move proxy controller to rigid body controller position
		if ( pad->wasButtonPressed(HKG_PAD_BUTTON_0) )
		{
			m_world->lock();
			m_characterProxy->setPosition(m_characterRigidBody->getPosition());
			m_world->unlock();
		}

		// Get user input data
		{
			hkReal deltaAngle = 0.f;
			CharacterUtils::getUserInputForCharacter(m_env, deltaAngle, m_posX, m_posY);
			m_currentAngle += deltaAngle;
			m_currentOrient.setAxisAngle(UP, m_currentAngle);
		}
	}
	
	// Simulate the world
	{
		m_world->lock();

		if ( m_controllerSwitch != 2 )
		{
			doCharacterProxy();			
		}

		if ( m_controllerSwitch != 1 )
		{
			doCharacterRigidBody();
		}

		m_world->unlock();

		// Step the world
		{
			hkDefaultPhysicsDemo::stepDemo();
		}

	}
		
	// Display character states
	{
		// Proxy controller state
		hkpCharacterStateType stateCC = m_characterProxyContext->getState();
		char * stateStrCC;

		switch (stateCC)
		{
			case HK_CHARACTER_ON_GROUND:
				stateStrCC = "On Ground";	break;
			case HK_CHARACTER_JUMPING:
				stateStrCC = "Jumping"; break;
			case HK_CHARACTER_IN_AIR:
				stateStrCC = "In Air"; break;
			case HK_CHARACTER_CLIMBING:
				stateStrCC = "Climbing"; break;
			default:
				stateStrCC = "Other";	break;
		}

		// Rigid body controller state
		hkpCharacterStateType stateRBC = m_characterRigidBodyContext->getState();
		char * stateStrRBC;

		switch (stateRBC)
		{
			case HK_CHARACTER_ON_GROUND:
				stateStrRBC = "On Ground";	break;
			case HK_CHARACTER_JUMPING:
				stateStrRBC = "Jumping"; break;
			case HK_CHARACTER_IN_AIR:
				stateStrRBC = "In Air"; break;
			case HK_CHARACTER_CLIMBING:
				stateStrRBC = "Climbing"; break;
			default:
				stateStrRBC = "Other";	break;
		}

		char buffer[255];
		hkString::snprintf(buffer, 255, "States: %s : %s", stateStrCC, stateStrRBC);
		m_env->m_textDisplay->outputText(buffer, 20, 450, 0xffffffff);


		// Display info about active controller
		char * stateInfo;

		switch (m_controllerSwitch)
		{
			case 1:
				stateInfo = "Proxy"; break;
			case 2:
				stateInfo = "Rigid Body"; break;
			default:
				stateInfo = "Both";	break;
		}

		hkString::snprintf(buffer, 255, "Active Character Controller: %s ", stateInfo);
		m_env->m_textDisplay->outputText(buffer, 20, 430, 0xffffffff);
	}
	
	// Camera Handling
	{
		m_world->lock();
		
		cameraHandling();

		m_world->unlock();
	}

	return hkDemo::DEMO_OK;
}

void CharacterProxyVsRigidBodyDemo::doCharacterProxy()
{

	hkpCharacterInput input;
	hkpCharacterOutput output;
	{
		input.m_inputLR = m_posX;
		input.m_inputUD = m_posY;

		input.m_wantJump =  m_env->m_window->getMouse().wasButtonPressed(HKG_MOUSE_LEFT_BUTTON);
		input.m_atLadder = m_listenerProxy->m_atLadder;

		input.m_up = UP;
		input.m_forward.set(1,0,0);
		input.m_forward.setRotatedDir( m_currentOrient, input.m_forward );

		input.m_stepInfo.m_deltaTime = m_timestep;
		input.m_stepInfo.m_invDeltaTime = 1.0f/m_timestep;
		input.m_characterGravity.set(0,0,-16);
		input.m_velocity = m_characterProxy->getLinearVelocity();
		input.m_position = m_characterProxy->getPosition();

		if (m_listenerProxy->m_atLadder)
		{
			hkVector4 right, ladderUp;
			right.setCross( UP, m_listenerProxy->m_ladderNorm );
			ladderUp.setCross( m_listenerProxy->m_ladderNorm, right );
			
			// Calculate the up vector for the ladder
			if (ladderUp.lengthSquared3() > HK_REAL_EPSILON)
			{
				ladderUp.normalize3();
			}

			// Reorient the forward vector so it points up along the ladder
			input.m_forward.addMul4( -m_listenerProxy->m_ladderNorm.dot3(input.m_forward), m_listenerProxy->m_ladderNorm);
			input.m_forward.add4( ladderUp );
			input.m_forward.normalize3();

			input.m_surfaceNormal = m_listenerProxy->m_ladderNorm;
			input.m_surfaceVelocity = m_listenerProxy->m_ladderVelocity;
		}
		else 
		{
			hkVector4 down;	down.setNeg4(UP);
			hkpSurfaceInfo ground;
			m_characterProxy->checkSupport(down, ground);
			input.m_isSupported = (ground.m_supportedState == hkpSurfaceInfo::SUPPORTED);
			input.m_surfaceNormal = ground.m_surfaceNormal;
			input.m_surfaceVelocity = ground.m_surfaceVelocity;
		}
	}

	// Apply the character state machine
	{
		m_characterProxyContext->update(input, output);
	}

	// Apply the player character controller
	{
		// Feed output from state machine into character proxy
		m_characterProxy->setLinearVelocity(output.m_velocity);

		hkStepInfo si;
		si.m_deltaTime = m_timestep;
		si.m_invDeltaTime = 1.0f/m_timestep;
		m_characterProxy->integrate( si, m_world->getGravity() );

	}

	// Handle crouching
	{
		hkBool wantCrouch = ( m_env->m_window->getMouse().getButtonState() & HKG_MOUSE_RIGHT_BUTTON )
			|| (m_env->m_gamePad->getButtonState() & HKG_PAD_BUTTON_2);

		hkBool isCrouching = m_phantom->getCollidable()->getShape() == m_crouchShape;


		// We want to stand
		if (isCrouching && !wantCrouch)
		{
			swapPhantomShape(m_standShape);
		}

		// We want to crouch
		if (!isCrouching && wantCrouch)
		{
			swapPhantomShape(m_crouchShape);
		}
	}

}

void CharacterProxyVsRigidBodyDemo::doCharacterRigidBody()
{
	hkBool atLadder = false;
	// Initialize these so gcc doesn't complain
	hkVector4 ladderNorm; ladderNorm.setZero4();
	hkVector4 ladderVelocity; ladderVelocity.setZero4();

	hkpLinkedCollidable* coll = m_characterRigidBody->getRigidBody()->getLinkedCollidable();
	for ( int i = 0; i < coll->m_collisionEntries.getSize(); ++i )
	{
		hkpRigidBody* rb = hkGetRigidBody( coll->m_collisionEntries[i].m_partner );
		if ( rb != HK_NULL && rb->hasProperty(HK_OBJECT_IS_LADDER) ) 
		{
			if ( coll->m_collisionEntries[i].m_agentEntry->m_contactMgr->m_type == hkpContactMgr::TYPE_SIMPLE_CONSTRAINT_CONTACT_MGR )
			{
				hkpSimpleConstraintContactMgr* mgr = (hkpSimpleConstraintContactMgr*)(coll->m_collisionEntries[i].m_agentEntry->m_contactMgr);
				if (mgr->m_contactConstraintData.getNumContactPoints() > 0)
				{
					atLadder = true;
					hkContactPoint* contactPoints = mgr->m_contactConstraintData.m_atom->getContactPoints();
					ladderNorm = contactPoints[0].getNormal();
					rb->getPointVelocity( contactPoints[0].getPosition(), ladderVelocity );
				}
			}
		}
	}



	hkpCharacterInput input;
	hkpCharacterOutput output;
	{
		input.m_inputLR = m_posX;
		input.m_inputUD = m_posY;

		input.m_wantJump =  m_env->m_window->getMouse().wasButtonPressed(HKG_MOUSE_LEFT_BUTTON);
		input.m_atLadder = atLadder;

		input.m_up = UP;
		input.m_forward.set(1,0,0);
		input.m_forward.setRotatedDir( m_currentOrient, input.m_forward );

		hkStepInfo stepInfo;
		stepInfo.m_deltaTime = m_timestep;
		stepInfo.m_invDeltaTime = 1.0f/m_timestep;
		
		input.m_stepInfo = stepInfo;

		input.m_characterGravity.set(0,0,-16);
		input.m_velocity = m_characterRigidBody->getLinearVelocity();
		input.m_position = m_characterRigidBody->getPosition();

		if ( atLadder )
		{
			hkVector4 right, ladderUp;
			right.setCross( UP, ladderNorm );
			ladderUp.setCross( ladderNorm, right );
			
			// Calculate the up vector for the ladder
			if (ladderUp.lengthSquared3() > HK_REAL_EPSILON)
			{
				ladderUp.normalize3();
			}

			// Reorient the forward vector so it points up along the ladder
			input.m_forward.addMul4( -ladderNorm.dot3(input.m_forward), ladderNorm);
			input.m_forward.add4( ladderUp );
			input.m_forward.normalize3();

			input.m_surfaceNormal = ladderNorm;
			input.m_surfaceVelocity = ladderVelocity;
		}
		else
		{

			hkpSurfaceInfo ground;
			m_characterRigidBody->checkSupport(stepInfo, ground);
			
			// Avoid accidental state changes (Smooth movement on stairs)
			// During transition supported->unsupported continue to return N-frames hkpSurfaceInfo data from previous supported state
			{

				// Number of frames to skip (continue with previous hkpSurfaceInfo data)
				const int skipFramesInAir = 3;

				if (input.m_wantJump)
				{
					m_framesInAir = skipFramesInAir;
				}

				if ( ground.m_supportedState != hkpSurfaceInfo::SUPPORTED )
				{
					if (m_framesInAir < skipFramesInAir)
					{
						input.m_isSupported = true;
						input.m_surfaceNormal = m_previousGround->m_surfaceNormal;
						input.m_surfaceVelocity = m_previousGround->m_surfaceVelocity;				
					}
					else
					{
						input.m_isSupported = false;
						input.m_surfaceNormal = ground.m_surfaceNormal;
						input.m_surfaceVelocity = ground.m_surfaceVelocity;	
					}			

					m_framesInAir++;

				}
				else
				{
					input.m_isSupported = true;
					input.m_surfaceNormal = ground.m_surfaceNormal;
					input.m_surfaceVelocity = ground.m_surfaceVelocity;

					m_previousGround->set(ground);

					// reset old number of frames
					if (m_framesInAir > skipFramesInAir)
					{
						m_framesInAir = 0;
					}			
				}

			}
			
		}

	}

	// Apply the character state machine
	{
		m_characterRigidBodyContext->update(input, output);		
	}

	// Apply the player character controller
	{
		// Feed output velocity from state machine into character rigid body
		m_characterRigidBody->setLinearVelocity(output.m_velocity, m_timestep);		
	}

	// Handle crouching
	{
		hkBool wantCrouch = ( m_env->m_window->getMouse().getButtonState() & HKG_MOUSE_RIGHT_BUTTON )
			|| (m_env->m_gamePad->getButtonState() & HKG_PAD_BUTTON_2);

		hkBool isCrouching = (m_characterRigidBody->getRigidBody()->getCollidable()->getShape() == m_crouchShape);


		// We want to stand
		if (isCrouching && !wantCrouch)
		{
			m_characterRigidBody->getRigidBody()->setShape(m_standShape);
		}

		// We want to crouch
		if (!isCrouching && wantCrouch)
		{
			m_characterRigidBody->getRigidBody()->setShape(m_crouchShape);
		}

	}

	/* 
	//		                         / \  
	// Change to the special shape 	[   ] 
	//						         \ /  
	*/
	{

		if ( m_env->m_gamePad->wasButtonPressed(HKG_PAD_BUTTON_3) )
		{
			if (m_characterRigidBody->getRigidBody()->getCollidable()->getShape() == m_newShape)
			{
				m_characterRigidBody->getRigidBody()->setShape(m_standShape);
			}
			else
			{
				m_characterRigidBody->getRigidBody()->setShape(m_newShape);
			}
		}

	}

}

void CharacterProxyVsRigidBodyDemo::cameraHandling()
{
	const hkReal height = .7f;
	hkVector4 forward;
	forward.set(1,0,0);
	forward.setRotatedDir( m_currentOrient, forward );

	hkVector4 from, to;

	switch (m_controllerSwitch)
	{
	case 1:
		to = m_characterProxy->getPosition(); break;
	case 2:
		to = m_characterRigidBody->getPosition(); break;
	default:
		to = m_characterRigidBody->getPosition();
		break;
	}
	to.addMul4(height, UP );

	hkVector4 dir;
	dir.setMul4( height, UP );
	dir.addMul4( -4.f, forward);

	from.setAdd4(to, dir);

	// Make object in the line of sight transparent
	{
		// Cast down to landscape to get an accurate position
		hkpWorldRayCastInput raycastIn;

		// Reverse direction for raycasting
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

		// Loop over all objects ids
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


void CharacterProxyVsRigidBodyDemo::swapPhantomShape( hkpShape* newShape )
{
	// Remember the current shape
	hkpShape* currentShape = const_cast<hkpShape*>(m_phantom->getCollidable()->getShape());

	// Swap to the new shape.
	// N.B. To be safe, we always remove the phantom from the world first, then change the shape,
	// then re-add, in order to refresh the cached agents in any hkCachingShapePhantoms which
	// may also be present in the world.
	// This also forces the display to be rebuilt, which is necessary for us to see the new shape!
	{
		// Note we do not have to add a reference before removing becasue we hold a hkpCharacterProxy
		// which has a reference to this phantom - hence removal from the world cannot cause this phantom to 
		// be accidentally deleted.
		m_world->removePhantom( m_phantom );
		m_phantom->setShape(newShape);
		m_world->addPhantom( m_phantom );
	}

	//
	// We use getClosestPoints to check for penetration
	//
	hkpClosestCdPointCollector collector;
	m_phantom->getClosestPoints( collector );

	// Allow a very slight tolerance (approx 1cm)

	if (collector.hasHit() && collector.getHit().m_contact.getDistance() < .01f)
	{
		// Switch the phantom back to our current shape.
		// N.B. To be safe, we always remove the phantom from the world first, then change the shape,
		// then re-add, in order to refresh the cached agents in any hkCachingShapePhantoms which
		// may also be present in the world.
		// This also forces the display to be rebuilt, which is necessary for us to see the new shape!	
		{
			// Note we do not have to add a reference before removing becasue we hold a hkpCharacterProxy
			// which has a reference to this phantom - hence removal from the world cannot cause this phantom to 
			// be accidentally deleted.
			m_world->removePhantom( m_phantom );
			m_phantom->setShape( currentShape );
			m_world->addPhantom( m_phantom );
		}
	}
}

#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
"This is our static scene for the character controllers testing" \
"This demo compares all the feature of the character rigid body controller " \
"and the classic character proxy controller including Walking, Jumping, Crouching and Climbing \n" \
"Controls:" \
"  Mouse / Analog Stick to turn \n" \
"  \224 / \225 Forward/Back \n" \
"  \221 Activate proxy/rigid body/both characters \n" \
"  \222 Crouch active character \n" \
"  \220 Merge position of characters (rigid body is the base)\n" \
"  \223 Switch to special shape for rigid body  \n";

HK_DECLARE_DEMO(CharacterProxyVsRigidBodyDemo, HK_DEMO_TYPE_PRIME, "RigidBody/Proxy Character Test", helpString);


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
