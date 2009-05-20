/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef DEMO_FILE_FINDER_H
#define DEMO_FILE_FINDER_H

#include <Common/Base/Container/Array/hkObjectArray.h>
#include <Common/Base/Container/String/hkString.h>

class hkIstream;

class FileFinder
{
	public:

		struct Filter
		{
			virtual ~Filter() {}
			virtual hkBool32 isOk(const char* path, hkBool32 isDir) = 0;
		};
		
		FileFinder();

		void addFolder(const char* s);
		hkString popNextFile();

		void addFilter(Filter* f) { m_filters.pushBack(f); }

	protected:

		void examineNextFolder();

		hkArray<Filter*> m_filters;
		hkObjectArray<hkString> m_foldersTodo;
		hkObjectArray<hkString> m_filesTodo;
};

#endif // DEMO_FILE_FINDER_H

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
