/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/Physics/UseCase/AssetStreaming/AssetStreamingDemo.h>

// System includes
#include <Common/Base/System/Io/IStream/hkIStream.h>

// Serialization includes
#include <Common/Serialize/Packfile/Binary/hkBinaryPackfileReader.h>
#include <Common/Serialize/Packfile/Binary/hkBinaryPackfileWriter.h>
#include <Common/Serialize/Util/hkRootLevelContainer.h>
#include <Common/Serialize/Util/hkBuiltinTypeRegistry.h>
#include <Common/Base/Reflection/Registry/hkVtableClassRegistry.h>
#include <Common/Serialize/Version/hkVersionUtil.h>

// Physics includes
#include <Physics/Utilities/Serialize/hkpPhysicsData.h>
#include <Physics/Utilities/Collide/ShapeUtils/ShapeSharing/hkpShapeSharingUtil.h>

// Demo includes
#include <Demos/DemoCommon/Utilities/Asset/hkAssetManagementUtil.h>
#include <Common/Base/Algorithm/PseudoRandom/hkPseudoRandomGenerator.h>
	
AssetStreamingDemo::AssetStreamingDemo( hkDemoEnvironment* env )
:hkDefaultPhysicsDemo( env )
{
	m_frameCounter = 0;

	// Clear section data
	{
		hkString::memSet( m_worldSections, HK_NULL, sizeof(hkpPhysicsData*) * HK_ASSET_STREAMING_DEMO_NUM_WORLD_SECTIONS );
		hkString::memSet( m_worldSectionMemoryBlocks, HK_NULL, sizeof(hkPackfileData*) * HK_ASSET_STREAMING_DEMO_NUM_WORLD_SECTIONS );
	}

	// Pathnames of original world sections	
	{				
		m_originalWorldSectionPathnames.pushBack( hkAssetManagementUtil::getFilePath("Resources/Physics/levels/World_Section_1.hkx") );
		m_originalWorldSectionPathnames.pushBack( hkAssetManagementUtil::getFilePath("Resources/Physics/levels/World_Section_2.hkx") );
		m_originalWorldSectionPathnames.pushBack( hkAssetManagementUtil::getFilePath("Resources/Physics/levels/World_Section_3.hkx") );
		m_originalWorldSectionPathnames.pushBack( hkAssetManagementUtil::getFilePath("Resources/Physics/levels/World_Section_4.hkx") );
		m_originalWorldSectionPathnames.pushBack( hkAssetManagementUtil::getFilePath("Resources/Physics/levels/World_Section_5.hkx") );
		m_originalWorldSectionPathnames.pushBack( hkAssetManagementUtil::getFilePath("Resources/Physics/levels/World_Section_6.hkx") );
	}

	// Pathnames of instanced world sections	
	{
		m_instancedWorldSectionPathnames.pushBack( "Resources/Physics/World_Section_1_Instanced.hkx" );
		m_instancedWorldSectionPathnames.pushBack( "Resources/Physics/World_Section_2_Instanced.hkx" );
		m_instancedWorldSectionPathnames.pushBack( "Resources/Physics/World_Section_3_Instanced.hkx" );
		m_instancedWorldSectionPathnames.pushBack( "Resources/Physics/World_Section_4_Instanced.hkx" );
		m_instancedWorldSectionPathnames.pushBack( "Resources/Physics/World_Section_5_Instanced.hkx" );
		m_instancedWorldSectionPathnames.pushBack( "Resources/Physics/World_Section_6_Instanced.hkx" );
	}

	// Pathname of shape registry
	m_shapeRegistryPathname = "Resources/Physics/ShapeRegistry.hkx";


	// Build shape sharing version of world 
	{
		instanceShapeDataInWorld( m_originalWorldSectionPathnames, m_instancedWorldSectionPathnames, m_shapeRegistryPathname );
	}

	// Initialize the physics world
	{
		hkpWorldCinfo worldCinfo;
		{			
			worldCinfo.m_gravity.set( 0.0f, 0.0f, -10.0f );
			worldCinfo.m_simulationType = hkpWorldCinfo::SIMULATION_TYPE_MULTITHREADED;
			worldCinfo.m_broadPhaseBorderBehaviour = hkpWorldCinfo::BROADPHASE_BORDER_FIX_ENTITY;
		}

		m_world = new hkpWorld( worldCinfo );

		m_world->lock();
		hkpAgentRegisterUtil::registerAllAgents( m_world->getCollisionDispatcher() );
	}

	// Load the shape registry.  This is shared between all world sections.
	{
		// Open the file
		hkIstream infile( m_shapeRegistryPathname );

		// Load the file into a binary packfile reader.  Use a hkXmlPackfileReader here if your data is XML.
		hkBinaryPackfileReader packfileReader;
		packfileReader.loadEntireFile( infile.getStreamReader() );		

		// Even though we don't need this data we still need to call getContents.  Get contents will fix up
		// all pointers in the loaded memory block.  If this is not done virtual functions will not work.
		packfileReader.getContents( hkRootLevelContainerClass.getName() );
		
		// Store a reference to the memory block the contents is stored in.  This block stores the shape exports
		// that will be used to fix up shape pointers 
		m_shapeRegistryMemoryBlock = packfileReader.getAllocatedData();
		m_shapeRegistryMemoryBlock->addReference();	
	}


	// Load the first section
	{
		loadSection( m_instancedWorldSectionPathnames[0], m_worldSections[0], m_worldSectionMemoryBlocks[0], m_shapeRegistryMemoryBlock );
		addSectionToWorld( m_worldSections[0], m_world );
	}
	m_world->unlock();

	// Setup graphics
	{
		setupGraphics();
		
		hkVector4 from( -50.0f, -50.0f, 50.0f );
		hkVector4 to( 0.0f, 0.0f, 0.0f );
		hkVector4 up( 0.0f, 0.0f, 1.0f );
		setupDefaultCameras( m_env, from, to, up );
	}
}

AssetStreamingDemo::~AssetStreamingDemo()
{
	m_world->lock();
	// Unload all world sections that are currently loaded
	for( hkInt32 i = 0; i < HK_ASSET_STREAMING_DEMO_NUM_WORLD_SECTIONS; ++i )
	{
		if( m_worldSections[i] != HK_NULL )
		{
			removeSectionFromWorld( m_worldSections[i], m_world );
			unloadSection( m_worldSections[i], m_worldSectionMemoryBlocks[i] );
		}
	}

	// Unload shape registry
	{
		m_shapeRegistryMemoryBlock->removeReference();
	}
	m_world->unlock();
}

hkDemo::Result AssetStreamingDemo::stepDemo()
{
	// Try to stream one section in and one out every 50 frames
	if( (++m_frameCounter) % 50 == 0 )
	{	
		m_world->lock();

		hkPseudoRandomGenerator generator(m_frameCounter + (int)m_world->getCurrentPsiTime());

		// Randomly pick a section to unload.  If it's not loaded we skip unloading this time.
		{
			hkInt32 sectionToUnloadIndex = (hkInt32)generator.getRandRange( 0, HK_ASSET_STREAMING_DEMO_NUM_WORLD_SECTIONS );

			if( m_worldSections[sectionToUnloadIndex] != HK_NULL )
			{
				removeSectionFromWorld( m_worldSections[sectionToUnloadIndex], m_world );
				unloadSection( m_worldSections[sectionToUnloadIndex], m_worldSectionMemoryBlocks[sectionToUnloadIndex] );
			}
		}

		// Randomly pick a section to load.  If it's loaded we skip loading this time.
		{
			hkInt32 sectionToLoadIndex = (hkInt32)generator.getRandRange( 0, HK_ASSET_STREAMING_DEMO_NUM_WORLD_SECTIONS );

			if( m_worldSections[sectionToLoadIndex] == HK_NULL )
			{
				loadSection( m_instancedWorldSectionPathnames[sectionToLoadIndex], m_worldSections[sectionToLoadIndex], m_worldSectionMemoryBlocks[sectionToLoadIndex], m_shapeRegistryMemoryBlock );
				addSectionToWorld( m_worldSections[sectionToLoadIndex], m_world );				
			}
		}
		m_world->unlock();
	}

	return hkDefaultPhysicsDemo::stepDemo();
}

void AssetStreamingDemo::instanceShapeDataInWorld( const hkObjectArray<hkString>& originalWorldSectionPathnames, const hkArray<const char*>& instancedWorldSectionPathnames, const char* shapeRegistryPathname )
{
	// This array stores the memory blocks created to store packfile data.  These memory blocks must stay in main
	// memory until we are doing working with the contents within them.
	hkArray<hkPackfileData*> loadedMemoryBlocks;

	// First load all physics data that we'll use in this demo.
	hkArray<hkpPhysicsData*> allLoadedPhysicsData;
	{
		loadAllPhysicsData( originalWorldSectionPathnames, allLoadedPhysicsData, loadedMemoryBlocks );
	}

	// Next collect all rigid bodies.
	hkArray<hkpRigidBody*> allRigidBodies;
	{		
		for( hkInt32 i = 0; i < allLoadedPhysicsData.getSize(); ++i )
		{
			hkpPhysicsData* physicsData = allLoadedPhysicsData[i];
			for( hkInt32 j = 0; j < physicsData->getPhysicsSystems().getSize(); ++j )
			{
				hkpPhysicsSystem* physicsSystem = physicsData->getPhysicsSystems()[j];
				for( hkInt32 k = 0; k < physicsSystem->getRigidBodies().getSize(); ++k )
				{					
					allRigidBodies.pushBack( physicsSystem->getRigidBodies()[k] );
				}
			}
		}
	}

	// Next, after we find all rigid bodies, run the hkpShapeSharingUtil on the list of bodies.  This utility 
	// will iterate through all of the bodies and identify which shapes can be shared.   When this method returns, 
	// all shareable hkpShape pointers will be assigned to the rigid bodies in the collection.  
	{
		hkpShapeSharingUtil::Options options;
		{
			options.m_equalityThreshold = 0.001f;
			options.m_detectPermutedComponents = true;
		}

		hkpShapeSharingUtil::Results results;

		hkpShapeSharingUtil::shareShapes( allRigidBodies, options, results );
		
		// Disable reports: 									
		if(m_env->m_reportingLevel >= hkDemoEnvironment::REPORT_INFO )
		{
			results.report();
		}
	}

	// Next collect all hkpShape pointers.  These will be the instanced pointers.  They will be stored in a "Shape Registry" packfile.
	hkArray<const void*> exports;	
	{
		for( hkInt32 i = 0; i < allLoadedPhysicsData.getSize(); ++i )
		{
			hkpPhysicsData* physicsData = allLoadedPhysicsData[i];
			for( hkInt32 j = 0; j < physicsData->getPhysicsSystems().getSize(); ++j )
			{
				hkpPhysicsSystem* physicsSystem = physicsData->getPhysicsSystems()[j];
				for( hkInt32 k = 0; k < physicsSystem->getRigidBodies().getSize(); ++k )
				{
					hkpRigidBody* rigidBody = physicsSystem->getRigidBodies()[k];
					const hkpShape* shape = rigidBody->getCollidable()->getShape();
					
					if( exports.indexOf( shape ) == -1 )
					{
						exports.pushBack( shape );
					}					
				}
			}
		}
	}	
	
	// Finally re-serialize the world section packfiles and the shape registry packfile
	{
		saveAllPhysicsDataWithExports( instancedWorldSectionPathnames, shapeRegistryPathname, allLoadedPhysicsData, exports );
	}

	// Deallocate all loaded packfile data
	{
		for( hkInt32 i = 0; i < loadedMemoryBlocks.getSize(); ++i )
		{			
			// Disable destructors for all packfile data.  Need to do this because the hkpShapeSharingUtil will move pointers in
			// in one packfile to another.  Calling destructors twice on one of these objects will corrupt memory.
			loadedMemoryBlocks[i]->disableDestructors();
			loadedMemoryBlocks[i]->removeReference();	
		}
	}
}

void AssetStreamingDemo::loadAllPhysicsData( const hkObjectArray<hkString>& allWorldSectionPathnames, hkArray<hkpPhysicsData*>& allLoadedPhysicsData, hkArray<hkPackfileData*>& allLoadedMemoryBlocks )
{
	// Load all world sections.  Store the loaded memory blocks in allLoadedMemoryBlocks.  Store the physics data in allLoadedPhysicsData.	
	for( hkInt32 i = 0; i < allWorldSectionPathnames.getSize(); ++i )
	{
		// Open the file
		hkIstream infile( allWorldSectionPathnames[i].cString() );

		// Load the file into a binary packfile reader.  Use a hkXmlPackfileReader here if your data is XML.
		hkBinaryPackfileReader packfileReader;
		packfileReader.loadEntireFile( infile.getStreamReader() );

		// Version contents
		hkVersionUtil::updateToCurrentVersion( packfileReader, hkVersionRegistry::getInstance() );

		// Get the contents of the binary packfile.
		hkRootLevelContainer* packfileContents = static_cast<hkRootLevelContainer*>(packfileReader.getContents( hkRootLevelContainerClass.getName() ));

		// Store a reference to the physics data in the packfile contents container
		hkpPhysicsData* physicsData = static_cast<hkpPhysicsData*>(packfileContents->findObjectByType( hkpPhysicsDataClass.getName() ));
		allLoadedPhysicsData.pushBack( physicsData );

		// Store a reference to the memory block the contents is stored in
		hkPackfileData* loadedMemoryBlock = packfileReader.getAllocatedData();
		loadedMemoryBlock->addReference();
		allLoadedMemoryBlocks.pushBack(loadedMemoryBlock);				
	}	
}

	// A special packfile listener that listens for object pointer writes.  This listener
	// is called when an object is about to be added to a packfile.  If an object is in the
	// exports list provided its value is set to HK_NULL.  These exports will be serialized
	// later.
class ExportListener : public hkPackfileWriter::AddObjectListener
{
public:
		/// Initializes the listener with a list of known exports.
	ExportListener( const hkArray<const void*>& exports );

		/// Callback raised for every pointer object serialized to a packfile
	virtual void addObjectCallback( ObjectPointer& obj, ClassPointer& klass );

		/// Returns an array of indices into the list of known exports that were encountered during serialization.
	const hkArray<hkInt32>& getEncounteredExports() const;

protected:
		/// List of export pointers to mark as HK_NULL if encountered
	const hkArray<const void*>& m_exports;		
		/// List of indices into ::m_exports of pointers encountered
	hkArray<hkInt32> m_encounteredExports;
};

ExportListener::ExportListener( const hkArray<const void*>& exports )
: m_exports( exports )
{	
}

void ExportListener::addObjectCallback( ObjectPointer& obj, ClassPointer& klass )
{
		// Note this is a slow linear search
	hkInt32 exportIndex = m_exports.indexOf( obj );

	if( exportIndex != -1 )
	{
		m_encounteredExports.pushBack( exportIndex );
		obj = HK_NULL;
		klass = HK_NULL;
	}
}

const hkArray<hkInt32>& ExportListener::getEncounteredExports() const
{
	return m_encounteredExports;
}


void AssetStreamingDemo::saveAllPhysicsDataWithExports( const hkArray<const char*>& allWorldSectionPathnames, const char* exportsPathname, const hkArray<hkpPhysicsData*>& allLoadedPhysicsData, const hkArray<const void*>& exports )
{		
	// Name all exports
	hkObjectArray<hkString> exportNames;
	{
		hkString* names = exportNames.expandBy(exports.getSize());

		for( hkInt32 i = 0; i < exports.getSize(); ++i )
		{
			names[i].printf( "Export-%d", i );
		}
	}

	// Serialize all world sections
	for( hkInt32 i = 0; i < allWorldSectionPathnames.getSize(); ++i )
	{		
		// Open the new file
		hkOstream outfile( allWorldSectionPathnames[i] );

		// Create a new ExportListener
		ExportListener exportListener(exports);

		// Serialize the data
		hkBinaryPackfileWriter packfileWriter;
		packfileWriter.setContents( allLoadedPhysicsData[i], hkpPhysicsDataClass, &exportListener );

		// Add imports
		for( hkInt32 j = 0; j < exportListener.getEncounteredExports().getSize(); ++j )
		{
			hkInt32 exportIndex = exportListener.getEncounteredExports()[j];
			packfileWriter.addImport( exports[exportIndex], exportNames[exportIndex].cString() );
		}

		// Save the serialized data
		hkBinaryPackfileWriter::Options options;
		packfileWriter.save( outfile.getStreamWriter(), options );
	}


	// Serialize all exports (this is the shape registry in this demo)
	{
		// Open the new file
		hkOstream outfile( exportsPathname );

		// Create a root level container with all exports
		hkRootLevelContainer exportsContainer;
		{
			exportsContainer.m_namedVariants = new hkRootLevelContainer::NamedVariant[exports.getSize()];
			exportsContainer.m_numNamedVariants = exports.getSize();

			for( hkInt32 i = 0; i < exportsContainer.m_numNamedVariants; ++i )
			{
				const char* name = exportNames[i].cString();
				const void* obj = exports[i];
				const hkClass* klass = hkBuiltinTypeRegistry::getInstance().getVtableClassRegistry()->getClassFromVirtualInstance( obj );

				exportsContainer.m_namedVariants[i].set( name, const_cast<void*>(obj), klass );
			}
		}

		// Serialize the data
		hkBinaryPackfileWriter packfileWriter;
		packfileWriter.setContents( &exportsContainer, hkRootLevelContainerClass );

		// Add exports 
		for( hkInt32 i = 0; i < exports.getSize(); ++i )
		{			
			packfileWriter.addExport( exports[i], exportNames[i].cString() );
		}

		// Save the serialized data
		hkBinaryPackfileWriter::Options options;
		packfileWriter.save( outfile.getStreamWriter(), options );

		// Deallocate the root level container
		{
			delete [] exportsContainer.m_namedVariants;
		}
	}
}

void AssetStreamingDemo::loadSection( const char* worldSectionPathname, hkpPhysicsData*& physicsDataOut, hkPackfileData*& dataMemoryBlockOut, hkResource* shapeRegistry )
{
	// Open the file
	hkIstream infile( worldSectionPathname );

	// Load the file into a binary packfile reader.  Use a hkXmlPackfileReader here if your data is XML.
	hkBinaryPackfileReader packfileReader;
	packfileReader.loadEntireFile( infile.getStreamReader() );
	
	// Get the contents of the binary packfile.
	physicsDataOut = static_cast<hkpPhysicsData*>(packfileReader.getContents( hkpPhysicsDataClass.getName() ));

	// Store a reference to the memory block the contents is stored in
	dataMemoryBlockOut = packfileReader.getAllocatedData();
	dataMemoryBlockOut->addReference();	

	// Fix up all imports with exports in shape registry
	{
		hkArray<hkResource::Import> sectionImports;
		hkArray<hkResource::Export> sectionExports;		
		dataMemoryBlockOut->getImportsExports( sectionImports, sectionExports );
		HK_ASSERT2( 0x3b51aa2d, sectionExports.getSize() == 0, "World section should not have any exports (i.e. other packfiles should not rely on pointers in the section)" );

		hkArray<hkResource::Import> shapeRegistryImports;
		hkArray<hkResource::Export> shapeRegistryExports;		
		shapeRegistry->getImportsExports( shapeRegistryImports, shapeRegistryExports );
		HK_ASSERT2( 0x3b51aa2d, shapeRegistryImports.getSize() == 0, "Shape registry should not have any imports (i.e. this packfile should not rely on pointers in other packfiles)" );
		
		// For each import find its export and fix it up
		for( hkInt32 i = 0; i < sectionImports.getSize(); ++i )
		{
			hkBool fixupFound = false;

			for( hkInt32 j = 0; j < shapeRegistryExports.getSize(); ++j )
			{
				if( hkString::strCmp( sectionImports[i].name, shapeRegistryExports[j].name ) == 0 )
				{
					*sectionImports[i].location = shapeRegistryExports[j].data;
					fixupFound = true;
					break;
				}
			}

			HK_ASSERT2( 0x3b51aa2d, fixupFound, "Shape not found in shape registry." );
		}		
	}	
}

void AssetStreamingDemo::unloadSection( hkpPhysicsData*& physicsDataOut, hkPackfileData*& dataMemoryBlockOut )
{
	physicsDataOut = HK_NULL;
	dataMemoryBlockOut->removeReference();
	dataMemoryBlockOut = HK_NULL;
}

void AssetStreamingDemo::addSectionToWorld( hkpPhysicsData*& physicsData, hkpWorld* world )
{
	for( hkInt32 i = 0; i < physicsData->getPhysicsSystems().getSize(); ++i )
	{
		// hkpWorld::addPhysicsSystem will batch add entities
		physicsData->getPhysicsSystems()[i]->setActive( true );
		world->addPhysicsSystem( physicsData->getPhysicsSystems()[i] );
	}		
}

void AssetStreamingDemo::removeSectionFromWorld( hkpPhysicsData* physicsData, hkpWorld* world )
{
	for( hkInt32 i = 0; i < physicsData->getPhysicsSystems().getSize(); ++i )
	{
		// hkpWorld::removePhysicsSystem will batch remove entities
		world->removePhysicsSystem( physicsData->getPhysicsSystems()[i] );
	}	
}

#if	defined	(	HK_COMPILER_MWERKS	)
	#	pragma	force_active	on
	#	pragma	fullpath_file	on
#	endif

HK_DECLARE_DEMO(AssetStreamingDemo, HK_DEMO_TYPE_PHYSICS, "", "" );

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
