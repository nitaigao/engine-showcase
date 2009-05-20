/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Common/Base/Monitor/hkMonitorStream.h>

#include <Demos/Physics/UseCase/CharacterControl/CharacterProxy/CharacterController/CharacterControllerDemo.h>

#include <Physics/Utilities/CharacterControl/CharacterProxy/hkpCharacterProxy.h>
#include <Physics/Utilities/CharacterControl/StateMachine/hkpDefaultCharacterStates.h>

#include <Physics/Collide/Shape/Convex/Capsule/hkpCapsuleShape.h>
#include <Physics/Dynamics/Phantom/hkpSimpleShapePhantom.h>
#include <Physics/Collide/Query/Collector/PointCollector/hkpClosestCdPointCollector.h>
#include <Physics/Collide/Query/Collector/PointCollector/hkpAllCdPointCollector.h>
#include <Physics/ConstraintSolver/Simplex/hkpSimplexSolver.h>
#include <Physics/Collide/Query/CastUtil/hkpLinearCastInput.h>

#include <Demos/DemoCommon/Utilities/GameUtils/GameUtils.h>
#include <Demos/DemoCommon/Utilities/Character/CharacterUtils.h>
//#include <hkdemoframework/hkDemoFramework.h>

#include <Demos/DemoCommon/DemoFramework/hkTextDisplay.h>

#include <Graphics/Common/Input/Pad/hkgPad.h>
#include <Graphics/Common/Window/hkgWindow.h>

#include <Common/Visualize/hkDebugDisplay.h>

#define HK_OBJECT_IS_LADDER 1234

class MyCharacterListener : public hkReferencedObject, public hkpCharacterProxyListener
{
	public:
		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO);

		MyCharacterListener( ) 
		: m_atLadder(false)
		{}

		// Ladder handling code goes here
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


CharacterDemo::CharacterDemo(hkDemoEnvironment* env)
:	hkDefaultPhysicsDemo(env)
{

	//
	// Setup the camera
	//
	{
		hkVector4 from(  0.0f, 20.0f, -80.0f);
		hkVector4 to  (  0.0f,  0.0f,   0.0f);
		hkVector4 up  (  0.0f,  1.0f,   0.0f);
		setupDefaultCameras( env, from, to, up );

		// disable back face culling
		setGraphicsState(HKG_ENABLED_CULLFACE, false);

		// don't really want shadows as makes it too dark
		forceShadowState(false);

		setupLights(m_env); // so that the extra lights are added
		// float lightDir[] = { 0, -0.5f, -1 };
		// setSoleDirectionLight(m_env, lightDir, 0xffffffff );
	}

	//
	// Create the world
	//
	{
		hkpWorldCinfo info;
		info.setBroadPhaseWorldSize( 350.0f );  
		info.m_gravity.set(0,0,-9.8f);
		info.m_collisionTolerance = 0.1f;		
		m_world = new hkpWorld( info );
		m_world->lock();

		hkpAgentRegisterUtil::registerAllAgents(m_world->getCollisionDispatcher());

		setupGraphics();
	}

	// Load the level
	{
		hkVector4 tkScaleFactor(.32f,.32f,.32f);
		hkString fullname("Resources/Physics/Tk/CharacterController/");

		// We load our test case level.
		//fullname += "testcases.tk";
		fullname += "level.tk";

		hkpShape* moppShape = GameUtils::loadTK2MOPP(fullname.cString(),tkScaleFactor, -1.0f);
		HK_ASSERT2(0x64232cc0, moppShape,"TK file failed to load to MOPP in GameUtils::loadTK2MOPP.");

		hkpRigidBodyCinfo ci;
		ci.m_shape = moppShape;
		ci.m_motionType = hkpMotion::MOTION_FIXED;
		ci.m_collisionFilterInfo = hkpGroupFilter::calcFilterInfo( 0, 1 );

		hkpRigidBody* entity = new hkpRigidBody(ci);
		moppShape->removeReference();
		m_world->addEntity(entity);
		entity->removeReference();
	}
	
	// Add a ladder
	hkVector4 baseSize( 1.0f, 0.5f, 3.6f);
	{ 
		hkpRigidBodyCinfo rci;
		rci.m_shape = new hkpBoxShape( baseSize );
		rci.m_position.set(3.4f, 8.f, 2);
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
	//
	//	Create a character proxy object
	//
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


		// Construct a Shape Phantom
		m_phantom = new hkpSimpleShapePhantom( m_standShape, hkTransform::getIdentity(), hkpGroupFilter::calcFilterInfo(0,2) );
		
		// Add the phantom to the world
		m_world->addPhantom(m_phantom);
		m_phantom->removeReference();

		// Construct a character proxy
		hkpCharacterProxyCinfo cpci;
		cpci.m_position.set(-9.1f, 35, .4f);
		cpci.m_staticFriction = 0.0f;
		cpci.m_dynamicFriction = 1.0f;
		cpci.m_up.setNeg4( m_world->getGravity() );
		cpci.m_up.normalize3();	
		cpci.m_userPlanes = 4;
		cpci.m_maxSlope = HK_REAL_PI / 3.f;

		cpci.m_shapePhantom = m_phantom;
		m_characterProxy = new hkpCharacterProxy( cpci );
	}
	
	//
	// Add in a custom friction model
	//
	{
		hkVector4 up( 0.f, 0.f, 1.f );
		m_listener = new MyCharacterListener();
		m_characterProxy->addCharacterProxyListener(m_listener);
	}

	//
	// Create the Character state machine and context
	//
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
	}
	
	// Current camera angle about up
	m_currentAngle = HK_REAL_PI * 0.5f;
	
	m_world->unlock();
}

CharacterDemo::~CharacterDemo()
{
	m_listener->removeReference();

	m_world->lock();

	m_world->removePhantom(m_characterProxy->getShapePhantom());

	m_characterProxy->removeReference();

	m_standShape->removeReference();

	m_crouchShape->removeReference();

	m_world->unlock();

	delete m_characterContext;
}

/////////////////////////////////////////////////////////////////////

hkDemo::Result CharacterDemo::stepDemo()
{

	hkVector4 up;
	hkQuaternion orient;

	{
		m_world->lock();

		//	Get user input data
		int m_upAxisIndex = 2;
		up.setZero4();
		up(m_upAxisIndex) = 1;

		hkReal posX = 0.f;
		hkReal posY = 0.f;
		{
			float deltaAngle = 0.f;
			CharacterUtils::getUserInputForCharacter(m_env, deltaAngle, posX, posY);
			m_currentAngle += deltaAngle;
			orient.setAxisAngle(up, m_currentAngle);
		}

		hkpCharacterInput input;
		hkpCharacterOutput output;
		{
			input.m_inputLR = posX;
			input.m_inputUD = posY;

			input.m_wantJump =  m_env->m_window->getMouse().wasButtonPressed(HKG_MOUSE_LEFT_BUTTON)
				|| m_env->m_gamePad->wasButtonPressed(HKG_PAD_BUTTON_1);
			input.m_atLadder = m_listener->m_atLadder;

			input.m_up = up;
			input.m_forward.set(1,0,0);
			input.m_forward.setRotatedDir( orient, input.m_forward );

			input.m_stepInfo.m_deltaTime = m_timestep;
			input.m_stepInfo.m_invDeltaTime = 1.0f / m_timestep;
			input.m_characterGravity.set(0,0,-16);
			input.m_velocity = m_characterProxy->getLinearVelocity();
			input.m_position = m_characterProxy->getPosition();

			if (m_listener->m_atLadder)
			{
				hkVector4 right, ladderUp;
				right.setCross( up, m_listener->m_ladderNorm );
				ladderUp.setCross( m_listener->m_ladderNorm, right );
				// Calculate the up vector for the ladder
				if (ladderUp.lengthSquared3() > HK_REAL_EPSILON)
				{
					ladderUp.normalize3();
				}

				// Reorient the forward vector so it points up along the ladder
				input.m_forward.addMul4( -m_listener->m_ladderNorm.dot3(input.m_forward), m_listener->m_ladderNorm);
				input.m_forward.add4( ladderUp );
				input.m_forward.normalize3();

				input.m_surfaceNormal = m_listener->m_ladderNorm;
				input.m_surfaceVelocity = m_listener->m_ladderVelocity;
			}
			else 
			{
				hkVector4 down;	down.setNeg4(up);
				hkpSurfaceInfo ground;
				m_characterProxy->checkSupport(down, ground);
				input.m_isSupported = ground.m_supportedState == hkpSurfaceInfo::SUPPORTED;

				input.m_surfaceNormal = ground.m_surfaceNormal;
				input.m_surfaceVelocity = ground.m_surfaceVelocity;
			}
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

			// Feed output from state machine into character proxy
			m_characterProxy->setLinearVelocity(output.m_velocity);

			hkStepInfo si;
			si.m_deltaTime = m_timestep;
			si.m_invDeltaTime = 1.0f/m_timestep;
			m_characterProxy->integrate( si, m_world->getGravity() );

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

		//
		// Handle crouching
		//
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
		m_world->unlock();
	}

	// Step the world
	hkDefaultPhysicsDemo::stepDemo();

	// Camera Handling
	{
		m_world->lock();
		{
			const hkReal height = .7f;
			hkVector4 forward;
			forward.set(1,0,0);
			forward.setRotatedDir( orient, forward );

			hkVector4 from, to;
			to = m_characterProxy->getPosition();
			to.addMul4(height, up);

			hkVector4 dir;
			dir.setMul4( height, up );
			dir.addMul4( -4.f, forward);

			from.setAdd4(to, dir);
			setupDefaultCameras(m_env, from, to, up, 1.0f);
		}

		m_world->unlock();
	}

	return hkDemo::DEMO_OK;
}

void CharacterDemo::swapPhantomShape( hkpShape* newShape )
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
"This is our static test scene for the Havok character controller " \
"This demo shows all the feature of the character controller including " \
"Walking, Jumping, Crouching and Climbing \n" \
"Controls:" \
"  Mouse / Analog Stick to turn \n" \
"  \224 / \225 Forward/Back \n" \
"  LMB / \221 to Jump \n" \
"  RMB / \222 to Crouch \n";

HK_DECLARE_DEMO(CharacterDemo, HK_DEMO_TYPE_PRIME, "CharacterTest", helpString);

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
