/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#include <Demos/DemoCommon/DemoFramework/hkDemo.h>
#include <Demos/demos.h>
#include <Demos/DemoCommon/DemoFramework/hkDemo.h>

#include <Demos/DemoCommon/DemoFramework/hkDemoConsole.h>

#include <Common/Base/System/Io/FileSystem/hkFileSystem.h>
#include <Common/Base/System/Io/Writer/hkStreamWriter.h>

HK_SINGLETON_IMPLEMENTATION(hkDemoConsole);


#	include <Common/Base/System/Io/Writer/Stdio/hkStdioStreamWriter.h>
	typedef hkStdioStreamWriter DefaultConsoleWriter;
	

hkDemoConsole::hkDemoConsole()
:	m_stdout( new DefaultConsoleWriter( hkFileSystem::STDOUT) ),
	m_stderr( new DefaultConsoleWriter( hkFileSystem::STDERR) )
{
	m_stdout.getStreamWriter()->removeReferenceLockUnchecked();
	m_stderr.getStreamWriter()->removeReferenceLockUnchecked();
}

hkOstream& hkDemoConsole::getStdout()
{
	return m_stdout;
}

hkOstream& hkDemoConsole::getStderr() 
{ 
	return m_stderr;
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
