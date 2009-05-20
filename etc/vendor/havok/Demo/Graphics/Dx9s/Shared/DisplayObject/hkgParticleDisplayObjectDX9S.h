/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


#ifndef HK_GRAPHICS_PARTICLE_DISPLAY_OBJECT_DX9S__H
#define HK_GRAPHICS_PARTICLE_DISPLAY_OBJECT_DX9S__H

#include <Graphics/Common/DisplayObject/hkgParticleDisplayObject.h>
#include <Graphics/Dx9s/Shared/DisplayContext/hkgDisplayContextDX9S.h>

class hkgVertexShaderDX9S;
class hkgPixelShaderDX9S;

class hkgParticleDisplayObjectDX9S : public hkgParticleDisplayObject
{
	public:

		static hkgParticleDisplayObject* HK_CALL createParticleDisplayObjectDX9S( hkgDisplayContext* ctx );

		hkgParticleDisplayObjectDX9S( hkgDisplayContext* ctx );

		virtual ~hkgParticleDisplayObjectDX9S();

		virtual void setMaxNumParticles( int numParticles, bool allocateBuffers = true );
		
		virtual void setPosition( const float* position, int objectIdx );
		virtual const float* getPosition( int objectIdx ) const;

		virtual void setVelocity( const float* vel, int objectIdx );
		virtual const float* getVelocity( int objectIdx ) const;

		virtual void setUserData( const float* data, int objectIdx );
		virtual const float* getUserData( int objectIdx ) const;

		virtual void render(hkgDisplayContext* context) const;

			// If you provide a Texture aswell as a VB, the renderer will assume
			// you want to do ATI style R2B (stream from the texture)
		void setPositionArray( LPDIRECT3DVERTEXBUFFER9 vb, LPDIRECT3DTEXTURE9 r2VBTex = HK_NULL );
		void setVelocityArray( LPDIRECT3DVERTEXBUFFER9 vb, LPDIRECT3DTEXTURE9 r2VBTex = HK_NULL ); 
		void setUserDataArray( LPDIRECT3DVERTEXBUFFER9 vb, LPDIRECT3DTEXTURE9 r2VBTex = HK_NULL ); 

		inline bool doingR2VB() const { return m_posTexR2VB || m_velTexR2VB || m_userTexR2VB; }
		
	protected:

		void connectR2VB() const;
		void disconnectR2VB() const;

		void renderParticles( hkgDisplayContext* context, hkgVertexShaderDX9S* currentVertexProgram, hkgPixelShaderDX9S* currentFragmentProgram) const;
		void renderParticlesPointSprites( hkgDisplayContext* context, hkgVertexShaderDX9S* currentVertexProgram, hkgPixelShaderDX9S* currentFragmentProgram ) const;
		void renderParticlesQuads( hkgDisplayContext* context, hkgVertexShaderDX9S* currentVertexProgram, hkgPixelShaderDX9S* currentFragmentProgram ) const;

		LPDIRECT3DDEVICE9 m_device;

		LPDIRECT3DVERTEXDECLARATION9 m_posTexDecl;
		LPDIRECT3DVERTEXDECLARATION9 m_posTexVelDecl;

		mutable LPDIRECT3DINDEXBUFFER9 m_pQuadIB;
		int m_numIndicesInQuadIB;

		mutable hkArray<float> m_pendingPositions;
		mutable hkArray<float> m_pendingVelocities;
		mutable hkArray<float> m_pendingUserData;

		IDirect3DVertexBuffer9 * m_cpuPositions;
		IDirect3DVertexBuffer9 * m_cpuVelocities;
		IDirect3DVertexBuffer9 * m_cpuUserData;

		bool m_userVBOs;
		LPDIRECT3DVERTEXBUFFER9 m_posVB;
		LPDIRECT3DVERTEXBUFFER9 m_velVB;
		LPDIRECT3DVERTEXBUFFER9 m_userVB;		

		//render to vb objects
		LPDIRECT3DTEXTURE9 m_posTexR2VB;
		LPDIRECT3DTEXTURE9 m_velTexR2VB;
		LPDIRECT3DTEXTURE9 m_userTexR2VB;
};

#endif // HK_GRAPHICS_PARTICLE_DISPLAY_OBJECT_DX9S__H

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
