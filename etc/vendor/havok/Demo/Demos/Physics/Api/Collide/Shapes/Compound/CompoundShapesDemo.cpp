/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/Physics/Api/Collide/Shapes/Compound/CompoundShapesDemo.h>

#include <Physics/Collide/Shape/Misc/ConvexList/hkpConvexListShape.h>
#include <Physics/Collide/Shape/Compound/Collection/List/hkpListShape.h>
#include <Physics/Collide/Shape/Convex/ConvexTranslate/hkpConvexTranslateShape.h>
#include <Physics/Collide/Shape/Misc/Transform/hkpTransformShape.h>
#include <Physics/Collide/Shape/Misc/Bv/hkpBvShape.h>


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Definitions of variants
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct ShapeVariant
{
	const char*	m_name;
	hkpShapeType m_shapeType;
	const char* m_details;
};

static const char bvShapeString[] = \
"A demo which shows creation of a BV (Bounding Volume) shape (and use in a hkpRigidBody). " \
"The bounding volume used is a sphere, the child shape (bounded by the sphere) is a box (not displayed). " \
"The sphere acts as a midphase for the box. Collision tests are first performed against the sphere before " \
"they pass down to the box.";

static const ShapeVariant g_variants[] =
{
{ "List", HK_SHAPE_LIST, "A compound 'list' shape, consisting of two boxes" },
{ "Convex List", HK_SHAPE_CONVEX_LIST, "A table created using a list of convex shapes" },
{ "Transform Shape", HK_SHAPE_TRANSFORM, "An 'transform' shape with a rotation and translation wrapped around a box shape." },
{ "BV shape", HK_SHAPE_BV, bvShapeString }
};



CompoundShapesDemo::CompoundShapesDemo(hkDemoEnvironment* env)
:	hkDefaultPhysicsDemo(env)
{
	// Disable warnings:									
	hkError::getInstance().setEnabled(0x2ff8c16f, false); // 'Your m_contactRestingVelocity seems to be too small'

	const ShapeVariant& variant =  g_variants[m_variantId];

	// Setup the camera.
	{
		hkVector4 from(0.0f, 5.0f, 10.0f);
		hkVector4 to  (0.0f, 0.0f,  0.0f);
		hkVector4 up  (0.0f, 1.0f,  0.0f);
		setupDefaultCameras(env, from, to, up);
	}

	// Create the world.
	{
		hkpWorldCinfo info;
		
		// Set gravity to zero so body floats.
		info.m_gravity.set(0.0f, 0.0f, 0.0f);	
		info.setBroadPhaseWorldSize( 100.0f );
		m_world = new hkpWorld(info);
		m_world->lock();

		setupGraphics();
	}

	// Create the shape variant
	hkpShape* shape = 0;
	switch (variant.m_shapeType)
	{
		// List
		case HK_SHAPE_LIST:
		{
			hkVector4 halfExtents1 = hkVector4(2.0f, 0.5f, 0.5f);
			hkVector4 halfExtents2 = hkVector4(0.5f, 2.0f, 0.5f);
			
			hkpBoxShape* boxShape1 = new hkpBoxShape(halfExtents1, 0);
			hkpBoxShape* boxShape2 = new hkpBoxShape(halfExtents2, 0);
			
			hkArray<hkpShape*> shapeArray;
			shapeArray.pushBack(boxShape1);
			shapeArray.pushBack(boxShape2);
			
			shape = new hkpListShape(shapeArray.begin(), shapeArray.getSize());
			
			// Remove temporary shapes as the list now 'owns' them
			boxShape1->removeReference();
			boxShape2->removeReference();

			break;
		}


		// Convex List
		case HK_SHAPE_CONVEX_LIST:
		{
			//
			// Data specific to this shape (a table)
			//
			const hkReal radius = 0.01f;
			hkVector4 tableExtents( 1.0f, 0.02f, 0.6f );
			hkpBoxShape* tableShape = new hkpBoxShape( tableExtents, radius );

			hkVector4 legExtents( 0.1f, 0.6f, 0.1f );
			hkpBoxShape* legShape = new hkpBoxShape( legExtents, radius );

			hkVector4 legInvertDist( 0.1f, 0.0f, 0.1f );
			hkVector4 l; l.setSub4( tableExtents, legInvertDist );
			l.sub4( legExtents );

			hkVector4 tablePos( 0, 2 * legExtents(1) + tableExtents(1), 0 ); 
			hkVector4 leg0Pos (  l(0), legExtents(1),  l(2) );
			hkVector4 leg1Pos ( -l(0), legExtents(1),  l(2) );
			hkVector4 leg2Pos (  l(0), legExtents(1), -l(2) );
			hkVector4 leg3Pos ( -l(0), legExtents(1), -l(2) );

			const hkpConvexShape* shapes[5];

			// Create the convex list elements
			shapes[0] = new hkpConvexTranslateShape( tableShape, tablePos );
			shapes[1] = new hkpConvexTranslateShape( legShape, leg0Pos );
			shapes[2] = new hkpConvexTranslateShape( legShape, leg1Pos );
			shapes[3] = new hkpConvexTranslateShape( legShape, leg2Pos );
			shapes[4] = new hkpConvexTranslateShape( legShape, leg3Pos );

			/////////////////// SHAPE CONSTRUCTION ////////////////
			shape = new hkpConvexListShape( &shapes[0], 5);

			// Remove temporary shapes
			{
				for (int i =0; i < 5; i++ ) shapes[i]->removeReference();
				legShape->removeReference();
				tableShape->removeReference();
			}

			break;
		}
	
		
		// BV shape
		case HK_SHAPE_BV:
		{
			// Data specific to this shape.
			hkVector4 halfExtents(1.0f, 1.0f, 1.0f);
			// The box is of side 2, so we must bound it by a sphere of radius >= sqrt(3)
			hkReal radius = 1.75f;


			/////////////////// SHAPE CONSTRUCTION ////////////////
			hkpBoxShape* boxShape = new hkpBoxShape( halfExtents , 0.0f );
			hkpSphereShape* sphereShape = new hkpSphereShape(radius);
			
			// The sphere will serve as bounding volume for box
			shape = new hkpBvShape(sphereShape, boxShape);
			
			// Remove reference since hkpBvShape owns these shapes
			sphereShape->removeReference();
			boxShape->removeReference();

			break;
		}

		case HK_SHAPE_TRANSFORM:
		{
			// Data specific to this shape.
			hkVector4 halfExtents = hkVector4(1.0f, 1.0f, 1.0f);

			hkTransform transform;
			hkVector4 axis(0.0f, 1.0f, 0.0f);
			hkQuaternion rotate(axis, 0.4f);
			hkVector4 translate = hkVector4(1.5f, .7f, 0.0f);
			transform.setRotation(rotate);
			transform.setTranslation(translate);
		
			// Note that this means that if we "position" the rigidbody at the origin, its "cube" shape
			// will appear away from the origin (but its centre of mass will be still at the centre of this
			// cube, because computeShapeVolumeMassProperties() takes this hkTransfromShape into account).
			hkpBoxShape* boxShape = new hkpBoxShape(halfExtents, 0.0f );
			shape = new hkpTransformShape(boxShape, transform);
			boxShape->removeReference();

			break;
		}


		default:
			break;
	}
	
	// Make sure that a shape was created
	HK_ASSERT2(0x0915bb13, shape, "No shape was created");

	// To illustrate using the shape, first define a rigid body template.
	hkpRigidBodyCinfo rigidBodyInfo;
	rigidBodyInfo.m_position.set(0.0f, 0.0f, 0.0f);
	rigidBodyInfo.m_angularDamping = 0.0f;
	rigidBodyInfo.m_linearDamping = 0.0f;
	rigidBodyInfo.m_shape = shape;

	// Compute the rigid body inertia.
	rigidBodyInfo.m_motionType = hkpMotion::MOTION_BOX_INERTIA;
	//hkpInertiaTensorComputer::setShapeVolumeMassProperties( rigidBodyInfo.m_shape, 100.0f, rigidBodyInfo );
		
	// Create a rigid body (using the template above).
	hkpRigidBody* rigidBody = new hkpRigidBody(rigidBodyInfo);

	// Remove reference since the body now "owns" the Shape.
	shape->removeReference();

	// Finally add body so we can see it, and remove reference since the world now "owns" it.
	m_world->addEntity(rigidBody);
	rigidBody->removeReference();

	m_world->unlock();
}

CompoundShapesDemo::~CompoundShapesDemo()
{
	// Re-enable warnings:									
	hkError::getInstance().setEnabled(0x2ff8c16f, true);
}


#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = "Examples of various compound shape types";

HK_DECLARE_DEMO_VARIANT_USING_STRUCT(CompoundShapesDemo, HK_DEMO_TYPE_PRIME, ShapeVariant, g_variants, helpString); 

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
