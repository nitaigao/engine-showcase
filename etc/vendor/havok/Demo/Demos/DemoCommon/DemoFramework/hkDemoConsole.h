/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK_DEMOFRAMEWORK_DEMOCONSOLE_H
#define HK_DEMOFRAMEWORK_DEMOCONSOLE_H

/// Contains the standard output and standard error used by Havok.
class hkDemoConsole : public hkReferencedObject, public hkSingleton<hkDemoConsole>
{
	public:

		hkDemoConsole();

		hkOstream& getStdout();
		hkOstream& getStderr();

		hkOstream m_stdout;
		hkOstream m_stderr;

	private:

		friend class hkSingleton<hkDemoConsole>;
};

/// Convenience macro to print to stdout
// This is a temporary solution, due to "printf"s in the demos - all printfs should be hkprintf, 
// and it should be defined as in the std namespace or not, depending on platform.
#include <Common/Base/Fwd/hkcstdio.h>
#define hkprintf printf
using namespace std; 

/// Convenience macro to access havok stdout
#define hkcout		hkDemoConsole::getInstance().getStdout()

/// Convenience macro to access havok stderr
#define hkcerr		hkDemoConsole::getInstance().getStderr()

#endif // HK_DEMOFRAMEWORK_DEMOCONSOLE_H

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
