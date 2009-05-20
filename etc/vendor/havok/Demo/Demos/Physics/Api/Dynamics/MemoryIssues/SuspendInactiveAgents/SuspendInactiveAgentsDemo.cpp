/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#include <Demos/demos.h>
#include <Common/Base/Memory/Memory/hkMemory.h>
#include <Common/Base/DebugUtil/StatisticsCollector/Simple/hkSimpleStatisticsCollector.h>

#include <Physics/Collide/Query/CastUtil/hkpWorldRayCastInput.h>
#include <Physics/Collide/Query/CastUtil/hkpWorldRayCastOutput.h>

#include <Demos/Physics/Api/Dynamics/MemoryIssues/SuspendInactiveAgents/SuspendInactiveAgentsDemo.h>
#include <Physics/Collide/Shape/HeightField/SampledHeightField/hkpSampledHeightFieldShape.h>

#include <Physics/Dynamics/World/hkpPhysicsSystem.h>
#include <Demos/DemoCommon/Utilities/GameUtils/Landscape/FlatLand.h>
#include <Demos/DemoCommon/Utilities/GameUtils/GameUtils.h>
#include <Demos/DemoCommon/DemoFramework/hkTextDisplay.h>
#include <Physics/Utilities/Dynamics/SuspendInactiveAgents/hkpSuspendInactiveAgentsUtil.h>
#include <Physics/Utilities/VisualDebugger/Viewer/Dynamics/hkpSimulationIslandViewer.h>

#include <Common/Serialize/Util/hkBuiltinTypeRegistry.h>

SuspendInactiveAgentsDemo::SuspendInactiveAgentsDemo( hkDemoEnvironment* env )
:	hkDefaultPhysicsDemo(env)
{
	
	// Disable warnings:									
	hkError::getInstance().setEnabled(0x7dd65995, false); //'The system has requested a heap allocation on stack overflow.'
	hkError::getInstance().setEnabled(0x86bc014f, false); //'For the default implementation to work the class must be passed in'

	//
	// Setup the camera. Actually overwritten by step function, and when we first add the vehicle.
	//
	{
		hkVector4 from(0.0f, 2.0f, 12.0f);
		hkVector4 to(0.0f, 0.0f, 0.0f);
		hkVector4 up(0.0f, 1.0f, 0.0f);
		setupDefaultCameras( m_env, from, to, up );
	}

	//
	// Create the world.
	//
	{
		hkpWorldCinfo info;
		info.m_collisionTolerance = 0.01f;
		info.m_gravity.set(0.0f, -9.8f, 0.0f);
		info.setBroadPhaseWorldSize(850.0f) ;
		info.setupSolverInfo(hkpWorldCinfo::SOLVER_TYPE_4ITERS_MEDIUM);
		m_world = new hkpWorld( info );
		m_world->lock();

		// Register all agents.
		hkpAgentRegisterUtil::registerAllAgents(m_world->getCollisionDispatcher());
		// Graphics.
		m_debugViewerNames.pushBack( hkpSimulationIslandViewer::getName());
		setupGraphics();
	}

	//
	//	Enable deleting inactive agents
	//
	{
		m_suspendInactiveAgentsUtil = new hkpSuspendInactiveAgentsUtil( m_world, hkpSuspendInactiveAgentsUtil::SUSPEND_ALL_COLLECTION_AGENTS );
	}

	//
	// Create a filter, so that the raycasts of car does not collide with the ragdolls
	//
	{
		hkpGroupFilter* filter = new hkpGroupFilter();
		filter->disableCollisionsBetween(1,3);

		m_world->setCollisionFilter( filter );
		filter->removeReference();
	}

	// Build the landscape to drive on and add it to m_world.
	buildLandscape();

	if (getDemoVariant() < 2)
	{
		int gridSize = hkMath::clamp(m_env->m_cpuMhz / 150, 2, 4);
		createRagdollGrid( m_world, gridSize, gridSize, 4.0f, 4.0f );
		m_boxRigidBody = HK_NULL;
	}
	else
	{
		//
		// Warm starting manifold - just illustrate with one box on a simple landscape
		//

		hkVector4 boxRadii( 1, .2f, 1);
		hkpShape* smallBox = new hkpBoxShape(boxRadii, 0 );

		hkpRigidBodyCinfo boxInfo;

		boxInfo.m_mass = 1.0f;
		boxInfo.m_shape = smallBox;
		hkpInertiaTensorComputer::setShapeVolumeMassProperties(boxInfo.m_shape, boxInfo.m_mass, boxInfo);
		boxInfo.m_rotation.setIdentity();
		boxInfo.m_motionType = hkpMotion::MOTION_BOX_INERTIA;
		boxInfo.m_position.set(2, 2, 2);
		boxInfo.m_restitution = .3f;
		boxInfo.m_qualityType = HK_COLLIDABLE_QUALITY_CRITICAL; //XXX

		m_boxRigidBody = new hkpRigidBody(boxInfo);
		smallBox->removeReference();

		m_world->addEntity( m_boxRigidBody );
		m_frameCount = 0;

		// Change the camera position to view the box close up

		hkVector4 from(0.91f, 0.87f, 5.52f);
		hkVector4 to(2.66f, -.16f, -.06f);
		hkVector4 up(0.0f, 1.0f, 0.0f);
		setupDefaultCameras( m_env, from, to, up );

	}

	m_world->unlock();
}

SuspendInactiveAgentsDemo::~SuspendInactiveAgentsDemo()
{
	// Re-enable warnings:									
	hkError::getInstance().setEnabled(0x7dd65995, true);
	hkError::getInstance().setEnabled(0x86bc0150, true);

	m_world->lock();

	if ( m_landscape )
	{
		m_landscape->removeReference();
		m_landscape = HK_NULL;
	}
	m_suspendInactiveAgentsUtil->removeReference();

	if (m_boxRigidBody)
	{
		m_boxRigidBody->removeReference();
	}

	m_world->unlock();
}


hkDemo::Result SuspendInactiveAgentsDemo::stepDemo()
{
	// If it is the warm starting variant, activate the body every 60 frames, and alternate between
	// warm starting and not warm starting
	if (getDemoVariant() == 2)
	{
		if ( m_frameCount % 300 == 0 )
		{
			if ( m_suspendInactiveAgentsUtil->m_initContactsMode == hkpSuspendInactiveAgentsUtil::INIT_CONTACTS_FIND )
			{
				m_suspendInactiveAgentsUtil->m_initContactsMode = hkpSuspendInactiveAgentsUtil::INIT_CONTACTS_DO_NOT_FIND;
				m_env->m_textDisplay->outputText("Not finding additional contacts (causes jitter on activation).", 20, 80, 0xffffffff, 290 );
			}
			else
			{
				m_suspendInactiveAgentsUtil->m_initContactsMode = hkpSuspendInactiveAgentsUtil::INIT_CONTACTS_FIND;
				m_env->m_textDisplay->outputText("Finding additional contacts (Default)", 20, 80, 0xffffffff, 290 );
			}
		}

		m_frameCount++;
		if ( m_frameCount % 60 == 0 && m_boxRigidBody )
		{
			m_world->lock();
			m_boxRigidBody->activate();
			m_world->unlock();
			m_env->m_textDisplay->outputText("Activating Body", 20, 120, 0xffffffff, 30 );
		}
	}



	hkDefaultPhysicsDemo::stepDemo();

	{
		m_world->lock();

		int currentPhysicsTotalMemory;
		{
			hkSimpleStatisticsCollector collector(hkBuiltinTypeRegistry::getInstance().getVtableClassRegistry());
            collector.beginSnapshot( 0 );
            collector.addReferencedObject( "World", m_world );
			collector.commitSnapshot(  );
			hkMonitorStreamAnalyzer::Node* rootNode = collector.m_snapshots[0];
			hkReal* values = &rootNode->m_children[0]->m_value[0];
			currentPhysicsTotalMemory = int(values[1]);
		}

		{
			hkMemoryStatistics stats;
			hkMemory::getInstance().calculateStatistics(stats);

			const int h = m_env->m_window->getHeight();
			hkString str;
            str.printf("Total memory used:      %i\nTotal physics memory:   %i", stats.m_used, currentPhysicsTotalMemory);
			m_env->m_textDisplay->outputText(str, 20, h-70);
		}

		m_world->unlock();
	}

	return DEMO_OK;
}
///>


class SuspendInactiveAgentsDemoSampledHeightFieldShape: public hkpSampledHeightFieldShape
{
	public:
		SuspendInactiveAgentsDemoSampledHeightFieldShape( const hkpSampledHeightFieldBaseCinfo& ci ): hkpSampledHeightFieldShape(ci)
		{
		}

		HK_FORCE_INLINE hkReal getHeightAtImpl( int x, int z ) const	{		return 0.0f;		}
		HK_FORCE_INLINE hkBool getTriangleFlipImpl() const		{		return false;		}
		virtual void collideSpheres( const CollideSpheresInput& input, SphereCollisionOutput* outputArray) const
		{
			hkSampledHeightFieldShape_collideSpheres(*this, input, outputArray);
		}
};

void SuspendInactiveAgentsDemo::buildLandscape()
{
	//
	// Create the ground we'll drive on.
	//
	{
		hkpRigidBodyCinfo groundInfo;


		//
		//	Set the if condition to 0 if you want to test the heightfield
		//
		const int dim = 64;
		const hkReal s = 0.3f;
		if ( this->getDemoVariant() == 1 )
		{
			// Triangle mesh
			FlatLand* fl = new FlatLand( dim );
			m_landscape = fl;
			hkVector4 scaling( s,0.0f,s );
			fl->setScaling( scaling );
			groundInfo.m_shape = fl->createMoppShape();
			//groundInfo.m_position.setMul4( -0.5f * dim, scaling );
			groundInfo.m_position(1) = 0;
		}
		else if (getDemoVariant() == 0)
		{
			// Heightfield
			hkpSampledHeightFieldBaseCinfo ci;
			ci.m_xRes = dim;
			ci.m_zRes = dim;
			ci.m_scale.set( s, 0.5f, s );
			m_landscape = HK_NULL;
			groundInfo.m_shape = new SuspendInactiveAgentsDemoSampledHeightFieldShape( ci );
			groundInfo.m_position.set(-0.5f * ci.m_xRes * ci.m_scale(0), -2.0f, -0.5f * ci.m_zRes * ci.m_scale(2));
		}
		else
		{
			// Warm starting collision manifold
			FlatLand* fl = new FlatLand( 2 );
			fl->setTriangleRadius(0);
			m_landscape = fl;
			hkVector4 scaling( 100,0,100);
			fl->setScaling( scaling );
			groundInfo.m_shape = fl->createMoppShape();
		}



		{
			groundInfo.m_motionType = hkpMotion::MOTION_FIXED;
			groundInfo.m_friction = 2.0f;
			if (getDemoVariant() == 2)
			{
				groundInfo.m_position.set(50,0,50);
			}
			hkpRigidBody* groundbody = new hkpRigidBody(groundInfo);

			m_world->addEntity(groundbody);
			groundbody->removeReference();
		}

		groundInfo.m_shape->removeReference();
	}
}

//
// Create a grid of ragdolls
//
void SuspendInactiveAgentsDemo::createRagdollGrid( hkpWorld* world, int x_size, int y_size, hkReal xStep, hkReal yStep )
{
	int systemGroup = 2;
	int collisionLayer = 2;
	hkReal ragdollHeight = 2.50f;

	for( int x = 0; x < x_size; x++ )
	{
		for( int y = 0; y < y_size; y++ )
		{
			hkVector4 position;
			// do a raycast to place the ragdoll
			{
				hkpWorldRayCastInput ray;
				ray.m_from.set( (x- 0.5f * x_size) * xStep,  10, (y-0.5f*y_size) * yStep );
				ray.m_to = ray.m_from;
				ray.m_to(1) = -10;
				hkpWorldRayCastOutput result;
				world->castRay( ray, result );
				position.setInterpolate4( ray.m_from, ray.m_to, result.m_hitFraction );
				position(1) += ragdollHeight* 0.5f;
			}

			hkQuaternion	rotation; rotation.setIdentity();

			rotation.setAxisAngle( hkTransform::getIdentity().getColumn(0), HK_REAL_PI  * -0.5f );

			hkpPhysicsSystem* ragdoll = GameUtils::createRagdoll( ragdollHeight, position, rotation, systemGroup, GameUtils::RPT_CAPSULE );
			hkpInertiaTensorComputer::optimizeInertiasOfConstraintTree(	ragdoll->getConstraints().begin(),
																		ragdoll->getConstraints().getSize(),
																		ragdoll->getRigidBodies()[0] );
			{
				for ( int i = 0; i < ragdoll->getRigidBodies().getSize(); i++)
				{
					hkpRigidBody* rb = ragdoll->getRigidBodies()[i];
					rb->setCollisionFilterInfo( hkpGroupFilter::calcFilterInfo( collisionLayer, systemGroup ) );
				}
			}
			systemGroup++;

			world->addPhysicsSystem(ragdoll);
			ragdoll->getRigidBodies()[0]->deactivate();
			ragdoll->removeReference();
		}
	}
}




#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif
static const char* help = "Test the memory consumption  of boxes on a landscape";
static const char* details =
"This demos shows the runtime memory consumption of the havok engine. "	\
"The landscape is highly tesselated, so this demos shows kind of worst case scenarios for game landscapes (hopefully). "\
"Also this demo uses the hkpSuspendInactiveAgentsUtil helper to reduce the runtime memory when ragdolls get deactivated. "\
"Activate ragdolls by picking them with the cursor.";

static const char* warmStartDetails =
"This variant shows the difference in quality between 'warm starting' a contact manifold and not doing so.";


HK_DECLARE_DEMO_VARIANT(SuspendInactiveAgentsDemo, HK_DEMO_TYPE_PHYSICS, "Using heightfield", 0, help, details);
HK_DECLARE_DEMO_VARIANT(SuspendInactiveAgentsDemo, HK_DEMO_TYPE_PHYSICS, "Mopp with small triangles", 1, help, details);
HK_DECLARE_DEMO_VARIANT(SuspendInactiveAgentsDemo, HK_DEMO_TYPE_PHYSICS, "Warm starting manifold", 2, help, warmStartDetails);


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
