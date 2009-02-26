/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2008 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_WORLDSNAPSHOTAPI_H
#define HK_WORLDSNAPSHOTAPI_H

#include <Demos/DemoCommon/DemoFramework/hkDefaultPhysicsDemo.h>
#include <Common/Base/System/Io/FileSystem/hkFileSystem.h>

class hkPackfileData;
class hkRootLevelContainer;

/// This demo shows how browse the filesystem to load a resource file.
class BrowseDemo : public hkDefaultPhysicsDemo
{
	public:

		BrowseDemo( hkDemoEnvironment* env );
		~BrowseDemo();

		Result stepDemo();


		struct DirectoryData
		{
			hkString m_curDir;
			hkString m_prevDir;
			int m_curIndex;

			hkFileSystem::DirectoryListing m_listing;
			hkObjectArray<hkString> m_toScan;


			DirectoryData()
			:	m_curDir("")
			,	m_prevDir(" ") //anything which does not equal m_curDir
			,	m_curIndex(0)
			{
			}

			hkBool parseAndDisplayDirectoryAndFiles(const hkDemoEnvironment* m_env);
		};

	protected:

//		hkResult getFolderContents( const char* pathName, hkFileSystem::DirectoryListing& dirsOut );
//		hkResult openDir(const char* dirPath);
		hkResult readAndSetupPackfile(const char* filename);
//		hkResult backOneLevel();
//		void setupWorld();

	protected:

			// A handle to a deallocator which will deallocate the packfile
		hkPackfileData* m_packfileData;
		hkRootLevelContainer* m_contents;

		DirectoryData m_directoryData;
		
		hkBool32 m_running;

};

#endif // HK_WORLDSNAPSHOTAPI_H

/*
* Havok SDK - NO SOURCE PC DOWNLOAD, BUILD(#20080925)
* 
* Confidential Information of Havok.  (C) Copyright 1999-2008
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
