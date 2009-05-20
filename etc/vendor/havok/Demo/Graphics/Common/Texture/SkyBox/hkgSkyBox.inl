/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

const hkgTexture* hkgSkyBox::getTexture(HKG_SKYBOX_FACE f) const
{
	return m_textures[f];
}

hkgTexture* hkgSkyBox::getTexture(HKG_SKYBOX_FACE f)
{
	return m_textures[f];
}

void hkgSkyBox::setTexture(HKG_SKYBOX_FACE f, hkgTexture* t, unsigned int color)
{
	if (t) t->reference();
	if (m_textures[f]) m_textures[f]->release();
	m_textures[f] = t;
	m_colors[f] = color;
}

void hkgSkyBox::setUpDirection(const float* up)
{
	hkgVec3Copy(m_up, up);
	hkgVec3Normalize(m_up);
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
