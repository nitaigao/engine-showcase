/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_GRAPHICS_PIXEL_SHADER_DX9S_H
#define HK_GRAPHICS_PIXEL_SHADER_DX9S_H

#include <Graphics/Common/Shader/hkgShader.h>

class hkgPixelShaderDX9S : public hkgShader
{
public:
		/// Create a blank, platform specific, shader object in the current context as given.
	static hkgShader* createPixelShaderDX9S(hkgDisplayContext* context)
	{
		return new hkgPixelShaderDX9S(context);
	}	
	
	virtual const char* getDefaultFileNameExtension() { return ".hlsl"; }

		/// Create the platform specific data for the shader. Will compile the shader from the given file.
	virtual bool realize(hkIstream& istream, HKG_SHADER_RENDER_STYLE style);	

	virtual bool realizeCompileFromFile(const char* filename, const char* entryPoint, HKG_SHADER_RENDER_STYLE style, const char* includePath = HK_NULL, const char** defines = HK_NULL);	
	virtual bool realizeCompile( hkIstream& s, const char* entryPoint, HKG_SHADER_RENDER_STYLE style, const char* includePath = HK_NULL, const char** defines = HK_NULL);
		
		/// Bind the data for the shader, set it as the current active shader program
	virtual bool bind();	

		/// Unbind from the shader
	virtual bool unbind();	

		/// Free the platform specific resources etc used by the shader. Call realize again 
		/// if you want to use the shader after this.
	virtual void free();	

		/// Direct access to the device etc
	inline LPDIRECT3DDEVICE9 getDevice();

	virtual void setFloatInputIndex( int i, const float* v );

	void setFloatInputByName( const char* n, const float* v );

protected:		
	
	inline hkgPixelShaderDX9S(hkgDisplayContext* context);
	virtual ~hkgPixelShaderDX9S();
		
	bool m_bound;
	LPDIRECT3DDEVICE9	m_pDevice;
	struct ID3DXConstantTable* m_pConstantTable;
	IDirect3DPixelShader9* m_pPShader;
};

#include <Graphics/Dx9s/Shared/Shader/hkgPixelShaderDX9S.inl>

#endif //HK_GRAPHICS_PIXEL_SHADER_DX9S_H

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
