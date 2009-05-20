/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_DEMOS_DEMOS_H
#define HK_DEMOS_DEMOS_H

// Do a full windows incl so that Com etc included.
#   define _WIN32_WINNT 0x0500   // Windows2000 or higher
#       if (_MSC_VER >= 1400)
#               define _CRT_SECURE_NO_DEPRECATE 1
#               define _CRT_NONSTDC_NO_DEPRECATE 1
#       endif
#       define NOMINMAX
#       include <windows.h>

#include <Demos/DemoCommon/DemoFramework/hkDemoFramework.h>

#include <Common/Base/KeyCode.h>

// Enable the next line if your compiler supports precompiled headers.
//#ifdef HK_COMPILER_SUPPORTS_PCH
#if defined(USING_HAVOK_PHYSICS)
#	include <Demos/DemoCommon/DemoFramework/hkDefaultPhysicsDemo.h>
#elif defined(USING_HAVOK_ANIMATION)
#	include <Demos/DemoCommon/DemoFramework/hkDefaultAnimationDemo.h>
#endif
//#endif

#endif // HK_DEMOS_DEMOS_H



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
