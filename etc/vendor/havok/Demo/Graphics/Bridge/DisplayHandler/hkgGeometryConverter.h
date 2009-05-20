/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


#ifndef HK_GRAPHICS_GEOMETRY_CONVERTER_H
#define HK_GRAPHICS_GEOMETRY_CONVERTER_H

#include <Common/Base/Types/Geometry/hkGeometry.h>

class hkgGeometry;
class hkDisplaySphere;
class hkDisplayGeometry;
class hkgDisplayContext;
class hkgGeometryConverter
{
	public:

		static hkgGeometry* HK_CALL createGraphicsDisplay(hkDisplayGeometry* geom, hkgDisplayContext* context, hkTransform* texWTransform = HK_NULL, float textureMapScale = 1.0f, bool fixedTexcoords = false);

		static hkgGeometry* HK_CALL createSmoothGraphicsDisplay(hkGeometry* geom, hkgDisplayContext* context, hkTransform* texWTransform = HK_NULL, float textureMapScale = 1.0f, bool fixedTexcoords = false);


			// helpers
		static hkgGeometry* HK_CALL createGraphicsDisplay(hkGeometry* geom, hkgDisplayContext* context, hkTransform* texWTransform = HK_NULL, float textureMapScale = 1.0f, bool fixedTexcoords = false);
		static hkgGeometry* HK_CALL createGraphicsDisplay(hkDisplaySphere* geom, hkgDisplayContext* context);

		static void HK_CALL computeBoxTextureCoords( const hkVector4& pos, const hkVector4& norm, float &u, float &v, float textureMapScale);

};



#endif //  HK_GRAPHICS_GEOMETRY_CONVERTER_H

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
