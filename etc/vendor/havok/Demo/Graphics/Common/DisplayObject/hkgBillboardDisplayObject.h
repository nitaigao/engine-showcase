/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_GRAPHICS_BILLBOARD_DISPLAY_OBJECT_H
#define HK_GRAPHICS_BILLBOARD_DISPLAY_OBJECT_H

#include <Graphics/Common/DisplayObject/hkgDisplayObject.h>

typedef hkUint8 HKG_BILLBOARD_DISPLAY_MODE;
#define HKG_BILLBOARD_DISPLAY_SIMPLE      0
#define HKG_BILLBOARD_DISPLAY_TEXTURED    1  

typedef hkUint8 HKG_BILLBOARD_DEPTH_MODE;
#define HKG_BILLBOARD_DEPTH_IGNORE      0
#define HKG_BILLBOARD_DEPTH_CORRECT     1  

typedef hkUint8 HKG_BILLBOARD_BLEND_MODE;
#define HKG_BILLBOARD_BLEND_NONE        0  
#define HKG_BILLBOARD_BLEND_ADDITIVE    1  
#define HKG_BILLBOARD_BLEND_SUBTRACTIVE 2
#define HKG_BILLBOARD_BLEND_MODULATE    4 

typedef hkUint8 HKG_BILLBOARD_ANCHOR;
#define HKG_BILLBOARD_ANCHOR_CENTER      0
#define HKG_BILLBOARD_ANCHOR_BASE        1
#define HKG_BILLBOARD_ANCHOR_LEFT        2
#define HKG_BILLBOARD_ANCHOR_RIGHT       3
#define HKG_BILLBOARD_ANCHOR_TOP         4

typedef hkUint8 HKG_BILLBOARD_CALC_MODE;
#define HKG_BILLBOARD_CALC_CYLINDRICAL_FAST     0
#define HKG_BILLBOARD_CALC_CYLINDRICAL_CORRECT  1
#define HKG_BILLBOARD_CALC_SPHERICAL            2
#define HKG_BILLBOARD_CALC_WORLD_FIXED          3


class hkgBillboardDisplayObject : public hkgDisplayObject
{
	public:

		static hkgBillboardDisplayObject* (HK_CALL *create)( hkgDisplayContext* ctx );

		virtual hkgDisplayObject* copy( HKG_DISPLAY_OBJECT_COPY_FLAGS f, hkgDisplayContext* ctx /*only required if doing deeper copies (facesets and/or verts)*/ );

		void setPosition( const float* position );
		inline const float* getPosition( ) const { return m_position; }

		void setOrientation( const float* orient );
		inline const float* getOrientation( ) const { return m_orient; }

		void setColor( const float* color );
		inline const float* getColor( ) const { return m_color; }

		inline void setSize( const float size ) { m_size = size; }
		inline float getSize( ) const { return m_size; }

		inline void setTime( const float t ) { m_time = t; }
		inline float getTime( ) const { return m_time; }

		void setMaterial( hkgMaterial* m );
		inline hkgMaterial* getMaterial() { return m_material; }
		inline const hkgMaterial* getMaterial() const { return m_material; }

		inline void setRenderMode( HKG_BILLBOARD_DISPLAY_MODE m ) { m_renderMode = m; }
		inline HKG_BILLBOARD_DISPLAY_MODE getRenderMode() const { return m_renderMode; }

		inline void setDepthMode( HKG_BILLBOARD_DEPTH_MODE m ) { m_depthMode = m; }
		inline HKG_BILLBOARD_DEPTH_MODE getDepthMode() const { return m_depthMode; }

		inline void setBlendMode( HKG_BILLBOARD_BLEND_MODE m ) { m_blendMode = m; }
		inline HKG_BILLBOARD_BLEND_MODE getBlendMode() const { return m_blendMode; }

		inline void setAnchor( HKG_BILLBOARD_ANCHOR m ) { m_anchor = m; }
		inline HKG_BILLBOARD_ANCHOR getAnchor() const { return m_anchor; }

		inline void setCalcMode( HKG_BILLBOARD_CALC_MODE m ) { m_calcMode = m; }
		inline HKG_BILLBOARD_CALC_MODE getCalcMode() const { return m_calcMode; }

		inline hkUlong getUserId() const { return m_userId; }
		inline void setUserId(hkUlong id) { m_userId = id; }

	protected:

		hkgBillboardDisplayObject();

		virtual ~hkgBillboardDisplayObject();
		
		hkReal m_position[4];
		hkReal m_orient[9];
		hkReal m_color[4];
		hkReal m_size, m_time;
		HKG_BILLBOARD_DISPLAY_MODE m_renderMode;
		HKG_BILLBOARD_DEPTH_MODE m_depthMode;
		HKG_BILLBOARD_BLEND_MODE m_blendMode; 
		HKG_BILLBOARD_ANCHOR m_anchor;
		HKG_BILLBOARD_CALC_MODE m_calcMode;
		hkgMaterial* m_material; // we don't store any geom (there will be no hkgGeometry) so instead we just store a material
		hkUlong m_userId;
};

#endif // HK_GRAPHICS_BILLBOARD_DISPLAY_OBJECT_H

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
