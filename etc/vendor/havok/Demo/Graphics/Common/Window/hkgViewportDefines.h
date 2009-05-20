/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_GRAPHICS_VIEWPORT_DEFINES__H
#define HK_GRAPHICS_VIEWPORT_DEFINES__H

typedef unsigned int HKG_VIEWPORT_SIDE;
#define HKG_VIEWPORT_SIDE_NONE		0
#define HKG_VIEWPORT_SIDE_TOP		1
#define HKG_VIEWPORT_SIDE_BOTTOM	2 
#define HKG_VIEWPORT_SIDE_LEFT		4
#define HKG_VIEWPORT_SIDE_RIGHT		8

/// This is used to define the default movement style of the mouse, fly or trackball
/// If trackball is used, 
typedef unsigned int HKG_CAMERA_NAVIGATION_MODE;
/// Ignore all mouse/pad/keyboard events that would normally be used for the navigation
#define	HKG_CAMERA_NAV_DISABLED 0
/// Virtual trackball around the point of interest of the camera
#define	HKG_CAMERA_NAV_TRACKBALL 1
/// Fly mode, quake style
#define HKG_CAMERA_NAV_FLY 2

/// This is used to define the convention used to transform mouse commands
/// to camera movements (in trackball mode)
typedef unsigned int HKG_MOUSE_CONVENTION;
/// Default : left button (LB) arc rotates, right button (RB) pans, middle button (down) zooms (in), wheel (back) zooms (in)
#define	HKG_MC_DEFAULT 0
/// Follows 3ds max conventions : ALT+MB arc rotates ; MB pans ; CTRL+ALT+MB (up) zooms (in), wheel (up) zooms (in)
/// If the MB is not used, it follows the default conventions for LB and RB
#define HKG_MC_3DSMAX 1
/// Follows Maya conventions : ALT+LB : arc rotate ; ALT+MB pans ; ALT+RB (up) zooms (out)
/// If the ALT key is not pressed, it follows the default convention for LB, MB and RB
#define HKG_MC_MAYA 2


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
