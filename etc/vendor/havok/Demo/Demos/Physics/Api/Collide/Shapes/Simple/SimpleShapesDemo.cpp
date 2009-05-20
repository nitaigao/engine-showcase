/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/Physics/Api/Collide/Shapes/Simple/SimpleShapesDemo.h>

#include <Physics/Collide/Shape/Misc/Bv/hkpBvShape.h>
#include <Physics/Collide/Shape/Convex/Triangle/hkpTriangleShape.h>
#include <Physics/Collide/Shape/Convex/Cylinder/hkpCylinderShape.h>
#include <Physics/Collide/Shape/Convex/Capsule/hkpCapsuleShape.h>
#include <Common/Internal/ConvexHull/hkGeometryUtility.h>


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Definitions of variants
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct ShapeVariant
{
	const char*	m_name;
	hkpShapeType m_shapeType;
	const char* m_details;
};


static const ShapeVariant g_variants[] =
{
	{ "Box", HK_SHAPE_BOX, "A single Box shape" },
	{ "Sphere", HK_SHAPE_SPHERE, "A single Sphere shape" },
	{ "Triangle", HK_SHAPE_TRIANGLE, "A single Triangle shape" },
	{ "Capsule", HK_SHAPE_CAPSULE, "A single Capsule shape" },
	{ "Cylinder", HK_SHAPE_CYLINDER, "A single Cylinder shape. There is currently an issue with cylinders rolling on their sides. See reference manual for details." },
	{ "Convex Vertices", HK_SHAPE_CONVEX_VERTICES, "A single ConvexVertices shape" }
};



SimpleShapesDemo::SimpleShapesDemo(hkDemoEnvironment* env)
:	hkDefaultPhysicsDemo(env)
{
	const ShapeVariant& variant =  g_variants[m_variantId];

	// Setup the camera.
	{
		hkVector4 from(0.0f, 5.0f, 10.0f);
		hkVector4 to  (0.0f, 0.0f,  0.0f);
		hkVector4 up  (0.0f, 1.0f,  0.0f);
		setupDefaultCameras(env, from, to, up);
	}

	// Create the world, setting gravity to zero so body floats.
	hkpWorldCinfo info;
	info.m_gravity.set(0.0f, 0.0f, 0.0f);	
	info.setBroadPhaseWorldSize( 100.0f );
	m_world = new hkpWorld(info);
	m_world->lock();

	setupGraphics();

	// Create the shape variant
	hkpShape* shape = 0;
	switch (variant.m_shapeType)
	{
		// Box
	case HK_SHAPE_BOX:
		{
			// Data specific to this shape.
			hkVector4 halfExtents(1.0f, 1.0f, 1.0f);

			/////////////////// SHAPE CONSTRUCTION ////////////////
			shape = new hkpBoxShape(halfExtents, 0 );

			break;
		}


		// Sphere
	case HK_SHAPE_SPHERE:
		{
			// The box is of side 2, so we must bound it by a sphere of radius >= sqrt(3)
			hkReal radius = 1.75f;

			/////////////////// SHAPE CONSTRUCTION ////////////////
			shape = new hkpSphereShape(radius);

			break;
		}


		// Triangle
	case HK_SHAPE_TRIANGLE:
		{
			// Disable face culling
			setGraphicsState(HKG_ENABLED_CULLFACE, false);

			float vertices[] = {
				-0.5f, -0.5f,  0.0f, 0.0f, // v0
				0.5f, -0.5f,  0.0f, 0.0f, // v1
				0.0f,  0.5f,  0.0f, 0.0f, // v2
			};

			/////////////////// SHAPE CONSTRUCTION ////////////////
			shape = new hkpTriangleShape();

			int index = 0;
			for (int i = 0; i < 3; i++)
			{
				static_cast<hkpTriangleShape*>(shape)->setVertex(i, hkVector4(vertices[index], vertices[index + 1], vertices[index + 2]));
				index = index + 4;
			}

			break;
		}


		// Capsule
	case HK_SHAPE_CAPSULE:
		{
			hkReal radius = 1.5f;
			hkVector4 top(0.0f, 1.5f, 0.0f);
			hkVector4 bottom(0.0f, -1.0f, 0.0f);

			/////////////////// SHAPE CONSTRUCTION ////////////////
			shape = new hkpCapsuleShape(top, bottom, radius);

			break;
		}


		// Cylinder
	case HK_SHAPE_CYLINDER:
		{
			hkReal radius = 1.5f;
			hkVector4 top(0.0f, 1.5f, 0.0f);
			hkVector4 bottom(0.0f, -1.0f, 0.0f);

			/////////////////// SHAPE CONSTRUCTION ////////////////
			shape = new hkpCylinderShape(top, bottom, radius);

			break;
		}


		// Convex vertices
	case HK_SHAPE_CONVEX_VERTICES:
		{
			// Data specific to this shape.
			int numVertices = 4;

			// 16 = 4 (size of "each float group", 3 for x,y,z, 1 for padding) * 4 (size of float)
			int stride = sizeof(float) * 4;

			float vertices[] = { // 4 vertices plus padding
				-2.0f, 2.0f, 1.0f, 0.0f, // v0
				1.0f, 3.0f, 0.0f, 0.0f, // v1
				0.0f, 1.0f, 3.0f, 0.0f, // v2
				1.0f, 0.0f, 0.0f, 0.0f  // v3
			};

			/////////////////// SHAPE CONSTRUCTION ////////////////
			hkArray<hkVector4> planeEquations;
			hkGeometry geom;

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

			break;
		}

	default:
		break;
	}

	// Make sure that a shape was created
	HK_ASSERT(0, shape);

	// To illustrate using the shape, first define a rigid body template.
	hkpRigidBodyCinfo rigidBodyInfo;
	rigidBodyInfo.m_position.set(0.0f, 0.0f, 0.0f);
	rigidBodyInfo.m_angularDamping = 0.0f;
	rigidBodyInfo.m_linearDamping = 0.0f;

	rigidBodyInfo.m_shape = shape;

	// Compute the rigid body inertia.
	rigidBodyInfo.m_motionType = hkpMotion::MOTION_BOX_INERTIA;
	hkpInertiaTensorComputer::setShapeVolumeMassProperties( rigidBodyInfo.m_shape, 100.0f, rigidBodyInfo );

	// Create a rigid body (using the template above).
	hkpRigidBody* rigidBody = new hkpRigidBody(rigidBodyInfo);

	// Remove reference since the body now "owns" the Shape.
	shape->removeReference();

	// Finally add body so we can see it, and remove reference since the world now "owns" it.
	m_world->addEntity(rigidBody);
	rigidBody->removeReference();

	m_world->unlock();
}


#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = "Examples of various simple shape types";

HK_DECLARE_DEMO_VARIANT_USING_STRUCT( SimpleShapesDemo, HK_DEMO_TYPE_PRIME, ShapeVariant, g_variants, helpString);

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
