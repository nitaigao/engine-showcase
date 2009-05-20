/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/Physics/Api/Dynamics/World/WorldSnapshot/WorldSnapshotDemo.h>




// Serialize stuff
#include <Common/Base/Reflection/Registry/hkTypeInfoRegistry.h>
#include <Common/Base/Reflection/Registry/hkVtableClassRegistry.h>
#include <Physics/Utilities/Serialize/hkpHavokSnapshot.h>
#include <Physics/Utilities/Serialize/hkpPhysicsData.h>
#include <Common/Base/System/Io/IStream/hkIStream.h>
#include <Physics/Dynamics/Phantom/hkpPhantom.h>

#include <Graphics/Common/Input/Pad/hkgPad.h>

static const char SCRATCH_FILE[] = "Resources/Physics/worldsnapshot.hkx";

static hkpWorld* loadWorld( const char* path, hkpPhysicsData** physicsData, hkPackfileReader::AllocatedData** memData )
{
	hkIstream infile( path );
	HK_ASSERT( 0x215d080c, infile.isOk() );
	*physicsData = hkpHavokSnapshot::load(infile.getStreamReader(), memData);

	// Ensure non-multithreaded simulation for non-multithreaded platforms
#if !defined(HK_PLATFORM_MULTI_THREAD) || (HK_CONFIG_THREAD == HK_CONFIG_SINGLE_THREADED)
	(*physicsData)->getWorldCinfo()->m_simulationType = hkpWorldCinfo::SIMULATION_TYPE_CONTINUOUS;
#endif

	hkpWorld* w = (*physicsData)->createWorld();

	return w;
}

static void saveWorld( hkpWorld* world, const char* path, bool binary )
{
	hkOstream outfile( path );
	HK_ON_DEBUG( hkBool res = ) hkpHavokSnapshot::save(world, outfile.getStreamWriter(), binary);
	HK_ASSERT( 0x215d080d, res );
}

void WorldSnapshotDemo::init(hkDemoEnvironment* env, const char* filename)
{
	hkpWorld::IgnoreForceMultithreadedSimulation ignoreForceMultithreaded;

	// Disable warning: 'm_contactRestingVelocity not set, setting it to REAL_MAX, so that the new collision restitution code will be disabled'
	hkError::getInstance().setEnabled(0xf03243ed, false);

	// Setup the camera
	{
		hkVector4 from(10, -10, 10);
		hkVector4 to(0, 0, 0);
		hkVector4 up(0, 0, 1);
		setupDefaultCameras( env, from, to, up );
	}

	// Load the startup scene
	{
		// Load the world.
		m_world = loadWorld( filename, &m_physicsData, &m_loadedData );
		m_world->lock();

		// Setup graphics
		setupGraphics();
		m_world->unlock();
	}
}

WorldSnapshotDemo::WorldSnapshotDemo( hkDemoEnvironment* env)
	: hkDefaultPhysicsDemo(env)
{
	hkString filename; // We have a different binary file depending on the compiler and platform
	filename.printf("Resources/Common/Api/Serialize/SimpleLoad/simple_L%d%d%d%d.hkx",
			hkStructureLayout::HostLayoutRules.m_bytesInPointer,
			hkStructureLayout::HostLayoutRules.m_littleEndian? 1 : 0,
			hkStructureLayout::HostLayoutRules.m_reusePaddingOptimization? 1 : 0,
			hkStructureLayout::HostLayoutRules.m_emptyBaseClassOptimization? 1 : 0);
	init(env, filename.cString());
}

WorldSnapshotDemo::WorldSnapshotDemo( hkDemoEnvironment* env, const char* startFile )
	: hkDefaultPhysicsDemo(env)
{
	init(env, startFile);
}

void WorldSnapshotDemo::cleanupWorld()
{
	hkReferencedObject::lockAll();
	m_world->markForWrite();
	m_world->removeReference();
	m_world = HK_NULL;

	m_physicsData->removeReference();
	m_loadedData->removeReference();
	hkReferencedObject::unlockAll();
}

WorldSnapshotDemo::~WorldSnapshotDemo()
{
	hkError::getInstance().setEnabled(0xf03243ed, true);
	cleanupWorld();
}

void WorldSnapshotDemo::makeFakeInput()
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

hkDemo::Result WorldSnapshotDemo::stepDemo()
{
	Result r = hkDefaultPhysicsDemo::stepDemo();
	if( r != DEMO_OK )
	{
		return r;
	}

	// The first button in the pad will load the last saved scene
	if( m_env->m_gamePad->wasButtonPressed( HKG_PAD_BUTTON_1 ) )
	{
		hkIstream in(SCRATCH_FILE);
		if( in.isOk() )
		{
			cleanupWorld();
			cleanupGraphics();

			hkpWorld::IgnoreForceMultithreadedSimulation ignoreForceMultithreaded;
			m_world = loadWorld(SCRATCH_FILE, &m_physicsData, &m_loadedData);

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

	return DEMO_OK;
}


#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
"This demo demonstrates how to use the hkHavokSnapshotSession to" \
"load and save an entire world including rigid bodies, shapes, mopps" \
"constraints and filters" \
"\n\221 Loads a snapshot\n\222 Saves a binary snapshot\n\223 Saves an XML snapshot";

HK_DECLARE_DEMO(WorldSnapshotDemo, HK_DEMO_TYPE_PRIME | HK_DEMO_TYPE_SERIALIZE, "Shows how to take a snapshot of a world", helpString);

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
