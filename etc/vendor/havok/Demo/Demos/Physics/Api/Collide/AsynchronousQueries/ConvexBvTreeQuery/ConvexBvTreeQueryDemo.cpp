/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


#include <Demos/demos.h>
#include <Demos/Physics/Api/Collide/AsynchronousQueries/ConvexBvTreeQuery/ConvexBvTreeQueryDemo.h>

#include <Common/Base/Algorithm/PseudoRandom/hkPseudoRandomGenerator.h>

#include <Common/Base/Monitor/hkMonitorStream.h>

// Needed to create planes of convex hull
// for hkpConvexVerticesShape
#include <Common/Internal/ConvexHull/hkGeometryUtility.h>

// Used to create the mesh and Mopp 'code' object
#include <Physics/Collide/Shape/Compound/Tree/Mopp/hkpMoppUtility.h>
#include <Physics/Collide/Shape/Compound/Tree/Mopp/hkpMoppBvTreeShape.h>
#include <Physics/Collide/Shape/Compound/Collection/ExtendedMeshShape/hkpExtendedMeshShape.h>

#include <Physics/Collide/Query/CastUtil/hkpWorldRayCastInput.h>
#include <Physics/Collide/Query/CastUtil/hkpWorldRayCastOutput.h>


#include <Physics/Internal/Collide/Util/hkpCollideTriangleUtil.h>
#include <Physics/Collide/Util/hkpTriangleUtil.h>

// We need to display the results of the raycast to confirm correctness.
#include <Common/Visualize/hkDebugDisplay.h>

// We need to display some text.
#include <Demos/DemoCommon/DemoFramework/hkTextDisplay.h>


//
// Geometric data 
//
		static float octVertices[] = { // 4 vertices plus padding
			3.6924f  ,	-11.5294f ,	0.f			,	0.0f,
			29.6231f ,	-11.5294f ,	-20.579f	,	0.0f,
			17.9186f ,	-11.5294f ,	-34.3475f	,	0.0f,
			-17.9186f,	 -11.5294f,	 -34.3475f	,	0.0f,
			-29.6231f,	 -11.5294f,	 -20.579f	,	0.0f,
			-29.6231f,	 -11.5294f,	 20.579f	,	0.0f,
			-17.9186f,	 -11.5294f,	 34.3475f	,	0.0f,
			17.9186f ,	-11.5294f ,	 34.3475f	,	0.0f,
			29.6231f ,	-11.5294f ,	 20.579f	,	0.0f,
			29.6231f ,	11.5294f ,	-20.579f	,	0.0f,
			17.9186f ,	11.5294f ,	-34.3475f	,	0.0f,
			-17.9186f,	 11.5294f ,	-34.3475f	,	0.0f,
			-29.6231f,	 11.5294f ,	-20.579f	,	0.0f,
			-29.6231f,	 11.5294f ,	20.579f		,	0.0f,
			-17.9186f,	 11.5294f ,	34.3475f	,	0.0f,
			17.9186f ,	11.5294f  ,	34.3475f	,	0.0f,
			29.6231f ,	11.5294f  ,	20.579f		,	0.0f,
			3.6924f ,	11.5294f ,	0.f			,	0.0f,
		};										 
												 
		static hkUint16 octTris[] = {			 
				0 ,	 2 	,	1  ,				 
				0 ,	 3 	,	2   ,				 
				0 ,	 4 	,	3   ,				 
				0 ,	 5 	,	4   ,
				0 ,	 6 	,	5   ,
				0 ,	 7 	,	6   ,
				0 ,	 8 	,	7   ,
				0 ,	 1 	,	8   ,
				1 ,	 10	,	9   ,
				1 ,	 2 	,	10   ,
				2 ,	 11	,	10   ,
				2 ,	 3 	,	11   ,
				3 ,	 12	,	11   ,
				3 ,	 4 	,	12   ,
				4 ,	 13	,	12   ,
				4 ,	 5 	,	13   ,
				5 ,	 14	,	13   ,
				5 ,	 6 	,	14   ,
				6 ,	 15 ,	14   ,
				6 ,	 7	,	15   ,
				7 ,	 16	,	15   ,
				7 ,	 8	,	16   ,
				8 ,	 9	,	16   ,
				8 ,	 1	,	9   ,
				17,	 9	,	10   ,
				17,	 10 ,	11   ,
				17,	 11 ,	12   ,
				17,	 12 ,	13   ,
				17,	 13 ,	14   ,
				17,	 14 ,	15   ,
				17,	 15 ,	16   ,
				17,	 16 ,	9   ,		  
		};		  
				  
				  
		// Data specific to this shape.
		static int g_numVertices = 18;
		static int g_numTris = 32;	

		static float* g_vertices = octVertices;
		static hkUint16* g_tris = octTris;
		static hkVector4 g_fireFrom( 75.f, 75.f, 75.f );



ConvexBvTreeQueryDemo::ConvexBvTreeQueryDemo(hkDemoEnvironment* env)
:	hkDefaultPhysicsDemo(env)
{
	m_generator = new hkPseudoRandomGenerator(747);

	//
	// Setup the camera.
	//
	{
		hkVector4 from(90, 80, 100);
		hkVector4 to(0.0f, 5.0f, 0.0f); 
		hkVector4 up(0.0f, 1.0f, 0.0f);
		setupDefaultCameras(env, from, to, up);
	}

	//
	// Create the world.
	//
	{
		hkpWorldCinfo info;
		
		// Set gravity to zero so body floats.
		info.m_gravity.set(0.0f, 0.0f, 0.0f);	
		info.setBroadPhaseWorldSize( 100.0f );
		m_world = new hkpWorld(info);
		m_world->lock();

		setupGraphics();
	}

	// We use two different shape types in this demo, a MOPP BV Tree shape and a Convex Vertices shape, although, only the
	// Convex Vertices shape is actually added to the world to 'physically' represent the object. The BV Tree shape
	// is solely used as a means to rapidly perform intersection tests via a sphere cast and extract the underlying
	// triangles.
	//
	// The code to create the MOPP BV Tree shape is given below:
	
	//
	// Mesh and BV Tree Construction
	//
	{
		hkpExtendedMeshShape* mesh = new hkpExtendedMeshShape();
		hkpExtendedMeshShape::TrianglesSubpart part;
		part.m_indexBase = g_tris;
		part.m_indexStriding = sizeof( hkUint16 ) * 3;
		part.m_numTriangleShapes = g_numTris;
		part.m_numVertices = g_numVertices;
		part.m_stridingType = hkpExtendedMeshShape::INDICES_INT16;
		part.m_vertexBase = g_vertices;
		part.m_vertexStriding = sizeof(float) * 4;

		mesh->addTrianglesSubpart( part );

		// Create MOPP Code  
		// Fit tolerance is important here to determine sphere query result accuracy.
		hkpMoppCompilerInput moppInput;
		moppInput.setAbsoluteFitToleranceOfTriangles(0.001f);

		hkpMoppCode* code = hkpMoppUtility::buildCode(mesh, moppInput);		

		// Create BV Tree Shape
		m_moppShape = new hkpMoppBvTreeShape(mesh, code);

		// Remove references since the MoppBvTreeShape now "owns" the mesh and code
		code->removeReference();
		mesh->removeReference();
	}

	// and the following is the core code to create the Convex Vertices shape:

	//
	// Shape Construction
	//
	hkpConvexVerticesShape* shape = HK_NULL;
	{	
		int stride = sizeof(float) * 4;
		hkArray<hkVector4> planeEquations;
		hkGeometry geom;

		hkStridedVertices stridedVerts;
		{
			stridedVerts.m_numVertices = g_numVertices;
			stridedVerts.m_striding = stride;
			stridedVerts.m_vertices = g_vertices;
		}

		hkGeometryUtility::createConvexGeometry( stridedVerts, geom, planeEquations );

		{
			stridedVerts.m_vertices = &(geom.m_vertices[0](0));
			stridedVerts.m_striding = hkSizeOf(hkVector4);
			stridedVerts.m_numVertices = geom.m_vertices.getSize();
		}
		shape = new hkpConvexVerticesShape( stridedVerts, planeEquations);
	}

	// As you can see from the above code snippets, the actually geometry that is passed to the two different shape
	// types is identical. The only difference is in the manner in which it is internally processed by Havok. Whereas
	// the Convex Vertices Shape uses the given vertices to directly compute the plane equations for collision detection,
	// the BV Tree shape uses a MOPP and the fit tolerance associated with it. Depending on the tightness of the fit the
	// number of triangles returned by both may differ.

	//
	// Create rigid body using convex vertices shape
	//
	{
		// To illustrate using the shape, create a rigid body by first defining a template.
		hkpRigidBodyCinfo rigidBodyInfo;

		rigidBodyInfo.m_shape = shape;
		rigidBodyInfo.m_position.set(0.0f, 0.0f, 0.0f);
		rigidBodyInfo.m_angularDamping = 0.0f;
		rigidBodyInfo.m_linearDamping = 0.0f;
		rigidBodyInfo.m_motionType = hkpMotion::MOTION_KEYFRAMED;

		// If we need to compute mass properties, we'll do this using the hkpInertiaTensorComputer.
		if (rigidBodyInfo.m_motionType != hkpMotion::MOTION_FIXED)
		{
			hkReal mass = 10.0f;
			hkpMassProperties massProperties;
			hkpInertiaTensorComputer::computeShapeVolumeMassProperties(shape, mass, massProperties);

			rigidBodyInfo.m_inertiaTensor = massProperties.m_inertiaTensor;
			rigidBodyInfo.m_centerOfMass = massProperties.m_centerOfMass;
			rigidBodyInfo.m_mass = massProperties.m_mass;			
		}	
			 
		// Create a rigid body (using the template above).
		m_rigidBody = new hkpRigidBody(rigidBodyInfo);

		// Finally add body so we can see it, we'll remove the reference when the demo finishes
		m_world->addEntity(m_rigidBody);

		HK_SET_OBJECT_COLOR((hkUlong)m_rigidBody->getCollidable(), hkColor::rgbFromChars(50, 50, 50));
	}
	
	// Remove reference since the body now "owns" the Shape.
	shape->removeReference();


	/// After creating the shapes we specify a radius for the sphere to be used during the sphere casting,

	m_radiusOfQuerySphere = 0.1f;

	m_frameCount = 0;

	// The size of the radius varies the 'accuracy' of the query; a small radius will only pick up triangles
	// that are very close to the intersection point, whereas a larger sphere will tend to pick up many
	// triangles in close proximity. 
	// If the 'actual' (closest) triangle is required, the one with minimum distance to the sphere centre 
	// could be chosen.

	m_world->unlock();
}


ConvexBvTreeQueryDemo::~ConvexBvTreeQueryDemo()
{
	m_world->lock();

	m_moppShape->removeReference();
	m_rigidBody->removeReference();
	delete m_generator;

	m_world->unlock();
}


hkDemo::Result ConvexBvTreeQueryDemo::stepDemo()
{
	m_world->lock();

	// Alternate between brute force triangle test and BV tree test
	static hkBool whichTest = false;

	// Initially we simply perform a world ray cast to see if the ray actually intersects with the object.

	//
	// Fire ray towards object
	//
	hkpWorldRayCastInput input;
	hkpWorldRayCastOutput output;

	input.m_from = g_fireFrom;
	input.m_to = m_rigidBody->getPosition();

	m_world->castRay( input, output );
	
	// Quit if no ray hit
	if( !output.hasHit() )
	{
		return hkDefaultPhysicsDemo::stepDemo();
	}

	//
	// Visualisation for the ray hit
	//
	hkVector4 intersectionPointWorld;
	{

		intersectionPointWorld.setInterpolate4(input.m_from, input.m_to, output.m_hitFraction );
		HK_DISPLAY_LINE( input.m_from, intersectionPointWorld, hkColor::RED);

		hkVector4 p = intersectionPointWorld;
		hkVector4 q = intersectionPointWorld;
		p(0) -= 1.0f;
		q(0) += 1.0f;		
		HK_DISPLAY_LINE(p, q, hkColor::RED);
		p = intersectionPointWorld;
		q = intersectionPointWorld;
		p(1) -= 1.0f;
		q(1) += 1.0f;	
		HK_DISPLAY_LINE(p, q, hkColor::RED);
	}


	//
	// Prepare source point for tests in Rigid Body LOCAL space
	//
	hkVector4 testPt;
    {
		// Move point back along normal to slightly less than radius
		hkVector4 rayDir;
		rayDir.setSub4( input.m_to,  input.m_from );

		hkVector4 normalEnd;
		normalEnd.setAdd4( output.m_normal, intersectionPointWorld );
		HK_DISPLAY_LINE( intersectionPointWorld, normalEnd, hkColor::YELLOW );

		// Once we've established that an intersection point exists we can then continue on with the shape level tests.
		// The first thing we must do is to prepare the source point for test in the objects local space, this is acheieved
		// with the following code:

		testPt = intersectionPointWorld;
		testPt.addMul4( m_radiusOfQuerySphere * 0.99f, output.m_normal );

		const hkTransform& tr = m_rigidBody->getTransform();
		testPt.setTransformedInversePos( tr, testPt );
	}


	{
		hkString s;
		if(whichTest)
		{
			s += hkString(" Using a sphere query on precalculated MOPP bounding volume data.");
		}
		else
		{ 
			s += hkString(" Brute force iteration over all triangles on the object.");
		}
		m_env->m_textDisplay->outputText(s, 10, 280);
		s = hkString(" Bring up Statistics to see relative timings of methods.");
		m_env->m_textDisplay->outputText(s, 10, 300);
	}

	//
	// Do Tests
	//
	hkSphere test( testPt, m_radiusOfQuerySphere );
	{
			
		//
		// Do BV Tree test - 'hits' contains the triangles returned
		//
		hkArray< hkpShapeKey > intersectingTris;
		int resultColor;

		if( whichTest )
		{
			HK_TIMER_BEGIN( "BV Tree test", HK_NULL );

			// The demo alternates between two different modes; one using the BV Tree Shape and the other using the Convex Vertices
			// Shape to test for intersection. We'll tackle the BV Tree Shape approach first:

			hkVector4 r; r.setAll(m_radiusOfQuerySphere);
			hkAabb aabb; 
			aabb.m_min.setSub4( testPt, r );
			aabb.m_max.setAdd4( testPt, r );

			m_moppShape->queryAabb( aabb, intersectingTris );

			HK_TIMER_END();
			resultColor = hkColor::rgbFromChars( 10, 100, 10 );
		}

		//
		// Brute force triangle test
		//
		else	
		{
			HK_TIMER_BEGIN( "Brute Force Sphere test", HK_NULL );


			// The call to querySphere(...) will return the hkpShapeKey for all shapes in the hkpShapeCollection that intersect with the sphereQuery.
			// Once we have these we simply iterate through them and push them back onto our list of intersecting triangles. If, however, the demo
			// is in the brute force mode the following code will be executed:

			// For each tri, test each vertex against sphere
			hkReal radiusSqrd = m_radiusOfQuerySphere * m_radiusOfQuerySphere; 
			hkpCollideTriangleUtil::ClosestPointTriangleCache cache;
			hkpCollideTriangleUtil::ClosestPointTriangleResult result;
			

			hkpTriangleShape tShape;

			// For each triangle, do a closest distance test
			for( int tri = 0; tri < g_numTris; tri++ )
			{
				buildTriangle( tri, tShape );

				// Test point against triangle
				hkpCollideTriangleUtil::setupClosestPointTriangleCache( tShape.getVertices(), cache );
				hkpCollideTriangleUtil::closestPointTriangle( testPt, tShape.getVertices(), cache, result );

				hkVector4 testvec;
				result.calcClosestPoint( testPt, testvec);

				// Here we check against the radius and append to a list of intersecting tris.
				if( ( result.distance * result.distance ) < radiusSqrd )
				{
					intersectingTris.pushBack( tri );
				}
			}

			// This method needs to construct each triangle 'on-the-fly' as the Convex Vertices shape does not store a list of triangles. Thus we
			// need to create the triangle and test to see if the intersection point lies within the sphere radius (using the squares of each for
			// speed purposes). If it is found to lie within this distance it is pushed back onto our list of intersecting triangles for displaying
			// later.

			HK_TIMER_END();
			resultColor = hkColor::BLUE;
		}

		// Display results
		for( int k = 0; k < intersectingTris.getSize(); k++ )
		{
			displayTriangle( intersectingTris[k], m_rigidBody->getTransform(), resultColor );
		}
	}


	if( ++m_frameCount == 250 )
	{
		// Use other test
		whichTest = !whichTest;
		m_frameCount = 0;

		// Once one mode of the demo has completed it simply switches over the the alternate mode and randomly orientates
		// the object before starting over.

		//
		// Randomly rotate object
		//
		if( !whichTest )
		{
			hkVector4 axis(m_generator->getRandRange(-1.0f, 1.0f), m_generator->getRandRange(-1.0f, 1.0f), m_generator->getRandRange(-1.0f, 1.0f));
			axis.normalize3();
			hkQuaternion q;
			q.setAxisAngle(axis, m_generator->getRandRange(0.0f, HK_REAL_PI));
			m_rigidBody->setRotation( q );
		}
	}
	
	m_world->unlock();

	return hkDefaultPhysicsDemo::stepDemo();
}


// Populate the vertices of the given triangle shape with the verts
// pointed to by the specified indices.

void ConvexBvTreeQueryDemo::buildTriangle( unsigned int tri, hkpTriangleShape& shape )
{
	int vertIndex = g_tris[tri*3]*4;
	hkVector4 v1( g_vertices[ vertIndex ], g_vertices[ vertIndex+1 ], g_vertices[ vertIndex+2 ] );

	vertIndex = g_tris[tri*3+1]*4;
	hkVector4 v2( g_vertices[ vertIndex ], g_vertices[ vertIndex+1 ], g_vertices[ vertIndex+2 ] );

	vertIndex = g_tris[tri*3+2]*4;
	hkVector4 v3( g_vertices[ vertIndex ], g_vertices[ vertIndex+1 ], g_vertices[ vertIndex+2 ] );

	shape.setVertex(0, v1);
	shape.setVertex(1, v2);
	shape.setVertex(2, v3);
}


void ConvexBvTreeQueryDemo::displayTriangle( unsigned int tri, const hkTransform& tr, unsigned int c ) const
{
	int vertIndex = g_tris[tri*3]*4;
	hkVector4 v1( g_vertices[ vertIndex ], g_vertices[ vertIndex+1 ], g_vertices[ vertIndex+2 ] );

	vertIndex = g_tris[tri*3+1]*4;
	hkVector4 v2( g_vertices[ vertIndex ], g_vertices[ vertIndex+1 ], g_vertices[ vertIndex+2 ] );

	vertIndex = g_tris[tri*3+2]*4;
	hkVector4 v3( g_vertices[ vertIndex ], g_vertices[ vertIndex+1 ], g_vertices[ vertIndex+2 ] );

	v1.setTransformedPos( tr, v1 );
	v2.setTransformedPos( tr, v2 );
	v3.setTransformedPos( tr, v3 );

	// Move display lines away from surface so triangles are displayed clearly
	hkVector4 normal;
	hkpTriangleUtil::calcNormal( normal, v1, v2, v3 );
	normal.normalize4();

	v1.addMul4( 0.1f, normal );
	v2.addMul4( 0.1f, normal );
	v3.addMul4( 0.1f, normal );

	HK_DISPLAY_LINE( v1, v2, c );
	HK_DISPLAY_LINE( v2, v3, c );
	HK_DISPLAY_LINE( v3, v1, c );
}


#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
"This demo shows a bounding volume tree (hkpMoppBvTreeShape) being " \
"created around a convex geometry. This allows OBB, AABB or sphere queries " \
"against the convex geometry. For comparison a brute force test is also " \
"performed which iterates through each triangle and tests whether it " \
"intersects with the sphere being queried. " \
"Each test is done on the same shape. Results of the brute force test " \
"are shown in blue for 50 frames, then results of the BV tree test are " \
"shown in green for 50 frames. The shape is then rotated and the " \
"tests re-run." ;

HK_DECLARE_DEMO(ConvexBvTreeQueryDemo, HK_DEMO_TYPE_PRIME, "Two methods to identify triangles hit by a convex shape raycast", helpString);

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
