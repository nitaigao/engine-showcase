/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_GRAPHICS_WINDOW_USER_FUNC_H
#define HK_GRAPHICS_WINDOW_USER_FUNC_H

#include <Common/Base/Types/hkBaseTypes.h>
#include <Graphics/Common/Input/Keyboard/hkgKeyboard.h>
#include <Graphics/Common/Input/Mouse/hkgMouse.h>
#include <Graphics/Common/Input/Pad/hkgPad.h>

class hkgWindow;

//
// All user functions return whether the user handled it / understood it / etc
// So return false if you want the window to do its default behavior for the 
// key / event.
//

// Keyboard
typedef bool (HK_CALL *HKG_USER_FUNC_KEY)(hkgWindow* w, HKG_KEYBOARD_VKEY key, bool state, void* userContext);

// Mouse
typedef bool (HK_CALL *HKG_USER_FUNC_MOUSE_MOVE)(hkgWindow* w, int x, int y, int dx, int dy, void* userContext);
typedef bool (HK_CALL *HKG_USER_FUNC_MOUSE_WHEEL)(hkgWindow* w, int delta, void* userContext);
typedef bool (HK_CALL *HKG_USER_FUNC_MOUSE_BUTTON)(hkgWindow* w, HKG_MOUSE_BUTTON button, bool state, void* userContext);

// Gamepad
typedef bool (HK_CALL *HKG_USER_FUNC_PAD_MOVE)(hkgWindow* w, unsigned short pad, unsigned short stick, float x, float y, float dx, float dy, void* userContext);
typedef bool (HK_CALL *HKG_USER_FUNC_PAD_BUTTON)(hkgWindow* w, unsigned short pad, HKG_PAD_BUTTON button, bool state, void* userContext);

// Window events
typedef void (HK_CALL *HKG_USER_FUNC_RESIZE)(hkgWindow* w,unsigned int width, unsigned int height, void* userContext);
typedef bool (HK_CALL *HKG_USER_FUNC_COMMAND)(hkgWindow* w, unsigned int code, unsigned int id, void* nativeHandle, void* userContext);



#endif // HK_GRAPHICS_WINDOW_USER_FUNC_H

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
