/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/Physics/Api/Collide/OfflineGeneration/ObbUtil/ObbUtilDemo.h>

#include <Physics/Collide/Shape/Misc/Transform/hkpTransformShape.h>
#include <Physics/Collide/Shape/Convex/Sphere/hkpSphereShape.h>
#include <Physics/Collide/Shape/Convex/Box/hkpBoxShape.h>
#include <Physics/Collide/Shape/Convex/Capsule/hkpCapsuleShape.h>
#include <Physics/Collide/Shape/Convex/ConvexVertices/hkpConvexVerticesShape.h>

#include <Demos/DemoCommon/Utilities/GameUtils/GameUtils.h>
#include <Common/Base/Container/LocalArray/hkLocalArray.h>
#include <Graphics/Common/Window/hkgWindow.h>
#include <Demos/DemoCommon/DemoFramework/hkTextDisplay.h>

#include <Common/Visualize/hkDebugDisplay.h>

#include <Common/Internal/ConvexHull/hkGeometryUtility.h>

ObbUtilDemo::ObbUtilDemo(hkDemoEnvironment* env)
:	hkDefaultPhysicsDemo(env), m_rand(747)
{
	//
	// hkpCollidable will warn about performance issues with transform shapes.
	// We are not interested in performance here so we temporarily disable the warning.	
	//
	hkError::getInstance().setEnabled(0x2ff8c16f, false);

	//
	// Setup the camera
	//
	{
		hkVector4 from(0.0f, 10.0f, 40.0f);
		hkVector4 to  (0.0f,  0.0f,  0.0f);
		hkVector4 up  (0.0f,  1.0f,  0.0f);
		setupDefaultCameras( env, from, to, up );
	}

	//
	// Create the world
	//
	{
		hkpWorldCinfo info;
		info.setBroadPhaseWorldSize( 100.0f );
		info.setupSolverInfo(hkpWorldCinfo::SOLVER_TYPE_4ITERS_MEDIUM);
		info.m_gravity.set(0,0,0);

		m_world = new hkpWorld( info );
		m_world->lock();

		// Register ALL agents (though some may not be necessary)
		hkpAgentRegisterUtil::registerAllAgents(m_world->getCollisionDispatcher());

		setupGraphics();
	}

	//
	// Create some random geometry and corresponding OBBs
	// Use these OBBs to create boxes spheres and capsules
	//
	const int numShapes = 6;
	const int maxVerts = 20;
	hkVector4 verts[maxVerts];
	float* vertFltPtr = static_cast<float*>(&verts[0](0));
	const int striding = sizeof(hkVector4);

	for (int i=0; i< numShapes; i++)
	{

		// Construct a random point cloud , scaled and offset

		hkVector4 scale;
		scale.set(m_rand.getRandReal01() + 1.f, m_rand.getRandReal01() + 1.f, m_rand.getRandReal01() + 1.f);

		hkVector4 offset;
		offset.set(m_rand.getRandRange(-1,1), m_rand.getRandRange(-1,1), m_rand.getRandRange(-1,1));

		// Min 4 verts
		int numVerts = (int)m_rand.getRandRange(0, maxVerts - 4) + 4;
		for (int v=0; v < numVerts; v++)
		{
			verts[v](0) = m_rand.getRandRange(-1,1) * scale(0) + offset(0);
			verts[v](1) = m_rand.getRandRange(-1,1) * scale(1) + offset(1);
			verts[v](2) = m_rand.getRandRange(-1,1) * scale(2) + offset(2);		
		}

		hkStridedVertices stridedVerts;
		{
			stridedVerts.m_vertices = vertFltPtr;
			stridedVerts.m_numVertices = numVerts;
			stridedVerts.m_striding = striding;
		}

		hkVector4 halfExtents;
		hkTransform obbTransform;

		hkGeometryUtility::calcObb( stridedVerts, halfExtents, obbTransform );

		//
		// We'll use the transform to create the shapes. hkpCollidable will warn that we have
		// a transform shape as our root collidable, in most cases we can bake this transform
		// into either the subshape (in the case of the capsule) or the scene graph display.
		//


		// Convex approximation
		hkpConvexVerticesShape* convexShape;
		{
			hkArray<hkVector4> planeEquations;
			hkGeometry geom;
			hkGeometryUtility::createConvexGeometry(stridedVerts, geom, planeEquations );
			convexShape = new hkpConvexVerticesShape(stridedVerts, planeEquations);
		}		

		// Box approximation
		hkpTransformShape* transformedBox;
		{
			hkpBoxShape* boxShape = new hkpBoxShape(halfExtents, 0 );
			transformedBox = new hkpTransformShape(boxShape, obbTransform);
			boxShape->removeReference();
		}

		// Capsule - Longest extent from axis, shortest forms radius
		hkpTransformShape* transformedContainedCapsule;
		{
			// Capsule is contained within OBB
			const int maxIndex = halfExtents.getMajorAxis();
			const int i1 = (maxIndex+1)%3;
			const int i2 = (maxIndex+2)%3;
			const hkReal radius = hkMath::max2( halfExtents(i1), halfExtents(i2) );

			// Work out the capsule parameters
			hkVector4 top; top.setZero4();
			hkVector4 bottom; bottom.setZero4();

			top(maxIndex)    -= halfExtents(maxIndex) - radius;
			bottom(maxIndex) += halfExtents(maxIndex) - radius;

			// readjust transform
			hkpCapsuleShape* capsuleShape = new hkpCapsuleShape(bottom, top, radius);
			transformedContainedCapsule = new hkpTransformShape(capsuleShape, obbTransform);
			capsuleShape->removeReference();
		}

		// Capsule - Longest extent from axis, next longest forms radius
		hkpTransformShape* transformedBoundingCapsule;
		{
			// Capsule is contained within OBB
			const int maxIndex = halfExtents.getMajorAxis();
			const int i1 = (maxIndex+1)%3;
			const int i2 = (maxIndex+2)%3;
			const hkReal radius = hkMath::sqrt( halfExtents(i1) * halfExtents(i1) +  halfExtents(i2) * halfExtents(i2));

			// Work out the capsule parameters
			hkVector4 top; top.setZero4();
			hkVector4 bottom; bottom.setZero4();
			top(maxIndex)    -= halfExtents(maxIndex) - radius;
			bottom(maxIndex) += halfExtents(maxIndex) - radius;

			hkpCapsuleShape* capsuleShape = new hkpCapsuleShape(bottom, top, radius);
			transformedBoundingCapsule = new hkpTransformShape(capsuleShape, obbTransform);
			capsuleShape->removeReference();
		}

		// Capsule - Longest extent form axis, shortest forms radius
		hkpTransformShape* transformedMaxSphere;
		{
			// Sphere is contained within OBB
			const int maxIndex = halfExtents.getMajorAxis();

			hkpSphereShape* sphereShape = new hkpSphereShape(halfExtents(maxIndex));
			transformedMaxSphere = new hkpTransformShape(sphereShape, obbTransform);
			sphereShape->removeReference();
		}
		
		// Make rigidbodies for each of the approximations


		// Compute the mass properties from the results of the OBB util too.
		// We could also simply use the initial geometry to compute the inertia tensor.
		hkpRigidBodyCinfo rigidBodyInfo;
		{
			rigidBodyInfo.m_motionType = hkpMotion::MOTION_BOX_INERTIA;

			hkReal mass = 10.0f;

			hkpMassElement me;
			me.m_transform = obbTransform;
			hkpInertiaTensorComputer::computeBoxVolumeMassProperties(halfExtents, mass, me.m_properties);

			hkArray<hkpMassElement> massElements;
			massElements.pushBack(me);

			hkpMassProperties massProperties;
			hkpInertiaTensorComputer::combineMassProperties(massElements, massProperties);
			rigidBodyInfo.m_inertiaTensor = massProperties.m_inertiaTensor;	
			rigidBodyInfo.m_centerOfMass = massProperties.m_centerOfMass;
			rigidBodyInfo.m_mass = massProperties.m_mass;
		}



		const hkReal separation = 6.0f;
		const int vPos = i- (numShapes / 2);

		// Create the convex rigid body 
		{
			rigidBodyInfo.m_shape = convexShape;
			rigidBodyInfo.m_position.set(separation * -2, vPos * separation, 0.0f);			
				
			hkpRigidBody* rigidBody = new hkpRigidBody(rigidBodyInfo);
			rigidBodyInfo.m_shape->removeReference();
			m_world->addEntity(rigidBody);
			rigidBody->removeReference();
		}
		
		// Create the box
		{
			rigidBodyInfo.m_shape = transformedBox;
			rigidBodyInfo.m_position.set(separation * -1, vPos * separation, 0.0f);

			hkpRigidBody* rigidBody = new hkpRigidBody(rigidBodyInfo);
			rigidBodyInfo.m_shape->removeReference();
			m_world->addEntity(rigidBody);
			rigidBody->removeReference();
			HK_SET_OBJECT_COLOR((hkUlong)rigidBody->getCollidable(), hkColor::RED);
		}
		
		// Create the contained capsule
		{
			rigidBodyInfo.m_shape = transformedContainedCapsule;
			rigidBodyInfo.m_position.set(separation * 0, vPos * separation, 0.0f);
			hkpRigidBody* rigidBody = new hkpRigidBody(rigidBodyInfo);
			rigidBodyInfo.m_shape->removeReference();
			m_world->addEntity(rigidBody);
			rigidBody->removeReference();
			HK_SET_OBJECT_COLOR((hkUlong)rigidBody->getCollidable(), hkColor::BLUE);
		}

		// Create the bounding capsule
		{
			rigidBodyInfo.m_shape = transformedBoundingCapsule;
			rigidBodyInfo.m_position.set(separation * 1, vPos * separation, 0.0f);
			hkpRigidBody* rigidBody = new hkpRigidBody(rigidBodyInfo);
			rigidBodyInfo.m_shape->removeReference();
			m_world->addEntity(rigidBody);
			rigidBody->removeReference();
			HK_SET_OBJECT_COLOR((hkUlong)rigidBody->getCollidable(), hkColor::YELLOW);
		}

		// Create the bounding sphere
		{
			rigidBodyInfo.m_shape = transformedMaxSphere;
			rigidBodyInfo.m_position.set(separation * 2, vPos * separation, 0.0f);
			hkpRigidBody* rigidBody = new hkpRigidBody(rigidBodyInfo);
			rigidBodyInfo.m_shape->removeReference();
			m_world->addEntity(rigidBody);
			rigidBody->removeReference();
			HK_SET_OBJECT_COLOR((hkUlong)rigidBody->getCollidable( ), hkColor::GREEN);
		}
	}
	
	const int h = m_env->m_window->getHeight();
	m_env->m_textDisplay->outputText("Original", 20, h-110,			(hkUint32)hkColor::GREY75, 10000);
	m_env->m_textDisplay->outputText("Box approx", 20, h-90,		(hkUint32)hkColor::RED, 10000);
	m_env->m_textDisplay->outputText("Capsule approx 1", 20, h-70,	(hkUint32)hkColor::BLUE, 10000);
	m_env->m_textDisplay->outputText("Capsule approx 2", 20, h-50,	(hkUint32)hkColor::YELLOW, 10000);
	m_env->m_textDisplay->outputText("Sphere approx", 20, h-30,		(hkUint32)hkColor::GREEN, 10000);

	m_world->unlock();
}

ObbUtilDemo::~ObbUtilDemo()
{
	// Re-enable the hkpCollidable warning
	hkError::getInstance().setEnabled(0x2ff8c16f, true);

	if (m_env && m_env->m_textDisplay)
		m_env->m_textDisplay->wipeText();
}

#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
"This demo shows how you might use the OBB utility to create primitives from arbitrary geometry. " \
"This is very useful if you want to generate optimized shapes at run time " \
"from any tool chain. Alternatively you could bake this functionality into " \
"a preprocessing tool or editor.\n" \
"For each random shape (left column) we produce 4 different approximations. " \
"Each of these are constructed from an oriented bounding box.";

HK_DECLARE_DEMO(ObbUtilDemo, HK_DEMO_TYPE_PRIME, "Approximating shapes", helpString);

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
