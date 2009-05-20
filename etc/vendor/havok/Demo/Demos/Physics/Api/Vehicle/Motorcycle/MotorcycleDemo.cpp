/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

///[TUTORIAL 2]

#include <Demos/demos.h>


#include <Demos/Physics/Api/Vehicle/Motorcycle/MotorcycleDemo.h>
#include <Demos/Physics/Api/Vehicle/Motorcycle/MotorcycleSetup.h>
#include <Demos/Physics/Api/Vehicle/VehicleApi/VehicleApiUtils.h>

// We will need these shapes.
#include <Demos/DemoCommon/Utilities/GameUtils/Landscape/FlatLand.h>
#include <Physics/Vehicle/Camera/hkp1dAngularFollowCam.h>

// Need to build and update "wheel" display objects.
#include <Physics/Utilities/VisualDebugger/Viewer/hkpShapeDisplayBuilder.h>
#include <Graphics/Bridge/DisplayHandler/hkgDisplayHandler.h>
#include <Common/Visualize/Shape/hkDisplayGeometry.h>
#include <Graphics/Common/Window/hkgWindow.h>

#include <Common/Internal/ConvexHull/hkGeometryUtility.h>

// Vehicle specific includes.

#include <Physics/Vehicle/hkpVehicleInstance.h>
#include <Physics/Vehicle/DriverInput/Default/hkpVehicleDefaultAnalogDriverInput.h>

	// The construction kit used to build the generic constraint
#include <Physics/Dynamics/Constraint/ConstraintKit/hkpConstraintConstructionKit.h>

#include <Common/Visualize/hkDebugDisplay.h>

#include <Demos/DemoCommon/Utilities/GameUtils/GameUtils.h>


MotorcycleDemo::MotorcycleDemo(hkDemoEnvironment* env)
:	CarDemo(env, false, 2, 1)
{
	m_world->lock();

	// Disable warning: 'The force feedback multiplier value is large...'
	hkError::getInstance().setEnabled(0x5582dad2, false);

	m_flags = DEMO_FLAGS_NO_SERIALIZE; //XXX
	m_genericConstraint.setSize( m_numVehicles );
	m_constraint.setSize( m_numVehicles );

	{
		//
		// Create vehicle's chassis shape.
		//
		hkpConvexVerticesShape* chassisShape = HK_NULL;
		{	
			hkReal xSize = 0.7f;
			//hkReal ySize = 0.3f;
			hkReal ySizeUp = 0.5f;
			hkReal ySizeDown = -0.3f;
			hkReal zSize = 0.2f;

			hkReal xBumperFront = 0.95f;
			hkReal xBumperBack = -1.1f;
			hkReal yBumper = 0.13f;
			hkReal zBumper = 0.05f;

			hkReal xBodyFront = 0.5f;
			hkReal xBodyBack = -0.5f;
			hkReal yBody = 0.13f;
			hkReal zBody = 0.25f;

			// Data specific to this shape.
			int numVertices = 16;

			// 16 = 4 (size of "each float group", 3 for x,y,z, 1 for padding) * 4 (size of float).
			int stride = sizeof(float) * 4;

			float vertices[] = { 
				xSize, ySizeUp, zSize, 0.0f,	// v0
				xSize, ySizeUp, -zSize, 0.0f,	// v1
				xSize, ySizeDown, zSize, 0.0f,	// v2
				xSize, ySizeDown, -zSize, 0.0f, // v3

				-xSize, ySizeUp, zSize, 0.0f,		// v4
				-xSize, ySizeUp, -zSize, 0.0f,		// v5
				-xSize, ySizeDown, zSize, 0.0f,		// v6
				-xSize, ySizeDown, -zSize, 0.0f,	// v7

				xBumperFront, yBumper, zBumper, 0.0f,	// v8
				xBumperFront, yBumper, -zBumper, 0.0f,	// v9
				xBumperBack, yBumper, zBumper, 0.0f,	// v10
				xBumperBack, yBumper, -zBumper, 0.0f,	// v11

				xBodyFront, yBody, zBody, 0.0f,		// v12
				xBodyFront, yBody, -zBody, 0.0f,	// v13
				xBodyBack, yBody, zBody, 0.0f,		// v14
				xBodyBack, yBody, -zBody, 0.0f,		// v15
			};
			
			//
			// SHAPE CONSTRUCTION.
			//

			
			hkArray<hkVector4> planeEquations;
			hkGeometry geom;
			{
				hkStridedVertices stridedVerts;
				{
					stridedVerts.m_numVertices = numVertices;
					stridedVerts.m_striding = stride;
					stridedVerts.m_vertices = vertices;
				}

				hkGeometryUtility::createConvexGeometry( stridedVerts, geom, planeEquations );

				{
					stridedVerts.m_numVertices = geom.m_vertices.getSize();
					stridedVerts.m_striding = sizeof(hkVector4);
					stridedVerts.m_vertices = &(geom.m_vertices[0](0));
				}

				chassisShape = new hkpConvexVerticesShape(stridedVerts, planeEquations);
			}
			chassisShape->setRadius(0.05f);
		}

		createDisplayWheels(0.4f, 0.25f);

		for (int vehicleId = 0; vehicleId < m_numVehicles; vehicleId++)
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
					chassisInfo.m_mass = 250.0f;
					chassisInfo.m_shape = chassisShape;
					chassisInfo.m_friction = 0.8f;
					chassisInfo.m_motionType = hkpMotion::MOTION_BOX_INERTIA;
					// Position chassis on the ground.
					chassisInfo.m_position.set(-10.0f, -4.5f, vehicleId * 5.0f);
					hkpInertiaTensorComputer::setShapeVolumeMassProperties(chassisInfo.m_shape,
																			chassisInfo.m_mass,
																			chassisInfo);

					chassisRigidBody = new hkpRigidBody(chassisInfo);

					m_world->addEntity(chassisRigidBody);
				}

				MotorcycleSetup setup;
				m_vehicles[vehicleId].m_vehicle = createVehicle( setup, chassisRigidBody);
				m_vehicles[vehicleId].m_lastRPM = 0.0f;

				///[SetUpConstaint]
				/// Use the constraint kit to create a 1d angular constraint to hold motorcycle
				/// upright and to tilt it while turning.
				///
				{
					// Up axis.
					hkVector4 upWorldSpace = m_vehicles[vehicleId].m_vehicle->m_data->m_chassisOrientation.getColumn(0);
					// Forward axis.
					hkVector4 forwardChassisSpace = m_vehicles[vehicleId].m_vehicle->m_data->m_chassisOrientation.getColumn(1);
					// Right axis.
					hkVector4 rightChassisSpace = m_vehicles[vehicleId].m_vehicle->m_data->m_chassisOrientation.getColumn(2);

					{
						// Constrain the chassis with respect to the world.
						m_genericConstraint[vehicleId] =	new hkpGenericConstraintData();
						hkpConstraintConstructionKit kit;

						// Must always start with this command (the constraint works like a program, every
						// command being executed sequentially).
						kit.begin(m_genericConstraint[vehicleId]);
						{
							// Forward axis, x.
							const hkVector4 colX(1.0f, 0.0f, 0.0f);

							// Up axis, y.
							const hkVector4 colY(0.0f, 1.0f, 0.0f);

							// Right axis, z.
							const hkVector4 colZ(0.0f, 0.0f, 1.0f);

							hkMatrix3 constrainB;
							hkMatrix3 constrainA;

							constrainA.setCols(colX, colY, colZ);
							constrainB.setCols(colX, colY, colZ);

							//
							// Set both basis' to Identity at start.
							//
							m_basisIndexA = kit.setAngularBasisA(constrainA);
							m_basisIndexB = kit.setAngularBasisB(constrainB);

							// Let angle vary bewteen -0.01 and 0.01 radians
							kit.setAngularLimit(0, -0.01f, 0.01f);
						}
						// Must always end with this command.
						kit.end();
								
						m_constraint[vehicleId] = m_world->createAndAddConstraintInstance(m_world->getFixedRigidBody(), chassisRigidBody, m_genericConstraint[vehicleId]);
					}
				}
				///>
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

MotorcycleDemo::~MotorcycleDemo()
{
	hkError::getInstance().setEnabled(0x5582dad2, true);

	//
	// Remove constraint for each motorcycle.
	//
	hkReferencedObject::lockAll();
	for (int vehicleId = 0; vehicleId < m_vehicles.getSize(); vehicleId++ )
	{
		m_genericConstraint[vehicleId]->removeReference();
		m_constraint[vehicleId]->removeReference();
	}
	hkReferencedObject::unlockAll();
}

/// The motorcycle is a little different as the constraint can is removed if the motorcycle crashes
/// and each motorcycle must be tilted while it turns.
void MotorcycleDemo::steer()
{
	for (int vehicleId = 0; vehicleId < m_vehicles.getSize(); vehicleId++)
	{
		hkpVehicleDriverInputAnalogStatus deviceStatus = *(hkpVehicleDriverInputAnalogStatus*)(m_vehicles[vehicleId].m_vehicle->m_deviceStatus);
		//
		// User has control of motorcycle 0.
		//
		if (vehicleId == 0 )
		{
			const hkgPad* pad = m_env->m_gamePad;	

			//
			// Activate chassis if any of the driving/reorientation keys have been pressed
			//
			if(pad->isButtonPressed(HKG_PAD_BUTTON_0)
				|| pad->isButtonPressed(HKG_PAD_BUTTON_2)
				|| pad->isButtonPressed(HKG_PAD_BUTTON_3)
				|| pad->isButtonPressed(HKG_PAD_DPAD_UP)
				|| pad->isButtonPressed(HKG_PAD_DPAD_DOWN)
				|| pad->isButtonPressed(HKG_PAD_DPAD_RIGHT)
				|| pad->isButtonPressed(HKG_PAD_DPAD_LEFT))
			{
				m_vehicles[0].m_vehicle->getChassis()->activate();
			}

			// User decides that motorcycle has tumbled irretrievably.
			if ( pad->isButtonPressed(HKG_PAD_BUTTON_3) != 0 )
			{
				// Upright the motorcycle.
				uprightMotorcycle();

				if (m_constraint[0]->getOwner() == HK_NULL)
				{
					m_world->addConstraint(m_constraint[0]);
				}
			}
			else
			{
				// Steer the vehicle.
				VehicleApiUtils::steer(m_env->m_gamePad,
					m_inputXPosition,
					m_inputYPosition,
					*m_vehicles[0].m_vehicle,
					m_timestep);
			}
		}

		//
		// Update the generic constraint.
		//
		{
			const hkVector4 xAxis(1.0f, 0.0f, 0.0f);
			const hkVector4 zAxis(0.0f, 0.0f, 1.0f);

			hkVector4 x(1.0f, 0.0f, 0.0f);
			hkVector4 y(0.0f, 1.0f, 0.0f);
			hkVector4 z(0.0f, 0.0f, 1.0f);

			// The world.
			const hkpRigidBody* rbB = static_cast<hkpRigidBody*>(m_constraint[vehicleId]->getEntityB());

			//
			// Find the horizontal forward direction for the motorcycle.
			// Get the cross product vector of the up and the right vectors.
			//
			x.setRotatedDir( rbB->getRotation(), x );
			x(3) = 0.0f;
			x.normalize3();

			//
			// Prevent degenerate case where forwardA == up (or lessen impact of this case
			// at least)
			//
			if (m_constraint[vehicleId]->getOwner() !=
				HK_NULL && (x.dot3(y) > 0.9f || x.dot3(y) < -0.9f) )
			{
				m_world->removeConstraint(m_constraint[vehicleId]);
			}

			z.setCross( x, y );
			y.setCross( z, x );
			y.normalize3();
			z.normalize3();

			//
			// Set the forward direction of the constraint basis.
			// This basis keeps the motorcycle upright relative to the world.
			//
			m_genericConstraint[vehicleId]->setParameters(m_basisIndexA, 1, &x);
			m_genericConstraint[vehicleId]->setParameters(m_basisIndexA + 1, 1, &y);
			m_genericConstraint[vehicleId]->setParameters(m_basisIndexA + 2, 1, &z);

			//
			// Draw some debug lines to represent the constraint.
			//
			{
				// Draw the horizontal forward direction.
				hkVector4 line;
					
				line = x;
				line.mul4(1.5f);
				line.setAdd4(rbB->getCenterOfMassInWorld(), line);
				HK_DISPLAY_LINE(rbB->getCenterOfMassInWorld(), line, hkColor::RED);

				// Draw the up direction.
				line = y;
				line.mul4(1.5f);
				line.setAdd4(rbB->getCenterOfMassInWorld(), line);
				HK_DISPLAY_LINE(rbB->getCenterOfMassInWorld(), line, hkColor::GREEN);

				// Draw the side direction.
				line = z;
				line.mul4(1.5f);
				line.setAdd4(rbB->getCenterOfMassInWorld(), line);
				HK_DISPLAY_LINE(rbB->getCenterOfMassInWorld(), line, hkColor::BLUE);
			}
			// Rotate the axis to tilt the motorcycle.
			const hkQuaternion quat(xAxis, -0.4f * deviceStatus.m_positionX);
			z.setRotatedDir(quat, zAxis);

			y.setCross(z, xAxis);

			//
			// This basis tilts the motorcycle.
			//
			m_genericConstraint[vehicleId]->setParameters(m_basisIndexB + 1, 1, &y);
			m_genericConstraint[vehicleId]->setParameters(m_basisIndexB + 2, 1, &z);
		}
	}
}



// Upright the Motorcycle.
void MotorcycleDemo::uprightMotorcycle()
{
    // Drop the motorcycle back in from just above its current position.
    hkVector4 currentPos;
    currentPos = m_vehicles[0].m_vehicle->getChassis()->getPosition();
    hkVector4 change(0.0f, 0.1f, 0.0f, 0.0f);
    currentPos.add4(change);
    m_vehicles[0].m_vehicle->getChassis()->setPosition(currentPos);
    
    // Re-orient the motorcycle to its original facing.
	const hkVector4 y(0.0f, 1.0f, 0.0f);
	const hkVector4 z(0.0f, 0.0f, 1.0f);

	m_genericConstraint[0]->setParameters(m_basisIndexB + 1, 1, &y);
	m_genericConstraint[0]->setParameters(m_basisIndexB + 2, 1, &z);
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

HK_DECLARE_DEMO(MotorcycleDemo, HK_DEMO_TYPE_PHYSICS, "Drive a simple motorcycle on a MOPP landscape.", helpString );

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
