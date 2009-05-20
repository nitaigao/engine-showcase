/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/Common/Api/Serialize/Versioning/DumpClassList/DumpClassListDemo.h>

// Serialize includes
#include <Common/Base/System/Io/IStream/hkIStream.h>
#include <Common/Base/Config/hkConfigVersion.h>
#include <Common/Base/Reflection/hkClass.h>
#include <Common/Serialize/Version/hkVersionUtil.h>

extern const hkClass CustomClassLocalDataClass;
extern const hkClass CustomClassClass;
extern const hkClass CustomClass2Class;

static const hkClass* classes[] =
{
	&CustomClassLocalDataClass,
	&CustomClassClass,
	&CustomClass2Class,
	HK_NULL
};

DumpClassListDemo::DumpClassListDemo( hkDemoEnvironment* env) 
	: hkDefaultDemo(env) 
{
	hkString filePath("Common/Api/Serialize/Versioning/DumpClassList/");
	hkString fileName = hkString(hkVersionUtil::getCurrentVersion()).replace("-","").replace(".","")+"Classes";
	const char registryVariableName[] = "CustomStaticRegistry";

	// Dump all currently reflected runtime classes
	// Open a stream to write the cpp header file
	{
		hkOstream outfile( hkString(filePath + fileName + ".h").cString() );
		HK_ASSERT2( 0x215d080f, outfile.isOk(), "Could not open '" << fileName.cString() << "h' for writing." );
		// extern the runtime classes into the header file
		hkVersionUtil::generateCppExternClassList(outfile, fileName.replace(".","_").asUpperCase().cString(), classes, registryVariableName);
	}
	// Open a stream to write the cpp source file
	{
		hkOstream outfile( hkString(filePath + fileName + ".cpp").cString() );
		HK_ASSERT2( 0x215d0810, outfile.isOk(), "Could not open '" << fileName.cString() << "cpp' for writing." );
		// set demos pch file
		const char* pchfilename = "demos/Demos.h";
		// dump the runtime classes into the cpp file
		hkVersionUtil::generateCppClassList(outfile, classes, pchfilename, registryVariableName);
	}
}

hkDemo::Result DumpClassListDemo::stepDemo()
{
	return hkDefaultDemo::stepDemo();
}

#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
"This demo dumps list of reflected classes to a CPP file.";

HK_DECLARE_DEMO(DumpClassListDemo, HK_DEMO_TYPE_PRIME | HK_DEMO_TYPE_SERIALIZE, "Shows how to dump list of custom reflected classes to a CPP file.", helpString);

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
