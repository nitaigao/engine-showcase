/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


#include <Demos/demos.h>

#include <Demos/Physics/Api/Vehicle/SerializedVehicle/SerializedVehicleDemo.h>

#include <Graphics/Bridge/DisplayHandler/hkgDisplayHandler.h>

// 
// Includes for default vehicle implementation (including camera).
// 
#include <Physics/Vehicle/hkpVehicleInstance.h>
#include <Physics/Vehicle/WheelCollide/RayCast/hkpVehicleRaycastWheelCollide.h>
#include <Physics/Vehicle/DriverInput/Default/hkpVehicleDefaultAnalogDriverInput.h>

#include <Physics/Vehicle/Camera/hkp1dAngularFollowCam.h>

//
// Include for camera setup and user input handling.
//
#include <Demos/Physics/Api/Vehicle/VehicleApi/VehicleApiUtils.h>


//
// Includes for serialization.
//
#include <Physics/Dynamics/World/hkpPhysicsSystem.h>

#include <Physics/Utilities/Serialize/hkpPhysicsData.h>
#include <Common/Base/System/Io/IStream/hkIStream.h>

#include <Physics/Utilities/Serialize/hkpHavokSnapshot.h>

#include <Common/Base/Reflection/Registry/hkTypeInfoRegistry.h>
#include <Common/Base/Reflection/Registry/hkVtableClassRegistry.h>

#include <Common/Serialize/Packfile/Binary/hkBinaryPackfileWriter.h>
#include <Common/Serialize/Packfile/Xml/hkXmlPackfileWriter.h>
#include <Common/Serialize/Util/hkRootLevelContainer.h>



SerializedVehicle::SerializedVehicle(hkDemoEnvironment* env )
	:	hkDefaultPhysicsDemo(env),
		m_loadedDataVehicle(HK_NULL),
		m_loadedDataLandscape(HK_NULL),
		m_vehicleSystem(HK_NULL)
{

	// Disable warnings:
	hkError::getInstance().setEnabled(0xafe97523, false); //'This utility is intended primarily for Havok demo use. If you wish to step the world asynchronously, you are encouraged to copy the code from this utility and integrate it into your game loop.'

	setUpWorld();

	m_world->lock();
	//
	// Load in vehicle from XML
	//
	{
		hkString path("Resources/Physics/Api/Vehicle/SerializedVehicle/vehicle.xml");

		loadVehicle( m_world, m_vehicleSystem, path, false );
		m_world->addPhysicsSystem( m_vehicleSystem );
	}

	//
	// The display wheels sould be created here.
	//
	VehicleApiUtils::createCamera( m_camera );

	m_world->unlock();
}

SerializedVehicle::~SerializedVehicle( )
{
	//Re-enable warnings
	hkError::getInstance().setEnabled(0xafe97523, true);

	m_world->markForWrite();
	m_world->removeReference();
	m_vehicleSystem->removeReference();
	m_loadedDataLandscape->removeReference();
	m_loadedDataVehicle->removeReference();

	// remove the wheel collision detection phantom from the world.
	//m_world->removePhantom( (hkpPhantom*)(static_cast< hkpVehicleRaycastWheelCollide*>(getVehicle()->m_wheelCollide)->m_phantom) );

	m_world = HK_NULL;
}


//////////////////////////////////////////////////////////////////////////

//
// Loads a landscape from the file specified by path and adds it to the world.
//
void SerializedVehicle::loadLandscape( hkpWorld* world, hkString path, bool binaryFormat )
{
	hkIstream infile( path.cString() );
	HK_ASSERT( 0x215d080c, infile.isOk() );

	hkpPhysicsData* physicsData = hkpHavokSnapshot::load(infile.getStreamReader(), &m_loadedDataLandscape);

	hkArray<hkpPhysicsSystem*> physicsSystems;
	physicsSystems = physicsData->getPhysicsSystems();
	HK_ASSERT( 0x0, physicsSystems.getSize() == 1 );

	hkArray< hkpRigidBody* > physicsRigidBodies;
	physicsRigidBodies = physicsSystems[0]->getRigidBodies();
	HK_ASSERT( 0x0, physicsRigidBodies.getSize() == 1 );

	m_landscape = static_cast< hkpRigidBody* >(physicsRigidBodies[0]);
	world->addEntity( m_landscape );
}


//
// The vehicle is an action, so we just need to get the actions from the saved physics system.
//

void SerializedVehicle::loadVehicle( hkpWorld* world, hkpPhysicsSystem* &vehicleSystem, hkString path, bool binaryFormat )
{
	// Load the hkpVehicleInstance action from the file given by path.
	hkIstream infile( path.cString() );
	HK_ASSERT( 0x215d080c, infile.isOk() );

	if (vehicleSystem)
		vehicleSystem->removeReference();
	if (m_loadedDataVehicle)
		m_loadedDataVehicle->removeReference();

	hkpPhysicsData* physicsData = hkpHavokSnapshot::load(infile.getStreamReader(), &m_loadedDataVehicle);

	hkArray<hkpPhysicsSystem*> physicsSystems;
	physicsSystems = physicsData->getPhysicsSystems();
	HK_ASSERT( 0x0, physicsSystems.getSize() == 1 );

	vehicleSystem = physicsSystems[0];
	vehicleSystem->addReference();
	physicsData->removeReference();
}

hkpVehicleInstance* SerializedVehicle::getVehicle()
{
	return static_cast<hkpVehicleInstance*>( m_vehicleSystem->getActions()[0] );
}


void SerializedVehicle::saveVehicle( hkpPhysicsSystem* vehicleSystem, hkString path, bool binaryFormat )
{
	hkOstream outfile( path.cString() );

	//
	// Create a physics system containing the vehicle.
	//
	hkpPhysicsData data;
	data.addPhysicsSystem( vehicleSystem );

	// Save this physics system.
	hkpHavokSnapshot::save( &data, outfile.getStreamWriter(), false );
}


void SerializedVehicle::saveLandscape( hkpRigidBody* landscape, hkString path, bool binaryFormat )
{
	hkpPhysicsSystem landscapeSystem;
	landscapeSystem.addRigidBody( landscape );
	landscapeSystem.setActive(landscape->isActive());

	hkOstream outfile( path.cString() );

	//
	// Create a physics system containing the vehicle.
	//
	hkpPhysicsData data;
	data.addPhysicsSystem( &landscapeSystem );

	// Save this physics system.
	hkpHavokSnapshot::save( &data, outfile.getStreamWriter(), false );
}

hkDemo::Result SerializedVehicle::stepDemo()
{
	//
	// Steer the vehicle from user input.
	//

	VehicleApiUtils::steer( m_env->m_gamePad,
		m_inputXPosition,
		m_inputYPosition,
		*getVehicle(),
		m_timestep);

	//
	// Load and restore the vehicle.
	// Press 
	//   * button L1 ("delete" on PC) to save 
	//   * button R1 ("end" on PC) to reload
	// the vehicle state.
	//
	{
		const hkgPad* pad = m_env->m_gamePad;

		// Save the vehicle
		hkString rpath("Resources/Physics/Api/Vehicle/SerializedVehicle/");
		if ( pad->wasButtonPressed(HKG_PAD_BUTTON_L1) )
		{
			hkString path; 
			path = rpath + "savedVehicle.xml";

			m_world->lock();
			saveVehicle( m_vehicleSystem, path, false );
			m_world->unlock();
		}

		// Load the vehicle
		if ( pad->wasButtonPressed(HKG_PAD_BUTTON_R1) )
		{
			hkString path;
			path = rpath + "savedVehicle.xml";

			m_world->lock();
			m_world->removePhysicsSystem( m_vehicleSystem );
			loadVehicle( m_world, m_vehicleSystem, path, false );
			m_world->addPhysicsSystem( m_vehicleSystem );
			m_world->unlock();
		}
	}

	//
	// Step the world.
	//
	{
		hkDefaultPhysicsDemo::stepAsynchronously(m_world, m_timestep, m_timestep);
	}


	//
	// Synchronize the display wheels, update the tyremarks display, update the RPM and
	// gear display (not done here).
	//
	VehicleApiUtils::updateCamera( m_env, *getVehicle()->getChassis(), m_timestep, m_camera);

	return DEMO_OK;
}

void SerializedVehicle::setUpWorld()
{
	// Initially "controller" is at (0,0), ie. neither pointing left/right nor up/down.
	m_inputXPosition = 0.0f;
	m_inputYPosition = 0.0f;

	//
	// Setup the camera. Actually overwritten by step function, and when we first add the vehicle.
	//
	{
		hkVector4 from(0.0f, 0.0f, 10.0f);
		hkVector4 to(0.0f, 0.0f, 0.0f);
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


	//
	// Load the landscape to drive on and add it to m_world.
	//
	hkString path("Resources/Physics/Api/Vehicle/SerializedVehicle");
	path += "/landscape.xml";
	
	loadLandscape( m_world, path, false );

	m_world->unlock();
}


#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] =	\
"This is a bare-bones vehicle demo (it doesn't even have wheels!).  The vehicle is loaded from XML.\n" \
"To save the vehicle, press \232. To reload, press \233.\n" \
"Controls: " \
"\224/\225 - accelerate / brake " \
"\226/\227 - steer " \
"\221 - handbrake";
 
HK_DECLARE_DEMO(SerializedVehicle, HK_DEMO_TYPE_PHYSICS | HK_DEMO_TYPE_SERIALIZE, "Drive a serialized vehicle on a MOPP landscape", helpString);

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
