/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/Physics/Api/Constraints/LimitedHinge/LimitedHingeDemo.h>


// We will need these collision agents
#include <Physics/Collide/Agent/ConvexAgent/BoxBox/hkpBoxBoxAgent.h>

// We need to create a constraint
#include <Physics/Dynamics/Constraint/Bilateral/LimitedHinge/hkpLimitedHingeConstraintData.h>

// We want to turn on constraint viewers for this demo
#include <Physics/Utilities/VisualDebugger/Viewer/Dynamics/hkpConstraintViewer.h>


LimitedHingeDemo::LimitedHingeDemo(hkDemoEnvironment* env)
:	hkDefaultPhysicsDemo(env)
{

	//
	// Setup the camera
	//
	{
		hkVector4 from(0.0f, 0.0f, 10.0f);
		hkVector4 to  (0.0f, 0.0f, 0.0f);
		hkVector4 up  (0.0f, 1.0f, 0.0f);
		setupDefaultCameras( env, from, to, up );
	}

	//
	// Create the world
	//
	{
		hkpWorldCinfo info;
		info.setupSolverInfo(hkpWorldCinfo::SOLVER_TYPE_8ITERS_HARD); 
		info.setBroadPhaseWorldSize( 100.0f );
		info.m_enableDeactivation = false;
		m_world = new hkpWorld( info );
		m_world->lock();

		//
		// Create constraint viewer
		//
		m_debugViewerNames.pushBack( hkpConstraintViewer::getName() );
		hkpConstraintViewer::m_scale = 1.0f;

		setupGraphics();

		// Register the single agent required (a hkpBoxBoxAgent)
		hkpAgentRegisterUtil::registerAllAgents( m_world->getCollisionDispatcher() );
	}

	//
	// Create vectors to be used for setup
	//
	hkVector4 halfSize(0.5f, 0.5f, 0.5f);
	hkVector4 size;
	size.setMul4(2.0f, halfSize);
	hkVector4 position(size(0), -size(1) -0.1f, 0.0f);


	//
	// Create Box Shape
	//
	hkpBoxShape* boxShape;
	{
		boxShape = new hkpBoxShape( halfSize , 0 );
	}

	//
	// Get fixed rigid body
	//
	hkpRigidBody* fixedBody;
	{
		fixedBody = m_world->getFixedRigidBody();
	}



	//
	// Create movable rigid body
	//
	hkpRigidBody* moveableBody;
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

		moveableBody = new hkpRigidBody(info);
		m_world->addEntity(moveableBody);
		moveableBody->removeReference();	
	}

	//
	//	Cleanup references to shared shapes
	//
	boxShape->removeReference();
	boxShape = HK_NULL;


	//
	// CREATE LIMITED HINGE CONSTRAINT
	// 
	{
		hkpLimitedHingeConstraintData* lhc = new hkpLimitedHingeConstraintData();

		// Set the pivot
		hkVector4 pivot;		
		pivot.setAdd4(position, halfSize);

		// Move pivot to center of side on cube
		pivot(0) -= size(0);	
		pivot(2) -= halfSize(2);

		hkVector4 axis(0.0f, 0.0f, 1.0f);

		// Create constraint
		lhc->setInWorldSpace(moveableBody->getTransform(), fixedBody->getTransform(), pivot, axis);
		lhc->setMinAngularLimit(-HK_REAL_PI/3.0f);
		lhc->setMaxAngularLimit(HK_REAL_PI/4.0f);
		

		{
			hkpConstraintInstance* constraint = new hkpConstraintInstance( moveableBody, fixedBody, lhc );
			m_world->addConstraint(constraint);
			constraint->removeReference();
		}
		
		lhc->removeReference();  
	}

	m_world->unlock();
}


LimitedHingeDemo::~LimitedHingeDemo()
{

}

hkDemo::Result LimitedHingeDemo::stepDemo()
{
	return hkDefaultPhysicsDemo::stepDemo();
}

////////////////////////////////////////////////////////////////////////////////

#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
"The hinge is limited between -PI/3 and PI/4.\n"
"Note: the moving body is constraint to an invisible global body (see hkpWorld::getFixedRigidBody())";

HK_DECLARE_DEMO(LimitedHingeDemo, HK_DEMO_TYPE_PRIME, "A single Limited Hinge constraint, with viewer", helpString);

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
