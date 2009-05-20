/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


///[TUTORIAL 2]

///[CODE allcode]<


#include <Demos/demos.h>

#include <Physics/Collide/Dispatch/hkpCollisionDispatcher.h>
#include <Physics/Internal/Collide/Agent3/PredGskAgent3/hkpPredGskAgent3.h>

	// Here's our funky action
#include <Demos/DemoCommon/Utilities/MarbleAction/MarbleAction.h>

	// we need to access our game pad
#include <Graphics/Common/Input/Pad/hkgPad.h>

#include <Demos/Physics/Test/Performance/Pyramid/PyramidDemo.h>

#include <Physics/Utilities/VisualDebugger/Viewer/Dynamics/hkpSimulationIslandViewer.h>




////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Definitions of game variants
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static const int MY_STACK_SIZE = 2000000;

struct PyramidVariant
{
	const char*	 m_name;
	bool   m_usePredictive;
	int    m_numRows;
	hkReal m_boxSize;
	hkpCollidableQualityType m_qualityType;
	bool   m_useGsk;
	hkReal m_allowedPenetration;
	const char* m_details;
};

#define M HK_COLLIDABLE_QUALITY_MOVING
#define C HK_COLLIDABLE_QUALITY_CRITICAL

#define BOX false
#define GSK true

static const PyramidVariant g_variants[] =
{
{ "Pyramid",     false,  5, 1.0f, M,  BOX,  0.f, "Pyramid made of 5 rows of 1 meter cubes, using the high quality box-box agent." },
{ "Discrete,   1m box agent, 5 rows",     false,  5, 1.0f, M,  BOX,  0.f, "Pyramid made of 5 rows of 1 meter cubes, using the high quality box-box agent." },
{ "Discrete,   1m box agent, 10 rows",     false, 10, 1.0f, M,  BOX,  0.f, "Same but 10 Rows." },
{ "Discrete,   1m box agent, 20 rows",     false, 20, 1.0f, M,  BOX,  0.f, "Same but 20 Rows, however changing solver to hard." },

{ "Discrete,   1m gsk, 5 rows",		  false,  5, 1.0f, M,  GSK,  0.f, "Pyramid of 1 meter cubes, using the general convex convex collision detection (gsk)." },

{ "Discrete,   10cm box agent, 5 rows",     false, 5,  0.1f, M,  BOX, .00f, "10 cm boxes using the box box agent, works well." },
{ "Discrete,   10cm gsk, 5 rows",     false, 5,  0.1f, M,  GSK, .00f, "If we use the general convex object agent, it still works, but some jitter is noticeable" },
{ "Continuous, 10cm QT_MOVING gsk, 5 rows",      true , 5,  0.1f, M,  BOX, .02f, "With continuous simulation between the boxes and the ground," \
																				 "the engine has to use the gsk agent. Therefor there is some small jitter "\
																				 "noticeable between the ground and the boxes." },
{ "Discrete,   3cm gsk, 4 rows", false, 4,  0.03f, M,  GSK,   .0f, "3 cm cubes work well. In this case we increased the inertia by a factor of 2." },
{ "Continuous, 3cm QT_MOVING gsk, 4 rows",  true, 4,  0.03f, M,  GSK, .007f, "Adding continuous physics between objects and ground stops objects from tunneling through the ground." },
{ "Continuous, 3cm QT_CRITICAL gsk, 4 rows",  true, 4,  0.03f, C,  GSK, .007f, "Using full continuous physics between all bodies gives us the highest quality using maximum CPU power." },

{ "Discrete,   2cm gsk, 3 rows",  false, 3,  0.02f, M,  GSK, .0f,   "2 cm cubes just work without continuous physics, however those object easily tunnel through." },
{ "Continuous, 2cm QT_MOVING gsk, 3 rows",  true , 3,  0.02f, M,  GSK, .005f, "Adding continuous physics between objects and ground stops objects from tunneling through the ground." },
{ "Continuous, 2cm QT_CRITICAL gsk, 3 rows",  true , 3,  0.02f, C,  GSK, .005f, "Using full continuous physics between all bodies gives us the highest quality using maximum CPU power." },

{ "Discrete,   1cm gsk, 2 rows", false, 2,  0.01f, M,  GSK, .0f,    "Very small boxes don't work well with discrete physics (they might or might not work)." },
{ "Continuous, 1cm QT_MOVING gsk, 2 rows", true , 2,  0.01f, M,  GSK, .0015f, "Using continuous physics just between the ground and the boxes doesn't help much." },
{ "Continuous, 1cm QT_CRITICAL gsk, 2 rows", true , 2,  0.01f, C,  GSK, .0015f, "Using continuous physics between all object is expensive but just works." },
};
#undef M
#undef C


PyramidDemo::PyramidDemo(hkDemoEnvironment* env)
:	hkDefaultPhysicsDemo(env)
{
	const PyramidVariant& variant =  g_variants[m_variantId];

	//
	//	Create some memory for our scratch pad
	//
	m_oldStack = hkThreadMemory::getInstance().getStack();
	m_stackBuffer = hkAllocate<char>( MY_STACK_SIZE, HK_MEMORY_CLASS_DEMO );
	hkThreadMemory::getInstance().setStackArea( m_stackBuffer, MY_STACK_SIZE );


	//
	// Setup the camera
	//
	{
		hkVector4 from(5.0f, 8.0f, 16.0f);
		from.mul4( variant.m_boxSize );
		hkVector4 to  (0.0f, 0.0f,  0.0f);
		hkVector4 up  (0.0f, 1.0f,  0.0f);
		setupDefaultCameras( env, from, to, up, 0.1f, 200.0f );
	}


	//
	// Create the world
	//

	{
		hkpWorldCinfo info;

		info.m_gravity.set(0.0f, -9.8f, 0.0f);
		info.m_enableDeactivation = false;

			//
			//	Use a hard solver for higher stacks
			//
		{
			info.setupSolverInfo(hkpWorldCinfo::SOLVER_TYPE_4ITERS_MEDIUM);
			if ( variant.m_numRows > 15)
			{
				info.setupSolverInfo(hkpWorldCinfo::SOLVER_TYPE_4ITERS_HARD);
			}
			else if ( variant.m_numRows > 25)
			{
				info.setupSolverInfo(hkpWorldCinfo::SOLVER_TYPE_4ITERS_HARD);
			}
		}

			//
			//	Use the faster gsk for bigger boxes
			//
		{
			if( variant.m_boxSize >= 0.1f )
			{
				info.m_contactPointGeneration = info.CONTACT_POINT_REJECT_MANY;
			}
			else
			{
				info.m_contactPointGeneration = info.CONTACT_POINT_ACCEPT_ALWAYS;
			}
		}
		info.m_collisionTolerance = 0.007f;
		info.setBroadPhaseWorldSize( 100.0f );

		if ( variant.m_usePredictive )
		{
			info.m_simulationType = hkpWorldCinfo::SIMULATION_TYPE_CONTINUOUS;
		}
		else
		{
			info.m_simulationType = hkpWorldCinfo::SIMULATION_TYPE_DISCRETE;
		}

		info.m_contactRestingVelocity = 0.5f;
		m_world = new hkpWorld( info );
		m_world->lock();

		// Register ALL agents (though some may not be necessary)
		hkpAgentRegisterUtil::registerAllAgents(m_world->getCollisionDispatcher());

		// override box box if we want to use GSK
		if ( variant.m_useGsk )
		{
			// reroute the box box to use gsk
			hkpCollisionDispatcher* dis = m_world->getCollisionDispatcher();
			dis->m_agent3TypesPred[ HK_SHAPE_BOX ][ HK_SHAPE_BOX ] = dis->m_agent3TypesPred[ HK_SHAPE_CONVEX ][ HK_SHAPE_CONVEX ];
			dis->m_agent3Types    [ HK_SHAPE_BOX ][ HK_SHAPE_BOX ] = dis->m_agent3Types    [ HK_SHAPE_CONVEX ][ HK_SHAPE_CONVEX ];
		}

		setupGraphics();
	}


	//
	// Create the base
	//

	///
	{
		hkVector4 baseSize( 15.0f, 0.5f, 15.0f);
		hkpConvexShape* shape = new hkpBoxShape( baseSize , 0 );

		hkpRigidBodyCinfo ci;

		ci.m_shape = shape;
		ci.m_restitution = 0.5f;
		ci.m_friction = 0.3f;
		ci.m_position.set( 0.0f, -0.5f, 0.0f );
		ci.m_motionType = hkpMotion::MOTION_FIXED;

		m_world->addEntity( new hkpRigidBody( ci ) )->removeReference();
		shape->removeReference();
	}	
	///>

	/// The boxes that make up the pyramid are created in a similar manner, only we give them mass properties,
	/// a hkpMotion::MOTION_BOX_INERTIA as their motion type and calculate an inertia tensor for them:

	//
	//	Some values 
	//

	const hkReal boxDim    = variant.m_boxSize;	// This is the size of the cube side of the box
	const hkReal boxRadius = boxDim * 0.01f;
	const hkReal gapx    = boxDim * 0.05f;		// This is the gap betwen boxes 
	const hkReal gapy    = boxRadius;
	const hkReal heightOffGround = 0.0f;	// This is the height of the pyramid off the gound

	int    pyramidSize = variant.m_numRows;	// This is the number of boxes along the base (also vertically)
	const hkReal sphereRadius = 0.35f;

	hkReal extendedBoxDimX = boxDim + gapx;
	hkReal extendedBoxDimY = boxDim + gapy;
	hkVector4 startPos( 0.0f , heightOffGround + gapy + boxDim * 0.5f, 0.0f);
	
	// Build pyramid
	{
		hkVector4 boxRadii(boxDim *.5f, boxDim *.5f, boxDim *.5f);

		hkpShape* boxShape = new hkpBoxShape( boxRadii , boxRadius ); 

		for(int i=0; i<pyramidSize; i++)
		{
				// This constructs a row, from left to right
			int rowSize = pyramidSize - i;
			hkVector4 start(-rowSize * extendedBoxDimX * 0.5f + extendedBoxDimX * 0.5f, i * extendedBoxDimY, 0);
			for(int j=0; j< rowSize; j++)
			{
				hkVector4 boxPos(start);
				hkVector4 shift(j * extendedBoxDimX, 0.0f, 0.0f);
				boxPos.setAdd4(boxPos, shift);
				boxPos.setAdd4(boxPos, startPos);

				///
				hkpRigidBodyCinfo boxInfo;

				boxInfo.m_mass = 100.0f;
				// calculate the correct inertia
				hkReal d = boxInfo.m_mass * boxDim * boxDim / 6.0f;

				// for small boxes increase inertia slightly
				if ( boxRadius < 0.1f )
				{
					d *= 2.0f;
					if ( boxRadius < 0.03f )
					{
						d *= 2.0f;
					}
				}
				boxInfo.m_inertiaTensor.setDiagonal(d,d,d);

				boxInfo.m_shape = boxShape;
				boxInfo.m_motionType = hkpMotion::MOTION_DYNAMIC;
				boxInfo.m_position = boxPos;
				boxInfo.m_qualityType = variant.m_qualityType; 
				boxInfo.m_restitution = 0.5f;
				boxInfo.m_friction = 0.6f;
				boxInfo.m_allowedPenetrationDepth = variant.m_allowedPenetration;
				boxInfo.m_solverDeactivation = hkpRigidBodyCinfo::SOLVER_DEACTIVATION_MAX;
				///>

				hkpRigidBody* boxRigidBody = new hkpRigidBody(boxInfo);

				// Now add to world. Body is "ready to go" as soon as this is called, and display
				// is (as a registered listener) automatically notified to build a new display object.
				m_world->addEntity( boxRigidBody );
				boxRigidBody->removeReference();	// Remove reference, since we no longer want to remember this
			}
		}
		boxShape->removeReference();
	}
	
	/// And finally we create our marble - which is simply a sphere. 

	//
	// Create the sphere/marble/character
	//

	///
	hkpRigidBody* sphereRigidBody;
	{
		hkpShape *sphereShape = new hkpSphereShape( sphereRadius );
		hkpRigidBodyCinfo si;

		si.m_motionType = hkpMotion::MOTION_SPHERE_INERTIA;
		si.m_position = hkVector4(0.0f,3.0f,5.0f);
		si.m_mass = 300.0f;
		si.m_shape = sphereShape;

		hkpMassProperties massProperties;
		hkpInertiaTensorComputer::computeSphereVolumeMassProperties(sphereRadius, si.m_mass, massProperties);

		si.m_inertiaTensor = massProperties.m_inertiaTensor;

		sphereRigidBody = new hkpRigidBody(si);

		m_world->addEntity( sphereRigidBody )->removeReference();	// Remove reference, since we no longer want to remember this
		sphereShape->removeReference();
	}
	///>

	/// We add all of these to the world and let the havok begin!

	///[MarbleAction]
	/// Since the marble action is central to this demo a slight detour into its workings is in order.
	///
	/// The MarbleAction (derived from hkpUnaryAction) applies impulses to a sphere to roll it around the scene.
	/// When the corresponding keys are pressed, the internal state of the marble is set to 'push' or 'rotate'.
	/// Since the marble is a sphere it will roll (due to the friction) when in contact with the ground.
	/// When 'rotated', i.e. turned left or right, the body itself is not actually rotated, only the 'forward' direction.
	///
	/// The set'X'Pressed() accessors allow you to change the internal state. The body maintains its state
	/// internally, so it is not necessary, for example, to continuously tell the action to go forward once
	/// setForwardPressed(true) is called. It will go forward until setForwardPressed(false) is called.
	///
	/// You can also make the body 'jump' by applying an upward impulse, and 'brake' by killing all velocity
	///
	/// The create a new marble action we simply give a rigid body, an initial direction and a position as follows:

	//
	// Create the "marble" action to drive him around
	//

	///
	hkVector4 forward(1.0f,0.0f,0.0f);
	m_marbleaction = new MarbleAction(sphereRigidBody, forward, sphereRigidBody->getPosition());
	m_world->addAction(m_marbleaction);
	///>


	m_world->unlock();
}


hkDemo::Result PyramidDemo::stepDemo()
{
	const hkgPad* pad = m_env->m_gamePad;

	// Check if action is still in the world : It should always be, but it's better to check here to make sure
	if(m_marbleaction->getWorld() != HK_NULL)
	{
		m_world->lock();
		///[StepGame]
		/// To step the game we check to see if the user has pressed one of the control keys:

		///
		if ( pad->wasButtonPressed(0xffffffff) )
		{
			m_marbleaction->getEntity()->activate();
		}
		///>

		/// and if they have we call activate() to ensure the rigid body is active prior to applying any impulses.
		/// If we did not take this precaution the body would not exhibit the effects of the impulse until it reactivated
		/// at a later time. Once we have we've done this we proceed with identifying which button was pressed. Given
		/// below is the check for a 'left' key press:

		///
		m_marbleaction->setLeftPressed((pad->getButtonState() & HKG_PAD_DPAD_LEFT) != 0);
		///>	
		m_marbleaction->setRightPressed((pad->getButtonState() & HKG_PAD_DPAD_RIGHT) != 0);
		m_marbleaction->setForwardPressed((pad->getButtonState() & HKG_PAD_DPAD_UP) != 0);
		m_marbleaction->setBackwardPressed((pad->getButtonState() & HKG_PAD_DPAD_DOWN) != 0);

		m_marbleaction->setJumpPressed(pad->wasButtonPressed(HKG_PAD_BUTTON_0));
		m_marbleaction->setBrakePressed((pad->getButtonState() & HKG_PAD_BUTTON_1) != 0);	

		/// As you can see if the 'left' key has not been pressed then the method will be passed FALSE as a parameter. 
		/// Continuing in this manner only the actual direction key pressed will receive a TRUE parameter and all others will
		/// be reset.

		if (pad->wasButtonPressed(HKG_PAD_BUTTON_3))
		{
			m_marbleaction->reset();
		}
		m_world->unlock();
	}
	return hkDefaultPhysicsDemo::stepDemo();
}

PyramidDemo::~PyramidDemo()
{
	hkDeallocate( m_stackBuffer );
	hkThreadMemory::getInstance().setStackArea( m_oldStack.m_current, m_oldStack.getFreeBytes() );
	m_marbleaction->removeReference();
}


#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
"A small game which creates a pyramid of boxes and a rolling marble which can interact with them. " \
"This demo shows the quality and speed of different collision detection types applied to different sizes of objects.";

HK_DECLARE_DEMO_VARIANT_USING_STRUCT( PyramidDemo, HK_DEMO_TYPE_PRIME, PyramidVariant, g_variants, helpString ); 

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
