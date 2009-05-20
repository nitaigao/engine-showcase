/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_GRAPHICS_CUBEMAP_TEXTURE_OGLS__H
#define HK_GRAPHICS_CUBEMAP_TEXTURE_OGLS__H

#include <Graphics/Common/Texture/hkgCubemapTexture.h>
#include <Graphics/Ogls/Shared/DisplayContext/hkgDisplayContextOGLS.h>

class hkgCubemapTextureOGLS : public hkgCubemapTexture
{
public:

	static hkgCubemapTexture* createCubemapTextureOGLS(hkgDisplayContext* context)
	{
		return new hkgCubemapTextureOGLS(context);
	}	

		// assumes filename will inlcude a %s to replace with
		// posx, negx, etc
	virtual bool loadFromFile(const char* filename, void * hinstance = HK_NULL); 
	virtual bool realize(bool dynamic = false);	
	virtual void free();		

	virtual HKG_TEXTURE_PIXEL_FORMAT getPixelFormat() const;

	virtual unsigned char* lock(int mipLevel = 0, bool willWriteAllPixels = false); 
	virtual void unlock(int mipLevel = 0); 

protected:
	
	virtual void bind(int stage, HKG_TEXTURE_MODE mode) const;		
	virtual void unbind(int stage) const;		

	GLuint m__glID;

	unsigned char* m_faceData[6];

	inline hkgCubemapTextureOGLS(hkgDisplayContext* context);
	virtual ~hkgCubemapTextureOGLS();
};

#include <Graphics/Ogls/Shared/Texture/hkgCubemapTextureOGLS.inl>

#endif // HK_GRAPHICS_CUBEMAP_TEXTURE_OGLS__H


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
