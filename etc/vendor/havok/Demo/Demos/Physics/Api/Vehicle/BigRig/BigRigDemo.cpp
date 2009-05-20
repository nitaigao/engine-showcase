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

#include <Physics/Dynamics/Constraint/Bilateral/Hinge/hkpHingeConstraintData.h>
#include <Physics/Dynamics/Constraint/Bilateral/BallAndSocket/hkpBallAndSocketConstraintData.h>
#include <Physics/Dynamics/Constraint/Bilateral/Ragdoll/hkpRagdollConstraintData.h>

#include <Physics/Vehicle/hkpVehicleInstance.h>
#include <Physics/Vehicle/DriverInput/Default/hkpVehicleDefaultAnalogDriverInput.h>

#include <Demos/Physics/Api/Vehicle/BigRig/BigRigDemo.h>
#include <Demos/Physics/Api/Vehicle/BigRig/TractorSetup.h>
#include <Demos/Physics/Api/Vehicle/BigRig/TrailerSetup.h>

#include <Demos/Physics/Api/Vehicle/VehicleApi/VehicleApiUtils.h>
#include <Demos/Physics/Api/Vehicle/VehicleApi/VehicleSetup.h>

// We will need these shapes.
#include <Demos/DemoCommon/Utilities/GameUtils/Landscape/FlatLand.h>
#include <Physics/Vehicle/Camera/hkp1dAngularFollowCam.h>

// Need to build and update "wheel" display objects.
#include <Graphics/Bridge/DisplayHandler/hkgDisplayHandler.h>
#include <Common/Visualize/Shape/hkDisplayGeometry.h>
#include <Graphics/Common/Window/hkgWindow.h>


	// Used to generate a convex hull for convexVerticesShape
#include <Common/Internal/ConvexHull/hkGeometryUtility.h>

#include <Physics/Utilities/VisualDebugger/Viewer/Collide/hkpShapeDisplayViewer.h>
#include <Physics/Utilities/VisualDebugger/Viewer/hkpShapeDisplayBuilder.h>
#include <Common/Visualize/hkDebugDisplay.h>


BigRigDemo::BigRigDemo(hkDemoEnvironment* env) : CarDemo(env, false)
{
	m_world->lock();

	hkpGroupFilter* groupFilter = new hkpGroupFilter();
	m_world->setCollisionFilter( groupFilter );
	groupFilter->removeReference();
    
    // Create the vehicles
    {
        //
        // Create tractor shape.
        //
        hkpListShape* tractorShape = HK_NULL;
        {
            hkReal xSize = 1.9f;
            hkReal xSizeFrontTop = 0.7f;
            hkReal xSizeFrontMid = 1.6f;
            hkReal ySize = 0.9f;
            hkReal ySizeMid = ySize - 0.7f;
            hkReal zSize = 1.0f;
            
            //hkReal xBumper = 1.9f;
            //hkReal yBumper = 0.35f;
            //hkReal zBumper = 1.0f;
            
            hkVector4 halfExtents(1.0f, 0.35f, 1.0f);
            
            // 16 = 4 (size of "each float group", 3 for x,y,z, 1 for padding) * 4 (size of float).
            int stride = sizeof(float) * 4;
            
            {
                int numVertices = 10;
                
                float vertices[] = { 
                    xSizeFrontTop, ySize, zSize, 0.0f,	// v0
                        xSizeFrontTop, ySize, -zSize, 0.0f,	// v1
                        xSize, -ySize, zSize, 0.0f,			// v2
                        xSize, -ySize, -zSize, 0.0f,		// v3
                        -xSize, ySize, zSize, 0.0f,			// v4
                        -xSize, ySize, -zSize, 0.0f,		// v5
                        -xSize, -ySize, zSize, 0.0f,		// v6
                        -xSize, -ySize, -zSize, 0.0f,		// v7
                        xSizeFrontMid, ySizeMid, zSize, 0.0f,	// v8
                        xSizeFrontMid, ySizeMid, -zSize, 0.0f,	// v9
                };
                
                hkpBoxShape* boxShape = new hkpBoxShape(halfExtents, 0 );
                boxShape->setRadius(0.05f);

				hkTransform transform;
                transform.setIdentity();
                transform.setTranslation(hkVector4(-2.5f, -0.55f, 0.0f));
                
                hkpConvexTransformShape* transformShape = new hkpConvexTransformShape(boxShape, transform);
				boxShape->removeReference();
                
				hkpConvexVerticesShape* convexShape;
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

					convexShape = new hkpConvexVerticesShape(stridedVerts, planeEquations);
				}
                convexShape->setRadius(0.05f);
                
                hkArray<hkpShape*> shapeArray;
                shapeArray.pushBack(transformShape);
                shapeArray.pushBack(convexShape);
                
                tractorShape = new hkpListShape(shapeArray.begin(), shapeArray.getSize());
				transformShape->removeReference();
				convexShape->removeReference();
            }
        }
        
        //
        // Create trailer shape.
        //
        hkpListShape* trailerShape = HK_NULL;
        {
            
            hkVector4 halfExtents1(5.0f, 1.5f, 1.0f); // Box
            hkVector4 halfExtents2(1.0f, 1.0f, 1.0f); // Tongue
            
            hkTransform transform;
            transform.setIdentity();
            
            {						
                hkpBoxShape* boxShape1 = new hkpBoxShape(halfExtents1, 0 );
                boxShape1->setRadius(0.05f);
                
                hkpBoxShape* boxShape2 = new hkpBoxShape(halfExtents2, 0 );
                boxShape2->setRadius(0.05f);
                
                transform.setTranslation(hkVector4(6.0f, 0.5f, 0.0f));
				hkpConvexTransformShape* transformShape = new hkpConvexTransformShape(boxShape2, transform);
				boxShape2->removeReference();
                
                hkArray<hkpShape*> shapeArray;
                shapeArray.pushBack(boxShape1);
                shapeArray.pushBack(transformShape);
                
                trailerShape = new hkpListShape(shapeArray.begin(), shapeArray.getSize());
				boxShape1->removeReference();
				transformShape->removeReference();
            }
        }
        
        
        // Create the tractor vehicles
		
        {
			m_vehicles.clear();
            for (int vehicleId = 0; vehicleId < m_numVehicles; vehicleId++)
            {
				int vehicleGroup = groupFilter->getNewSystemGroup();
                //
                // Create the tractor.
                //
                {
                    //
                    // Create the tractor body.
                    //
                    hkpRigidBody* tractorRigidBody;
                    {
                        hkpRigidBodyCinfo tractorInfo;
                        tractorInfo.m_shape = tractorShape;
                        tractorInfo.m_friction = 0.8f;
                        tractorInfo.m_motionType = hkpMotion::MOTION_BOX_INERTIA;
						tractorInfo.m_collisionFilterInfo = hkpGroupFilter::calcFilterInfo(0, vehicleGroup );

						// Position chassis on the ground.
                        tractorInfo.m_position.set(0.0f, -3.0f, (vehicleId + 1) * 9.0f);
                        //tractorInfo.m_angularDamping = 0.5f;

						// No need to set the inertia tensor, as this will be set automatically by the vehicle setup
						tractorInfo.m_mass = 15000.0f;
						tractorInfo.m_inertiaTensor.setDiagonal( 1.f, 1.f, 1.f );
						tractorInfo.m_centerOfMass.set( -1.0f, -0.8f, 0.0f);
                                              
                        tractorRigidBody = new hkpRigidBody(tractorInfo);
                        
                        m_world->addEntity(tractorRigidBody);
                    }

					m_vehicles.expandOne();
					TractorSetup tsetup;
                    createTractorVehicle( tsetup, tractorRigidBody, m_vehicles.getSize()-1);
                    tractorRigidBody->removeReference();
                }

				//
                // Create the trailer body.
                //
				{
                    hkpRigidBody* trailerRigidBody;
                    {
                        hkpRigidBodyCinfo trailerInfo;
                        trailerInfo.m_shape = trailerShape;
                        trailerInfo.m_friction = 0.8f;
                        trailerInfo.m_motionType = hkpMotion::MOTION_BOX_INERTIA;
                        trailerInfo.m_position.set(-9.0f, -3.0f, (1 + vehicleId) * 9.0f);
						trailerInfo.m_collisionFilterInfo = hkpGroupFilter::calcFilterInfo(0, vehicleGroup );

						// No need to set the inertia tensor, as this will be set automatically by the vehicle setup
                        trailerInfo.m_mass = 10000.0f;
						trailerInfo.m_inertiaTensor.setDiagonal( 1.f, 1.f, 1.f );

						// Trailers are generally bottom-heavy to improve stability.
						// Move the centre of mass lower but keep it within the chassis limits.
						trailerInfo.m_centerOfMass.set(-1.0f, -1.0f, 0.0f);
                                              
                        trailerRigidBody = new hkpRigidBody(trailerInfo);
                        
                        m_world->addEntity(trailerRigidBody);

						HK_SET_OBJECT_COLOR((hkUlong)trailerRigidBody->getCollidable(), 0x80ff8080);
                    }
                    
					m_vehicles.expandOne();
					TrailerSetup tsetup;
					createTrailerVehicle( tsetup, trailerRigidBody, m_vehicles.getSize()-1);
                    
                    trailerRigidBody->removeReference();
                }
            }
            
            tractorShape->removeReference();
            trailerShape->removeReference();
        }
		
    }

	//
	//	Create the wheels
	//
	{
		createDisplayWheels(0.5f, 0.2f);
	}


    // Attach Tractors and Trailers
	
    {
        for (int vehicleId = 0; vehicleId < m_vehicles.getSize(); vehicleId+=2)
        {
            hkpRigidBody* tractor = m_vehicles[vehicleId].m_vehicle->getChassis();
            hkpRigidBody* trailer = m_vehicles[vehicleId + 1 ].m_vehicle->getChassis();


			//
			// Use a hinge constraint to connect the tractor to the trailer
			//
			if (0)
	        {
				//
			    // Set the pivot
				//
		        hkVector4 axis(0.0f, 1.0f, 0.0f);
				hkVector4 point1(-3.5f, 0.4f, 0.0f);
				hkVector4 point2(6.0f, -0.4f, 0.0f);

			        // Create constraint
				hkpHingeConstraintData* hc = new hkpHingeConstraintData(); 
		        hc->setInBodySpace(point1, point2, axis, axis); 
		        m_world->createAndAddConstraintInstance( tractor, trailer, hc )->removeReference();
		        hc->removeReference();  
	        }
			
			//
			// Use a ball-and-socket constraint to connect the tractor to the trailer
			//
			else if (0)
			{
				hkVector4 point1(-3.5f, 0.4f, 0.0f);
				hkVector4 point2(6.0f, -0.4f, 0.0f);

				// Create the constraint
				hkpBallAndSocketConstraintData* bs = new hkpBallAndSocketConstraintData(); 
				bs->setInBodySpace(point1, point2);
				m_world->createAndAddConstraintInstance( tractor, trailer, bs )->removeReference();
				bs->removeReference();  
			}

			//
			// Use a ragdoll constraint to connect the tractor to the trailer
			//
			else if(1)
			{
				hkReal planeMin =  HK_REAL_PI * -0.45f;
				hkReal planeMax =  HK_REAL_PI * 0.45f;
				hkReal twistMin =  HK_REAL_PI * -0.005f;
				hkReal twistMax =  HK_REAL_PI *  0.005f;
				hkReal coneMin  =  HK_REAL_PI * -0.55f;
				hkReal coneMax = HK_REAL_PI * 0.55f;

				hkpRagdollConstraintData* rdc = new hkpRagdollConstraintData();
				rdc->setPlaneMinAngularLimit(planeMin);
				rdc->setPlaneMaxAngularLimit(planeMax);
				rdc->setTwistMinAngularLimit(twistMin);
				rdc->setTwistMaxAngularLimit(twistMax);

				hkVector4 twistAxisA(1.0f, 0.0f, 0.0f);
				hkVector4 planeAxisA(0.0f, 0.0f, 1.0f);
				hkVector4 twistAxisB(1.0f, 0.0f, 0.0f);
				hkVector4 planeAxisB(0.0f, 0.0f, 1.0f);
				hkVector4 point1(-2.3f, -0.7f, 0.0f);
				hkVector4 point2( 7.2f, -1.3f, 0.0f);

				rdc->setInBodySpace(point1, point2, planeAxisA, planeAxisB, twistAxisA, twistAxisB);
				rdc->setAsymmetricConeAngle(coneMin, coneMax);
				m_world->createAndAddConstraintInstance(tractor, trailer, rdc)->removeReference();
				rdc->removeReference();
			}
        }
    }
    
    //
    // Create the camera.
    //
    {
		hkp1dAngularFollowCamCinfo cinfo;

		cinfo.m_yawSignCorrection = 1.0f; 
		cinfo.m_upDirWS.set(0.0f, 1.0f, 0.0f); 
		cinfo.m_rigidBodyForwardDir.set(1.0f, 0.0f, 0.0f); 

		cinfo.m_set[0].m_velocity = 10.0f;
		cinfo.m_set[1].m_velocity = 50.0f;
		cinfo.m_set[0].m_speedInfluenceOnCameraDirection = 1.0f;
		cinfo.m_set[1].m_speedInfluenceOnCameraDirection = 1.0f;
		cinfo.m_set[0].m_angularRelaxation = 3.5f;
		cinfo.m_set[1].m_angularRelaxation = 4.5f;

		// The two camera positions ("slow" and "fast" rest positions) are both the same here,
		// -6 units behind the chassis, and 2 units above it. Again, this is dependent on 
		// m_chassisCoordinateSystem.
		cinfo.m_set[0].m_positionUS.set( -27.0f, 8.0f, 0.0f); 
		cinfo.m_set[1].m_positionUS.set( -27.0f, 8.0f, 0.0f); 

		cinfo.m_set[0].m_lookAtUS.set ( 2.0f, 0.0f, 0.0f );
		cinfo.m_set[1].m_lookAtUS.set ( 2.0f, 0.0f, 0.0f );

		cinfo.m_set[0].m_fov = 70.0f;
		cinfo.m_set[1].m_fov = 70.0f;

		m_camera.reinitialize( cinfo );
    }
	m_world->unlock();
}

hkDemo::Result BigRigDemo::stepDemo()
{
	return CarDemo::stepDemo();
}

void BigRigDemo::createDisplayWheels(hkReal radius, hkReal thickness)
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
    
    m_displayWheelId.setSize(m_vehicles.getSize());
    
    for (int tractorId = 0; tractorId < m_vehicles.getSize(); tractorId+=2)
    {
		{
			m_displayWheelId[tractorId].setSize( m_vehicles[tractorId].m_vehicle->m_data->m_numWheels );
        
			// Create a display wheel for each wheel on the vehicle.
			for(int i = 0; i < m_vehicles[tractorId].m_vehicle->m_data->m_numWheels; i++)
			{
				VehicleApiUtils::createDisplayWheels(m_env, m_tag, displayId, radius, thickness);
				m_displayWheelId[tractorId][i] = displayId;
				displayId += 2;
			}
		}

		{
			int trailerId = tractorId+1;
			m_displayWheelId[trailerId].setSize(m_vehicles[trailerId].m_vehicle->m_data->m_numWheels);
        
			// Create a display wheel for each wheel on the vehicle.
			for(int i = 0; i < m_vehicles[trailerId].m_vehicle->m_data->m_numWheels; i++)
			{
				// Trailer wheels are thicker, like two wheels side by side.
				VehicleApiUtils::createDisplayWheels(m_env, m_tag, displayId, radius, 0.5f);
				m_displayWheelId[trailerId][i] = displayId;
				displayId += 2;
			}
		}
    }
}

void BigRigDemo::createTractorVehicle(VehicleSetup& vehicleSetup, hkpRigidBody* chassis, int id)
{
	m_vehicles[id].m_vehicle = CarDemo::createVehicle( vehicleSetup, chassis);
	m_vehicles[id].m_lastRPM = 0.0f;

	// These big tractors need to accelerate a little harder
	hkpVehicleDriverInputAnalogStatus* deviceStatus = (hkpVehicleDriverInputAnalogStatus*)(m_vehicles[id].m_vehicle->m_deviceStatus);
	deviceStatus->m_positionY = -0.4f;
	deviceStatus->m_positionX = 0.12f;

	m_vehicles[id].m_lastRPM = 0.0f;
}

void BigRigDemo::createTrailerVehicle( VehicleSetup& vehicleSetup, hkpRigidBody* chassis, int id)
{
	m_vehicles[id].m_vehicle = CarDemo::createVehicle( vehicleSetup, chassis);
	m_vehicles[id].m_lastRPM = 0.0f;


	// No input to the trailers as they are dragged by the tractors, not driven.
	hkpVehicleDriverInputAnalogStatus* deviceStatus = (hkpVehicleDriverInputAnalogStatus*)(m_vehicles[id].m_vehicle->m_deviceStatus);

	deviceStatus->m_positionY = 0.0f;
	deviceStatus->m_positionX = 0.0f;

	m_vehicles[id].m_lastRPM = 0.0f;
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

HK_DECLARE_DEMO(BigRigDemo, HK_DEMO_TYPE_PHYSICS, "Drive a tractor-trailer on a MOPP landscape", helpString );

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
