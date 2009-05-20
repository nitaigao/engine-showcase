/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>

#include <Physics/Collide/Agent/ConvexAgent/BoxBox/hkpBoxBoxAgent.h>
#include <Physics/Dynamics/Constraint/Bilateral/BallAndSocket/hkpBallAndSocketConstraintData.h>
#include <Physics/Dynamics/Constraint/hkpConstraintInstance.h>
#include <Physics/Utilities/VisualDebugger/Viewer/Dynamics/hkpConstraintViewer.h>

#include <Demos/Physics/Api/Constraints/BallAndSocket/BallAndSocketDemo.h>


BallAndSocketDemo::BallAndSocketDemo(hkDemoEnvironment* env)
:	hkDefaultPhysicsDemo(env)
{
	//
	// Setup the camera
	//
	{
		hkVector4 from(0.0f, 0.0f, 10.0f);
		hkVector4 to  (0.0f, 0.0f,  0.0f);
		hkVector4 up  (0.0f, 1.0f,  0.0f);
		setupDefaultCameras( env, from, to, up );
	}

	//
	// Create the world
	//
	{
		hkpWorldCinfo info;
		{
			info.setupSolverInfo(hkpWorldCinfo::SOLVER_TYPE_4ITERS_MEDIUM); 
			info.setBroadPhaseWorldSize( 100.0f );
			info.m_enableDeactivation = false;
		}
		m_world = new hkpWorld(info);

		m_world->lock();

		// Register the single agent required (a hkBoxBoxAgent)
		hkpBoxBoxAgent::registerAgent(m_world->getCollisionDispatcher());

		//
		// Create constraint viewer
		//
		m_debugViewerNames.pushBack( hkpConstraintViewer::getName() );
		hkpConstraintViewer::m_scale = 1.0f;

		setupGraphics();
	}

	//
	// Some vectors to be used for setup
	//
	hkVector4 halfSize(0.5f, 0.5f, 0.5f);
	hkVector4 size;
	size.setMul4(2.0f, halfSize);
	hkVector4 position(size(0), -size(1) -0.1f, 0);

	//
	// Create Box Shape
	//
	hkpBoxShape* boxShape = new hkpBoxShape(halfSize , 0);

	//
	// Create fixed rigid body
	//
	hkpRigidBody* fixedBody;
	{
		hkpRigidBodyCinfo info;
		{
			info.m_position		. set(0.0f, 0.0f, 0.0f);
			info.m_shape		= boxShape;
			info.m_motionType	= hkpMotion::MOTION_FIXED;
		}

		fixedBody = new hkpRigidBody(info);
		m_world->addEntity(fixedBody);
		fixedBody->removeReference();	
	}

	//
	// Create movable rigid body
	//
	hkpRigidBody* moveableBody;
	{

		hkpRigidBodyCinfo info;
		{
			hkReal mass = 10.0f;

			// Compute the box inertia tensor
			hkpMassProperties massProperties;
			hkpInertiaTensorComputer::computeBoxVolumeMassProperties(halfSize, mass, massProperties);

			info.m_position			= position;
			info.m_shape			= boxShape;
			info.m_mass				= mass;
			info.m_inertiaTensor	= massProperties.m_inertiaTensor;
			info.m_centerOfMass		= massProperties.m_centerOfMass;
			info.m_motionType		= hkpMotion::MOTION_BOX_INERTIA;
		}

		moveableBody = new hkpRigidBody(info);
		m_world->addEntity(moveableBody);
		moveableBody->removeReference();	
	}

	//  Remove reference from shape
	boxShape->removeReference();

	//
	// Create ball and socket constraint.
	// 
	{
		// Set the pivot
		hkVector4 pivot;
		pivot.setAdd4(position, halfSize);
		pivot(0) -= size(0);	// Move pivot to corner of cube

		// Create the constraint
		hkpBallAndSocketConstraintData* data = new hkpBallAndSocketConstraintData(); 
		data->setInWorldSpace(moveableBody->getTransform(), fixedBody->getTransform(), pivot);
		hkpConstraintInstance* constraint = new hkpConstraintInstance(moveableBody, fixedBody, data);
		m_world->addConstraint( constraint); 	
		data->removeReference();
		constraint->removeReference();
	}

	m_world->unlock();
}


#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif


HK_DECLARE_DEMO( BallAndSocketDemo, HK_DEMO_TYPE_PRIME, "Simple example of a ball & socket constraint.", "" ); 


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
