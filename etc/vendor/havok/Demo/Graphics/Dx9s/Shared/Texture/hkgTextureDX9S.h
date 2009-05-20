/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_GRAPHICS_TEXTURE_DX9S_H
#define HK_GRAPHICS_TEXTURE_DX9S_H

#include <Graphics/Common/Texture/hkgTexture.h>
#include <Graphics/Dx9s/Shared/DisplayContext/hkgDisplayContextDX9S.h>

class hkgTextureDX9S : public hkgTexture
{
public:

	static hkgTexture* createTextureDX9S(hkgDisplayContext* context)
	{
		return new hkgTextureDX9S(context);
	}	

	virtual bool loadFromFile(const char* filename, void * hinstance = HK_NULL); 
	virtual bool loadFromDDS(hkIstream& s);
	virtual bool realize(bool dynamic = false);	
	virtual void free();		

	virtual HKG_TEXTURE_PIXEL_FORMAT getPixelFormat() const;
	virtual int getRowByteLength(int mipLevel = 0); 
	virtual unsigned char* lock(int mipLevel = 0, bool willWriteAllPixels = false); 
	virtual void unlock(int mipLevel = 0); 

	inline bool isManaged() { return m_bIsManaged; }

protected:
	
	virtual void bind(int stage, HKG_TEXTURE_MODE mode) const;		
	virtual void unbind(int stage) const;		
	bool compressTexture();

	LPDIRECT3DTEXTURE9	m_texture; // one of the two is in use at a time
	LPDIRECT3DVOLUMETEXTURE9 m_volumeTexture;
	LPDIRECT3DDEVICE9	m_device;
	bool m_bIsManaged;
	bool m_bSupportsAniso;

	unsigned int m_ddsSize;
	inline hkgTextureDX9S(hkgDisplayContext* context);
	virtual ~hkgTextureDX9S();

};

#include <Graphics/Dx9s/Shared/Texture/hkgTextureDX9S.inl>

#endif // HK_GRAPHICS_TEXTURE_DX9S_H

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
