/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/DemoCommon/DemoFramework/hkDefaultPhysicsDemo.h>
#include <Physics/Utilities/Serialize/hkpPhysicsData.h>
#include <Common/Serialize/hkSerialize.h>
#include <Common/Serialize/Util/hkLoader.h>
#include <Common/Serialize/Util/hkStructureLayout.h>
#include <Common/Serialize/Util/hkRootLevelContainer.h>
#include <Common/Serialize/Packfile/Binary/hkBinaryPackfileWriter.h>


class TargetPlatformDemo : public hkDefaultPhysicsDemo
{
	public:

		TargetPlatformDemo(hkDemoEnvironment* env);
		~TargetPlatformDemo();

		Result stepDemo();

		hkLoader m_loader;
		hkRootLevelContainer* m_contents;
};

TargetPlatformDemo::TargetPlatformDemo( hkDemoEnvironment* env) 
	: hkDefaultPhysicsDemo(env) 
{
	// Disable warnings:									
	hkError::getInstance().setEnabled(0x9fe65234, false);  // 'Unsupported simulation on type, setting to SIMULATION_TYPE_CONTINUOUS. See documentation on world stepping and time management'

	// Setup the camera
	{
		hkVector4 from(31, 31, 14);
		hkVector4 to(2.5f, -2, -.3f);
		hkVector4 up(0, 0, 1);
		setupDefaultCameras( env, from, to, up );
	}

	// Load a world.

	// For most people, their asset processing machine will be some sort of PC, (4011 or 8011)
	// so it would make sense to read a binary for that platform. However this demo must
	// run on all platforms.
	m_contents = m_loader.load("Resources/Common/Api/Serialize/SimpleLoad/simple.xml");
	HK_ASSERT2(0xa6451543, m_contents != HK_NULL, "Could not load root level obejct" );
	hkpPhysicsData* physicsData = static_cast<hkpPhysicsData*>( m_contents->findObjectByType( hkpPhysicsDataClass.getName() ) );
	HK_ASSERT2(0xa6451544, physicsData != HK_NULL, "Could not find physics data in root level object" );

	// Create a world and add the physics systems to it
	// In a real use we would not create a world, but since it's a demo
	// we'd like to see something happening.
	m_world = new hkpWorld( *physicsData->getWorldCinfo() );
	m_world->lock();

	hkpAgentRegisterUtil::registerAllAgents( m_world->getCollisionDispatcher() );
	for ( int i = 0; i < physicsData->getPhysicsSystems().getSize(); ++i )
	{
		m_world->addPhysicsSystem( physicsData->getPhysicsSystems()[i] );
	}

	// Setup graphics - this creates graphics objects for all rigid bodies and phantoms in the world
	setupGraphics();

	m_world->unlock();
}

TargetPlatformDemo::~TargetPlatformDemo()
{
	// Re-enable warnings:									
	hkError::getInstance().setEnabled(0x9fe65234, false);  

	m_world->markForWrite();
	m_world->removeReference();
	m_world = HK_NULL;
}

hkDemo::Result TargetPlatformDemo::stepDemo()
{
	if( m_env->m_gamePad->wasButtonPressed( HKG_PAD_BUTTON_0 ) )
	{
		// Set the packfile contents. After the contents are set, we can
		// save several times with different options. Here we will save
		// with all possible layout rules.
		hkBinaryPackfileWriter writer;
		writer.setContents( m_contents, hkRootLevelContainerClass );

		for( int i = 0; i < 16; ++i )
		{
			// Usually you would use one of the static presets in hkStructureLayout.
			// Here, we run through all layouts for testing.
			hkStructureLayout::LayoutRules rules;
			rules.m_bytesInPointer = i&8 ? 4 : 8;
			rules.m_littleEndian = i&4 ? 0 : 1;
			rules.m_reusePaddingOptimization = i&2 ? 0 : 1;
			rules.m_emptyBaseClassOptimization = i&1 ? 0 : 1;
			
			// For this demo, write to an discarded in-memory buffer.
			hkArray<char> buf;
			hkOstream out(buf);

			// Set the rules and save
			hkPackfileWriter::Options options;
			options.m_layout = rules;
			writer.save( out.getStreamWriter(), options );
		}
		m_env->m_textDisplay->outputText( "Saved all 16 combinations of targets to memory stream.",
				30, 150, 0xffffffff, 100 );
	}
	return hkDefaultPhysicsDemo::stepDemo();
}

#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

HK_DECLARE_DEMO(TargetPlatformDemo, HK_DEMO_TYPE_PRIME | HK_DEMO_TYPE_SERIALIZE, "Shows how to save binaries for different platforms.",
	"This demo shows how to export binaries for different platform.\n"\
	"It loads a scene and exports it in all possible binary formats.\n" \
	"Press \220 to export to in-memory buffers."	);

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
