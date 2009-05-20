/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

hkgMaterial::hkgMaterial()
: hkgReferencedObject(), m_shaders(HK_NULL), m_isTransparent(false), m_isDefaultMaterial(false), m_userData(HK_NULL)
{
	m_diffuse[0] = m_diffuse[1] = m_diffuse[2] = m_diffuse[3] = 1.0f;
	m_specular[0] = m_specular[1] = m_specular[2] = 0.0f;
	m_specularPower = 20; // for most spec algos this is the normal val (0 to infinity allowq
	for (int i=0; i < HKG_MAX_TEXTURE_STAGES; ++i)
		m_textureMode[i] = HKG_TEXTURE_MODULATE;
}

void hkgMaterial::setTexture(int i, hkgTexture* t)
{
	if (t)
	{
		t->reference();
	}
	
	if (m_textures[i]) // leave it to base to assert on bad i
	{
		m_textures[i]->release();
	}

	m_textures[i] = t;

	if (i == 0) // base transparent flag on t0 for the moment.
	{
		if (m_textures[0])
		{
			m_isTransparent = (m_diffuse[3] < 0.99f) || m_textures[0]->isTransparent();
		}
		else
		{
			// just need to recompute transparency
			m_isTransparent = (m_diffuse[3] < 0.99f);
		}
	}
}

void hkgMaterial::addTexture( hkgTexture* t)
{
	if (t)
	{
		t->reference();
	}
	
	m_textures.pushBack( t );

	if (m_textures.getSize() == 1) // base transparent flag on t0 for the moment.
	{
		if (m_textures[0])
		{
			m_isTransparent = (m_diffuse[3] < 0.99f) || m_textures[0]->isTransparent();
		}
		else
		{
			// just need to recompute transparency
			m_isTransparent = (m_diffuse[3] < 0.99f);
		}
	}
}

void hkgMaterial::setShaderCollection(hkgShaderCollection* s)
{
	if (s)
	{
		s->reference();
	}
	
	if (m_shaders)
	{
		m_shaders->release();
	}

	m_shaders = s;
}

const hkgTexture* hkgMaterial::getTexture(int stage) const
{
	if (m_textures.getSize() > stage)
		return m_textures[stage];
	return HK_NULL;
}

hkgTexture* hkgMaterial::getTexture(int stage)
{
	if (m_textures.getSize() > stage)
		return m_textures[stage];
	return HK_NULL;
}

int hkgMaterial::getNumTextures() const
{
	return m_textures.getSize();
}

HKG_TEXTURE_MODE hkgMaterial::getTextureMode(int stage) const
{
	return m_textureMode[stage];
}


void hkgMaterial::setTextureMode( int stage, HKG_TEXTURE_MODE m )
{
	 m_textureMode[stage] = m;
}

inline HKG_MATERIAL_TRANS_SOURCE hkgMaterial::getTransparentSource() const
{
	HKG_MATERIAL_TRANS_SOURCE s = HKG_MATERIAL_TRANS_SOURCE_NONE;

	if (m_diffuse[3] < 0.99f)
		s |= HKG_MATERIAL_TRANS_SOURCE_COLOR;

	if (m_textures.getSize() && m_textures[0]->isTransparent())
		s |= HKG_MATERIAL_TRANS_SOURCE_TEXTURE;

	return s;
}

const hkgShaderCollection* hkgMaterial::getShaderCollection() const
{
	return m_shaders;
}

hkgShaderCollection* hkgMaterial::getShaderCollection()
{
	return m_shaders;
}

inline hkUlong hkgMaterial::getUserData() const
{
	return m_userData;
}
	
inline void hkgMaterial::setUserData(hkUlong d)
{	
	m_userData = d;
}

inline void hkgMaterial::setDiffuseColor( float r, float g, float b, float a)
{
	m_diffuse[0] = r;
	m_diffuse[1] = g;
	m_diffuse[2] = b;
	m_diffuse[3] = a;

	if ( getTransparentSource() == HKG_MATERIAL_TRANS_SOURCE_COLOR)
		m_isTransparent = (a < 0.99f);
	else
		m_isTransparent	= (a < 0.99f) || (m_textures.getSize() && m_textures[0]->isTransparent());
}

inline void hkgMaterial::getDiffuseColor( float& r, float& g, float& b, float& a) const
{
	r = m_diffuse[0];
	g = m_diffuse[1];
	b = m_diffuse[2];
	a = m_diffuse[3];
}

inline const float* hkgMaterial::getDiffuseColorPtr() const
{
	return m_diffuse;
}

inline void hkgMaterial::setSpecularColor( float r, float g, float b )
{
	m_specular[0] = r;
	m_specular[1] = g;
	m_specular[2] = b;
}

inline void hkgMaterial::getSpecularColor( float& r, float& g, float& b ) const
{
	r = m_specular[0];
	g = m_specular[1];
	b = m_specular[2];
}

inline const float* hkgMaterial::getSpecularColorPtr() const
{
	return m_specular;
}

inline void hkgMaterial::setSpecularPower( float p )
{
	m_specularPower = p;
}

inline float hkgMaterial::getSpecularPower( ) const
{
	return m_specularPower;
}

inline bool hkgMaterial::isTransparent() const
{
	return m_isTransparent;
}

inline void hkgMaterial::realizeTextures()
{
	for (int i=0; i < m_textures.getSize(); ++i)
	{
		if (m_textures[i])
			m_textures[i]->realize();
	}
}

inline bool hkgMaterial::operator==( const hkgMaterial& other ) const
{
	bool colorSame = ( hkgVec3Equals( m_diffuse, other.m_diffuse, 0.01f)) // diffuse
					&& ( hkg_fabs(m_diffuse[3] - other.m_diffuse[3]) < 0.01f)  // alpha
					&& ( hkgVec3Equals( m_specular, other.m_specular, 0.01f ) ); // specular

	if (colorSame && (m_textures.getSize() == other.m_textures.getSize()) )
	{
		for (int i=0; i < m_textures.getSize(); ++i)	
		{
			if ( (m_textures[i] != other.m_textures[i]) ||
				 (m_textureMode[i]!= other.m_textureMode[i]) )
				return false;
		}

		return (other.m_shaders == m_shaders);
	}
	else return false;
}

inline hkgMaterial& hkgMaterial::operator=( const hkgMaterial& other )
{
	int i;
	for (i=0; i < other.m_textures.getSize(); ++i)
		other.m_textures[i]->reference();

	for (i=0; i < m_textures.getSize(); ++i)
		m_textures[i]->release();
	
	m_textures.setSize(other.m_textures.getSize());
	for (i=0; i < other.m_textures.getSize(); ++i)
		m_textures[i] = other.m_textures[i];

	setShaderCollection( other.m_shaders );
	
	for (i=0; i < HKG_MAX_TEXTURE_STAGES; ++i)
		m_textureMode[i] = other.m_textureMode[i];
	
	m_isDefaultMaterial = false;
	m_isTransparent = other.m_isTransparent;
	hkgVec4Copy(m_diffuse, other.m_diffuse);
	hkgVec3Copy(m_specular, other.m_specular);
	m_specularPower = other.m_specularPower;
	m_userData = other.m_userData;

	return *this;
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
