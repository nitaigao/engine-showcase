/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_GRAPHICS_POST_EFFECT_DX9_H
#define HK_GRAPHICS_POST_EFFECT_DX9_H

#include <Graphics/Common/Shader/hkgShader.h>

/// TODO: Generalize and add to the HKG base for all platforms
/// here just so we can have FX file based post processed (Glow, sepia tone etc) files
class hkgPostEffectDX9 : public hkgReferencedObject
{
public:
		/// Create a blank, platform specific, shader object in the current context as given.
	static hkgPostEffectDX9* create(hkgDisplayContext* context)
	{
		return new hkgPostEffectDX9(context);
	}	
	
	void setName(const char* n);
	const char* getName() const;

		/// Create the data for the shader. Will compile the shader from the given file. Assumes it is a .fx file 
		/// Call free to release data
	bool realize(hkIstream& istream, class hkgWindow* owner);	

	bool applyPreScene(); // returns if did the clear of the buffers

	bool applyPostScene();
	
		/// Free the platform specific resources etc used by the shader. Call realize again 
		/// if you want to use the shader after this.
	void free();	

	void invalidateDevice();
	void restoreDevice();

protected:		
	
	bool createResources();
	bool executePreScript(const char* str); //DXSAS script
	bool executePostScript(const char* str); //DXSAS script
	bool executePassScript(const char* str);
	bool apply(bool post);

	hkgPostEffectDX9(hkgDisplayContext* context);
	virtual ~hkgPostEffectDX9();
		
	LPDIRECT3DDEVICE9	m_pDevice;
	LPDIRECT3DSURFACE9	m_pOrigRT;
	LPDIRECT3DSURFACE9	m_pOrigDS;
	struct ID3DXEffect*	m_pEffect;
	hkgWindow* m_owner;
	LPCSTR m_viewportParam;
	hkString m_name;
	bool m_haveChangedRT;
	bool m_haveChangedDS;
	struct TextureMap
	{
		LPCSTR name;
		LPDIRECT3DTEXTURE9 texture;
		UINT w;
		UINT h;
	};

	TextureMap* findTexture(const char* name);
	hkArray<TextureMap> m_textures;
};

#endif // HK_GRAPHICS_POST_EFFECT_DX9_H

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
