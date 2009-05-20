/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_GRAPHICS_SHADOW_UTILS__H
#define HK_GRAPHICS_SHADOW_UTILS__H

#include <Graphics/Common/hkgObject.h>
#include <Graphics/Common/Math/hkgBounds.h>
#include <Graphics/Common/Window/hkgWindowDefines.h>
#include <Graphics/Common/Camera/hkgCamera.h>

class hkgShadowUtils : public hkgReferencedObject
{
public:

	explicit hkgShadowUtils();
	virtual ~hkgShadowUtils();

		// Currently will use PSM as default
	void setMode(HKG_SHADOWMAP_MODE m);
	HKG_SHADOWMAP_MODE getMode() const { return m_mode; }
	void setFixedModeLightCamera( class hkgCamera* lightCam );

	void setCamera(class hkgCamera* viewCam, float maxShadowZ = 10);
	void setLight(class hkgLight* light);
	void setWorld(class hkgDisplayWorld* world);

	void buildProjectionMatrix();
	
	inline const float* getLightViewProj() const { return m_mLightViewProj; }
	inline const float* getLightViewProjCS() const { return m_mLightViewProjCS; }
	
	inline float getShadowMapDistance() const { return (m_mode==HKG_SHADOWMAP_MODE_FIXED? (m_fixedLightCam->getProjectionMode() == HKG_CAMERA_ORTHOGRAPHIC? m_fixedLightCam->getOrthoFar() : m_fixedLightCam->getFar() ) : m_ppFar ); }
	inline const float* getShadowMapStartLocation() const { return m_mode==HKG_SHADOWMAP_MODE_FIXED? m_fixedLightCam->getFromPtr() : m_vLightFrom; }
	
protected:

	void computeBounds();
	void buildFixedProjection();
	
	// current state
	HKG_SHADOWMAP_MODE m_mode;
	hkgDisplayWorld* m_world;
	float m_mView[16];
	float m_mProj[16];
	float m_vLightDir[3];
	float m_vLightFrom[3];
	float m_fAspect;
	float m_fNearMin;
	float m_fFarMax;
	float m_fShadowZClip;

		// fixed mode:
	hkgCamera*  m_fixedLightCam;

	bool m_bProj01; // DX style unit box of 0..1 not -1 to 1 in z
	bool m_bLeftHanded; // DX style left handed projections

	// computed state
	float m_mLightViewProj[16];
	float m_mLightViewProjCS[16];

	hkgArray<hkgAabb> m_shadowCasterAABBs;
	hkgArray<hkgAabb> m_shadowReceiverAABBs;
	hkgArray<const class hkgDisplayObject*> m_shadowCasterObjects;
	hkgArray<const class hkgDisplayObject*> m_shadowReceiverObjects;

	// computed parameters
	bool m_bShadowTestInverted;
	float m_zNear;
	float m_ppNear;
	float m_zFar;
	float m_ppFar;
	float m_fInfiniteZ;
	float m_fZOffset;
};

#endif //HK_GRAPHICS_SHADOW_UTILS__H

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
