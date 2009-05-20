/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_GRAPHICS_DISPLAY_CONTEXT_DEFINES__H
#define HK_GRAPHICS_DISPLAY_CONTEXT_DEFINES__H

typedef unsigned short HKG_VERTEX_OPTIONS;
#define HKG_VTX_USE_STREAMS		  (1<<0)
#define HKG_VTX_DISABLE_TEXTURE2D (1<<1)
#define HKG_VTX_DISABLE_COLOR	  (1<<2)

typedef unsigned short HKG_ENABLED_STATE;
#define HKG_ENABLED_NONE					(0)
#define HKG_ENABLED_LIGHTING				(1<<0)
#define HKG_ENABLED_TEXTURE2D				(1<<1)
#define HKG_ENABLED_ALPHABLEND				(1<<2)
#define HKG_ENABLED_ZREAD					(1<<3)
#define HKG_ENABLED_ZWRITE					(1<<4)
#define HKG_ENABLED_CULLFACE				(1<<5) 
#define HKG_ENABLED_WIREFRAME				(1<<6)
#define HKG_ENABLED_POINTSPRITESCALING		(1<<7)
#define HKG_ENABLED_POINTSPRITETEXTURING	(1<<8)
#define HKG_ENABLED_ALL						(0xffff)

typedef unsigned short HKG_BLEND_MODE;
#define HKG_BLEND_MODULATE		(1<<0) // Standard, (SRC_APLHA, 1-SRC_ALPHA) style
#define HKG_BLEND_ADD           (1<<1) // Additive, (SRC_ALPHA, 1) style

typedef unsigned short HKG_TEXTURE_MODE;
#define HKG_TEXTURE_MODULATE	(1<<0) // Default, multiply material color with texel
#define HKG_TEXTURE_DECAL       (1<<1) // Overwrite material color with texture

typedef unsigned short HKG_CULLFACE_MODE;
#define HKG_CULLFACE_CW			(1<<0) // Clockwise in a RHS
#define HKG_CULLFACE_CCW        (1<<1) // Counter-Clockwise in a RHS, default

typedef unsigned int HKG_COLOR_MODE;
#define HKG_COLOR_MATERIAL		      (1<<0) // Get the color from the material + texture and/or vertex color
#define HKG_COLOR_GLOBAL		      (1<<1) // The color is a global, manual, setting (set by explict material binds)
#define HKG_COLOR_GLOBAL_SHADER	      (1<<2) // The current shaders etc are all global, material may or may not be
#define HKG_COLOR_GLOBAL_SHADER_COLLECTION	 (1<<3) // The current shader collection is all global, material may or may not be


typedef unsigned int HKG_TEXTURE_STAGE_LOCK_MODE;
#define HKG_TEXTURE_STAGE_ALL_UNLOCKED    0
#define HKG_TEXTURE_STAGE_LOCKED_TEXTURE0 1 // Texture x is used globally (such as a shadow map in a given slot)
#define HKG_TEXTURE_STAGE_LOCKED_TEXTURE1 2
#define HKG_TEXTURE_STAGE_LOCKED_TEXTURE2 3
#define HKG_TEXTURE_STAGE_LOCKED_TEXTURE3 4
#define HKG_TEXTURE_STAGE_LOCKED_TEXTURE4 5
#define HKG_TEXTURE_STAGE_LOCKED_TEXTURE5 6
#define HKG_TEXTURE_STAGE_LOCKED_TEXTURE6 7
#define HKG_TEXTURE_STAGE_LOCKED_TEXTURE7 8

typedef unsigned int HKG_MATERIAL_VERTEX_HINT;
#define HKG_MATERIAL_VERTEX_HINT_NONE     0  
#define HKG_MATERIAL_VERTEX_HINT_VCOLOR   1  // Per Vertex Color 
#define HKG_MATERIAL_VERTEX_HINT_TANGENTS 2  // Normal Mapping etc
#define HKG_MATERIAL_VERTEX_HINT_BLENDING 4  // Skinning


typedef unsigned short HKG_IMM_GROUP;
#define HKG_IMM_LINES           (1<<0)
#define HKG_IMM_TRIANGLE_LIST	(1<<1)
#define HKG_IMM_TRIANGLE_STRIP	(1<<2)
#define HKG_IMM_POINTS          (1<<3)

typedef unsigned short HKG_DEBUG_FLAGS;
#define HKG_DEBUG_NONE			(0)
#define HKG_DEBUG_PRIMITIVES    (1<<0)

typedef unsigned short HKG_RENDER_PASS_INFO; // allows platforms to know the current type of rendering beeing done
#define HKG_RENDER_PASS_NORMAL			(1) // normal, run of the mill, no shadow maps etc
#define HKG_RENDER_PASS_TO_DEPTHMAP		(1<<1) // rendering depth values only
#define HKG_RENDER_PASS_USING_DEPTHMAP	(1<<2) // rendering based on the shadow map results. Shadow map type can be found using hkgWindow::getShadowMapSupport()

#define HKG_MAX_TEXTURE_STAGES   8

#endif //HK_GRAPHICS_DISPLAY_CONTEXT_DEFINES__H

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
