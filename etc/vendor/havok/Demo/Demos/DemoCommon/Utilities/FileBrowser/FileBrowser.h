/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_FILE_BROWSER_H
#define HK_FILE_BROWSER_H

#include <Common/Base/System/Io/FileSystem/hkFileSystem.h>

// Useful filters
hkBool HK_CALL hkFileFilter_isValidPackfile( const char* );
hkBool HK_CALL hkDirectoryFilter_notCvs( const char* );

class FileBrowser
{
	public:

			/// Return false if this file or directory should be filtered
		typedef hkBool (HK_CALL *FILTER)( const char* );

			/// Constuct a file browser
		FileBrowser(FILTER fileFilter = hkFileFilter_isValidPackfile, FILTER directoryFilter = hkDirectoryFilter_notCvs);

			/// The full path to the current directory
		hkString m_curDir;

			/// The current directory listing
		hkFileSystem::DirectoryListing m_listing;

			/// The currently selected entry in the directory listing
		int m_curIndex;

			/// Display the current directory at (startX, startY) and update based on gamepad
		hkBool parseAndDisplayDirectoryAndFiles(const hkDemoEnvironment* m_env, int startX, int startY, bool recursive = false);

			// Returns the full path to the selected file / directory
		hkString getFullPath( ) const;

	protected:

		FILTER m_directoryFilter;
		FILTER m_fileFilter;

		hkString m_prevDir;
		hkObjectArray<hkString> m_toScan;

};



#endif // HK_FILE_BROWSER_H

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
