/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#include <Demos/demos.h>


#if defined( HK_DEBUG ) && defined ( HK_PLATFORM_WIN32 ) && (HK_COMPILER_MSVC_VERSION >= 1400)
	// Disallow all calls to global new 
	// Make sure your allocation is using the Havok memory manager
	#include <new>
	void * __cdecl operator new (size_t size)
	{
		HK_BREAKPOINT(0x0);
		return 0;
	} 

#endif

extern void HK_CALL DemoPlatformInit(hkDemoFrameworkOptions*)
{

#if defined(HK_COMPILER_HAS_INTRINSICS_IA32) && HK_CONFIG_SIMD == HK_CONFIG_SIMD_ENABLED
	// Flush all denormal/subnormal numbers (2^-1074 to 2^-1022) to zero.
	// Typically operations on denormals are very slow, up to 100 times slower than normal numbers.
	_MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_ON);
#endif


	HANDLE thread = GetCurrentThread();
	SetThreadIdealProcessor(thread, 0); // try to keep on single core
	// can use XP/Vista/Server2003 etc SetThreadAffinityMask too if we want to be more forceful

	// XAudio requires COM init
	// If the following line does not compile, it means that windows.h was included with
	// a lot for #defines to cut down linkage. A full windows.h include is required, with winnt ver >= 4.
	// In the demos this is done on the PCH, demos.h, before any Havok base windows includes.
	CoInitializeEx(0, COINIT_MULTITHREADED);

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
