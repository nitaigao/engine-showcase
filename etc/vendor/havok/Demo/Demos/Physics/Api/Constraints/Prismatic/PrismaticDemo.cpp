/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/Physics/Api/Constraints/Prismatic/PrismaticDemo.h>


// We will need these collision agents
#include <Physics/Collide/Agent/ConvexAgent/BoxBox/hkpBoxBoxAgent.h>

// We need to create a constraint
#include <Physics/Dynamics/Constraint/Bilateral/Prismatic/hkpPrismaticConstraintData.h>

// We want to turn on constraint viewers for this demo
#include <Physics/Utilities/VisualDebugger/Viewer/Dynamics/hkpConstraintViewer.h>


PrismaticDemo::PrismaticDemo(hkDemoEnvironment* env)
:	hkDefaultPhysicsDemo(env)
{

	//
	// Setup the camera
	//
	{
		hkVector4 from(10.0f, 20.0f, 30.0f);
		hkVector4 to  ( 0.0f,  0.0f,  0.0f);
		hkVector4 up  ( 0.0f,  1.0f,  0.0f);
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
		m_world = new hkpWorld( info );
		m_world->lock();

				// Register the single agent required (a hkpBoxBoxAgent)
		hkpAgentRegisterUtil::registerAllAgents( m_world->getCollisionDispatcher() );

		//
		// Create constraint viewer
		//
		m_debugViewerNames.pushBack( hkpConstraintViewer::getName() );
		hkpConstraintViewer::m_scale = 1.0f;
		
		setupGraphics();
	}

	//
	// Create vectors to be used for setup
	//
	hkVector4 position(0.0f, 0.0f, 0.0f);
	hkVector4 box1Size(1.0f, 10.0f, 0.5f);
	hkVector4 box2Size(.4f, 2.5f, .25f);
		
	//
	// Create Box Shapes
	//
	hkpBoxShape* boxShape1;
	hkpBoxShape* boxShape2;
	{
		boxShape1 = new hkpBoxShape( box1Size , 0 );
		boxShape2 = new hkpBoxShape( box2Size , 0 );
	}

	//
	// Create fixed rigid body
	//
	hkpRigidBody* fixedBody;
	{
		hkpRigidBodyCinfo info;
		info.m_position = position;
		info.m_shape = boxShape1;
		info.m_motionType = hkpMotion::MOTION_FIXED;
		fixedBody = new hkpRigidBody(info);
		m_world->addEntity(fixedBody);
		fixedBody->removeReference();	
	}


	//
	// Create movable rigid body
	//
	hkpRigidBody* movingBody;
	{
		position(2) += 1.0f;

		hkpRigidBodyCinfo info;
		info.m_position = position;
		info.m_shape = boxShape2;
		
			// Compute the box inertia tensor
		hkpMassProperties massProperties;
		info.m_mass = 10.0f;
		hkpInertiaTensorComputer::computeBoxVolumeMassProperties(box2Size, info.m_mass, massProperties);
		info.m_inertiaTensor = massProperties.m_inertiaTensor;
		info.m_centerOfMass = massProperties.m_centerOfMass;	
		info.m_motionType = hkpMotion::MOTION_BOX_INERTIA;

		movingBody = new hkpRigidBody(info);
		m_world->addEntity(movingBody);
		movingBody->removeReference();	
	}

	//
	//	Cleanup references to shared shapes
	//
	boxShape1->removeReference();
	boxShape2->removeReference();
	boxShape1 = HK_NULL;
	boxShape2 = HK_NULL;

	
	//
	// CREATE PRISMATIC CONSTRAINT
	// 
	{
		
		hkpPrismaticConstraintData* pris = new hkpPrismaticConstraintData(); 

		// Create constraint
		hkVector4 axis(0.0f, 1.0f, 0.0f);
		pris->setMaxLinearLimit(10.0f);
		pris->setMinLinearLimit(-10.0f);
		pris->setInWorldSpace(movingBody->getTransform(), fixedBody->getTransform(), position, axis);
		
		
		//
		//	Create and add the constraint
		//
		{
			hkpConstraintInstance* constraint = new hkpConstraintInstance( movingBody, fixedBody, pris);
			m_world->addConstraint(constraint);
			constraint->removeReference();
		}		

		pris->removeReference();        
	}

	m_world->unlock();
}

PrismaticDemo::~PrismaticDemo()
{

}


hkDemo::Result PrismaticDemo::stepDemo()
{
	return hkDefaultPhysicsDemo::stepDemo();
}

#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
"Prismatic constraints constrain motion to a line in space.";

HK_DECLARE_DEMO(PrismaticDemo, HK_DEMO_TYPE_PRIME, "A single Prismatic constraint", helpString);

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
