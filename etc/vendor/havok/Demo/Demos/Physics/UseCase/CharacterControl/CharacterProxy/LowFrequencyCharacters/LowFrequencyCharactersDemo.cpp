/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Common/Base/Monitor/hkMonitorStream.h>

#include <Demos/Physics/UseCase/CharacterControl/CharacterProxy/LowFrequencyCharacters/LowFrequencyCharactersDemo.h>

#include <Physics/Utilities/CharacterControl/CharacterProxy/hkpCharacterProxy.h>
#include <Physics/Utilities/CharacterControl/StateMachine/hkpDefaultCharacterStates.h>

#include <Physics/Collide/Shape/Convex/Capsule/hkpCapsuleShape.h>
#include <Physics/Dynamics/Phantom/hkpSimpleShapePhantom.h>
#include <Physics/Collide/Query/Collector/PointCollector/hkpClosestCdPointCollector.h>
#include <Physics/Collide/Query/Collector/PointCollector/hkpAllCdPointCollector.h>

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

class TerrainHeightFieldShape: public hkpSampledHeightFieldShape
{
public:

	TerrainHeightFieldShape( const hkpSampledHeightFieldBaseCinfo& ci, hkReal* data ): hkpSampledHeightFieldShape(ci) , m_data(data)
	{
	}

	/// Generate a rough terrain
	HK_FORCE_INLINE hkReal getHeightAtImpl( int x, int z ) const
	{
		return m_data[x * m_zRes + z];
	}

	///		this should return true if the two triangles share the edge p00-p11
	///     otherwise it should return false if the triangles sphere the edge p01-p10
	HK_FORCE_INLINE hkBool getTriangleFlipImpl() const
	{	
		return false;
	}

	virtual void collideSpheres( const CollideSpheresInput& input, SphereCollisionOutput* outputArray) const
	{
		hkSampledHeightFieldShape_collideSpheres(*this, input, outputArray);
	}

	hkReal* m_data;
};


LowFrequencyCharactersDemo::LowFrequencyCharactersDemo(hkDemoEnvironment* env)
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
	//	Create a terrain 
	//
	TerrainHeightFieldShape* heightFieldShape;
	{
		hkpSampledHeightFieldBaseCinfo ci;
		ci.m_xRes = 64;
		ci.m_zRes = 64;
		ci.m_scale.set(4,1,4);

		//
		// Fill in a data array
		//
		m_data = hkAllocate<hkReal>(ci.m_xRes * ci.m_zRes, HK_MEMORY_CLASS_DEMO);
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
	//	Create a character proxy object
	//
	{
		m_numBoxes = 0;
		m_numCapsules = 0;
		m_numSpheres = 0;
		
		// We'll store the simulation frequency in this 
		hkpPropertyValue val;

		// Construct shape phantoms for the characters
		hkPseudoRandomGenerator random(123);
		for (int i=0; i < NUM_CHARACTERS; i++)
		{

			// Create a random shape to represent the character
			hkpConvexShape* shape = HK_NULL;
			{
				hkReal scale = random.getRandReal01() * 0.25f + 0.75f;

				//Set the simulation frequency
				val.setInt( random.getRand32() % 3 );

				switch (val.getInt())
				{
					case 0:
						{
							hkVector4 top(0, scale * 0.4f, 0);
							hkVector4 bottom(0, -scale * 0.4f, 0);					
							shape = new hkpCapsuleShape(top, bottom, scale * 0.6f);
							m_numCapsules++;
						}
						break;
					case 1:
						{
							hkVector4 size(scale * 0.5f, scale , scale * 0.5f);
							shape = new hkpBoxShape(size);
							m_numBoxes++;
						}
						break;
					default:
						{
							shape = new hkpSphereShape(scale);
							m_numSpheres++;
						}
						break;
				}
			}

			hkpShapePhantom* phantom = new hkpSimpleShapePhantom( shape, hkTransform::getIdentity() );
			shape->removeReference();

			// Add the phantom to the world
			m_world->addPhantom(phantom);
			phantom->removeReference();

			HK_SET_OBJECT_COLOR( (hkUlong)phantom->getCollidable(), 0x7fffffff & hkColor::getRandomColor() );

			// Construct a character proxy
			hkpCharacterProxyCinfo cpci;
			random.getRandomVector11( cpci.m_position );
			cpci.m_position.mul4(32);
			cpci.m_position(1) = 10;
			cpci.m_up.setNeg4( m_world->getGravity() );
			cpci.m_up.normalize3();

			cpci.m_shapePhantom = phantom;
			m_characterProxy[i] = new hkpCharacterProxy( cpci );

			// Player is simulated at full frequency
			if (i==0)
			{
				val.setInt(0);
			}

			// Add the schedule property
			phantom->addProperty(HK_SCHEDULE_FREQUENCY, val);
		}
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
	{
		for (int i=0; i < NUM_CHARACTERS; i++)
		{
			m_characterContext[i] = new hkpCharacterContext(manager, HK_CHARACTER_ON_GROUND);
		}
		manager->removeReference();
	}
	
	// Current camera angle about up
	m_currentAngle = 0.0f;

	//Initialised the round robin counter
	m_tick = 0;

	m_world->unlock();
}

LowFrequencyCharactersDemo::~LowFrequencyCharactersDemo()
{
	m_world->lock();

	hkDeallocate<hkReal>( m_data );

	for (int i=0; i < NUM_CHARACTERS; i++)
	{
		m_characterProxy[i]->removeReference();

		delete m_characterContext[i];
	}

	m_world->unlock();
}

hkBool LowFrequencyCharactersDemo::shouldSimulate(int characterNumber, int currentTick)
{

	HK_ASSERT(0x614cce11, characterNumber < NUM_CHARACTERS);
	HK_ASSERT(0x3ca8a029, m_characterProxy[characterNumber]->getShapePhantom()->hasProperty(HK_SCHEDULE_FREQUENCY));
	hkInt32 slot = m_characterProxy[characterNumber]->getShapePhantom()->getProperty(HK_SCHEDULE_FREQUENCY).getInt();

	switch (slot)
	{
	case 0:	return true;
	case 1: return (currentTick & 1)!=0; // Every second tick (offset by 1)
	case 2: return (currentTick & 3)==0; // Every fourth tick
	default : return false;
	}
}
/////////////////////////////////////////////////////////////////////

hkDemo::Result LowFrequencyCharactersDemo::stepDemo()
{
	hkVector4 up;
	hkpCharacterInput input[NUM_CHARACTERS];

	{
		m_world->lock();

		//	Get user input data
		int m_upAxisIndex = 1;
		up.setZero4();
		up(m_upAxisIndex) = 1;

		m_tick++;

		hkQuaternion orient;
		hkReal posX = 0.f;
		hkReal posY = 0.f;
		{
			float deltaAngle = 0.f;
			CharacterUtils::getUserInputForCharacter(m_env, deltaAngle, posX, posY);
			m_currentAngle += deltaAngle;
			orient.setAxisAngle(up, m_currentAngle);
		}

		// Fill in the state machine input structure
		hkpCharacterOutput output[NUM_CHARACTERS];
		{
			for (int i=0; i < NUM_CHARACTERS; i++)
			{
				hkInt32 slot = m_characterProxy[i]->getShapePhantom()->getProperty(HK_SCHEDULE_FREQUENCY).getInt();
				if (shouldSimulate(i, m_tick))
				{
					if (i==0)
					{
						input[i].m_inputLR = posX;
						input[i].m_inputUD = posY;
						input[i].m_wantJump = m_env->m_window->getMouse().wasButtonPressed(HKG_MOUSE_LEFT_BUTTON)
							|| m_env->m_gamePad->wasButtonPressed(HKG_PAD_BUTTON_1);
						input[i].m_forward.set(1,0,0);
						input[i].m_forward.setRotatedDir( orient, input[i].m_forward );
					}
					else
					{
						hkReal time = hkReal(m_tick) / 60.0f;
						input[i].m_inputLR = hkMath::sin(time + i);
						input[i].m_inputUD = hkMath::cos(time + i);
						input[i].m_wantJump = false;
						input[i].m_forward.set(1,0,0);
					}

					input[i].m_atLadder = false;
					input[i].m_up = up;


					input[i].m_stepInfo.m_deltaTime = m_timestep;
					input[i].m_stepInfo.m_invDeltaTime = 1.0f / (m_timestep * (1 << slot));
					input[i].m_characterGravity.set(0,-40,0);
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
		}


		// Apply the character state machine
		{
			HK_TIMER_BEGIN( "update character state", HK_NULL );

			for (int i=0; i < NUM_CHARACTERS; i++)
			{
				if (shouldSimulate(i, m_tick))
				{
					m_characterContext[i]->update(input[i], output[i]);
				}
			}

			HK_TIMER_END();

		}

		//Apply the player character controller
		{
			HK_TIMER_BEGIN( "simulate character", HK_NULL );

			for (int i=0; i < NUM_CHARACTERS; i++)
			{
				if (shouldSimulate(i, m_tick))
				{
					// Since we're simulatating some characters at less than full frequency we need to adjust the
					// timestep we pass to the proxy
					hkInt32 slot = m_characterProxy[i]->getShapePhantom()->getProperty(HK_SCHEDULE_FREQUENCY).getInt();

					hkStepInfo csi;
					csi.m_deltaTime = m_timestep * (1 << slot);
					csi.m_invDeltaTime = 1.0f / csi.m_deltaTime;

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
			}

			HK_TIMER_END();
		}

		m_world->unlock();
	}

	// Step the world
	hkDefaultPhysicsDemo::stepDemo();

	{
		m_world->lock();

		// Camera Handling
		{
			const hkReal height = 1.7f;

			hkVector4 from, to;
			to = m_characterProxy[0]->getPosition();
			to.addMul4(height, up);

			hkVector4 dir;
			dir.setMul4( height, up );
			dir.addMul4( -8.f, input[0].m_forward);

			from.setAdd4(to, dir);
			setupDefaultCameras(m_env, from, to, up, 1.0f);
		}

		{
			char buffer[255];
			hkString::snprintf(buffer, 255, "%d capsules at 60Hz, %d boxes at 30Hz, %d spheres at 15Hz", m_numCapsules, m_numBoxes, m_numSpheres);
			m_env->m_textDisplay->outputText(buffer, 20, 410, 0xffffffff);
		}

		m_world->unlock();
	}

	return hkDemo::DEMO_OK;
}

#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
"This game shows how the character controller may be used efficiently for NPCs " \
"The characters are simulated at different frequencies. Capsules are simulated at 60Hz (every frame) " \
"Boxes are simulated at 30Hz (every 2nd frame) and spheres are simulated at 15Hz (every 4th frame) " \
"\nAll characters are running on a heightfield. \n" \
"NOTE: We explicitly don't interpolate the positions of the lower frequency characters between frames. " \
"This is done to highlight the fact that they are run at different frequencies";

HK_DECLARE_DEMO(LowFrequencyCharactersDemo, HK_DEMO_TYPE_PRIME | HK_DEMO_TYPE_CRITICAL, "CharacterTest", helpString);

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
