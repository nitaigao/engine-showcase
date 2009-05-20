/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>


#include <Demos/Physics/Api/Dynamics/Actions/WindAction/WindAction/WindActionDemo.h>

#include <Physics/Utilities/Actions/Wind/hkpWindAction.h>
#include <Physics/Utilities/Actions/Wind/hkpPrevailingWind.h>

#include <Physics/Collide/Shape/Convex/Capsule/hkpCapsuleShape.h>
#include <Physics/Collide/Shape/Convex/Cylinder/hkpCylinderShape.h>
#include <Common/Internal/ConvexHull/hkGeometryUtility.h>
#include <Physics/Collide/Shape/Compound/Collection/List/hkpListShape.h>
#include <Physics/Collide/Shape/Misc/Transform/hkpTransformShape.h>
#include <Physics/Collide/Shape/Convex/ConvexVertices/hkpConvexVerticesConnectivity.h>
#include <Physics/Collide/Shape/Convex/ConvexVertices/hkpConvexVerticesConnectivityUtil.h>

#include <Demos/DemoCommon/Utilities/GameUtils/GameUtils.h>

#include <Common/Visualize/hkDebugDisplay.h>
#include <Common/Visualize/Shape/hkDisplayBox.h>
#include <Common/Base/Types/Color/hkColor.h>

#include <Common/Base/Algorithm/PseudoRandom/hkPseudoRandomGenerator.h>
#include <Common/Base/DebugUtil/DeterminismUtil/hkCheckDeterminismUtil.h>

hkpRigidBody* HK_CALL createCylindricalGeometry( const hkReal radius, const hkReal height, const hkReal mass, const hkVector4& position, const int numVerticesAtEnd )
{
	hkVector4 pointA( 0.0f, -height / 2.0f, 0.0f );
	hkVector4 pointB( 0.0f, height / 2.0f, 0.0f );

	// create an array of vertices
	hkArray<hkVector4> vertices;
	{
		vertices.reserve( numVerticesAtEnd * 2 );
		for( int i = 0; i < numVerticesAtEnd; ++i )
		{
			const hkReal theta = 2.0f * HK_REAL_PI * ( (hkReal) i / numVerticesAtEnd );
			const hkReal cosTheta = hkMath::cos( theta );
			const hkReal sinTheta = hkMath::sin( theta );
			vertices.pushBack( hkVector4( cosTheta * radius, -height / 2.0f, sinTheta * radius ) );
			vertices.pushBack( hkVector4( cosTheta * radius, height / 2.0f, sinTheta * radius ) );
		}
	}

	hkStridedVertices stridedVerts;
	{
		stridedVerts.m_numVertices = vertices.getSize();
		stridedVerts.m_striding = sizeof(hkVector4);
		stridedVerts.m_vertices = &(vertices[0](0));
	}

	// create a convexVerticesShape from this
	hkpConvexVerticesShape* cvs;
	hkArray<hkVector4> planeEquations;
	hkGeometry geom;
	{
		hkGeometryUtility::createConvexGeometry( stridedVerts, geom, planeEquations );

		{
			stridedVerts.m_numVertices = geom.m_vertices.getSize();
			stridedVerts.m_striding = sizeof(hkVector4);
			stridedVerts.m_vertices = &(geom.m_vertices[0](0));
		}

		cvs = new hkpConvexVerticesShape(stridedVerts, planeEquations);
	}

	cvs->setRadius(0.05f);	// This helps to avoid the penetration depth algorithm which is costly

	hkpRigidBodyCinfo convexInfo;

	convexInfo.m_shape = cvs;
	if(mass != 0.0f)
	{
		hkpMassProperties massProperties;
		hkpInertiaTensorComputer::computeVertexHullVolumeMassProperties( stridedVerts.m_vertices, stridedVerts.m_striding, stridedVerts.m_numVertices, mass, massProperties );
		convexInfo.m_mass = mass;
		convexInfo.m_centerOfMass = massProperties.m_centerOfMass;
		convexInfo.m_inertiaTensor = massProperties.m_inertiaTensor;
		convexInfo.m_motionType = hkpMotion::MOTION_BOX_INERTIA;
	}
	else
	{
		convexInfo.m_motionType = hkpMotion::MOTION_FIXED;
	}

	convexInfo.m_rotation.setIdentity();
	convexInfo.m_position = position;

	hkpRigidBody* convexRigidBody = new hkpRigidBody(convexInfo);

	cvs->removeReference();

	return convexRigidBody;
}

// A demo which shows drag being applied to several sphere-like objects.
WindActionDemo::WindActionDemo(hkDemoEnvironment* env)
	: hkDefaultPhysicsDemo(env)
{
	// X and Z are the half-extents of the floor, and Y is the height of the walls.
	hkReal x = 20.0f;
	hkReal y = 4.0f;
	hkReal z = 20.0f;
	
	//
	// Setup the camera.
	//
	{
		hkVector4 from( 0.0f, y * (4.0f), z * 2.5f );
		hkVector4 to(0.0f, 0.0f, 0.0f);
		hkVector4 up(0.0f, 1.0f, 0.0f);
		setupDefaultCameras( env, from, to, up );
	}

	//
	// Create the world.
	//
	{
		hkpWorldCinfo info;
		m_world = new hkpWorld( info );
		m_world->lock();

		setupGraphics();
	}

	//
	// Register all collision agents.
	//
	hkpAgentRegisterUtil::registerAllAgents( m_world->getCollisionDispatcher() );

	// Create the wind
	hkpWind* air;
	{
		air = new hkpPrevailingWind( hkVector4( 2.0f, 0.0f, 0.0f ) );
	}

	hkVector4 areaSize( x, y, z );

	hkReal thickness = 0.5f;

	{
		hkpRigidBody* lowerFloor;
		hkVector4 fixedBoxSize( x , thickness , z );
		hkpBoxShape* fixedBoxShape = new hkpBoxShape( fixedBoxSize , 0 );

		hkpRigidBodyCinfo info;
		{
			info.m_shape = fixedBoxShape;
			info.m_motionType = hkpMotion::MOTION_FIXED;
			info.m_position.set(0.0f, -thickness, 0.0f);
		}

		lowerFloor = new hkpRigidBody(info);
		m_world->addEntity(lowerFloor);

		lowerFloor->removeReference();
		fixedBoxShape->removeReference();
	}


	{
		hkpRigidBody* wallPosX;
		hkVector4 fixedBoxSize( thickness, ( y / 2.0f ) , z );
		hkpBoxShape* fixedBoxShape = new hkpBoxShape( fixedBoxSize , 0 );

		hkpRigidBodyCinfo info;
		{
			info.m_shape = fixedBoxShape;
			info.m_motionType = hkpMotion::MOTION_FIXED;
			info.m_position.set( x + thickness, ( y / 2.0f ) - (thickness * 2.0f), 0.0f);
		}

		wallPosX = new hkpRigidBody(info);
		m_world->addEntity(wallPosX);
		HK_SET_OBJECT_COLOR((hkUlong)wallPosX->getCollidable(), hkColor::rgbFromChars( 100, 100, 100, 100 )); 

		wallPosX->removeReference();
		fixedBoxShape->removeReference();
	}

	// Create a grid of objects.
	{
		hkQuaternion rot;
		hkVector4 axis( 0.0f, 0.5f, 1.0f );
		axis.normalize3();
		
		const float startY = 12.0f;
		float startX = 0.0f;
		const float gapZ = 4.0f;
		float startZ = -gapZ * 2.0f;

	
		{
			hkpRigidBody* body = GameUtils::createSphere( 1.0f, 1.0f, hkVector4( startX, startY, startZ) );
			m_world->addEntity( body );
			hkpWindAction* action = new hkpWindAction( body, air, 0.1f );
			m_world->addAction( action );
			action->removeReference();
			body->removeReference();
			startZ += gapZ;
		}
		
		{ // Create capsules
			hkpRigidBody* body = GameUtils::createCapsuleFromBox( hkVector4( 2.0f, 2.0f, 2.0f ), 1.0f, hkVector4( startX, startY, startZ ) );
			m_world->addEntity( body );
			hkpWindAction* action = new hkpWindAction( body, air, 0.1f );
			m_world->addAction( action );
			action->removeReference();
			body->removeReference();
			startZ += gapZ;
		}

		hkPseudoRandomGenerator* generator = HK_NULL;
		HK_ON_DETERMINISM_CHECKS_ENABLED(generator = new hkPseudoRandomGenerator(234));

		{ // A random sphere-like convex hull
			hkpRigidBody* body = GameUtils::createRandomConvexGeometric( 1.0f, 1.0f, hkVector4( startX, startY, startZ ), 50, generator );
			m_world->addEntity( body );
			hkpWindAction* action = new hkpWindAction( body, air, 0.1f );
			m_world->addAction( action );
			action->removeReference();
			body->removeReference();
			startZ += gapZ;
		}

		{ // A random sphere-like convex hull, whose wind action is approximated by its OOB.
			hkpRigidBody* body = GameUtils::createRandomConvexGeometric( 1.0f, 1.0f, hkVector4( startX, startY, startZ ), 50, generator );
			m_world->addEntity( body );
			HK_SET_OBJECT_COLOR((hkUlong)body->getCollidable(), hkColor::rgbFromChars( 255, 0, 0 )); 
			hkpWindAction* action = new hkpWindAction( body, air, 0.1f, 0.7f );
			m_world->addAction( action );
			action->removeReference();
			body->removeReference();
			startZ += gapZ;
		}

		HK_ON_DETERMINISM_CHECKS_ENABLED(delete generator);
		HK_ON_DETERMINISM_CHECKS_ENABLED(generator = HK_NULL);

		startX = -12.0f;
		startZ = -gapZ;

		{	// Create cylindrical shape
			hkpRigidBody* body = GameUtils::createCylinder( 1.0f, 2.5f, 1.0f, hkVector4( startX, startY, startZ ) );
			m_world->addEntity( body );
			hkpWindAction* action = new hkpWindAction( body, air, 0.1f );
			m_world->addAction( action );
			action->removeReference();
			body->removeReference();
			startZ += gapZ;
		}

		{	// Create cylindrical shape
			hkpRigidBody* body = createCylindricalGeometry( 1.0f, 2.5f, 1.0f, hkVector4( startX, startY, startZ ), 20 );
			m_world->addEntity( body );
			hkpWindAction* action = new hkpWindAction( body, air, 0.1f );
			m_world->addAction( action );
			action->removeReference();
			body->removeReference();
			startZ += gapZ;
		}
		
		{	// Create cylindrical shape, whose wind action is approximated by its OBB.
			hkpRigidBody* body = createCylindricalGeometry( 1.0f, 2.5f, 1.0f, hkVector4( startX, startY, startZ ), 20 );
			m_world->addEntity( body );
			HK_SET_OBJECT_COLOR((hkUlong)body->getCollidable(), hkColor::rgbFromChars( 255, 0, 0 )); 
			hkpWindAction* action = new hkpWindAction( body, air, 0.1f, 0.9f );
			m_world->addAction( action );
			action->removeReference();
			body->removeReference();
			startZ += gapZ;
		}

	}
	
	air->removeReference();

	m_world->unlock();
}

#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
"This demo shows the effect of wind on some rigid bodies, with those in red being approximated by oriented bounding boxes.";

HK_DECLARE_DEMO(WindActionDemo, HK_DEMO_TYPE_PRIME, "This demo shows the effect of drag on different types of rigid bodies", helpString);

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
