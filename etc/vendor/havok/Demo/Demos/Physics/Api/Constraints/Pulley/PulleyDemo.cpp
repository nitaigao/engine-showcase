/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/Physics/Api/Constraints/Pulley/PulleyDemo.h>


// We will need these collision agents
#include <Physics/Collide/Agent/ConvexAgent/BoxBox/hkpBoxBoxAgent.h>

// We need to create a constraint
//#include <hkdynamics/constraint/bilateral/Pulley/hkpPulleyConstraintData.h>

#include <Physics/Dynamics/Constraint/Pulley/hkpPulleyConstraintData.h>

// We want to turn on constraint viewers for this demo
#include <Physics/Utilities/VisualDebugger/Viewer/Dynamics/hkpConstraintViewer.h>

#include <Physics/Dynamics/Constraint/hkpConstraintInstance.h>

#include <Common/Visualize/hkDebugDisplay.h>



PulleyDemo::PulleyDemo(hkDemoEnvironment* env)
:	hkDefaultPhysicsDemo(env)
{

	//
	// Setup the camera
	//
	{
		hkVector4 from(0.0f, -15.0f, 0.0f);
		hkVector4 to(0.0f, 0.0f, 0.0f);
		hkVector4 up(0.0f, 0.0f, 1.0f);
		setupDefaultCameras( env, from, to, up );
	}

	//
	// Create the world
	//
	{
		hkpWorldCinfo info;
		info.setupSolverInfo(hkpWorldCinfo::SOLVER_TYPE_4ITERS_MEDIUM); 
		info.setBroadPhaseWorldSize( 3000.0f );
		info.m_enableDeactivation = false;
		info.m_gravity.set(0,0,-10);
		m_world = new hkpWorld( info );
		m_world->lock();

		hkpAgentRegisterUtil::registerAllAgents( m_world->getCollisionDispatcher() );

		//
		// Create constraint viewer 
		//
		m_debugViewerNames.pushBack( hkpConstraintViewer::getName() );
		hkpConstraintViewer::m_scale = 1.0f;
		
		setupGraphics();
	}


	{
		//
		// Create Box Shape
		//
		hkVector4 halfSize(0.5f, 0.5f, 0.5f);
		hkpBoxShape* boxShape = new hkpBoxShape( halfSize , 0 );
		
		//
		// Create movable rigid bodies
		//
		hkpRigidBody* moveableBody0;
		hkpRigidBody* moveableBody1;
		{

			hkpRigidBodyCinfo info;
			info.m_shape = boxShape;
			
				// Compute the box inertia tensor
			hkpInertiaTensorComputer::setShapeVolumeMassProperties(boxShape, info.m_mass, info);
			info.m_motionType = hkpMotion::MOTION_BOX_INERTIA;

			hkVector4 from(1.0f, 1.0f, 1.0f);
			from.normalize3();
			hkVector4 to(0.0f, 0.0f, 1.0f);
			info.m_rotation.setShortestRotation( from, to );
			info.m_rotation.normalize();

			info.m_position.set(-3.0f, 0.0f, -2.25f);
			moveableBody0 = new hkpRigidBody(info);

			info.m_position.set(3.0f, 0.0f, -1.25f);
			moveableBody1 = new hkpRigidBody(info);

			m_world->addEntity(moveableBody0);
			m_world->addEntity(moveableBody1);

			boxShape->removeReference();

		}

		//
		// Create the pulley constraint
		// 
		hkVector4 worldPivots[2];
		worldPivots[0] = hkVector4(-3.0,0,1);
		worldPivots[1] = hkVector4(3.0,0,1);
		{
			
			hkpPulleyConstraintData* pulley = new hkpPulleyConstraintData(); 

			hkVector4 bodyPivots[2];
			bodyPivots[0] = halfSize;
			bodyPivots[1] = halfSize;
			hkReal leverageOnBodyB = 3.0f;

			pulley->setInBodySpace(moveableBody1->getTransform(), moveableBody0->getTransform(), 
									bodyPivots[1], bodyPivots[0], 
									worldPivots[1], worldPivots[0], 
									leverageOnBodyB );

			pulley->setRopeLength(9.0f);

			hkpConstraintInstance* constraint = new hkpConstraintInstance(moveableBody1, moveableBody0, pulley);

			m_world->addConstraint( constraint); 	
			constraint->removeReference();

			pulley->removeReference();
		}
		//
		// Remove references to local rigidBodies, we do not need them here anymore
		//
		moveableBody0->removeReference(); 
		moveableBody1->removeReference();

		//
		// Helper bodies preventing the constrained bodies from getting too close to the pulley pivot points.
		//
		hkpRigidBodyCinfo info;
		info.m_motionType = hkpMotion::MOTION_FIXED;
		info.m_position = worldPivots[0];
		info.m_shape = new hkpSphereShape(0.3f);
		hkpRigidBody* body = new hkpRigidBody(info);
		m_world->addEntity(body);
		body->removeReference();

		info.m_position = worldPivots[1];
		body = new hkpRigidBody(info);
		m_world->addEntity(body);
		body->removeReference();

		info.m_shape->removeReference();
	}

	m_world->unlock();
}





#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
"Example usage: A rope holding a lamp at the ceiling.";

HK_DECLARE_DEMO(PulleyDemo, HK_DEMO_TYPE_PRIME, "A single Pulley constraint, with viewer", helpString);

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
