/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_GRAPHICS_CAMERA_DX9_H
#define HK_GRAPHICS_CAMERA_DX9_H

#include <Graphics/Common/Camera/hkgCamera.h>

class hkgCameraDX9 : public hkgCamera
{
public:

	static hkgCamera* createCameraDX9();	

	virtual void setAsCurrent(hkgDisplayContext* state);

	virtual float* getPlatformViewMatrix() { computeViewMatrixDX(); return m_platformView; }
	virtual float* getPlatformProjectionMatrix() { computeProjMatrixDX(); return m_platformProj; }

	static void setMatrices( hkgDisplayContext* state, const float* view, const float* proj );

protected:

	void computeViewMatrixDX();
	void computeProjMatrixDX();

	inline hkgCameraDX9( const float* from, const float* to, const float* up, float fov);
	virtual ~hkgCameraDX9() {}

	float m_platformView[16];
	float m_platformProj[16];
};

#include <Graphics/Dx9/Shared/Camera/hkgCameraDX9.inl>

#endif // HK_GRAPHICS_CAMERA_DX9_H


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
