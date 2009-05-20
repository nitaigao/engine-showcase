/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Common/Base/Monitor/hkMonitorStream.h>

#include <Demos/Physics/UseCase/CharacterControl/CharacterProxy/CharacterPriority/CharacterPriorityDemo.h>
#include <Physics/Utilities/CharacterControl/CharacterProxy/hkpCharacterProxy.h>
#include <Physics/Utilities/CharacterControl/StateMachine/hkpDefaultCharacterStates.h>

#include <Demos/DemoCommon/Utilities/GameUtils/GameUtils.h>

#include <Demos/DemoCommon/DemoFramework/hkTextDisplay.h>
#include <Common/Base/Algorithm/PseudoRandom/hkPseudoRandomGenerator.h>

#include <Graphics/Common/Input/Pad/hkgPad.h>
#include <Graphics/Common/Window/hkgWindow.h>
#include <Physics/Collide/Query/Collector/PointCollector/hkpClosestCdPointCollector.h>
#include <Physics/Collide/Shape/Convex/Capsule/hkpCapsuleShape.h>
#include <Physics/Dynamics/Phantom/hkpSimpleShapePhantom.h>
#include <Physics/ConstraintSolver/Simplex/hkpSimplexSolver.h>

#include <Physics/Utilities/Constraint/Keyframe/hkpKeyFrameUtility.h>

#include <Common/Visualize/hkDebugDisplay.h>

#define HK_SURFACE_PRIORITY 41414

// The listener controls how the priority is dynamically set for objects
class MyCharacterPriorityListener : public hkReferencedObject, public hkpCharacterProxyListener
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DEMO );

		// We set the priority dynamically through the callback interface
		void processConstraintsCallback( const hkArray<hkpRootCdPoint>& manifold, hkpSimplexSolverInput& input ) 
		{
			// Iterate through the constraints and adjust the priority
			for (int i=0; i < input.m_numConstraints; i++)
			{
				hkpRigidBody* body = hkGetRigidBody( manifold[i].m_rootCollidableB );

				// If the object has a priority preset
				if (body && ( body->hasProperty(HK_SURFACE_PRIORITY) ) )
				{
					// Pass this priority to the simplex solver
					input.m_constraints[i].m_priority = body->getProperty(HK_SURFACE_PRIORITY).getInt();
				}
			}
		}
};

CharacterPriorityDemo::CharacterPriorityDemo(hkDemoEnvironment* env)
:	hkDefaultPhysicsDemo(env)
{
	//
	// Setup the camera
	//
	{
		hkVector4 from(0.0f, 10.0f, 10.0f);
		hkVector4 to  (0.0f,  0.0f,  0.0f);
		hkVector4 up  (0.0f,  1.0f,  0.0f);
		setupDefaultCameras( env, from, to, up );
	}

	//
	// Create the world
	//
	{
		hkpWorldCinfo info;
		info.setupSolverInfo( hkpWorldCinfo::SOLVER_TYPE_4ITERS_MEDIUM );
		info.setBroadPhaseWorldSize( 350.0f );  
		info.m_gravity.set(0, -9.81f, 0);
		info.m_collisionTolerance = 0.1f;
		
		m_world = new hkpWorld( info );
		m_world->lock();

		hkpAgentRegisterUtil::registerAllAgents(m_world->getCollisionDispatcher());

		setupGraphics();
	}

	// Build a Base
	hkVector4 baseSize( 50.0f, 1.0f, 50.0f);
	{ 
		hkpRigidBodyCinfo rci;
		rci.m_shape = new hkpBoxShape( baseSize );
		rci.m_position.set(0.0f, -0.5f, 0.0f);
		rci.m_motionType = hkpMotion::MOTION_FIXED;
			
		// Create a rigid body (using the template above).
		hkpRigidBody* base = new hkpRigidBody(rci);

		// Remove reference since the body now "owns" the Shape.
		rci.m_shape->removeReference();

		// Finally add body so we can see it, and remove reference since the world now "owns" it.
		m_world->addEntity( base )->removeReference();
	} 
	
	
	// Create a circle of keyframed objects 
	// Each of the objects is given a different increasing priority
	// We set the priority as a property on the object and extract this i nthe callback.
	hkVector4 blockerSize(1.0f, 1.5, 5);
	hkpShape* blocker = new hkpBoxShape( blockerSize );
	{
		hkPseudoRandomGenerator ran(100);
		for (int b = 0; b < NUM_OBJECTS; b++ )
		{
			hkVector4 up(0,1,0);
			hkReal angle = hkReal(b) / NUM_OBJECTS * HK_REAL_PI * 2;

			hkpRigidBodyCinfo rci;
			rci.m_position.set(3,0,0);
			rci.m_rotation.setAxisAngle( up, angle );
			rci.m_position.setRotatedDir( rci.m_rotation, rci.m_position );
			rci.m_shape = blocker;
			
			// If we set this to true, the body is fixed, and no mass properties need to be computed.
			rci.m_motionType = hkpMotion::MOTION_KEYFRAMED;

			m_objects[b] = new hkpRigidBody( rci );
			m_world->addEntity( m_objects[b] );
			m_objects[b]->removeReference();

			// As priority increases objects go from yellow to red
			hkReal r = hkReal(b) / (NUM_OBJECTS-1); 
			int color = hkColor::rgbFromFloats(1.0f, 1.0f-r, 0.0f, 1.0f);

			HK_SET_OBJECT_COLOR((hkUlong)m_objects[b]->getCollidable(), color );

			// We set this property and check it in the callback to see if we should modify how the character interacts with objects
			hkpPropertyValue priority( b + 5 );
			m_objects[b]->addProperty( HK_SURFACE_PRIORITY, priority );
		}
	}
	blocker->removeReference();

	//
	//	Create character controller
	//
	{
		// Construct a shape

		hkVector4 top(0, 0.4f, 0);
		hkVector4 bottom(0, -0.4f ,0);		

		// Create a capsule to represent the character standing
		hkpShape* standCapsule = new hkpCapsuleShape(top, bottom, 0.6f);

		// Construct a Shape Phantom
		m_phantom = new hkpSimpleShapePhantom( standCapsule, hkTransform::getIdentity() );
		standCapsule->removeReference();

		// Add the phantom to the world
		m_world->addPhantom(m_phantom);
		m_phantom->removeReference();

		// Construct a character proxy
		hkpCharacterProxyCinfo cpci;
		cpci.m_position.set(0.0f, 1.0f, 0.0f);
		cpci.m_dynamicFriction = 1.0f;
		cpci.m_up.setNeg4( m_world->getGravity() );
		cpci.m_up.normalize3();

		cpci.m_shapePhantom = m_phantom;
		m_characterProxy = new hkpCharacterProxy( cpci );
	}

	//
	// Add a listener that sets priority
	//
	{
		m_listener = new MyCharacterPriorityListener();
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
	
	m_prevObj = 0;

	// Zero current time at start
	m_currentTime = 0.0f;

	m_world->unlock();
}

CharacterPriorityDemo::~CharacterPriorityDemo()
{
	m_world->lock();

	m_world->removePhantom(m_characterProxy->getShapePhantom());
	m_characterProxy->removeReference();

	m_world->unlock();

	delete m_listener;
	delete m_characterContext;
}


/////////////////////////////////////////////////////////////////////

hkDemo::Result CharacterPriorityDemo::stepDemo()
{
	{
		m_world->lock();

		m_currentTime += m_timestep;

		// Move the blockers
		{
			hkReal scaledTime = m_currentTime * 0.2f;

			int currentObject = (int)scaledTime % NUM_OBJECTS;
			if (currentObject != m_prevObj)
			{
				m_prevObj = currentObject;

				hkVector4 pos;
				pos.set(0,3.0f,0);

				// Reset position
				m_characterProxy->setPosition(pos);

				// Reset velocity
				m_characterProxy->setLinearVelocity( hkVector4::getZero() );
			}

			for (int b = 0; b < NUM_OBJECTS; b++ )
			{
				hkVector4 pos;
				pos.setZero4();
				if (b == currentObject)
				{
					pos.set(hkMath::cos( scaledTime * HK_REAL_PI * 2 ) + 1.0f, 0, 0);
					pos(0) += 0.1f;
					pos.mul4(2.0f);
					pos(1) = b * 0.01f;
				}
				else
				{
					pos(0) = 1.7f;
					pos(1) = b * 0.01f;
				}

				pos.setRotatedDir( m_objects[b]->getRotation(), pos );
				m_objects[b]->activate();
				hkpKeyFrameUtility::applyHardKeyFrame( pos, m_objects[b]->getRotation(), 1.f / m_timestep, m_objects[b] );
			}
		}

		m_world->unlock();
	}

	// Step the world
	hkDefaultPhysicsDemo::stepDemo();

	{
		m_world->lock();

		//	Apply the player character controller
		{
			HK_TIMER_BEGIN( "simulate character", HK_NULL );

			// Add in some gravity
			hkVector4 velocity; 
			// To make the character fall faster in order to see the priorities better we use 4x gravity 
			const hkReal scale = 4.0f;
			velocity.setAddMul4( m_characterProxy->getLinearVelocity(), m_world->getGravity(), m_timestep * scale);
			m_characterProxy->setLinearVelocity(velocity);


			hkStepInfo si;
			si.m_deltaTime = m_timestep;
			si.m_invDeltaTime = 1.0f/m_timestep;
			m_characterProxy->integrate( si, m_world->getGravity() );

			HK_TIMER_END();
		}

		m_world->unlock();
	}

	return hkDemo::DEMO_OK;
}



#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

const char helpString [] =
"This game shows how the character controller uses priority to handle insolvable situations. " \
"By default, surfaces are automatically given one of three priorities based on their motion type: \n" \
"0 : Dynamic Motion types\n" \
"1 : Keyframed motion\n" \
"2 : Fixed motion\n" \
"However in this game each body is of KEYFRAMED motion type and we explicitly set each body's priority " \
"dynamically through the processConstraints() callback. " \
"The priority ranges from Low Priority (Yellow) to High Priority (Red).";

HK_DECLARE_DEMO(CharacterPriorityDemo, HK_DEMO_TYPE_PRIME, "Character Interaction Test", helpString);

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
