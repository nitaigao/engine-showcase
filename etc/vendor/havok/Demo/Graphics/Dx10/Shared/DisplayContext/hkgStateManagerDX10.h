/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_GRAPHICS_STATE_MANAGER_DX10_H
#define HK_GRAPHICS_STATE_MANAGER_DX10_H

#include <Graphics/Common/Window/hkgWindow.h>

#define HKG_STATE_MANAGER_MAX_DESC 8
	
class hkgShaderDX10;

class hkgStateManagerDX10 : public hkgReferencedObject
{
	friend hkgDisplayContextDX10;

public:

		/// Get the DX10 Input Layout for a given vertex layout and shader combo
	ID3D10InputLayout* getInputLayout( D3D10_INPUT_ELEMENT_DESC* vertexLayout, hkUint32 numVertexComponents, hkgShaderDX10* shader );									
	void removeShader( hkgShaderDX10* s );

	/// Get the DX10 Texture sampler state for a give HKG texture state (shared between textures)
	ID3D10SamplerState* getSamplerState( const hkgTexture* texture );
	
	/// State objects
	ID3D10RasterizerState* getRasterState( const HKG_ENABLED_STATE state, const HKG_CULLFACE_MODE cullfaceMode);
	ID3D10DepthStencilState* getDepthStencilState( const HKG_ENABLED_STATE state);
	ID3D10BlendState* getBlendState( const HKG_ENABLED_STATE state, const HKG_BLEND_MODE blendMode );

protected:

	hkgStateManagerDX10(hkgDisplayContextDX10* owner);
	virtual ~hkgStateManagerDX10();

	hkgDisplayContextDX10* m_owner;

	struct InputLayouts
	{
		D3D10_INPUT_ELEMENT_DESC m_ed[HKG_STATE_MANAGER_MAX_DESC];
		hkUint32 m_edLen;
		hkgShaderDX10* m_shader;
		ID3D10InputLayout* m_il;
	};

	hkArray<InputLayouts> m_inputLayouts;

	hkArray<ID3D10SamplerState*> m_textureSamplers;

	hkArray<ID3D10RasterizerState*> m_rasterStates;
	hkArray<ID3D10DepthStencilState*> m_depthStencilStates;
	hkArray<ID3D10BlendState*> m_blendStates;
};

#include <Graphics/Dx10/Shared/DisplayContext/hkgStateManagerDX10.inl>

#endif //HK_GRAPHICS_STATE_MANAGER_DX10_H
	

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
