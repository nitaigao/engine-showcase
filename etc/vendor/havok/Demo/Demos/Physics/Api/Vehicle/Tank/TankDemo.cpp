/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>

#include <Physics/Collide/Shape/Compound/Collection/List/hkpListShape.h>
#include <Physics/Collide/Shape/Convex/ConvexTransform/hkpConvexTransformShape.h>
#include <Physics/Collide/Agent/CompoundAgent/BvTree/hkpBvTreeAgent.h>

#include <Demos/Physics/Api/Vehicle/Tank/TankDemo.h>
#include <Demos/Physics/Api/Vehicle/Tank/TankSetup.h>

#include <Demos/Physics/Api/Vehicle/VehicleApi/VehicleApiUtils.h>

// We will need these shapes.
#include <Demos/DemoCommon/Utilities/GameUtils/Landscape/FlatLand.h>
#include <Physics/Vehicle/Camera/hkp1dAngularFollowCam.h>

// Need this to generate plane equations for hkpConvexVerticesShape
#include <Common/Internal/ConvexHull/hkGeometryUtility.h>

// Need this to generate plane equations for hkpConvexVerticesShape
#include <Common/Internal/ConvexHull/hkGeometryUtility.h>

// Need to build and update "wheel" display objects.
#include <Physics/Utilities/VisualDebugger/Viewer/hkpShapeDisplayBuilder.h>
#include <Graphics/Bridge/DisplayHandler/hkgDisplayHandler.h>
#include <Common/Visualize/Shape/hkDisplayGeometry.h>
#include <Common/Visualize/hkDebugDisplay.h>
#include <Graphics/Common/Window/hkgWindow.h>

// Vehicle specific includes.
#include <Physics/Utilities/Actions/Reorient/hkpReorientAction.h>

TankDemo::TankDemo(hkDemoEnvironment* env)
:	CarDemo(env, false, 4, 1),
	m_numTimesteps(0)
{
	m_world->lock();

	m_flags = DEMO_FLAGS_NO_SERIALIZE;
	{

		hkpConvexVerticesShape* body = HK_NULL;
		hkpConvexVerticesShape* turret = HK_NULL;
		{
			hkReal xSize = 1.6f;
			hkReal ySize = 0.5f;
			hkReal zSize = 1.0f;

			hkReal xBumper = 1.9f;
			hkReal yBumper = 0.35f;
			hkReal zBumper = 1.0f;

			// 16 = 4 (size of "each float group", 3 for x,y,z, 1 for padding) * 4 (size of float).
			int stride = sizeof(float) * 4;
			
			{
				int numVertices = 12;

				float vertices[] = { 
					xSize, ySize, zSize, 0.0f,		// v0
					xSize, ySize, -zSize, 0.0f,		// v1
					xSize, -ySize, zSize, 0.0f,		// v2
					xSize, -ySize, -zSize, 0.0f,	// v3
					-xSize, ySize, zSize, 0.0f,		// v4
					-xSize, ySize, -zSize, 0.0f,	// v5
					-xSize, -ySize, zSize, 0.0f,	// v4
					-xSize, -ySize, -zSize, 0.0f,	// v5

					xBumper, yBumper, zBumper, 0.0f,	// v6
					xBumper, yBumper, -zBumper, 0.0f,	// v7
					-xBumper, yBumper, zBumper, 0.0f,	// v8
					-xBumper, yBumper, -zBumper, 0.0f,	// v9
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

					body = new hkpConvexVerticesShape(stridedVerts, planeEquations);
				}

				body->setRadius(0.05f);
			}

			hkReal xRoofFront = 0.8f;
			hkReal xRoofBack = -0.9f;
			hkReal yRoof = ySize + 0.6f;
			hkReal zRoof = 0.7f;
			hkReal zRoofFront = zRoof - 0.2f;

			hkReal xRoofFrontTop = xRoofFront - 0.4f;
			hkReal xRoofBackTop = xRoofBack + 0.2f;
			hkReal zRoofTop = zRoof - 0.2f;
			hkReal zRoofTopFront = zRoofTop - 0.2f;

			{
				int numVertices = 8;

				float vertices[] = { 
					xRoofFront, ySize, zRoofFront, 0.0f,	// v0
					xRoofFront, ySize, -zRoofFront, 0.0f,	// v1
					xRoofBack, ySize, zRoof, 0.0f,			// v2
					xRoofBack, ySize, -zRoof, 0.0f,			// v3

					xRoofFrontTop, yRoof, zRoofTopFront, 0.0f,	// v4
					xRoofFrontTop, yRoof, -zRoofTopFront, 0.0f,	// v5
					xRoofBackTop, yRoof, zRoofTop, 0.0f,		// v6
					xRoofBackTop, yRoof, -zRoofTop, 0.0f,		// v7
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

					turret = new hkpConvexVerticesShape(stridedVerts, planeEquations);
				}
				turret->setRadius(0.1f);
			}
		}

		//
		// Create vehicle's chassis shape.
		//
		hkpListShape* chassisShape;
		{
			hkVector4 halfSize1( 1.9f, 0.5f, 1.0f);
			hkVector4 halfSize2( 0.9f, 0.3f, 0.8f);
			hkVector4 halfSize3( 1.2f, 0.1f, 0.1f);

			hkArray<hkpShape*> shapeArray;

			hkpBoxShape* gun = new hkpBoxShape(halfSize3, 0 );

			body->setRadius(0.05f);
			turret->setRadius(0.05f);
			gun->setRadius(0.05f);

			hkTransform transform;
			transform.setIdentity();

			hkVector4 translate2 = hkVector4(1.4f, 0.8f, 0.0f);
			transform.setTranslation(translate2);
			
			hkpConvexTransformShape* gunTrans = new hkpConvexTransformShape(gun, transform);

			shapeArray.pushBack(body);
			shapeArray.pushBack(turret);
			shapeArray.pushBack(gunTrans);

			chassisShape = new hkpListShape(shapeArray.begin(), shapeArray.getSize());

			gunTrans->removeReference();

			body->removeReference();
			turret->removeReference();
			gun->removeReference();
		}

		createDisplayWheels(0.4f, 0.4f);

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

					chassisInfo.m_mass = 16000.0f;	
					chassisInfo.m_shape = chassisShape;
					chassisInfo.m_friction = 0.8f;
					chassisInfo.m_motionType = hkpMotion::MOTION_BOX_INERTIA;
					// Position chassis on the ground.
					chassisInfo.m_position.set(-40.0f, -4.5f, vehicleId * 7.0f);
					hkpInertiaTensorComputer::setShapeVolumeMassProperties(chassisInfo.m_shape,
																			chassisInfo.m_mass,
																			chassisInfo);

					chassisRigidBody = new hkpRigidBody(chassisInfo);

					m_world->addEntity(chassisRigidBody);

					HK_SET_OBJECT_COLOR((hkUlong)chassisRigidBody->getCollidable(), 0x80ff8080);
				}

				// This hkpAction flips the car upright if it turns over. 	 
				if (vehicleId == 0) 	 
				{ 	 
					hkVector4 rotationAxis(1.0f, 0.0f, 0.0f); 	 
					hkVector4 upAxis(0.0f, 1.0f, 0.0f); 	 
					hkReal strength = 10.0f; 	 
					m_reorientAction = new hkpReorientAction(chassisRigidBody, rotationAxis, upAxis, strength); 	 
				}

				TankSetup tsetup;
				m_vehicles[vehicleId].m_vehicle = createVehicle( tsetup, chassisRigidBody);
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

hkDemo::Result TankDemo::stepDemo()
{
	m_numTimesteps++;
	if (m_numTimesteps > 220)
	{
		m_numTimesteps = 0;
	}

	//
	// Steer the tanks other than the one driven by the user.
	//
	for (int vehicleId = 1; vehicleId < m_numVehicles; vehicleId++ )
	{
		hkpVehicleDriverInputAnalogStatus* deviceStatus = (hkpVehicleDriverInputAnalogStatus*)m_vehicles[vehicleId].m_vehicle->m_deviceStatus;
		deviceStatus->m_positionY = -0.35f;

		if (m_numTimesteps > 200)
		{
			// Turn the tanks to the right.
			deviceStatus->m_positionX = 0.2f;
		}
		else
		{
			// Drive straight ahead.
			deviceStatus->m_positionX = 0.0f;
		}
	}

	// Save the scene to an XML file.
	///saveScene(m_vehicle[0]);

	return CarDemo::stepDemo();
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

HK_DECLARE_DEMO(TankDemo, HK_DEMO_TYPE_PHYSICS, "Drive a heavy tank on a MOPP landscape", helpString );

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
