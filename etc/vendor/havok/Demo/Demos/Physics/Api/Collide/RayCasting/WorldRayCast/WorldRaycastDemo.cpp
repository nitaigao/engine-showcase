/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/Physics/Api/Collide/RayCasting/WorldRayCast/WorldRaycastDemo.h>

#include <Physics/Collide/Query/CastUtil/hkpWorldRayCastInput.h>
#include <Physics/Collide/Query/CastUtil/hkpWorldRayCastOutput.h>

// We need to display the results of the raycast to confirm correctness.
#include <Common/Visualize/hkDebugDisplay.h>


// let's reuse some functions in ShapeRaycastDemo
#include <Demos/Physics/Api/Collide/RayCasting/ShapeRayCast/ShapeRaycastDemo.h>
#include <Physics/Collide/Query/Collector/RayCollector/hkpClosestRayHitCollector.h>

#include <Common/Internal/ConvexHull/hkGeometryUtility.h>

#include <Common/Base/Algorithm/PseudoRandom/hkPseudoRandomGenerator.h>


WorldRaycastDemo::WorldRaycastDemo(hkDemoEnvironment* env)
:	hkDefaultPhysicsDemo(env)
{
	//
	// Setup the camera.
	//
	{
		hkVector4 from(30.0f, 8.0f, 25.0f);
		hkVector4 to  ( 4.0f, 0.0f, -3.0f);
		hkVector4 up  ( 0.0f, 1.0f,  0.0f);
		setupDefaultCameras(env, from, to, up);
	}

	//
	// Create the world.
	//
	{
		hkpWorldCinfo info;
		info.setupSolverInfo(hkpWorldCinfo::SOLVER_TYPE_4ITERS_MEDIUM); 
		
		// Set gravity to zero so body floats.
		info.m_gravity.set(0.0f, 0.0f, 0.0f);	
		info.setBroadPhaseWorldSize( 100.0f );
		m_world = new hkpWorld(info);
		m_world->lock();

		// Disable backface culling, since we have mopp's etc.
		setGraphicsState(HKG_ENABLED_CULLFACE, false);

		setupGraphics();
	}

	// register all agents(however, we put all objects into the some group,
	// so no collision will be detected
	hkpAgentRegisterUtil::registerAllAgents( m_world->getCollisionDispatcher() );

	// Add a collision filter to the world to allow the bodies interpenetrate
	{
		hkpGroupFilter* filter = new hkpGroupFilter();
		filter->disableCollisionsBetween( hkpGroupFilterSetup::LAYER_DEBRIS, hkpGroupFilterSetup::LAYER_DEBRIS );
		m_world->setCollisionFilter( filter );
		filter->removeReference();
	}

	//
	// Set the simulation time to 0
	//
	m_time = 0;


	//
	// Create some bodies (reuse the ShapeRaycastApi demo)
	//
	createBodies();

	m_world->unlock();
}


hkDemo::Result WorldRaycastDemo::stepDemo()
{
	m_world->lock();

	m_time += m_timestep;

	// The start point of the ray remains fixed in world space with the destination point of the
	// ray being varied in both the X and Y directions. This is achieved with simple
	// sine and cosine functions calls using the current time as the varying parameter:

	hkReal xDir = 12.0f * hkMath::sin(m_time * 0.3f);
	hkReal yDir = 12.0f * hkMath::cos(m_time * 0.3f);
	
	// The start and end points are both specified in World Space as we are using the world castRay() method
	// to fire the ray.

	hkpWorldRayCastInput input;
	input.m_from.set(0.0f, 0.0f, 15.0f);
	input.m_to.set( xDir, yDir, -15.0f);

	hkpClosestRayHitCollector output;
	m_world->castRay(input, output );

	// To visualise the raycast we make use of a macro defined in "hkDebugDisplay.h" called HK_DISPLAY_LINE.
	// The macro takes three parameters: a start point, an end point and the line color.
	// If a hit is found we display a RED line from the raycast start point to the point of intersection and mark that
	// point with a small RED cross. The intersection point is calculated using: startWorld + (result.m_mindist * endWorld).
	//
	// If no hit is found we simply display a GREY line between the raycast start and end points.

	if( output.hasHit() )
	{
		hkVector4 intersectionPointWorld;
		intersectionPointWorld.setInterpolate4( input.m_from, input.m_to, output.getHit().m_hitFraction );
		HK_DISPLAY_LINE( input.m_from, intersectionPointWorld, hkColor::RED);
		HK_DISPLAY_ARROW( intersectionPointWorld, output.getHit().m_normal, hkColor::CYAN);
	}
	else
	{
		// Otherwise draw as GREY
		HK_DISPLAY_LINE(input.m_from, input.m_to, hkColor::rgbFromChars(200, 200, 200));
	}

	m_world->unlock();

	return hkDefaultPhysicsDemo::stepDemo();
}



void WorldRaycastDemo::createBodies()
{
	hkpRigidBodyCinfo rigidBodyInfo;
	rigidBodyInfo.m_collisionFilterInfo = hkpGroupFilter::calcFilterInfo(1, 1);
	hkPseudoRandomGenerator rand(100);


	for( int i = 0; i < 100; i++)
	{
		// All bodies created below are movable
		rigidBodyInfo.m_motionType = hkpMotion::MOTION_SPHERE_INERTIA;

		// A collection of 100 rigid bodies is randomly created by generating an integer in the range(0,4) and choosing
		// one of the following shapes; MOPP, Convex Vertices, Box, Sphere or Triangle depending on the outcome:
		int shapeType = (int) (rand.getRandRange(0,1) * 5);
		switch(shapeType)
		{
			case 0:
			// Create MOPP body
			{
			
				hkpMoppBvTreeShape* shape = ShapeRaycastDemo::createMoppShape();
				rigidBodyInfo.m_shape = shape;
				break;
			}

			// The construction of each of these is quite similar and for the purposes of this tutorial we will just outline
			// that of the Convex Vertices object, the code for which is presented below.

			// Create ConvexVertices body
			case 1:
			{
				// Data specific to this shape.
				int numVertices = 4;

					// 16 = 4 (size of "each float group", 3 for x,y,z, 1 for padding) * 4 (size of float)
				int stride = 16;

				float vertices[] = { // 4 vertices plus padding
					-2.0f, 1.0f, 1.0f, 0.0f, // v0
					 1.0f, 2.0f, 0.0f, 0.0f, // v1
					 0.0f, 0.0f, 3.0f, 0.0f, // v2
					 1.0f, -1.0f, 0.0f, 0.0f  // v3
				};
				

				hkpConvexVerticesShape* shape;
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

					shape = new hkpConvexVerticesShape(stridedVerts, planeEquations);
				}

				rigidBodyInfo.m_shape = shape;
				break;
			}

			// Create Box body
			case 2:
			{
				// Data specific to this shape.
				hkVector4 halfExtents = hkVector4(1.0f, 2.0f, 3.0f);
				hkpBoxShape* shape = new hkpBoxShape(halfExtents );
				rigidBodyInfo.m_shape = shape;
				break;
			}

			// Create Sphere body
			case 3:
			{
				hkReal radius = rand.getRandRange(0.5f, 2.0f);
				hkpConvexShape* shape = new hkpSphereShape(radius);
				rigidBodyInfo.m_shape = shape;		
				break;
			}

			// Create Triangle body
			case 4:
			{
				hkVector4 a(-1.5f, -1.5f,  0.0f);
				hkVector4 b(1.5f, -1.5f,  0.0f);
				hkVector4 c(0.0f,  1.5f,  0.0f);

				hkpTriangleShape* shape = new hkpTriangleShape(a, b, c);
				rigidBodyInfo.m_shape = shape;
				break;
			}
		}	// end case

		// As usual we fill out the hkpRigidBodyCinfo 'blueprint' for the rigidbody, with the code above specifying
		// the necessary information for the 'm_shape' member. To create a hkpConvexVerticesShape we need a set of vertices and
		// we must generate a set of plane equations from these points. As you can see from the code this is all performed 
		// prior to instantiating the shape.

		// Fake Inertia tensor for simplicity, assume it's a unit cube
		{
			hkVector4 halfExtents(0.5f, 0.5f, 0.5f);
			hkReal mass = 10.0f;
			hkpMassProperties massProperties;
			hkpInertiaTensorComputer::computeBoxVolumeMassProperties(halfExtents, mass, massProperties);

			rigidBodyInfo.m_inertiaTensor = massProperties.m_inertiaTensor;
			rigidBodyInfo.m_centerOfMass = massProperties.m_centerOfMass;
			rigidBodyInfo.m_mass = massProperties.m_mass;			
		}	

		// The object is then assigned a random position, orientation and angular velocity and added to the world:

		rigidBodyInfo.m_position.set( rand.getRandRange(-10.0f, 10.0f), rand.getRandRange(-10.0f, 10.0f), rand.getRandRange(0.0f, -40.0f));
		rand.getRandomRotation( rigidBodyInfo.m_rotation );

		rigidBodyInfo.m_collisionFilterInfo = hkpGroupFilterSetup::LAYER_DEBRIS;

		hkpRigidBody* rigidBody = new hkpRigidBody(rigidBodyInfo);
		rigidBodyInfo.m_shape->removeReference();

		// Give them an initial velocity
		hkVector4 angularVel(rand.getRandRange(-1.0f, 1.0f), rand.getRandRange(-1.0f, 1.0f), rand.getRandRange(-1.0f, 1.0f));
		rigidBody->setAngularVelocity(angularVel);
		rigidBody->setAngularDamping(0.0f);

		m_world->addEntity(rigidBody);
		rigidBody->removeReference();

		// There is no gravity vector for this world and so the bodies will appear to float in space.
	}
}

/*
hkpMoppBvTreeShape* WorldRaycastDemo::createMoppShape()
{
	const int side = 5;
	const int numVertices = side * side;
	const int numTriangles = 2 * (side - 1) * (side-1);

	static float vertices[numVertices * 3];
	static hkUint16 indices[numTriangles * 3];

	int i,j;

	for(i = 0; i < side; i++)
	{
		for (j = 0; j < side; j++ )
		{
			vertices[(i * side + j) * 3 + 0] = i * 1.0f - side * 0.5f;
			vertices[(i * side + j) * 3  + 1] = j * 1.0f;
			vertices[(i * side + j) * 3  + 2] = hkMath::cos((hkReal)j + i) + 0.5f * hkMath::sin( 2.0f * i) ;
		}
	}

	int corner = 0;
	int index = 0;
	for(i = 0; i < side - 1; i++)
	{
		for (j = 0; j < side - 1; j++ )
		{
			indices[index] = corner;
			indices[index + 1] = corner + 1;
			indices[index + 2] = corner + side;

			indices[index + 3] = corner + 1;
			indices[index + 4] = corner + side;
			indices[index + 5] = corner + side + 1;

			index += 6;
			corner++; 
		}
		corner++; 
	}



	hkpMeshShape* meshShape = new hkpMeshShape;
	meshShape->setRadius( 0.05f );
	{
		hkpMeshShape::Subpart part;

		part.m_vertexBase = vertices;
		part.m_vertexStriding = sizeof(float) * 3;
		part.m_numVertices = numVertices;

		part.m_indexBase = indices;
		part.m_indexStriding = sizeof( hkUint16 ) * 3;
		part.m_numTriangles = numTriangles;
		part.m_stridingType = hkpMeshShape::INDICES_INT16;

		meshShape->addSubpart( part );
	}

	hkpMoppCompilerInput mci;
	hkpMoppCode* code = hkpMoppUtility::buildCode( meshShape ,mci);

	hkpMoppBvTreeShape* moppShape = new hkpMoppBvTreeShape(meshShape, code);
	code->removeReference();
	meshShape->removeReference();

	return moppShape;
}
*/

#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
"A demo which shows use of the hkpWorld raycast which searches all Shapes of all the RigidBodies " \
"in the World for intersection, returning the minimum if an intersection occurs. It is optimized " \
"to use the broad-phase to avoid expensive low-level intersection tests, and thus will usually be " \
"considerably faster to use than simply iterating through all RigidBodies in the World. " \
"The ray is moved over time in this demo to illustrate correctness of the raycast even as the bodies " \
"move around the world. ";

HK_DECLARE_DEMO(WorldRaycastDemo, HK_DEMO_TYPE_PRIME, "Using World raycasts. Ray is clipped to first intersection point", helpString);

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
