/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/Physics/UseCase/ContinuousPhysics/DiscreteVsContinuous/DiscreteVsContinuousDemo.h>


#include <Demos/DemoCommon/Utilities/GameUtils/GameUtils.h>
#include <Graphics/Common/Input/Pad/hkgPad.h>


/////////////////////////////////////////////////////////////////////////////////////////////////
#include <Graphics/Common/Window/hkgViewport.h>
#include <Graphics/Common/Window/hkgWindow.h>
#include <Graphics/Common/Camera/hkgCamera.h>

#include <Physics/Dynamics/World/Simulation/hkpSimulation.h>
#include <Physics/Utilities/Dynamics/TimeSteppers/hkpAsynchronousTimestepper.h>


#include <Physics/Utilities/VisualDebugger/Viewer/Collide/hkpContactPointViewer.h>
#include <Physics/Utilities/VisualDebugger/Viewer/Collide/hkpBroadphaseViewer.h>
#include <Common/Visualize/hkDebugDisplay.h>

#include <Graphics/Bridge/DisplayHandler/hkgDisplayHandler.h>

#include <Demos/DemoCommon/DemoFramework/hkTextDisplay.h>

#include <Common/Base/System/Stopwatch/hkStopwatch.h>
#include <Common/Base/Monitor/hkMonitorStream.h>

#define HK_PARALLEL_WORLD_COUNT 2


DiscreteVsContinuousDemo::DiscreteVsContinuousDemo(hkDemoEnvironment* env): hkDefaultPhysicsDemo( env )
{
	// Disable warnings:									
	hkError::getInstance().setEnabled(0xafe97523, false); //'This utility is intended primarily for Havok demo use. If you wish to step the world asynchronously,...'

	hkpWorld::IgnoreForceMultithreadedSimulation ignoreForceMultithreaded;

	// Reset timing records.
	for (int i = 0; i < 2*10; i++)
	{
		m_avgTimings[0][i] = 0.0f;
	}
	m_nextTimingIndex = 0;

	// Setup simulation parameters
	m_simulationBeingControlled = 0;
	m_simulationFrequency[0] = 60.0f;
	m_simulationFrequency[1] = 60.0f;
	m_frameCount = 0;
	m_elapsedSimulationFrameTime = 0.0f;


	//
	// Setup the camera
	//
	{
		hkVector4 from(-20.0f, 50.0f, -20.0f );
		hkVector4 to ( 0, 0, 0 );

		hkVector4 up(1.0f, 0.0f, 1.0f);
		setupDefaultCameras(env, from, to, up);
	}

	// Set the start position for the rigid body gun
	m_startPos.set(-3.f, 2.0f + 1.43401f, -3);

	// Set the velocity for the rigid bodies - 60 m/s is about 120 mph.
	m_velocity = 60;

	// Set the number of frames between rigid body firing.
	m_duration = 5;

	// A global "time" parameter, used to change the direction of firing.
	m_time = 0;
	
	m_nextBody = 0;
	m_counter = 0;

	//
	//	Create the world
	//
	{
		hkpWorldCinfo info;
		info.m_simulationType = hkpWorldCinfo::SIMULATION_TYPE_DISCRETE;
		m_world = new hkpWorld( info );
		m_world->lock();

		hkpWorldCinfo info2;
		info2.m_simulationType = hkpWorldCinfo::SIMULATION_TYPE_CONTINUOUS;
		m_world2 = new hkpWorld( info2 );
		m_world2->lock();

		// Register ALL agents (though some may not be necessary)
		hkpAgentRegisterUtil::registerAllAgents( m_world->getCollisionDispatcher() );
		hkpAgentRegisterUtil::registerAllAgents( m_world2->getCollisionDispatcher() );

		setupGraphics();
		m_physicsViewersContext->addWorld(m_world2);
	}

	hkpWorld* worlds[] = { m_world, m_world2 };
	hkArray<hkpRigidBody*>* bodyArrays[] = { &m_rigidBodies, &m_rigidBodies2 };
	hkVector4 offset[] = { hkVector4(10.0f, 0.0f, -10.0f), hkVector4(-10.0f, 0.0f, 10.0f) };

	for (int w = 0; w < HK_PARALLEL_WORLD_COUNT; w++)
	{

		//
		// Create an array bodies which cycle through being fired at the wall
		//
		{
			bodyArrays[w]->setSize( 40 );
			for (int i = 0; i < bodyArrays[w]->getSize(); ++i )
			{

				//
				// Create a box shape
				//
				hkpBoxShape* boxShape = new hkpBoxShape( hkVector4(.1f, .1f, .1f), 0 );


				//
				// Create a rigid body with the convex sweep shape
				//
				hkpRigidBodyCinfo info;
				hkpInertiaTensorComputer::setShapeVolumeMassProperties(boxShape, 10, info );
				//info.m_mass *= 10.0f;
				info.m_motionType = hkpMotion::MOTION_BOX_INERTIA;
				info.m_rotation.setIdentity();
				info.m_shape = boxShape;
				//info.m_position.setZero4();
				info.m_position = offset[w];
				info.m_qualityType = HK_COLLIDABLE_QUALITY_BULLET;

				(*bodyArrays[w])[i] = new hkpRigidBody( info );
				boxShape->removeReference();

				(*bodyArrays[w])[i]->getMaterial().setRestitution(.5f);
			}
		}

		//
		// create a simple open room
		//
		{
			hkVector4 scale(2.0f, 2.0f, 2.0f);
			
			{
				hkpRigidBodyCinfo rinfo;
				rinfo.m_motionType = hkpMotion::MOTION_FIXED;
				rinfo.m_shape = GameUtils::loadTK2MOPP("Resources/Physics/Tk/DiscreteVsContinuous/room.tk",scale, 0.05f);
				rinfo.m_position = offset[w];
				hkpRigidBody* room = new hkpRigidBody(rinfo);
				worlds[w]->addEntity(room);
				room->removeReference();
				rinfo.m_shape->removeReference();
			}
		}
	}

	m_world2->unlock();
	m_world->unlock();
}


DiscreteVsContinuousDemo::~DiscreteVsContinuousDemo()
{
	// Re-enable warnings:									
	hkError::getInstance().setEnabled(0xafe97523, true); 

	m_world->lock();
	m_world2->markForWrite();

	m_world2->removeReference();
	m_world2 = HK_NULL;

	hkArray<hkpRigidBody*>* bodyArrays[] = { &m_rigidBodies, &m_rigidBodies2 };

	for (int w = 0; w < HK_PARALLEL_WORLD_COUNT; w++)
	{
		for (int i = 0; i < bodyArrays[w]->getSize(); ++i )
		{
			(*bodyArrays[w])[i]->removeReference();
		}
	}

	m_world->unlock();
}

hkDemo::Result DiscreteVsContinuousDemo::stepDemo()
{
	m_world->lock();
	m_world2->lock();

	if (m_env->m_gamePad->isButtonPressed(HKG_PAD_BUTTON_1))
	{
		m_simulationBeingControlled =  0;
	}
	else if (m_env->m_gamePad->isButtonPressed(HKG_PAD_BUTTON_2))
	{
		m_simulationBeingControlled =  1;
	}

	// Read keys to increase/lower simulation
	m_simulationFrequency[m_simulationBeingControlled] += int(m_env->m_gamePad->isButtonPressed(HKG_PAD_DPAD_UP));
	if (m_simulationFrequency[m_simulationBeingControlled] > 1.0f)
	{
		m_simulationFrequency[m_simulationBeingControlled] -= int(m_env->m_gamePad->isButtonPressed(HKG_PAD_DPAD_DOWN));
	}

	m_velocity += int(m_env->m_gamePad->isButtonPressed(HKG_PAD_DPAD_RIGHT));
	if (m_velocity > 1.0f)
	{
		m_velocity -= int(m_env->m_gamePad->isButtonPressed(HKG_PAD_DPAD_LEFT));
	}

	//m_simulationFrequency[1] = hkMath::cos( hkReal(m_frameCount) * HK_REAL_PI / 30.0f ) * 10.0f + 12.0f;

	// Perform the rest of the code from the BulletVsPaper demo.
	if ( m_counter++ % m_duration == 0 )
	{
		hkpWorld* worlds[] = { m_world, m_world2 };
		hkArray<hkpRigidBody*>* bodyArrays[] = { &m_rigidBodies, &m_rigidBodies2 };
		hkVector4 offset[] = { hkVector4(10.0f, 0.0f, -10.0f), hkVector4(-10.0f, 0.0f, 10.0f) };

		for (int w = 0; w < HK_PARALLEL_WORLD_COUNT; w++)
		{

			// if the body has not been added, then add it
			if ( (*bodyArrays[w])[m_nextBody]->getWorld() == HK_NULL )
			{
				worlds[w]->addEntity( (*bodyArrays[w])[ m_nextBody ] );
				if (w == 0)
				{
					HK_SET_OBJECT_COLOR( hkUlong((*bodyArrays[w])[ m_nextBody ]->getCollidable()), 0xffffff33);
				}
			}


			hkVector4 dir; dir.setZero4();
			while ( 1 )
			{
				if ( w == HK_PARALLEL_WORLD_COUNT -1 )
				{
					m_time += m_timestep * 5.0f;
				}
				dir(0) = hkMath::fabs( hkMath::sin( m_time ) );
				dir(1) = -0.3f;
				dir(2) = hkMath::fabs( hkMath::cos( m_time ) );

				if ( dir.length3() > HK_REAL_EPSILON )
				{
					dir.normalize3();
					break;
				}
			}

			hkVector4 startPos; startPos.setAdd4(m_startPos, offset[w]);
			(*bodyArrays[w])[ m_nextBody ]->setPosition( startPos );

			hkVector4 velocity;
			velocity.setMul4( m_velocity, dir );

			(*bodyArrays[w])[ m_nextBody ]->setLinearVelocity( velocity );
			(*bodyArrays[w])[ m_nextBody ]->setAngularVelocity( hkVector4(0.0f, 0.0f, 0.0f) );

			if ( w == HK_PARALLEL_WORLD_COUNT -1 )
			{
				worlds[w]->reintegrateAndRecollideEntities((hkpEntity**)(&((*bodyArrays[w])[ m_nextBody ])), 1);
				m_nextBody++;
			}

			if ( m_nextBody == m_rigidBodies.getSize() )
			{
				m_nextBody = 0;
			}
		}
	}

	m_world->unlock();
	m_world2->unlock();

	// Run simulations, do timings
	hkStopwatch stopWatch[2];
	{
		stopWatch[0].reset();
		stopWatch[0].start();
		HK_TIMER_BEGIN("DiscreteSimulation", HK_NULL);
		m_elapsedSimulationFrameTime += m_timestep;
		while(m_elapsedSimulationFrameTime > 1.0f / m_simulationFrequency[0])
		{
			hkDefaultPhysicsDemo::stepAsynchronously(m_world, m_timestep, 1.0f / m_simulationFrequency[0]);
			m_elapsedSimulationFrameTime -= 1.0f / m_simulationFrequency[0];
		}
		HK_TIMER_END();
		stopWatch[0].stop();
	}
	{
		stopWatch[1].reset();
		stopWatch[1].start();
		HK_TIMER_BEGIN("ContinuousSimulation", HK_NULL);
		hkAsynchronousTimestepper::stepAsynchronously(m_world2, m_timestep, 1.0f / m_simulationFrequency[1] );
		HK_TIMER_END();
		stopWatch[1].stop();
	}

	m_world->lock();
	m_world2->lock();

	// Store timings
	m_avgTimings[0][m_nextTimingIndex  ] = stopWatch[0].getSplitSeconds();
	m_avgTimings[1][m_nextTimingIndex++] = stopWatch[1].getSplitSeconds();
	m_nextTimingIndex %= HK_TIMING_SAMPLES;

	// Calculate avg timings
	hkReal avgs[2] = {0,0};
	for (int i = 0; i < 2 * HK_TIMING_SAMPLES ; i++)
	{
		avgs[i/HK_TIMING_SAMPLES] += m_avgTimings[0][i];
	}
	avgs[0] /= HK_TIMING_SAMPLES;
	avgs[1] /= HK_TIMING_SAMPLES;

	// Output info
	char buffer[2][512];
	hkString::sprintf(buffer[0], "Initial  Vel:     %3.1f m/s\n\n"\
		                         "                 Discrete  Continuous\n"\
							     "Simulation Freq: %3.0f Hz    %3.0f Hz\n"\
							     "      CPU Usage: %6.3f ms %6.3f ms",
							     m_velocity, m_simulationFrequency[0], m_simulationFrequency[1], avgs[0] * 1000.0f, avgs[1] * 1000.0f);

	hkString::sprintf(buffer[1], "Use left/right arrows to modify velocity.\n\n"\
		                         "Press [1] or [2] button to select simulation.\n"\
		                         "Use up/down arrows to modify frequency of the\n"\
								 "selected simulation.");
	
	m_env->m_textDisplay->outputText( buffer[0], 20, int(m_env->m_window->getHeight() - 100), 0xffffffff );
	m_env->m_textDisplay->outputText( buffer[1], 400, int(m_env->m_window->getHeight() - 100), 0xffffffff );

	hkString strDiscrete("Discrete");
	hkString strContinuous("Continuous");
	hkVector4 offset[] = { hkVector4(10.0f-6.0f, 0.0f, -10.0f-6.0f), hkVector4(-10.0f-6.0f, 0.0f, 10.0f-6.0f) };
	m_env->m_textDisplay->outputText3D(strDiscrete,   offset[0](0), offset[0](1), offset[0](2));
	m_env->m_textDisplay->outputText3D(strContinuous, offset[1](0), offset[1](1), offset[1](2));


	m_frameCount++;

	m_world2->unlock();
	m_world->unlock();

	return DEMO_OK;
}




#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
"This demo presents the difference in computations done in discrete and continuous simulations."\
"Use left/right arrows to change initial velocity of particles. Use up/down arrows to change simulation frequency. "\
"Use (1) and (2) keys to change discrete or continuous simulation's frequency.";

HK_DECLARE_DEMO( DiscreteVsContinuousDemo, HK_DEMO_TYPE_PRIME | HK_DEMO_TYPE_CRITICAL, "Computational difference between discrete and continuous mode", helpString);

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
