/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

//
///	this demo demonstrates the use of ListShape, TransformShape and HingeConstraint
//
#include <Demos/demos.h>

#define NUM_TEETH 36
#define mScaling 100.0f 

#include <Physics/Collide/Shape/Convex/ConvexTransform/hkpConvexTransformShape.h>
#include <Physics/Collide/Shape/Compound/Collection/List/hkpListShape.h>

#include <Physics/Dynamics/Constraint/Bilateral/Hinge/hkpHingeConstraintData.h>

#include <Demos/Physics/UseCase/Machine/Pinion/PinionDemo.h>
#include <Demos/Physics/UseCase/Machine/Pinion/PinionMotorAction.h>

#include <Physics/Collide/Shape/Compound/Tree/Mopp/hkpMoppUtility.h>
#include <Physics/Internal/Collide/Mopp/Code/hkpMoppCode.h>

	// Used to generate a convex hull for convexVerticesShape
#include <Common/Internal/ConvexHull/hkGeometryUtility.h>

hkpShape* PinionDemo::createPinionShape()
{
	hkArray<hkVector4> vertices;
	vertices.setSize(32);

	vertices[0].setMul4(  mScaling, hkVector4(0.536f,	0.0f,	0.02f) );
	vertices[1].setMul4(  mScaling, hkVector4(0.541f,	0.0f,	-0.003f) );
	vertices[2].setMul4(  mScaling, hkVector4(0.555f,	0.0f,	-0.003f) );
	vertices[3].setMul4(  mScaling, hkVector4(0.564f,	0.0f,	-0.002f) );
	vertices[4].setMul4(  mScaling, hkVector4(0.573f,	0.0f,	0.0f) );
	vertices[5].setMul4(  mScaling, hkVector4(0.589f,	0.0f,	0.005f) );
	vertices[6].setMul4(  mScaling, hkVector4(0.605f,	0.0f,	0.012f) );
	vertices[7].setMul4(  mScaling, hkVector4(0.605f,	0.0f,	0.0260f) );

	vertices[8].setMul4(  mScaling, hkVector4(0.536f,	0.1f,	0.02f) );
	vertices[9].setMul4(  mScaling, hkVector4(0.541f,	0.1f,	-0.003f) );
	vertices[10].setMul4( mScaling, hkVector4(0.555f,	0.1f,	-0.003f) );
	vertices[11].setMul4( mScaling, hkVector4(0.564f,	0.1f,	-0.002f) );
	vertices[12].setMul4( mScaling, hkVector4(0.573f,	0.1f,	0.0f) );
	vertices[13].setMul4( mScaling, hkVector4(0.589f,	0.1f,	0.005f) );
	vertices[14].setMul4( mScaling, hkVector4(0.605f,	0.1f,	0.012f) );
	vertices[15].setMul4( mScaling, hkVector4(0.605f,	0.1f,	0.0260f) );

	{
		hkRotation rot;
		hkVector4 axis(0.0f,1.0f,0.0f);
		hkReal angle = (2.0f*3.14159265380f)/NUM_TEETH  *0.5f;
		rot.setAxisAngle(axis, angle);
		for (int i=0;i<16;i++)
		{
			vertices[i+16]._setRotatedDir(rot,vertices[i]);
			vertices[i+16](2)*=-1;
		}
	}


	hkpConvexVerticesShape* shape;
	hkArray<hkVector4> planeEquations;
	hkGeometry geom;
	{
		hkStridedVertices stridedVerts;
		{
			stridedVerts.m_numVertices = vertices.getSize();
			stridedVerts.m_striding = sizeof(hkVector4);
			stridedVerts.m_vertices = &(vertices[0](0));
		}

		hkGeometryUtility::createConvexGeometry( stridedVerts, geom, planeEquations );

		{
			stridedVerts.m_numVertices = geom.m_vertices.getSize();
			stridedVerts.m_striding = sizeof(hkVector4);
			stridedVerts.m_vertices = &(geom.m_vertices[0](0));
		}

		shape = new hkpConvexVerticesShape(stridedVerts, planeEquations);
	}

	
	hkpShape* shapes[ NUM_TEETH ];
	for (int kk=0;kk<NUM_TEETH  ;kk++)
	{
		hkTransform trans;
		{
			trans.setIdentity();
			hkRotation rot2;
			hkVector4 axis(0.0f,1.0f,0.0f);
			hkReal angle = (2.0f*HK_REAL_PI)/NUM_TEETH * kk;
			rot2.setAxisAngle(axis,angle);
			trans.setRotation(rot2);
		}

		hkpConvexTransformShape* transshape = new hkpConvexTransformShape(shape, trans);
		shapes[kk] = transshape;
	}

	hkpListShape*  list = new hkpListShape(shapes,NUM_TEETH );

	for (int j = 0; j < NUM_TEETH; ++j)
	{
		shapes[j]->removeReference();
	}

	shape->removeReference();

	//
	//	Build mopp shape
	// 
	hkpMoppBvTreeShape* moppShape;
	{
		hkpMoppCompilerInput mci;
		hkpMoppCode* code = hkpMoppUtility::buildCode( list ,mci);

		moppShape = new hkpMoppBvTreeShape(list, code);

		list->removeReference();
		code->removeReference();
	}
	return moppShape;
}

/// this adds a motor to one wheel so it will rotate
hkpAction* PinionDemo::addMotorAction(hkpRigidBody *body)
{

	hkVector4 axis(0.0f, 1.0f, 0.0f);
	hkReal angVel(0.1f);
	hkReal gain(10.0f);

	PinionMotorAction* motorAction = new PinionMotorAction(body, axis, angVel, gain);
	m_world->addAction(motorAction);
	motorAction->removeReference();

	return motorAction;
}


void PinionDemo::constrainCog(hkpRigidBody *body)
{
	hkpRigidBody* fixedRigidBody = m_world->getFixedRigidBody();
	hkVector4 pos = body->getPosition();
	hkVector4 axis(0.0f,1.0f,0.0f);
	{
		hkpHingeConstraintData* bas = new hkpHingeConstraintData();
		bas->setInWorldSpace(body->getTransform(), fixedRigidBody->getTransform(), pos, axis );
		m_world->createAndAddConstraintInstance( body, fixedRigidBody, bas )->removeReference(); 
		bas->removeReference();
	}
}

//
//	Pinion demo demonstrates the use of ListShape, TransformShape and Hinge Constraint
// Constructor

PinionDemo::PinionDemo(hkDemoEnvironment* env)
	:	hkDefaultPhysicsDemo(env, DEMO_FLAGS_NO_SERIALIZE)
{
	//
	// Setup the camera
	//
	{
		hkVector4 from(10.0f, 10.0f, -30.0f);
		hkVector4 to  ( 0.0f,  0.0f,   0.0f);
		hkVector4 up  ( 0.0f,  1.0f,   0.0f);
		setupDefaultCameras( env, from, to, up );
	}

	//
	// Create the world
	//
	{
		hkpWorldCinfo info;
		info.m_gravity.set(0.0f, -9.5f, 0.0f);
		info.setBroadPhaseWorldSize( 500.0f );
		info.setupSolverInfo(hkpWorldCinfo::SOLVER_TYPE_4ITERS_MEDIUM);
		info.m_collisionTolerance = 0.02f;
		info.m_deactivationReferenceDistance = 0.0001f;
		m_world = new hkpWorld( info );
		m_world->lock();

		// Register ALL agents (though some may not be necessary)
		hkpAgentRegisterUtil::registerAllAgents(m_world->getCollisionDispatcher());

		setupGraphics();
	}


	//create Pinion Shape and Rigidbody
	float mass = 10.0f;
	hkpRigidBodyCinfo rbci;
	{
		rbci.m_shape = createPinionShape();

		rbci.m_motionType = hkpMotion::MOTION_BOX_INERTIA;
		rbci.m_friction = 0.02f;
		rbci.m_processContactCallbackDelay = 0;
		rbci.m_angularDamping = 0.5f;
		rbci.m_linearDamping  = HK_REAL_PI;
		rbci.m_mass = 1.0f;
		rbci.m_position = hkVector4(-60.0f, -20.0f, 40.0f);
		rbci.m_solverDeactivation = hkpRigidBodyCinfo::SOLVER_DEACTIVATION_OFF;
		hkpMassProperties mp;
		hkAabb aabb;
		rbci.m_shape->getAabb(hkTransform::getIdentity(), 0.0f, aabb);
		hkVector4 extent; extent.setSub4(aabb.m_max, aabb.m_min);
		hkReal radius = extent.length3();
		hkpInertiaTensorComputer::computeSphereVolumeMassProperties( radius, mass, mp);
		rbci.m_inertiaTensor = mp.m_inertiaTensor;
		rbci.m_centerOfMass  = mp.m_centerOfMass;
	}

	hkpRigidBody* rb = new hkpRigidBody(rbci);

	rbci.m_shape->removeReference();
	
	//
	// add a Motor and a HingeConstraint to Pinion
	//
	m_world->addEntity(rb);
	rb->removeReference();

	addMotorAction(rb);
	constrainCog(rb);

	// create the other Pinion with a small translation + Hinge
	rbci.m_position(0) += 115.0f;
	hkVector4 axis(0.0f, 1.0f, 0.0f);
	hkReal angle = (2.0f * HK_REAL_PI)/NUM_TEETH;
	rbci.m_rotation.setAxisAngle(axis, angle);
	
	rb = new hkpRigidBody(rbci);
	m_world->addEntity(rb);
	rb->removeReference();
	constrainCog(rb);

	// Set mouse spring to be strong enough to slow down the wheel.
	m_oldMouseSpringMaxRelativeForce = m_mouseSpringMaxRelativeForce;
	m_mouseSpringMaxRelativeForce = 20000.0f;

	m_world->unlock();
}


PinionDemo::~PinionDemo()
{
	//m_env->m_drawWireframe = true;
	m_mouseSpringMaxRelativeForce = m_oldMouseSpringMaxRelativeForce;
}


#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
"A classic (simulated) Rack and Pinion system";

HK_DECLARE_DEMO(PinionDemo, HK_DEMO_TYPE_PRIME, "Pinion", helpString);

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
