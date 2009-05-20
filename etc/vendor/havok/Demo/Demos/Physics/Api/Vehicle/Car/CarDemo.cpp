/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

///[TUTORIAL 2]

#include <Demos/demos.h>


#include <Physics/Collide/Shape/HeightField/SampledHeightField/hkpSampledHeightFieldShape.h>

#include <Physics/Collide/Query/CastUtil/hkpWorldRayCastInput.h>
#include <Physics/Collide/Query/CastUtil/hkpWorldRayCastOutput.h>

#include <Common/Visualize/Shape/hkDisplayGeometry.h>

#include <Demos/Physics/Api/Vehicle/Car/CarDemo.h>
#include <Demos/Physics/Api/Vehicle/VehicleApi/VehicleSetup.h>
#include <Demos/Physics/Api/Vehicle/VehicleApi/VehicleApiUtils.h>

#include <Demos/DemoCommon/Utilities/GameUtils/Landscape/FlatLand.h>
#include <Demos/DemoCommon/Utilities/GameUtils/Landscape/RoundTrack.h>
#include <Demos/DemoCommon/Utilities/GameUtils/GameUtils.h>



#include <Physics/Dynamics/World/hkpPhysicsSystem.h>
#include <Physics/Vehicle/Camera/hkp1dAngularFollowCam.h>

#include <Graphics/Bridge/DisplayHandler/hkgDisplayHandler.h>
#include <Common/Visualize/Shape/hkDisplayGeometry.h>
#include <Graphics/Common/DisplayObject/hkgDisplayObject.h>
//#include <hkgraphics/hkgWindow.h>
#include <Graphics/Common/Material/hkgMaterial.h>

#include <Physics/Vehicle/hkpVehicleInstance.h>


#include <Graphics/Common/Window/hkgWindow.h>
#include <Physics/Utilities/Actions/Reorient/hkpReorientAction.h>
//#include <demos/gameelements/rigidbody/brickWall/BrickWall.h>

#include <Common/Visualize/hkDebugDisplay.h>
#include <Common/Base/Types/Color/hkColor.h>

// Need this to get user 'friction' property we will tag some bodies with.
#include <Demos/Physics/Api/Vehicle/VehicleApi/FrictionMapVehicleRaycastWheelCollide.h>



CarDemo::CarDemo(hkDemoEnvironment* env, hkBool createWorld, int numWheels, int numVehicles)
:	hkDefaultPhysicsDemo(env)
{
	m_bootstrapIterations = 150;

	m_numVehicles = numVehicles;
	m_numWheels = numWheels;

	m_vehicles.setSize( m_numVehicles );

	setUpWorld();

	// Setup a skybox
	//setupSkyBox(m_env);

	if (!createWorld)
	{
		return;
	}
	m_world->lock();
	VehicleSetup setup;
	{
		//
		// Create vehicle's chassis shape.
		//
		hkpConvexVerticesShape* chassisShape = VehicleApiUtils::createCarChassisShape(); 

		createDisplayWheels();

		int chassisLayer = 1;

		for (int vehicleId = 0; vehicleId < m_vehicles.getSize(); vehicleId++)
		{
			//
			// Create the vehicle.
			//
			{
				//
				// Create the chassis body.
				//
				hkpRigidBody* chassisRigidBody;
				{
					hkpRigidBodyCinfo chassisInfo;
					chassisInfo.m_mass = 750.0f;	
					chassisInfo.m_shape = chassisShape;
					chassisInfo.m_friction = 0.8f;
					chassisInfo.m_motionType = hkpMotion::MOTION_BOX_INERTIA;
					// Position chassis on the ground.

					// Inertia tensor will be set by VehicleSetup.
					chassisInfo.m_position.set(-40.0f, -4.5f, vehicleId * 5.0f);
					chassisInfo.m_inertiaTensor.setDiagonal(1.0f, 1.0f, 1.0f);
					
					chassisInfo.m_centerOfMass.set( -0.037f, 0.143f, 0.0f);
					chassisInfo.m_collisionFilterInfo = hkpGroupFilter::calcFilterInfo( chassisLayer, 0 );

					chassisRigidBody = new hkpRigidBody(chassisInfo);

					m_world->addEntity(chassisRigidBody, HK_ENTITY_ACTIVATION_DO_ACTIVATE );
				}

				// This hkpAction flips the car upright if it turns over. 	 
				if (vehicleId == 0) 	 
				{ 	 
					hkVector4 rotationAxis(1.0f, 0.0f, 0.0f); 	 
					hkVector4 upAxis(0.0f, 1.0f, 0.0f); 	 
					m_reorientAction = new hkpReorientAction(chassisRigidBody, rotationAxis, upAxis); 	 
				}

				m_vehicles[vehicleId].m_vehicle = createVehicle( setup, chassisRigidBody);
				m_vehicles[vehicleId].m_lastRPM = 0.0f;
 
				chassisRigidBody->removeReference();
			}
		}
		chassisShape->removeReference();
	} 

	//
	// Create the camera.
	//
	{
		VehicleApiUtils::createCamera( m_camera );
	}

	m_world->unlock();
}

CarDemo::~CarDemo( )
{
	m_world->lock();

	for (int vehicleId = 0; vehicleId < m_vehicles.getSize(); vehicleId++ )
	{
		// Check size of each element as not all vehicles have same number of wheels.
		for (int wheelNum = 0; wheelNum < m_displayWheelId[vehicleId].getSize(); wheelNum++)
		{
			m_env->m_displayHandler->removeGeometry(m_displayWheelId[vehicleId][wheelNum], m_tag, 0);
		}

		// remove the wheel collision detection phantom from the world.
		m_world->removePhantom( (hkpPhantom*)(static_cast< hkpVehicleRaycastWheelCollide*>(m_vehicles[vehicleId].m_vehicle->m_wheelCollide)->m_phantom) );
		m_vehicles[vehicleId].m_vehicle->removeReference();
	}

	// Release the underlying geometry data.
	// Note: normally this data is owned by the graphics system (not in our case though).
	if (m_track)
	{
		m_track->removeReference();
	}

	if (m_reorientAction != HK_NULL)
	{
		m_reorientAction->removeReference();
	}

	m_world->unlock();
}

void CarDemo::makeFakeInput()
{
	m_env->m_gamePad->forceButtonPressed(HKG_PAD_DPAD_UP);
}

///[stepGame]
/// This is called every simulation timestep. We need to
/// - Steer the first vehicle based on user input.
/// - Step the simulation.
/// - Sync each vehicle's display wheels.
/// - Update the camera that follows the first vehicle.
/// - Draw skidmarks for the first vehicle if it is skidding.
/// - Update the RPM meter and speedometer.
///
hkDemo::Result CarDemo::stepDemo()
{
	{
		m_world->markForWrite();

		//
		// Steer the vehicle from user input.
		//
		steer();

		m_world->unmarkForWrite();
	}

	//
	// Step the world.
	//
	{
		hkDefaultPhysicsDemo::stepDemo();
	}

	{
		m_world->markForWrite();

		for (int vehicleId = 0; vehicleId < m_vehicles.getSize(); vehicleId++ )
		{
			VehicleApiUtils::syncDisplayWheels(m_env, 
				*m_vehicles[vehicleId].m_vehicle,
				m_displayWheelId[vehicleId], m_tag);
		}

		// Update the "follow" camera.
		VehicleDisplayUtils::VehicleDataAndDisplayInfo& playerVehicle = m_vehicles[0];

		VehicleApiUtils::updateCamera( m_env, *playerVehicle.m_vehicle->getChassis(), m_timestep, m_camera);

		VehicleDisplayUtils::updateTyremarks( m_timestep, playerVehicle.m_vehicle );

		VehicleDisplayUtils::updateInfo( m_env, m_vehicles[0] );

		m_world->unmarkForWrite();
	}

	return DEMO_OK;
}
///>

void CarDemo::steer()
{
	//
	// Activate chassis if any of the driving/reorientation keys have been pressed
	//
	if(m_env->m_gamePad->isButtonPressed(HKG_PAD_BUTTON_0)
		|| m_env->m_gamePad->isButtonPressed(HKG_PAD_BUTTON_2)
		|| m_env->m_gamePad->isButtonPressed(HKG_PAD_BUTTON_3)
		|| m_env->m_gamePad->isButtonPressed(HKG_PAD_DPAD_UP)
		|| m_env->m_gamePad->isButtonPressed(HKG_PAD_DPAD_DOWN)
		|| m_env->m_gamePad->isButtonPressed(HKG_PAD_DPAD_RIGHT)
		|| m_env->m_gamePad->isButtonPressed(HKG_PAD_DPAD_LEFT)
		|| m_env->m_gamePad->isButtonPressed(HKG_PAD_BUTTON_R2)
		)
	{
		m_vehicles[0].m_vehicle->getChassis()->activate();
	}

	// 	 
	// Reorient the vehicle if it has flipped 	 
	//
	if( m_reorientAction && m_world )
	{
		VehicleApiUtils::reorient( m_env->m_gamePad->isButtonPressed( HKG_PAD_BUTTON_R2 ), m_reorientAction, m_world );
	}

	// Steer the vehicle.

	VehicleApiUtils::steer( m_env->m_gamePad,
							m_inputXPosition,
							m_inputYPosition,
						   *m_vehicles[0].m_vehicle,
							m_timestep);
}


class CarSampledHeightFieldShape: public hkpSampledHeightFieldShape
{
	public:
		CarSampledHeightFieldShape( const hkpSampledHeightFieldBaseCinfo& ci )
			: hkpSampledHeightFieldShape(ci)
		{
		}

		HK_FORCE_INLINE hkReal getHeightAtImpl( int x, int z ) const
		{
			HK_ASSERT(0x31f89011,  x>=0 && x < m_xRes );
			HK_ASSERT(0x4c32b1bc,  z>=0 && z < m_zRes );
			if (0)
			{
				hkReal sx = hkMath::sin( x * 0.8f );
				hkReal sy = hkMath::sin( z * 0.99f );

				return sx + sy;
			}
			return (x == m_xRes/2) && (z == m_zRes/2);
		}

		/// this should return true if the two triangles share the edge p00-p11
		///      otherwise it should return false if the triangles shere the edge p01-p10
		HK_FORCE_INLINE hkBool getTriangleFlipImpl() const
		{	
			return false;
		}

		virtual void collideSpheres( const CollideSpheresInput& input, SphereCollisionOutput* outputArray) const
		{
			hkSampledHeightFieldShape_collideSpheres(*this, input, outputArray);
		}
};


void CarDemo::buildLandscape()
{
	{
		// Setup some materials to use
		hkgMaterial* floorMaterial;
		{
			hkgDisplayContext* ctx = m_env->m_window->getContext();

			floorMaterial = hkgMaterial::create();
			floorMaterial->setDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
			floorMaterial->setSpecularColor(1.0f, 0.7f, 0.7f);
			floorMaterial->setSpecularPower(10);
			hkgTexture* texture = hkgTexture::create(ctx);
			texture->loadFromFile(".\\Resources\\Internal\\Showcase\\Gdc2005\\Level\\bricks1.png" );
			//texture->loadBuiltIn( HKG_TEXTURE_PERLINNOISE );
			texture->setAutoMipMaps(true);
			texture->realize();
			floorMaterial->addTexture(texture);
			texture->release();

			m_env->m_displayHandler->setTextureMapScale(0.5f);
			m_env->m_displayHandler->setTextureMapLocalMode(false);
		}

		//
		// Create the ground we'll drive on.
		//
		{
			hkpRigidBodyCinfo groundInfo;


			//
			//	Set the if condition to 0 if you want to test the heightfield
			//
			if ( 1 )
			{
				FlatLand* fl = new FlatLand();
				m_track = fl;
				groundInfo.m_shape = fl->createMoppShapeForSpu();
				groundInfo.m_position.set(5.0f, -2.0f, 5.0f);
			}
			else
			{
				hkpSampledHeightFieldBaseCinfo ci;
				ci.m_xRes = 64;
				ci.m_zRes = 64;
				ci.m_scale.set( 4.0f, 1.5f, 4.0f );
				m_track = HK_NULL;
				groundInfo.m_shape = new CarSampledHeightFieldShape( ci );
				groundInfo.m_position.set(-0.5f * ci.m_xRes * ci.m_scale(0), -2.0f, -0.5f * ci.m_zRes * ci.m_scale(2));
			}

			{
				groundInfo.m_motionType = hkpMotion::MOTION_FIXED;
				groundInfo.m_friction = 0.5f;
				hkpRigidBody* groundbody = new hkpRigidBody(groundInfo);
				m_world->addEntity(groundbody);
				groundbody->removeReference();

				hkgDisplayObject* dispObj = m_env->m_displayHandler->findDisplayObject( (hkUlong)groundbody->getCollidable() );
				if (dispObj)
				{
					dispObj->setMaterialOnAll(floorMaterial);
				}
			}

			groundInfo.m_shape->removeReference();
		}
		floorMaterial->removeReference();
	}

	if (1)
	{
		hkVector4 halfExtents(0.5f, 0.5f, 0.5f);
		hkVector4 startPos(-240.0f, -7.0f, 0.0f);
		hkVector4 diffPos (30.0f, 0.0f, 0.0f);
		createDodgeBoxes(5, halfExtents, startPos, diffPos);
	}

	if (1)
	{
		int gridSize = 1 + int(hkMath::sqrt( hkReal(m_env->m_cpuMhz/100) ));
		createRagdollGrid( m_world, gridSize, gridSize, 4.0f, 4.0f, m_ragdolls );
	}

	if (1)
	{
		// Make a body tagged with 'low friction' 
		
		hkpRigidBodyCinfo icyBoxInfo;
		icyBoxInfo.m_position.set(5.0f, -2.0f, 5.0f);
		icyBoxInfo.m_motionType = hkpMotion::MOTION_FIXED;
		icyBoxInfo.m_friction = 1.0f;

	
		hkVector4 halfExtentsBox(30.0f, 2.0f, 30.0f);
	
		icyBoxInfo.m_shape = new hkpBoxShape(halfExtentsBox, 0 );

		icyBoxInfo.m_position.set(-50.0f, -9.95f, 130.0f);
		hkpRigidBody* rb = new hkpRigidBody(icyBoxInfo);
		m_world->addEntity(rb);
		rb->removeReference();

		// Color so we can see it
		HK_SET_OBJECT_COLOR((hkUlong)rb->getCollidable(), hkColor::rgbFromChars(200, 200, 255));	
		hkpPropertyValue val(1);
		rb->addProperty(VEHICLE_RAYCAST_ICY_FRICTION_PROPERTY, val);
	
		icyBoxInfo.m_shape->removeReference();
	}

	/*
	if ( 0 )
	{
		hkVector4 boxSize( 0.5f, 0.3f, 0.3f);
		hkpBoxShape* box = new hkpBoxShape( boxSize , 0 );
		{		
			hkVector4 position( 20, -5, 50 );
			BrickWall::createBrickWall( m_world, 5, 5, position, 0.2f, box );
		}
		{		
			hkVector4 position( 20, -5, 60 );
			BrickWall::createBrickWall( m_world, 6, 6, position, 0.2f, box );
		}
		{		
			hkVector4 position( 20, -5, 70 );
			BrickWall::createBrickWall( m_world, 5, 5, position, 0.2f, box );
		}
		box->removeReference();
	}
	//
	//	Create a moving plattform
	//
	if(0)
	{
		hkpRigidBodyCinfo plattformInfo;

		hkVector4 halfExtents( 5.0f, 0.1f, 5.0f );
		plattformInfo.m_shape = new hkpBoxShape( halfExtents, 0.01f);
		{
			plattformInfo.m_motionType = hkpMotion::MOTION_DYNAMIC;
			plattformInfo.m_mass = 1500.0f;
			const hkReal d = plattformInfo.m_mass * hkReal(halfExtents.length3());
			plattformInfo.m_inertiaTensor.setDiagonal( d, d, d);
			plattformInfo.m_friction = 2.0f;
			plattformInfo.m_position.set( 30, -4.8f, 0 );
			plattformInfo.m_angularVelocity.set( 0,0,.1f);


			hkpRigidBody* plattform = new hkpRigidBody(plattformInfo);
			m_world->addEntity(plattform);

			//
			//	Create a point to point constraint
			//
			{
				hkBallAndSocketConstraintDataCinfo bsi;
				bsi.setInWorldSpace( plattform, m_world->getFixedRigidBody(), plattformInfo.m_position);
				m_world->addConstraint(new hkpBallAndSocketConstraintData( bsi ))->removeReference();
			}
			plattform->removeReference();

		}

		plattformInfo.m_shape->removeReference();

	}
	*/
}

void CarDemo::createDodgeBoxes(hkUint16 num, hkVector4& halfExtents, hkVector4& startPos, hkVector4& diffPos)
{
	for(int i = 0; i < num; i++)
	{
		/////////////////// SHAPE CONSTRUCTION ////////////////
		hkpBoxShape* shape = new hkpBoxShape(halfExtents);
		///////////////////////////////////////////////////////


		hkVector4 gapp;
		gapp.setMul4((hkReal)i, diffPos);
		
		// To illustrate using the shape, create a rigid body by first defining a template.
		hkpRigidBodyCinfo rigidBodyInfo;

		rigidBodyInfo.m_shape = shape;
		rigidBodyInfo.m_position.setAdd4(startPos, gapp);
		rigidBodyInfo.m_angularDamping = 0.0f;
		rigidBodyInfo.m_linearDamping = 0.0f;

		rigidBodyInfo.m_motionType = hkpMotion::MOTION_FIXED;

		// If we set this to true, the body is fixed, and no mass properties need to be computed.
		//rigidBodyInfo.m_motionType = hkpMotion::MOTION_BOX_INERTIA;

		// If we need to compute mass properties, we'll do this using the hkpInertiaTensorComputer.
		if (rigidBodyInfo.m_motionType != hkpMotion::MOTION_FIXED)
		{
			hkReal mass = 10.0f;
			hkpMassProperties massProperties;
			hkpInertiaTensorComputer::computeBoxVolumeMassProperties(halfExtents, mass, massProperties);

			rigidBodyInfo.m_inertiaTensor = massProperties.m_inertiaTensor;
			rigidBodyInfo.m_centerOfMass = massProperties.m_centerOfMass;
			rigidBodyInfo.m_mass = massProperties.m_mass;			
		}	
			
		 
		// Create a rigid body (using the template above).
		hkpRigidBody* rigidBody = new hkpRigidBody(rigidBodyInfo);

		// Remove reference since the body now "owns" the Shape.
		shape->removeReference();

		// Finally add body so we can see it, and remove reference since the world now "owns" it.
		m_world->addEntity(rigidBody);
		rigidBody->removeReference();
	}

}

//
// Create a grid of ragdolls
//

void CarDemo::createRagdollGrid( hkpWorld* world, int x_size, int y_size, hkReal xStep, hkReal yStep, hkArray<hkRagdoll*>&	ragdollsOut)
{
	int systemGroup = 2;
	int collisionLayer = 2;
	hkReal ragdollHeight = 2.50f;
	
	for( int x = 0; x < x_size; x++ )
	{
		for( int y = 0; y < y_size; y++ )
		{
			hkVector4 position; 
			// do a raycast to place the ragdoll
			{
				hkpWorldRayCastInput ray;
				ray.m_from.set( x * xStep,  10, y * yStep );
				ray.m_to.  set( x * xStep, -10, y * yStep );
				hkpWorldRayCastOutput result;
				world->castRay( ray, result );
				position.setInterpolate4( ray.m_from, ray.m_to, result.m_hitFraction );
				position(1) += ragdollHeight* 0.5f;
			}

			hkQuaternion	rotation; rotation.setIdentity();

			rotation.setAxisAngle( hkTransform::getIdentity().getColumn(0), HK_REAL_PI  * -0.5f );
			
			hkpPhysicsSystem* ragdoll = GameUtils::createRagdoll( ragdollHeight, position, rotation, systemGroup, GameUtils::RPT_CAPSULE );
			{
				for ( int i = 0; i < ragdoll->getRigidBodies().getSize(); i++)
				{
					hkpRigidBody* rb = ragdoll->getRigidBodies()[i];
					rb->setCollisionFilterInfo( hkpGroupFilter::calcFilterInfo( collisionLayer, systemGroup ) );
				}
			}
			systemGroup++;
	
			world->addPhysicsSystem(ragdoll);
			ragdoll->getRigidBodies()[0]->deactivate();
			ragdoll->removeReference();
		}
	}
}


void CarDemo::setUpWorld()
{
	m_tag = 0;

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
		info.setBroadPhaseWorldSize(2050.0f) ;
		info.setupSolverInfo(hkpWorldCinfo::SOLVER_TYPE_4ITERS_MEDIUM);
		m_world = new hkpWorld( info );
		m_world->lock();

		// Register all agents.
		hkpAgentRegisterUtil::registerAllAgents(m_world->getCollisionDispatcher());
		// Graphics.
		setupGraphics();
	}

	//
	// Create a filter, so that the raycasts of car does not collide with the ragdolls
	//
	{
		hkpGroupFilter* filter = new hkpGroupFilter();
		filter->disableCollisionsBetween(1,3);

		m_world->setCollisionFilter( filter );
		filter->removeReference();
	}



	// Build the landscape to drive on and add it to m_world.
	buildLandscape();

	m_reorientAction = HK_NULL;
	m_world->unlock();
}

hkpVehicleInstance* CarDemo::createVehicle( VehicleSetup& vehicleSetup, hkpRigidBody* chassis)
{
	hkpVehicleInstance* vehicle;
	//
	// Create the basic vehicle.
	//
	{
		vehicle = new hkpVehicleInstance( chassis );
		vehicleSetup.buildVehicle( m_world, *vehicle );
	}

	//
	// The vehicle is an action
	//
	{
		m_world->addAction(vehicle);
	}
	
	return vehicle;
}

void CarDemo::createDisplayWheels(hkReal radius, hkReal thickness)
{
	//
	// Create the wheel display geometries. Note: we use a Havok hkDisplayGeometry here, to
	// make binding to the Havok demo framework and Visual Debugger graphics engine easier.
	// However you can use whatever you want!
	//

	// The id for the wheel displayGeometry is an odd number. This prevents it conflicting
	// with hkpCollidable pointers which are used as ids for rigid bodies by convention in 
	// the Visual Debugger.
	int displayId = 1;

	m_displayWheelId.setSize( m_vehicles.getSize() );

	for (int vehicleId = 0; vehicleId < m_vehicles.getSize(); vehicleId++)
	{
		m_displayWheelId[vehicleId].setSize( m_numWheels );

		// Create a display wheel for each wheel on the vehicle.
		for(int i = 0; i < m_numWheels; i++)
		{
			VehicleApiUtils::createDisplayWheels(m_env, m_tag, displayId, radius, thickness);

			m_displayWheelId[vehicleId][i] = displayId;

			displayId += 2;
		}
	}
}

#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = "Controls:\n" \
"\224\225 - accelerate / brake " \
"\226\227 - steer\n" \
"\221 - handbrake " \
"\223 - brake / reverse " \
"\235 - reorient\n"
"Turn right from start position to find 'icy patch'.";

HK_DECLARE_DEMO(CarDemo, HK_DEMO_TYPE_PHYSICS, "Drive a car on a MOPP landscape", helpString);

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
