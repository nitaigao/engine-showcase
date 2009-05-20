/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_GRAPHICS_TEXTURE_DEFINES__H
#define HK_GRAPHICS_TEXTURE_DEFINES__H

typedef unsigned short HKG_TEXTURE_DEFAULTS;
#define HKG_TEXTURE_HAVOKLOGO	1
#define HKG_TEXTURE_FONT		1<<1
#define HKG_TEXTURE_POINTER		1<<2	
#define HKG_TEXTURE_CHECKER		1<<3
#define HKG_TEXTURE_PERLINNOISE	1<<4
#define HKG_TEXTURE_DIAGONAL	1<<5

typedef unsigned short HKG_TEXTURE_PIXEL_FORMAT;
#define HKG_TEXTURE_PIXEL_RGB      1
#define HKG_TEXTURE_PIXEL_RGBA     1<<1
#define HKG_TEXTURE_PIXEL_BGR      1<<2
#define HKG_TEXTURE_PIXEL_BGRA     1<<3
#define HKG_TEXTURE_PIXEL_ARGB     1<<4
#define HKG_TEXTURE_PIXEL_ABGR     1<<5
#define HKG_TEXTURE_PIXEL_SWIZZLED 1<<6 // can be combined with any of the above formats. It means it is not a linear internal format.

typedef unsigned short HKG_TEXTURE_UVMODE;
#define HKG_TEXTURE_CLAMP	1
#define HKG_TEXTURE_WRAP	1<<1

typedef unsigned short HKG_TEXTURE_TYPE;
#define HKG_TEXTURE_SINGLE	1
#define HKG_TEXTURE_CUBEMAP 1<<1

typedef unsigned short HKG_TEXTURE_COORD_GEN;
#define HKG_TEXTURE_COORDGEN_NONE 1
#define HKG_TEXTURE_COORDGEN_CSNORMAL 1<<1  // camera space normal
#define HKG_TEXTURE_COORDGEN_CSPOS    1<<2  // camera space pos
#define HKG_TEXTURE_COORDGEN_CSREFLECTION    1<<3  // camera space reflection

typedef unsigned short HKG_TEXTURE_FILTER;
#define HKG_TEXTURE_FILTER_POINT	    1
#define HKG_TEXTURE_FILTER_LINEAR      1<<1
#define HKG_TEXTURE_FILTER_ANISOTROPIC 1<<2

typedef unsigned short HKG_DDS_FORMAT_TYPE;
#define HKG_COMPRESSED_RGBA_DXT1 1
#define HKG_COMPRESSED_RGB_DXT1  1<<1
#define HKG_COMPRESSED_RGBA_DXT3 1<<2
#define HKG_COMPRESSED_RGBA_DXT5 1<<3
#define HKG_RGBA                 1<<4
#define HKG_RGB                  1<<5
#define HKG_BGRA                 1<<6
#define HKG_BGR                  1<<7
#define HKG_UNSIGNED_BYTE        1<<8
#define HKG_UNSIGNED_SHORT565    1<<9
#define HKG_UNSIGNED_SHORT1555   1<<10
#define HKG_RGB565               1<<11
#define HKG_RGBA1555             1<<12

#endif //HK_GRAPHICS_TEXTURE_DEFINES__H

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
