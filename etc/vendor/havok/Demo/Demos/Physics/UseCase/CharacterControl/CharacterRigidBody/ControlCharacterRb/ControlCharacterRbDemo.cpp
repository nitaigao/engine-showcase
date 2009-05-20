/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Common/Base/Container/LocalArray/hkLocalArray.h>

#include <Demos/Physics/UseCase/CharacterControl/CharacterRigidBody/ControlCharacterRb/ControlCharacterRbDemo.h>

// Used for
// character rigid body
#include <Physics/Utilities/CharacterControl/CharacterRigidBody/hkpCharacterRigidBody.h>
// character proxy
#include <Physics/Utilities/CharacterControl/CharacterProxy/hkpCharacterProxy.h>
// state machine
#include <Physics/Utilities/CharacterControl/StateMachine/hkpDefaultCharacterStates.h>

// Capsule shape
#include <Physics/Collide/Shape/Convex/Capsule/hkpCapsuleShape.h>

// Serialization
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

// Reducing collision tolerances between characters and fixed entities.
#include <Physics/Collide/Agent/hkpCollisionQualityInfo.h>
#include <Physics/Collide/Dispatch/hkpCollisionDispatcher.h>

#include <Physics/Internal/Collide/Agent3/Machine/Nn/hkpAgentNnTrack.h>


// Ladder property
#define HK_OBJECT_IS_LADDER 1234
// Level colors (
#define NORMAL_GRAY				hkColor::rgbFromChars( 192, 192, 192, 255 )
#define TRANSPARENT_GREY		hkColor::rgbFromChars( 192, 192, 192, 125 )

static const hkVector4 UP (0,0,1);


ControlCharacterRbDemo::ControlCharacterRbDemo(hkDemoEnvironment* env)
:	hkDefaultPhysicsDemo(env)
{

	// Setup graphics
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
		info.m_contactPointGeneration = hkpWorldCinfo::CONTACT_POINT_ACCEPT_ALWAYS;
		m_world = new hkpWorld( info );
		m_world->lock();

		hkpAgentRegisterUtil::registerAllAgents(m_world->getCollisionDispatcher());
		// The world has the default collision tolerance of 0.1f. Decreasing the collision tolerance between characters
		// and fixed entities as follows improves character rigid bodies' ability to climb stairs smoothly.
		{
			const hkReal characterTolerance = 0.01f;
			hkpCollisionQualityInfo& characterCollisionQuality = m_world->m_collisionDispatcher->m_collisionQualityInfo[ hkpCollisionDispatcher::COLLISION_QUALITY_CHARACTER ];
			characterCollisionQuality.m_manifoldTimDistance = characterTolerance;
			characterCollisionQuality.m_createContact = characterTolerance;
			characterCollisionQuality.m_keepContact = characterTolerance;
			characterCollisionQuality.m_create4dContact = characterTolerance;
		}

		setupGraphics();
	}

	// Load the level
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

	//	load newShape (convex hull) for character from file
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

				// Take only the first shape
				hkpRigidBody* body = system->getRigidBodies()[0];

				m_newShape = const_cast<hkpShape*>(body->getCollidable()->getShape());

			}
		}

	}

	//	Create a character rigid body object
	{
		// Construct a shape
		hkVector4 vertexA(0,0, 0.4f);
		hkVector4 vertexB(0,0,-0.4f);

		// Create a capsule to represent the character standing
		m_standShape = new hkpCapsuleShape(vertexA, vertexB, .6f);

		// Create a capsule to represent the character crouching
		// Note that we create the smaller capsule with the base at the same position as the larger capsule.		
		vertexA.setZero4();
		m_crouchShape = new hkpCapsuleShape(vertexA, vertexB, .6f);


		// Construct a character rigid body
		hkpCharacterRigidBodyCinfo info;
		info.m_mass = 100.0f;
		info.m_shape = m_standShape;

		info.m_maxForce = 1000.0f;
		info.m_up = UP;
		info.m_position.set(10.0f, 0.0f, -6.0f);
		info.m_maxSlope = 70.0f * HK_REAL_DEG_TO_RAD;
		info.m_supportDistance = 0.01f;


		m_characterRigidBody = new hkpCharacterRigidBody( info );
		m_world->addEntity( m_characterRigidBody->getRigidBody() );

	}
	
	// Create the Character state machine and context
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
		manager->removeReference();

		// Set character type
		m_characterContext->setCharacterType(hkpCharacterContext::HK_CHARACTER_RIGIDBODY);
	}

	// Initialize hkpSurfaceInfo for old ground info holding
	m_previousGround = new hkpSurfaceInfo();
	m_framesInAir = 0;

	// Current camera angle about up
	m_currentAngle = HK_REAL_PI * 0.5f;	

	m_world->unlock();
}

ControlCharacterRbDemo::~ControlCharacterRbDemo()
{
	m_world->markForWrite();

	m_characterRigidBody->removeReference();

	m_standShape->removeReference();
	m_crouchShape->removeReference();

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

hkDemo::Result ControlCharacterRbDemo::stepDemo()
{
	{
		m_world->lock();

		//	Get user input data
		hkReal posX = 0.f;
		hkReal posY = 0.f;
		{
			float deltaAngle = 0.f;
			CharacterUtils::getUserInputForCharacter(m_env, deltaAngle, posX, posY);
			m_currentAngle += deltaAngle;
			m_currentOrient.setAxisAngle(UP, m_currentAngle);
		}
		
		//
		// Detect ladder
		//
		hkBool atLadder = false;
		// Initialize these so gcc doesn't complain
		hkVector4 ladderNorm; ladderNorm.setZero4();
		hkVector4 ladderVelocity; ladderVelocity.setZero4();

		// Process all collisions points to see if there is a collision at a ladder. This could be done in a callback, however
		// it is done this way here to ensure compatibility with SPU simulation.
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
						break;
					}
				}
			}
		}

		HK_TIMER_BEGIN( "set character state", HK_NULL );

		hkpCharacterInput input;
		hkpCharacterOutput output;
		{
			input.m_inputLR = posX;
			input.m_inputUD = posY;

			input.m_wantJump =  m_env->m_window->getMouse().wasButtonPressed(HKG_MOUSE_LEFT_BUTTON)
				|| m_env->m_gamePad->wasButtonPressed(HKG_PAD_BUTTON_1);
			input.m_atLadder = atLadder;

			input.m_up = UP;
			input.m_forward.set(1,0,0);
			input.m_forward.setRotatedDir( m_currentOrient, input.m_forward );

			hkStepInfo stepInfo;
			stepInfo.m_deltaTime = m_timestep;
			stepInfo.m_invDeltaTime = 1.0f/m_timestep;
			
			input.m_stepInfo = stepInfo;

			input.m_characterGravity.set(0,0,-16);
			input.m_velocity = m_characterRigidBody->getRigidBody()->getLinearVelocity();
			input.m_position = m_characterRigidBody->getRigidBody()->getPosition();

			if (atLadder)
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

				// Change character rigid body color according to its state
				if( ground.m_supportedState == hkpSurfaceInfo::SUPPORTED )
				{
					HK_SET_OBJECT_COLOR( (hkUlong) m_characterRigidBody->getRigidBody()->getCollidable(), hkColor::GREEN );
				}
				else
				{
					HK_SET_OBJECT_COLOR( (hkUlong) m_characterRigidBody->getRigidBody()->getCollidable(), hkColor::BLUE );
				}

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
							input.m_surfaceMotionType = m_previousGround->m_surfaceMotionType;
						}
						else
						{
							input.m_isSupported = false;
							input.m_surfaceNormal = ground.m_surfaceNormal;
							input.m_surfaceVelocity = ground.m_surfaceVelocity;
							input.m_surfaceMotionType = ground.m_surfaceMotionType;
						}			

						m_framesInAir++;
					}
					else
					{
						input.m_isSupported = true;
						input.m_surfaceNormal = ground.m_surfaceNormal;
						input.m_surfaceVelocity = ground.m_surfaceVelocity;
						input.m_surfaceMotionType = ground.m_surfaceMotionType;

						m_previousGround->set(ground);

						// reset old number of frames
						if (m_framesInAir > skipFramesInAir)
						{
							m_framesInAir = 0;
						}			

					}
				}
			}

			HK_TIMER_END();
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

			// Set output velocity from state machine into character rigid body
			m_characterRigidBody->setLinearVelocity(output.m_velocity, m_timestep);

			HK_TIMER_END();

			m_world->unlock();
		}

		// Step the world
		{
			hkDefaultPhysicsDemo::stepDemo();
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
		
		//
		// Handle crouching (only for capsule)
		//
		{
			m_world->lock();
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

			m_world->unlock();
		}

		//                           /\  //
		// change to the new shape	[  ] //
		//						     \/  //
		{

			if ( m_env->m_gamePad->wasButtonPressed(HKG_PAD_BUTTON_3) )
			{
				m_world->lock();

				if (m_characterRigidBody->getRigidBody()->getCollidable()->getShape() == m_newShape)
				{
					m_characterRigidBody->getRigidBody()->setShape(m_standShape);
				}
				else
				{
					m_characterRigidBody->getRigidBody()->setShape(m_newShape);
				}
				m_world->unlock();
			}

		}
		
	}

	// Transparent camera handling
	{
		m_world->lock();

		cameraHandling();

		m_world->unlock();
	}

	return hkDemo::DEMO_OK;
}

void ControlCharacterRbDemo::cameraHandling()
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
	dir.addMul4( -4.f, forward);

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

		// 
		hkLocalArray<hkUlong>	transp(5);

		// Loop over all collected objects
		for(int i=0; i < collector.getHits().getSize();i++)
		{
			hkpWorldRayCastOutput raycastOut = collector.getHits()[i];

			transp.pushBack(hkUlong(raycastOut.m_rootCollidable));
		}

		// Loop over all objects display ids
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
"This is our static scene for the character controller testing " \
"This demo shows all the feature of the character rigid body controller including " \
"walking, jumping, crouching and climbing \n" \
"Controls:" \
"  Mouse / Analog Stick to turn \n" \
"  \224 / \225 Forward/Back \n" \
"  LMB / \221 to Jump \n" \
"  RMB / \222 to Crouch \n"\
"      / \223 to Change to special shape\n";

HK_DECLARE_DEMO(ControlCharacterRbDemo, HK_DEMO_TYPE_PRIME, "CharacterTest", helpString);


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
