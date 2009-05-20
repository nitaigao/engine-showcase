/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#include <Demos/demos.h>
#include <Demos/DemoCommon/Utilities/Asset/hkAssetManagementUtil.h>
#include <Common/Base/System/Io/IStream/hkIStream.h>

#define NEED_PLATFORM_SPECIFIC_EXTENSION

void hkAssetManagementUtil::getFileEnding(hkString& e, hkStructureLayout::LayoutRules rules)
{
	hkStructureLayout l;
	e.printf("_L%d%d%d%d", 
		rules.m_bytesInPointer,
		rules.m_littleEndian? 1 : 0,
		rules.m_reusePaddingOptimization? 1 : 0,
		rules.m_emptyBaseClassOptimization? 1 : 0);
		
}

static bool _fileExists( const char* filename )
{
	// Open
	hkIfstream file( filename );
	
	// Check
	if (file.isOk())
	{
		// Dummy read
		char ch;
		file.read( &ch , 1);
		return file.isOk();
	}

	return false;

}

hkString HK_CALL hkAssetManagementUtil::getFilePath( const char* filename, hkStructureLayout::LayoutRules rules )
{
	hkString assetName(filename); // straight through

#ifdef NEED_PLATFORM_SPECIFIC_EXTENSION
	if (! _fileExists( filename ) )
	{
		// Try platform extension
		int extn = assetName.lastIndexOf('.');
		if (extn != -1)
		{
			assetName = hkString(filename, extn);
			hkString fe; getFileEnding(fe, rules); 
			assetName += fe;
			assetName += &(filename[extn]);
		}
	}
#endif
	
#ifdef HK_DEBUG
	{
		int a0 = assetName.lastIndexOf('\\');
		int a1 = assetName.lastIndexOf('/');
		int aLen = assetName.getLength() - 1; // last index
		int mD0 = a0 >= 0? a0 : 0;
		int mD1 = a1 >= 0? a1 : 0;
		int maxSlash = mD0 > mD1? mD0 : mD1;
		if ( (aLen - maxSlash) >= 42 )
		{
			hkString w;
			w.printf("Your file name [%s] is longer than 42 characters. May have issues on some consoles (like Xbox360).", assetName.cString() );
			HK_WARN(0x04324, w.cString() );
		}
	}
#endif

	return assetName;
}

hkString hkAssetManagementUtil::getFilePath( const char* filename )
{
	return getFilePath( filename, hkStructureLayout::HostLayoutRules );
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
