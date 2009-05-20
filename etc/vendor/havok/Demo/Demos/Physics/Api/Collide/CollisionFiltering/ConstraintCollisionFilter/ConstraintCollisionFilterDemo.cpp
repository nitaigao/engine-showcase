/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


#include <Demos/demos.h>

// We will need these collision agents
//#include <hkcollide/agent/boxbox/hkpBoxBoxAgent.h>

#include <Physics/Utilities/Collide/Filter/constraint/hkpConstraintCollisionFilter.h>

#include <Physics/Dynamics/Constraint/Bilateral/Hinge/hkpHingeConstraintData.h>

// We want to turn on constraint viewers for this demo
//#include <hkutilities/visualdebugger/viewer/hkpConstraintViewer.h>

#include <Demos/Physics/Api/Collide/CollisionFiltering/ConstraintCollisionFilter/ConstraintCollisionFilterDemo.h>


ConstrainedSystemCollisionFilterDemo::ConstrainedSystemCollisionFilterDemo(hkDemoEnvironment* env)
:	hkDefaultPhysicsDemo(env)
{

	//
	// Setup the camera
	//
	{
		hkVector4 from(0.0f, 0.0f, 10.0f);
		hkVector4 to(0.0f, 0.0f, 0.0f);
		hkVector4 up(0.0f, 1.0f, 0.0f);
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
		}
		m_world = new hkpWorld( info );
		m_world->lock();

		hkpAgentRegisterUtil::registerAllAgents( m_world->getCollisionDispatcher() );

		//m_debugViewerNames.pushBack( hkpConstraintViewer::getName() );

		setupGraphics();
	}

	//
	//	Create constrained-system collision filter.
	//
	hkpConstraintCollisionFilter* filter;
	{
		filter = new hkpConstraintCollisionFilter();
		filter->updateFromWorld(m_world);

		m_world->setCollisionFilter(filter);
		m_world->addConstraintListener(filter);
		filter->removeReference();
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
	hkReal radius ;
	hkpBoxShape* boxShape;
	{
		boxShape = new hkpBoxShape( halfSize , 0 );
		radius = boxShape->getRadius();
	}

	//
	// Create fixed rigid body
	//
	hkpRigidBody* rb0;
	{
		hkpRigidBodyCinfo info;
		info.m_position.set(0.0f, 0.0f, 0.0f);
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

	// Remove reference from box shape since rigid bodies now have ownership
	boxShape->removeReference();

	//
	// CREATE HINGE CONSTRAINT
	// 
	hkpHingeConstraintData* hc = new hkpHingeConstraintData();
	{
		// Set the pivot
		hkVector4 pivot;		
		pivot.setAdd4(position, halfSize);

		// Move pivot to center of side on cube
		pivot(0) -= size(0);
		pivot(2) -= halfSize(2);

		hkVector4 axis(0.0f, 0.0f, 1.0f);

		// Create constraint
		hc->setInWorldSpace(rb0->getTransform(), rb1->getTransform(), pivot, axis);		

		hkpConstraintInstance* constraint = new hkpConstraintInstance(rb0, rb1, hc);
		m_world->addConstraint( constraint); 
		constraint->removeReference();

		hc->removeReference();  
	}

	m_world->unlock();
}


#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif


static const char helpString[] = \
	"This demo shows the use of the hkpConstraintCollisionFilter. Use this filter to disable collisions between all pairs of objects that " \
	"are connected with a constraint (excluding contact point constraints). Note that collisions between unconnected objects will still " \
	"occur unless you disable them using any of the other available filters.";


HK_DECLARE_DEMO(ConstrainedSystemCollisionFilterDemo, HK_DEMO_TYPE_PRIME, "A filter between constrained objects.", helpString);

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
