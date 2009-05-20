/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>

#include <Demos/Physics/Api/Dynamics/World/WorldSnapshotWithContactPoints/WorldSnapshotWithContactPointsDemo.h>


// Serialize stuff
#include <Physics/Utilities/Serialize/hkpHavokSnapshot.h>
#include <Physics/Utilities/Serialize/hkpPhysicsData.h>
#include <Common/Base/System/Io/IStream/hkIStream.h>
#include <Physics/Dynamics/Phantom/hkpPhantom.h>
#include <Demos/DemoCommon/Utilities/Asset/hkAssetManagementUtil.h>

#include <Physics/Dynamics/World/hkpPhysicsSystem.h>

#include <Physics/Collide/Shape/Compound/Collection/List/hkpListShape.h>

#include <Graphics/Common/Input/Pad/hkgPad.h>

#include <Physics/Utilities/VisualDebugger/Viewer/Collide/hkpActiveContactPointViewer.h>
#include <Physics/Utilities/VisualDebugger/Viewer/Collide/hkpInactiveContactPointViewer.h>


#include <Physics/Utilities/Dynamics/SaveContactPoints/hkpSaveContactPointsUtil.h>

#include <Common/Base/Thread/Job/ThreadPool/Cpu/hkCpuJobThreadPool.h>

static const char ORIGINAL_FILE[] = "Resources/Physics/Api/Dynamics/World/WorldSnapshotWithContactPoints/ChairsAndTables.hkx";

static const char SCRATCH_FILE[] = "Resources/Physics/Api/Dynamics/World/WorldSnapshotWithContactPoints/snapshot.hkx";


static hkpWorld* loadWorld( const char* path, hkpPhysicsData** physicsData, hkPackfileReader::AllocatedData** memData )
{
	hkIstream infile( path );
	HK_ASSERT( 0x215d080c, infile.isOk() );
	*physicsData = hkpHavokSnapshot::load(infile.getStreamReader(), memData);

	hkpWorld* w = (*physicsData)->createWorld();
	return w;
}

static void saveWorld( hkpWorld* world, const char* path, bool binary )
{
	hkStructureLayout::LayoutRules layouts[] =   {	hkStructureLayout::MsvcWin32LayoutRules, 
													hkStructureLayout::MsvcXboxLayoutRules,
													hkStructureLayout::Xbox360LayoutRules, 
													hkStructureLayout::Sn31Ps2LayoutRules,
													hkStructureLayout::Gcc32Ps2LayoutRules, 
													hkStructureLayout::GccPs3LayoutRules 
												  };

	const int numLayouts = sizeof(layouts) /  sizeof (hkStructureLayout);

	const bool saveContactPoints = true;

	for (int i = 0; i < numLayouts; i++)
	{
		hkString filename = hkAssetManagementUtil::getFilePath( path, layouts[i] );
		hkOstream outfile( filename.cString() );
		HK_ON_DEBUG( hkBool res = ) hkpHavokSnapshot::save(world, outfile.getStreamWriter(), binary, &layouts[i], saveContactPoints );
		HK_ASSERT( 0x215d080d, res );
	}
}


WorldSnapshotWithContactPointsDemo::WorldSnapshotWithContactPointsDemo( hkDemoEnvironment* env) 
	: hkDefaultPhysicsDemo(env) 
{
	hkpWorld::IgnoreForceMultithreadedSimulation ignoreForceMultithreaded;

	// Setup the camera
	{
		hkVector4 from(10, -10, 6);
		hkVector4 to(0, 0, -4);
		hkVector4 up(0, 0, 1);
		setupDefaultCameras( env, from, to, up );
	}

	// Load the startup scene
	{
		// Load the world.
		m_world = loadWorld(ORIGINAL_FILE, &m_physicsData, &m_loadedData);
		m_world->lock();

		m_debugViewerNames.pushBack( hkpActiveContactPointViewer::getName() );
		m_debugViewerNames.pushBack( hkpInactiveContactPointViewer::getName() );

		// Disable culling
		setGraphicsState(HKG_ENABLED_CULLFACE, false);

		// Enable wire frame display mode
		setGraphicsState(HKG_ENABLED_WIREFRAME, true);

		// Setup graphics
		setupGraphics();
		m_world->unlock();
	}

	// Disable warning: 'm_contactRestingVelocity not set, setting it to REAL_MAX, so that the new collision restitution code will be disabled'
	hkError::getInstance().setEnabled(0xf03243ed, false);
}


void WorldSnapshotWithContactPointsDemo::cleanupWorld()
{
	if ( m_jobThreadPool != HK_NULL )
	{
		m_jobThreadPool->waitForCompletion();

		// Destroy m_jobThreadPool before deleting the world (as the world might be still locked by physics threads).
		delete m_jobThreadPool;
		m_jobThreadPool = HK_NULL;
	}

	m_world->markForWrite();
	m_world->removeReference();
	m_world = HK_NULL;
	
	m_physicsData->removeReference();
	m_loadedData->removeReference();
}

WorldSnapshotWithContactPointsDemo::~WorldSnapshotWithContactPointsDemo()
{
	hkError::getInstance().setEnabled(0xf03243ed, true);
	cleanupWorld();
}

void WorldSnapshotWithContactPointsDemo::makeFakeInput()
{
	// frame 10 - save XML
	// frame 20 - load
	// frame 30 - save binary
	// frame 40 - load
	switch (m_physicsStepCounter)
	{
	case 10:
		{
			m_env->m_gamePad->forceButtonPressed(HKG_PAD_BUTTON_3);
			break;
		}
	case 20:
		{
			m_env->m_gamePad->forceButtonPressed(HKG_PAD_BUTTON_1);
			break;
		}
	case 30:
		{
			m_env->m_gamePad->forceButtonPressed(HKG_PAD_BUTTON_2);
			break;
		}
	case 40:
		{
			m_env->m_gamePad->forceButtonPressed(HKG_PAD_BUTTON_1);
			break;
		}
	default:
		{ /* do nothing */ }
	}
}


hkDemo::Result WorldSnapshotWithContactPointsDemo::stepDemo()
{
	Result r = hkDefaultPhysicsDemo::stepDemo();
	if( r != DEMO_OK )
	{
		return r;
	}

	// The first button in the pad will load the last saved scene
	if( m_env->m_gamePad->wasButtonPressed( HKG_PAD_BUTTON_1 ) )
	{
		hkString filename = hkAssetManagementUtil::getFilePath( SCRATCH_FILE );
		hkIstream in(filename.cString());
		if( in.isOk() )
		{
			cleanupWorld();
			cleanupGraphics();

			hkpWorld::IgnoreForceMultithreadedSimulation ignoreForceMultithreaded;
			m_world = loadWorld(filename.cString(), &m_physicsData, &m_loadedData);

			m_debugViewerNames.pushBack( hkpActiveContactPointViewer::getName() );
			m_debugViewerNames.pushBack( hkpInactiveContactPointViewer::getName() );

			setupGraphics();
		}
		else
		{
			m_env->m_textDisplay->outputText("Please save before attempting to load", 20U, m_env->m_window->getHeight()/2, 0xffffffffU, 60);
		}
	}
	else if (m_env->m_gamePad->wasButtonPressed (HKG_PAD_BUTTON_2) )
	{
		// The second button in the pad will save the current scene
		saveWorld(m_world, SCRATCH_FILE, true);
	}
	else if (m_env->m_gamePad->wasButtonPressed (HKG_PAD_BUTTON_3) )
	{
		// The second button in the pad will save the current scene
		saveWorld(m_world, SCRATCH_FILE, false);
	}

	m_world->lock();
	m_world->updateCollisionFilterOnWorld( HK_UPDATE_FILTER_ON_WORLD_FULL_CHECK, HK_UPDATE_COLLECTION_FILTER_PROCESS_SHAPE_COLLECTIONS);
	m_world->unlock();

	return DEMO_OK;
}


#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
"This demo demonstrates how to use the hkpHavokSnapshot to " \
"load and save an entire world together with contact information." \
"\n\221 Loads a snapshot\n\222 Saves a binary snapshot\n\223 Saves an XML snapshot";

HK_DECLARE_DEMO(WorldSnapshotWithContactPointsDemo, HK_DEMO_TYPE_OTHER, "Shows how to take a snapshot of a world", helpString);

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
