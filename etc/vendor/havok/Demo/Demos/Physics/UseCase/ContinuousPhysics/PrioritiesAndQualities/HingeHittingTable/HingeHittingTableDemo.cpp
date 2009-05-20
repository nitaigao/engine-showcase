/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/Physics/UseCase/ContinuousPhysics/PrioritiesAndQualities/HingeHittingTable/HingeHittingTableDemo.h>

#include <Physics/Collide/Filter/Group/hkpGroupFilter.h>
#include <Physics/Collide/Filter/Group/hkpGroupFilterSetup.h>

#include <Physics/Dynamics/Constraint/Bilateral/Hinge/hkpHingeConstraintData.h>
#include <Physics/Dynamics/World/hkpPhysicsSystem.h>

#include <Demos/DemoCommon/Utilities/GameUtils/GameUtils.h>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Definitions of game variants
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

enum SceneType
{
	VARIANT_HINGE = 0,
	VARIANT_RAGDOLL,
	VARIANT_RAGDOLL_INCREASED_INERTIA
};

struct HingeHittingTableDemoVariant
{
	const char*							m_name;
	SceneType							m_sceneType;
	hkpCollidableQualityType 			m_objectQualityType;
	hkpConstraintInstance::ConstraintPriority	m_constraintPriorityLevel;
	int m_goRealtime;
	const char* m_details;

};


static const HingeHittingTableDemoVariant g_variants[] =
{
#define ASYNC hkpWorldCinfo::SIMULATION_TYPE_ASYNCHRONOUS
#define CONT  hkpWorldCinfo::SIMULATION_TYPE_CONTINUOUS
#define PSI hkpConstraintInstance::PRIORITY_PSI
#define TOI hkpConstraintInstance::PRIORITY_TOI
#define D HK_COLLIDABLE_QUALITY_DEBRIS
#define M HK_COLLIDABLE_QUALITY_MOVING
	//	**************************************  SimType, frameHz,physicsHz, solverIterations,				InertiaFactor
	{ "Hinge Discrete",                             VARIANT_HINGE,   D,  PSI, 0, "A fast hinge completely tunnels through a static object" },
	{ "Hinge Continuous",                           VARIANT_HINGE,   M,  PSI, 0, "Using continuous collisions but PSI constraints, " \
																		   "the objects hit correctly, however the hinge can separate "\
																		   "causing the two pieces to end up on different sides" },
	{ "Hinge Continuous + Toi constraints",	        VARIANT_HINGE,   M,  TOI, 0, "Increasing the priority of the hinge to TOI "\
																		   "we use more CPU, however we get a really nice quality" },
	{ "Ragdoll Discrete",                           VARIANT_RAGDOLL, D,  PSI, 0, "A discrete ragdoll tunneling" },
	{ "Ragdoll Continuous",                         VARIANT_RAGDOLL, M,  PSI, 0, "A continuous ragdoll with PSI constraints. Not great quality " \
																		   "but relative good CPU performance" },
	{ "Ragdoll Continuous + Toi constraints",       VARIANT_RAGDOLL, M,  TOI, 0, "A continuous ragdoll with TOI constraints. Nice quality" },
	{ "Ragdoll Continuous + Toi constraints (REALTIME)",       VARIANT_RAGDOLL, M,  TOI, 1, "A continuous ragdoll with TOI constraints. Realtime." },
	{ "Ragdoll IncreasedInertia Cont.",             VARIANT_RAGDOLL_INCREASED_INERTIA, M,  PSI, 0, "By increaing the rotation inertia of the ragdoll " \
	                                                                                         "we can increase the quality, thereby reducing the " \
																							 "the need for TOI constraints." },
	{ "Ragdoll IncreasedInertia Cont. + Toi constraints", VARIANT_RAGDOLL_INCREASED_INERTIA, M,  TOI, 0, "Best quality: Toi constraints and increased inertia" }
};
#undef PSI
#undef TOI
#undef D
#undef M

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Local functions for setting up the scene
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static void HK_CALL createHinge(hkpWorld* world, 
								hkpCollidableQualityType bodyQualityType, 
								hkpConstraintInstance::ConstraintPriority constraintPriority,
								hkVector4& position, 
								hkVector4& velocity)
{
	// create to boards connected by a hinge
	hkpRigidBody* bodyA;
	hkpRigidBody* bodyB;
	hkReal pivotY = 0.40f;
	{
		hkVector4 halfSize(.1f, 1.0f, 0.1f);
		hkpConvexShape* shape = new hkpBoxShape(halfSize, 0.01f); 
		
		hkpRigidBodyCinfo ci;
		ci.m_motionType = hkpMotion::MOTION_SPHERE_INERTIA;
		ci.m_shape = shape;
		ci.m_angularDamping = 0.0f;
		ci.m_linearDamping = 0.0f;
		ci.m_restitution = 0.0f;
		ci.m_friction = 0.2f;
		ci.m_linearVelocity = velocity;
		hkpInertiaTensorComputer::setShapeVolumeMassProperties( shape, 1.0f, ci );
		ci.m_qualityType = bodyQualityType;
		ci.m_collisionFilterInfo = hkpGroupFilter::calcFilterInfo(30);


		ci.m_position.set( -2.0f, -1.0f - pivotY, 0.0f );
		bodyA = new hkpRigidBody(ci);
		bodyA->setName("Body0");
		world->addEntity( bodyA );

		ci.m_position.set( -2.0f, 1.0f - pivotY, 0.0f );
		bodyB = new hkpRigidBody(ci);
		bodyB->setName("Body1");
		world->addEntity( bodyB );

		shape->removeReference();
	}

	// create a hing connecting the two bodies
	{
		hkVector4 axis( 0,0,1.f);
		hkVector4 pivot( -2.0f, -pivotY, 0.0f );
		hkpHingeConstraintData* constraintData = new hkpHingeConstraintData();
		constraintData->setInWorldSpace( bodyA->getTransform(), bodyB->getTransform(), pivot, axis );

		world->addConstraint( new hkpConstraintInstance( bodyA, bodyB, constraintData, constraintPriority ))->removeReference();
		constraintData->removeReference();
	}

	bodyA->removeReference();
	bodyB->removeReference();
}



static void HK_CALL createRagdoll(hkpWorld* world, 
								  hkpCollidableQualityType bodyQualityType, 
								  hkpConstraintInstance::ConstraintPriority constraintPriority,
								  hkVector4& position, hkVector4& velocity,
								  SceneType ragdollVariant)
{
	hkQuaternion    rotation; 	rotation.setAxisAngle( hkTransform::getIdentity().getColumn(0), HK_REAL_PI / - 2.0f );

	hkpGroupFilter& filter = static_cast<hkpGroupFilter&>(*world->m_collisionFilter);
	hkpPhysicsSystem* ragdoll    = GameUtils::createRagdoll( 1.6f, position, rotation, filter.getNewSystemGroup(), GameUtils::RPT_CONVEX );

	// do any special ragdoll setup
	{
		const hkArray<hkpRigidBody*>& rigidbodies = ragdoll->getRigidBodies();
		for( int iRB = 0; iRB < rigidbodies.getSize(); iRB++ )
		{
			hkpRigidBody* body = rigidbodies[iRB];
			body->getMaterial().setFriction(1.0f);
			body->setQualityType( bodyQualityType );
			body->setLinearVelocity( velocity );

			hkReal f = 1.0f;
			if (ragdollVariant == VARIANT_RAGDOLL_INCREASED_INERTIA )
			{
				f = .1f;
			}
			// make sphere inertia and downscale inertia
			{
				hkMatrix3 invTensor;	rigidbodies[iRB]->getInertiaInvLocal(invTensor);
				hkReal minV = hkMath::min2( invTensor(0,0), hkMath::min2( invTensor(1,1), invTensor(2,2) ) );
				minV *= f;
				invTensor.setDiagonal( minV, minV, minV );
				body->setInertiaInvLocal(invTensor);

			}
		}
		const hkArray<hkpConstraintInstance*>& constraints = ragdoll->getConstraints();
		for( int iC = 0; iC < constraints.getSize(); iC++ )
		{
			hkpConstraintInstance* constraint = constraints[iC];
			constraint->setPriority(constraintPriority);
		}
	}

	world->addPhysicsSystem(ragdoll);
	ragdoll->removeReference(); // will delete the system container
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Demo construction and stepDemo()
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

HingeHittingTableDemo::HingeHittingTableDemo(hkDemoEnvironment* env)
:	hkDefaultPhysicsDemo(env)
{
	// Disable warnings:									
	hkError::getInstance().setEnabled(0xafe97523, false); //'This utility is intended primarily for Havok demo use. If you wish to step the world asynchronously,...'

	enableDisplayingToiInformation(true);

	//
	// Setup the camera
	//
	{
		hkVector4 from( 0, 0, 10);
		hkVector4 to  ( 0, 0, 0);
		hkVector4 up  ( 0, 1, 0);
		setupDefaultCameras(env, from, to, up);
	}


	//
	// Setup and create the hkpWorld
	//
	{
		hkpWorldCinfo info;
		info.setBroadPhaseWorldSize(150.0f);
		info.m_collisionTolerance = .03f; 
		info.m_gravity.setZero4();
		info.m_simulationType = info.SIMULATION_TYPE_CONTINUOUS;
		
		m_world = new hkpWorld( info );
		m_world->lock();

		hkpAgentRegisterUtil::registerAllAgents( m_world->getCollisionDispatcher() );

		setupGraphics();		
	}

	const HingeHittingTableDemoVariant& variant =  g_variants[this->m_variantId];

	//
	// Collision Filter
	//
	{ 
		hkpGroupFilter* filter = new hkpGroupFilter();
		hkpGroupFilterSetup::setupGroupFilter( filter );
		m_world->setCollisionFilter(filter);
		filter->disableCollisionsBetween(30,30);
		filter->removeReference();
	}

	// Create a box
	{
		hkReal mass = 0.0f;
		hkVector4 size( 2.0f, 0.2f, 2.0f );
		hkVector4 position( 1.0f, 0.0f, 0.0f );

		hkpRigidBody* body = HK_NULL;

		if( 1 ) 
		{
			// use a hkpBoxShape
			body = GameUtils::createBox( size, mass, position );
		}
		else
		{
			// use a hkpConvexVerticesShape
			body = GameUtils::createConvexVerticesBox( size, mass, position );
		}

		body->setName("table0");
		m_world->addEntity( body )->removeReference();
	}

	hkVector4 pos(-2.0f, 0.0f, 0.0f);
	hkVector4 vel(45.0f, 0.0f, 0.0f);

	switch(variant.m_sceneType)
	{
		case VARIANT_HINGE:
			createHinge(m_world, variant.m_objectQualityType, variant.m_constraintPriorityLevel, pos, vel);
			break;
		case VARIANT_RAGDOLL:
		case VARIANT_RAGDOLL_INCREASED_INERTIA:
			createRagdoll(m_world, variant.m_objectQualityType, variant.m_constraintPriorityLevel, pos, vel, variant.m_sceneType);
			break;

	}

	m_world->unlock();
}

HingeHittingTableDemo::~HingeHittingTableDemo()
{
	// Re-enable warnings:									
	hkError::getInstance().setEnabled(0xafe97523, true); 

	// bodies->removeReference
}

hkDemo::Result HingeHittingTableDemo::stepDemo()
{
	hkReal physicsDeltaTime = 1.0f / 60.0f;
	hkReal frameDeltaTime = physicsDeltaTime;
	if (!g_variants[m_env->m_variantId].m_goRealtime)
	{
		frameDeltaTime *= 0.1f;
	}

	if(m_world)
	{
		hkDefaultPhysicsDemo::stepAsynchronously(m_world, frameDeltaTime, physicsDeltaTime);
	}
	return DEMO_OK;
}



#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif


HK_DECLARE_DEMO_VARIANT_USING_STRUCT( HingeHittingTableDemo, HK_DEMO_TYPE_PHYSICS | HK_DEMO_TYPE_CRITICAL, HingeHittingTableDemoVariant, g_variants, HK_NULL); 

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
