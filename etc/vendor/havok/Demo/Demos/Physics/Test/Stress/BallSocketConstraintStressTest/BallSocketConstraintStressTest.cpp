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

#include <Demos/Physics/Test/Stress/BallSocketConstraintStressTest/BallSocketConstraintStressTest.h>

#include <Physics/Collide/Shape/Convex/ConvexTranslate/hkpConvexTranslateShape.h>
#include <Physics/Collide/Shape/Compound/Collection/List/hkpListShape.h>


BallSocketConstraintStressTest::BallSocketConstraintStressTest(hkDemoEnvironment* env)
:	hkDefaultPhysicsDemo(env)
{
	
	// Disable warnings:
	hkError::getInstance().setEnabled(0x3ad17e8b, false); //'Have you called hkpAgentRegisterUtil::registerAllAgents? Do not know how to get closest points between HK_SHAPE_BOX and HK_SHAPE_BOX types.'
	hkError::getInstance().setEnabled(0x3ad17e8a, false); 

	//
	// Setup the camera
	//
	{
		hkVector4 from(0.0f, -10.0f, 30.0f);
		hkVector4 to  (0.0f, -10.0f,  0.0f);
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
			info.setBroadPhaseWorldSize( 100000.0f );
			info.m_enableDeactivation = false;
		}
		m_world = new hkpWorld(info);

		m_world->lock();

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
	//hkVector4 position(size(0), -size(1) -0.1f, 0);

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

	hkpRigidBody* prevBody = fixedBody;
	for (int i = 1; i < 11; i++)
	{
		//
		// Create movable rigid body
		//
		hkpRigidBody* moveableBody;
		{

			hkpRigidBodyCinfo info;
			{
				hkReal mass = 10.0f;

				//info.m_position.set(0.0f, -size(1)*4.0f, 0.0f);
				info.m_position.set(-size(1)*2.0f * i, 0.0f, 0.0f);

				info.m_shape = boxShape;
				hkpInertiaTensorComputer::setShapeVolumeMassProperties(boxShape, mass, info);
				info.m_motionType		= hkpMotion::MOTION_BOX_INERTIA;

				//info.m_angularDamping = 0.8f;
			}

			info.m_inertiaTensor.mul(100.0f);

			moveableBody = new hkpRigidBody(info);
			m_world->addEntity(moveableBody);
		}



		//
		// Create ball and socket constraint.
		// 
		{
			// Set the pivot
			hkVector4 pivot;
			pivot.setAdd4(prevBody->getPosition(), moveableBody->getPosition());
			pivot.mul4(0.5f);

			// Create the constraint
			hkpBallAndSocketConstraintData* data = new hkpBallAndSocketConstraintData(); 
			data->setInWorldSpace(moveableBody->getTransform(), prevBody->getTransform(), pivot);
			hkpConstraintInstance* constraint = new hkpConstraintInstance(moveableBody, prevBody, data);
			m_world->addConstraint( constraint); 	
			data->removeReference();
			constraint->removeReference();
		}

		prevBody = moveableBody;
		prevBody->removeReference();
	}

	boxShape->removeReference();



	m_world->unlock();
}

BallSocketConstraintStressTest::Result BallSocketConstraintStressTest::stepDemo()
{
	for (int i = 0; i < 10; i++)
	{
		hkDefaultPhysicsDemo::stepDemo();
	}
	return DEMO_OK;
}

BallSocketConstraintStressTest::~BallSocketConstraintStressTest()
{
	// Re-enable warning
	hkError::getInstance().setEnabled(0x3ad17e8b, true);
	hkError::getInstance().setEnabled(0x3ad17e8a, true);
}

#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif


HK_DECLARE_DEMO( BallSocketConstraintStressTest, HK_DEMO_TYPE_PRIME | HK_DEMO_TYPE_CRITICAL, "Shows behavior of 10 ball-socket constraints chained together.", "" ); 


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
