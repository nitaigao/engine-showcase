/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_GRAPHICS_OGLS_PRECOMPILE_H
#define HK_GRAPHICS_OGLS_PRECOMPILE_H

#include <Graphics/Common/hkGraphics.h> // the source header for the hkg precompiles

#ifdef HK_PLATFORM_PS3_PPU
#   define CELL_GCM_LITTLE_ENDIAN 0
#	include <PSGL/psgl.h>
#	include <PSGL/psglu.h>
#elif defined(WIN32)
#	ifndef _WINDOWS_
#		define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#		undef _WIN32_WINNT 
#		define _WIN32_WINNT 0x0500	  // xp/2000
#		undef _WIN32_WINDOWS  
#		define _WIN32_WINDOWS 0x0500 // xp/2000
#		if (_MSC_VER >= 1400)
#			define _CRT_SECURE_NO_DEPRECATE 1
#			define _CRT_NONSTDC_NO_DEPRECATE 1
#		endif
#		include <windows.h>
#	endif
#	include <GL/gl.h>
#	include <GL/glext.h>
#elif defined(HK_ARCH_X64)
#	define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
//#	define _WIN32_WINNT 0x0500	  // xp/2000
#	define _WIN32_WINDOWS 0x0500 // xp/2000
#	include <windows.h>
#	include <GL/gl.h>
#	include <GL/glext.h>
#endif

// Assumes Cg on all supported platforms
#include <Cg/cg.h>
#include <Cg/cgGL.h>

#endif //HK_GRAPHICS_PS3_PRECOMPILE_H

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
