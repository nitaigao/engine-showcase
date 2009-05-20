/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/Physics/UseCase/ContinuousPhysics/ReintegrateBodies/ReintegrateBodiesDemo.h>

#include <Physics/Utilities/VisualDebugger/Viewer/Dynamics/hkpSimulationIslandViewer.h>
#include <Common/Base/Math/Vector/hkVector4Util.h>
#include <Common/Visualize/hkDebugDisplay.h>

#include <Demos/DemoCommon/DemoFramework/hkTextDisplay.h>
#include <Graphics/Common/Window/hkgWindow.h>

#define HK_NUM_CIRCLING_BODIES 10
#define HK_CIRCLING_RADIUS 2.0f
#define HK_CIRCLING_VELOCITY 200.f


int g_numToiEventsPerFrame;

enum ShapeToUse
{
	USE_BOXES,
	USE_SPHERES
};

struct ReintegrateBodiesVariant
{
	const char*							m_name;
	hkpWorldCinfo::SimulationType		m_simulationType;
	bool								m_performReintegration;
	int									m_frameDistanceBetweenObjects;
	ShapeToUse							m_shapesToUse;
	int									m_mode;

	const char*							m_details;
};



static const ReintegrateBodiesVariant g_variants[] =
{
#define DISC hkpWorldCinfo::SIMULATION_TYPE_DISCRETE
#define CONT  hkpWorldCinfo::SIMULATION_TYPE_CONTINUOUS

	{ "No reintegration",									CONT, 0,  2, USE_SPHERES,   0, "When no reintegration is done, newly added objects are frozen until the next PSI step."},
	{ "With reintegration",									CONT, 1,  2, USE_SPHERES,   0, "With reintegration, new objects start moving from the moment they are added to the world."},
	{ "With reintegration, boxes",							CONT, 1, 10, USE_BOXES,     0, "With reginegration, new objects start moving from the moment they are added to the world."},
	{ "Reintegration, discrete",							DISC, 1, 10, USE_SPHERES,   1, "In this demo, bodies are reintegrated mid-step, to keep them moving in a circular motion. In the discrete mode collisions are only resolved once every PSI step."},
	{ "Reintegration, continuous",							CONT, 1, 10, USE_SPHERES,   1, "In this demo, bodies are reintegrated mid-step, to keep them moving in a circular motion. In the continuous mode collisions are also resolved at the exact time of impact."},

#undef DISC
#undef CONT

};


void ReintegrateBodiesDemo::shootBodies()
{
		//fire new balls
	for (int i = 0; i < 2; i++)
	{
		hkpRigidBodyCinfo info;
		if (g_variants[m_env->m_variantId].m_shapesToUse == USE_SPHERES)
		{
			info.m_shape = new hkpSphereShape(0.5f);
		}
		else // USE_BOXES
		{
			hkVector4 halfExt(0.5f, 0.5f, 0.5f);
			info.m_shape = new hkpBoxShape(halfExt, 0.01f);
		}
		hkpInertiaTensorComputer::setShapeVolumeMassProperties(info.m_shape, 1.0f, info);
		info.m_linearVelocity = hkVector4(100.0f, (i?-1.0f:1.0f) * 100.0f  , 0.0f);
		info.m_position       = hkVector4(-5.0f,  (i?1.0f:-1.0f) * 10.0f  , 0.0f);
		info.m_qualityType = HK_COLLIDABLE_QUALITY_CRITICAL;
		info.m_allowedPenetrationDepth = 0.01f;

		hkpRigidBody* rb = new hkpRigidBody(info);
		info.m_shape->removeReference();
		m_world->addEntity(rb);
		m_bodies.pushBack(rb);
	}

}

ReintegrateBodiesDemo::ReintegrateBodiesDemo(hkDemoEnvironment* env)
:	hkDefaultPhysicsDemo(env), m_generator(747)
{
	// Disable warnings:									
	hkError::getInstance().setEnabled(0xafe97523, false); //'This utility is intended primarily for Havok demo use. If you wish to step the world asynchronously,...'

	enableDisplayingToiInformation(true);
	m_frameCount = 0;

	//
	// Setup the camera
	//
	{
		hkVector4 from(-25.0f,  0.0f, 2.5f);
		hkVector4 to  (20.0f,  0.0f,  0.0f);
		hkVector4 up  (0.0f,  0.0f,  1.0f);
		setupDefaultCameras(env, from, to, up);
	}


	//
	// Setup and create the hkpWorld
	//
	{
		hkpWorldCinfo info;
		info.m_gravity.setZero4();
		info.setBroadPhaseWorldSize(150.0f);
		//info.m_collisionTolerance = 0.01f; 
		info.m_simulationType = g_variants[m_env->m_variantId].m_simulationType;
		info.m_contactPointGeneration = hkpWorldCinfo::CONTACT_POINT_ACCEPT_ALWAYS;
		
		m_world = new hkpWorld( info );
		m_world->lock();

		m_debugViewerNames.pushBack( hkpSimulationIslandViewer::getName());

		setupGraphics();		
	}

	// Register the single agent required (a hkpBoxBoxAgent)
	{
		hkpAgentRegisterUtil::registerAllAgents(m_world->getCollisionDispatcher());
	}

	if (g_variants[m_env->m_variantId].m_mode == 0)
	{
		shootBodies();
	}
	else if (g_variants[m_env->m_variantId].m_mode == 1)
	{
		// Create spheres on a virtual sphere boundary

		for (int i = 0; i < HK_NUM_CIRCLING_BODIES; i++)
		{
			hkpRigidBodyCinfo info;
			info.m_shape = new hkpSphereShape(0.5f);
			info.m_mass = 1.0f;
			info.m_inertiaTensor.setDiagonal(1, 1, 1);

			hkQuaternion quat; m_generator.getRandomRotation(quat);
			hkVector4 vec(1,0,0); 
			info.m_position.setRotatedDir(quat, vec);
			hkVector4Util::calculatePerpendicularVector(info.m_position, info.m_linearVelocity);
			info.m_position.mul4(HK_CIRCLING_RADIUS);
			info.m_linearVelocity.mul4(HK_CIRCLING_VELOCITY);
			info.m_friction = 0.0f;
			info.m_restitution = 1.0f;
			info.m_qualityType = HK_COLLIDABLE_QUALITY_CRITICAL;
			info.m_motionType = hkpMotion::MOTION_DYNAMIC;

			hkpRigidBody* body = new hkpRigidBody(info);
			info.m_shape->removeReference();

			m_world->addEntity(body);

			m_bodies.pushBack(body);
		}
	}

	m_world->unlock();
}

hkDemo::Result ReintegrateBodiesDemo::stepDemo()
{
	m_world->lock();

	hkReal physicsDt = 1.0f / 60.0f;
	hkReal frameDt   = physicsDt / 18.0f;

	if (g_variants[m_env->m_variantId].m_mode == 0)
	{
		if (m_frameCount++ == g_variants[m_env->m_variantId].m_frameDistanceBetweenObjects)
		{
			m_frameCount = 0;
			{
				shootBodies();

				if (g_variants[m_env->m_variantId].m_performReintegration)
				{
					// reintegrate the last two bodies added
					hkpEntity* entities[2] = { m_bodies[m_bodies.getSize()-2], m_bodies[m_bodies.getSize()-1] };
					m_world->reintegrateAndRecollideEntities(entities, 2);
				}
			}
		}
	}
	else 
	{
		// circling spheres
		for (int i = 0; i < m_bodies.getSize(); i++)
		{
			hkTransform transform;
			m_bodies[i]->getRigidMotion()->getMotionState()->getSweptTransform().approxTransformAt(m_world->getCurrentTime(), transform);

				// bounce objects off a virtual hollow sphere of radius HK_CIRCLING_RADIUS
			if (transform.getTranslation().lengthSquared3() > HK_CIRCLING_RADIUS * HK_CIRCLING_RADIUS)
			{
				hkVector4 normal = transform.getTranslation();
				normal.normalize3();
				hkReal dot = normal.dot3(m_bodies[i]->getLinearVelocity());
				if (dot > 0.0f)
				{
					hkVector4 newVelocity; newVelocity.setAddMul4(m_bodies[i]->getLinearVelocity(), normal, - 2.0f * dot);
					m_bodies[i]->setLinearVelocity(newVelocity);

					m_world->reintegrateAndRecollideEntities(reinterpret_cast<hkpEntity**>(&m_bodies[i]), 1);
				}
			}
		}

	}

	for (int i = 0; i < m_bodies.getSize(); i++)
	{
		HK_SET_OBJECT_COLOR( hkUlong(m_bodies[i]->getCollidable()), 0xffdddddd );
	}

	m_world->unlock();

	hkDefaultPhysicsDemo::stepAsynchronously(m_world, frameDt, physicsDt);

	return DEMO_OK;
}

ReintegrateBodiesDemo::~ReintegrateBodiesDemo()
{
	// Re-enable warnings:									
	hkError::getInstance().setEnabled(0xafe97523, true); 

	m_world->lock();

	while(m_bodies.getSize())
	{
		m_bodies.back()->removeReference();
		m_bodies.popBack();
	}

	m_world->unlock();
}




#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
"Shows how to reintegrate bodies. When bodies are added to the world in between PSI steps they are frozen until the next integration step. "\
"If you want to force them to start moving immediately use the hkpSimulation::reintegrateAndRecollideEntityBatchImmediately() function";

HK_DECLARE_DEMO_VARIANT_USING_STRUCT( ReintegrateBodiesDemo, HK_DEMO_TYPE_PRIME, ReintegrateBodiesVariant, g_variants, HK_NULL); 

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
