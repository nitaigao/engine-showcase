/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/Physics/Api/Vehicle/VehicleCloning/VehicleCloningDemo.h>
#include <Physics/Dynamics/World/hkpPhysicsSystem.h>

#include <Demos/Physics/Api/Vehicle/VehicleApi/VehicleApiUtils.h>
#include <Demos/DemoCommon/Utilities/GameUtils/Landscape/LandscapeRepository.h>

#include <Demos/DemoCommon/Utilities/GameUtils/Landscape/FlatLand.h>

#include <Graphics/Bridge/DisplayHandler/hkgDisplayHandler.h>

#include <Demos/DemoCommon/Utilities/GameUtils/Landscape/LandscapeRepository.h>

struct VehicleCloningVariant
{
	const char*	 m_name;
	int    m_numVehicles;
	bool m_isStatsDemo;
	const char* m_details;
};

static const VehicleCloningVariant g_variants[] =
{
	{ "4 Vehicles on 4800-Triangle Landscape", 4, false, "Create an original vehicle and clone it, sharing components between the clones." },
	{ "20 Vehicles on 4800-Triangle Landscape", 20, true, "A demo variant used for automatic statistics collection" }
};


VehicleCloning::VehicleCloning(hkDemoEnvironment* env, hkBool createWorld)
:	hkDefaultPhysicsDemo(env)
{
	m_bootstrapIterations = 200;

	m_track = HK_NULL;
	m_numWheels = 4;
	
	m_landscapeContainer = HK_NULL;
	setUpWorld();

	if (!createWorld)
	{
		return;
	}

	m_world->lock();

	const VehicleCloningVariant& variant =  g_variants[m_variantId];

	m_vehicles.setSize( variant.m_numVehicles );

	// Create a vehicle to use as a template for cloning other vehicles
	hkpPhysicsSystem* vehicleSystem = createVehicle();


	// Clone the vehicle and add the clones to the world

	hkArray<hkAabb> spawnVolumes;
	spawnVolumes.pushBack(hkAabb(hkVector4(-95, 2, -95), hkVector4(95, 8, 95)));

	AabbSpawnUtil spawnUtil( spawnVolumes );

	for ( int i = 0; i < variant.m_numVehicles; ++i )
	{
		hkpPhysicsSystem* newVehicleSystem = vehicleSystem->clone();

		hkVector4 objectSize( 4, 4, 4 );
		hkVector4 position; 
		spawnUtil.getNewSpawnPosition( objectSize, position );

		newVehicleSystem->getRigidBodies()[0]->setPosition(position);
		m_vehicles[i] = static_cast<hkpVehicleInstance*>(newVehicleSystem->getActions()[0]);
		m_vehicles[i]->addReference();
		m_world->addPhysicsSystem( newVehicleSystem );
		newVehicleSystem->removeReference();
	}

	createWheels(variant.m_numVehicles);

	vehicleSystem->removeReference();

	m_world->unlock();
}


hkpPhysicsSystem* VehicleCloning::createVehicle()
{
	//
	// Create vehicle's chassis shape.
	//
	hkpConvexVerticesShape* chassisShape = VehicleApiUtils::createCarChassisShape();

	//
	// Create the chassis body.
	//
	hkpRigidBody* chassisRigidBody;
	{
		hkpRigidBodyCinfo chassisInfo;

		// The mass value is reset by the vehicle SDK.
		chassisInfo.m_mass = 750.0f;	
		chassisInfo.m_shape = chassisShape;
		chassisInfo.m_friction = 0.8f;		
		chassisInfo.m_motionType = hkpMotion::MOTION_BOX_INERTIA;

		// Position chassis on the ground.

		// mass and inertia tensor will be set by VehicleSetup
		chassisInfo.m_position.set(0.0f, -3.0f, -10.0f);
		chassisInfo.m_inertiaTensor.setDiagonal(1.0f, 1.0f, 1.0f);

		chassisInfo.m_centerOfMass.set( -0.037f, 0.143f, 0.0f);

		int chassisLayer = 1;
		chassisInfo.m_collisionFilterInfo = hkpGroupFilter::calcFilterInfo( chassisLayer, 0 );

		chassisRigidBody = new hkpRigidBody(chassisInfo);
	}

	//
	// Instantiate the vehicle components.
	//
	hkpVehicleInstance* vehicle = new hkpVehicleInstance(chassisRigidBody);
	{
		vehicle->m_data				= new hkpVehicleData;
		vehicle->m_aerodynamics		= new hkpVehicleDefaultAerodynamics;
		vehicle->m_brake			= new hkpVehicleDefaultBrake;
		vehicle->m_driverInput		= new hkpVehicleDefaultAnalogDriverInput;
		vehicle->m_engine			= new hkpVehicleDefaultEngine;
		vehicle->m_steering			= new hkpVehicleDefaultSteering;
		vehicle->m_suspension		= new hkpVehicleDefaultSuspension;
		vehicle->m_transmission		= new hkpVehicleDefaultTransmission;
		vehicle->m_velocityDamper	= new hkpVehicleDefaultVelocityDamper;	
		vehicle->m_wheelCollide		= new hkpVehicleRaycastWheelCollide;

		VehicleSetup setup;

		setup.setupVehicleData( m_world, *vehicle->m_data );

		// initialise the tyre marks controller with 128 tyre mark points.
		vehicle->m_tyreMarks = new hkpTyremarksInfo( *vehicle->m_data, 128 );

		setup.setupComponent( *vehicle->m_data, *static_cast< hkpVehicleDefaultAnalogDriverInput* >(vehicle->m_driverInput) );
		setup.setupComponent( *vehicle->m_data, *static_cast< hkpVehicleDefaultSteering*>(vehicle->m_steering));
		setup.setupComponent( *vehicle->m_data, *static_cast< hkpVehicleDefaultEngine*>(vehicle->m_engine) );
		setup.setupComponent( *vehicle->m_data, *static_cast< hkpVehicleDefaultTransmission*>(vehicle->m_transmission) );
		setup.setupComponent( *vehicle->m_data, *static_cast< hkpVehicleDefaultBrake*>(vehicle->m_brake) );
		setup.setupComponent( *vehicle->m_data, *static_cast< hkpVehicleDefaultSuspension*>(vehicle->m_suspension) );
		setup.setupComponent( *vehicle->m_data, *static_cast< hkpVehicleDefaultAerodynamics*>(vehicle->m_aerodynamics) );
		setup.setupComponent( *vehicle->m_data, *static_cast< hkpVehicleDefaultVelocityDamper*>(vehicle->m_velocityDamper) );

		// The wheel collide component performs collision detection. To do this, it needs to create an 
		// aabbPhantom from the vehicle information that has been set here already.
		setup.setupWheelCollide( m_world, *vehicle, *static_cast< hkpVehicleRaycastWheelCollide*>(vehicle->m_wheelCollide) );

		setup.setupTyremarks( *vehicle->m_data, *static_cast< hkpTyremarksInfo*>(vehicle->m_tyreMarks) );
	}

	// Give the driver input default values so that the vehicle will drive, even if it is in circles!
	{
		vehicle->m_deviceStatus = new hkpVehicleDriverInputAnalogStatus;
		hkpVehicleDriverInputAnalogStatus* deviceStatus = (hkpVehicleDriverInputAnalogStatus*)vehicle->m_deviceStatus;

		// Velocity.
		deviceStatus->m_positionY = -0.0f;

		// Turning.
		deviceStatus->m_positionX = -0.0f;

		// Defaults.
		deviceStatus->m_handbrakeButtonPressed = false;
		deviceStatus->m_reverseButtonPressed = false;
	}

	vehicle->init();

	// Create a physics system and add the action, rigid body and phantom that comprise the vehicle to it.
	hkpPhysicsSystem* vehicleSystem = new hkpPhysicsSystem();
	vehicleSystem->addAction( vehicle );
	vehicleSystem->addRigidBody( chassisRigidBody );
	vehicleSystem->addPhantom( (hkpPhantom*) static_cast<hkpVehicleRaycastWheelCollide*>(vehicle->m_wheelCollide)->m_phantom );

	chassisRigidBody->removeReference();
	chassisShape->removeReference();
	vehicle->removeReference();

	return vehicleSystem;
}


void VehicleCloning::createWheels( int numVehicles )
{
	//
	// Create the wheel display geometries. Note: we use a Havok hkDisplayGeometry here, to
	// make binding to the Havok demo framework and Visual Debugger graphics engine easier.
	// However you can use whatever you want!
	// The id for the wheel displayGeometry is an odd number. This prevents it conflicting
	// with hkpCollidable pointers which are used as ids for rigid bodies by convention in 
	// the Visual Debugger.
	//
	int displayId = 1;
	m_displayWheelId.setSize( numVehicles );

	for (int vehicleId = 0; vehicleId < numVehicles; vehicleId++)
	{
		m_displayWheelId[vehicleId].setSize( m_numWheels );

		// Create a display wheel for each wheel on the vehicle.
		for(int i = 0; i < m_numWheels; i++)
		{
			VehicleApiUtils::createDisplayWheels(m_env, m_tag, displayId, 0.4f, 0.2f);

			m_displayWheelId[vehicleId][i] = displayId;

			displayId += 2;
		}
	}
}

VehicleCloning::~VehicleCloning( )
{
	// Remove the references maintained for displaying the wheels.
	for (int vehicleId = 0; vehicleId < m_vehicles.getSize(); vehicleId++ )
	{
		// Check size of each element in case not all vehicles have the same number of wheels.
		for (int wheelNum = 0; wheelNum < m_displayWheelId[vehicleId].getSize(); wheelNum++)
		{
			m_env->m_displayHandler->removeGeometry(m_displayWheelId[vehicleId][wheelNum], m_tag, 0);
		}

		m_vehicles[vehicleId]->removeReference();
	}

	// Release the underlying geometry data.
	// Note: normally this data is owned by the graphics system (not in our case though).
	if (m_track)
	{
		m_track->removeReference();
	}
	if (m_landscapeContainer)
	{
		m_landscapeContainer->removeReference();
	}
}

// This is called every simulation time step. We need to
// - Step the simulation.
// - Sync each vehicle's display wheels.
// - Draw skid marks for the first vehicle if it is skidding.
hkDemo::Result VehicleCloning::stepDemo()
{
	//
	// Step the world.
	//

	{
		hkDefaultPhysicsDemo::stepDemo();
	}

	//
	// Synch the display wheels.
	//
	for (int vehicleId = 0; vehicleId < m_vehicles.getSize(); vehicleId++ )
	{
		if (m_vehicles[vehicleId]->getChassis()->getPosition()(1) < -20 )
		{
			m_vehicles[vehicleId]->getChassis()->setPosition(hkVector4(0,4,0));
		}		

		VehicleApiUtils::syncDisplayWheels(m_env, 
			*m_vehicles[vehicleId],
			m_displayWheelId[vehicleId], m_tag);
	}

	

	// Update the tyre marks for the first vehicle.
	VehicleDisplayUtils::updateTyremarks( m_timestep, m_vehicles[0] );

	return DEMO_OK;
}

void VehicleCloning::buildLandscape()
{
	//
	// Create the ground we will drive on.
	//
	{
		hkpRigidBodyCinfo groundInfo;

		FlatLand* fl = new FlatLand();
		m_track = fl;
		groundInfo.m_shape = fl->createMoppShape();
		groundInfo.m_position.set(5.0f, -2.0f, 5.0f);

		groundInfo.m_motionType = hkpMotion::MOTION_FIXED;
		groundInfo.m_friction = 2.0f;
		hkpRigidBody* groundbody = new hkpRigidBody(groundInfo);
		m_world->addEntity(groundbody);
		groundbody->removeReference();

		groundInfo.m_shape->removeReference();
	}
}

void VehicleCloning::setUpWorld()
{
	m_tag = 0;

	//
	// Setup the camera. 
	//
	{
		hkVector4 from(0.0f, 10.0f, 20.0f);
		hkVector4 to(0.0f, 0.0f, -30.0f);
		hkVector4 up(0.0f, 1.0f, 0.0f);
		setupDefaultCameras( m_env, from, to, up );
	}

	//
	// Create the world.
	//
	{
		hkpWorldCinfo info;
		info.m_collisionTolerance = 0.1f;
		info.m_gravity.set(0.0f, -9.8f, 0.0f);
		info.setBroadPhaseWorldSize(850.0f) ;
		
		info.setupSolverInfo(hkpWorldCinfo::SOLVER_TYPE_4ITERS_MEDIUM);
		m_world = new hkpWorld( info );
		m_world->lock();

		// Register all agents.
		hkpAgentRegisterUtil::registerAllAgents(m_world->getCollisionDispatcher());
		// Graphics.
		setupGraphics();
	}

	// Build the landscape to drive on and add it to m_world.
	buildLandscape();
	m_world->unlock();
}

#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

HK_DECLARE_DEMO_VARIANT(VehicleCloning, HK_DEMO_TYPE_PHYSICS                   , g_variants[0].m_name, 0, "", g_variants[0].m_details);
HK_DECLARE_DEMO_VARIANT(VehicleCloning, HK_DEMO_TYPE_PHYSICS|HK_DEMO_TYPE_STATS,  g_variants[1].m_name, 1, "", g_variants[1].m_details);

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
