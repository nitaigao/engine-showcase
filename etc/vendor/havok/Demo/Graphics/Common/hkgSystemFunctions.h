/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_GRAPHICS_SYSTEM_FUNCTIONS__H
#define HK_GRAPHICS_SYSTEM_FUNCTIONS__H


struct hkgSystemCreateFuncs
{
	void* m_hkgDisplayWorldCreate;
	void* m_hkgGeometryCreate; 
	void* m_hkgDisplayObjectCreate; 
	void* m_hkgLightManagerCreate; 
	void* m_hkgMaterialFaceSetCreate; 
	void* m_hkgMaterialManagerCreate; 
	void* m_hkgShaderCollectionCreate; 
	void* m_hkgShaderContextCreate; 
	void* m_hkgSkyBoxCreate; 
	
	
	void* m_hkgCameraCreate; 
	void* m_hkgDisplayContextCreate; 
	void* m_hkgFaceSetCreate; 
	void* m_hkgFontCreate; 
	void* m_hkgLightCreate; 
	void* m_hkgMaterialCreate; 
	void* m_hkgTextureCreate; 
	void* m_hkgVertexSetCreate; 
	void* m_hkgViewportCreate; 
	void* m_hkgShaderCreateVertexShader; 
	void* m_hkgShaderCreatePixelShader; 
	void* m_hkgBlendMatrixSetCreate; 
	void* m_hkgInstancedDisplayObjectCreate;  
	void* m_hkgParticleDisplayObjectCreate; 
	void* m_hkgCubemapTextureCreate; 
	
	void* m_hkgWindowCreate; 

	int m_maxNumLights;
};

void HK_CALL hkgSystemSetCreateFunctions( const hkgSystemCreateFuncs& funcs );
void HK_CALL hkgSystemGetCreateFunctions( hkgSystemCreateFuncs& funcs );

#endif

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
