/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


#include <Demos/demos.h>
#include <Demos/Physics/Api/Collide/RayCasting/ShapeRayCast/ShapeRaycastDemo.h>
#include <Common/Base/Algorithm/PseudoRandom/hkPseudoRandomGenerator.h>
#include <Physics/Dynamics/World/hkpWorld.h>

// We will need these shapes
#include <Physics/Collide/Shape/Compound/Tree/Mopp/hkpMoppBvTreeShape.h>
#include <Physics/Collide/Shape/Compound/Collection/SimpleMesh/hkpSimpleMeshShape.h>
#include <Physics/Collide/Shape/Convex/Triangle/hkpTriangleShape.h>
#include <Physics/Collide/Shape/Convex/Capsule/hkpCapsuleShape.h>
#include <Physics/Collide/Shape/HeightField/Plane/hkpPlaneShape.h>
#include <Physics/Collide/Shape/Misc/Transform/hkpTransformShape.h>
#include <Physics/Collide/Shape/HeightField/SampledHeightField/hkpSampledHeightFieldShape.h>
#include <Physics/Collide/Shape/HeightField/SampledHeightField/hkpSampledHeightFieldBaseCinfo.h>
#include <Physics/Collide/Shape/Convex/Cylinder/hkpCylinderShape.h>
#include <Physics/Collide/Shape/Convex/ConvexTranslate/hkpConvexTranslateShape.h>
#include <Physics/Collide/Shape/Convex/ConvexTransform/hkpConvexTransformShape.h>
#include <Physics/Collide/Shape/Query/hkpRayHitCollector.h>
#include <Physics/Collide/Shape/Compound/Collection/List/hkpListShape.h>

#include <Common/Visualize/hkDebugDisplay.h>
#include <Graphics/Common/Camera/hkgCamera.h>

#include <Common/Visualize/Shape/hkDisplayGeometry.h>

// Used to create the Mopp 'code' object
#include <Physics/Collide/Shape/Compound/Tree/Mopp/hkpMoppUtility.h>

// Used to generate a convex hull for convexVerticesShape
#include <Common/Internal/ConvexHull/hkGeometryUtility.h>


// We need to display the results of the raycast to confirm correctness.
#include <Common/Visualize/hkDebugDisplay.h>

// for our raycast
#include <Physics/Collide/Shape/Query/hkpShapeRayCastInput.h>
#include <Physics/Collide/Shape/Query/hkpShapeRayCastOutput.h>

namespace
{
	struct SimpleHeightFieldShape: public hkpSampledHeightFieldShape
	{
		SimpleHeightFieldShape( const hkpSampledHeightFieldBaseCinfo& ci )
			: hkpSampledHeightFieldShape(ci)
		{
		}

		HK_FORCE_INLINE hkReal getHeightAtImpl( int x, int z ) const
		{
			x = x&1 ? -x : x;
			z = z&1 ? -z : z;
			return 1 * (hkReal(x)/m_xRes + hkReal(z)/m_zRes);
		}

		HK_FORCE_INLINE hkBool getTriangleFlipImpl() const
		{	
			return false;
		}

		virtual void collideSpheres( const CollideSpheresInput& input, SphereCollisionOutput* outputArray) const
		{
			hkSampledHeightFieldShape_collideSpheres(*this, input, outputArray);
		}
	};

	struct RayHitPoint
	{
		hkReal hitFraction;
		hkVector4 normal;
		int extraInfo;
		hkpShapeKey keys[hkpShapeRayCastOutput::MAX_HIERARCHY_DEPTH];
	};

	struct AllHitsCollector : public hkpRayHitCollector
	{
		AllHitsCollector( hkArray<RayHitPoint>& hits ) : m_hits(hits) {}

		virtual void addRayHit( const hkpCdBody& cdBody, const hkpShapeRayCastCollectorOutput& output )
		{
			RayHitPoint& hit = m_hits.expandOne();
			hit.hitFraction = output.m_hitFraction;
			hit.normal = output.m_normal;
			hit.extraInfo = output.m_extraInfo;
			shapeKeysFromCdBody(hit.keys, sizeof(hit.keys)/sizeof(hkpShapeKey), cdBody);
		}

		hkArray<RayHitPoint>& m_hits;
	};
}


ShapeRaycastDemo::ShapeRaycastDemo(hkDemoEnvironment* env)
	:	hkDefaultPhysicsDemo(env, DEMO_FLAGS_NO_SERIALIZE), m_planeDisp(HK_NULL), m_time(0.0f), m_castMode(CAST_RAY_SINGLE), m_simulationPaused(false)
{
	// Disable backface culling
	setGraphicsState(HKG_ENABLED_CULLFACE, false);
	
	// Setup the camera.
	{
		hkVector4 from(0.0f, 5.0f, 50.0f);
		hkVector4 to  (2.0f, 0.0f, 0.0f);
		hkVector4 up  (0.0f, 1.0f, 0.0f);
		setupDefaultCameras(env, from, to, up);
	}

	// Create the world.
	{
		hkpWorldCinfo info;
		
		// Set gravity to zero so body floats.
		info.setBroadPhaseWorldSize( 100.0f );
		m_world = new hkpWorld(info);
		m_world->lock();

		m_world->setGravity(hkVector4::getZero());

		setupGraphics();
	}

	hkpAgentRegisterUtil::registerAllAgents(m_world->getCollisionDispatcher());

	createBodies(m_bodies, m_world->getCollisionInput());

	int lightGrey = hkColor::rgbFromChars(128,128,128);
	for ( int i =0; i< m_bodies.getSize(); i++ )
	{
		m_world->addEntity( m_bodies[i] );
		HK_SET_OBJECT_COLOR((hkUlong)m_bodies[i]->getCollidable(), lightGrey);
	}

	m_world->unlock();
}

ShapeRaycastDemo::~ShapeRaycastDemo()
{
	m_world->lock();

	// Tidy up the geometry
	hkDebugDisplay::getInstance().removeGeometry( (hkUlong)m_planeDisp, 0, 0);
	delete m_planeDisp;

	for ( int i =0; i< m_bodies.getSize(); i++ )
	{
		m_bodies[i]->removeReference();
	}

	m_world->unlock();
}

void ShapeRaycastDemo::makeFakeInput()
{
	// Changes shapes every frame
	 m_env->m_gamePad->forceButtonPressed(HKG_PAD_BUTTON_2);

	 // Change cast modes after we go through each shape
	 if( (m_physicsStepCounter % m_bodies.getSize()) == 0)
	 {
		 m_env->m_gamePad->forceButtonPressed(HKG_PAD_BUTTON_1);
	 }

}

hkDemo::Result ShapeRaycastDemo::stepDemo()
{
	m_world->lock();

	m_time += m_timestep;

	// All rays originate from the same start point in world space with the destination point of each
	// ray being swept up and down across the surface of each object. This is achieved using a simple
	// sinus variation for the height based on the current time:

	hkReal height = 4.0f * hkMath::sin(m_time);
	height = 0;

	// For each body cast a ray TOWARDS ONLY THAT BODY and draw results
	//for(int bodyIndex = 0; bodyIndex < m_bodies.getSize(); ++bodyIndex)
	static int bodyIndex = 7;
	{
		const hkpRigidBody* body = m_bodies[bodyIndex];
		// We initially specify the ray start and end points in World Space and then proceed to transform
		// these to the relevant Local Space for each object and cast the ray:

		// Start and end of ray in World space
		hkVector4 startWorld(0.0f, 0.0f, 15.0f);
		hkVector4 endWorld = body->getTransform().getTranslation();
		{
			hkVector4 dir; dir.setSub4(endWorld, startWorld);
			dir.mul4(1.5f);
			endWorld.add4( dir );
		}
		endWorld(1) += height;

		const hkpShape* topShape = body->getCollidable()->getShape();

		hkpShapeRayCastInput input;
		
		input.m_from.setTransformedInversePos(body->getTransform(), startWorld);
		input.m_to.setTransformedInversePos(body->getTransform(), endWorld);

		hkInplaceArray<RayHitPoint,hkpShapeRayCastOutput::MAX_HIERARCHY_DEPTH> hits;

		if( m_castMode == CAST_RAY_SINGLE )
		{
			hkpShapeRayCastOutput output;
			if( topShape->castRay(input, output ) == true )
			{
				RayHitPoint& hit = hits.expandOne();
				hit.hitFraction = output.m_hitFraction;
				// Convert from local to world space
				// When using the castRay interface, the shape does not know which cdbody it
				// is connected to so we must manually transform from shape space.
				hit.normal.setRotatedDir( body->getTransform().getRotation(), output.m_normal );
				hit.normal.normalize3();
				hit.extraInfo = output.m_extraInfo;
				for( int i = 0; i < hkpShapeRayCastOutput::MAX_HIERARCHY_DEPTH; ++i )
				{
					hit.keys[i] = output.m_shapeKeys[i];
				}
			}			
		}
		else // if( m_castMode == CAST_RAY_COLLECTOR )
		{
			// When using the collector interface, the normal is in the correct space as
			// it uses the transform from the cdbody.
			AllHitsCollector collector( hits );
			topShape->castRayWithCollector(input, *body->getCollidable(), collector );
		}

		// To visualise the raycast we make use of a macro defined in "hkDebugDisplay.h" called HK_DISPLAY_LINE.
		// The macro takes three parameters: a start point, an end point and the line color.
		// If a hit is found we display a RED line from the raycast start point to the point of intersection and mark that
		// point with a small RED cross. The intersection point is calculated using: startWorld + (result.m_mindist * endWorld).
		// A short CYAN line is also displayed to show the hit normal (normalized).
		// If no hit is found we simply display a GREY line between the raycast start and end points.

		for(int hitIndex = 0; hitIndex < hits.getSize(); ++hitIndex )
		{
			RayHitPoint& hit = hits[hitIndex];

			hkVector4 intersectionPointWorld;
			intersectionPointWorld.setInterpolate4(startWorld, endWorld, hit.hitFraction );
			HK_DISPLAY_LINE(startWorld, intersectionPointWorld, hkColor::RED);

			// Display a small cross at the point of intersection
			hkVector4 p = intersectionPointWorld;
			hkVector4 q = intersectionPointWorld;
			p(0) -= 0.2f;
			q(0) += 0.2f;		
			HK_DISPLAY_LINE(p, q, hkColor::RED);
			p = intersectionPointWorld;
			q = intersectionPointWorld;
			p(1) -= 0.2f;
			q(1) += 0.2f;	
			HK_DISPLAY_LINE(p, q, hkColor::RED);

			HK_DISPLAY_ARROW( intersectionPointWorld, hit.normal, hkColor::CYAN );

			char buf[256];
			buf[0] = 0;
			int cur = 0;
			const hkpShape* s = topShape;
			hkpShapeContainer::ShapeBuffer buffer;
			for(int keyIndex = 0; keyIndex==0 || hit.keys[keyIndex-1] != HK_INVALID_SHAPE_KEY; ++keyIndex )
			{
				cur += hkString::snprintf(&buf[cur], sizeof(buf)-cur, "%s(%i)\n", hkGetShapeTypeName(s->getType()), hit.keys[keyIndex] );
				const hkpShapeContainer* container = s->getContainer();
				if( container )	s = container->getChildShape(hit.keys[keyIndex], buffer);
			}
			cur += hkString::snprintf(&buf[cur], sizeof(buf)-cur, "extra(%i)", hit.extraInfo );
			{
				// Move rendering point for 3d text towards camera to avoid being obscured by shape rendering!
				hkVector4 textPosition = intersectionPointWorld;
				hkgWindow* w = m_env->m_window;
				hkgViewport* v = w->getCurrentViewport();
				hkgCamera* c = v->getCamera();
				hkVector4 from(c->getFromPtr()[0], c->getFromPtr()[1], c->getFromPtr()[2]);
				hkVector4 dir;	dir.setSub4(textPosition, from);	dir.normalize3();
				textPosition.addMul4( -4.0f, dir);
				m_env->m_textDisplay->outputText3D(buf, textPosition(0), textPosition(1), textPosition(2) );
			}
		}
		if( hits.getSize() == 0 )
		{
			// Otherwise draw as GREY
			HK_DISPLAY_LINE(startWorld, endWorld, hkColor::rgbFromChars(200, 200, 200));
		}
	}

	char buf[256];
	hkString::snprintf(buf, sizeof(buf), "Using hkpShape::castRay%s\nPress \221 to toggle modes\nPress \222 to move ray to next shape\nPress \223 topause/unpause simulation\n",
		m_castMode==CAST_RAY_SINGLE ? "" : "WithCollector");
	m_env->m_textDisplay->outputText(buf, 20, 340);

	if( m_env->m_gamePad->wasButtonPressed(HKG_PAD_BUTTON_1) )
	{
		m_castMode = CastMode(!m_castMode);
	}
	if( m_env->m_gamePad->wasButtonPressed(HKG_PAD_BUTTON_2) )
	{
		bodyIndex = (bodyIndex+1) % m_bodies.getSize();
	}
	if( m_env->m_gamePad->wasButtonPressed(HKG_PAD_BUTTON_3) )
	{
		m_simulationPaused = !m_simulationPaused;
	}

	
	m_world->unlock();

	if(!m_simulationPaused)
	{
		return hkDefaultPhysicsDemo::stepDemo();
	}
	else
	{
		return DEMO_OK;
	}
}


// In this demo the raycast is performed against a variety of shape types; MOPP, Convex Vertices, Box, Sphere and Triangle.
// The construction of each of these is quite similar and for the purposes of this tutorial we will just outline
// that of the box object.

void ShapeRaycastDemo::createBodies(hkArray<hkpRigidBody*>&	bodiesOut, const hkpProcessCollisionInput* collisionInput )
{
	// Use a common base hkpRigidBodyCinfo for all bodies.

	hkpRigidBodyCinfo rigidBodyInfo;

	rigidBodyInfo.m_angularDamping = 0;
	rigidBodyInfo.m_angularVelocity.set(.01f, .05f, .1f);
	rigidBodyInfo.m_angularVelocity.mul4(5);
	rigidBodyInfo.m_motionType = hkpMotion::MOTION_DYNAMIC;

	const hkReal SHIFT_AMOUNT = 6.0f;
	rigidBodyInfo.m_position.set(-(6/2-0.5f)*SHIFT_AMOUNT, 1.0f + SHIFT_AMOUNT, 0.0f);
	hkVector4 shift; shift.set( SHIFT_AMOUNT, 0.0f, 0.0f );

	//
	//	Create Capsule
	//
	{
		hkVector4 A( 0.f, 1.2f, 0.f);
		hkVector4 B( 0.f,-1.2f, 0.f);
		hkReal radius = 1.0f;

		hkpCapsuleShape* shape = new hkpCapsuleShape(A, B, radius);
		rigidBodyInfo.m_shape = shape;

		bodiesOut.pushBack(new hkpRigidBody(rigidBodyInfo));
		shape->removeReference();
		rigidBodyInfo.m_position.add4( shift );
	}

	//
	//	Create Cylinder
	//
	{
		hkVector4 A( 0.f, 1.2f, 0.f);
		hkVector4 B( 0.f,-1.2f, 0.f);
		hkReal radius = 1.0f;

		hkpCylinderShape* shape = new hkpCylinderShape(A, B, radius);

		rigidBodyInfo.m_shape = shape;
		rigidBodyInfo.m_rotation = hkQuaternion( hkVector4(1.0f, 0.0f, 0.0f), HK_REAL_PI / 6.f);

		bodiesOut.pushBack(new hkpRigidBody(rigidBodyInfo));
		shape->removeReference();
		rigidBodyInfo.m_position.add4( shift );
	}

	//
	// Create MOPP body
	//
 	{
		hkpMoppBvTreeShape* shape = createMoppShape();
		rigidBodyInfo.m_shape = shape;

		bodiesOut.pushBack(new hkpRigidBody(rigidBodyInfo));
		shape->removeReference();
		rigidBodyInfo.m_position.add4( shift );
	}



	//
	// Create ConvexVertices body
	//
	{
		// Data specific to this shape.
		int numVertices = 4;

		// 16 = 4 (size of "each float group", 3 for x,y,z, 1 for padding) * 4 (size of float)
		int stride = hkSizeOf(hkVector4);

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

		bodiesOut.pushBack(new hkpRigidBody(rigidBodyInfo));
		shape->removeReference();
		rigidBodyInfo.m_position.add4( shift );
	}

	// We then proceed to fill in the various details of the hkpRigidBodyCinfo 'blueprint' for this body.
	// Note the reference removal for the shape and the rigid body, this is done as new references are added by the
	// rigid body itself to the shape and by the world to the rigid body. Havok is now responsible for these
	// objects and will delete them when appropriate.

	//
	//	Create Box
	//
	{
		// Data specific to this shape.
		hkVector4 oneOneOne = hkVector4(1.0f, 1.0f, 1.0f);
		hkpBoxShape* boxshape = new hkpBoxShape(oneOneOne, 0 );
		//hkpShape* shape = new hkpConvexTranslateShape(boxshape, oneOneOne);
		hkTransform trans;
		hkPseudoRandomGenerator prg(0);
		prg.getRandomRotation(trans.getRotation());
		prg.getRandomVector11(trans.getTranslation());
		hkpShape* shape = new hkpConvexTransformShape(boxshape, trans);
		boxshape->removeReference();

		rigidBodyInfo.m_shape = shape;

		bodiesOut.pushBack(new hkpRigidBody(rigidBodyInfo));
		shape->removeReference();
		rigidBodyInfo.m_position.add4( shift );
	}

	rigidBodyInfo.m_position(0) -= 6*SHIFT_AMOUNT;
	rigidBodyInfo.m_position(1) -= SHIFT_AMOUNT;

	//
	// Create Sphere
	//
	{
		hkReal radius = 1.5f;
		hkpConvexShape* shape = new hkpSphereShape(radius);
		rigidBodyInfo.m_shape = shape;

		bodiesOut.pushBack(new hkpRigidBody(rigidBodyInfo));
		shape->removeReference();
		rigidBodyInfo.m_position.add4( shift );
	}

	//
	// Create Triangle body
	//
	{
		hkVector4 a(-1.5f, -1.5f,  0.0f);
		hkVector4 b(1.5f, -1.5f,  0.0f);
		hkVector4 c(0.0f,  1.5f,  0.0f);

		hkpTriangleShape* shape = new hkpTriangleShape(a, b, c);

		rigidBodyInfo.m_shape = shape;

		bodiesOut.pushBack(new hkpRigidBody(rigidBodyInfo));
		shape->removeReference();
		rigidBodyInfo.m_position.add4( shift );
	}
	
	//
	// Create Plane body
	//
	{
		hkVector4 dir( 0,0,1);
		hkVector4 point( 0,0,0);
		hkVector4 extents( 1.5f, 1.5f, 1.5f);

		hkpPlaneShape* shape = new hkpPlaneShape(dir, point, extents);

		rigidBodyInfo.m_shape = shape;

		bodiesOut.pushBack(new hkpRigidBody(rigidBodyInfo));
		shape->removeReference();
		rigidBodyInfo.m_position.add4( shift );
	}


	{
		hkReal radius = 1.5f;
		hkpConvexShape* shape = new hkpSphereShape(radius);

		rigidBodyInfo.m_shape = shape;

		bodiesOut.pushBack(new hkpRigidBody(rigidBodyInfo));
		shape->removeReference();
		rigidBodyInfo.m_position.add4( shift );
	}


	//
	// Create a heightfield
	//
	{
		hkpSampledHeightFieldBaseCinfo ci;
		ci.m_xRes = 6;
		ci.m_zRes = 6;

		hkpHeightFieldShape* hfshape = new SimpleHeightFieldShape( ci );

		// We center the heightfield using a transform shape.
		// We do this, because it simplifies our demo.
		// However we recommend updating the rigid body position correctly
			hkTransform transform;
			transform.setIdentity();
		transform.getTranslation().set( -0.5f * ci.m_xRes, 0, 0.5f * -ci.m_zRes);
		hkpShape* shape = new hkpTransformShape( hfshape, transform );
			hfshape->removeReference();

		rigidBodyInfo.m_shape = shape;

		// We disable the warning about using transform shapes
		// while we create the rigid body
		hkError::getInstance().setEnabled(0x2ff8c16f, false);
		bodiesOut.pushBack(new hkpRigidBody( rigidBodyInfo ));
		hkError::getInstance().setEnabled(0x2ff8c16f, true);
		shape->removeReference();
		rigidBodyInfo.m_position.add4( shift );
	}

	rigidBodyInfo.m_position(0) -= 6*SHIFT_AMOUNT;
	rigidBodyInfo.m_position(1) -= SHIFT_AMOUNT;

	//
	// Create a listshape
	//
	{
		hkArray<hkpShape*> shapes;
		{
			hkVector4 A( 1.0f, 0.2f, 1.4f);
			hkVector4 B( -1.0f,-0.2f, 1.4f);
			hkReal radius = 0.6f;

			hkpCapsuleShape* shape = new hkpCapsuleShape(A, B, radius);
			shapes.pushBack(shape);
		}
		{
			hkVector4 halfExtents( 0.5f, 0.6f, 1.5f);

			hkpBoxShape* shape = new hkpBoxShape(halfExtents);
			shapes.pushBack(shape);
		}
		hkpListShape* list = new hkpListShape(shapes.begin(), shapes.getSize());
		shapes[0]->removeReference();
		shapes[1]->removeReference();
		rigidBodyInfo.m_shape = list;

		bodiesOut.pushBack(new hkpRigidBody(rigidBodyInfo));
		list->removeReference();
		rigidBodyInfo.m_position.add4( shift );
	}
}

void createMeshShape( const int side, hkpSimpleMeshShape* meshShape )
{
	{
		meshShape->m_vertices.setSize( side * side );
		for(int i = 0; i < side; i++)
		{
			for (int j = 0; j < side; j++ )
			{
				hkVector4 vertex ( i * 1.0f - side * 0.5f,
					j * 1.0f - side * 0.5f,
					0.6f * hkMath::cos((hkReal)j + i) + 0.3f * hkMath::sin( 2.0f * i) );
				meshShape->m_vertices[i*side + j] = vertex ;
			}
		}
	}

	{
		meshShape->m_triangles.setSize( (side-1) * (side-1) * 2);
		int corner = 0;
		int curTri = 0;
		for(int i = 0; i < side - 1; i++)
		{
			for (int j = 0; j < side - 1; j++ )
			{
				meshShape->m_triangles[curTri].m_a = corner;
				meshShape->m_triangles[curTri].m_b = corner+side;
				meshShape->m_triangles[curTri].m_c = corner+1;
				curTri++;

				meshShape->m_triangles[curTri].m_a = corner+1;
				meshShape->m_triangles[curTri].m_b = corner+side;
				meshShape->m_triangles[curTri].m_c = corner+side+1;
				curTri++;
				corner++; 
			}
			corner++; 
		}
	}
}

hkpMoppBvTreeShape* ShapeRaycastDemo::createMoppShape()
{
	const int side = 5;

	//
	//	We use a storage mesh in our example, which copies all data.
	//  If you want to share graphics and physics, use the hkpMeshShape instead
	//
	hkpSimpleMeshShape* meshShape = new hkpSimpleMeshShape( 0.05f /*radius*/);

	createMeshShape( side, meshShape );

	hkpMoppCompilerInput mci;
	hkpMoppCode* code = hkpMoppUtility::buildCode( meshShape ,mci);

	hkpMoppBvTreeShape* moppShape = new hkpMoppBvTreeShape(meshShape, code);
	code->removeReference();
	meshShape->removeReference();

	return moppShape;
}


#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
"A demo which shows use of the hkpShape raycast, which determines the closest point of intersection " \
"between a ray (finite, given start and end points) and a hkpShape, if it exists. " \
"In addition, the normal at the point of intersection is calculated. " \
"In this demo the ray is swept over time to illustrate the correctness of the raycast. " \
"The hit point is shown in red. The normal is shown in cyan.";

HK_DECLARE_DEMO(ShapeRaycastDemo, HK_DEMO_TYPE_PRIME, "Using Shape raycasts. Ray is clipped to first intersection point.", helpString);

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
