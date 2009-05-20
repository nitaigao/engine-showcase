/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

///[TUTORIAL 4]
#include <Demos/demos.h>


#include <Demos/Physics/Api/Vehicle/VehicleApi/VehicleSetup.h>
#include <Demos/Physics/Api/Vehicle/VehicleApi/FrictionMapVehicleRaycastWheelCollide.h>

void VehicleSetup::buildVehicle(hkpWorld* world, hkpVehicleInstance& vehicle )
{
	//
	// All memory allocations are made here.
	//

	vehicle.m_data				= new hkpVehicleData;
	vehicle.m_driverInput		= new hkpVehicleDefaultAnalogDriverInput;
	vehicle.m_steering			= new hkpVehicleDefaultSteering;
	vehicle.m_engine			= new hkpVehicleDefaultEngine;
	vehicle.m_transmission		= new hkpVehicleDefaultTransmission;
	vehicle.m_brake				= new hkpVehicleDefaultBrake;
	vehicle.m_suspension		= new hkpVehicleDefaultSuspension;
	vehicle.m_aerodynamics		= new hkpVehicleDefaultAerodynamics;
	vehicle.m_velocityDamper	= new hkpVehicleDefaultVelocityDamper;

	// For illustrative purposes we use a custom hkpVehicleRaycastWheelCollide
	// which implements varying 'ground' friction in a very simple way.
	vehicle.m_wheelCollide		= new FrictionMapVehicleRaycastWheelCollide;

	setupVehicleData( world, *vehicle.m_data );

	// initialise the tyremarks controller with 128 tyremark points.
	vehicle.m_tyreMarks		= new hkpTyremarksInfo( *vehicle.m_data, 128 );

	setupComponent( *vehicle.m_data, *static_cast< hkpVehicleDefaultAnalogDriverInput* >(vehicle.m_driverInput) );
	setupComponent( *vehicle.m_data, *static_cast< hkpVehicleDefaultSteering*>(vehicle.m_steering));
	setupComponent( *vehicle.m_data, *static_cast< hkpVehicleDefaultEngine*>(vehicle.m_engine) );
	setupComponent( *vehicle.m_data, *static_cast< hkpVehicleDefaultTransmission*>(vehicle.m_transmission) );
	setupComponent( *vehicle.m_data, *static_cast< hkpVehicleDefaultBrake*>(vehicle.m_brake) );
	setupComponent( *vehicle.m_data, *static_cast< hkpVehicleDefaultSuspension*>(vehicle.m_suspension) );
	setupComponent( *vehicle.m_data, *static_cast< hkpVehicleDefaultAerodynamics*>(vehicle.m_aerodynamics) );
	setupComponent( *vehicle.m_data, *static_cast< hkpVehicleDefaultVelocityDamper*>(vehicle.m_velocityDamper) );


	// The wheel collide component performs collision detection. To do this, it needs to create an 
	// aabbPhantom from the vehicle information that has been set here already.
	setupWheelCollide( world, vehicle, *static_cast< hkpVehicleRaycastWheelCollide*>(vehicle.m_wheelCollide) );

	setupTyremarks( *vehicle.m_data, *static_cast< hkpTyremarksInfo*>(vehicle.m_tyreMarks) );

	//
	// Check that all components are present.
	//
	HK_ASSERT(0x0, vehicle.m_data );
	HK_ASSERT(0x7708674a,  vehicle.m_driverInput );
	HK_ASSERT(0x5a324a2d,  vehicle.m_steering );
	HK_ASSERT(0x7bcb2aff,  vehicle.m_engine );
	HK_ASSERT(0x29bddb50,  vehicle.m_transmission );
	HK_ASSERT(0x2b0323a2,  vehicle.m_brake );
	HK_ASSERT(0x7a7ade23,  vehicle.m_suspension );
	HK_ASSERT(0x6ec4d0ed,  vehicle.m_aerodynamics );
	HK_ASSERT(0x67161206,  vehicle.m_wheelCollide );
	HK_ASSERT(0x295015f1,  vehicle.m_tyreMarks );

	//
	// Set up any variables that store cached data.
	//


	// Give driver input default values so that the vehicle (if this input is a default for non
	// player cars) will drive, even if it is in circles!

	// Accelerate.
	vehicle.m_deviceStatus = new hkpVehicleDriverInputAnalogStatus;
	hkpVehicleDriverInputAnalogStatus* deviceStatus = (hkpVehicleDriverInputAnalogStatus*)vehicle.m_deviceStatus;
	deviceStatus->m_positionY = -0.4f;

	// Turn.
	deviceStatus->m_positionX = 0.3f;

	// Defaults
	deviceStatus->m_handbrakeButtonPressed = false;
	deviceStatus->m_reverseButtonPressed = false;


	//
	// Don't forget to call init! (This function is necessary to set up derived data)
	//
	vehicle.init();

	//
	// The phantom for collision detection needs to be explicitly added to the world
	//

	world->addPhantom( (hkpPhantom*)(static_cast< hkpVehicleRaycastWheelCollide*>(vehicle.m_wheelCollide)->m_phantom) );
}

void VehicleSetup::setupVehicleData( hkpWorld* world, hkpVehicleData& data )
{
	data.m_gravity = world->getGravity();

	//
	// The vehicleData contains information about the chassis.
	//

	// The coordinates of the chassis system, used for steering the vehicle.
	//										up					forward				right
	data.m_chassisOrientation.setCols( hkVector4(0, 1, 0), hkVector4(1, 0, 0), hkVector4(0, 0, 1));

	data.m_frictionEqualizer = 0.5f; 


	// Inertia tensor for each axis is calculated by using : 
	// (1 / chassis_mass) * (torque(axis)Factor / chassisUnitInertia)
	data.m_torqueRollFactor = 0.625f; 
	data.m_torquePitchFactor = 0.5f; 
	data.m_torqueYawFactor = 0.35f; 

	data.m_chassisUnitInertiaYaw = 1.0f; 
	data.m_chassisUnitInertiaRoll = 1.0f;
	data.m_chassisUnitInertiaPitch = 1.0f; 

	// Adds or removes torque around the yaw axis 
	// based on the current steering angle.  This will 
	// affect steering.
	data.m_extraTorqueFactor = -0.5f; 
	data.m_maxVelocityForPositionalFriction = 0.0f; 

	//
	// Wheel specifications
	//
	data.m_numWheels = 4;

	data.m_wheelParams.setSize( data.m_numWheels );

	data.m_wheelParams[0].m_axle = 0;
	data.m_wheelParams[1].m_axle = 0;
	data.m_wheelParams[2].m_axle = 1;
	data.m_wheelParams[3].m_axle = 1;

	data.m_wheelParams[0].m_friction = 1.5f;
	data.m_wheelParams[1].m_friction = 1.5f;
	data.m_wheelParams[2].m_friction = 1.5f;
	data.m_wheelParams[3].m_friction = 1.5f;

	data.m_wheelParams[0].m_slipAngle = 0.0f;
	data.m_wheelParams[1].m_slipAngle = 0.0f;
	data.m_wheelParams[2].m_slipAngle = 0.0f;
	data.m_wheelParams[3].m_slipAngle = 0.0f;

	for ( int i = 0 ; i < data.m_numWheels ; i++ )
	{
		// This value is also used to calculate the m_primaryTransmissionRatio.
		data.m_wheelParams[i].m_radius = 0.4f;
		data.m_wheelParams[i].m_width = 0.2f;
		data.m_wheelParams[i].m_mass = 10.0f;

		data.m_wheelParams[i].m_viscosityFriction = 0.25f;
		data.m_wheelParams[i].m_maxFriction = 2.0f * data.m_wheelParams[i].m_friction;
		data.m_wheelParams[i].m_forceFeedbackMultiplier = 0.1f;
		data.m_wheelParams[i].m_maxContactBodyAcceleration = hkReal(data.m_gravity.length3()) * 2;
	}
}


void VehicleSetup::setupComponent( const hkpVehicleData& data, hkpVehicleDefaultAnalogDriverInput& driverInput )
{
	// We also use an analog "driver input" class to help converting user input to vehicle behavior.

	driverInput.m_slopeChangePointX = 0.8f;
	driverInput.m_initialSlope = 0.7f;
	driverInput.m_deadZone = 0.0f;
	driverInput.m_autoReverse = true;
}

void VehicleSetup::setupComponent( const hkpVehicleData& data, hkpVehicleDefaultSteering& steering )
{   
	steering.m_doesWheelSteer.setSize( data.m_numWheels );

	// degrees
	steering.m_maxSteeringAngle = 35 * ( HK_REAL_PI / 180 );

	// [mph/h] The steering angle decreases linearly 
	// based on your overall max speed of the vehicle. 
	steering.m_maxSpeedFullSteeringAngle = 70.0f * (1.605f / 3.6f);
	steering.m_doesWheelSteer[0] = true;
	steering.m_doesWheelSteer[1] = true;
	steering.m_doesWheelSteer[2] = false;
	steering.m_doesWheelSteer[3] = false;
}

void VehicleSetup::setupComponent( const hkpVehicleData& data, hkpVehicleDefaultEngine& engine )
{
	engine.m_maxTorque = 500.0f;

	engine.m_minRPM = 1000.0f;
	engine.m_optRPM = 5500.0f;

	// This value is also used to calculate the m_primaryTransmissionRatio.
	engine.m_maxRPM = 7500.0f;

	engine.m_torqueFactorAtMinRPM = 0.8f;
	engine.m_torqueFactorAtMaxRPM = 0.8f;

	engine.m_resistanceFactorAtMinRPM = 0.05f;
	engine.m_resistanceFactorAtOptRPM = 0.1f;
	engine.m_resistanceFactorAtMaxRPM = 0.3f;
}

void VehicleSetup::setupComponent( const hkpVehicleData& data, hkpVehicleDefaultTransmission& transmission )
{
	int numGears = 4;

	transmission.m_gearsRatio.setSize( numGears );
	transmission.m_wheelsTorqueRatio.setSize( data.m_numWheels );

	transmission.m_downshiftRPM = 3500.0f;
	transmission.m_upshiftRPM = 6500.0f;

	transmission.m_clutchDelayTime = 0.0f;
	transmission.m_reverseGearRatio = 1.0f;
	transmission.m_gearsRatio[0] = 2.0f;
	transmission.m_gearsRatio[1] = 1.5f;
	transmission.m_gearsRatio[2] = 1.0f;
	transmission.m_gearsRatio[3] = 0.75f;
	transmission.m_wheelsTorqueRatio[0] = 0.2f;
	transmission.m_wheelsTorqueRatio[1] = 0.2f;
	transmission.m_wheelsTorqueRatio[2] = 0.3f;
	transmission.m_wheelsTorqueRatio[3] = 0.3f;

	const hkReal vehicleTopSpeed = 130.0f; 	 
	const hkReal wheelRadius = 0.4f;
	const hkReal maxEngineRpm = 7500.0f;
	transmission.m_primaryTransmissionRatio = hkpVehicleDefaultTransmission::calculatePrimaryTransmissionRatio( vehicleTopSpeed,
																												wheelRadius,
																												maxEngineRpm,
																												transmission.m_gearsRatio[ numGears - 1 ] );
}

void VehicleSetup::setupComponent( const hkpVehicleData& data, hkpVehicleDefaultBrake& brake )
{
	brake.m_wheelBrakingProperties.setSize( data.m_numWheels );

	const float bt = 1500.0f;
	brake.m_wheelBrakingProperties[0].m_maxBreakingTorque = bt;
	brake.m_wheelBrakingProperties[1].m_maxBreakingTorque = bt;
	brake.m_wheelBrakingProperties[2].m_maxBreakingTorque = bt;
	brake.m_wheelBrakingProperties[3].m_maxBreakingTorque = bt;

	// Handbrake is attached to rear wheels only.
	brake.m_wheelBrakingProperties[0].m_isConnectedToHandbrake = false;
	brake.m_wheelBrakingProperties[1].m_isConnectedToHandbrake = false;
	brake.m_wheelBrakingProperties[2].m_isConnectedToHandbrake = true;
	brake.m_wheelBrakingProperties[3].m_isConnectedToHandbrake = true;
	brake.m_wheelBrakingProperties[0].m_minPedalInputToBlock = 0.9f;
	brake.m_wheelBrakingProperties[1].m_minPedalInputToBlock = 0.9f;
	brake.m_wheelBrakingProperties[2].m_minPedalInputToBlock = 0.9f;
	brake.m_wheelBrakingProperties[3].m_minPedalInputToBlock = 0.9f;
	brake.m_wheelsMinTimeToBlock = 1000.0f;
}

void VehicleSetup::setupComponent( const hkpVehicleData& data, hkpVehicleDefaultSuspension& suspension ) 
{
	suspension.m_wheelParams.setSize( data.m_numWheels );
	suspension.m_wheelSpringParams.setSize( data.m_numWheels );

	suspension.m_wheelParams[0].m_length = 0.35f;
	suspension.m_wheelParams[1].m_length = 0.35f;
	suspension.m_wheelParams[2].m_length = 0.35f;
	suspension.m_wheelParams[3].m_length = 0.35f;

	const float str = 50.0f;
	suspension.m_wheelSpringParams[0].m_strength = str;
	suspension.m_wheelSpringParams[1].m_strength = str;
	suspension.m_wheelSpringParams[2].m_strength = str;
	suspension.m_wheelSpringParams[3].m_strength = str;

	const float wd = 3.0f; 
	suspension.m_wheelSpringParams[0].m_dampingCompression = wd;
	suspension.m_wheelSpringParams[1].m_dampingCompression = wd;
	suspension.m_wheelSpringParams[2].m_dampingCompression = wd;
	suspension.m_wheelSpringParams[3].m_dampingCompression = wd;

	suspension.m_wheelSpringParams[0].m_dampingRelaxation = wd;
	suspension.m_wheelSpringParams[1].m_dampingRelaxation = wd;
	suspension.m_wheelSpringParams[2].m_dampingRelaxation = wd;
	suspension.m_wheelSpringParams[3].m_dampingRelaxation = wd;
	
	//
	// NB: The hardpoints MUST be positioned INSIDE the chassis.
	//
	{
		const hkReal hardPointFrontX = 1.3f;
		const hkReal hardPointBackX = -1.1f;
		const hkReal hardPointY = -0.05f;
		const hkReal hardPointZ = 1.1f;

		suspension.m_wheelParams[0].m_hardpointChassisSpace.set ( hardPointFrontX, hardPointY, -hardPointZ); 
		suspension.m_wheelParams[1].m_hardpointChassisSpace.set ( hardPointFrontX, hardPointY,  hardPointZ); 
		suspension.m_wheelParams[2].m_hardpointChassisSpace.set ( hardPointBackX, hardPointY, -hardPointZ); 
		suspension.m_wheelParams[3].m_hardpointChassisSpace.set ( hardPointBackX, hardPointY,  hardPointZ); 
	}

	const hkVector4 downDirection( 0.0f, -1.0f, 0.0f );
	suspension.m_wheelParams[0].m_directionChassisSpace = downDirection;
	suspension.m_wheelParams[1].m_directionChassisSpace = downDirection;
	suspension.m_wheelParams[2].m_directionChassisSpace = downDirection;
	suspension.m_wheelParams[3].m_directionChassisSpace = downDirection;
}

void VehicleSetup::setupComponent( const hkpVehicleData& data, hkpVehicleDefaultAerodynamics& aerodynamics ) 
{
	aerodynamics.m_airDensity = 1.3f;
	// In m^2.
	aerodynamics.m_frontalArea = 1.0f;	

	aerodynamics.m_dragCoefficient = 0.7f;
	aerodynamics.m_liftCoefficient = -0.3f;

	// Extra gavity applies in world space (independent of m_chassisCoordinateSystem).
	aerodynamics.m_extraGravityws.set( 0.0f, -5.0f, 0.0f); 
}

void VehicleSetup::setupComponent( const hkpVehicleData& data, hkpVehicleDefaultVelocityDamper& velocityDamper )
{
	// Caution: setting negative damping values will add energy to system. 
	// Setting the value to 0 will not affect the angular velocity. 

	// Damping the change of the chassis’ angular velocity when below m_collisionThreshold. 
	// This will affect turning radius and steering.
	velocityDamper.m_normalSpinDamping    = 0.0f; 

	// Positive numbers dampen the rotation of the chassis and 
	// reduce the reaction of the chassis in a collision. 
	velocityDamper.m_collisionSpinDamping = 4.0f; 

	// The threshold in m/s at which the algorithm switches from 
	// using the normalSpinDamping to the collisionSpinDamping. 	
	velocityDamper.m_collisionThreshold   = 1.0f; 
}

void VehicleSetup::setupWheelCollide( hkpWorld* world, const hkpVehicleInstance& vehicle, hkpVehicleRaycastWheelCollide& wheelCollide )
{
	wheelCollide.m_wheelCollisionFilterInfo = vehicle.getChassis()->getCollidable()->getCollisionFilterInfo();
}

void VehicleSetup::setupTyremarks( const hkpVehicleData& data, hkpTyremarksInfo& tyreMarks ) 
{
	tyreMarks.m_minTyremarkEnergy = 100.0f;
	tyreMarks.m_maxTyremarkEnergy  = 1000.0f;
}


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
