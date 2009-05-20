/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_GRAPHICS_SHADER_DEFINES__H
#define HK_GRAPHICS_SHADER_DEFINES__H

// two types of shaders, pixel and vertex, although the input data
// in terms of the graphics engine constants is the same for both
typedef unsigned short HKG_SHADER_TYPE;
#define HKG_VERTEX_SHADER 1
#define HKG_PIXEL_SHADER 2
#define HKG_GEOMETRY_SHADER 3

// semantics for inputs (imply sizes)
typedef unsigned int HKG_SHADER_MAT_INPUT_CONSTANT;
#define HKG_SHADER_MAT_INPUT_NONE					0
#define HKG_SHADER_MAT_INPUT_VIEW					(1<<0)
#define HKG_SHADER_MAT_INPUT_VIEW_INV				(1<<1)
#define HKG_SHADER_MAT_INPUT_VIEW_INV_TRANSPOSE		(1<<2)

#define HKG_SHADER_MAT_INPUT_PROJ					(1<<3)
#define HKG_SHADER_MAT_INPUT_PROJ_INV				(1<<4)
#define HKG_SHADER_MAT_INPUT_PROJ_INV_TRANSPOSE		(1<<5)

#define HKG_SHADER_MAT_INPUT_WV						(1<<6) // WORLD * VIEW
#define HKG_SHADER_MAT_INPUT_WV_INV					(1<<7)
#define HKG_SHADER_MAT_INPUT_WV_INV_TRANSPOSE		(1<<8)

#define HKG_SHADER_MAT_INPUT_VP						(1<<9) // VIEW * PROJ (hkgCamera 'camera' matrix)
#define HKG_SHADER_MAT_INPUT_VP_INV					(1<<10)
#define HKG_SHADER_MAT_INPUT_VP_INV_TRANSPOSE		(1<<11)

#define HKG_SHADER_MAT_INPUT_WVP					(1<<12) // WORLD * VIEW * PROJ 
#define HKG_SHADER_MAT_INPUT_WVP_INV				(1<<13) 
#define HKG_SHADER_MAT_INPUT_WVP_INV_TRANSPOSE		(1<<14) 

#define HKG_SHADER_MAT_INPUT_WORLD					(1<<15)
#define HKG_SHADER_MAT_INPUT_WORLD_INV				(1<<16) 
#define HKG_SHADER_MAT_INPUT_WORLD_INV_TRANSPOSE	(1<<17)

#define HKG_SHADER_MAT_INPUT_VIEW_TO_LIGHTPROJ		(1<<18) // for shadow map projection (includes texture coord offsets)
#define HKG_SHADER_MAT_INPUT_4x4					(1<<19) // generic 4x4 matrix. Use setFloatInputIndex with known index to set.
#define HKG_SHADER_MAT_INPUT_3x4					(1<<20) // generic 3x4 matrix. Use setFloatInputIndex with known index to set.


typedef unsigned int HKG_SHADER_INPUT_CONSTANT;

#define HKG_SHADER_INPUT_NONE					0
#define HKG_SHADER_INPUT_VECTOR					(1<<0) // generic vector. Use setFloatInputIndex with known index to set.

#define HKG_SHADER_INPUT_AMBIENT_COLOR			(1<<1) // ambient (material + scene) color
#define HKG_SHADER_INPUT_DIFFUSE_COLOR			(1<<2) // diffuse material color
#define HKG_SHADER_INPUT_SPECULAR_COLOR			(1<<3) // specular material color
#define HKG_SHADER_INPUT_SPECULAR_POWER			(1<<4) // specular power exponent for material 

#define HKG_SHADER_INPUT_SHADOWMAP_SIZE			(1<<5) // dimensions of the current shadow map (for no hw based shadow bilerp)
#define HKG_SHADER_INPUT_SHADOWMAP_DISTANCE     (1<<6) // dimensions of the current shadow map (for no hw based shadow bilerp)
#define HKG_SHADER_INPUT_SHADOWMAP_VSM_BIAS     (1<<7)
#define HKG_SHADER_INPUT_SHADOWMAP_VSM_EPSILON  (1<<8)

#define HKG_SHADER_INPUT_LIGHT_POS				(1<<9) // assumes that light pos or dir, if seen indicates a new light.
#define HKG_SHADER_INPUT_LIGHT_DIR				(1<<10) 
#define HKG_SHADER_INPUT_LIGHT_COLOR			(1<<11) // assumes spec == diffuse color
#define HKG_SHADER_INPUT_LIGHT_SPOT			 	(1<<12) 
#define HKG_SHADER_INPUT_LIGHT_SHADOW_START     (1<<13)

#define HKG_SHADER_INPUT_NUM_LIGHTS				(1<<14) // the current active num lights

#define HKG_SHADER_CAMERA_MASK					(0x00007FFF) // first 15 bits
#define HKG_SHADER_WVP_MASK					    (0x0003FFFF) // first 18 bits
#define HKG_SHADER_MATRIX_MASK					(0x0003FFFF | HKG_SHADER_INPUT_MAT_4x4 | HKG_SHADER_INPUT_MAT_3x4)  // first 18 bits or generic matrix

#define HKG_SHADER_INPUT_MATERIAL_MASK			(HKG_SHADER_INPUT_AMBIENT_COLOR | HKG_SHADER_INPUT_DIFFUSE_COLOR | HKG_SHADER_INPUT_SPECULAR_COLOR | HKG_SHADER_INPUT_SPECULAR_POWER) 

typedef unsigned int HKG_SHADER_RENDER_STYLE;
#define HKG_SHADER_RENDER_NOSTYLE 0
#define HKG_SHADER_RENDER_1LIGHTS 1
#define HKG_SHADER_RENDER_2LIGHTS 2
#define HKG_SHADER_RENDER_3LIGHTS 3
#define HKG_SHADER_RENDER_4LIGHTS 4
#define HKG_SHADER_RENDER_5LIGHTS 5
#define HKG_SHADER_RENDER_6LIGHTS 6
#define HKG_SHADER_RENDER_7LIGHTS 7
#define HKG_SHADER_RENDER_LIGHT_MASK (0x07)

#define HKG_SHADER_RENDER_MODULATE_TEXTURE0  (1<<4)
#define HKG_SHADER_RENDER_MODULATE_TEXTURE1  (1<<5)
#define HKG_SHADER_RENDER_MODULATE_TEXTURE2  (1<<6)
#define HKG_SHADER_RENDER_MODULATE_TEXTURE3  (1<<7)
#define HKG_SHADER_RENDER_DECAL_TEXTURE0	 (1<<8)
#define HKG_SHADER_RENDER_DECAL_TEXTURE1	 (1<<9)
#define HKG_SHADER_RENDER_DECAL_TEXTURE2	 (1<<10)
#define HKG_SHADER_RENDER_DECAL_TEXTURE3	 (1<<11)
#define HKG_SHADER_RENDER_TEXTURE_MASK		 (0x0FF<<4)

#define HKG_SHADER_RENDER_VERTEX_COLOR      (1<<12) // per vertex color source supported
#define HKG_SHADER_RENDER_BLENDING          (1<<13) // per vertex weights and indices supported
#define HKG_SHADER_RENDER_TANGENTS          (1<<14) // per vertex tangent and binormal supported
#define HKG_SHADER_RENDER_MATERIAL_HINT_MASK (0x07 << 12) // all the above 3 flags

#define HKG_SHADER_RENDER_INSTANCING_USING_TCOORDS (1<<15) // shader can handle transform as set via tcoords (for instancing)
#define HKG_SHADER_RENDER_TO_DEPTHMAP			(1<<16) // can do first pass of shadow map render
#define HKG_SHADER_RENDER_TO_DEPTHMAP_VSM		(1<<17) // can do first pass of shadow map render, stores variance aswell as a normalized depth
#define HKG_SHADER_RENDER_BASED_ON_DEPTHMAP		(1<<18) // assumes it has to do PCF (precentage closer filtering) itself and r/q
#define HKG_SHADER_RENDER_BASED_ON_DEPTHMAP_HW	(1<<19) // assumes it h/w will do r/q etc for it and the p.c.f.
#define HKG_SHADER_RENDER_BASED_ON_DEPTHMAP_VSM	(1<<20) // assumes it has a two component texture, meant for Variance Shadow Maps

#endif // HK_GRAPHICS_SHADER_DEFINES__H

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
