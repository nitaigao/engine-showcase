/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_GRAPHICS_VERTEX_SET_DEFINES__H
#define HK_GRAPHICS_VERTEX_SET_DEFINES__H

typedef hkUint16 HKG_VERTEX_COMPONENT;
#define HKG_VERTEX_COMPONENT_POS					 0
#define HKG_VERTEX_COMPONENT_COLOR					 1
#define HKG_VERTEX_COMPONENT_TEX0					 2
#define HKG_VERTEX_COMPONENT_TEX1					 3
#define HKG_VERTEX_COMPONENT_TEX2					 4
#define HKG_VERTEX_COMPONENT_TEX3					 5
#define HKG_VERTEX_COMPONENT_NORMAL					 6
#define HKG_VERTEX_COMPONENT_TANGENT				 7
#define HKG_VERTEX_COMPONENT_BITANGENT				 8 // usually known as the binormal.
#define HKG_VERTEX_COMPONENT_BLENDWEIGHTS			 9
#define HKG_VERTEX_COMPONENT_BLENDINDICES			10
#define HKG_VERTEX_COMPONENT_POINTSPRITESIZE		11
#define HKG_VERTEX_COMPONENT_INSTANCED_USER_DATA	12
#define HKG_VERTEX_COMPONENT_NUM					13

typedef hkUint16 HKG_VERTEX_FORMAT;
#define HKG_VERTEX_FORMAT_NONE	   	      0    // Error state really
#define HKG_VERTEX_FORMAT_POS		      (1<<HKG_VERTEX_COMPONENT_POS)						// has pos
#define HKG_VERTEX_FORMAT_COLOR           (1<<HKG_VERTEX_COMPONENT_COLOR)					// has color
#define HKG_VERTEX_FORMAT_TEX0            (1<<HKG_VERTEX_COMPONENT_TEX0)					// has tex coords for stage 0
#define HKG_VERTEX_FORMAT_TEX1            (1<<HKG_VERTEX_COMPONENT_TEX1)					// has tex coords for stage 1
#define HKG_VERTEX_FORMAT_TEX2            (1<<HKG_VERTEX_COMPONENT_TEX2)					// has tex coords for stage 2
#define HKG_VERTEX_FORMAT_TEX3            (1<<HKG_VERTEX_COMPONENT_TEX3)					// has tex coords for stage 3
#define HKG_VERTEX_FORMAT_NORMAL	      (1<<HKG_VERTEX_COMPONENT_NORMAL)					// has normal
#define HKG_VERTEX_FORMAT_TANGENT		  (1<<HKG_VERTEX_COMPONENT_TANGENT)					// has tangent
#define HKG_VERTEX_FORMAT_BITANGENT		  (1<<HKG_VERTEX_COMPONENT_BITANGENT)				// has bitangent (aka binormal)
#define HKG_VERTEX_FORMAT_BLENDWEIGHTS    (1<<HKG_VERTEX_COMPONENT_BLENDWEIGHTS)			// has bone/blend weights
#define HKG_VERTEX_FORMAT_BLENDINDICES    (1<<HKG_VERTEX_COMPONENT_BLENDINDICES)			// has blend indices (to world blend matrices)
#define HKG_VERTEX_FORMAT_POINTSPRITESIZE (1<<HKG_VERTEX_COMPONENT_POINTSPRITESIZE)			// size of point sprite (particle effects)

#define HKG_VERTEX_FORMAT_INSTANCED_USER_DATA (1<<HKG_VERTEX_COMPONENT_INSTANCED_USER_DATA)	// additional per-instance data

typedef hkUint8 HKG_LOCK_FLAG;
#define HKG_LOCK_NONE  0  
#define HKG_LOCK_READWRITE  1
#define HKG_LOCK_DEFAULT    1
#define HKG_LOCK_READONLY     2 // will only read elements
#define HKG_LOCK_WRITEONLY    4 // will only write (some) elements
#define HKG_LOCK_WRITEDISCARD 8 // will write to all elements, so no need for previous vert state to be fetched etc

#define HKG_LOCK_READMASK (HKG_LOCK_READWRITE | HKG_LOCK_READONLY)
#define HKG_LOCK_WRITEMASK (HKG_LOCK_WRITEDISCARD | HKG_LOCK_WRITEONLY | HKG_LOCK_READWRITE)

typedef hkUint8 HKG_VERTEX_STORAGE;
#define HKG_VERTEX_STORAGE_NONE  0  
#define HKG_VERTEX_STORAGE_4UB   1     // 4 unsigned bytes (color only)
#define HKG_VERTEX_STORAGE_1F    2  
#define HKG_VERTEX_STORAGE_2F    3  // 2 floats (common for UV coords, for Tex0-3)
#define HKG_VERTEX_STORAGE_3F    4  // 3 floats (Normals or Pos), and weights
#define HKG_VERTEX_STORAGE_4F    5  // Color in float form
#define HKG_VERTEX_STORAGE_8UB   6  // 8 unsigned bytes for blending weights on PSP
#define HKG_VERTEX_STORAGE_NUM   (HKG_VERTEX_STORAGE_8UB + 1)

#define HK_VERTEX_DEFAULT_INSTANCING_TCOORD_OFFSET 5 //5,6,7 
#define HK_VERTEX_DEFAULT_TANGENTS_TCOORD_OFFSET   3 //3,4
#define HK_VERTEX_DEFAULT_TEXTURE_TCOORD_OFFSET   0 //0,1,2 (TEX3 will start to interfere with tangents (push them up on tcoord) etc.)
#define HK_VERTEX_DEFAULT_WEIGHTS_TCOORD_OFFSET   5 //5,6 for weights and indices (can share with instances as don't instance skins (at the mo anyway, and prob doesn't make sense unless they are all in the same pose anyway)

#endif //HK_GRAPHICS_VERTEX_SET_DEFINES__H

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
