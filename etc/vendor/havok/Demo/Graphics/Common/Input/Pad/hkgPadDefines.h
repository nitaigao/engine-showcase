/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_GRAPHICS_PAD_DEFINES__H
#define HK_GRAPHICS_PAD_DEFINES__H

/// Pads are assumed to have 4 main action buttons.
/// They are numbered clockwise starting from the main button.
/// 
/// PlayStation(R)2 : Cross, Square, Triangle, Circle.
/// Xbox : Green, Blue, Yellow, Red.
/// PC : Space, 1, 2, 3
/// Gamecube : A, B, Y, X
typedef unsigned int HKG_PAD_BUTTON;

#define HKG_PAD_BUTTON_0    (1)
#define HKG_PAD_BUTTON_1    (1<<1)
#define HKG_PAD_BUTTON_2    (1<<2)
#define HKG_PAD_BUTTON_3    (1<<3)

// DPAD maps to POV hats on PC sticks
#define HKG_PAD_DPAD_UP     (1<<4)
#define HKG_PAD_DPAD_DOWN   (1<<5)
#define HKG_PAD_DPAD_LEFT   (1<<6)
#define HKG_PAD_DPAD_RIGHT  (1<<7)

#define HKG_PAD_SELECT	    (1<<8)	
#define HKG_PAD_START       (1<<9)

// try not to depend on any of these as a lot of controllers might not have them:
#define HKG_PAD_BUTTON_L1   (1<<10)
#define HKG_PAD_BUTTON_R1   (1<<11)

#define HKG_PAD_BUTTON_L2   (1<<12)
#define HKG_PAD_BUTTON_R2   (1<<13)

#define HKG_PAD_BUTTON_LSTICK  (1<<14)
#define HKG_PAD_BUTTON_RSTICK  (1<<15)

#define	HKG_PAD_NUM_BUTTONS 16
	
#endif // HK_GRAPHICS_PAD_DEFINES__H

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
