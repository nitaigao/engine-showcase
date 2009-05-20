/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


#include <Demos/demos.h>
#include <Demos/DemoCommon/DemoFramework/hkDefaultPhysicsDemo.h>
#include <Common/Base/Memory/Memory/hkMemory.h>
#include <Physics/Utilities/Actions/MouseSpring/hkpMouseSpringAction.h>

#include <Common/Base/Reflection/hkClass.h>
#include <Common/Base/System/hkBaseSystem.h>
#include <Common/Base/System/Stopwatch/hkStopwatch.h>
#include <Common/Base/DebugUtil/StatisticsCollector/hkStatisticsCollector.h>
#include <Common/Base/Thread/CriticalSection/hkCriticalSection.h>

#include <Physics/Dynamics/World/Simulation/hkpSimulation.h>

#include <Graphics/Common/hkGraphics.h>
#include <Graphics/Common/Input/Pad/hkgPad.h>
#include <Graphics/Common/Window/hkgWindow.h>
#include <Graphics/Common/Camera/hkgCamera.h>
#include <Graphics/Common/DisplayObject/hkgDisplayObject.h>
#include <Graphics/Common/DisplayObject/hkgInstancedDisplayObject.h>
#include <Graphics/Common/DisplayWorld/hkgDisplayWorld.h>
#include <Graphics/Common/Light/hkgLightManager.h>

#include <Common/Serialize/Util/hkBuiltinTypeRegistry.h>
#include <Common/Base/Reflection/Registry/hkVtableClassRegistry.h>

#include <Common/SceneData/Graph/hkxNode.h>
#include <Common/SceneData/Mesh/hkxMesh.h>
#include <Common/SceneData/Scene/hkxScene.h>
#include <Common/SceneData/Attributes/hkxAttributeGroup.h>

#include <Graphics/Bridge/DisplayHandler/hkgDisplayHandler.h>
#include <Graphics/Bridge/SceneData/hkgSceneDataConverter.h>

//#include <Physics/Utilities/VisualDebugger/Viewer/Collide/hkpBroadphaseViewer.h>
//#include <Physics/Utilities/VisualDebugger/Viewer/Collide/hkpConvexRadiusViewer.h>
#include <Physics/Utilities/VisualDebugger/Viewer/Collide/hkpActiveContactPointViewer.h>
#include <Physics/Utilities/VisualDebugger/Viewer/Collide/hkpInactiveContactPointViewer.h>
//#include <Physics/Utilities/VisualDebugger/Viewer/Collide/hkpToiContactPointViewer.h>
#include <Physics/Utilities/VisualDebugger/Viewer/Collide/hkpWeldingViewer.h>
//#include <Physics/Utilities/VisualDebugger/Viewer/Dynamics/hkpConstraintViewer.h>
//#include <Physics/Utilities/VisualDebugger/Viewer/Dynamics/hkpSweptTransformDisplayViewer.h>
//#include <Physics/Utilities/VisualDebugger/Viewer/Dynamics/hkpRigidBodyCentreOfMassViewer.h>
//#include <Physics/Utilities/VisualDebugger/Viewer/Dynamics/hkpRigidBodyInertiaViewer.h>
//#include <Physics/Utilities/VisualDebugger/Viewer/Dynamics/hkpSimulationIslandViewer.h>
//#include <Physics/Utilities/VisualDebugger/Viewer/Vehicle/hkpVehicleViewer.h>

#include <Physics/Utilities/VisualDebugger/Viewer/Collide/hkpShapeDisplayViewer.h>
#include <Physics/Utilities/VisualDebugger/Viewer/Dynamics/hkpPhantomDisplayViewer.h>

#include <Physics/Utilities/VisualDebugger/hkpPhysicsContext.h>
#include <Physics/Utilities/Dynamics/TimeSteppers/hkpAsynchronousTimestepper.h>

#include <Common/Visualize/Process/hkDebugDisplayProcess.h>

#include <Common/Visualize/hkProcess.h>
#include <Common/Visualize/hkProcessRegisterUtil.h>
#include <Common/Visualize/hkVisualDebugger.h>
#include <Common/Visualize/hkProcessFactory.h>

#include <Physics/Dynamics/hkpDynamics.h>
#include <Physics/Dynamics/World/hkpPhysicsSystem.h>
#include <Physics/Dynamics/World/hkpSimulationIsland.h>

#include <Common/Base/Thread/JobQueue/hkJobQueue.h>


#include <Demos/DemoCommon/Utilities/Asset/hkAssetManagementUtil.h>
#include <Graphics/Bridge/SceneData/hkgSceneDataConverter.h>
#include <Common/Serialize/Util/hkRootLevelContainer.h>
#include <Common/Serialize/Util/hkLoader.h>
#include <Physics/Utilities/Serialize/hkpPhysicsData.h>

#include <Common/Base/Thread/Job/ThreadPool/Cpu/hkCpuJobThreadPool.h>
#include <Common/Base/Thread/Job/ThreadPool/Spu/hkSpuJobThreadPool.h>



#if defined HK_PLATFORM_PS3_PPU
#include <Common/Base/Memory/PlattformUtils/Spu/hkSpuMemoryInternal.h>
#define	SPURS_THREAD_GROUP_PRIORITY 250
#define SPURS_HANDLER_THREAD_PRIORITY 1
#endif

#	include <Common/Base/Fwd/hkwindows.h>

#include <Common/Base/Monitor/MonitorStreamAnalyzer/hkMonitorStreamAnalyzer.h>



class hkStatisticsCollector;
class hkDemoEnvironment;

hkDefaultPhysicsDemo::hkDefaultPhysicsDemo(hkDemoEnvironment* env, DemoFlags flags)
	:	hkDefaultDemo(env),
		m_mouseSpring(HK_NULL),
		m_mouseSpringMaxRelativeForce(1000.0f),
		m_physicsViewersContext(HK_NULL),
		m_displayToiInformation(false),
		m_world(HK_NULL),
		m_flags(flags)
{
	hkReferencedObject::lockAll();
	m_physicsStepCounter = 0;

	//HK_ON_DETERMINISM_CHECKS_ENABLED(hkCheckDeterminismUtil::createInstance());
	//HK_ON_DETERMINISM_CHECKS_ENABLED(hkCheckDeterminismUtil::getInstance().start());

	// Disable warnings that commonly occur in the demos
	hkError::getInstance().setEnabled(0x6e8d163b, false); // hkpMoppUtility.cpp:18
	hkError::getInstance().setEnabled(0x34df5494, false); //hkGeometryUtility.cpp:26
	hkError::getInstance().setEnabled(0xf013323d, false); //hkBroadphaseBorder

	m_oldForceMultithreadedSimulation = hkpWorld::m_forceMultithreadedSimulation;
	hkpWorld::m_forceMultithreadedSimulation = env->m_options->m_forceMT;

	m_physicsViewersContext = new hkpPhysicsContext;
	if ( m_env->m_window )
	{
			hkpPhysicsContext::registerAllPhysicsProcesses(); // all physics only ones
		}		

	hkMemory::getInstance().m_memoryState = hkMemory::MEMORY_STATE_OK;
	hkMemory::getInstance().m_criticalMemoryLimit = 0x7fffffff;

	// Most physics demos look good with shadows

	m_forcedFPSChange = false;
	m_simulationStarted = false;

	if (m_env->m_window)
	{
		setupLights(m_env); // after we decide if we want shadows
	}

	hkReferencedObject::unlockAll();

#if defined(HK_PLATFORM_MULTI_THREAD) && (HK_CONFIG_THREAD == HK_CONFIG_MULTI_THREADED)
	hkpWorld::registerWithJobQueue( m_jobQueue );
#endif

}


void hkDefaultPhysicsDemo::setAutoInstancingEnabled( bool on )
{
	hkpShapeDisplayViewer* viewer = (hkpShapeDisplayViewer*) getLocalViewerByName("Shapes");
	if (viewer)
	{
		viewer->setInstancingEnabled( on );
	}
}

void hkDefaultPhysicsDemo::setAutoDisplayCachingEnabled( bool on )
{
	hkpShapeDisplayViewer* viewer = (hkpShapeDisplayViewer*) getLocalViewerByName("Shapes");
	if (viewer)
	{
		viewer->setDisplayBodyCachingEnabled( on );
	}
	if (m_env->m_displayHandler)
	{
		m_env->m_displayHandler->setDisplayBodyCachingEnabled( on );
	}
}




void hkDefaultPhysicsDemo::addTimersToVdb( const hkArray<hkTimerData>& threadStreams, const hkArray<hkTimerData>& spuStreams )
{
	// reset our VDB stats list
	if (m_physicsViewersContext)
	{
		m_physicsViewersContext->m_monitorStreamBegins.setSize(0);
		m_physicsViewersContext->m_monitorStreamEnds.setSize(0);
	}

	for ( int i = 0; i < threadStreams.getSize(); ++i )
	{
		m_physicsViewersContext->m_monitorStreamBegins.pushBack(threadStreams[i].m_streamBegin);
		m_physicsViewersContext->m_monitorStreamEnds.pushBack(threadStreams[i].m_streamEnd);
	}
	for ( int ii = 0; ii < spuStreams.getSize(); ++ii )
	{
		m_physicsViewersContext->m_monitorStreamBegins.pushBack(spuStreams[ii].m_streamBegin);
		m_physicsViewersContext->m_monitorStreamEnds.pushBack(spuStreams[ii].m_streamEnd);
	}
}




hkDefaultPhysicsDemo::~hkDefaultPhysicsDemo()
{
	if ( ( m_world != HK_NULL ) && ( m_env->m_options->m_renderParallelWithSimulation && m_simulationStarted ) )
	{
		m_world->finishMtStep( );
	}
	hkReferencedObject::lockAll();
	if(m_world)
	{
		m_world->markForWrite();
		m_world->removeReference();
		m_world = HK_NULL;
	}

	hkMemory::getInstance().freeRuntimeBlocks();

	shutdownVDB();

	if (m_physicsViewersContext)
	{
		m_physicsViewersContext->removeReference();
		m_physicsViewersContext = HK_NULL;
	}
	hkReferencedObject::unlockAll();

	if (m_forcedFPSChange)
	{
		m_env->m_options->m_lockFps = m_oldFPSLock;
	}

	//HK_ON_DETERMINISM_CHECKS_ENABLED(hkCheckDeterminismUtil::getInstance().finish());
	//HK_ON_DETERMINISM_CHECKS_ENABLED(hkCheckDeterminismUtil::destroyInstance());

	hkpWorld::m_forceMultithreadedSimulation = m_oldForceMultithreadedSimulation;
}



void hkDefaultPhysicsDemo::addOrRemoveThreads()
{
	hkDefaultDemo::addOrRemoveThreads();

}


void hkDefaultPhysicsDemo::calcContentStatistics( hkStatisticsCollector* collector, const hkClass* cls ) const
{
	if (!m_world)
	{
		return;
	}
	{
		m_world->lock();
        
        collector->addReferencedObject( "hkpWorld", m_world );
		
		m_world->unlock();

        collector->addNormalChunk("hkpPhysicsContext", m_physicsViewersContext, sizeof(hkpPhysicsContext));
	}
}

void hkDefaultPhysicsDemo::setupContexts(hkArray<hkProcessContext*>& contexts)
{
	if ( m_world )
	{
		m_world->markForWrite();
		if ( (m_physicsViewersContext->findWorld(m_world) < 0) )
		{
			m_physicsViewersContext->addWorld(m_world);
		}
		m_world->unmarkForWrite();
	}
	contexts.pushBack( m_physicsViewersContext );

	// Add viewers to the demo display.
	// Uncomment these to use them.
	//  m_debugViewerNames.pushBack( hkpBroadphaseViewer::getName()  );
	 //m_debugViewerNames.pushBack( hkpConstraintViewer::getName()  );
	//  m_debugViewerNames.pushBack( hkpActiveContactPointViewer::getName()  );
	//	m_debugViewerNames.pushBack( hkpInactiveContactPointViewer::getName()  );
	//	m_debugViewerNames.pushBack( hkpRigidBodyCentreOfMassViewer::getName()  );
	//	m_debugViewerNames.pushBack( hkpRigidBodyInertiaViewer::getName()  );
	//	m_debugViewerNames.pushBack( hkpSimulationIslandViewer::getName()  );
	//	m_debugViewerNames.pushBack( hkpVehicleViewer::getName()  );
	//	m_debugViewerNames.pushBack( hkStatisticsProcess::getName()  );
	//	m_debugViewerNames.pushBack( hkpSweptTransformDisplayViewer::getName() );
	//	m_debugViewerNames.pushBack( hkpToiContactPointViewer::getName()  );

	// A viewer to look at the collision radius of convex objects.
	// m_debugViewerNames.pushBack( hkpConvexRadiusViewer::getName() );

	// These are the three "default" viewers which display shapes,
	// phantoms and debug lines.

	m_debugViewerNames.pushBack( hkpShapeDisplayViewer::getName() );
	m_debugViewerNames.pushBack( hkpPhantomDisplayViewer::getName() );
	m_debugViewerNames.pushBack( hkDebugDisplayProcess::getName() );

	// register all our classes we know about with the vdb for tweaking
	if (m_vdbClassReg)
	{
		m_vdbClassReg->registerList(hkBuiltinTypeRegistry::StaticLinkedTypeInfos, hkBuiltinTypeRegistry::StaticLinkedClasses);
	}
}



void hkDefaultPhysicsDemo::waitForStepCompletion()
{
	if ( m_env->m_options->m_renderParallelWithSimulation )
	{
		if ( !m_simulationStarted )
		{
			return;
		}

		m_simulationStarted = false;
		if(	m_world && m_world->m_simulationType == hkpWorldCinfo::SIMULATION_TYPE_MULTITHREADED &&	m_jobThreadPool != HK_NULL )
		{
			m_world->checkUnmarked();
			m_jobQueue->processAllJobs();
			m_jobThreadPool->waitForCompletion();
			m_world->finishMtStep( m_jobQueue, m_jobThreadPool );
		}
	}
}

void hkDefaultPhysicsDemo::startNewSimulateStep()
{
	m_simulationStarted = true;
	if(	m_world )
	{
		if ( m_world->m_simulationType == hkpWorldCinfo::SIMULATION_TYPE_MULTITHREADED && m_jobThreadPool != HK_NULL )
		{
			m_world->checkUnmarked();
			m_world->initMtStep( m_jobQueue, m_timestep );
			m_jobThreadPool->processAllJobs( m_jobQueue );
		}
		else
		{
			m_world->stepDeltaTime(m_timestep);
		}
	}
}


hkDemo::Result hkDefaultPhysicsDemo::stepDemo()
{
	if(m_world)
	{
		m_world->checkUnmarked();
#if defined(HK_PLATFORM_MULTI_THREAD) && (HK_CONFIG_THREAD == HK_CONFIG_MULTI_THREADED)
		if ( m_world->m_simulationType == hkpWorldCinfo::SIMULATION_TYPE_MULTITHREADED )
		{
			if ( m_env->m_options->m_renderParallelWithSimulation )
			{
				Result res = hkDefaultDemo::stepDemo();
				if (res == DEMO_OK)
				{
					startNewSimulateStep();
				}
				m_physicsStepCounter++;
				addOrRemoveThreads();
				return res;
			}
			else
			{
				hkCheckDeterminismUtil::workerThreadStartFrame(true);

				m_world->stepMultithreaded( m_jobQueue, m_jobThreadPool, m_timestep );

				hkCheckDeterminismUtil::workerThreadFinishFrame();

			}
		}
		else
#endif
		{
			m_world->stepDeltaTime(m_timestep);
		}

		//	updateDisplay( m_world );
	}
	m_physicsStepCounter++;
	return hkDefaultDemo::stepDemo();
}

hkpStepResult hkDefaultPhysicsDemo::stepAsynchronously(hkpWorld* world, hkReal frameDeltaTime, hkReal physicsDeltaTime)
{

	hkpStepResult result;

	if (  m_world->m_simulationType == hkpWorldCinfo::SIMULATION_TYPE_MULTITHREADED )
	{

		world->setFrameTimeMarker( frameDeltaTime );

		world->advanceTime();
		while ( !world->isSimulationAtMarker() ) 
		{
			HK_ASSERT( 0x11179564, world->isSimulationAtPsi() );

			{
				// Interact from game to physics
			}

			hkCheckDeterminismUtil::workerThreadStartFrame(true);

			m_world->initMtStep( m_jobQueue, m_timestep );

			m_jobThreadPool->processAllJobs( m_jobQueue );
			m_jobQueue->processAllJobs( );

			m_jobThreadPool->waitForCompletion();

			m_world->finishMtStep( m_jobQueue, m_jobThreadPool );

			hkCheckDeterminismUtil::workerThreadFinishFrame();
		}

		result = HK_STEP_RESULT_SUCCESS;
	}
	else
	{
		hkAsynchronousTimestepper::stepAsynchronously(world, frameDeltaTime, physicsDeltaTime);
		result = HK_STEP_RESULT_SUCCESS;
	}

	return result;
}


void hkDefaultPhysicsDemo::preDeleteDemo()
{
}


hkBool hkDefaultPhysicsDemo::objectPicked( const hkgDisplayObject* displayObject, const hkVector4& worldPosition, int geomIndex )
{
	HK_ASSERT(0x65b2643b, m_env->m_displayHandler);
	hkgDisplayHandler* dhandler = m_env->m_displayHandler;

	if( displayObject && m_world )
	{
		hkUlong id = 0;
		void* userPtr = HK_NULL;
		if (displayObject->getStatusFlags() & HKG_DISPLAY_OBJECT_INSTANCED)
		{
			id = ((hkgInstancedDisplayObject*)displayObject)->getUserDataFromIndex( geomIndex );
		}
		else
		{
			id = dhandler->getDisplayObjectId( displayObject );
			userPtr = displayObject->getUserPointer();
		}

		// HACK!  We know the id is actually the address of the Collidable
		if ( (id > 0) || (userPtr != HK_NULL) )
		{
			hkpRigidBody* rb = HK_NULL;
			// Check if doLink->m_id is a valid address.
			if (id > 0)
			{
				if ( (id & 0x03) == 0x03) // 0x1 == swept transform from, 0x2 = swept transform to, 0x3 = convex radius (ok to pick)
				{
					id &= ~0x03;
				}

				if (id % 4 != 0 || id < 64)
				{
					return false;
				}

				hkpCollidable* col = reinterpret_cast<hkpCollidable*>( id );
				if( col->getOwner() )
				{
					rb = hkGetRigidBody(col);
				}
			}
			else
			{
				rb = reinterpret_cast<hkpRigidBody*>( userPtr ); // true in our demos that use assets
			}

			if( rb && !rb->isFixed() )
			{
				hkpWorld* mouseWorld = rb->getWorld();
				if (mouseWorld)
				{
					hkVector4 positionAinA;
					positionAinA.setTransformedInversePos( rb->getTransform(), worldPosition );

					const hkReal springDamping = 0.5f;
					const hkReal springElasticity = 0.3f;
					const hkReal objectDamping = 0.95f;

					mouseWorld->lock();
						m_mouseSpring = new hkpMouseSpringAction( positionAinA, worldPosition, springDamping, springElasticity, objectDamping, rb, &m_mouseSpringAppliedCallbacks );
						m_mouseSpring->setMaxRelativeForce(m_mouseSpringMaxRelativeForce);

 						mouseWorld->addAction( m_mouseSpring );
					mouseWorld->unlock();
				}

				return true;
			}
		}
	}
	return false;
}

void hkDefaultPhysicsDemo::objectDragged( const hkVector4& newWorldSpacePoint )
{
	// has mousespring been deleted when entity was deleted?
	if( m_mouseSpring != HK_NULL )
	{
		m_world->lock();
		m_mouseSpring->setMousePosition( newWorldSpacePoint );
		m_world->unlock();
	}
}

void hkDefaultPhysicsDemo::objectReleased()
{
	if( m_mouseSpring != HK_NULL)
	{
		hkpWorld* mouseWorld = m_mouseSpring->getWorld();
		if ( mouseWorld )
		{
			mouseWorld->lock();
			{
				mouseWorld->removeAction( m_mouseSpring );
				hkpRigidBody* body = static_cast<hkpRigidBody*>(m_mouseSpring->getEntity());
				body->activate();
			}
		}

		m_mouseSpring->removeReference();

		if (mouseWorld)
		{
			mouseWorld->unlock();
		}

		m_mouseSpring = HK_NULL;
	}
}


static void HK_CALL displayTimePoint( hkgDisplayContext* displayContext, hkTime time, hkReal physicsDeltaTime, hkReal scale )
{
	hkReal angle = 2.0f * HK_REAL_PI * scale * time / physicsDeltaTime;

	hkReal sx = hkMath::sin( angle );
	hkReal cx = hkMath::cos( angle );

	hkReal size = 40.0f;

	hkReal rs = size;
	hkReal re = size * 0.5f;

	// ortho view in window is 0..-2
	hkVector4 start( rs * sx, rs * cx, -0.001f);
	hkVector4 end  ( re * sx, re * cx, -0.001f);

	hkVector4 offset(size + 10.0f, size + 10.0f, 0.0f);
	start.add4( offset );
	end  .add4( offset );

	displayContext->setCurrentPosition( &start(0) );
	displayContext->setCurrentPosition( &end(0) );

	//HK_DISPLAY_LINE( start, end, color );
}


static void HK_CALL displayTimeWheel(hkDemoEnvironment* env, hkpWorld* world, hkReal physicsDeltaTime)
{
	hkgViewport* origView = env->m_window->getCurrentViewport();
	hkgViewport* orthoView = env->m_window->getWindowOrthoView();

	// Set as the current view
	hkgDisplayContext* context = env->m_window->getContext();
	context->lock();
	orthoView->setAsCurrent( context );

	context->setDepthReadState(false);
	context->setDepthWriteState(true);
	context->setTexture2DState(false);

	//
	//	Display a nice time bar to see the physics steps
	//

	{
		context->beginGroup( HKG_IMM_LINES );

		context->setCurrentColorPacked((unsigned int)hkColor::BLUE);// reinterpret_cast<float*>(&color) );
		for (int i = 0; i < 8; i++)
		{
			displayTimePoint( context, hkTime(i* physicsDeltaTime), physicsDeltaTime, 0.125f );
		}

		context->setCurrentColorPacked((unsigned int)hkColor::GREEN);// reinterpret_cast<float*>(&color) );

		hkTime time = world->m_simulation->getSimulateUntilTime();
		displayTimePoint( context, time, physicsDeltaTime, 0.125f );

		context->endGroup();
	}

	origView->setAsCurrent( context );
	context->unlock();
}

void hkDefaultPhysicsDemo::updateDisplay(hkpWorld* w)
{
	// XX Should only do this if we have rbs that need to be
	// update displayObjects from Asset files (no demos currently)
	if (0)
	{
		w->markForRead();

		const hkArray<hkpSimulationIsland*>& activeIslands = w->getActiveSimulationIslands();
		for(int i = 0; i < activeIslands.getSize(); i++)
		{
			const hkArray<hkpEntity*>& activeEntities = activeIslands[i]->getEntities();
			for(int j = 0; j < activeEntities.getSize(); j++)
			{
				hkpRigidBody* rigidBody = static_cast<hkpRigidBody*>(activeEntities[j]);
				hkpPropertyValue p = rigidBody->getProperty(HK_PROPERTY_DISPLAY_PTR);
				hkgDisplayObject* dispObj = reinterpret_cast<hkgDisplayObject*>( p.getPtr() );
				if (dispObj)
				{
					hkMatrix4 m;
					hkTransform transform;
					rigidBody->approxCurrentTransform( transform );
					m.set( transform );

					dispObj->setTransform((float*)&m, false);
				}
			}
		}

		w->unmarkForRead();
	}
}

void hkDefaultPhysicsDemo::postRenderDisplayWorld(hkgViewport* )
{
	if (m_displayToiInformation)
	{
		hkReal physicsDeltaTime = m_world->m_simulation->getPhysicsDeltaTime();
		if ( hkMath::equal( physicsDeltaTime, 0.0f) )
		{
			physicsDeltaTime = 1.0f;
		}
		displayTimeWheel(m_env, m_world, physicsDeltaTime);
	}
}

void hkDefaultPhysicsDemo::enableDisplayingToiInformation(hkBool enable)
{
	m_displayToiInformation = enable;
}

static hkxNode* _findMeshNodeByName(hkxNode* node, const char* name)
{
	if (!node) return HK_NULL;

	if (node->m_name && node->m_object.m_class)
	{
		if (hkString::strCmp( node->m_name, name )==0)
		{
			// have a node called the correct thing, do we have a mesh?
			if ( node->m_object.m_object && (hkString::strCmp( node->m_object.m_class->getName(), hkxMeshClass.getName()) ==0) )
			{
				return node;
			}
		}
	}
	for(int ni=0; ni < node->m_numChildren; ++ni)
	{
		hkxNode* m = _findMeshNodeByName( node->m_children[ni], name );
		if (m) return m;
	}
	return HK_NULL;
}

static hkgDisplayObject* _findDisplayObject( hkxMesh* mesh, hkgArray<hkgAssetConverter::Mapping>& mappings)
{
	for (int mi=0; mi < mappings.getSize(); ++mi)
	{
		if (mappings[mi].m_origObject == mesh)
			return (hkgDisplayObject*)( mappings[mi].m_hkgObject );
	}
	return HK_NULL;
}

static void _mergeCompoundNodes(hkgDisplayObject* dispObj, hkxNode* node, hkgArray<hkgAssetConverter::Mapping>& meshes, const hkArray<hkpRigidBody*>& rbs )
{
	// See if it is in itself an rb or not. If it is then return.
	int gi;
	for (gi=0; gi < node->m_numAttributeGroups; ++gi)
	{
		hkxAttributeGroup* group = &node->m_attributeGroups[gi];
		if ( (group->m_numAttributes > 0) && (hkString::strCmp( group->m_name, "hkpRigidBody") == 0) )
			return; // it is an rb, not a shape or ramdom mesh
	}

	// could be an old style export scene, so will not have attributes
	// check for rbs of the same name..
	if (node->m_name)
	{
		for (int rbl = 0; rbl < rbs.getSize(); ++rbl)
		{
			if (rbs[rbl]->getName() && (hkString::strCmp( rbs[rbl]->getName(), node->m_name) == 0) )
				return; // rb by the same name as this node so can assume it is this one
		}
	}

	// merge the mesh (if it is a mesh) into the rb, transforming it as required.
	if ( hkString::strCmp( node->m_object.m_class->getName(), hkxMeshClass.getName()) == 0 )
	{
		hkxMesh* theMesh = (hkxMesh*)node->m_object.m_object;
		hkgDisplayObject* toBeMerged = _findDisplayObject( theMesh, meshes );

		const float* tA = toBeMerged->getTransform();
		const float* tB = dispObj->getTransform();
		float bInv[16];
		float geomT[16];
		hkgMat4Invert(bInv, tB);
		hkgMat4Mult(geomT, bInv, tA);

		int numGeom = toBeMerged->getNumGeometry();
		for (gi=numGeom-1; gi >= 0; --gi)
		{
			hkgGeometry* geom = toBeMerged->removeGeometry(gi);
			geom->transform(geomT); // bake in relative transform
			dispObj->addGeometry(geom);
			geom->removeReference(); // remove ref given back by previous remove
		}
	}

	// recurse
	for (int ci=0; ci < node->m_numChildren; ++ci)
	{
		_mergeCompoundNodes(dispObj, node->m_children[ci], meshes, rbs );
	}
}

void hkDefaultPhysicsDemo::loadAndAddRigidBodies( hkLoader* loader, const char* filename )
{
	//hkString assetFile = hkAssetManagementUtil::getFilePath("Resources/Physics/levels/test_level.hkx");
	hkString assetFile = hkAssetManagementUtil::getFilePath(filename);
	//hkString assetFile = hkAssetManagementUtil::getFilePath("Resources/Physics/levels/onemesh_test_level.hkx");
	hkRootLevelContainer* container = loader->load( assetFile.cString() );
	HK_ASSERT2(0x27343437, container != HK_NULL , "Could not load asset");
	hkxScene* scene = reinterpret_cast<hkxScene*>( container->findObjectByType( hkxSceneClass.getName() ));

	HK_ASSERT2(0x27343635, scene, "No scene loaded");
	m_env->m_sceneConverter->convert( scene, hkgAssetConverter::CONVERT_ALL );

	hkpPhysicsData* physics = reinterpret_cast<hkpPhysicsData*>( container->findObjectByType( hkpPhysicsDataClass.getName() ));
	HK_ASSERT2(0x27343635, physics, "No physics loaded");

	// Physics
	if (physics)
	{
		const hkArray<hkpPhysicsSystem*>& psys = physics->getPhysicsSystems();

		// Tie the two together
		for (int i=0; i<psys.getSize(); i++)
		{
			hkpPhysicsSystem* system = psys[i];

			// Associate the display and physics (by name)
			if (scene)
			{
				hkDefaultPhysicsDemo::addPrecreatedDisplayObjectsByName( psys[i]->getRigidBodies(), scene );
			}
			// add the lot to the world
			m_world->addPhysicsSystem(system);
		}
	}
}

int hkDefaultPhysicsDemo::addPrecreatedDisplayObjectsByName( const hkArray<hkpRigidBody*>& rbs, const hkxScene* scene, hkArray<hkgDisplayObject*>* createdDisplayObjects, bool castShadows )
{
	int numConverted = 0;
//	hkxScene* scene = (hkxScene*) ontainer->findObjectByType( hkxSceneClass.getName() );

	m_env->m_displayWorld->lock();
	if (createdDisplayObjects)
	{
		createdDisplayObjects->setSize(rbs.getSize(), HK_NULL);
	}
	for (int rbi=0; rbi < rbs.getSize(); ++rbi)
	{
		const char* name = rbs[rbi]->getName();
		hkxNode* meshNode = _findMeshNodeByName(scene->m_rootNode, name);
		hkgDisplayObject* dispObj = HK_NULL;
		if (meshNode)
		{
			hkxMesh* mesh = (hkxMesh*)( meshNode->m_object.m_object );
			dispObj = _findDisplayObject( mesh, m_env->m_sceneConverter->m_meshes );
		}

		if (dispObj)
		{
			hkUlong id = (hkUlong)( rbs[rbi]->getCollidable() );
			// set flag before adding to help internal state (esp for shadow casters)
			if ( (castShadows) && ( !rbs[rbi]->isFixed() ) ) // make it a shadow caster.
			{
				int idx = m_env->m_displayWorld->findDisplayObject(dispObj);
				if (idx >= 0)
				{
					m_env->m_window->getContext()->lock();


					for (int ci=0; ci < meshNode->m_numChildren; ++ci)
					{
						_mergeCompoundNodes(dispObj, meshNode->m_children[ci], m_env->m_sceneConverter->m_meshes, rbs );
					}

					m_env->m_displayWorld->removeDisplayObject(idx);// gives back ref
					dispObj->setStatusFlags( dispObj->getStatusFlags() | HKG_DISPLAY_OBJECT_DYNAMIC | HKG_DISPLAY_OBJECT_SHADOW );

					dispObj->computeAABB();
					m_env->m_window->getContext()->unlock();

					m_env->m_displayWorld->addDisplayObject(dispObj);
					dispObj->release();


				}
				}

			if (createdDisplayObjects)
			{
				(*createdDisplayObjects)[rbi] = dispObj;
			}

			m_env->m_displayHandler->addPrecreatedDisplayObject( id, dispObj );
			numConverted++;
		}
	}

	// due to merging, we may be left with some empty display objects at this stage, so we can remove them
	// to avoid work
	for (int doi=0; doi < m_env->m_displayWorld->getNumDisplayObjects();)
	{
		if (m_env->m_displayWorld->getDisplayObject(doi)->getNumGeometry() < 1)
		{
			m_env->m_displayWorld->removeDisplayObject(doi)->removeReference();
		}
		else
		{
			doi++;
		}
	}

	m_env->m_displayWorld->unlock();

	return numConverted;
}

hkgDisplayObject* hkDefaultPhysicsDemo::findMeshDisplay( const char* meshName, const hkxScene* scene )
{
	if (meshName)
	{
		hkxNode* node = _findMeshNodeByName(scene->m_rootNode, meshName);
		if (node && node->m_object.m_object)
		{
			hkxMesh* mesh = (hkxMesh*)( node->m_object.m_object );
			hkgDisplayObject* dispObj = _findDisplayObject( mesh, m_env->m_sceneConverter->m_meshes );
			return dispObj;
		}
	}
	return HK_NULL;
}

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
