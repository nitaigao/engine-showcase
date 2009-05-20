/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_GRAPHICS_KEYBOARD_DEFINES__H
#define HK_GRAPHICS_KEYBOARD_DEFINES__H

typedef unsigned char HKG_KEYBOARD_VKEY;

#define HKG_VKEY_BACK           0x08
#define HKG_VKEY_TAB            0x09

#define HKG_VKEY_RETURN         0x0D
#define HKG_VKEY_SHIFT          0x10
#define HKG_VKEY_CONTROL        0x11
#define HKG_VKEY_MENU           0x12

#define HKG_VKEY_CAPITAL        0x14

#define HKG_VKEY_ESCAPE         0x1B
#define HKG_VKEY_SPACE          0x20
#define HKG_VKEY_END            0x23
#define HKG_VKEY_HOME           0x24

#define HKG_VKEY_LEFT           0x25
#define HKG_VKEY_UP             0x26
#define HKG_VKEY_RIGHT          0x27
#define HKG_VKEY_DOWN           0x28

#define HKG_VKEY_INSERT         0x2D
#define HKG_VKEY_DELETE         0x2E

/* HKG_VKEY_0 thru HKG_VKEY_9 are the same as ASCII '0' thru '9' (0x30 - 0x39) */
/* HKG_VKEY_A thru HKG_VKEY_Z are the same as ASCII 'A' thru 'Z' (0x41 - 0x5A) */

#define HKG_VKEY_LWIN           0x5B
#define HKG_VKEY_RWIN           0x5C
#define HKG_VKEY_APPS           0x5D
#define HKG_VKEY_NUMPAD0        0x60
#define HKG_VKEY_NUMPAD1        0x61
#define HKG_VKEY_NUMPAD2        0x62
#define HKG_VKEY_NUMPAD3        0x63
#define HKG_VKEY_NUMPAD4        0x64
#define HKG_VKEY_NUMPAD5        0x65
#define HKG_VKEY_NUMPAD6        0x66
#define HKG_VKEY_NUMPAD7        0x67
#define HKG_VKEY_NUMPAD8        0x68
#define HKG_VKEY_NUMPAD9        0x69

#define HKG_VKEY_MULTIPLY       0x6A
#define HKG_VKEY_ADD            0x6B
#define HKG_VKEY_SEPARATOR      0x6C
#define HKG_VKEY_SUBTRACT       0x6D
#define HKG_VKEY_DECIMAL        0x6E
#define HKG_VKEY_DIVIDE         0x6F

#define HKG_VKEY_F1             0x70
#define HKG_VKEY_F2             0x71
#define HKG_VKEY_F3             0x72
#define HKG_VKEY_F4             0x73
#define HKG_VKEY_F5             0x74
#define HKG_VKEY_F6             0x75
#define HKG_VKEY_F7             0x76
#define HKG_VKEY_F8             0x77
#define HKG_VKEY_F9             0x78
#define HKG_VKEY_F10            0x79
#define HKG_VKEY_F11            0x7A
#define HKG_VKEY_F12            0x7B
#define HKG_VKEY_F13            0x7C
#define HKG_VKEY_F14            0x7D
#define HKG_VKEY_F15            0x7E
#define HKG_VKEY_F16            0x7F
#define HKG_VKEY_F17            0x80
#define HKG_VKEY_F18            0x81
#define HKG_VKEY_F19            0x82
#define HKG_VKEY_F20            0x83
#define HKG_VKEY_F21            0x84
#define HKG_VKEY_F22            0x85
#define HKG_VKEY_F23            0x86
#define HKG_VKEY_F24            0x87

#define HKG_VKEY_NUMLOCK        0x90
#define HKG_VKEY_SCROLL         0x91
#define HKG_VKEY_LSHIFT         0xA0
#define HKG_VKEY_RSHIFT         0xA1
#define HKG_VKEY_LCONTROL       0xA2
#define HKG_VKEY_RCONTROL       0xA3
#define HKG_VKEY_LMENU          0xA4
#define HKG_VKEY_RMENU          0xA5

#define HKG_VKEY_OEM_1_SEMICOLON	 0xBA // on US
#define HKG_VKEY_OEM_PLUS			 0xBB // 2000/XP 
#define HKG_VKEY_OEM_COMMA			 0xBC // 2000/XP 
#define HKG_VKEY_OEM_MINUS			 0xBD // 2000/XP 
#define HKG_VKEY_OEM_PERIOD			 0xBE // 2000/XP 
#define HKG_VKEY_OEM_2_FORWARDSLASH  0xBF //on US
#define HKG_VKEY_OEM_3_TILDA         0xC0 //on US 
#define HKG_VKEY_OEM_4_OPENBRACKET   0xDB //on US 
#define HKG_VKEY_OEM_5_BACKSLASH     0xDC //on US 
#define HKG_VKEY_OEM_6_CLOSE_BRACKET 0xDD //on US 


#define HKG_KEYBOARD_NUM_VKEYS  0xFF // 255 possible virtual key values

#endif //HK_GRAPHICS_KEYBOARD_DEFINES__H

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
