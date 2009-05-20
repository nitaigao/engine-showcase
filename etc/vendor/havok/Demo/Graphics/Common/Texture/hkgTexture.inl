/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

hkgTexture::hkgTexture(hkgDisplayContext* owner)
	:	hkgReferencedObject(), 
		m_owner(owner),
		m_hasAlpha(false),
		m_isTransparent(false),
		m_bIsLockble(true),
		m_wantMipMaps(false),
		m_wantCompression(false),
		m_mipLevels(0),
		m_clut(HK_NULL),
		m_data(HK_NULL),
		m_width(0),
		m_height(0),
		m_depth(0),
		m_bpp(33),
		m_mapType(HKG_TEXTURE_SINGLE),
		m_uWrapMode( HKG_TEXTURE_WRAP ),
		m_vWrapMode( HKG_TEXTURE_WRAP ),
		m_coordGen(HKG_TEXTURE_COORDGEN_NONE),
		m_filterMode(HKG_TEXTURE_FILTER_LINEAR),
		m_userData(HK_NULL)
{
	m_owner->registerTexture(this);

	HKG_CHECK_CONTEXT_LOCK_STATUS(owner);
}
	
inline bool hkgTexture::hasData() const
{
	return (m_bpp!= 0) && (m_data != HK_NULL);
}

inline unsigned char* hkgTexture::getDataPointer(int mipLevel) const
{
	if (mipLevel < m_mipLevels)
	{
		int l =0; 
		int offset = 0;
		int width = m_width;
		int height= m_height;
		int bytesPerPixel = (m_bpp/8);
		while (l != mipLevel)
		{
			offset += height * width * bytesPerPixel;
			height >>= 1;
			width  >>= 1;
			if (height < 1) height = 1;
			if (width < 1) width = 1;
			l++;
		}
		return (unsigned char*)( m_data + offset );
	}
	else
	{
		return HK_NULL;
	}
}

inline const unsigned char* hkgTexture::getClutPointer() const
{
	return m_clut;
}

inline unsigned char* hkgTexture::getDataPointer()
{
	return m_data;
}

inline unsigned char* hkgTexture::getClutPointer()
{
	return m_clut;
}

inline unsigned int hkgTexture::getWidth() const
{
	return m_width;
}

inline unsigned int hkgTexture::getHeight() const
{
	return m_height;
}

inline unsigned int hkgTexture::getBPP() const
{
	return m_bpp;
}

inline bool hkgTexture::isTransparent() const
{
	return m_isTransparent;
}

inline bool hkgTexture::hasAlphaChannel() const
{
	return m_hasAlpha;
}

inline hkUlong hkgTexture::getUserData() const
{
	return m_userData;
}

inline void hkgTexture::setUserData(hkUlong d)
{
	m_userData = d;
}

inline void hkgTexture::setAutoMipMaps(bool on)
{
	m_wantMipMaps = on;
}

inline void hkgTexture::setAutoCompression(bool on)
{
	m_wantCompression = on;
}

inline bool hkgTexture::isLockable() const
{
	return m_bIsLockble;
}

inline HKG_TEXTURE_UVMODE hkgTexture::getTextureWrapModeU() const
{
	return m_uWrapMode;
}

inline HKG_TEXTURE_UVMODE hkgTexture::getTextureWrapModeV() const
{
	return m_vWrapMode;
}

inline HKG_TEXTURE_UVMODE hkgTexture::getTextureWrapModeW() const
{
	return m_wWrapMode;
}

inline void hkgTexture::setTextureWrapModeU(HKG_TEXTURE_UVMODE m)
{
	m_uWrapMode = m;
}

inline void hkgTexture::setTextureWrapModeV(HKG_TEXTURE_UVMODE m)
{
	m_vWrapMode = m;
}

inline void hkgTexture::setTextureWrapModeW(HKG_TEXTURE_UVMODE m)
{
	m_wWrapMode = m;
}

inline HKG_TEXTURE_TYPE hkgTexture::getMapType() const
{
	return m_mapType;
}

inline void hkgTexture::setCoordGen( HKG_TEXTURE_COORD_GEN g )
{
	m_coordGen = g;
}

inline HKG_TEXTURE_COORD_GEN hkgTexture::getCoordGen( ) const
{
	return m_coordGen;
}

inline void hkgTexture::setFilterMode( HKG_TEXTURE_FILTER f )
{
	m_filterMode = f;
}

inline HKG_TEXTURE_FILTER hkgTexture::getFilterMode( ) const
{
	return m_filterMode;
}

inline int hkgTexture::getNumMipLevels( ) const
{
	return m_mipLevels;
}



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
