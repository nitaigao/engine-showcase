/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/Physics/Api/Constraints/PointToPath/PointToPathDemo.h>


// We will need these collision agents
#include <Physics/Collide/Agent/ConvexAgent/BoxBox/hkpBoxBoxAgent.h>

// We need to create a constraint
#include <Physics/Dynamics/Constraint/Bilateral/PointToPath/hkpPointToPathConstraintData.h>
#include <Physics/Dynamics/Constraint/Bilateral/PointToPath/hkpLinearParametricCurve.h>

// We want to turn on constraint viewers for this demo
#include <Physics/Utilities/VisualDebugger/Viewer/Dynamics/hkpConstraintViewer.h>


PointToPathDemo::PointToPathDemo(hkDemoEnvironment* env)
:	hkDefaultPhysicsDemo(env)
{

	//
	// Create a helix curve
	//
	{
	    
		m_helix = new hkpLinearParametricCurve();
		const hkReal twists = 3.0f;
		const hkReal height = 15.0f;
		const hkReal radius = 10.0f;
		hkVector4 pt;
		for (hkReal t = 1.0f; t > 0.0f; t -= 1.0f / 300.0f)
		{
			const hkReal angle = t * HK_REAL_PI * 2.0f * twists;
			pt = hkVector4(hkMath::cos( angle ) * radius * ( 1.0f + t), t * height, hkMath::sin( angle ) * radius * (1.0f + t) );

			m_helix->addPoint( pt );
		}
		
	} 


	//
	// Setup the camera
	//
	{
		hkVector4 from(0.0f, 10.0f, 50.0f);
		hkVector4 to  (0.0f,  0.0f,  0.0f);
		hkVector4 up  (0.0f,  1.0f,  0.0f);
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
	hkVector4 halfSize1(1.0f, 1.0f, 1.0f);
	hkVector4 halfSize2(5.0f, 0.25f, 5.0f);

	//
	// Create 2 Box Shape
	//
	hkpBoxShape* boxShape1;
	hkpBoxShape* boxShape2;
	{
		boxShape1 = new hkpBoxShape( halfSize1 , 0 );
		boxShape2 = new hkpBoxShape( halfSize2 , 0 );
	}

	//
	// Create fixed rigid body
	//
	hkpRigidBody* fixedBody;
	{
		hkpRigidBodyCinfo info;
		info.m_position.set(0.0f, 0.0f, 0.0f);
		info.m_shape = boxShape2;
		info.m_motionType = hkpMotion::MOTION_FIXED;
		fixedBody = new hkpRigidBody(info);
		m_world->addEntity(fixedBody);
		fixedBody->removeReference();	
	}

	//
	// Create a transform of the helix in local space of the fixed body
	//
	hkTransform helixTransform;
	{
		helixTransform.getTranslation().set( 0, -3, 0);
		helixTransform.getRotation().setAxisAngle( hkTransform::getIdentity().getColumn(2), 0.3f );
	}

	//
	// Create movable rigid body, positioned at top of helix.
	//
	hkpRigidBody* smallBox;
	{

		hkpRigidBodyCinfo info;

			// Find "start" of helix and position the body here
		hkVector4 pos;		m_helix->getPoint( m_helix->getStart(), pos);
		pos.setTransformedPos( helixTransform, pos );

		info.m_position.setTransformedPos(fixedBody->getTransform(), pos);

		hkVector4 offset( 0,0.4f,0);		info.m_position.add4( offset );

		info.m_rotation.setAxisAngle( hkTransform::getIdentity().getColumn(1), 0.8f );

		info.m_shape = boxShape1;

			// Compute the box inertia tensor
		hkpInertiaTensorComputer::setShapeVolumeMassProperties(boxShape1, 10.0f, info);

		info.m_motionType = hkpMotion::MOTION_SPHERE_INERTIA;
		smallBox = new hkpRigidBody(info);
		hkVector4 linVel(0.0f, 0.0f, -10.0f);
		smallBox->setLinearVelocity(linVel);
		m_world->addEntity(smallBox);
		smallBox->removeReference();	
	}


	//
	//	Cleanup references to shared shapes
	//
	boxShape1->removeReference();
	boxShape2->removeReference();
	boxShape1 = HK_NULL;
	boxShape2 = HK_NULL;

	//
	// Create the point to path constraint
	//
	{
		hkpPointToPathConstraintData* p2Path = new hkpPointToPathConstraintData();

		// constrain angular DOFs so that it can only spin around the path
		p2Path->setOrientationType(hkpPointToPathConstraintData::CONSTRAIN_ORIENTATION_ALLOW_SPIN);
		
		p2Path->setInWorldSpace(smallBox->getTransform(), fixedBody->getTransform(), smallBox->getPosition(), m_helix, helixTransform);
		p2Path->setMaxFrictionForce( .1f );
	

		//
		//	Create and add the constraint
		//
		{
			hkpConstraintInstance* constraint = new hkpConstraintInstance( smallBox, fixedBody, p2Path );
			m_world->addConstraint(constraint);
			constraint->removeReference();
		}		


		p2Path->removeReference();
	}

	m_world->unlock();
}


PointToPathDemo::~PointToPathDemo()
{
	m_helix->removeReference();
}

hkDemo::Result PointToPathDemo::stepDemo()
{
	return hkDefaultPhysicsDemo::stepDemo();
}

#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
"The helix is an instance of our hkpLinearParametricCurve. The object is free to rotate around the axis of the path.";

HK_DECLARE_DEMO(PointToPathDemo, HK_DEMO_TYPE_PRIME, "A single Point-To-Path constraint", helpString);

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
