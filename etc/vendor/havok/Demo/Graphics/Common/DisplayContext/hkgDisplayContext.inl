/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

inline HKG_ENABLED_STATE  hkgDisplayContext::getEnabledState() const
{
	return m_currentState;
}

inline HKG_VERTEX_OPTIONS hkgDisplayContext::getVertexOptions() const
{
	return m_vertexOptions;
}

inline void hkgDisplayContext::setVertexOptions( HKG_VERTEX_OPTIONS o )
{
	m_vertexOptions = o;
}

inline void  hkgDisplayContext::setCurrentViewport(hkgViewport* v)
{
	m_currentViewport = v; // not worth referencing as amanged all together by window anyway
}

inline hkgViewport* hkgDisplayContext::getCurrentViewport() const
{
	return m_currentViewport;
}

inline hkgTexture* hkgDisplayContext::getCurrentTexture(int stage) const
{
	return m_currentBoundTextures[stage];
}

inline hkgMaterial* hkgDisplayContext::getCurrentMaterial() const
{
	return m_currentBoundMaterial;
}

inline HKG_MATERIAL_VERTEX_HINT hkgDisplayContext::getCurrentMaterialHints() const
{
	return m_currentBoundMaterialHints;
}

inline hkgShaderContext* hkgDisplayContext::getShaderContext() const
{
	return m_shaderContext;
}

inline hkgShader* hkgDisplayContext::getCurrentVertexShader() const
{
	return m_currentBoundVertexShader;
}

inline hkgShader* hkgDisplayContext::getCurrentPixelShader() const
{
	return m_currentBoundPixelShader;
}

inline hkgBlendMatrixSet* hkgDisplayContext::getCurrentBlendMatrices() const
{
	return m_currentBlendMatrices;
}

inline hkgVertexSet* hkgDisplayContext::getCurrentVertexSet() const
{
	return m_currentVertexSet;
}

inline const hkgWindow* hkgDisplayContext::getOwner() const
{
	return m_owner;
}
inline hkgWindow* hkgDisplayContext::getOwner()
{
	return m_owner;
}

inline HKG_CULLFACE_MODE hkgDisplayContext::getCullfaceMode() const
{
	return m_cullfaceMode;
}

inline void hkgDisplayContext::setCullfaceMode( HKG_CULLFACE_MODE m )
{
	m_cullfaceMode = m;

	if (m_currentState & HKG_ENABLED_CULLFACE) 
		setCullFaceState(true); // to reflect the change in mode.
}

inline HKG_BLEND_MODE hkgDisplayContext::getBlendMode() const
{
	return m_blendMode;
}

inline void hkgDisplayContext::setBlendMode( HKG_BLEND_MODE m )
{
	m_blendMode = m;

	if (m_currentState & HKG_ENABLED_ALPHABLEND ) 
		setBlendState(true); // to reflect the change in mode.
}

inline HKG_COLOR_MODE hkgDisplayContext::getColorMode() const
{
	return m_colorMode;
}

inline void hkgDisplayContext::setColorMode( HKG_COLOR_MODE c )
{
	// color mode change invalidates current material set
	setCurrentMaterial(HK_NULL);

	m_colorMode = c;
}

inline HKG_DEBUG_FLAGS hkgDisplayContext::getDebugFlags() const
{
	return m_debugFlags;
}

inline void hkgDisplayContext::setDebugFlags( HKG_DEBUG_FLAGS d )
{
	m_debugFlags = d;
}

inline HKG_RENDER_PASS_INFO hkgDisplayContext::getRenderPassInfo() const
{
	return m_renderPass;
}

inline void hkgDisplayContext::setRenderPassInfo( HKG_RENDER_PASS_INFO p )
{
	m_renderPass = p;
}

inline HKG_TEXTURE_STAGE_LOCK_MODE hkgDisplayContext::getTextureStageLockMode() const
{
	return m_textureStageLock;
}

inline void hkgDisplayContext::setTextureStageLockMode( HKG_TEXTURE_STAGE_LOCK_MODE l )
{
	m_textureStageLock = l;
}

inline void hkgDisplayContext::registerTexture(hkgTexture* t) const
{
	if (m_texturePalette.indexOf(t) == -1)
	{
		m_texturePalette.pushBack(t);
	}
}

inline bool hkgDisplayContext::isRegisteredTexture(hkgTexture* t) const
{
	return (m_texturePalette.indexOf(t) != -1);
}

inline void hkgDisplayContext::unregisterTexture(hkgTexture* t) const
{
	int i = m_texturePalette.indexOf(t);
	if (i >= 0)
	{
		m_texturePalette.removeAt(i);
	}
}


inline void hkgDisplayContext::registerVertexSet(hkgVertexSet* t) const
{
	if (m_vertexSets.indexOf(t) == -1)
	{
		m_vertexSets.pushBack(t);
	}
}

inline bool hkgDisplayContext::isRegisteredVertexSet(hkgVertexSet* t) const
{
	return (m_vertexSets.indexOf(t) != -1);
}

inline void hkgDisplayContext::unregisterVertexSet(hkgVertexSet* t) const
{
	int i = m_vertexSets.indexOf(t);
	if (i >= 0)
	{
		m_vertexSets.removeAt(i);
	}
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
