/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


#include <Demos/demos.h>

#include <Physics/Collide/Shape/Compound/Collection/List/hkpListShape.h>
#include <Physics/Collide/Shape/Compound/Tree/Mopp/hkpMoppBvTreeShape.h>
#include <Physics/Collide/Shape/Compound/Tree/Mopp/hkpMoppUtility.h>
#include <Physics/Collide/Shape/Convex/ConvexTranslate/hkpConvexTranslateShape.h>

#include <Common/Internal/ConvexHull/hkGeometryUtility.h>
#include <Physics/Dynamics/World/hkpPhysicsSystem.h>

#include <Graphics/Common/Window/hkgViewport.h>
#include <Graphics/Common/Window/hkgWindow.h>
#include <Graphics/Common/Camera/hkgCamera.h>

#include <Common/Visualize/hkDebugDisplay.h>

#include <Demos/Physics/UseCase/ContinuousPhysics/CrashTestDummies/CrashTestDummiesDemo.h>

#include <Demos/DemoCommon/Utilities/GameUtils/GameUtils.h>




hkpRigidBody* CrashTestDummiesDemo::createSimpleCarHull()
{
	// Create the shape and a rigid body to view it.
	hkpShape* carShape;
	{
		// Data specific to this shape.
		int numVertices[2] = {8, 10};

			// 16 = 4 (size of "each float group", 3 for x,y,z, 1 for padding) * 4 (size of float)
		int stride = sizeof(float) * 4;

		float vertices[2][10*4] = 
		{
			{ // 4 vertices plus padding
				 0.00f, 0.00f, 0.00f, 0.00f,
				 0.55f, 0.00f, 0.00f, 0.00f, 
				 0.05f, 0.00f, 0.40f, 0.00f,
				 0.45f, 0.00f, 0.40f, 0.00f,
				 0.00f, 2.00f, 0.00f, 0.00f,
				 0.55f, 2.00f, 0.00f, 0.00f, 
				 0.05f, 2.00f, 0.40f, 0.00f,
				 0.45f, 2.00f, 0.40f, 0.00f
			},
			{
				 0.00f, 0.00f, 0.00f, 0.00f,
				 0.05f, 0.00f, 0.40f, 0.00f,
				 0.00f, 0.00f, 0.80f, 0.00f,
				-0.05f, 0.00f, 0.80f, 0.00f,
				-0.05f, 0.00f, 0.00f, 0.00f,
				 0.00f, 2.00f, 0.00f, 0.00f,
				 0.05f, 2.00f, 0.40f, 0.00f,
				 0.00f, 2.00f, 0.80f, 0.00f,
				-0.05f, 2.00f, 0.80f, 0.00f,
				-0.05f, 2.00f, 0.00f, 0.00f,
			}
		};

		
		/////////////////// SHAPE CONSTRUCTION ////////////////
		hkArray<hkpShape*> shapes;

		{
			{
				for (int i = 0; i < 2; i++)
				{
					hkStridedVertices stridedVerts;
					{
						stridedVerts.m_numVertices = numVertices[i];
						stridedVerts.m_striding = stride;
						stridedVerts.m_vertices = vertices[i];
					}

					hkGeometry geom;
					hkArray<hkVector4> planeEquations;

					hkGeometryUtility::createConvexGeometry( stridedVerts, geom, planeEquations );

					{
						stridedVerts.m_numVertices = geom.m_vertices.getSize();
						stridedVerts.m_striding = sizeof(hkVector4);
						stridedVerts.m_vertices = &(geom.m_vertices[0](0));
					}

					hkpConvexVerticesShape* shape = new hkpConvexVerticesShape(stridedVerts, planeEquations);
					shape->setRadius(0.01f);

					hkpConvexTranslateShape* tshape = new hkpConvexTranslateShape(shape, hkVector4(1.5f, 0.0f, 0.0f));

					shapes.pushBack(shape);
					shapes.pushBack(tshape);
				}		
			}

		///////////////////////////////////////////////////////

			{
				hkVector4 carDim(4.5f, 2.0f, 1.7f);
				carDim.mul4(0.5f);
				hkReal thick = 0.03f;
				hkVector4 thickness(thick, thick, thick);

				for (int i = 0; i < 3; i++)
				{
					hkVector4 halfExt; halfExt.setZero4();
					halfExt(i) = carDim(i);
					halfExt((i+1)%3) = carDim((i+1)%3);
					halfExt.add4(thickness);

					hkpBoxShape* box = new hkpBoxShape(halfExt, 0.01f);

					hkVector4 shift; shift.setZero4();
					shift((i+2)%3) = carDim((i+2)%3);

					hkVector4 transl[2]; 
					transl[0].setAdd4(carDim, shift);
					transl[1].setSub4(carDim, shift);
					
					hkpConvexTranslateShape* tbox = new hkpConvexTranslateShape(box, transl[0]);
					shapes.pushBack(tbox);
											tbox = new hkpConvexTranslateShape(box, transl[1]);
					shapes.pushBack(tbox);

					box->removeReference();
				}
			}
		}

		hkpListShape* listShape = new hkpListShape(shapes.begin(), shapes.getSize());
		carShape = listShape;

		for (int s = 0; s < shapes.getSize(); s++)
		{
			shapes[s]->removeReference();
		}

		///////////////////////////////////////

		
		// Optionally Construct MOPP around list
		if(0)
		{
			hkpMoppCompilerInput moppInput;
			moppInput.setAbsoluteFitToleranceOfTriangles(0.01f);
			hkpMoppCode* code = hkpMoppUtility::buildCode(listShape, moppInput);		
			carShape = new hkpMoppBvTreeShape(listShape, code);
			listShape->removeReference();
			code->removeReference();
		}
	}

	// create the rigid body
	hkpRigidBody* rigidBody;
	{
		// To illustrate using the shape, create a rigid body by first defining a template.
		hkpRigidBodyCinfo rigidBodyInfo;

		rigidBodyInfo.m_shape = carShape; 
		rigidBodyInfo.m_position.set(0.0f, 0.0f, 0.0f);
		rigidBodyInfo.m_angularDamping = 0.0f;
		rigidBodyInfo.m_linearDamping = 0.0f;


		hkReal mass = 100.0f;
		hkpInertiaTensorComputer::setShapeVolumeMassProperties(carShape, mass, rigidBodyInfo);
		rigidBodyInfo.m_motionType = hkpMotion::MOTION_DYNAMIC;


		rigidBodyInfo.m_friction = 1.0f;
		rigidBodyInfo.m_qualityType = HK_COLLIDABLE_QUALITY_CRITICAL;
			
	 
		// Create a rigid body 
		rigidBody = new hkpRigidBody(rigidBodyInfo);

			// Remove reference since the body now "owns" the Shape.
		rigidBodyInfo.m_shape->removeReference();
	}
	return rigidBody;
}

	// creates ragdolls in a car with an initial velocity 
void CrashTestDummiesDemo::fitRagdollsIn(const hkVector4& velocity)
{
	for (int j = 0; j < 1; j++)
	{
		for (int i = 0; i < 1; i++)
		{
			GameUtils::RagdollPartType ragdollType = GameUtils::RPT_BOX;

			hkVector4		position = hkVector4(0.5f + j * 1.5f, 0.8f + i * 0.4f, 0.8f);
			hkQuaternion	rotation1; rotation1.setAxisAngle( hkVector4( 0, 0, 1 ),  0.50f * HK_REAL_PI );
			hkQuaternion	rotation2; rotation2.setAxisAngle( hkVector4( 0, 1, 0 ), -0.125f * HK_REAL_PI );

			hkQuaternion    rotation; rotation.setMul(rotation2, rotation1);
			///
			m_ragdoll = GameUtils::createRagdoll( 1.6f, position, rotation, m_filter->getNewSystemGroup(), ragdollType );
			///>
			addRagdoll();
		}
	}	
}

void CrashTestDummiesDemo::putBoxesIn(const hkVector4& pos)
{
	//hkpShape* shape = new hkpBoxShape(hkVector4(0.25f, 0.25f, 0.25f), 0.01f);
	hkpShape* shape = new hkpSphereShape(0.25f);
	hkpRigidBodyCinfo info;
	info.m_shape = shape;
	info.m_motionType = hkpMotion::MOTION_DYNAMIC;
	info.m_qualityType = HK_COLLIDABLE_QUALITY_CRITICAL; 
	info.m_position = pos;
	info.m_friction = 1.0f;
	hkpRigidBody* body = new hkpRigidBody(info);
	m_world->addEntity(body);
	body->removeReference();
	shape->removeReference();

}

void CrashTestDummiesDemo::createGroundBox()
{
	hkpShape* shape = new hkpBoxShape(hkVector4(20,20,0.1f), 0.01f);
	hkpRigidBodyCinfo info;
	info.m_shape = shape;
	info.m_motionType = hkpMotion::MOTION_FIXED;
	info.m_position = hkVector4(2.0f, 1.0f, -0.1f);
	info.m_friction = 1.0f;
	hkpRigidBody* body = new hkpRigidBody(info);
	m_world->addEntity(body);
	body->removeReference();
	shape->removeReference();
}

void CrashTestDummiesDemo::createFastObject()
{
	hkpShape* shape = new hkpSphereShape(0.3f);
	hkpRigidBodyCinfo info;
	info.m_shape = shape;
	info.m_motionType = hkpMotion::MOTION_DYNAMIC;
	info.m_qualityType = HK_COLLIDABLE_QUALITY_BULLET;
	info.m_position(2) -= 0.1f;
	info.m_mass = 10.100f;
	info.m_position = hkVector4(30.0f, 1.0f, 1.3f);
	info.m_linearVelocity = hkVector4(-200.0f, 0.0f, 0.0f);
	hkpRigidBody* body = new hkpRigidBody(info);
	m_world->addEntity(body);
	body->removeReference();
	shape->removeReference();
}


CrashTestDummiesDemo::CrashTestDummiesDemo(hkDemoEnvironment* env): hkDefaultPhysicsDemo( env )
{
	// Disable warnings:									
	hkError::getInstance().setEnabled(0xafe97523, false); //'This utility is intended primarily for Havok demo use. If you wish to step the world asynchronously,...'

	// XXX remove once async stepping fixed
	hkpWorld::IgnoreForceMultithreadedSimulation ignoreForceMultithreaded;

	enableDisplayingToiInformation(true);

	m_ragdoll = HK_NULL;

	//
	//	Create the world
	//
	{
		hkpWorldCinfo info;
		info.m_gravity.set( 0.0f, 0.0f, -10.0f );
		//info.m_enableDeactivation = false;
		info.m_simulationType = hkpWorldCinfo::SIMULATION_TYPE_CONTINUOUS;
		//info.m_simulationType = hkpWorldCinfo::SIMULATION_TYPE_BACKSTEP_SIMPLE;
		m_world = new hkpWorld( info );
		m_world->lock();

		// Register ALL agents (though some may not be necessary)
		hkpAgentRegisterUtil::registerAllAgents(m_world->getCollisionDispatcher());
	}

	//
	// Collision Filter
	//
	{ 
		m_filter = new hkpGroupFilter();
		hkpGroupFilterSetup::setupGroupFilter( m_filter );
		m_world->setCollisionFilter(m_filter);
	}

	//
	// Setup the camera
	//
	{
		hkVector4 from(0.0f, 8.0f, 3.0f);
		hkVector4 to(0.0f, 0.0f, 1.0f);
		hkVector4 up(0.0f, 0.0f, 1.0f);
		setupDefaultCameras( env, from, to, up, 1.f, 1000.0f );

		setupGraphics();
	}

	m_car =	createSimpleCarHull();
	m_world->addEntity( m_car )->removeReference();
	HK_SET_OBJECT_COLOR(hkUlong(m_car->getCollidable()), hkColor::rgbFromChars(255, 255, 255, 50));

	fitRagdollsIn(hkVector4::getZero());
	//hkVector4 pos(3.0f, 1.0f, 0.8f);
	//putBoxesIn( pos );
	//putBoxesIn(hkVector4(4.1f, 1.0f, 0.8f));
	
	createGroundBox();
	createFastObject();

	m_world->unlock();
}

CrashTestDummiesDemo::~CrashTestDummiesDemo()
{
	// Re-enable warnings:									
	hkError::getInstance().setEnabled(0xafe97523, true); 

	m_world->lock();

	if( m_ragdoll )
	{
		delete m_ragdoll;
		m_ragdoll = HK_NULL;
	}
	m_filter->removeReference();

	m_world->unlock();
}

void CrashTestDummiesDemo::addRagdoll()
{
	const hkArray<hkpRigidBody*>& rigidbodies = m_ragdoll->getRigidBodies();
	for( int iRB = 0; iRB < rigidbodies.getSize(); iRB++ )
	{
		rigidbodies[iRB]->getMaterial().setFriction(1.0f);
		rigidbodies[iRB]->setQualityType(HK_COLLIDABLE_QUALITY_CRITICAL);
	}
	/*
	const hkArray<hkpConstraintInstance*>& constraints = m_ragdoll->getConstraints();
	for( int iC = 0; iC < constraints.getSize(); iC++ )
	{
		//constraints[iC]->setPriority( hkpConstraintInstance::PRIORITY_TOI );
	}
	*/

	m_world->addPhysicsSystem(m_ragdoll);
}




hkDemo::Result CrashTestDummiesDemo::stepDemo()
{
	hkReal m_frameToSimulationFrequencyRatio = 10.0f;

	hkReal physicsDeltaTime = 1.0f / 60.0f;
	hkReal frameDeltaTime = physicsDeltaTime / m_frameToSimulationFrequencyRatio;

	//frameDeltaTime = physicsDeltaTime;
	hkDefaultPhysicsDemo::stepAsynchronously(m_world, frameDeltaTime, physicsDeltaTime );

	return DEMO_OK;

}







#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
"This demo shows a single 11 bone ragdoll inside a car during a crash ";

HK_DECLARE_DEMO( CrashTestDummiesDemo, HK_DEMO_TYPE_PHYSICS, "CrashTestDummies", helpString);

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
