/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_GRAPHICS_INSTANCED_DISPLAY_OBJECT_DX9S__H
#define HK_GRAPHICS_INSTANCED_DISPLAY_OBJECT_DX9S__H

#include <Graphics/Common/DisplayObject/hkgInstancedDisplayObject.h>

#include <Graphics/Dx9s/hkGraphicsDX9S.h>
#include <Graphics/Dx9s/Shared/DisplayContext/hkgDisplayContextDX9S.h>
#include <Graphics/Dx9s/Shared/Window/hkgWindowDX9S.h>

class hkgInstancedDisplayObjectDX9S : public hkgInstancedDisplayObject, public hkgWindowDX9SResetEventHandler
{
	public:

		inline static hkgInstancedDisplayObject* HK_CALL createInstancedDisplayObjectDX9S( hkgDisplayContext* ctx );

			/// special render call to make use of DirectX hardware instancing support in Shader Specification 3.0
		virtual void render( hkgDisplayContext* context ) const;

			/// special picking support
		virtual int intersect( const hkgRay& ray, bool ignoreBackfaces, hkgDisplayObjectPickData& data) const;

			/// set the total number of instanced objects, must be called BEFORE cloning or rendering ( finalises the VBO )
			/// No need to call if you are going to use replace buffers anyway
		virtual void setMaxNumObjects( int numObjects, bool allocateBuffers = true );

		virtual void setRenderMode(HKG_INSTANCED_DISPLAY_MODE mode);
		
			/// adds a shallow copy (shares hkgGeometry down) with the same transform and same flags to m_displayObjects
		virtual hkgDisplayObject* clone( float* transform = HK_NULL );

		virtual void setTransform( const float* transform, int objectIdx );

		virtual const float* getTransform( int objectIdx ) const;

			// take the transforms form an externaly created vertex buf source.
			// Will resize m_numObjects etc to suit size of rows.
			// If you provide the 3 textures it will assume you want to do 
			// ATI style R2VB (render to vertex buffer). You must provide a (dummy) VB and a Tex in that case for each row
		void replaceVertexBuffers(LPDIRECT3DVERTEXBUFFER9 row0, LPDIRECT3DVERTEXBUFFER9 row1, LPDIRECT3DVERTEXBUFFER9 row2,
								  LPDIRECT3DTEXTURE9 row0TexR2VB = HK_NULL, LPDIRECT3DTEXTURE9 row1TexR2VB = HK_NULL, LPDIRECT3DTEXTURE9 row2TexR2VB = HK_NULL);

		virtual void releaseD3D9S();
		virtual void resetD3D9S();

#ifdef HK_PLATFORM_XBOX360
		inline bool doingR2VB() const { return false; }
#else
		inline bool doingR2VB() const { return (m_instanceTransformRowTexture[0] || m_instanceTransformRowTexture[1] || m_instanceTransformRowTexture[2]) && m_enableHardwareInstancing; }
#endif

		inline bool doingHwInstancedRendering() const { return m_instanceTransformRow[0] && m_enableHardwareInstancing; }

	protected:

		inline hkgInstancedDisplayObjectDX9S( hkgDisplayContextDX9S* ctx );
		inline virtual ~hkgInstancedDisplayObjectDX9S();

		// enable DX9SScompatible hardware instancing
		bool m_enableHardwareInstancing;

		hkArray<float> m_cpuTransforms;
		mutable hkArray<float> m_pendingTransformsRow0; // for CPU update of VB driven (hw instanced) display
		mutable hkArray<float> m_pendingTransformsRow1; // for CPU update of VB driven (hw instanced) display
		mutable hkArray<float> m_pendingTransformsRow2; // for CPU update of VB driven (hw instanced) display
		mutable hkArray<float> m_pendingTransformIndices; // for CPU update of VB driven (hw instanced) display

		LPDIRECT3DDEVICE9	m_device;
		class hkgWindowDX9S* m_ownerWindow;

		// an array of floats representing the per instance ( 4 x 4 ) transforms ( gathered from the clones )
		bool m_createdOwnBuffers;
		mutable LPDIRECT3DVERTEXBUFFER9 m_instanceTransformRow[3];
		mutable LPDIRECT3DTEXTURE9		m_instanceTransformRowTexture[3];
		mutable LPDIRECT3DTEXTURE9		m_instanceTransformRowTextureSystem[3];
};

#include <Graphics/Dx9s/Shared/DisplayObject/hkgInstancedDisplayObjectDX9S.inl>

#endif // HK_GRAPHICS_INSTANCED_DISPLAY_OBJECT_DX9S__H


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
