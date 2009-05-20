/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_GRAPHICS_DX10_PRECOMPILE_H
#define HK_GRAPHICS_DX10_PRECOMPILE_H

#ifndef _WINDOWS_
#define STRICT 1
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#undef _WIN32_WINNT
#define _WIN32_WINNT 0x0500	  // win2000
#undef _WIN32_WINDOWS 
#define _WIN32_WINDOWS 0x0500 // win2000
#undef WINVER	
#define WINVER 0x0500	      // win2000

//#define DIRECT3D_VERSION         0x0900
//#define DIRECTINPUT_VERSION		 0x0800
#include <windows.h>
#endif // ifndef _WINDOWS_

#include <d3d10.h>

#include <Graphics/Common/hkGraphics.h> // the source header for the hkg precompiles

#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }
#define SAFE_REMOVEREFERENCE(p)      { if(p) { (p)->removeReference(); (p)=NULL; } }


#endif //HK_GRAPHICS_DX10_PRECOMPILE_H

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
