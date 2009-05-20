/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_GRAPHICS_FRAMEBUFFER_OBJECT_OGLS__H
#define HK_GRAPHICS_FRAMEBUFFER_OBJECT_OGLS__H


///TODO: Make proper base class and use in D3D etc

class hkgFrameBufferOGLS : public hkgReferencedObject
{
	public:

		hkgFrameBufferOGLS();
		virtual ~hkgFrameBufferOGLS();

		void bind();

		virtual void attachRenderTarget( GLenum attachment, GLuint buffId );
		virtual void attachTexture( GLenum attachment, GLenum texType, GLuint texId, int mipLevel = 0, int zSlice = 0);

		void unbind( GLenum attachment );
		
		GLenum getAttachedType( GLenum attachment );
		GLuint getAttachedId( GLenum attachment );
		GLint  getAttachedMipLevel( GLenum attachment );
		GLint  getAttachedCubeFace( GLenum attachment );
		GLint  getAttachedZSlice( GLenum attachment );

		int getMaxColorAttachments();
		
		static void HK_CALL disableFBO();
		
	protected:

		GLuint genFBO();
		
		GLint m_maxColorAttachments;
		GLuint m_fboId;
};

#endif // HK_GRAPHICS_FRAMEBUFFER_OBJECT_OGLS__H

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
