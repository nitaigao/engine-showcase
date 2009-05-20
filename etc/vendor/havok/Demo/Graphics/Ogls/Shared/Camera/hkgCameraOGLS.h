/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_GRAPHICS_CAMERA_OGLS_H
#define HK_GRAPHICS_CAMERA_OGLS_H

#include <Graphics/Common/Camera/hkgCamera.h>

class hkgCameraOGLS : public hkgCamera
{
public:

	virtual void setAsCurrent(hkgDisplayContext* state);

	static hkgCamera* createCameraOGLS() { return new hkgCameraOGLS(hkgCamera::DEFAULT_FROM, hkgCamera::DEFAULT_TO, hkgCamera::DEFAULT_UP, hkgCamera::DEFAULT_FOV); }

	static void setMatrices( hkgDisplayContext* state, const float* view, const float* proj );

protected:

	inline hkgCameraOGLS( const float* from, const float* to, const float* up, float fov);
	virtual ~hkgCameraOGLS() {}

};

#include <Graphics/Ogls/Shared/Camera/hkgCameraOGLS.inl>

#endif // HK_GRAPHICS_CAMERA_OGLS_H


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
