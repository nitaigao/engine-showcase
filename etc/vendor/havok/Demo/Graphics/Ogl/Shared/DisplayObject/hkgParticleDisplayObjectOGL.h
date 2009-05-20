/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


#ifndef HK_GRAPHICS_PARTICLE_DISPLAY_OBJECT_OGL__H
#define HK_GRAPHICS_PARTICLE_DISPLAY_OBJECT_OGL__H

#include <Graphics/Common/DisplayObject/hkgParticleDisplayObject.h>

#define HKG_PARTICLE_OGL_POS_ATTRIB 0
#define HKG_PARTICLE_OGL_USER_ATTRIB 8
#define HKG_PARTICLE_OGL_VEL_ATTRIB 9

class hkgParticleDisplayObjectOGL : public hkgParticleDisplayObject
{
	public:

		static hkgParticleDisplayObject* HK_CALL createParticleDisplayObjectOGL( hkgDisplayContext* ctx );

		hkgParticleDisplayObjectOGL( );

		virtual ~hkgParticleDisplayObjectOGL();

		virtual void setMaxNumParticles( int numParticles, bool allocateBuffers = false );
		
		virtual void render(hkgDisplayContext* context) const;

		virtual void setPosition( const float* position, int objectIdx );
		virtual const float* getPosition( int objectIdx ) const;

		virtual void setVelocity( const float* vel, int objectIdx );
		virtual const float* getVelocity( int objectIdx ) const;

		virtual void setUserData( const float* data, int objectIdx );
		virtual const float* getUserData( int objectIdx ) const;

		void setElementTypeAndSize( unsigned int type, unsigned int elementSize); 
		void setPositionArray( unsigned int vb );
		void setVelocityArray( unsigned int vb ); 
		void setUserDataArray( unsigned int vb ); 
		
	protected:

		void renderParticles( hkgDisplayContext* context, CGprogram currentVertexProgram, CGprogram currentFragmentProgram) const;
		void renderParticlesPointSprites( hkgDisplayContext* context, CGprogram currentVertexProgram, CGprogram currentFragmentProgram ) const;
		void renderParticlesQuads( hkgDisplayContext* context, CGprogram currentVertexProgram, CGprogram currentFragmentProgram ) const;

		mutable unsigned int m_elementType;
		mutable unsigned int m_elementSize;

		hkArray<float> m_cpuPositions;
		hkArray<float> m_cpuVelocities;
		hkArray<float> m_cpuUserData;

		void updateVBOs() const;

		mutable bool m_vboDirty;
		GLuint m_ownPosVB;
		GLuint m_ownVelVB;
		GLuint m_ownUserVB;

		bool m_userVBOs;
		GLuint m_posVB;
		GLuint m_velVB;
		GLuint m_userVB;		
};

#endif // HK_GRAPHICS_PARTICLE_DISPLAY_OBJECT_OGL__H


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
