/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


#include <Demos/demos.h>
#include <Demos/Physics/Api/Dynamics/RigidBodies/MotionTypeChange/MotionTypeChangeDemo.h>

#include <Physics/Dynamics/Constraint/Bilateral/StiffSpring/hkpStiffSpringConstraintData.h>
#include <Physics/Dynamics/Constraint/Bilateral/Hinge/hkpHingeConstraintData.h>

#include <Physics/Collide/Shape/Misc/Transform/hkpTransformShape.h>
#include <Physics/Collide/Shape/Compound/Collection/List/hkpListShape.h>


	// We need to display some collision based info
#include <Common/Visualize/hkDebugDisplay.h>

#include <Demos/DemoCommon/DemoFramework/hkTextDisplay.h>


// Given below are the core functions for this demo.
// To switch between Fixed, Dynamic and Keyframing for any given object all that
// we need do is set their motion type to the desired one.

// Note: In order to be able to switch between motion types the object
//       must initially not be created with a fixed motion type.


void MotionTypeChangeDemo::setClockToFixed()
{

	// Fix the pointer
	m_pointer->setMotionType( hkpMotion::MOTION_FIXED );

	// Fix the spheres that make up the hand
	for( int i = 0; i<m_handSpheres.getSize(); i++ )
	{
		m_handSpheres[i]->setMotionType( hkpMotion::MOTION_FIXED );
	}

}

void MotionTypeChangeDemo::setClockToKeyframed()
{
	// We need only keyframe the pointer as the other parts will follow due to the contraints.
	m_pointer->setMotionType( hkpMotion::MOTION_KEYFRAMED );
}


void MotionTypeChangeDemo::setClockToDynamic()
{
	// Set the pointer to dynamic
	m_pointer->setMotionType( hkpMotion::MOTION_DYNAMIC );

	// Set the spheres that make up the hand to be dynamic (except the 1st one, otherwise the hand would fall away from the clock!)
	for( int i = 1; i<m_handSpheres.getSize(); i++ )
	{
		m_handSpheres[i]->setMotionType( hkpMotion::MOTION_DYNAMIC );
	}

}


MotionTypeChangeDemo::MotionTypeChangeDemo(hkDemoEnvironment* env)
:	hkDefaultPhysicsDemo(env)

{
	//
	// Setup the camera
	//
	{
		hkVector4 from( 0.0f,  -0.5f, 18.0f);
		hkVector4 to  ( 0.0f,  -0.5f,   0.0f);
		hkVector4 up  ( 0.0f,  1.0f,   0.0f);
		setupDefaultCameras( env, from, to, up );
	}

	//
	// Setup and create the hkpWorld
	//
	{
		hkpWorldCinfo info;
		info.m_gravity = hkVector4(0.0f, -4.9f, 0.0f);
		info.setBroadPhaseWorldSize(150.0f);
		info.setupSolverInfo(hkpWorldCinfo::SOLVER_TYPE_8ITERS_MEDIUM);
		info.m_collisionTolerance = 0.01f; 
		
		m_world = new hkpWorld( info );
		m_world->lock();

		setupGraphics();		
	}

	// Register all agents
	{
		hkpAgentRegisterUtil::registerAllAgents(m_world->getCollisionDispatcher());
	}

	// Setup a group collision filter so that the spheres will not collide
	{
		hkpGroupFilter* filter = new hkpGroupFilter();
		filter->disableCollisionsBetween( hkpGroupFilterSetup::LAYER_DYNAMIC, hkpGroupFilterSetup::LAYER_DYNAMIC );
		m_world->setCollisionFilter( filter );
		filter->removeReference();
	}

	// Set shade of grey for the hand in the different game states
	m_fixedColour = 0x90;
	m_dynamicColour = 0xe0;

	//
	// Create the clock
	// The clock is created in four sections; the 'face', the central hub, the 'pointer' and the rest of the hand.
	//

	{
		hkpRigidBodyCinfo info;

		// Different pieces of the clock do not collide
		info.m_collisionFilterInfo = hkpGroupFilterSetup::LAYER_DYNAMIC;

		info.m_angularDamping = 0.0f;
		info.m_linearDamping = 0.0f;

		//
		// Create the twelve numeral points of the face
		//

		{
			hkpSphereShape* numeralPointShape = new hkpSphereShape( 1.0f );
			info.m_shape = numeralPointShape;

			info.m_rotation.setIdentity();
			info.m_motionType = hkpMotion::MOTION_FIXED;

			for( int i = 0; i<12; i++ )
			{
				info.m_position.set( hkMath::sin( NUMERAL_ANGLE * i ) * FACE_RADIUS, hkMath::cos( NUMERAL_ANGLE * i ) * FACE_RADIUS, 0.0f );

				hkpRigidBody* rb = new hkpRigidBody( info );
				m_world->addEntity( rb );

				// Colour the quarterly points lighter
				if( !(i%3) )
				{
					HK_SET_OBJECT_COLOR((hkUlong)rb->getCollidable(), 0xffb0b0b0);
				}

				rb->removeReference();
			}

			numeralPointShape->removeReference();
		}
		
		//
		// Create the central hub
		//

		{
			hkpSphereShape* centralHubShape = new hkpSphereShape( 1.3f );
			info.m_shape = centralHubShape;

			info.m_position.setZero4();

			hkpRigidBody* rb = new hkpRigidBody( info );
			m_world->addEntity( rb );

			HK_SET_OBJECT_COLOR((hkUlong)rb->getCollidable(), 0xffb0b000);		

			rb->removeReference();
			centralHubShape->removeReference();
		}

		//
		// Create the hand pointer piece
		//

		{
			hkArray<hkVector4> spheres;
			
			spheres.pushBack( hkVector4( 0.0f, 0.45f, 0.0f, 0.3f ) );
			spheres.pushBack( hkVector4( -0.45f, -0.45f, 0.0f, 0.3f ) );
			spheres.pushBack( hkVector4( 0.45f, -0.45f, 0.0f, 0.3f ) );

			hkVector4 middleSphere( 0.0f, 0.0f, 0.0f );
			middleSphere.setInterpolate4( spheres[0], spheres[1], 0.5f );
			spheres.pushBack( middleSphere );

			middleSphere.setZero4();
			middleSphere.setInterpolate4( spheres[1], spheres[2], 0.5f );
			spheres.pushBack( middleSphere );

			middleSphere.setZero4();
			middleSphere.setInterpolate4( spheres[0], spheres[2], 0.5f );
			spheres.pushBack( middleSphere );

			hkpSphereShape* handPointerReusedSphere = new hkpSphereShape(spheres[0](3));
			hkArray<hkpShape*> handPointerSpheres(spheres.getSize());
			for(int q=0;q<spheres.getSize();++q)
			{
				hkTransform xfrm;
				xfrm.setIdentity();
				xfrm.setTranslation(spheres[q]);
				xfrm.getTranslation()(3) = 0;
				hkpTransformShape * xfrmShape = new hkpTransformShape(handPointerReusedSphere,xfrm);
				handPointerSpheres[q] = xfrmShape;
			}
			handPointerReusedSphere->removeReference();
			hkpListShape * handPointerShape = new hkpListShape(handPointerSpheres.begin(),handPointerSpheres.getSize());
			for(int r=0;r<handPointerSpheres.getSize();++r)
			{
				handPointerSpheres[r]->removeReference();
			}
			info.m_shape = handPointerShape;

			// Start pointer at 1 o'clock
			hkVector4 numeralOne( hkMath::sin( NUMERAL_ANGLE ) * FACE_RADIUS, hkMath::cos( NUMERAL_ANGLE ) * FACE_RADIUS, 0.0f );

			info.m_position.setInterpolate4( hkVector4::getZero(), numeralOne, 0.7f );
			info.m_position(2) = 1.5f;

			const hkQuaternion Quat( hkVector4( 0.0f, 0.0f, -1.0f ), NUMERAL_ANGLE );
			hkRotation initialRotation; initialRotation.set( Quat );
			info.m_rotation.set( initialRotation );

			info.m_motionType = hkpMotion::MOTION_SPHERE_INERTIA;
			
			// Since a Multisphere shape is not handled directly we'll use the sphere centers as a cloud mass for the calculation.
			hkReal mass = 10.0f;
			hkpMassProperties massProperties;
			hkpInertiaTensorComputer::computeVertexCloudMassProperties( reinterpret_cast<float*>(&spheres[0]), 4*sizeof(float), 6, mass, massProperties);
	
			info.m_inertiaTensor = massProperties.m_inertiaTensor;
			info.m_centerOfMass = massProperties.m_centerOfMass;
			info.m_mass = massProperties.m_mass;			

			m_pointer = new hkpRigidBody( info );
			m_world->addEntity( m_pointer );

			HK_SET_OBJECT_COLOR((hkUlong)m_pointer->getCollidable(), ( 0xff000000 | ( m_fixedColour << 16 ) | ( m_fixedColour << 8 ) | m_fixedColour ) );	

			handPointerShape->removeReference();
		}

		//
		// Create the hand itself
		//

		{
			info.m_motionType = hkpMotion::MOTION_SPHERE_INERTIA;

			hkReal mass = 10.0f;
			hkpMassProperties massProperties;
			hkpInertiaTensorComputer::computeSphereVolumeMassProperties( 0.2f, mass, massProperties );

			info.m_inertiaTensor = massProperties.m_inertiaTensor;
			info.m_centerOfMass = massProperties.m_centerOfMass;
			info.m_mass = massProperties.m_mass;			

			info.m_rotation.setIdentity();

			hkVector4 numeralOne( hkMath::sin( NUMERAL_ANGLE ) * FACE_RADIUS, hkMath::cos( NUMERAL_ANGLE ) * FACE_RADIUS, 0.0f );

			hkReal spacing = 0.6f/NUM_HAND_PIECES;

			for( int i = 0; i<NUM_HAND_PIECES; i++ )
			{

				info.m_position.setInterpolate4( hkVector4::getZero(), numeralOne, spacing * i );
				info.m_position(2) = 1.5f;

				// Increase the radius of the spheres as we go.
				info.m_shape = new hkpSphereShape( 0.1f+(i*0.045f) );

				hkpRigidBody* rb = new hkpRigidBody( info );
				info.m_shape->removeReference();

				m_world->addEntity( rb );

				HK_SET_OBJECT_COLOR((hkUlong)rb->getCollidable(), ( 0xff000000 | ( m_fixedColour << 16 ) | ( m_fixedColour << 8 ) | m_fixedColour ) );

				m_handSpheres.pushBack( rb );

			}


		}

		//
		// Create constraints between the pieces of the hands and pointer
		//

		// Depending on the value of CONTRAINT_TYPE we use either stiff spring constraints or hinge constraints
		if( CONSTRAINT_TYPE )
		{
			hkReal spacing = 0.6f/NUM_HAND_PIECES;

			for( int i = 0; i<NUM_HAND_PIECES-1; i++ )
			{
				hkpStiffSpringConstraintData* spring = new hkpStiffSpringConstraintData();
				spring->setSpringLength( spacing );
				spring->setInWorldSpace( m_handSpheres[i]->getTransform(), m_handSpheres[i+1]->getTransform(),
					m_handSpheres[i]->getTransform().getTranslation(), 
					m_handSpheres[i+1]->getTransform().getTranslation() );

				hkpConstraintInstance* constraint = new hkpConstraintInstance(m_handSpheres[i], m_handSpheres[i+1], spring);
				m_world->addConstraint( constraint );
				constraint->removeReference();
				spring->removeReference();
			}

			{
				hkpStiffSpringConstraintData* spring = new hkpStiffSpringConstraintData();
				spring->setSpringLength( spacing );
				spring->setInWorldSpace( m_handSpheres[NUM_HAND_PIECES-1]->getTransform(), m_pointer->getTransform(), 
					m_handSpheres[NUM_HAND_PIECES-1]->getTransform().getTranslation(), 
					m_pointer->getTransform().getTranslation() );
                
				hkpConstraintInstance* constraint = new hkpConstraintInstance(m_handSpheres[NUM_HAND_PIECES-1], m_pointer, spring);
				m_world->addConstraint( constraint );
				constraint->removeReference();
				spring->removeReference();
			}
		}
		else
		{
			hkVector4 axis(0.0f, 0.0f, 1.0f);

			for( int i = 0; i<NUM_HAND_PIECES-1; i++ )
			{
				hkVector4 pivot;		
				pivot.setInterpolate4( m_handSpheres[i]->getTransform().getTranslation(), m_handSpheres[i+1]->getTransform().getTranslation(), 0.5f);
	
				// Create constraint
				hkpHingeConstraintData* hc = new hkpHingeConstraintData(); 
				hc->setInWorldSpace( m_handSpheres[i]->getTransform(), m_handSpheres[i+1]->getTransform(), pivot, axis);
				
				hkpConstraintInstance* constraint = new hkpConstraintInstance(m_handSpheres[i], m_handSpheres[i+1], hc);
				m_world->addConstraint( constraint );
				constraint->removeReference();
				hc->removeReference();
			}

			{
				hkVector4 pivot;		
				pivot.setInterpolate4( m_handSpheres[NUM_HAND_PIECES-1]->getTransform().getTranslation(), m_pointer->getTransform().getTranslation(), 0.2f);

				hkpHingeConstraintData* hc = new hkpHingeConstraintData(); 
				hc->setInWorldSpace( m_handSpheres[NUM_HAND_PIECES-1]->getTransform(), m_pointer->getTransform(),
									pivot, axis);

				hkpConstraintInstance* constraint = new hkpConstraintInstance( m_handSpheres[NUM_HAND_PIECES-1], m_pointer, hc);
				m_world->addConstraint( constraint );
				constraint->removeReference();
				hc->removeReference();
			}
		}
	}

	// Set game state
	m_demoState = DEMO_STATE_FIXED;

	// Set the initial states for this mode
	m_fixedState = INIT_FIXED_STATE;

	// Reset our game state switching timer
	m_stateSwitchTimer = 0;

	m_world->unlock();
}


hkDemo::Result MotionTypeChangeDemo::stepDemo()
{
	m_world->lock();

	//
	// This is the main game state changing logic.
	// The states are switched between FIXED, DYNAMIC and KEYFRAMED.
	// Each of these states has a sub state: the INIT and MID states.
	// These are used to setup the state and continue in that state.
	//

	switch( m_demoState )
	{
	case DEMO_STATE_FIXED:
		{
			switch( m_fixedState )
			{
			case INIT_FIXED_STATE:
				{
					// Set our clock to be fixed
					setClockToFixed();
					m_fixedState = MID_FIXED_STATE;

					hkString s("Clock has changed to fixed state.");
					m_env->m_textDisplay->outputText(s, 170, 425);

					break;
				}

			case MID_FIXED_STATE:
				{
					// Increment our state switching timer
					if( (m_stateSwitchTimer++) == STATE_SWITCH_PERIOD )
					{
						// Reset timer
						m_stateSwitchTimer = 0;

						// Set new game state
						m_demoState = DEMO_STATE_DYNAMIC;
						m_dynamicState = INIT_DYNAMIC_STATE;

						// Update object colours
						HK_SET_OBJECT_COLOR((hkUlong)m_pointer->getCollidable(), ( 0xff000000 | ( m_dynamicColour << 16 ) | ( m_dynamicColour << 8 ) | m_dynamicColour ) );

						for( int i = 0; i<m_handSpheres.getSize(); i++ )
						{
							HK_SET_OBJECT_COLOR((hkUlong)m_handSpheres[i]->getCollidable(), ( 0xff000000 | ( m_dynamicColour << 16 ) | ( m_dynamicColour << 8 ) | m_dynamicColour ) );
						}

					}

					hkString s("Clock has changed to fixed state.");
					m_env->m_textDisplay->outputText(s, 170, 425);

					break;
				}
			}

			break;

		}

	case DEMO_STATE_KEYFRAMED:
		{
			switch( m_keyframedState )
			{
			case INIT_KEYFRAMED_STATE:
				{
					// Set our clock to be keyframed
					setClockToKeyframed();
					m_keyframedState = MID_KEYFRAMED_STATE;

					// Set the source and destination points (position and orientation) for the keyframed motion
					m_keyframedSourceP = m_pointer->getTransform().getTranslation();
					m_keyframedSourceQ = m_pointer->getRotation();

					hkVector4 numeralOne( hkMath::sin( NUMERAL_ANGLE ) * FACE_RADIUS, hkMath::cos( NUMERAL_ANGLE ) * FACE_RADIUS, 0.0f );
					numeralOne.setInterpolate4( hkVector4::getZero(), numeralOne, 0.7f );
					numeralOne(2) = 1.5f;
					m_keyframedDestinationP = numeralOne;

					hkVector4 axis( 0.0f, 0.0f, -1.0f );
					hkQuaternion quat( axis, NUMERAL_ANGLE );
					m_keyframedDestinationQ = quat;

					// Reset keyframe counter
					m_keyframeCounter = 1;

					hkString s("Clock has changed to keyframed state.");
					m_env->m_textDisplay->outputText(s, 170, 425);

					break;
				}

			case MID_KEYFRAMED_STATE:
				{
					// Calculate the new position to keyframe to
					hkVector4 newPosition;
					newPosition.setInterpolate4( m_keyframedSourceP, m_keyframedDestinationP, (1.0f/NUM_KEYFRAMED_STEPS) * m_keyframeCounter );

					// Calculate the new orientation to keyframe to
					hkQuaternion quat;
					quat.setSlerp( m_keyframedSourceQ, m_keyframedDestinationQ, (1.0f/NUM_KEYFRAMED_STEPS) * m_keyframeCounter );

					// Perform the keyframing
					hkpKeyFrameUtility::applyHardKeyFrame( newPosition, quat, 1.0f/m_timestep, m_pointer );

					// Interpolate between the dynamic and fixed colours
					hkUint32 newColour = m_dynamicColour - m_fixedColour;
					newColour *= m_keyframeCounter;
					newColour /= NUM_KEYFRAMED_STEPS;
					hkUint32 rgbColour = m_dynamicColour - newColour;

					HK_SET_OBJECT_COLOR((hkUlong)m_pointer->getCollidable(), ( 0xff000000 | ( rgbColour << 16 ) | ( rgbColour << 8 ) | rgbColour ) );	

					for( int i = 0; i<m_handSpheres.getSize(); i++ )
					{
						HK_SET_OBJECT_COLOR((hkUlong)m_handSpheres[i]->getCollidable(), ( 0xff000000 | ( rgbColour << 16 ) | ( rgbColour << 8 ) | rgbColour ) );
					}

					// Increment the keyframe counter
					if( (m_keyframeCounter++) == NUM_KEYFRAMED_STEPS )
					{
						// Set new game state
						m_demoState = DEMO_STATE_FIXED;
						m_fixedState = INIT_FIXED_STATE;
					}

					hkString s("Clock has changed to keyframed state.");
					m_env->m_textDisplay->outputText(s, 170, 425);

					break;
				}

			}

			break;

		}

	case DEMO_STATE_DYNAMIC:
		{
			switch( m_dynamicState )
			{
			case INIT_DYNAMIC_STATE:
				{
					// Set our clock to be dynamic
					setClockToDynamic();
					m_dynamicState = MID_DYNAMIC_STATE;

					hkString s("Clock has changed to dynamic state.");
					m_env->m_textDisplay->outputText(s, 170, 425);

					break;
				}

			case MID_DYNAMIC_STATE:
				{
					// Increment our state switching timer
					if( (m_stateSwitchTimer++) == STATE_SWITCH_PERIOD )
					{
						// Reset timer
						m_stateSwitchTimer = 0;

						// Set new game state
						m_demoState = DEMO_STATE_KEYFRAMED;
						m_keyframedState = INIT_KEYFRAMED_STATE;

					}

					hkString s("Clock has changed to dynamic state.");
					m_env->m_textDisplay->outputText(s, 170, 425);

					break;
				}
			}

			break;
		}
	}

	m_world->unlock();

	return hkDefaultPhysicsDemo::stepDemo();
}

MotionTypeChangeDemo::~MotionTypeChangeDemo()
{
	m_world->lock();

	// Tidy up our references
	m_pointer->removeReference();

	for( int i = 0; i<m_handSpheres.getSize(); i++ )
	{
		m_handSpheres[i]->removeReference();
	}

	m_world->unlock();
}



#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
"This game shows how to switch between DYNAMIC, FIXED and KEYFRAMED motion states";

HK_DECLARE_DEMO(MotionTypeChangeDemo, HK_DEMO_TYPE_PRIME, "Dynamically changing the motion type for a rigid body", helpString);

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
