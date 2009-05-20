/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_GRAPHICS_SKYBOX_H
#define HK_GRAPHICS_SKYBOX_H

#include <Common/Base/Types/hkBaseTypes.h>
#include <Graphics/Common/Texture/hkgTexture.h>
#include <Graphics/Common/Texture/SkyBox/hkgSkyBoxDefines.h>


class hkgSkyBox : public hkgReferencedObject
{
	public:
			/// Create a platform specific sky box object.
		static hkgSkyBox* (HK_CALL *create)();

			/// Default create function, use ::create instead.
		static hkgSkyBox* defaultCreateInternal();

			/// Set the textures to be used by the sky box.
			/// Optional as they can be solid color sometimes, use (null,color) then.
			/// To never draw a face, use (null, 0x00******) as alpha is not allowed. 
		inline void setTexture(HKG_SKYBOX_FACE f, hkgTexture* t, unsigned int color); 
		inline const hkgTexture* getTexture(HKG_SKYBOX_FACE f) const;
		inline hkgTexture* getTexture(HKG_SKYBOX_FACE f);
		
		inline void setUpDirection(const float* up);

		void render( class hkgDisplayContext* context, class hkgCamera* camera);

	protected:

		hkgSkyBox();
		virtual ~hkgSkyBox();

		hkgCamera*		m_skyBoxCam;
		hkgTexture*		m_textures[6];
		unsigned int    m_colors[6];

		float			m_up[3];
};

#include <Graphics/Common/Texture/SkyBox/hkgSkyBox.inl>

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
