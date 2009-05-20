/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_GRAPHICS_TEXTURE_DX10_H
#define HK_GRAPHICS_TEXTURE_DX10_H

#include <Graphics/Common/Texture/hkgTexture.h>
#include <Graphics/Dx10/Shared/DisplayContext/hkgDisplayContextDX10.h>

class hkgTextureDX10 : public hkgTexture
{

public:

	static hkgTexture* createTextureDX10(hkgDisplayContext* context)
	{
		return new hkgTextureDX10(context);
	}	

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
	
	ID3D10Texture2D*			m_texture;
	ID3D10ShaderResourceView*	m_textureShaderView;
	ID3D10Device*				m_device;

	bool m_bIsManaged;

	inline hkgTextureDX10(hkgDisplayContext* context);
	virtual ~hkgTextureDX10();

};

#include <Graphics/Dx10/Shared/Texture/hkgTextureDX10.inl>

#endif // HK_GRAPHICS_TEXTURE_DX10_H

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
