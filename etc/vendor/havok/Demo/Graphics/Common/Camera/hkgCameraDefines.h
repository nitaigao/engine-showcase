/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_GRAPHICS_CAMERA_DEFINES__H
#define HK_GRAPHICS_CAMERA_DEFINES__H

typedef unsigned short HKG_CAMERA_PROJECTION_MODE;
#define HKG_CAMERA_PERSPECTIVE  1
#define HKG_CAMERA_ORTHOGRAPHIC 2
#define HKG_CAMERA_COMPUTE_ORTHOGRAPHIC 4 // orthographic, but get settings from world size given to pos, fov, etc

#define HKG_CAMERA_DEFAULT_NEAR_PLANE   1
#define HKG_CAMERA_DEFAULT_FAR_PLANE    5000
#define HKG_CAMERA_STANDARD_ASPECT		(4.0f/3.0f)
#define HKG_CAMERA_WIDESCREEN_ASPECT	(16.0f/9.0f)
#define HKG_CAMERA_DEFAULT_ASPECT		(HKG_CAMERA_STANDARD_ASPECT)

typedef unsigned short HKG_CAMERA_HANDEDNESS_MODE;
#define HKG_CAMERA_HANDEDNESS_RIGHT     1 // default, ala OpenGL
#define HKG_CAMERA_HANDEDNESS_LEFT      2

#endif // HK_GRAPHICS_CAMERA_DEFINES__H

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
