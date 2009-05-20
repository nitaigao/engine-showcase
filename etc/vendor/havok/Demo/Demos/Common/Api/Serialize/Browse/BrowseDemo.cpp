/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/Common/Api/Serialize/Browse/BrowseDemo.h>

#include <Common/Serialize/Packfile/Binary/hkBinaryPackfileReader.h>
#include <Common/Serialize/Packfile/Xml/hkXmlPackfileReader.h>
#include <Common/Serialize/Util/hkStructureLayout.h>
#include <Common/Serialize/Util/hkRootLevelContainer.h>
#include <Common/Base/System/Io/IStream/hkIStream.h>
#include <Physics/Utilities/Serialize/hkpPhysicsData.h>
#include <Common/Base/System/Io/FileSystem/hkFileSystem.h>

// We can optionally version the packfile contents on load
// This requires linking with hkcompat
#define SIMPLE_LOAD_WITH_VERSIONING
#include <Common/Serialize/Version/hkVersionUtil.h>
#include <Graphics/Common/Font/hkgFont.h>
#include <Common/Serialize/Packfile/Binary/hkPackfileHeader.h>
#include <Physics/Utilities/Serialize/hkpHavokSnapshot.h>

#define START_HORIZONTAL (20+m_env->m_window->getTVDeadZoneH())
#define START_VERTICAL (20+m_env->m_window->getTVDeadZoneV())

#if defined(SIMPLE_LOAD_WITH_VERSIONING)
#	include <Common/Serialize/Version/hkVersionRegistry.h>
	void versionContents(hkPackfileReader* reader)
	{
		if( hkVersionUtil::updateToCurrentVersion( *reader, hkVersionRegistry::getInstance() ) != HK_SUCCESS )
		{
			HK_WARN_ALWAYS(0, "Couldn't update version, skipping.\n");
		}
	}
#else
	void maybeVersionContents(hkPackfileReader* reader)
	{
		if( hkString::strCmp( reader->getContentsVersion(), hkVersionUtil::getCurrentVersion() ) != 0 )
		{
			HK_WARN_ALWAYS(0, "Versioning is disabled, but file is from an older version.\n");
		}
	}
#endif

// We have a different binary file depending on the compiler and platform
static inline void BrowseDemo_getBinaryFileName(hkString& e)
{
	e.printf("%d%d%d%d.hkx", 
		hkStructureLayout::HostLayoutRules.m_bytesInPointer,
		hkStructureLayout::HostLayoutRules.m_littleEndian? 1 : 0,
		hkStructureLayout::HostLayoutRules.m_reusePaddingOptimization? 1 : 0,
		hkStructureLayout::HostLayoutRules.m_emptyBaseClassOptimization? 1 : 0);
} 

BrowseDemo::BrowseDemo( hkDemoEnvironment* env) 
:	hkDefaultPhysicsDemo(env)
,	m_packfileData(HK_NULL)
,	m_contents(HK_NULL)
,	m_running(false)
{
	// Disable warning: 'm_contactRestingVelocity not set, setting it to REAL_MAX, so that the new collision restitution code will be disabled'
	hkError::getInstance().setEnabled(0xf03243ed, false);
	
	// Setup the camera
	{
		hkVector4 from(31, 31, 14);
		hkVector4 to(2.5f, -2, -.3f);
		hkVector4 up(0, 0, 1);
		setupDefaultCameras( env, from, to, up );
	}
}

BrowseDemo::~BrowseDemo()
{
	// Re-enable warning
	hkError::getInstance().setEnabled(0xf03243ed, true);

	if(m_world != HK_NULL)
	{
		m_world->markForWrite();
		m_world->removeReference();
		m_world = HK_NULL;
	}
	if(m_packfileData != HK_NULL)
	{
		m_packfileData->removeReference();
	}
}

hkResult BrowseDemo::readAndSetupPackfile(const char* filename)
{
	hkIstream infile( filename );
	if( infile.isOk() == false )
	{
		return HK_FAILURE;
	}

	// create thew suitable reader
	hkPackfileReader* reader = HK_NULL;
	switch( hkPackfileReader::detectFormat(infile.getStreamReader()) )
	{
		case hkPackfileReader::FORMAT_BINARY:
			reader = new hkBinaryPackfileReader();
			break;
		case hkPackfileReader::FORMAT_XML:
			reader = new hkXmlPackfileReader();
			break;
		default:
			return HK_FAILURE;
	}

	// Load the entire file
	hkResult res = reader->loadEntireFile( infile.getStreamReader() );
	if( res == HK_SUCCESS )
	{
		versionContents(reader);

		// Get a handle to the memory allocated by the reader, and add a reference to it.
		// This allows us to delete the reader.
		m_packfileData = reader->getPackfileData();
		m_packfileData->addReference();

		// Get the top level object in the file
		m_contents = static_cast<hkRootLevelContainer*>( reader->getContents( hkRootLevelContainerClass.getName() ) );
		HK_ASSERT2(0xa6451543, m_contents != HK_NULL, "Could not load root level obejct" );

		if( hkpPhysicsData* physicsData = static_cast<hkpPhysicsData*>( m_contents->findObjectByType( hkpPhysicsDataClass.getName() ) ) )
		{
			//HK_ASSERT2(0xa6451544, physicsData != HK_NULL, "Could not find physics data in root level object" );
			//HK_ASSERT2(0xa6451535, physicsData->getWorldCinfo() != HK_NULL, "No physics cinfo in loaded file - cannot create a hkWorld" );
			if( physicsData->getWorldCinfo() )
			{
				// Create a world and add the physics systems to it
				m_world = new hkpWorld( *physicsData->getWorldCinfo() );
				m_world->lock();

				// Register all collision agents
				hkpAgentRegisterUtil::registerAllAgents( m_world->getCollisionDispatcher() );

				// Add all the physics systems to the world
				for ( int i = 0; i < physicsData->getPhysicsSystems().getSize(); ++i )
				{
					m_world->addPhysicsSystem( physicsData->getPhysicsSystems()[i] );
				}

				// Setup graphics - this creates graphics objects for all rigid bodies and phantoms in the world
				setupGraphics();
				m_world->unlock();
			}
		}
	}

	// We can now delete the reader. Note we still keep a reference to the allocated data.
	reader->removeReference(); // same as delete since we have the only reference

	return res;
}


hkDemo::Result BrowseDemo::stepDemo()
{
	// the file list must be displayed only if we have the directory contents and 
	// we have not loaded a file yet
	if( m_contents == HK_NULL )
	{
		if(m_fileBrowser.parseAndDisplayDirectoryAndFiles( m_env, 20, 20, true ))
		{
			hkString fname = m_fileBrowser.m_curDir + "/" + m_fileBrowser.m_listing.getEntries()[m_fileBrowser.m_curIndex].name;
			if( readAndSetupPackfile( fname.cString() ) == HK_SUCCESS )
			{
				// this is necessary because the first time it was called the world was NULL
				// and setup was not completed
				postConstruct(); 
				m_fileBrowser.m_curDir = fname; // abuse m_curDir and store loaded filename
			}
		}
		
	}
	else
	{
		int startH = START_HORIZONTAL; 
		int startV = START_VERTICAL; 

		hkString s; s.printf("%s loaded%s\nPress \220 to %spause\n\222 or 'X' for an XML snapshot\n\223 or 'B' for a binary snapshot",
			m_fileBrowser.m_curDir.cString(), m_world ? "" : " (no physics found)", m_running ? "" : "un" );
		m_env->m_textDisplay->outputText(s, startH, startV, 0xffffffff);
		startV += 4*20;
		const hkgPad& pad = *m_env->m_gamePad;
		const hkgKeyboard& key = m_env->m_window->getKeyboard();
		if( pad.wasButtonPressed( HKG_PAD_BUTTON_0 ) )
		{
			m_running = !m_running;
		}
		hkBool32 useBinary = -1;
		if( pad.wasButtonPressed( HKG_PAD_BUTTON_2 ) || key.wasKeyPressed('X') )
		{
			useBinary = 0;
		}
		else if( pad.wasButtonPressed( HKG_PAD_BUTTON_3 ) || key.wasKeyPressed('B') )
		{
			useBinary = 1;
		}

		if( useBinary != -1 )
		{
			hkString fname = m_fileBrowser.m_curDir + "_SNAP.hkx";
			hkOstream os( fname.cString() );
			hkpHavokSnapshot::save( m_contents, hkRootLevelContainerClass, os.getStreamWriter(), useBinary == 1 );
			s.printf("Wrote to %s", fname.cString() );
			m_env->m_textDisplay->outputText(s, startH, startV, 0xffffffff, 120 );
		}
	}
	if( m_running )
	{
		return hkDefaultPhysicsDemo::stepDemo();
	}
	return DEMO_OK;
}

#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
"This demo loads havok files and can reexport as xml or binary.\n" \
"Use arrow keys and \220 to start";

HK_DECLARE_DEMO(BrowseDemo, HK_DEMO_TYPE_TEST, helpString, "");

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
