/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_ASSET_STREAMING_DEMO_H
#define HK_ASSET_STREAMING_DEMO_H

#include <Demos/DemoCommon/DemoFramework/hkDefaultPhysicsDemo.h>

class hkpWorld;
class hkResource;
class hkPackfileData;
class hkpPhysicsData;

	// Hardcoded number of world sections in this demo
#define HK_ASSET_STREAMING_DEMO_NUM_WORLD_SECTIONS 6

	//
	// A demo that loads several packfiles, identifies shareable shapes within them and then uses
	// imports & exports to save shared shape data into a separate packfile.
	//
	// The demo assumes that all hkpShape data used by the demo can be loaded into memory at once.
	//
	// After loading all physics data (including rigid bodies, constraints, phantoms, etc.) the demo
	// uses the hkpShapeSharingUtil to identify hkpShapes that can be shared between any loaded
	// hkpRigidBody instance.  
	//
	// After instance-able shapes are identified, they are gathered and serialized to a "shape 
	// registry" packfile.  Each shape pointer is marked, with a unique name, as an export in this 
	// packfile.
	//
	// The original physics packfiles are then re-saved.  During this save each shared hkpShape instance
	// is set to HK_NULL since the actual hkpShape instance now lives in the shape registry packfile. When
	// setting each shape instance to HK_NULL, each instances is also marked, with a unique name, as an
	// import.
	//
	// After the shape registry and "instanced" packfiles are serialized, they are randomly loaded
	// and unload by the demo.  When loading imports, or the HK_NULL hkpShape pointers, the matching
	// export is found in the shape registry.  The demo does not serialize state information when 
	// unloading sections.
	//

class AssetStreamingDemo: public hkDefaultPhysicsDemo
{
	public:
			/// Initializes the demo.  Loads reference world sections, identifies sharable shapes, builds
			/// the shape registry, serializes the registry and modified reference packfiles and loads
			/// the first world section into the physics world.
		AssetStreamingDemo( hkDemoEnvironment* env );
			/// Unloads all loaded packfiles.
		~AssetStreamingDemo();

			/// Steps the demo by randomly loading and unloading world sections 
		hkDemo::Result stepDemo();

	protected:
			//
			// Shape sharing / instancing functionality (used as setup)
			//

			/// Top level function to load packfiles, instance shapes and save modified packfiles.  
		void instanceShapeDataInWorld( const hkObjectArray<hkString>& originalWorldSectionPathnames, const hkArray<const char*>& instancedWorldSectionPathnames, const char* shapeRegistryPathname );
			/// Loads all physics data from the originalWorldSectionPathnames provided.
		void loadAllPhysicsData( const hkObjectArray<hkString>& originalWorldSectionPathnames, hkArray<hkpPhysicsData*>& allLoadedPhysicsData, hkArray<hkPackfileData*>& allLoadedData );
			/// Saves all physics data in allLoadedPhysicsData to files identified by instancedWorldSectionPathnames.  Exports are saved to the file identified by shapeRegistryPathname
		void saveAllPhysicsDataWithExports( const hkArray<const char*>& instancedWorldSectionPathnames, const char* exportsPathname, const hkArray<hkpPhysicsData*>& allLoadedPhysicsData, const hkArray<const void*>& exports );

			//
			// Streaming functionality
			//	

			/// Loads the section at worldSectionPathname into memory.
		void loadSection( const char* worldSectionPathname, hkpPhysicsData*& physicsDataOut, hkPackfileData*& dataMemoryBlockOut, hkResource* shapeRegistry );
			/// Unloads the section pointed to by dataMemoryBlockOut from memory.
		void unloadSection( hkpPhysicsData*& physicsDataOut, hkPackfileData*& dataMemoryBlockOut );
			/// Add the physics data to the world
		void addSectionToWorld( hkpPhysicsData*& physicsData, hkpWorld* world );
			/// Removes the physics data from the world
		void removeSectionFromWorld( hkpPhysicsData* physicsData, hkpWorld* world );

	protected:		
		/// Memory block of the shape registry
		hkPackfileData* m_shapeRegistryMemoryBlock;

		/// Pointers to currently loaded world sections.  Set to HK_NULL if section is not loaded.
		hkpPhysicsData* m_worldSections[HK_ASSET_STREAMING_DEMO_NUM_WORLD_SECTIONS];
		/// Pointers to currently loaded world section memory blocks.  Set to HK_NULL if section is not loaded.
		hkPackfileData* m_worldSectionMemoryBlocks[HK_ASSET_STREAMING_DEMO_NUM_WORLD_SECTIONS];

		/// Used for setup.  Pathnames for read only (original) world sections.
		hkObjectArray<hkString> m_originalWorldSectionPathnames;
		/// Used for setup and loading.  Pathnames for modified world sections.
		hkArray<const char*> m_instancedWorldSectionPathnames;
		/// Used for setup and loading.  Pathname to shape registry.
		char* m_shapeRegistryPathname;

		int m_frameCounter;
};

#endif	//HK_ASSET_STREAMING_DEMO_H

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
