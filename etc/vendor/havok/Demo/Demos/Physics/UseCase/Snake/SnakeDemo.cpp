/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


///////////////////////////////////////////////////////////
//	Snakes, why did it have to be snakes...
///////////////////////////////////////////////////////////
#include <Demos/demos.h>
 #include <Demos/Physics/UseCase/Snake/SnakeDemo.h>

// We need to create a World
#include <Demos/DemoCommon/Utilities/GameUtils/GameUtils.h>

// We need to create bodies
#include <Physics/Dynamics/Constraint/Bilateral/Hinge/hkpHingeConstraintData.h>
#include <Physics/Dynamics/Constraint/Bilateral/LimitedHinge/hkpLimitedHingeConstraintData.h>
#include <Physics/Dynamics/Constraint/Motor/Position/hkpPositionConstraintMotor.h>

#include <Physics/Utilities/Collide/Filter/ConstrainedSystem/hkpConstrainedSystemFilter.h>
#include <Physics/Utilities/Constraint/Bilateral/hkpConstraintUtils.h>
#include <Physics/Utilities/Collide/Filter/GroupFilter/hkpGroupFilterUtil.h>

#include <Graphics/Common/Input/Pad/hkgPad.h>


// snake has 3 speeds
#define SNAKE_FAST_PERIOD 0.75f
#define SNAKE_NORMAL_PERIOD 1.5f
#define SNAKE_SLOW_PERIOD 3.0f


// This is an example of using hkpGroupFilter and hkConstraintBasedCollisionFilter.
//
// The constraint based collision filter disables collisions between bodies having at least
// one (non-contact) constraint. To make it work, you actually have to call updateCollisionFilter
// on every entity once it is added to the world, and its constraints are added. That's because
// the collision filter is called initially upon body addition, when it doesn't have any constraints yet, 
// and agents are created.

//#define USE_CONSTRAINT_BASED_COLLISION_FILTER 	//otherwise use hkpGroupFilter

DEMO_OPTIONS_DEFINE(SnakeDemo,Options);

SnakeDemo::SnakeDemo(hkDemoEnvironment* env) : hkDefaultPhysicsDemo(env)
{
	//
	// Setup the camera
	//
	{
		hkVector4 from(-20.0f, 20.0f, -20.0f);
		hkVector4 to  (  0.0f,  0.0f,   0.0f);
		hkVector4 up  (  0.0f,  1.0f,   0.0f);
		setupDefaultCameras( env, from, to, up );
	}


	//
	// Create the world
	//
	{
		hkpWorldCinfo info;
		info.setupSolverInfo(hkpWorldCinfo::SOLVER_TYPE_4ITERS_MEDIUM);
		info.setBroadPhaseWorldSize( 350.0f );  
		m_world = new hkpWorld( info );
		m_world->lock();

		setupGraphics();

		hkpAgentRegisterUtil::registerAllAgents(m_world->getCollisionDispatcher());
	}
         
	//
	// Disable collisions
	//
#if defined USE_CONSTRAINT_BASED_COLLISION_FILTER
	{
		hkpConstrainedSystemFilter* cf = new hkpConstrainedSystemFilter();
		m_world->setCollisionFilter(cf);
			// Note: you must remember to manually remove the constrained-system filter from the constraint listeners list
			//       when replacing the filter.
		m_world->addConstraintListener(cf);
		cf->removeReference();
	}
#else
	{
       hkpGroupFilter* cFilter1 = new hkpGroupFilter();
	   cFilter1->disableCollisionsBetween(LAYER_SNAKE, LAYER_SNAKE); 
	   cFilter1->enableCollisionsBetween(LAYER_GROUND, LAYER_SNAKE); 
	   m_world->setCollisionFilter(cFilter1);
	   cFilter1->removeReference();
	}	
#endif 
	//
	// Create ground 
	//
	{ 
		hkVector4 halfSize(150.0f, 1.0f, 150.0f);
		hkVector4 position(0.0f, 0.0f, 0.0f);
		hkReal mass = 0.0f;
		hkpRigidBody* ground = GameUtils::createBox( halfSize, mass, position );
		m_world->addEntity(ground);
		ground->removeReference();
#		if !defined USE_CONSTRAINT_BASED_COLLISION_FILTER
			ground->getCollidableRw()->setCollisionFilterInfo( hkpGroupFilter::calcFilterInfo(LAYER_GROUND) );
#		endif
	} 

	//
    // create our snake
	//
	{
		hkpRigidBody* front = HK_NULL;
		hkpRigidBody* back = HK_NULL;

		int numSegments = 16;
		hkVector4 segmentHalfSize( 0.25f, 0.25f, 0.5f ); 
		hkVector4 segmentPosition( 0.0f, 1.0f, -numSegments * segmentHalfSize(2) );
		hkVector4 axis( 0.0f, 1.0f, 0.0f );

		// link together segments with powered hinges
		int iSegment;
		for( iSegment = 0; iSegment < numSegments; iSegment++ )
		{
			//
			// We create a new segment here. A body is added, and collision agents are created.
			// This function also adds constraints linking to other semgents, which triggers
			// an extra verification of the collision filter (triggered via its contraint listener
			// functionality).
			//
			front = createSegment( segmentPosition, segmentHalfSize );

			if( back )
			{
				hkVector4 linkPosition = segmentPosition;
				linkPosition(2) -= segmentHalfSize(2);
				linkSegment( front, back, linkPosition, axis );
			}

			segmentPosition(2) += segmentHalfSize(2)*2.0f;
			back = front;

		}

		m_segmentLength = segmentHalfSize(2)*2.0f;

		// wave front keeps track of the translation of the wave, increasing over time.
		m_waveFront = 0.0f;
		// our steering value
		m_steering = 0.0f;
	}

	m_world->unlock();
}

/////////////////////////////////////////////////////////////////////

// link together two segments of a snake with a powered hinge and add the motor
void SnakeDemo::linkSegment( hkpRigidBody* front, hkpRigidBody* back, hkVector4& pos, hkVector4& axis )
{
	hkpLimitedHingeConstraintData* lhc = new hkpLimitedHingeConstraintData( );
	lhc->setInWorldSpace( front->getTransform(), back->getTransform(), pos, axis );

	// don't need limits
	lhc->disableLimits();

	m_muscles.pushBack( lhc );

	hkpConstraintInstance* instance = m_world->createAndAddConstraintInstance( front,back, lhc );
	lhc->removeReference();

	// create the motor that drives the snake ( simulates the muscle ).
	// set it up as inactive to start, we control all snake power in slither()
	hkpPositionConstraintMotor* pcm = new hkpPositionConstraintMotor( 0 );
	pcm->m_maxForce = 15.0f;
	pcm->m_tau = 0.1f;

	lhc->setMotor( pcm );
	lhc->setMotorActive(instance, true);
	pcm->removeReference();
	instance->removeReference();
}


// create a snake body segment with a wheel on the bottom.
// the wheel provides asymetric friction, which is essential for this type of snake locomotion
hkpRigidBody* SnakeDemo::createSegment( hkVector4& segmentPosition, hkVector4& segmentHalfSize )
{

	hkVector4 segmentSize;
	segmentSize.setMul4( 2.0f, segmentHalfSize );
	hkpRigidBody* segment = GameUtils::createBox( segmentSize, 0.5f, segmentPosition );

#	if !defined USE_CONSTRAINT_BASED_COLLISION_FILTER
		// don't want wheels colliding with body
		segment->getCollidableRw()->setCollisionFilterInfo( hkpGroupFilter::calcFilterInfo( LAYER_SNAKE ) );
#	endif

	m_world->addEntity(segment);
	segment->removeReference();

	// create the wheel rigidbody
	hkVector4 wheelPos = segmentPosition;
	wheelPos(1) -= segmentHalfSize(1) * 0.75f;
	hkpRigidBody* wheel = GameUtils::createSphere( segmentHalfSize(1) * 0.75f, 0.5f, wheelPos );
	wheel->getMaterial().setFriction( 1.0f );
#	if !defined USE_CONSTRAINT_BASED_COLLISION_FILTER
		wheel->getCollidableRw()->setCollisionFilterInfo( hkpGroupFilter::calcFilterInfo( LAYER_SNAKE ) );
#	endif
	// give the wheels a bit of friction, because a snakes belly would have more friction.
	wheel->setAngularDamping( 2.0f ); 

	m_world->addEntity(wheel);
	wheel->removeReference();

	// create the hinge constraint
	hkpHingeConstraintData* wheelCon = new hkpHingeConstraintData( );
	hkVector4 axel(1.0f, 0.0f, 0.0f);
	wheelCon->setInWorldSpace( wheel->getTransform(), segment->getTransform(), wheelPos, axel );

	m_wheels.pushBack( wheelCon );

	m_world->createAndAddConstraintInstance( wheel, segment, wheelCon )->removeReference();
	wheelCon->removeReference(); 
	
	return segment;
}


// simple snake locomotion technique.  
// http://www.snakerobots.com to see one of these in the wild 
void SnakeDemo::slither( hkReal dt )
{
	int numMuscles = m_muscles.getSize();

	hkReal scaleStart = 1.0f;

	// start of the snake with smaller waves.  
	// I think starting the wave at the tail would work better ( while it still goes from head to tail )
	if( m_waveFront < m_segmentLength*4.0f )
	{
		scaleStart = m_waveFront/(m_segmentLength*4.0f);
	}

	int segment = (int)numMuscles-1;

	// we simply move a sin wave down the body of the snake.
	// this snake may be going backwards, but who can tell ;)
	for( ; segment >= 0; segment-- )
	{
		// map segment to phase
		hkReal phase = (m_waveFront - (segment+1)*m_segmentLength)/ m_options.m_waveLength;
		phase -= floorf(phase);

		phase *= HK_REAL_PI * 2.0f;

		// map phase to curvature

		// set our motor
		hkpLimitedHingeConstraintData* lhc = static_cast<hkpLimitedHingeConstraintData*>( m_muscles[segment] );

		// set up the muscles of the snake
		lhc->setMotorTargetAngle( hkMath::sin( phase ) * scaleStart * m_options.m_waveAmplitude );


		// steer snake by squashing +ve or -ve side of sin curve
		if( m_steering > 0 && lhc->getMotorTargetAngle() < 0 )
		{
			lhc->setMotorTargetAngle( lhc->getMotorTargetAngle() * 1.0f/( 1.0f + m_steering ) );
		}
		
		if( m_steering < 0 && lhc->getMotorTargetAngle() > 0 )
		{
			lhc->setMotorTargetAngle( lhc->getMotorTargetAngle() * 1.0f/( 1.0f - m_steering ) );
		}

	}
	// wave keeps track of where the wave is in time
	m_waveFront += dt/m_options.m_wavePeriod * m_options.m_waveLength;
}


// get user input and drive the snake
hkDemo::Result SnakeDemo::stepDemo()
{
	const hkgPad* pad = m_env->m_gamePad;

	m_steering = 0.0f;
	m_options.m_wavePeriod = SNAKE_NORMAL_PERIOD;

	// steer left and right
	if( (pad->getButtonState() & HKG_PAD_DPAD_LEFT) )
	{
		m_steering = -5.0f;
	}

	if( (pad->getButtonState() & HKG_PAD_DPAD_RIGHT) )
	{
		m_steering = 5.0f;
	}

	// control speed
	if( (pad->getButtonState() & HKG_PAD_DPAD_UP) )
	{
		m_options.m_wavePeriod = SNAKE_FAST_PERIOD;
	}

	if( (pad->getButtonState() & HKG_PAD_DPAD_DOWN) )
	{
		m_options.m_wavePeriod = SNAKE_SLOW_PERIOD;
	}

	// modify the wave length
	if( (pad->getButtonState() & HKG_PAD_BUTTON_3) )
	{
		if( m_options.m_waveLength < m_muscles.getSize() * m_segmentLength * 0.5f)
		{
			m_options.m_waveLength++;
		}
	}

	// drive the snake with side-to-side slithering
	slither( m_timestep ); 

	return hkDefaultPhysicsDemo::stepDemo();
}


//////////////////////////////////////////////////

#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif


static const char helpString[] = \
"This demo creates a snake creature out of powered hinges. " \
"The forward locomotion falls naturally out of the simulation. " \
"Left and right to steer, forward and back to change speed. " \
"\223 to increase the wavelength.";

HK_DECLARE_DEMO(SnakeDemo, HK_DEMO_TYPE_PRIME, "Powered Constraint Snake Demo", helpString);

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
