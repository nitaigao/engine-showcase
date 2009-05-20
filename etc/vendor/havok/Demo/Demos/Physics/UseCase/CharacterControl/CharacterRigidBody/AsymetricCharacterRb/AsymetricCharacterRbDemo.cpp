/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


#include <Demos/demos.h>
#include <Common/Base/Monitor/hkMonitorStream.h>

#include <Demos/Physics/UseCase/CharacterControl/CharacterRigidBody/AsymetricCharacterRb/AsymetricCharacterRbDemo.h>
#include <Physics/Utilities/CharacterControl/CharacterRigidBody/hkpCharacterRigidBody.h>
#include <Physics/Utilities/CharacterControl/StateMachine/hkpDefaultCharacterStates.h>

// Used for a terrain heigtfield (taken from character proxy demo)
#include <Demos/Physics/UseCase/CharacterControl/CharacterProxy/MultipleCharacters/TerrainHeightFieldShape.h>

#include <Physics/Collide/Shape/Convex/Capsule/hkpCapsuleShape.h>

// Used for graphics and I/O
#include <Demos/DemoCommon/Utilities/GameUtils/GameUtils.h>
#include <Demos/DemoCommon/Utilities/Character/CharacterUtils.h>
#include <Demos/DemoCommon/DemoFramework/hkTextDisplay.h>
#include <Common/Base/Algorithm/PseudoRandom/hkPseudoRandomGenerator.h>
#include <Common/Visualize/hkDebugDisplay.h>

// Attention: Y is UP now!
static const hkVector4 UP(0,1,0);

AsymetricCharacterRbDemo::AsymetricCharacterRbDemo(hkDemoEnvironment* env)
:	hkDefaultPhysicsDemo(env)
{
		
	// Create the world
	{
		hkpWorldCinfo info;
		info.setBroadPhaseWorldSize( 350.0f );  
		info.m_gravity.set(0, -9.8f, 0);
		info.m_collisionTolerance = 0.01f;		
		info.m_contactPointGeneration = hkpWorldCinfo::CONTACT_POINT_ACCEPT_ALWAYS;
		m_world = new hkpWorld( info );
		m_world->lock();

		hkpAgentRegisterUtil::registerAllAgents(m_world->getCollisionDispatcher());

		setupGraphics();
	}
	
	//	Create a terrain (more bumpy as in the classical character proxy demo)
	TerrainHeightFieldShape* heightFieldShape;
	{
		hkpSampledHeightFieldBaseCinfo ci;
		ci.m_xRes = 64;
		ci.m_zRes = 64;
		ci.m_scale.set(1.6f, 0.2f, 1.6f);

		// Fill in a data array
		m_data = hkAllocate<hkReal>((ci.m_xRes * ci.m_zRes), HK_MEMORY_CLASS_DEMO);
		
		for (int x = 0; x < ci.m_xRes; x++)
		{
			for (int z = 0; z < ci.m_zRes; z++)
			{
				hkReal dx,dz,height = 0;
				int octave = 1;
				
				// Add together a few sine and cose waves
				for (int i=0; i< 3; i++)
				{
					dx = hkReal(x * octave) / ci.m_xRes;
					dz = hkReal(z * octave) / ci.m_zRes;
					height +=  4 * i * hkMath::cos(dx * HK_REAL_PI) * hkMath::sin(dz * HK_REAL_PI);
					height -= 2.5f;
					octave *= 2;				
				}

				m_data[x*ci.m_zRes + z] = height;
			}
		}

		heightFieldShape = new TerrainHeightFieldShape( ci , m_data );
		
		//	Create terrain as a fixed rigid body
		{
			hkpRigidBodyCinfo rci;
			rci.m_motionType = hkpMotion::MOTION_FIXED;
			rci.m_position.setMul4( -0.5f, heightFieldShape->m_extents ); // center the heighfield
			rci.m_shape = heightFieldShape;
			rci.m_friction = 0.5f;
			
			hkpRigidBody* terrain = new hkpRigidBody( rci );

			m_world->addEntity(terrain);

			terrain->removeReference();
		}

		heightFieldShape->removeReference();
	}
	
	// Create some random static pilars (green) and smaller dynamic boxes (blue)
	{
		hkPseudoRandomGenerator randgen(12345);

		for (int i=0; i < 80; i++)
		{
			
			if (i%2)
			{
				// Dynamic boxes of random size
				hkVector4 size;
				randgen.getRandomVector11(size);
				size.setAbs4( size );
				size.mul4(0.5f);
				hkVector4 minSize; minSize.setAll3(0.25f);
				size.add4(minSize);
				
				// Random position
				hkVector4 position;
				randgen.getRandomVector11( position );
				position(0) *= 25; position(2) *= 25; position(1) = 4;

				{ 
					// To illustrate using the shape, create a rigid body by first defining a template.
					hkpRigidBodyCinfo rci;
					rci.m_shape = new hkpBoxShape( size );
					rci.m_position = position;
					rci.m_friction = 0.5f;
					rci.m_restitution = 0.0f;
					// Density of concrete
					const hkReal density = 2000.0f;
					rci.m_mass = size(0)*size(1)*size(2)*density;
					
					hkVector4 halfExtents(size(0) * 0.5f, size(1) * 0.5f, size(2) * 0.5f);
					hkpMassProperties massProperties;
					hkpInertiaTensorComputer::computeBoxVolumeMassProperties(halfExtents, rci.m_mass, massProperties);
					rci.m_inertiaTensor = massProperties.m_inertiaTensor;
					rci.m_motionType = hkpMotion::MOTION_BOX_INERTIA;
										
					// Create a rigid body (using the template above).
					hkpRigidBody* box = new hkpRigidBody(rci);

					// Remove reference since the body now "owns" the Shape.
					rci.m_shape->removeReference();

					box->addProperty(HK_PROPERTY_DEBUG_DISPLAY_COLOR, int(hkColor::DARKBLUE)); 

					// Finally add body so we can see it, and remove reference since the world now "owns" it.
					m_world->addEntity(box)->removeReference();
				} 

			}
			else
			{
				// Fixed pilars of random size
				hkVector4 size;
				randgen.getRandomVector11(size);
				size.setAbs4( size );
				hkVector4 minSize; minSize.setAll3(0.5f);
				size.add4(minSize);
				size(1) = 2.5f;

				// Random position
				hkVector4 position;
				randgen.getRandomVector11( position );
				position(0) *= 25; position(2) *= 25; position(1) = 0;

				{ 
					// To illustrate using the shape, create a rigid body by first defining a template.
					hkpRigidBodyCinfo rci;
					
					rci.m_shape = new hkpBoxShape( size );
					rci.m_position = position;
					rci.m_friction = 0.1f;
					rci.m_motionType = hkpMotion::MOTION_FIXED;
					

					// Create a rigid body (using the template above).
					hkpRigidBody* pilar = new hkpRigidBody(rci);

					// Remove reference since the body now "owns" the shape.
					rci.m_shape->removeReference();

					pilar->addProperty(HK_PROPERTY_DEBUG_DISPLAY_COLOR, int(hkColor::DARKGREEN)); 

					// Finally add body so we can see it, and remove reference since the world now "owns" it.
					m_world->addEntity(pilar);

					pilar->removeReference();
				} 
			}
		}
	}

	//	Create a character rigid body
	{
	
		// Construct a shape
		hkVector4 vertexA(0.4f,0,0);
		hkVector4 vertexB(-0.4f,0,0);		

		// Create a capsule to represent the character standing
		hkpShape* capsule = new hkpCapsuleShape(vertexA, vertexB, 0.6f);
		

		// Construct a character rigid body
		hkpCharacterRigidBodyCinfo info;
		info.m_mass = 100.0f;
		info.m_shape = capsule;
		info.m_maxForce = 1000.0f;
		info.m_up = UP;
		info.m_position.set(32.0f, 3.0f, 10.0f);
		info.m_maxSlope = HK_REAL_PI/2.0f;	// Only vertical plane is too steep


		m_characterRigidBody = new hkpCharacterRigidBody( info );
		m_world->addEntity( m_characterRigidBody->getRigidBody() );

		capsule->removeReference();

	}
	
	// Create the character state machine and context
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

		m_characterContext = new hkpCharacterContext(manager, HK_CHARACTER_IN_AIR);	
		manager->removeReference();			

		// Set new filter parameters for final output velocity filtering
		// Smoother interactions with small dynamic boxes
		m_characterContext->setCharacterType(hkpCharacterContext::HK_CHARACTER_RIGIDBODY);
		m_characterContext->setFilterParameters(0.9f,12.0f,200.0f);
	}

	// Initialize hkpSurfaceInfo of ground from previous frame
	// Specific case (character is in the air, UP is Y)
	m_previousGround = new hkpSurfaceInfo(UP,hkVector4::getZero(),hkpSurfaceInfo::UNSUPPORTED);
	m_framesInAir = 0;
	
	// Current camera angle about up
	m_currentAngle = 0.0f;

	// Init actual time
	m_time = 0.0f;

	// Init rigid body normal
	m_rigidBodyNormal = UP;

	m_world->unlock();
}

AsymetricCharacterRbDemo::~AsymetricCharacterRbDemo()
{
	m_world->lock();

	m_characterRigidBody->removeReference();
	m_characterContext->removeReference();

	delete m_previousGround;

	m_world->unlock();

	hkDeallocate<hkReal>(m_data); 
	
}

hkDemo::Result AsymetricCharacterRbDemo::stepDemo()
{

	// Update actual time
	m_time += m_timestep;	


	hkQuaternion orient;
	{
		m_world->lock();

		hkReal posX = 0.f;
		hkReal posY = 0.f;
		{
			float deltaAngle = 0.f;
			CharacterUtils::getUserInputForCharacter(m_env, deltaAngle, posX, posY);
			m_currentAngle += deltaAngle;
			orient.setAxisAngle(UP, m_currentAngle);
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
			input.m_forward.setRotatedDir( orient, input.m_forward );

			hkStepInfo stepInfo;
			stepInfo.m_deltaTime = m_timestep;
			stepInfo.m_invDeltaTime = 1.0f/m_timestep;
			stepInfo.m_endTime = m_time;
			
			input.m_stepInfo = stepInfo;
			
			input.m_characterGravity.set(0,-16,0);
			input.m_velocity = m_characterRigidBody->getLinearVelocity();
			input.m_position = m_characterRigidBody->getPosition();

			hkpSurfaceInfo ground;
			m_characterRigidBody->checkSupport( stepInfo, ground);
		
			// Avoid accidental state changes (Smooth movement on stairs)
			// During transition supported->unsupported continue to return N-frames hkpSurfaceInfo data from previous supported state
			{

				// Number of frames to skip (continue with previous hkpSurfaceInfo data)
				const int skipFramesInAir = 5;

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

					// Reset old number of frames
					if (m_framesInAir > skipFramesInAir)
					{
						m_framesInAir = 0;
					}			

				}
			}			
	
		}

		// Apply the character state machine
		{
			HK_TIMER_BEGIN( "update character state", HK_NULL );

			m_characterContext->update(input, output);

			HK_TIMER_END();
		}

		// Apply the player character controller
		{
			HK_TIMER_BEGIN( "simulate character", HK_NULL );

			m_characterRigidBody->setLinearVelocity(output.m_velocity, m_timestep);	

			HK_TIMER_END();
		}

		// Rotate the character
		{
			hkVector4 offset; offset.set(1,0,0);
			offset.setRotatedDir( orient , offset);

			hkRotation rotation;
			hkVector4& col0 = rotation.getColumn(0);
			hkVector4& col1 = rotation.getColumn(1);
			hkVector4& col2 = rotation.getColumn(2);

			// Smoothed surface normal
			hkVector4 surfaceNorm;
			surfaceNorm = input.m_isSupported ? input.m_surfaceNormal : (hkVector4)UP;
			m_rigidBodyNormal.addMul4( 0.05f, surfaceNorm );
			m_rigidBodyNormal.normalize3();

			col1 = m_rigidBodyNormal;
			col2.setCross( col1, offset); 
			col2.normalize3();
			col0.setCross( col1, col2 );

#ifdef HK_DEBUG

			HK_DISPLAY_ARROW(m_characterRigidBody->getPosition(), col0, 0xffff00ff);
			HK_DISPLAY_ARROW(m_characterRigidBody->getPosition(), col1, 0xff00ffff);
			HK_DISPLAY_ARROW(m_characterRigidBody->getPosition(), col2, 0xff0000ff);

#endif
			// Forward orientation controller
			reorientCharacter( rotation );

		}

		// Display states infos
		{
			// classical controller state
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
			hkString::snprintf(buffer, 255, "State: %s ", stateStr);
			m_env->m_textDisplay->outputText(buffer, 20, 450, 0xffffffff);			
		
		m_world->unlock();

		}

		// Step the world
		{
			hkDefaultPhysicsDemo::stepDemo();
		}
		
		m_world->lock();

		// Camera Handling
		{
			const hkReal height = 1.25f;
			hkVector4 forward;
			forward.set(1,0,0);
			forward.setRotatedDir( orient, forward );

			hkVector4 from, to;
			to = m_characterRigidBody->getRigidBody()->getPosition();

			hkVector4 dir;
			dir.setMul4( height, UP );
			dir.addMul4( -4.0f, forward);

			from.setAdd4(to, dir);
			setupDefaultCameras(m_env, from, to, UP, 1.0f);
		}

		m_world->unlock();
	
	}

	return hkDemo::DEMO_OK;
}

hkBool AsymetricCharacterRbDemo::reorientCharacter(const hkRotation& rotation)
{
	// Calculate desired rotation velocity to follow expected orientation 
	//
	// angularVel = gain*(desiredOrient - currentOrient)/timestep
	//

	const hkReal gain = 0.5f;

	const hkQuaternion& currentOrient = m_characterRigidBody->getRigidBody()->getRotation();

	hkQuaternion desiredOrient; desiredOrient.set(rotation);

	hkVector4 angle; currentOrient.estimateAngleTo(desiredOrient,angle);
	
	hkVector4 angularVelocity; angularVelocity.setMul4(gain/m_timestep,angle);

	m_characterRigidBody->setAngularVelocity(angularVelocity);

	return true;
}

#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
" This demo shows how to create a character represented by rotationally asymmetric rigid body" \
" and how this character interacts with statics and dynamics boxes on a bumpy terrain."
" Orientation of the character is controlled by setting its angular velocity. The character rotation" \
" is safe with this approach. See code for more details about forward orientation controller\n" \
" Use cases: Swimming characters, animals e.g. horses / dogs.\n"\
" Controls:" \
"  Mouse / Analog Stick to turn \n" \
"  \224 / \225 Forward/Back \n" \
"  LMB / \221 to Jump \n";

HK_DECLARE_DEMO(AsymetricCharacterRbDemo, HK_DEMO_TYPE_PRIME, "Asymetric Character Rigid Body", helpString);

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
