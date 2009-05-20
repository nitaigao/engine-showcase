/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/Physics/Test/Stress/RagdollConstraintStressTest/RagdollConstraintStressTest.h>


#include <Physics/Dynamics/World/hkpSimulationIsland.h>

// We will need these collision agents
#include <Physics/Collide/Agent/ConvexAgent/BoxBox/hkpBoxBoxAgent.h>

// We need to create a constraint
#include <Physics/Dynamics/Constraint/Bilateral/Ragdoll/hkpRagdollConstraintData.h>

// We want to turn on constraint viewers for this demo
#include <Physics/Utilities/VisualDebugger/Viewer/Dynamics/hkpConstraintViewer.h>


RagdollConstraintStressTest::RagdollConstraintStressTest(hkDemoEnvironment* env)
: hkDefaultPhysicsDemo(env)
{
	//
	// Setup the camera
	//
	{
		hkVector4 from(3.0f, 2.0f, 5.0f);
		hkVector4 to(0.0f, 0.0f, 0.0f);
		hkVector4 up(1.0f, 0.0f, 0.0f);
		setupDefaultCameras( env, from, to, up );
	}


	//
	// Create the world
	//
	{
		hkpWorldCinfo info;
		info.setupSolverInfo(hkpWorldCinfo::SOLVER_TYPE_4ITERS_MEDIUM); 
		info.m_enableDeactivation = false;
		info.m_gravity.set(-9.81f, 0.0f, 0.0f);

		//info.m_solverMicrosteps = info.m_solverIterations;
		//info.m_solverIterations = 1;

		m_world = new hkpWorld( info );
		m_world->lock();

		//
		// Create constraint viewer
		//
		m_debugViewerNames.pushBack( hkpConstraintViewer::getName() );
		
		setupGraphics();
	}
		  
	hkVector4 pivot(0.0f, 0.0f, 0.0f);
	hkVector4 halfSize(1.0f, 0.25f, 0.5f);

	// Create Box Shapes
	//
	hkpBoxShape* boxShape;
	{
		boxShape = new hkpBoxShape( halfSize , 0 );
	}

	//
	// Create fixed rigid body
	//
	hkpRigidBody* rb0;
	{
		hkpRigidBodyCinfo info;
		info.m_position.set(-halfSize(0) - 1.0f, 0.0f, 0.0f);
		info.m_shape = boxShape;
		info.m_motionType = hkpMotion::MOTION_FIXED;
		rb0 = new hkpRigidBody(info);
		m_world->addEntity(rb0);
		rb0->removeReference();	
	}


	//
	// Create movable rigid body
	//
	hkpRigidBody* rb1;
	{
		hkpRigidBodyCinfo info;
		info.m_position.set(halfSize(0) + 1.0f, 0.0f, 0.0f);
		info.m_shape = boxShape;
		
			// Compute the box inertia tensor
		hkpMassProperties massProperties;
		info.m_mass = 10.0f;
		hkpInertiaTensorComputer::computeBoxVolumeMassProperties(halfSize, info.m_mass, massProperties);
		info.m_inertiaTensor = massProperties.m_inertiaTensor;
		info.m_centerOfMass = massProperties.m_centerOfMass;	
		info.m_motionType = hkpMotion::MOTION_BOX_INERTIA;
		info.m_linearVelocity.set(0.0f, 1.0f, 1.0f);
		info.m_angularVelocity.set(1.0f, 1.0f, 0.0f);
		rb1 = new hkpRigidBody(info);
		m_world->addEntity(rb1);
		rb1->removeReference();	
	}


	//
	//	Cleanup shape references
	//
	boxShape->removeReference();
	boxShape = HK_NULL;

	//
	// CREATE RAGDOLL CONSTRAINT
	// 
	{
		hkReal planeMin =  -HK_REAL_PI / 2.0f;
		hkReal planeMax =  HK_REAL_PI / 2.0f;
		hkReal twistMin =  -HK_REAL_PI / 2.0f;
		hkReal twistMax =  HK_REAL_PI / 2.0f;

		hkReal coneMin  =  HK_REAL_PI * 0.3f;//0.9f;

		hkpRagdollConstraintData* rdc = new hkpRagdollConstraintData();

		rdc->setConeAngularLimit(coneMin);
		rdc->setPlaneMinAngularLimit(planeMin);
		rdc->setPlaneMaxAngularLimit(planeMax);
		rdc->setTwistMinAngularLimit(twistMin);
		rdc->setTwistMaxAngularLimit(twistMax);

		hkVector4 twistAxis(1.0f, 0.0f, 0.0f);
		hkVector4 planeAxis(0.0f, 1.0f, 0.0f);
		pivot.set(0.0f, 0.0f, 0.0f);
		rdc->setInWorldSpace(rb1->getTransform(), rb0->getTransform(), pivot, twistAxis, planeAxis);
		rdc->setMaxFrictionTorque(1.1f);
		
		//
		//	Create and add the constraint
		//
		{
			hkpConstraintInstance* constraint = new hkpConstraintInstance(rb1, rb0, rdc );
			m_world->addConstraint(constraint);
			constraint->removeReference();
		}		

		rdc->removeReference();
	}

	m_world->unlock();

	m_frameCount = 0;
}


RagdollConstraintStressTest::~RagdollConstraintStressTest()
{

}

hkDemo::Result RagdollConstraintStressTest::stepDemo()
{
	if (m_frameCount++ < 60)
	{
		// Active entity
		m_world->lock();
		hkpEntity* e = m_world->getActiveSimulationIslands()[0]->getEntities()[0];
		hkpRigidBody* rb = static_cast<hkpRigidBody*>(e);
		const hkVector4& pos = rb->getPosition();
		hkVector4 imp; imp.setCross(pos, m_world->getGravity());
		imp.mul4(0.3f);
		rb->applyLinearImpulse(imp);
		m_world->unlock();
	}
	return hkDefaultPhysicsDemo::stepDemo();
}


#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
"The ragdoll constraint's cone limit explodes easily when no extra stabilization is applied. This example shows, how "\
"the constraint remains 'controllable' even at high angular velocities. The rotating body has an initial force applied "\
"for the first 60 frames, and then slowly accelerates purely due to accumulating computational errors. The constraint "\
"is stopped from exploding by softening the cone limit at high angular velocity.";

HK_DECLARE_DEMO(RagdollConstraintStressTest, HK_DEMO_TYPE_PHYSICS, "A violated ragdoll constraint", helpString);

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
