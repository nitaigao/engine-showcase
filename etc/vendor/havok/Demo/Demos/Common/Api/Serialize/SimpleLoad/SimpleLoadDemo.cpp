/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/Common/Api/Serialize/SimpleLoad/SimpleLoadDemo.h>


// Serialize includes
#include <Common/Base/System/Io/IStream/hkIStream.h>
#include <Common/Base/Reflection/hkClass.h>
#include <Common/Base/Reflection/Registry/hkTypeInfoRegistry.h>
#include <Common/Serialize/Packfile/hkPackfileReader.h>
#include <Common/Serialize/Util/hkStructureLayout.h>
#include <Common/Serialize/Util/hkRootLevelContainer.h>
#include <Common/Serialize/Util/hkSerializeUtil.h>
#include <Physics/Utilities/Serialize/hkpPhysicsData.h>

// We have a different binary file depending on the compiler and platform
static inline void SimpleLoadDemo_getBinaryFileName(hkString& e)
{
	e.printf("/simple_L%d%d%d%d.hkx", 
		hkStructureLayout::HostLayoutRules.m_bytesInPointer,
		hkStructureLayout::HostLayoutRules.m_littleEndian? 1 : 0,
		hkStructureLayout::HostLayoutRules.m_reusePaddingOptimization? 1 : 0,
		hkStructureLayout::HostLayoutRules.m_emptyBaseClassOptimization? 1 : 0);
}


SimpleLoadDemo::SimpleLoadDemo( hkDemoEnvironment* env) 
:	hkDefaultPhysicsDemo(env),
	m_packfileData(HK_NULL)
{
	// Disable warnings
	hkError::getInstance().setEnabled(0xf03243ed, false); // 'm_contactRestingVelocity not set, setting it to REAL_MAX, so that the new collision restitution code will be disabled'
	hkError::getInstance().setEnabled(0x9fe65234, false);  // 'Unsupported simulation on type, setting to SIMULATION_TYPE_CONTINUOUS. See documentation on world stepping and time management'

	// Build the path
	hkString path("Resources/Common/Api/Serialize/SimpleLoad");
	{
		hkString fileName;

		switch (m_variantId)
		{
			case hkPackfileReader::FORMAT_BINARY:
			{
				SimpleLoadDemo_getBinaryFileName(fileName);
				break;
			}
		
			case hkPackfileReader::FORMAT_XML:
			{
				fileName = "/simple.xml";
				break;
			}

			default:
			{
				HK_ASSERT2(0xdbc0f1bd, 0, "Bad demo variant");
			}
		}

		path += fileName;
	}
	
	// Load the data
	m_packfileData = static_cast<hkPackfileData*>(hkSerializeUtil::load(path.cString()));
	
	// Get the top level object in the file, which we know is a hkRootLevelContainer
	hkRootLevelContainer* container = m_packfileData->getContents<hkRootLevelContainer>();
	HK_ASSERT2(0xa6451543, container != HK_NULL, "Could not load root level obejct" );
	
	// Get the physics data
	hkpPhysicsData* physicsData = static_cast<hkpPhysicsData*>( container->findObjectByType( hkpPhysicsDataClass.getName() ) );
	HK_ASSERT2(0xa6451544, physicsData != HK_NULL, "Could not find physics data in root level object" );
	HK_ASSERT2(0xa6451535, physicsData->getWorldCinfo() != HK_NULL, "No physics cinfo in loaded file - cannot create a hkpWorld" );
	
	// Create a world and add the physics systems to it
	{
		m_world = new hkpWorld( *physicsData->getWorldCinfo() );
		m_world->lock();

		// Register all collision agents
		hkpAgentRegisterUtil::registerAllAgents( m_world->getCollisionDispatcher() );

		// Add all the physics systems to the world
		for ( int i = 0; i < physicsData->getPhysicsSystems().getSize(); ++i )
		{
			m_world->addPhysicsSystem( physicsData->getPhysicsSystems()[i] );
		}

		m_world->unlock();
	}

	// Set up graphics
	{
		hkVector4 from(31, 31, 14);
		hkVector4 to(2.5f, -2, -.3f);
		hkVector4 up(0, 0, 1);
		setupDefaultCameras( env, from, to, up );

		// this creates graphics objects for all rigid bodies and phantoms in the world
		setupGraphics();
	}
}

SimpleLoadDemo::~SimpleLoadDemo()
{
	// Re-enable warnings
	hkError::getInstance().setEnabled(0xf03243ed, true);
	hkError::getInstance().setEnabled(0x9fe65234, true);

	m_world->markForWrite();
	m_world->removeReference();
	m_world = HK_NULL;

	m_packfileData->removeReference();
	m_packfileData = HK_NULL;
}

hkDemo::Result SimpleLoadDemo::stepDemo()
{
	return hkDefaultPhysicsDemo::stepDemo();
}


#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
"This demo demonstrates how to load a simple physics file.";

enum
{
	BINARY	= hkPackfileReader::FORMAT_BINARY,
	XML		= hkPackfileReader::FORMAT_XML
};

HK_DECLARE_DEMO_VARIANT(SimpleLoadDemo, HK_DEMO_TYPE_PRIME | HK_DEMO_TYPE_SERIALIZE | HK_DEMO_TYPE_CRITICAL, "Binary",	BINARY ,	"Shows how to load a simple binary physics file", helpString);
HK_DECLARE_DEMO_VARIANT(SimpleLoadDemo, HK_DEMO_TYPE_PRIME | HK_DEMO_TYPE_SERIALIZE | HK_DEMO_TYPE_CRITICAL, "XML",		XML,	"Shows how to load a simple xml physics file", helpString);

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
