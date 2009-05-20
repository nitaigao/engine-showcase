/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_GRAPHICS_WINDOW_DEFINES__H
#define HK_GRAPHICS_WINDOW_DEFINES__H

typedef int HKG_WINDOW_CREATE_FLAG;
#define HKG_WINDOW_DEFAULT    0  // whatever suits the paltform (usually HKG_WINDOW_WINDOWED | HKG_WINDOW_PRIMARY_DEVICE on PC, HKG_WINDOW_FULLSCREEN | HKG_WINDOW_PRIMARY_DEVICE on Consoles)
#define HKG_WINDOW_WINDOWED   1
#define HKG_WINDOW_FULLSCREEN 2
#define HKG_WINDOW_PRIMARY_DEVICE 4    // use operating systems primary display device (default)
#define HKG_WINDOW_SECONDARY_DEVICE 8  // use operating systems secondary display device (handy in multi mon with game on primary fullscreen)
#define HKG_WINDOW_TERTIARY_DEVICE  16 // use operating systems third (tertiary) display device (handy in multi mon with game on primary fullscreen)
#define HKG_WINDOW_QUATERNARY_DEVICE  32 // use operating systems fourth (quaternary) display device (handy in multi mon with game on primary fullscreen)
#define HKG_WINDOW_DEVICE_MASK (4 | 8 | 16 | 32)
#define HKG_WINDOW_FSAA       64   //try to create using FSAA



// Extra hints (create flags aswell)
#define HKG_WINDOW_HINT_VGA		0x1000
#define HKG_WINDOW_HINT_480i	0x1100
#define HKG_WINDOW_HINT_480p	0x1200
#define HKG_WINDOW_HINT_576i	0x1300
#define HKG_WINDOW_HINT_576p	0x1400
#define HKG_WINDOW_HINT_720i	0x1500
#define HKG_WINDOW_HINT_720p	0x1600
#define HKG_WINDOW_HINT_1080i	0x1700
#define HKG_WINDOW_HINT_1080p	0x1800

typedef int HKG_WINDOW_MSG_STATUS;
#define HKG_WINDOW_MSG_QUIT     0
#define HKG_WINDOW_MSG_CONTINUE 1	

typedef int HKG_WINDOW_BUFFERS;
#define HKG_WINDOW_BUF_COLOR   1
#define HKG_WINDOW_BUF_DEPTH16 2
#define HKG_WINDOW_BUF_DEPTH32 4
#define HKG_WINDOW_BUF_STENCIL 8

typedef short HKG_SHADOWMAP_SUPPORT;
#define HKG_SHADOWMAP_NOTKNOWN  0 // unknown / untested. Not a valid return type.
#define HKG_SHADOWMAP_NOSUPPORT 1 // none
#define HKG_SHADOWMAP_HARDWARE  2 // A proper depth map and the hw can do the depth comaprison automagically
#define HKG_SHADOWMAP_FLOATMAP  4 // like directx R32F format 
#define HKG_SHADOWMAP_VSM       8 // Variance Shadowmaps. Requires float texture typez 

typedef short HKG_SHADOWMAP_MODE;
#define HKG_SHADOWMAP_MODE_DEFAULT   0 // don't care (usally PSM)
#define HKG_SHADOWMAP_MODE_FIXED     1 // fixed projection (you need to set the fov for the light proj)
#define HKG_SHADOWMAP_MODE_PSM       2 // Perspective Shadowmaps. Auto compute the best fit of light and cam frustum. Can get a bit skewed.

typedef short HKG_SHADOWMAP_QUALITY;
#define HKG_SHADOWMAP_QUALITY_UNKNOWN   0 // 
#define HKG_SHADOWMAP_QUALITY_16F       1 // 16 bit float buffers
#define HKG_SHADOWMAP_QUALITY_16F_PLUS  2 // 16 bit float buffers with multipke
#define HKG_SHADOWMAP_QUALITY_24F       3 // 24 bit float buffers (usually a h/w depth buffer in this case)
#define HKG_SHADOWMAP_QUALITY_32F       4 // 32 bit float buffers 

typedef short HKG_MESHBLENDING_SUPPORT;
#define HKG_MESHBLENDING_NOTKNOWN  0 // unknown / untested. Not a valid return type.
#define HKG_MESHBLENDING_NOSUPPORT 1 // none. Calling code must do the skinning itself.
#define HKG_MESHBLENDING_HARDWARE  2 // Supports full skinning in H/W (VU code on PS2 or has a built in shader in DX9S etc). Assumed to suport local skinning with post object transform.
#define HKG_MESHBLENDING_SOFTWARE_WORLD 4 // Supports skinning in S/W based on world bone matrices (DirectX for instance)
#define HKG_MESHBLENDING_SOFTWARE_LOCAL 8 // Supports skinning in S/W and post transforms with object world matrix.

#endif // HK_GRAPHICS_WINDOW_DEFINES__H

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
