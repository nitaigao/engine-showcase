/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


#include <Demos/demos.h>
#include <Common/Base/Monitor/hkMonitorStream.h>

#include <Demos/Physics/UseCase/CharacterControl/CharacterProxy/AsymetricCharacter/AsymetricCharacterDemo.h>

#include <Physics/Utilities/CharacterControl/CharacterProxy/hkpCharacterProxy.h>
#include <Physics/Utilities/CharacterControl/StateMachine/hkpDefaultCharacterStates.h>

#include <Physics/Collide/Shape/Convex/Capsule/hkpCapsuleShape.h>
#include <Physics/Collide/Shape/Misc/Transform/hkpTransformShape.h>

#include <Physics/Dynamics/Phantom/hkpSimpleShapePhantom.h>
#include <Physics/Collide/Query/Collector/PointCollector/hkpClosestCdPointCollector.h>

#include <Demos/DemoCommon/Utilities/GameUtils/GameUtils.h>
#include <Demos/DemoCommon/Utilities/Character/CharacterUtils.h>

#include <Demos/DemoCommon/DemoFramework/hkTextDisplay.h>
#include <Common/Base/Algorithm/PseudoRandom/hkPseudoRandomGenerator.h>

#include <Graphics/Common/Input/Pad/hkgPad.h>
#include <Graphics/Common/Window/hkgWindow.h>

#include <Physics/Utilities/Constraint/Keyframe/hkpKeyFrameUtility.h>

#include <Common/Visualize/hkDebugDisplay.h>

#include <Physics/Collide/Shape/HeightField/SampledHeightField/hkpSampledHeightFieldShape.h>
#include <Physics/Collide/Shape/HeightField/SampledHeightField/hkpSampledHeightFieldBaseCinfo.h>
#include <Demos/Physics/UseCase/CharacterControl/CharacterProxy/MultipleCharacters/TerrainHeightFieldShape.h>

#include <Physics/Collide/Agent/hkpProcessCollisionInput.h>

AsymetricCharacterDemo::AsymetricCharacterDemo(hkDemoEnvironment* env)
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
	//	Create a terrain 
	//
	TerrainHeightFieldShape* heightFieldShape;
	{
		hkpSampledHeightFieldBaseCinfo ci;
		ci.m_xRes = 64;
		ci.m_zRes = 64;
		ci.m_scale.set(1.6f, 0.2f, 1.6f);

		//
		// Fill in a data array
		//
		m_data = hkAllocate<hkReal>( ci.m_xRes * ci.m_zRes, HK_MEMORY_CLASS_DEMO );
		for (int x = 0; x < ci.m_xRes; x++)
		{
			for (int z = 0; z < ci.m_xRes; z++)
			{
				hkReal dx,dz,height = 0;
				int octave = 1;
				// Add togther a few sine and cose waves
				for (int i=0; i< 3; i++)
				{
					dx = hkReal(x * octave) / ci.m_xRes;
					dz = hkReal(z * octave) / ci.m_zRes;
					height +=  (5 - (i * 2)) * hkMath::cos(dx * HK_REAL_PI) * hkMath::sin(dz * HK_REAL_PI);
					octave *= 4;
				}

				m_data[x*ci.m_zRes + z] = height;
			}
		}

		heightFieldShape = new TerrainHeightFieldShape( ci , m_data );
		//
		//	Create a fixed rigid body
		//
		{
			hkpRigidBodyCinfo rci;
			rci.m_motionType = hkpMotion::MOTION_FIXED;
			rci.m_position.setMul4( -0.5f, heightFieldShape->m_extents ); // center the heightfield
			rci.m_shape = heightFieldShape;
			rci.m_friction = 0.05f;

			hkpRigidBody* body = new hkpRigidBody( rci );

			m_world->addEntity(body)->removeReference();
		}

		heightFieldShape->removeReference();
	}

	// 
	// Create some random pilars
	//
	{
		hkPseudoRandomGenerator randgen(12345);

		for (int i=0; i< 40; i++)
		{
			// Random size
			hkVector4 size;
			randgen.getRandomVector11(size);
			size.setAbs4( size );
			hkVector4 minSize; minSize.setAll3(0.2f);
			size.add4(minSize);
			size(1) = 2.0f;

			// Random position
			hkVector4 position;
			randgen.getRandomVector11( position );
			position(0) *= 25; position(2) *= 25; position(1) = 1;

			{ 
				hkpRigidBodyCinfo rci;
				// To illustrate using the shape, create a rigid body by first defining a template.

				rci.m_shape = new hkpBoxShape( size );
				rci.m_position = position;
				rci.m_motionType = hkpMotion::MOTION_FIXED;

				// Create a rigid body (using the template above).
				hkpRigidBody* pilar = new hkpRigidBody(rci);

				// Remove reference since the body now "owns" the Shape.
				rci.m_shape->removeReference();

				// Finally add body so we can see it, and remove reference since the world now "owns" it.
				m_world->addEntity(pilar)->removeReference();
			} 
		}
	}


	//
	//	Create a character proxy object
	//
	{
		// Construct a shape

		hkVector4 vertexA(0.4f,0,0);
		hkVector4 vertexB(-0.4f,0,0);		

		// Create a capsule to represent the character standing
		hkpShape* capsule = new hkpCapsuleShape(vertexA, vertexB, 0.6f);
		
		// Construct a Shape Phantom
		m_phantom = new hkpSimpleShapePhantom( capsule, hkTransform::getIdentity() );
		capsule->removeReference();

		
		// Add the phantom to the world
		m_world->addPhantom(m_phantom);
		m_phantom->removeReference();

		// Construct a character proxy
		hkpCharacterProxyCinfo cpci;
		cpci.m_position.set(24,6,6);
		cpci.m_staticFriction = 0.0f;
		cpci.m_dynamicFriction = 1.0f;
		cpci.m_up.setNeg4( m_world->getGravity() );
		cpci.m_up.normalize3();	

		cpci.m_shapePhantom = m_phantom;
		m_characterProxy = new hkpCharacterProxy( cpci );
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
	m_currentAngle = 0.0f;

	m_normal.setZero4();

	m_world->unlock();
}

AsymetricCharacterDemo::~AsymetricCharacterDemo()
{
	hkDeallocate<hkReal>(m_data);

	m_world->lock();

	m_world->removePhantom(m_characterProxy->getShapePhantom());

	m_characterProxy->removeReference();

	m_world->unlock();

	delete m_characterContext;
}


/////////////////////////////////////////////////////////////////////

hkDemo::Result AsymetricCharacterDemo::stepDemo()
{
	hkVector4 up;
	hkQuaternion orient;

	{
		m_world->lock();

		//	Get user input data
		int m_upAxisIndex = 1;
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

			input.m_wantJump = m_env->m_window->getMouse().wasButtonPressed(HKG_MOUSE_LEFT_BUTTON)
				|| m_env->m_gamePad->wasButtonPressed(HKG_PAD_BUTTON_1);
			input.m_atLadder = false;

			input.m_up = up;
			input.m_forward.set(1,0,0);
			input.m_forward.setRotatedDir( orient, input.m_forward );

			input.m_stepInfo.m_deltaTime = m_timestep;
			input.m_stepInfo.m_invDeltaTime = 1.0f / m_timestep;
			input.m_characterGravity.set(0,-16,0);
			input.m_velocity = m_characterProxy->getLinearVelocity();
			input.m_position = m_characterProxy->getPosition();

			hkVector4 down;	down.setNeg4(up);
			hkpSurfaceInfo ground;
			m_characterProxy->checkSupport(down, ground);

			input.m_isSupported = ground.m_supportedState == hkpSurfaceInfo::SUPPORTED;
			input.m_surfaceNormal = ground.m_surfaceNormal;
			input.m_surfaceVelocity = ground.m_surfaceVelocity;
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

			hkStepInfo si( hkTime(0.0f), hkTime(m_timestep) );
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
			const int h = m_env->m_window->getHeight();
			m_env->m_textDisplay->outputText(buffer, 20, h-40, 0xffffffff);
		}

		// Rotate the character
		{
			hkVector4 offset; offset.set(1,0,0);
			offset.setRotatedDir( orient , offset);

			hkRotation rotation;
			hkVector4& col0 = rotation.getColumn(0);
			hkVector4& col1 = rotation.getColumn(1);
			hkVector4& col2 = rotation.getColumn(2);

			hkVector4 surfaceNorm;
			surfaceNorm = input.m_isSupported ? input.m_surfaceNormal : up;
			m_normal.addMul4( 0.1f, surfaceNorm );
			m_normal.normalize3();

			col1 = m_normal;
			col2.setCross( col1, offset); 
			col2.normalize3();
			col0.setCross( col1, col2 );

			HK_DISPLAY_ARROW(m_characterProxy->getPosition(), col0, 0xffff00ff);
			HK_DISPLAY_ARROW(m_characterProxy->getPosition(), col1, 0xff00ffff);
			HK_DISPLAY_ARROW(m_characterProxy->getPosition(), col2, 0xff0000ff);

			reorientPhantom( rotation );
		}

		m_world->unlock();
	}

	// Step the world
	hkDefaultPhysicsDemo::stepDemo();

	{
		m_world->lock();

		// Camera Handling
		{
			const hkReal height = 0.7f;
			hkVector4 forward;
			forward.set(1,0,0);
			forward.setRotatedDir( orient, forward );

			hkVector4 from, to;
			to = m_characterProxy->getPosition();
			to.addMul4(height, up);

			hkVector4 dir;
			dir.setMul4( height, up );
			dir.addMul4( -3.2f, forward);

			from.setAdd4(to, dir);
			setupDefaultCameras(m_env, from, to, up, 1.0f);
		}

		m_world->unlock();
	}

	return hkDemo::DEMO_OK;
}

hkBool AsymetricCharacterDemo::reorientPhantom(const hkRotation& rotation)
{
	// Set the new orientation for the phantom	
	hkTransform oldTransform = m_phantom->getTransform();
	hkTransform newTransform = oldTransform;
	newTransform.setRotation(rotation);
	m_phantom->setTransform( newTransform );

	// We use getClosestPoints to query our penetration
	hkpClosestCdPointCollector closestCollector;
	m_world->getClosestPoints( m_phantom->getCollidable(), *m_world->getCollisionInput(), closestCollector );

	// We allow penetration up to 10 cms.
	// You can tweak this tolerance arbitrarily
	const hkReal tolerance = 0.1f;
	if (closestCollector.hasHit() && (closestCollector.getHit().m_contact.getDistance() < -tolerance) )
	{
		m_phantom->setTransform( oldTransform );
		return false;
	}
	else
	{
		return true;		
	}

}


#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
"This demo shows how to create a character with is rotationally asymmetric. " \
"We interactively alter the transform for the character and query the world to " \
"cross check that the new transformation is safe. Note that we continue to accumulate the " \
"rotation from the input even if our cross check disallows the rotation. This causes the character " \
"to snap into position once the rotation is safe\n" \
"Use cases: Swimming characters, animals e.g. horses / dogs.";

HK_DECLARE_DEMO(AsymetricCharacterDemo, HK_DEMO_TYPE_PRIME, "AsymetricCharacter", helpString);

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
