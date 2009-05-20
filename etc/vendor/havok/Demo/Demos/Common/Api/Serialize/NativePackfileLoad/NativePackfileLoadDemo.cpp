/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/Common/Api/Serialize/NativePackfileLoad/NativePackfileLoadDemo.h>


// Serialize includes
#include <Common/Base/Reflection/hkClass.h>
#include <Common/Base/System/Io/Reader/hkStreamReader.h>
#include <Common/Base/System/Io/IStream/hkIStream.h>
#include <Common/Serialize/Util/hkNativePackfileUtils.h>
#include <Common/Serialize/Util/hkRootLevelContainer.h>
#include <Common/Serialize/Util/hkStructureLayout.h>

#include <Physics/Utilities/Serialize/hkpPhysicsData.h>

// We can optionally version the packfile contents on load by defining PACKFILE_LOAD_WITH_VERSIONING
// This requires linking with hkcompat
//
// IMPORTANT: The main purpose of the hkNativePackfileUtils functions is fast
// loading of native packfiles. The version steps with kNativePackfileUtils functions
// is slower than using hkBinaryPackfileReader::loadEntireFileInplace method,
// the updatePackfileDataVersion function clearly shows extra step where
// the original packfile is converted into native packfile (hkpHavokSnapshot::save call).
// However, there are still some benefits using hkNativePackfileUtils
// functions if version steps are required:
// - use less memory during packfile lifetime;
// - reuse preallocated memory blocks for loading/unloading different packfiles.

#define PACKFILE_LOAD_WITH_VERSIONING

#if defined(PACKFILE_LOAD_WITH_VERSIONING)

	// If this was a standalone application, we would include the list
	// of versions we are compatible with here. 
//	#define HK_COMPAT_FILE <Common/Compat/hkCompatVersions.h>
//	#include <hkcompat/hkCompat_All.cxx>

#	include <Common/Serialize/Packfile/Xml/hkXmlPackfileReader.h>
#	include <Common/Serialize/Packfile/Binary/hkBinaryPackfileReader.h>
#	include <Common/Serialize/Version/hkVersionRegistry.h>
#	include <Common/Serialize/Version/hkVersionUtil.h>
#	include <Physics/Utilities/Serialize/hkpHavokSnapshot.h>
	void updatePackfileDataVersion(hkArray<char>& ioBuf)
	{
		hkArray<char> origBuf;
		origBuf.swap(ioBuf);

		// Load the entire file inplace
		hkBinaryPackfileReader reader;
		reader.loadEntireFileInplace(origBuf.begin(), origBuf.getSize());

		// check versioning
		if( hkVersionUtil::updateToCurrentVersion( reader, hkVersionRegistry::getInstance() ) != HK_SUCCESS )
		{
			HK_WARN_ALWAYS(0, "Couldn't update version, skipping.\n");
		}
		// Get the top level object in the file
		hkRootLevelContainer* container = static_cast<hkRootLevelContainer*>( reader.getContents( hkRootLevelContainerClass.getName() ) );
		HK_ASSERT2(0xa6451543, container != HK_NULL, "Could not load root level obejct" );

		hkOstream nativeFile(ioBuf);
		hkBool success = hkpHavokSnapshot::save(container, hkRootLevelContainerClass, nativeFile.getStreamWriter(), true);
		HK_ASSERT(0xa6451545, success == true);
	}
#else

	// We don't need to link with hkcompat.
	// Also if you don't use the hkLoader, hkXmlPackfileReader, hkBinaryPackfileReader
	// classes, they may be stripped by the linker.

#endif

void loadEntireFileIntoBuffer(const char* filepath, hkArray<char>& outBuf)
{
	// Load the entire file
	// Open a stream to read the file
	hkIstream infile( filepath );
	HK_ASSERT( 0x215d080c, infile.isOk() );

	if( infile.getStreamReader()->seekTellSupported() )
	{
		infile.getStreamReader()->seek(0, hkStreamReader::STREAM_END);
		outBuf.reserve( infile.getStreamReader()->tell() );
		infile.getStreamReader()->seek(0, hkStreamReader::STREAM_SET);
	}

	// read entire file into local buffer
	int nread = 1;
	while( nread )
	{
		const int CSIZE = 8192;
		char* b = outBuf.expandBy( CSIZE ); // outBuf.reserve( outBuf.getSize() + CSIZE ); b = outBuf.begin() + outBuf.getSize();
		nread = infile.read( b, CSIZE );
		outBuf.setSize( outBuf.getSize() + nread - CSIZE );
	}
}

// We have a different binary file depending on the compiler and platform
static inline void NativePackfileLoadDemo_getBinaryFileName(hkString& e)
{
	e.printf("/simple_L%d%d%d%d.hkx", 
		hkStructureLayout::HostLayoutRules.m_bytesInPointer,
		hkStructureLayout::HostLayoutRules.m_littleEndian? 1 : 0,
		hkStructureLayout::HostLayoutRules.m_reusePaddingOptimization? 1 : 0,
		hkStructureLayout::HostLayoutRules.m_emptyBaseClassOptimization? 1 : 0);
}

NativePackfileLoadDemo::NativePackfileLoadDemo( hkDemoEnvironment* env) 
	: hkDefaultPhysicsDemo(env)
{
	// Disable warning: 'm_contactRestingVelocity not set, setting it to REAL_MAX, so that the new collision restitution code will be disabled'
	hkError::getInstance().setEnabled(0xf03243ed, false);
	hkError::getInstance().setEnabled(0x9fe65234, false);
	
	// Setup the camera
	{
		hkVector4 from(31, 31, 14);
		hkVector4 to(2.5f, -2, -.3f);
		hkVector4 up(0, 0, 1);
		setupDefaultCameras( env, from, to, up );
	}

	hkString path("Resources/Common/Api/Serialize/SimpleLoad");
	hkString fileName;
	NativePackfileLoadDemo_getBinaryFileName(fileName);

	hkRootLevelContainer* container = HK_NULL;

	{
		// read entire file into local buffer
		hkArray<char> buf;
		loadEntireFileIntoBuffer( hkString(path+fileName).cString(), buf );

#if defined(PACKFILE_LOAD_WITH_VERSIONING)
		updatePackfileDataVersion( buf );
#endif
		// Now we have native packfile data in the 'buf' variable.

		// extract only valuable data, stripping out all extras, such as headers and fixups tables
		// and load it into m_dataBuffer
		int bufferSize = hkNativePackfileUtils::getRequiredBufferSize( buf.begin(), buf.getSize() );
		m_dataBuffer.reserveExactly(bufferSize);

		// Load the buffer and get the top level object in the file in one step
		container = static_cast<hkRootLevelContainer*>(hkNativePackfileUtils::load(buf.begin(), buf.getSize(), m_dataBuffer.begin(), m_dataBuffer.getCapacity()));
	}
	HK_ASSERT2(0xa6451543, container != HK_NULL, "Could not load root level obejct" );
	hkpPhysicsData* physicsData = static_cast<hkpPhysicsData*>( container->findObjectByType( hkpPhysicsDataClass.getName() ) );
	HK_ASSERT2(0xa6451544, physicsData != HK_NULL, "Could not find physics data in root level object" );
	HK_ASSERT2(0xa6451535, physicsData->getWorldCinfo() != HK_NULL, "No physics cinfo in loaded file - cannot create a hkpWorld" );

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

NativePackfileLoadDemo::~NativePackfileLoadDemo()
{
	// Re-enable warning
	hkError::getInstance().setEnabled(0xf03243ed, true);
	hkError::getInstance().setEnabled(0x9fe65234, true);

	m_world->markForWrite();
	m_world->removeReference();
	m_world = HK_NULL;

	hkNativePackfileUtils::unload(m_dataBuffer.begin(), m_dataBuffer.getCapacity());
}

hkDemo::Result NativePackfileLoadDemo::stepDemo()
{
	return hkDefaultPhysicsDemo::stepDemo();
}

#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
"This demo demonstrates how to load a native physics file.";

HK_DECLARE_DEMO_VARIANT(NativePackfileLoadDemo, HK_DEMO_TYPE_PRIME | HK_DEMO_TYPE_SERIALIZE, "Native Binary",  0 , "Shows how to load a native binary physics file", helpString);

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
