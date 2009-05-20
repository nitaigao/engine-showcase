/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/Physics/Test/Performance/BallAndSocketChains/BallAndSocketChainsDemo.h>


// We will need these collision agents
#include <Physics/Collide/Agent/ConvexAgent/BoxBox/hkpBoxBoxAgent.h>

// We need to create a constraint
#include <Physics/Dynamics/Constraint/Bilateral/BallAndSocket/hkpBallAndSocketConstraintData.h>

// We want to turn on constraint viewers for this demo
#include <Physics/Utilities/VisualDebugger/Viewer/Dynamics/hkpConstraintViewer.h>

#include <Physics/Dynamics/Constraint/hkpConstraintInstance.h>

// Filters
#include <Physics/Utilities/Collide/Filter/ConstrainedSystem/hkpConstrainedSystemFilter.h>
#include <Physics/Utilities/Constraint/Bilateral/hkpConstraintUtils.h>
#include <Physics/Utilities/Collide/Filter/GroupFilter/hkpGroupFilterUtil.h>


BallAndSocketChainsDemo::BallAndSocketChainsDemo(hkDemoEnvironment* env)
:	hkDefaultPhysicsDemo(env)
{

	// Disable warnings:
	hkError::getInstance().setEnabled(0x2a1db936, false); //'Constraint added between two *colliding* dynamic rigid bodies. Check your collision filter logic...' 

	//
	// Setup the camera
	//
	{
		hkVector4 from(0.0f, 0.0f, 10.0f);
		hkVector4 to(0.0f, 0.0f, 0.0f);
		hkVector4 up(0.0f, 1.0f, 0.0f);

		// Change the camera for displaying 10 constraints
		from.set(-9.0f,  0.0f, 15.0f);
		to.set( 8.0f, -6.0f,  0.0f);

		setupDefaultCameras( env, from, to, up );
	}

	//
	// Create the world
	//
	{
		hkpWorldCinfo info;
		info.setupSolverInfo(hkpWorldCinfo::SOLVER_TYPE_4ITERS_MEDIUM); 
		info.setBroadPhaseWorldSize( 100.0f );
		info.m_enableDeactivation = false;
		info.m_collisionTolerance = 0.01f;
		m_world = new hkpWorld( info );
		m_world->lock();

		// Register the single agent required (a hkpBoxBoxAgent)
		hkpAgentRegisterUtil::registerAllAgents( m_world->getCollisionDispatcher() );

		//
		// Create constraint viewer
		//
		m_debugViewerNames.pushBack( hkpConstraintViewer::getName() );
		
		setupGraphics();
	}

	//
	// Some vectors to be used for setup
	//
	hkVector4 halfSize(0.5f, 0.5f, 0.5f);
	hkVector4 size;
	size.setMul4(2.0f, halfSize);
	hkVector4 position(0.0f, 0.0f, 0.0f);
	hkReal xSpacing = 3;
	hkReal ySpacing = size(1) + 0.1f;

	//
	// Create Box Shape
	//
	hkpBoxShape* boxShape;
	{
		boxShape = new hkpBoxShape( halfSize , 0 );
	}

	const int numPairs = 10;
	for (int i = 0; i < numPairs; ++i)
	{
		position(1) = 0;

		//
		// Create fixed rigid body
		//
		hkpRigidBody* rb0;
		{
			hkpRigidBodyCinfo info;
			info.m_position.set(i * xSpacing, 0.0f, 0.0f);
			info.m_shape = boxShape;
			info.m_motionType = hkpMotion::MOTION_FIXED;
			rb0 = new hkpRigidBody(info);
			m_world->addEntity(rb0);
			rb0->removeReference();	
		}

		for (int k=0; k<10; k++)
		{

			hkVector4 positionRb0 = position;

			position(1) -= ySpacing;

			//
			// Create movable rigid body
			//
			hkpRigidBody* rb1;
			{

				hkpRigidBodyCinfo info;
				info.m_position = position;
				info.m_shape = boxShape;
				
					// Compute the box inertia tensor
				hkpMassProperties massProperties;
				info.m_mass = 10.0f;
				hkpInertiaTensorComputer::computeBoxVolumeMassProperties(halfSize, info.m_mass, massProperties);
				info.m_inertiaTensor = massProperties.m_inertiaTensor;
				info.m_centerOfMass = massProperties.m_centerOfMass;
				info.m_motionType = hkpMotion::MOTION_BOX_INERTIA;

				rb1 = new hkpRigidBody(info);
				m_world->addEntity(rb1);
				rb1->removeReference();	
			}

		
			//
			// CREATE BALL AND SOCKET CONSTRAINT
			// 
			hkpBallAndSocketConstraintData* bs;
			{
				
				// Create the constraint
				bs = new hkpBallAndSocketConstraintData(); 

				// Set the pivot
				hkVector4 pivot = positionRb0;
				bs->setInWorldSpace(rb1->getTransform(), rb0->getTransform(), pivot);

				hkpConstraintInstance* constraint = new hkpConstraintInstance(rb1, rb0, bs);

				m_world->addConstraint( constraint); 	
				constraint->removeReference();
			}
			bs->removeReference(); 

			rb0 = rb1;

		}

		position(0) += xSpacing;

	}

	//  Remove reference from shape
	boxShape->removeReference();

	//
	//	As our demo does not use any references to the ball socket constraint, we can remove our reference
	//

	m_world->unlock();

}

BallAndSocketChainsDemo::~BallAndSocketChainsDemo()
{
	// Re-enable warning
	hkError::getInstance().setEnabled(0x2a1db936,true);
}

hkDemo::Result BallAndSocketChainsDemo::stepDemo()
{
	return hkDefaultPhysicsDemo::stepDemo();
}

#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif


HK_DECLARE_DEMO( BallAndSocketChainsDemo, HK_DEMO_TYPE_PRIME, "Several rows of boxes ties together with ball and socket constraints.", "" ); 


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
