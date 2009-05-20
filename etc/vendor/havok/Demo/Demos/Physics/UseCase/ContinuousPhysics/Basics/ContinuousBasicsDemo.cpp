/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/Physics/UseCase/ContinuousPhysics/Basics/ContinuousBasicsDemo.h>

#include <Physics/Utilities/VisualDebugger/Viewer/Dynamics/hkpSweptTransformDisplayViewer.h>
#include <Physics/Utilities/VisualDebugger/Viewer/Collide/hkpShapeDisplayViewer.h>

#include <Common/Visualize/Process/hkDebugDisplayProcess.h>
#include <Common/Visualize/hkDebugDisplay.h>


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Definitions of game variants
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct ContinuousBasicsDemoVariant
{
	const char*	 m_name;
	hkReal m_ballVelocity;
	bool   m_usePredictive;
	bool   m_showToiClock;
	hkReal m_slowMotionFactor;
	bool   m_showInterpolation;
	const char* m_details;
};

static const hkReal VIEW_RADIUS = 10.0f;


static const ContinuousBasicsDemoVariant g_variants[] =
{
	//	name										vel,    cont	clock, slow, interpolate
	{ "Slow ball hitting other ball"			,	3.0f,   false,	false, 1.0f, false, "When a slow ball hits another ball, everything somehow works as expected." },
	{ "Faster ball using discrete physics"		,   30.0f,	false,	false, 1.0f, false, "In discrete physics, the ball is simulated at discrete timesteps only. " \
																							"Therefore a hit is recorded at a somehow random moment. As a result the " \
																							"collision response looks kind of random." },
	{ "Fast ball in slow motion"				,   30.0f,	false,	false, 10.0f,false, "The example (B) played in slow motions shows that balls are penetrating deeply " \
																							"at the moment when collision response is calculated. This is the reason " 
																							"for the variance in collision results." },
	{ "Fast ball in slow motion interpolated"	,   30.0f,	false,	false, 10.0f, true, "The example (C) introduces the concept of interpolation of the state of the bodies. " \
																							"The bodies' state is represented with a hkSweptTransform which holds the positions " \
																							"of the bodies at two consecutive time frames. To get a proper position at a time in between " \
																							"those two frames we simply interpolate them. This way we can run physical simulation at " \
																							"a frequency lower than the frequency of our graphics renderer, and still get "\
																							"an illusion of constant motion." },
	{ "With physics clock"						,   30.0f,	false,	true,  10.0f, true, "Note the clock in the lower left corner. The eight blue lines represent physical simulation steps. " \
																							"Whenever the spinning green line crosses one of the blue lines a physical simulation step is executed. "
																							"This is the time when the swept transform of the bodies is calculated." },
	{ "Very fast ball"							,   100.0f,	false,	true,  10.0f, true, "This shows that discrete physics may fail totally for fast moving objects." \
																							"Note how the fast ball sometimes goes through the stationary ball without a change in its motion." },
	{ "Very fast ball with continuous physics"	,   100.0f,	true,	true,  10.0f, true, "Continuous physics handles the collision at its time of impact. It requires continuous collision detection " \
																							"and proper handling of such collisions in between the main simulation steps (marked with blue lines on the clock." },
};

void ContinuousBasicsDemo::setupContexts(hkArray<hkProcessContext*>& contexts)
{
	hkDefaultPhysicsDemo::setupContexts(contexts);
	m_debugViewerNames.clear();
	m_debugViewerNames.pushBack( hkDebugDisplayProcess::getName() );

	const ContinuousBasicsDemoVariant& variant =  g_variants[m_variantId];

	if ( variant.m_showToiClock )
	{
		enableDisplayingToiInformation( true );
	}

	if ( variant.m_showInterpolation )
	{
		m_debugViewerNames.pushBack(hkpSweptTransformDisplayViewer::getName());
	}
	m_debugViewerNames.pushBack( hkpShapeDisplayViewer::getName() );
}

ContinuousBasicsDemo::ContinuousBasicsDemo(hkDemoEnvironment* env)
:	hkDefaultPhysicsDemo(env), m_currentTime( 0.0f ), m_random(1)
{
	// Disable warnings:
	hkError::getInstance().setEnabled(0xafe97523, false); //'This utility is intended primarily for Havok demo use. If you wish to step the world asynchronously, you are encouraged to copy the code from this utility and integrate it into your game loop.'

	hkpWorld::IgnoreForceMultithreadedSimulation ignoreForceMultithreaded;

	const ContinuousBasicsDemoVariant& variant =  g_variants[env->m_variantId];

	//
	// Setup the camera
	//
	{
		hkVector4 from(0.0f, 0.0f, 8.0f);
		hkVector4 to  (0.0f, 0.0f,  0.0f);
		hkVector4 up  (0.0f, 1.0f,  0.0f);
		setupDefaultCameras( env, from, to, up, 0.1f, 200.0f );
	}


	//
	// Create the world
	//

	{
		hkpWorldCinfo info;

		info.m_gravity.set(0.0f, 0.0f, 0.0f);
		info.setupSolverInfo(hkpWorldCinfo::SOLVER_TYPE_4ITERS_MEDIUM);
		info.m_collisionTolerance = 0.05f;
		info.m_contactPointGeneration = info.CONTACT_POINT_ACCEPT_ALWAYS;
		info.setBroadPhaseWorldSize( 100.0f );

		if ( variant.m_usePredictive )
		{
			info.m_simulationType = hkpWorldCinfo::SIMULATION_TYPE_CONTINUOUS;
		}
		else
		{
			info.m_simulationType = hkpWorldCinfo::SIMULATION_TYPE_DISCRETE;
		}

		m_world = new hkpWorld( info );
		m_world->lock();

		m_world->addCollisionListener( this );

		// Register ALL agents (though some may not be necessary)
		hkpAgentRegisterUtil::registerAllAgents(m_world->getCollisionDispatcher());

		setupGraphics();
	}


	/// This demo uses only spheres for its components.
	hkpSphereShape* sphereShape = new hkpSphereShape(0.5f);
	{
		hkpRigidBodyCinfo si;
		si.m_motionType = hkpMotion::MOTION_SPHERE_INERTIA;
		si.m_shape = sphereShape;
		si.m_qualityType = HK_COLLIDABLE_QUALITY_CRITICAL;
		si.m_allowedPenetrationDepth = 0.001f;
		hkpInertiaTensorComputer::setShapeVolumeMassProperties(sphereShape, 1.0f, si);

		m_bodyA = new hkpRigidBody(si);
		m_world->addEntity( m_bodyA );
		
		m_bodyB = new hkpRigidBody(si);
		m_world->addEntity( m_bodyB );
		resetScene();
	}
	sphereShape->removeReference();

	//for (int i =0; i < 15; i++) resetScene();

	m_world->unlock();
}

void ContinuousBasicsDemo::resetScene()
{
	const ContinuousBasicsDemoVariant& variant =  g_variants[m_variantId];

	hkReal maxDist = 0.8f * VIEW_RADIUS;

	hkVector4 zero; zero.setZero4();

	hkQuaternion q; q.setIdentity();
	const hkpSphereShape* sphere = static_cast<const hkpSphereShape*>(m_bodyA->getCollidable()->getShape());
	hkReal sphereRadius = sphere->getRadius();
	hkVector4 posB( 1,-1,0);
	posB.normalize3();
	posB.mul4( sphereRadius );

	hkVector4 posA( -maxDist, 0, 0 );
	posA.sub4( posB);

	// randomize position A
	hkReal distPerStep = 0.02f * variant.m_ballVelocity;
	hkReal rand01 = m_random.getRandReal01();
	posA(0) += rand01 * distPerStep;

	m_bodyA->setPositionAndRotation( posA, q );
	m_bodyB->setPositionAndRotation( posB, q );

	hkVector4 vel( variant.m_ballVelocity, 0, 0 );
	m_bodyA->setLinearVelocity( vel );
	m_bodyA->setAngularVelocity( zero );
	m_bodyB->setLinearVelocity( zero );
	m_bodyB->setAngularVelocity( zero );

	m_collA.setNeg4( posB );
	m_collB = m_bodyB->getTransform().getTranslation();
}

void ContinuousBasicsDemo::contactPointAddedCallback(	hkpContactPointAddedEvent& event) 
{
	const ContinuousBasicsDemoVariant& variant =  g_variants[m_variantId];
	if ( variant.m_usePredictive == false)
	{
		m_collA = m_bodyA->getTransform().getTranslation();
		m_collB = m_bodyB->getTransform().getTranslation();
	}
}

void ContinuousBasicsDemo::contactPointConfirmedCallback( hkpContactPointConfirmedEvent& event)
{
}

void ContinuousBasicsDemo::contactPointRemovedCallback( hkpContactPointRemovedEvent& event )
{
}

void ContinuousBasicsDemo::showTrajectories()
{
	for (int i =0; i < m_displayVectors.getSize(); i++ )
	{
		const Segment& v = m_displayVectors[i];
		HK_DISPLAY_LINE( v.m_start, v.m_end, hkColor::GREEN );
	}
}

hkDemo::Result ContinuousBasicsDemo::stepDemo()
{
	const ContinuousBasicsDemoVariant& variant =  g_variants[m_variantId];
	hkReal physicsDeltaTime = 0.02f;
	hkReal frameDeltaTime = physicsDeltaTime / variant.m_slowMotionFactor;
	if ( variant.m_showInterpolation )
	{
		hkDefaultPhysicsDemo::stepAsynchronously(m_world, frameDeltaTime, physicsDeltaTime);
	}
	else
	{
		m_currentTime += frameDeltaTime;
		if ( m_currentTime >= physicsDeltaTime)
		{
			m_timestep = physicsDeltaTime;
			hkDefaultPhysicsDemo::stepDemo();
			m_currentTime = 0.0f;
		}
	}

	m_world->lock();

	hkReal distA = m_bodyA->getTransform().getTranslation().length3();
	hkReal distB = m_bodyB->getTransform().getTranslation().length3();
	hkReal maxDist = VIEW_RADIUS;
	if ( distA + distB > maxDist )
	{
		Segment& a= m_displayVectors.expandOne();
		a.m_start = m_collA;
		a.m_end = m_bodyA->getTransform().getTranslation();
		Segment& b= m_displayVectors.expandOne();
		b.m_start = m_collB;
		b.m_end = m_bodyB->getTransform().getTranslation();
		resetScene();
	}
	showTrajectories();

	m_world->unlock();

	return DEMO_OK;
}

void ContinuousBasicsDemo::postRenderDisplayWorld(hkgViewport* v)
{
	hkDefaultPhysicsDemo::postRenderDisplayWorld(v);
	showTrajectories();
}


ContinuousBasicsDemo::~ContinuousBasicsDemo()
{
	// Re-enable warnings:
	hkError::getInstance().setEnabled(0xafe97523, true);

	m_world->lock();
	m_world->removeCollisionListener( this ); // so that the virtual call to it upon world remove entity does not fail
	m_bodyA->removeReference();
	m_bodyB->removeReference();
	m_world->unlock();
}


#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
"A demo shows the different concepts introduced by continuous physics, such as "	\
"interpolation of graphics, artefacts of discrete physics, and continuous physics";

HK_DECLARE_DEMO_VARIANT_USING_STRUCT( ContinuousBasicsDemo, HK_DEMO_TYPE_PRIME, ContinuousBasicsDemoVariant, g_variants, HK_NULL); 

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
