/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_GRAPHICS_OGL_TEXTURE_H
#define HK_GRAPHICS_OGL_TEXTURE_H

#include <Graphics/Common/Texture/hkgTexture.h>

class hkgTextureOGL : public hkgTexture
{
public:

	virtual bool realize(bool dynamic=false);
	virtual void free();		

	static hkgTexture* createTextureOGL(hkgDisplayContext* c) { return new hkgTextureOGL(c); }

	  /// lock etc are as per base class. 
	  /// unlock updates texture object.
	virtual void unlock(int mipLevel = 0); 

	virtual bool loadFromFile(const char* filename, void * hinstance);
	virtual bool loadFromDDS(hkIstream& s);

protected:
	
	virtual void bind(int stage, HKG_TEXTURE_MODE mode) const;		
	virtual void unbind(int stage) const;		
	
	bool setupFiltering();

	GLint getGLinternalFormat(HKG_DDS_FORMAT_TYPE f) const;
	bool realizeFromCompressed(DDSLoadInfo& info);

	unsigned int m_ddsSize;
	unsigned char* m_ddsData;
	inline hkgTextureOGL(hkgDisplayContext* context);
	virtual ~hkgTextureOGL();
	
	GLuint m_glID;
};

#include <Graphics/Ogl/Shared/Texture/hkgTextureOGL.inl>

#endif // HK_GRAPHICS_OGL_TEXTURE_H

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
