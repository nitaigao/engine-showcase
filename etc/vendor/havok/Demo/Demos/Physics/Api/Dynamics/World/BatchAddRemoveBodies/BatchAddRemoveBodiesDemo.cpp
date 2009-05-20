/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


#include <Demos/demos.h>
#include <Demos/Physics/Api/Dynamics/World/BatchAddRemoveBodies/BatchAddRemoveBodiesDemo.h>



	// We will need these shapes
#include <Demos/DemoCommon/Utilities/GameUtils/Landscape/FlatLand.h>

#include <Graphics/Common/Input/Pad/hkgPad.h>

#include <Common/Base/System/Stopwatch/hkStopwatch.h>

#include <Demos/DemoCommon/DemoFramework/hkTextDisplay.h>



	// We need to create a phantom
#include <Physics/Dynamics/Phantom/hkpAabbPhantom.h>

// this demonstrates how to add and remove entities in large batches.
// the performance timings will be output to the screen
// display of the bodies is disabled to avoid graphics setup code polluting the timings
// button 0 clears the screen
// button 1 adds 100 bodies
// button 2 removes 100 bodes
// button 3 toggles between batch add and adding objects one at a time
// all objects are added in a non-interpenetrating state.
// a maximum of 1200 objects can be in the scene

#ifdef HK_PS2
# include <libpc.h>
#endif

//!me  add a phantom to for testing purposes
/// An example of a phantom implementation, which just counts the penetrating shapes
class MyPhantom: public hkpAabbPhantom
{
	public:

		MyPhantom( const hkAabb& aabb ): hkpAabbPhantom(aabb), m_penetratingShapesCount(0) { }

		int m_penetratingShapesCount;


		virtual void addOverlappingCollidable( hkpCollidable* handle )
		{
			m_penetratingShapesCount++; 
		}
			/// This callback is called when the phantom shape stops intersecting with another shape.
		virtual void removeOverlappingCollidable( hkpCollidable* handle )
		{
			m_penetratingShapesCount--;
		}
};


class myFlatLand : public FlatLand
{
	public:
		
		hkReal getHeight( int i, int j )
		{
			return 0;
		}

		hkReal getBorderHeight()
		{
			return 6.0f;
		}

};


BatchAddRemoveBodiesDemo::BatchAddRemoveBodiesDemo(hkDemoEnvironment* env)
:	hkDefaultPhysicsDemo(env), m_rand(123123), m_capture( 512*1024*0 ), m_textOutput( "" )
{

	m_addRemoveToggle = false;

	m_batchMode = true;
	m_numBodies = 1;

	//
	// Setup the camera. Actually overwritten by step function, and when we fisrt add the vehicle.
	//
	{
		hkVector4 from(50.0f, 150.0f, 50);
		hkVector4 to(50.0f, 0, 60);
		hkVector4 up(0.f, 1.f, 0.f);
		setupDefaultCameras( env, from, to, up );
	}

	//
	// Create the world
	//
	{
		hkpWorldCinfo info;
		info.m_collisionTolerance = 0.01f;
		info.m_gravity.set(0, -9.8f, 0);
		info.setBroadPhaseWorldSize(2850.0f) ;
		info.setupSolverInfo(hkpWorldCinfo::SOLVER_TYPE_4ITERS_MEDIUM);
		// NOTE: Disabling this until the marker crash issue is fixed.
		// info.m_broadPhaseNumMarkers = 8;
		m_world = new hkpWorld( info );
		m_world->lock();

		// agents
		hkpAgentRegisterUtil::registerAllAgents(m_world->getCollisionDispatcher());
		//graphics
		
		m_showGraphics = false;
		if(m_showGraphics)
		{
			setupGraphics();	
		}
	}


	//
	// Create the ground we'll drive on
	//
	{ 
		m_myFlatLand = new myFlatLand();

		hkpMoppBvTreeShape* groundShape = m_myFlatLand->createMoppShape();
		{
			hkpRigidBodyCinfo groundInfo;
			groundInfo.m_shape = groundShape;
			groundInfo.m_position.set(5.0f, 0, 5.0f);
			groundInfo.m_motionType = hkpMotion::MOTION_FIXED;
			groundInfo.m_friction = 2.0f;
			hkpRigidBody* groundbody = new hkpRigidBody(groundInfo);
			m_world->addEntity(groundbody);
			groundbody->removeReference();
		}
		groundShape->removeReference();
	}

	{
		m_gridWidth = 100;
		m_gridHeight = 100;

		m_cellSize = 2.0f;

		m_gridOccupied.setSize( m_gridWidth * m_gridHeight );
		m_entities.setSize( m_gridWidth * m_gridHeight );

		int x,y;
		for( x = 0; x < m_gridWidth; x++ )
		{
			for( y = 0; y < m_gridHeight; y++ )
			{
				m_gridOccupied[ y*m_gridWidth + x ] = false;
			}
		}
	}

	// QA test for phantoms with batchadd/remove
	if(0)
	{
		hkAabb infoAABB;
		infoAABB.m_min = hkVector4( -50.0f, 0.0f, -50.0f );
		infoAABB.m_max = hkVector4( 50.0f, 20.0f, 50.0f ) ;
		MyPhantom* phantom = new MyPhantom( infoAABB );
		m_world->addPhantom( phantom );
		phantom->removeReference(); 
	}

	int numBodies = m_env->m_cpuMhz / 10;
	addToGrid( numBodies, m_batchMode );

	m_timer.reset();
	m_timer.start();

	m_world->unlock();
}


double BatchAddRemoveBodiesDemo::addToGrid( int num, hkBool addBatch )
{
	hkArray< hkpEntity* > bodies;
	bodies.reserve(num);

	hkVector4 halfExtent( 0.5f, 0.5f, 0.5f);
	hkpShape* shape = new hkpBoxShape( halfExtent);

	int i;
	for( i = 0; i < num; i++ )
	{
		int guard = 10000;
		int x = m_rand.getRandChar( m_gridWidth );
		int y = m_rand.getRandChar( m_gridHeight );
		do
		{
			x = (x+1);
			if ( x >= m_gridWidth)
			{
				x = 0; 
				y = (y+1) % m_gridHeight;
			}
		}while( m_gridOccupied[ y*m_gridWidth + x ] && guard-- > 0 );

		hkVector4 spherePosition;
		spherePosition.set( x * m_cellSize, 1.1f, y * m_cellSize );


		hkpRigidBodyCinfo info;

		hkpMassProperties massProperties;
		hkpInertiaTensorComputer::computeSphereVolumeMassProperties(1.0f, 5.0f, massProperties);

		info.m_restitution = 0.1f;
		info.m_mass = 5.0f;
		info.m_shape = shape;
		if ( i & 1)
		{
			info.m_motionType = hkpMotion::MOTION_SPHERE_INERTIA;
		}
		else
		{
			info.m_motionType = hkpMotion::MOTION_FIXED;
		}

		info.m_inertiaTensor = massProperties.m_inertiaTensor;

		// Create movable sphere
		hkpRigidBody* sphere = new hkpRigidBody(info);


		if( guard > 0 )
		{
			sphere->setPosition(spherePosition);

			bodies.pushBackUnchecked( sphere );
			m_entities[ y*m_gridWidth + x ] = sphere;
			m_gridOccupied[ y*m_gridWidth + x ] = true;
		}
	}
	shape->removeReference();


	hkUint64 ticks = m_timer.getElapsedTicks();

	num = bodies.getSize();

	// add as a batch
	if( addBatch )
	{
		m_world->addEntityBatch( bodies.begin(), bodies.getSize() );
		for( i = 0; i < num; i++ )
		{
			bodies[i]->removeReference();
		}
	}
	// add one at a time
	else
	{
		HK_TIMER_BEGIN("AddOneByOne", HK_NULL);
		for( i = 0; i < num; i++ )
		{
			m_world->addEntity( bodies[i] );
			bodies[i]->removeReference();
		}
		HK_TIMER_END();
	}

	m_numBodies += num;

	double elapsedS = double(m_timer.getElapsedTicks() - ticks);
	elapsedS *= 1.0/double(m_timer.getTicksPerSecond()); 

	return elapsedS;

}


double BatchAddRemoveBodiesDemo::removeFromGrid( int num, hkBool removeBatch )
{
	hkInplaceArray< hkpEntity*, 1024 > bodies;
	bodies.reserve(num);

	int i;
	for( i = 0; i < num; i++ )
	{
		int guard = 10000;
		int x = m_rand.getRandChar( m_gridWidth );
		int y = m_rand.getRandChar( m_gridHeight );
		do
		{
			x = (x+1);
			if ( x >= m_gridWidth)
			{
				x = 0; 
				y = (y+1) % m_gridHeight;
			}
		}while( !m_gridOccupied[ y*m_gridWidth + x ] && guard-- > 0 );
		
		if( guard > 0 )
		{
			bodies.pushBackUnchecked( m_entities[ y*m_gridWidth + x ] );
			m_gridOccupied[ y*m_gridWidth + x ] = false;
		}

	}


	hkUint64 ticks = m_timer.getElapsedTicks();

	// remove as a batch
	if( removeBatch )
	{
		m_world->removeEntityBatch( bodies.begin(), bodies.getSize() );
	}
	// remove one at a time
	else
	{
		HK_TIMER_BEGIN("RemOneByOne", HK_NULL);
		for( i = 0; i < bodies.getSize(); i++ )
		{
			m_world->removeEntity( bodies[i] );
		}
		HK_TIMER_END();
	}

	m_numBodies -= bodies.getSize();

	double elapsedS = double(m_timer.getElapsedTicks() - ticks);
	elapsedS *= 1.0/double(m_timer.getTicksPerSecond()); 

	return elapsedS;

}


hkDemo::Result BatchAddRemoveBodiesDemo::stepDemo()
{
	hkBool statsLogOn = false;
	hkMonitorStreamFrameInfo frameInfo;

	{
		m_world->lock();

		//begin frame
		const hkgPad* pad = m_env->m_gamePad;

		if( statsLogOn )
		{
			hkMonitorStream& stream = hkMonitorStream::getInstance();
			stream.resize( 5000000 ); // 2 meg for timer info
			stream.reset();
#ifdef HK_PS2
			scePcStart( SCE_PC_CTE | SCE_PC_U0 | SCE_PC_U1 | SCE_PC0_CPU_CYCLE | SCE_PC1_CPU_CYCLE, 0 ,0 );  
			//  scePcStart( SCE_PC_CTE | SCE_PC_U0 | SCE_PC_U1 | SCE_PC0_NO_EVENT | SCE_PC1_BRANCH_MISS_PREDICT, 0 ,0 );
			//  scePcStart( SCE_PC_CTE | SCE_PC_U0 | SCE_PC_U1 | SCE_PC1_DCACHE_MISS | SCE_PC0_CPU_CYCLE, 0 ,0 );
			//scePcStart( SCE_PC_CTE | SCE_PC_U0 | SCE_PC_U1 | SCE_PC0_ICACHE_MISS | SCE_PC1_CPU_CYCLE, 0 ,0 );
			frameInfo.m_indexOfTimer0 = 1;
			frameInfo.m_indexOfTimer1 = 0;
			frameInfo.m_timerFactor0 = 35.0f / 300.f;
			frameInfo.m_timerFactor1 = 1.0f / 300.f;
			frameInfo.m_heading = "usec: IcachePenalty( assuming 35cycles penalty)";
#	else
			frameInfo.m_indexOfTimer0 = 0;
			frameInfo.m_indexOfTimer1 = -1;
			frameInfo.m_heading = "usecs";
			frameInfo.m_timerFactor0 = 1e6f / hkReal(hkStopwatch::getTicksPerSecond());
#endif
		}

		//!me these are massively heavyweight stats tests, which brings any mortal machine to it's knees.
		//!me so go with the streamlined ones below.
		if(0)
		{
			HK_TIMER_BEGIN("SingleAdd10", HK_NULL); addToGrid( 10, false );  HK_TIMER_END();
			HK_TIMER_BEGIN("SingleAdd100", HK_NULL); addToGrid( 100, false );  HK_TIMER_END();
			HK_TIMER_BEGIN("SingleAdd1000", HK_NULL); addToGrid( 1000, false );  HK_TIMER_END();
			HK_TIMER_BEGIN("SingleAdd100b", HK_NULL); addToGrid( 100, false );  HK_TIMER_END();
			HK_TIMER_BEGIN("SingleAdd10b", HK_NULL); addToGrid( 10, false );  HK_TIMER_END();
			HK_TIMER_BEGIN("SingleRem10", HK_NULL); removeFromGrid( 10, false );  HK_TIMER_END();
			HK_TIMER_BEGIN("SingleRem100", HK_NULL); removeFromGrid( 100, false );  HK_TIMER_END();
			HK_TIMER_BEGIN("SingleRem1000", HK_NULL); removeFromGrid( 1000, false );  HK_TIMER_END();
			HK_TIMER_BEGIN("SingleRem100b", HK_NULL); removeFromGrid( 100, false );  HK_TIMER_END();
			HK_TIMER_BEGIN("SingleRem10b", HK_NULL); removeFromGrid( 10, false );  HK_TIMER_END();

			HK_TIMER_BEGIN("BatchAdd10", HK_NULL); addToGrid( 10, true );  HK_TIMER_END();
			HK_TIMER_BEGIN("BatchAdd100", HK_NULL); addToGrid( 100, true );  HK_TIMER_END();
			HK_TIMER_BEGIN("BatchAdd1000", HK_NULL); addToGrid( 1000, true );  HK_TIMER_END();
			HK_TIMER_BEGIN("BatchAdd100b", HK_NULL); addToGrid( 100, true );  HK_TIMER_END();
			HK_TIMER_BEGIN("BatchAdd10b", HK_NULL); addToGrid( 10, true );  HK_TIMER_END();
			HK_TIMER_BEGIN("BatchRem10", HK_NULL); removeFromGrid( 10, true );  HK_TIMER_END();
			HK_TIMER_BEGIN("BatchRem100", HK_NULL); removeFromGrid( 100, true );  HK_TIMER_END();
			HK_TIMER_BEGIN("BatchRem1000", HK_NULL); removeFromGrid( 1000, true );  HK_TIMER_END();
			HK_TIMER_BEGIN("BatchRem100b", HK_NULL); removeFromGrid( 100, true );  HK_TIMER_END();
			HK_TIMER_BEGIN("BatchRem10b", HK_NULL); removeFromGrid( 10, true );  HK_TIMER_END();

		}
		else
		{
			HK_TIMER_BEGIN("SingleAdd100", HK_NULL); addToGrid( 100, false );  HK_TIMER_END();
			HK_TIMER_BEGIN("SingleAdd100", HK_NULL); addToGrid( 100, false );  HK_TIMER_END();
			HK_TIMER_BEGIN("SingleRem100", HK_NULL); removeFromGrid( 100, false );  HK_TIMER_END();
			HK_TIMER_BEGIN("SingleRem100", HK_NULL); removeFromGrid( 100, false );  HK_TIMER_END();
			// single add and remove is too expensive to run each frame.  Just do the batch, for testing.
			HK_TIMER_BEGIN("BatchAdd100", HK_NULL); addToGrid( 100, true );  HK_TIMER_END();
			HK_TIMER_BEGIN("BatchAdd100", HK_NULL); addToGrid( 100, true );  HK_TIMER_END();
			HK_TIMER_BEGIN("BatchRem100", HK_NULL); removeFromGrid( 100, true );  HK_TIMER_END();
			HK_TIMER_BEGIN("BatchRem100", HK_NULL); removeFromGrid( 100, true );  HK_TIMER_END();

			// always show graphics for boot-strap test ( default ).
			if( !m_showGraphics )
			{
				setupGraphics();	
				m_showGraphics = true;
			}
		}

		m_timer.reset();
		m_timer.start();

		char buffer[64];


		if( pad->wasButtonPressed(HKG_PAD_BUTTON_1) )
		{
			int prevBods = m_numBodies;
			int numAdded = m_numBodies;
			double timeToAdd = addToGrid( 100, m_batchMode );
			numAdded = m_numBodies - numAdded;
			hkString::sprintf(buffer, "%i + %i: %f seconds to add\n", prevBods, numAdded, timeToAdd );
			m_textOutput += buffer;

		}

		if( pad->wasButtonPressed(HKG_PAD_BUTTON_2) )
		{
			int prevBods = m_numBodies;
			int numRemoved = m_numBodies;
			double timeToRemove = removeFromGrid( 100, m_batchMode );
			numRemoved = numRemoved - m_numBodies;
			hkString::sprintf(buffer, "%i - %i: %f seconds to remove\n", prevBods, numRemoved, timeToRemove );
			m_textOutput += buffer;

		}

		if( pad->wasButtonPressed(HKG_PAD_BUTTON_3) )
		{
			m_batchMode = !m_batchMode; 

			hkString::sprintf(buffer, "Batch Mode: %s\n", (( m_batchMode ) ? "on" : "off" ) );
			m_textOutput += buffer;
		}

		if( pad->wasButtonPressed(HKG_PAD_BUTTON_0) )
		{
			m_textOutput = "";
		}


		if( pad->wasButtonPressed( HKG_PAD_BUTTON_L1) )
		{
			if( !m_showGraphics )
			{
				setupGraphics();	
				m_showGraphics = true;
			}
		}

		m_env->m_textDisplay->outputText(m_textOutput, 20,220 );

		m_world->unlock();
	}

	hkDemo::Result r = hkDefaultPhysicsDemo::stepDemo();

	{
		m_world->lock();

		if( statsLogOn )
		{
			hkMonitorStream& stream = hkMonitorStream::getInstance();
			m_capture.captureFrameDetails(stream.getStart(), stream.getEnd(), frameInfo );
		}

		m_world->unlock();
	}

	return r;
}


BatchAddRemoveBodiesDemo::~BatchAddRemoveBodiesDemo()
{
	m_world->lock();

	m_myFlatLand->removeReference();

	m_world->unlock();
}


#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
"Adding an removing objects in reasonable batches is faster than adding objects individually " \
"The broadphase cost is spread over the batch operations.\n" \
"Press \221 to add, \222 to remove and \223 to switch modes\n" \
"Press \220 to clear the text screen\n" \
"Press \232 to show graphics";

HK_DECLARE_DEMO(BatchAddRemoveBodiesDemo, HK_DEMO_TYPE_PRIME, "Add and remove objects in batches", helpString);

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
