/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/Common/Api/Serialize/PackfileImportExport/PackfileImportExport.h>

#include <Common/Base/Reflection/hkClass.h>
#include <Common/Base/System/Io/IStream/hkIStream.h>
#include <Common/Base/System/Io/OStream/hkOStream.h>
#include <Common/Serialize/Util/hkPointerMultiMap.h>
#include <Common/Serialize/Packfile/Xml/hkXmlPackfileReader.h>
#include <Common/Serialize/Packfile/Xml/hkXmlPackfileWriter.h>
#include <Common/Serialize/Packfile/Binary/hkBinaryPackfileWriter.h>
#include <Common/Serialize/Packfile/Binary/hkBinaryPackfileReader.h>
#include <Common/Serialize/Util/hkRootLevelContainer.h>
#include <Common/Serialize/Version/hkVersionRegistry.h>
#include <Common/Serialize/Version/hkVersionUtil.h>
#include <Physics/Utilities/Serialize/hkpPhysicsData.h>
#include <Common/Base/Reflection/Registry/hkVtableClassRegistry.h>
#include <Common/Serialize/Util/hkBuiltinTypeRegistry.h>
#include <Common/Serialize/Util/hkChainedClassNameRegistry.h>


//extern "C" int printf(...);

DEMO_OPTIONS_DEFINE(PackfileImportExport,Options);

static hkResult loadFile
	(
	const char* name,
	ResourceLinker& linker,
	hkpPhysicsData** physicsOut
	)
{
	*physicsOut = HK_NULL;

	hkXmlPackfileReader reader;
	hkIstream infile( name );
	if( infile.isOk() )
	{
		reader.loadEntireFile(infile.getStreamReader());
		hkVersionUtil::updateToCurrentVersion( reader, hkVersionRegistry::getInstance() );

		if( hkRootLevelContainer* container = static_cast<hkRootLevelContainer*>(
					reader.getContents( "hkRootLevelContainer" ) ) )
		{
			hkPackfileData* data = reader.getAllocatedData();
			data->setName(name);
			linker.add( data );
			*physicsOut = static_cast<hkpPhysicsData*>( container->findObjectByType("hkpPhysicsData") );
			return HK_SUCCESS;
		}
	}
	return HK_FAILURE;
}

PackfileImportExport::PackfileImportExport( hkDemoEnvironment* env) 
	: hkDefaultPhysicsDemo(env) 
{
	// Disable warnings
	hkError::getInstance().setEnabled(0x9fe65234, false);

	// Setup the camera
	{
		hkVector4 from(31, 31, 14);
		hkVector4 to(2.5f, -2, -.3f);
		hkVector4 up(0, 0, 1);
		setupDefaultCameras( env, from, to, up );
	}

	if( HK_SUCCESS == loadFile(
			"Resources/Common/Api/Serialize/SimpleLoad/simple.xml",
			m_linker,
			&m_physicsData ) )
	{
		setup();
	}
}

PackfileImportExport::~PackfileImportExport()
{
	hkError::getInstance().setEnabled(0x9fe65234, true);
	//printf("\n\nCLEANUP\n");
	cleanup();
}

void PackfileImportExport::cleanup()
{
	if( m_world )
	{
		m_world->markForWrite();
		const hkArray<hkpPhysicsSystem*>& s = m_physicsData->getPhysicsSystems();
		for( int i = 0; i < s.getSize(); ++i )
		{
			m_world->removePhysicsSystem(s[i]);
		}
		m_world->removeReference();
		m_world = HK_NULL;
	}
	m_linker.reset();
	cleanupGraphics();
}

void PackfileImportExport::setup()
{
	m_world = new hkpWorld( *m_physicsData->getWorldCinfo() );
	m_world->lock();
	hkpAgentRegisterUtil::registerAllAgents( m_world->getCollisionDispatcher() );
	for ( int i = 0; i < m_physicsData->getPhysicsSystems().getSize(); ++i )
	{
		m_world->addPhysicsSystem( m_physicsData->getPhysicsSystems()[i] );
	}
	setupGraphics();
	m_world->unlock();
}

hkDemo::Result PackfileImportExport::stepDemo()
{
	if( m_physicsData == HK_NULL )
	{
		m_error = "Could not load file";
		return DEMO_ERROR;
	}

	if( m_options.m_reload )
	{
		m_options.m_reload = 0;
		exportData();
	}

	return hkDefaultPhysicsDemo::stepDemo();
}

extern const hkClass hkpShapeClass;
namespace
{
	struct ShapeListener : public hkPackfileWriter::AddObjectListener
	{
		virtual void addObjectCallback( ObjectPointer& objP, ClassPointer& klassP )
		{
			if( hkpShapeClass.isSuperClass(*klassP))
			{
				m_shapes.m_array.pushBack( static_cast<const hkReferencedObject*>(objP) );
				objP = HK_NULL;
				klassP = HK_NULL;
			}
		}
		PackfileImportExport::ReferencedObjectArray m_shapes;
	};
}

static hkPackfileWriter* makeWriter( int bin )
{
	if( bin ) return new hkBinaryPackfileWriter; 
	else return new hkXmlPackfileWriter;
}
static hkPackfileReader* makeReader( int bin )
{
	if( bin ) return new hkBinaryPackfileReader; 
	else return new hkXmlPackfileReader;
}

void PackfileImportExport::exportData()
{
	hkPackfileWriter::Options options;

	ShapeListener shapeListener;
	hkArray<char> names;

	//
	// Write to two temporary files
	//

	const char* filenames[2][2] = {{"bodies.xml", "bodies.bin"}, {"shapes.xml", "shapes.bin"}};
	{
		hkPackfileWriter* writer = makeWriter( m_options.m_bodiesFormat );
		writer->setContents( m_physicsData, hkpPhysicsDataClass, &shapeListener );
		hkArray<const hkReferencedObject*>& array = shapeListener.m_shapes.m_array;
		names.reserve(array.getSize()*10);
		for( int i = 0; i < array.getSize(); ++i )
		{
			char* name = names.begin()+10*i;
			hkString::snprintf(name, 10, "shape_%0i", i);
			writer->addImport( array[i], name);
		}

		hkOstream out(filenames[0][m_options.m_bodiesFormat]);
		writer->save( out.getStreamWriter(), options );
		writer->removeReference();
	}

	{
		hkPackfileWriter* writer = makeWriter( m_options.m_shapesFormat );
		writer->setContents( &shapeListener.m_shapes, PackfileImportExportReferencedObjectArrayClass );
		hkArray<const hkReferencedObject*>& array = shapeListener.m_shapes.m_array;
		for( int i = 0; i < array.getSize(); ++i )
		{
			char* name = names.begin()+10*i;
			writer->addExport( array[i], name);
		}
		hkOstream out(filenames[1][m_options.m_shapesFormat]);
		writer->save( out.getStreamWriter(), options );
		writer->removeReference();
	}

	//
	// Destroy world, m_physicsData etc.
	//

	cleanup();

	//
	// Reload
	//

	{
		int formats[2];
		int bodiesFirst = m_options.m_loadOrder;
		formats[bodiesFirst^1] = m_options.m_bodiesFormat;
		formats[bodiesFirst  ] = m_options.m_shapesFormat;
		const char* filename[2];
		filename[bodiesFirst^1] = filenames[0][m_options.m_bodiesFormat];
		filename[bodiesFirst  ] = filenames[1][m_options.m_shapesFormat];

		for( int fileIndex = 0; fileIndex < 2; ++fileIndex )
		{
			hkIstream instream(filename[fileIndex]);
			hkPackfileReader* reader = makeReader( formats[fileIndex] );
			reader->loadEntireFile(instream.getStreamReader());
			reader->getPackfileData()->setName(filename[fileIndex]);
			m_linker.add( reader->getPackfileData() );

			if( fileIndex != bodiesFirst )
			{
				m_physicsData = (hkpPhysicsData*)reader->getContents("hkpPhysicsData");
			}
			else
			{
				/*void* unused = */ reader->getContents("PackfileImportExportReferencedObjectArray");
			}
			reader->removeReference();
		}
		
		HK_ASSERT(0, m_linker.m_dangling.getSize() == 0 );
	}

	//
	// Create new world from loaded physicsdata.
	//

	setup();
}

#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

HK_DECLARE_DEMO(PackfileImportExport, HK_DEMO_TYPE_PRIME | HK_DEMO_TYPE_SERIALIZE, "Shows symbol import and export between packfiles", 
	"Bring up the tweak menu to change the file format used and the order of loading.\nDebug logging is printed to the console");

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
