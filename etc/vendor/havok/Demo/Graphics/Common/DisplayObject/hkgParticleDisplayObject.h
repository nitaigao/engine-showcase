/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_GRAPHICS_PARTICLE_DISPLAY_OBJECT_H
#define HK_GRAPHICS_PARTICLE_DISPLAY_OBJECT_H

#include <Graphics/Common/DisplayObject/hkgDisplayObject.h>
#include <Graphics/Common/DisplayObject/hkgParticleDisplayObjectDefines.h>



class hkgParticleDisplayObject : public hkgDisplayObject
{
	public:

		static hkgParticleDisplayObject* (HK_CALL *create)( hkgDisplayContext* ctx );

		virtual hkgDisplayObject* copy( HKG_DISPLAY_OBJECT_COPY_FLAGS f, hkgDisplayContext* ctx /*only required if doing deeper copies (facesets and/or verts)*/ );
	
		virtual void setMaxNumParticles( int numParticles, bool allocateBuffers = false ) = 0;

		virtual void setPosition( const float* position, int objectIdx ) = 0;
		virtual const float* getPosition( int objectIdx ) const = 0;

		virtual void setVelocity( const float* vel, int objectIdx ) = 0;
		virtual const float* getVelocity( int objectIdx ) const = 0;

		virtual void setUserData( const float* data, int objectIdx ) = 0;
		virtual const float* getUserData( int objectIdx ) const = 0;

		virtual void setNumParticles( int numParticles );
		virtual void setStartIndex( int index );
		inline int getNumParticles() const { return m_currentNumParticles; }
		inline int getStartIndex() const { return m_startIndex; }

		void setMaterial( hkgMaterial* m );
		inline hkgMaterial* getMaterial() { return m_material; }
		inline const hkgMaterial* getMaterial() const { return m_material; }

		inline void setRenderMode( HKG_PARTICLE_DISPLAY_MODE m ) { m_renderMode = m; }
		inline HKG_PARTICLE_DISPLAY_MODE getRenderMode() const { return m_renderMode; }

		inline void setBlendMode( HKG_PARTICLE_BLEND_MODE m ) { m_blendMode = m; }
		inline HKG_PARTICLE_BLEND_MODE getBlendMode() const { return m_blendMode; }

		inline void setParticlePointSize(float size) { m_particlePointSize = size; }
		inline void setParticleTime( float t ) { m_particleTime = t; }
		inline void setParticleLifeTime( float t ) { m_particleLifeTime = t; }

		inline hkUlong getUserId() const { return m_userId; }
		inline void setUserId(hkUlong id) { m_userId = id; }

	protected:

		hkgParticleDisplayObject();

		virtual ~hkgParticleDisplayObject();
		
		int m_maxNumParticles;
		int m_startIndex;
		int m_currentNumParticles;
		float m_particlePointSize;
		float m_particleTime;
		float m_particleLifeTime;
		HKG_PARTICLE_BLEND_MODE m_blendMode; 
		HKG_PARTICLE_DISPLAY_MODE m_renderMode;
		hkgMaterial* m_material; // we don't store any geom (there will be no hkgGeometry) so instead we just store a material
		hkUlong m_userId;
};

#endif // HK_GRAPHICS_PARTICLE_DISPLAY_OBJECT_H


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
