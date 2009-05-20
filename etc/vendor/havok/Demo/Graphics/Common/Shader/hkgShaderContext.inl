/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

hkgShaderContext::hkgShaderContext(hkgDisplayContext* owner) 
: m_owner(owner), m_currentMaterial(HK_NULL), m_lightManager(HK_NULL), m_cleanMatrices(0)
{	
	// as no matices are clean, they will be computed as need.	
	hkgMat4Identity(m_world);
	hkgMat4Identity(m_view);	
	hkgMat4Identity(m_proj);
}

inline hkgShaderContext* hkgShaderContext::defaultCreateInternal(hkgDisplayContext* owner)
{
	return new hkgShaderContext(owner);
}

inline void hkgShaderContext::setWorldMatrix(float* wm)
{
	hkgMat4Copy( m_world, wm );
	const HKG_SHADER_MAT_INPUT_CONSTANT affectedByWorld = 
		  HKG_SHADER_MAT_INPUT_WORLD | HKG_SHADER_MAT_INPUT_WORLD_INV | HKG_SHADER_MAT_INPUT_WORLD_INV_TRANSPOSE
		| HKG_SHADER_MAT_INPUT_WV | HKG_SHADER_MAT_INPUT_WV_INV | HKG_SHADER_MAT_INPUT_WV_INV_TRANSPOSE 
		| HKG_SHADER_MAT_INPUT_WVP | HKG_SHADER_MAT_INPUT_WVP_INV | HKG_SHADER_MAT_INPUT_WVP_INV_TRANSPOSE;
	m_cleanMatrices &= ~affectedByWorld;
}

inline const float* hkgShaderContext::getWorldMatrix() const
{
	return m_world;
}

inline void hkgShaderContext::setViewMatrix(float* wm)
{
	hkgMat4Copy( m_view, wm );
	const HKG_SHADER_MAT_INPUT_CONSTANT affectedByView = 
		HKG_SHADER_MAT_INPUT_VIEW | HKG_SHADER_MAT_INPUT_VIEW_INV | HKG_SHADER_MAT_INPUT_VIEW_INV_TRANSPOSE
		| HKG_SHADER_MAT_INPUT_WV | HKG_SHADER_MAT_INPUT_WV_INV | HKG_SHADER_MAT_INPUT_WV_INV_TRANSPOSE 
		| HKG_SHADER_MAT_INPUT_VP | HKG_SHADER_MAT_INPUT_VP_INV | HKG_SHADER_MAT_INPUT_VP_INV_TRANSPOSE	
		| HKG_SHADER_MAT_INPUT_WVP | HKG_SHADER_MAT_INPUT_WVP_INV | HKG_SHADER_MAT_INPUT_WVP_INV_TRANSPOSE;
	m_cleanMatrices &= ~affectedByView;
}

inline const float* hkgShaderContext::getViewMatrix() const
{
	return m_view;
}

inline void hkgShaderContext::setProjMatrix(float* wm)
{
	hkgMat4Copy( m_proj, wm );
	const HKG_SHADER_MAT_INPUT_CONSTANT affectedByProj = 
		HKG_SHADER_MAT_INPUT_PROJ | HKG_SHADER_MAT_INPUT_PROJ_INV | HKG_SHADER_MAT_INPUT_PROJ_INV_TRANSPOSE
		| HKG_SHADER_MAT_INPUT_VP | HKG_SHADER_MAT_INPUT_VP_INV | HKG_SHADER_MAT_INPUT_VP_INV_TRANSPOSE	
		| HKG_SHADER_MAT_INPUT_WVP | HKG_SHADER_MAT_INPUT_WVP_INV | HKG_SHADER_MAT_INPUT_WVP_INV_TRANSPOSE;
	m_cleanMatrices &= ~affectedByProj;
}

inline const float* hkgShaderContext::getProjMatrix() const
{
	return m_proj;
}

inline void hkgShaderContext::setCurrentMaterial(hkgMaterial* m)
{
	if (m) m->reference();
	if (m_currentMaterial) m_currentMaterial->release();
	m_currentMaterial = m;
}

inline const hkgMaterial* hkgShaderContext::getCurrentMaterial() const
{
	return m_currentMaterial;
}

inline void hkgShaderContext::setLightManager(hkgLightManager* m)
{
	if ( m != m_lightManager )
	{
		if (m) m->reference();
		if (m_lightManager) m_lightManager->release();
		m_lightManager = m;
	}
}

inline const hkgLightManager* hkgShaderContext::getLightManager() const
{
	return m_lightManager;
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
