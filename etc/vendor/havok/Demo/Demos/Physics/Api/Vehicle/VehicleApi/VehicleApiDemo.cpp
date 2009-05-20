/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/Physics/Api/Vehicle/VehicleApi/VehicleApiDemo.h>
#include <Demos/Physics/Api/Vehicle/VehicleApi/VehicleSetup.h>
#include <Demos/Physics/Api/Vehicle/VehicleApi/VehicleApiUtils.h>


// Needed only to display the wheels.
#include <Graphics/Bridge/DisplayHandler/hkgDisplayHandler.h>


VehicleApiDemo::VehicleApiDemo(hkDemoEnvironment* env)
:	hkDefaultPhysicsDemo(env)
{
	m_tag = 0;

	///[driverInput]
	/// Initially controller is set to 0,0 i.e. neither turning left/right or forward/backward,
	/// so vehicle is not accelerating or turning.	///
	m_inputXPosition = 0.0f;
	m_inputYPosition = 0.0f;
	///>

	{
		hkVector4 from(0.0f, 0.0f, 10.0f);
		hkVector4 to(0.0f, 0.0f, 0.0f);
		hkVector4 up(0.0f, 1.0f, 0.0f);
		setupDefaultCameras( env, from, to, up );
	}

	//
	// Create the world.
	//
	{
		hkpWorldCinfo info;
		info.m_collisionTolerance = 0.1f;
		info.m_gravity.set(0.0f, -9.8f, 0.0f);
		info.setBroadPhaseWorldSize(1000.0f) ;
		info.setupSolverInfo(hkpWorldCinfo::SOLVER_TYPE_4ITERS_MEDIUM);
		m_world = new hkpWorld( info );
		m_world->lock();

		// Register all agents.
		hkpAgentRegisterUtil::registerAllAgents(m_world->getCollisionDispatcher());

		// Graphics.
		setupGraphics();

	}

	///[buildLandscape]
	/// Build the landscape to drive on and add it to m_world. The landscape is simply five
	/// boxes, one for the ground and four for the walls.
	///
	buildLandscape();
	///>

	///
	/// Create the chassis
	///
	hkpConvexVerticesShape* chassisShape = VehicleApiUtils::createCarChassisShape(); 
	hkpRigidBody* chassisRigidBody;
	{
		hkpRigidBodyCinfo chassisInfo;

		// NB: The inertia value is reset by the vehicle SDK.  However we give it a
		// reasonable value so that the hkpRigidBody does not assert on construction. See
		// VehicleSetup for the yaw, pitch and roll values used by hkVehicle.
		chassisInfo.m_mass = 750.0f;	
		chassisInfo.m_shape = chassisShape;
		chassisInfo.m_friction = 0.4f;

		// The chassis MUST have m_motionType hkpMotion::MOTION_BOX_INERTIA to correctly simulate
		// vehicle roll, pitch and yaw.
		chassisInfo.m_motionType = hkpMotion::MOTION_BOX_INERTIA;
		chassisInfo.m_position.set(0.0f, 1.0f, 0.0f);
		hkpInertiaTensorComputer::setShapeVolumeMassProperties(chassisInfo.m_shape,
										chassisInfo.m_mass,
										chassisInfo);

		chassisRigidBody = new hkpRigidBody(chassisInfo);

		// No longer need reference to shape as the hkpRigidBody holds one.
		chassisShape->removeReference();

		m_world->addEntity(chassisRigidBody);
	}
	///>
	/// In this example, the chassis is added to the Vehicle Kit in the createVehicle() method.


	///
	createVehicle( chassisRigidBody );
	chassisRigidBody->removeReference();
	///>

	///[buildVehicleCamera]
	/// This camera follows the vehicle when it moves.
	///
	{
		VehicleApiUtils::createCamera( m_camera );
	}
	///>

	createDisplayWheels();

	m_world->unlock();
}

VehicleApiDemo::~VehicleApiDemo( )
{
	m_world->lock();
	//
	// Remove the wheel display geometries.
	//
	for (int i = 0; i < m_vehicle->m_data->m_numWheels; i++)
	{
		m_env->m_displayHandler->removeGeometry(m_displayWheelId[i], m_tag, 0);
	}


	// Remove the vehicle phantom from the world.	
	m_world->removePhantom( (hkpPhantom*)(static_cast< hkpVehicleRaycastWheelCollide*>(m_vehicle->m_wheelCollide)->m_phantom) );

	m_vehicle->removeReference();
	m_world->unlock();
}

///[stepGame]
/// - Steer the vehicle.
/// - Step the world.
/// - Sync the display wheels.
/// - Move the "follow" camera.
///
hkDemo::Result VehicleApiDemo::stepDemo()
{
	{
		m_world->lock();

		//
		// Activate chassis if any of the driving/reorientation keys have been pressed
		//
		if(m_env->m_gamePad->isButtonPressed(HKG_PAD_BUTTON_0)
			|| m_env->m_gamePad->isButtonPressed(HKG_PAD_BUTTON_2)
			|| m_env->m_gamePad->isButtonPressed(HKG_PAD_DPAD_UP)
			|| m_env->m_gamePad->isButtonPressed(HKG_PAD_DPAD_DOWN)
			|| m_env->m_gamePad->isButtonPressed(HKG_PAD_DPAD_RIGHT)
			|| m_env->m_gamePad->isButtonPressed(HKG_PAD_DPAD_LEFT))
		{
		m_vehicle->getChassis()->activate();
		}

		//
		// Steer the vehicle from user input in each simulation step.
		//
		VehicleApiUtils::steer(m_env->m_gamePad,
			m_inputXPosition,
			m_inputYPosition,
			*m_vehicle,
			m_timestep);

		m_world->unlock();
	}

	//
	// Step the world.
	//
	{
		hkDefaultPhysicsDemo::stepDemo();
	}

	{
		m_world->lock();

		//
		// Sync wheels. This is for display only.
		//
		VehicleApiUtils::syncDisplayWheels(m_env, *m_vehicle, m_displayWheelId, m_tag);

		//
		// Update the "follow" camera.
		//
		VehicleApiUtils::updateCamera(m_env, *m_vehicle->getChassis(), m_timestep, m_camera);
		m_world->unlock();
	}

	return DEMO_OK;
}
///>

/// The implementation of steer() is as follows:
///[INSERT_CODE steerCode]

/// The implementation of updateCamera() is as follows:
///[INSERT_CODE updateCameraCode]

void VehicleApiDemo::buildLandscape()
{
	//
	// Create the landscape to drive on. Just a flat box bounded by 4 walls (also boxes).
	//
	{
		const hkReal sideLength = 100.0f;
		const hkReal depth = 1.0f;
		const hkReal width = 3.0f;
		const hkReal height = 5.0f;

		// Size of the boxes.
		hkVector4 halfExtentsGround = hkVector4(sideLength, depth, sideLength);
		hkVector4 halfExtents1      = hkVector4(width, height, sideLength);
		hkVector4 halfExtents2      = hkVector4(sideLength, height, width);
		


		hkpRigidBodyCinfo groundInfo;
		groundInfo.m_position.set(5.0f, -2.0f, 5.0f);
		groundInfo.m_motionType = hkpMotion::MOTION_FIXED;
		groundInfo.m_friction = 0.5f;

		// ground
		{
			groundInfo.m_shape = new hkpBoxShape(halfExtentsGround, 0 );

			groundInfo.m_position.set( 0, -2, 0 );
			m_world->addEntity( new hkpRigidBody(groundInfo) )->removeReference();

			groundInfo.m_shape->removeReference();
		}

		// wall12
		{
			groundInfo.m_shape = new hkpBoxShape(halfExtents1, 0 );

			groundInfo.m_position.set( sideLength+width, height - 2.0f, 0.0f);
			m_world->addEntity( new hkpRigidBody(groundInfo) )->removeReference();
			groundInfo.m_position.set( -sideLength-width, height - 2.0f, 0.0f);
			m_world->addEntity( new hkpRigidBody(groundInfo) )->removeReference();

			groundInfo.m_shape->removeReference();
		}
		// wall34
		{
			groundInfo.m_shape = new hkpBoxShape(halfExtents2, 0 );

			groundInfo.m_position.set( 0.0f, height - 2.0f, -(sideLength+width) );
			m_world->addEntity( new hkpRigidBody(groundInfo) )->removeReference();
			groundInfo.m_position.set( 0.0f, height - 2.0f, +(sideLength+width) );
			m_world->addEntity( new hkpRigidBody(groundInfo) )->removeReference();

			groundInfo.m_shape->removeReference();
		}

	}
}

void VehicleApiDemo::createVehicle(hkpRigidBody* chassis)
{

	// Create the basic vehicle.
	m_vehicle = new hkpVehicleInstance( chassis );
	VehicleSetup setup;
	setup.buildVehicle( m_world, *m_vehicle );


	///[integrationWithSDK]
	/// Actions are the interface between user controllable behavior of the physical simulation and the Havok core. 
	/// You can easily integrate the Vehicle Kit with the Havok physical simulation using the hkpVehicleInstance action, 
	/// which establishes the connection between the two SDKs. 
	///
	///	To simulate a vehicle, you first need to create a hkpVehicleInstance instance in your game. 
	/// You then add it to the actions of your core physical simulation, just like any other user action:
	///

	m_world->addAction(m_vehicle);
	///>
	/// Once you have added the action to the simulation, no extra work is required to simulate the vehicle. 
	/// On each call to step the core physical simulation, the vehicle action will be updated automatically.

}

///[createDisplayWheels]
/// Adding wheels to your vehicle is another area where you need to be careful when using the Vehicle Kit.
/// The default wheel collide vehicle component, hkpVehicleRaycastWheelCollide, calculates its wheel positions 
/// using raycasting - the wheels themselves are not physically simulated by the core physical simulation.
///
/// Here, the car's wheels are first created as rigid bodies using the demo's createDisc() helper function, 
/// but are not actually added to the simulation. Instead, the rigid bodies are used to create display geometries. 
///
void VehicleApiDemo::createDisplayWheels(hkReal radius, hkReal thickness)
{
	//
	// Create the wheel display geometries. Note: we use a Havok hkDisplayGeometry here, to
	// make binding to the Havok demo framework and Visual Debugger graphics engine easier.
	// However you can use whatever you want!
	//
	m_displayWheelId.setSize( m_vehicle->m_data->m_numWheels );

	// Create a display wheel for each wheel on the vehicle.
	for(int i = 0; i < m_displayWheelId.getSize(); i++)
	{
		// The id for the wheel displayGeometry is an odd number. This prevents it conflicting
		// with hkpCollidable pointers which are used as ids for rigid bodies by convention in 
		// the Visual Debugger.
		int displayId = (i * 2) + 1;

		VehicleApiUtils::createDisplayWheels(m_env, m_tag, displayId);

		m_displayWheelId[i] = displayId;
	}
}
///>
/// Each of these wheel shapes can then be displayed in the appropriate place as the car moves around, using information from the car's WheelsComponent.


///[epilogue]
/// Once you have explored the VehicleApiDemo demo, try changing the behavior of the vehicle. 
/// You do this by altering the vehicle component blueprint values. When you have done this, you can observe the resulting behavior of the simulated vehicle. 
/// For example, try to create a sports car or a monster truck! 
/// Make sure not to change too many things at once, as this will make it harder to see which change had what influence on the vehicle's behavior.
///
/// One interesting change to try is to lower the center of mass of the chassis rigid body. 
/// This has a big affect on the drivability of the car, as it is less likely to roll over on sharp corners with a lower center of mass. 
/// You can do this easily by calling the hkpRigidBody::setCenterOfMassLocal() method. 
/// Be careful, however, not to move the center of mass outside the rigid body shape, as this will result in very unrealistic physical behavior. 
/// Also, as the suspension hardpoints are specified relative to the center of mass of the chassis rigid body, you will have to move these in the 
/// opposite direction to your center of mass shift to put them back into the correct position after the shift has been applied.




#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
"This game shows how to use the Vehicle API to construct a basic vehicle with  " \
"(untuned) parameters, including an hkpAction used to update the vehicle, and a simple " \
"controller to drive it. The constructor creates a vehicle and a box landscape to drive on. " \
"stepDemo() updates the vehicle based on user input (steering, acceleration  etc.), updates the " \
"camera to follow the vehicle and synchronizes the vehicles display wheels. " \
"More complex examples can be found in the gamelements/vehicle folder. This demo " \
"is deliberately simplified to illustrate the basic concepts of building a vehicle " \
"in Havok. ";


HK_DECLARE_DEMO(VehicleApiDemo, HK_DEMO_TYPE_PHYSICS | HK_DEMO_TYPE_CRITICAL, "A vehicle on a box", helpString);

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
