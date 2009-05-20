/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_GRAPHICS_DISPLAY_OBJECT_DEFINES__H
#define HK_GRAPHICS_DISPLAY_OBJECT_DEFINES__H

typedef unsigned int HKG_DISPLAY_OBJECT_STATUS;
#define HKG_DISPLAY_OBJECT_NOFLAG  0

#define HKG_DISPLAY_OBJECT_STATIC  (1   ) // does not move (position and geom)
#define HKG_DISPLAY_OBJECT_DYNAMIC (1<<1) // object moves around with changing transform, but verts are fixed.
#define HKG_DISPLAY_OBJECT_MUTABLE (1<<2) // object geometry will kepp changing. it will recalc mutable aabbs every few frames, but leave them a bit big

#define HKG_DISPLAY_OBJECT_SHADOWCASTER (1<<3) // is meant to cast shadows
#define HKG_DISPLAY_OBJECT_SHADOWRECEIVER (1<<4)// is meant to cast shadows
#define HKG_DISPLAY_OBJECT_SHADOW (HKG_DISPLAY_OBJECT_SHADOWRECEIVER | HKG_DISPLAY_OBJECT_SHADOWCASTER)// is meant to take part in all shadow passes etc
#define HKG_DISPLAY_OBJECT_UNLIT   (1<<5) // object is not lit, so will turn off lighting if required
#define HKG_DISPLAY_OBJECT_NOT_CULLED (1<<6) // objects that should never be culled 
#define HKG_DISPLAY_OBJECT_USER_AABB  (1<<7) // objects that have a user set aabb (usually are mutable too, as normally are because they are h/w skinned)

#define HKG_DISPLAY_OBJECT_NOT_PICKABLE  (1<<8) // objects do not take part in ray cast picking

#define HKG_DISPLAY_OBJECT_NO_OVERDRAW (1<<9) // objects do not take part in wireframe overdraw passes, if one

#define HKG_DISPLAY_OBJECT_NO_AUTO_DRAW  (1<<10) // objects will never be drawn automatically, you will have to call their render methods explicitly

#define HKG_DISPLAY_OBJECT_BAKED_SCALE (1<<11) // the display object has scale baked into it

#define HKG_DISPLAY_OBJECT_INSTANCED (1<<12) // the display object is a instanced display object 
#define HKG_DISPLAY_OBJECT_PARTICLES (1<<13) // the display object is a special particle display object 
#define HKG_DISPLAY_OBJECT_BILLBOARD (1<<14) // the display object is a special billboard display object 

typedef unsigned int HKG_DISPLAY_OBJECT_COPY_FLAGS;
#define HKG_DISPLAY_OBJECT_DEEP_COPY  0      // share nothing (other than Textures ptrs, Shader ptrs, etc)
#define HKG_DISPLAY_OBJECT_SHARE_MATERIALS 1 // share the hkgMaterial ptr 
#define HKG_DISPLAY_OBJECT_SHARE_VERTS 2     // share the hkgVertexSet ptr 
#define HKG_DISPLAY_OBJECT_SHARE_FACESETS (4 | HKG_DISPLAY_OBJECT_SHARE_VERTS) // share the hkgFaceSet ptrs
#define HKG_DISPLAY_OBJECT_SHARE_MATFACESETS (8 | (HKG_DISPLAY_OBJECT_SHARE_FACESETS | HKG_DISPLAY_OBJECT_SHARE_MATERIALS)) // share the hkgMatFaceSet ptrs
#define HKG_DISPLAY_OBJECT_SHARE_GEOM (16 | HKG_DISPLAY_OBJECT_SHARE_MATFACESETS) // share the hkgGeometry ptr as a whole (common for RigidBodies say, but for cloth, SHARE_MAT only would be commmon)
#define HKG_DISPLAY_OBJECT_SHARE_WHOLEOBJECT (32 | HKG_DISPLAY_OBJECT_SHARE_GEOM ) // rare, only used in converters to get cached item perhaps

#endif // HK_GRAPHICS_DISPLAY_OBJECT_DEFINES__H

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
