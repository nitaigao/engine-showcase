/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#include <Demos/demos.h>
#include <Demos/DemoCommon/Utilities/FileFinder/FileFinder.h>
#include <Common/Base/System/Io/FileSystem/hkFileSystem.h>

FileFinder::FileFinder()
{
}

void FileFinder::addFolder(const char* s)
{
	m_foldersTodo.pushBack(s);
}

hkString FileFinder::popNextFile()
{
	while( m_filesTodo.getSize() == 0 )
	{
		if( m_foldersTodo.getSize() == 0 )
		{
			return hkString();
		}
		examineNextFolder();
	}
	hkString s = m_filesTodo.back();
	m_filesTodo.popBack();
	return s;
}

void FileFinder::examineNextFolder()
{
	const hkString curFolder = m_foldersTodo.back();
	m_foldersTodo.popBack();

	hkFileSystem& fs = hkFileSystem::getInstance();
	hkFileSystem::DirectoryListing listing;
	fs.listDirectory(curFolder.cString(), listing);
	const hkArray<hkFileSystem::Entry>& entries = listing.getEntries();
	for( int entryIndex = 0; entryIndex < entries.getSize(); ++entryIndex )
	{
		const hkFileSystem::Entry& entry = entries[entryIndex];
		hkString fullName; fullName.printf("%s/%s", curFolder.cString(), entry.name);
		hkBool32 isOk = true;
		for( int i = 0; isOk && i < m_filters.getSize(); ++i )
		{
			if( m_filters[i]->isOk(fullName.cString(), entry.isDir() ) == false )
			{
				isOk = false;
			}
		}
		if( isOk )
		{
			hkString& s = entry.isFile()
				? m_filesTodo.expandOne()
				: m_foldersTodo.expandOne();
			s = fullName;
		}
	}
}

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
