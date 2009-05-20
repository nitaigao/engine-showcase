/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_GRAPHICS_SHADER_CONTEXT
#define HK_GRAPHICS_SHADER_CONTEXT

#include <Graphics/Common/Material/hkgMaterial.h>
#include <Graphics/Common/Light/hkgLightManager.h>

/// A context for shaders to run in under HKG. It buffers and precomputes 
/// camera matrices, current materials etc. 
#define HKG_MAX_SHADER_LIGHTS 8

class hkgShaderContext : public hkgReferencedObject
{
public:
	
	static hkgShaderContext* (HK_CALL* create)(class hkgDisplayContext* owner); 
	
		/// Default create function, use ::create instead.
	inline static hkgShaderContext* defaultCreateInternal(hkgDisplayContext* owner);

		/// Current world matrix
	inline void setWorldMatrix(float* wm); 
	inline const float* getWorldMatrix() const;

		/// Current view matrix (device ready, so post any Z flips etc)
	inline void setViewMatrix(float* wm); 
	inline const float* getViewMatrix() const;
	
		/// Current proj matrix (device ready, so post any change in normalized dc size etc)
	inline void setProjMatrix(float* wm); 
	inline const float* getProjMatrix() const;

		/// To allow access to the current material properties
	inline void setCurrentMaterial(class hkgMaterial* m); // can be null
	inline const hkgMaterial* getCurrentMaterial() const;

		/// To allow access to the current lights, scene ambients etc
	inline void setLightManager(class hkgLightManager* m); // can be null
	inline const hkgLightManager* getLightManager() const;

		/// Given a shader, will look at all its inputs and set the required data for it.
	void feedShader( class hkgShader* s, bool onlyFeedDirty);

protected:		
	
	inline hkgShaderContext(hkgDisplayContext* owner);
	virtual ~hkgShaderContext();

	void recomputeMatrix( HKG_SHADER_INPUT_CONSTANT mat );

/*  HKG_SHADER_MAT_INPUT_WORLD, HKG_SHADER_MAT_INPUT_WORLD_INV, HKG_SHADER_MAT_INPUT_WORLD_INV_TRANSPOSE */
	HK_ALIGN16( float m_world[16] ); // not recomputed but set externally
	float m_worldInv[16];
	float m_worldInvTranspose[16];

/*	HKG_SHADER_MAT_INPUT_VIEW, HKG_SHADER_MAT_INPUT_VIEW_INV, HKG_SHADER_MAT_INPUT_VIEW_INV_TRANSPOSE */
	float m_view[16]; 
	float m_viewInv[16];
	float m_viewInvTranspose[16];

/*  HKG_SHADER_MAT_INPUT_PROJ, HKG_SHADER_MAT_INPUT_PROJ_INV, HKG_SHADER_MAT_INPUT_PROJ_INV_TRANSPOSE */
	float m_proj[16]; 
	float m_projInv[16];
	float m_projInvTranspose[16];

/*  HKG_SHADER_MAT_INPUT_WV, HKG_SHADER_MAT_INPUT_WV_INV, HKG_SHADER_MAT_INPUT_WV_INV_TRANSPOSE */
	float m_worldView[16];
	float m_worldViewInv[16];
	float m_worldViewInvTranspose[16];

/*	HKG_SHADER_MAT_INPUT_VP, HKG_SHADER_MAT_INPUT_VP_INV, HKG_SHADER_MAT_INPUT_VP_INV_TRANSPOSE	*/
	float m_viewProj[16];
	float m_viewProjInv[16];
	float m_viewProjInvTranspose[16];

/*  HKG_SHADER_MAT_INPUT_WVP, HKG_SHADER_MAT_INPUT_WVP_INV, HKG_SHADER_MAT_INPUT_WVP_INV_TRANSPOSE */
	float m_worldViewProj[16];
	float m_worldViewProjInv[16];
	float m_worldViewProjInvTranspose[16];

	hkgDisplayContext* m_owner;
	hkgMaterial* m_currentMaterial;
	hkgLightManager* m_lightManager;

	HKG_SHADER_MAT_INPUT_CONSTANT m_cleanMatrices;

	float m_lastAmbient[4];
	float m_lastDiffuse[4];
	float m_lastSpecular[4];
	float m_lastSpecularPower;

};

#include <Graphics/Common/Shader/hkgShaderContext.inl>

#endif //HK_GRAPHICS_SHADER_CONTEXT

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
