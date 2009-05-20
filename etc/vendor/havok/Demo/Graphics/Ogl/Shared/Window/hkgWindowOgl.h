/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_GRAPHICS_WINDOW_OGL_H
#define HK_GRAPHICS_WINDOW_OGL_H

#include <Graphics/Common/Window/hkgWindow.h>

#if defined(HK_PLATFORM_UNIX) && !defined(__glext_h_)
typedef int GLsizeiptrARB;
#endif

// For non-Win32 platforms
#ifndef APIENTRY
#define APIENTRY
#endif
#ifndef APIENTRYP
#define APIENTRYP APIENTRY *
#endif
#ifndef GLAPI
#define GLAPI extern
#endif

// support VBO functions on Win32
#define GL_ARRAY_BUFFER_ARB 0x8892
#define GL_STATIC_DRAW_ARB 0x88E4

typedef void (APIENTRY * PFNGLBINDBUFFERARBPROC) (GLenum target, GLuint buffer);
typedef void (APIENTRY * PFNGLDELETEBUFFERSARBPROC) (GLsizei n, const GLuint *buffers);
typedef void (APIENTRY * PFNGLGENBUFFERSARBPROC) (GLsizei n, GLuint *buffers);
typedef void (APIENTRY* PFNGLBUFFERDATAARBPROC) (GLenum target, GLsizeiptrARB size, const GLvoid *data, GLenum usage);
typedef GLvoid* (APIENTRY* PFNGLMAPBUFFERPROC) (GLenum target, GLenum access);
typedef GLboolean (APIENTRY* PFNGLUNMAPBUFFERPROC) (GLenum target);

typedef void (APIENTRY* PFNGLENABLEVERTEXATTRIBARRAYARBPROC) (GLuint index);
typedef void (APIENTRY* PFNGLDISABLEVERTEXATTRIBARRAYARBPROC) (GLuint index);
typedef void (APIENTRY* PFNGLVERTEXATTRIBPOINTERARBPROC) (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer);

typedef void (APIENTRY* PFNGLMULTITEXCOORD4FVARBPROC) (GLenum target, const GLfloat *v);
typedef void (APIENTRY* PFNGLMULTITEXCOORD4FARBPROC) (GLenum target, GLfloat s, GLfloat t, GLfloat r, GLfloat q);
typedef void (APIENTRY* PFNGLACTIVETEXTUREARBPROC) (GLenum texture);
typedef void (APIENTRY* PFNGLCLIENTACTIVETEXTUREARBPROC) (GLenum texture);

extern PFNGLGENBUFFERSARBPROC _glGenBuffersARB;
extern PFNGLBINDBUFFERARBPROC _glBindBufferARB;
extern PFNGLBUFFERDATAARBPROC _glBufferDataARB;
extern PFNGLDELETEBUFFERSARBPROC _glDeleteBuffersARB;
extern PFNGLMAPBUFFERPROC _glMapBufferARB;
extern PFNGLUNMAPBUFFERPROC _glUnmapBufferARB;

extern PFNGLENABLEVERTEXATTRIBARRAYARBPROC _glEnableVertexAttribArrayARB;
extern PFNGLDISABLEVERTEXATTRIBARRAYARBPROC _glDisableVertexAttribArrayARB;
extern PFNGLVERTEXATTRIBPOINTERARBPROC _glVertexAttribPointerARB;

extern PFNGLMULTITEXCOORD4FVARBPROC _glMultiTexCoord4fvARB;
extern PFNGLMULTITEXCOORD4FARBPROC _glMultiTexCoord4fARB;
extern PFNGLACTIVETEXTUREARBPROC _glActiveTextureARB;
extern PFNGLCLIENTACTIVETEXTUREARBPROC _glClientActiveTextureARB;

// GL_ARB_framebuffer_object
typedef GLboolean (APIENTRY * PFNGLISRENDERBUFFEREXTPROC) (GLuint renderbuffer);
typedef void (APIENTRY * PFNGLBINDRENDERBUFFEREXTPROC) (GLenum target, GLuint renderbuffer);
typedef void (APIENTRY * PFNGLDELETERENDERBUFFERSEXTPROC) (GLsizei n, const GLuint *renderbuffers);
typedef void (APIENTRY * PFNGLGENRENDERBUFFERSEXTPROC) (GLsizei n, GLuint *renderbuffers);
typedef void (APIENTRY * PFNGLRENDERBUFFEERSTORAGEEXTPROC) (GLenum target, GLenum internalformat, GLsizei w, GLsizei h);
typedef void (APIENTRY * PFNGLGETRENDERBUFFERPARAMETERIVEXTPROC) (GLenum target, GLenum pname, GLint* params);
typedef GLboolean (APIENTRY * PFNGLISFRAMEBUFFEREXTPROC) ( GLuint framebuffer);
typedef void (APIENTRY * PFNGLBINDFRAMEBUFFEREXTPROC) (GLenum target, GLuint framebuffer);
typedef void (APIENTRY * PFNGLDELETEFRAMEBUFFERSEXTPROC) (GLsizei n, const GLuint *framebuffers);
typedef void (APIENTRY * PFNGLGENFRAMEBUFFERSEXTPROC) (GLsizei n, GLuint *framebuffers);
typedef GLenum (APIENTRY * PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC) (GLenum target);
typedef void (APIENTRY * PFNGLFRAMEBUFFERTEXTURE1DEXTPROC) (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
typedef void (APIENTRY * PFNGLFRAMEBUFFERTEXTURE2DEXTPROC) (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
typedef void (APIENTRY * PFNGLFRAMEBUFFERTEXTURE2DEXTPROC) (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
typedef void (APIENTRY * PFNGLFRAMEBUFFERTEXTURE3DEXTPROC) (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset);
typedef void (APIENTRY * PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC) (GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
typedef void (APIENTRY * PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVEXTPROC) (GLenum target, GLenum attachment, GLenum pname, GLint *params);

typedef void (APIENTRY * PFNGLGENERATEMIPMAPEXTPROC) (GLenum target);
typedef void (APIENTRY * PFNGLPOINTPARAMETERFEXTPROC) (GLenum pname, GLfloat param );

extern PFNGLISRENDERBUFFEREXTPROC          _glIsRenderbufferEXT;
extern PFNGLBINDRENDERBUFFEREXTPROC        _glBindRenderbufferEXT;
extern PFNGLDELETERENDERBUFFERSEXTPROC     _glDeleteRenderbuffersEXT;
extern PFNGLGENRENDERBUFFERSEXTPROC        _glGenRenderbuffersEXT;
extern PFNGLRENDERBUFFEERSTORAGEEXTPROC    _glRenderbufferStorageEXT;
extern PFNGLGETRENDERBUFFERPARAMETERIVEXTPROC _glGetRenderbufferParameterivEXT;
extern PFNGLISFRAMEBUFFEREXTPROC           _glIsFramebufferEXT;
extern PFNGLBINDFRAMEBUFFEREXTPROC         _glBindFramebufferEXT;
extern PFNGLDELETEFRAMEBUFFERSEXTPROC      _glDeleteFramebuffersEXT;
extern PFNGLGENFRAMEBUFFERSEXTPROC         _glGenFramebuffersEXT;
extern PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC  _glCheckFramebufferStatusEXT;
extern PFNGLFRAMEBUFFERTEXTURE1DEXTPROC    _glFramebufferTexture1DEXT;
extern PFNGLFRAMEBUFFERTEXTURE2DEXTPROC    _glFramebufferTexture2DEXT;
extern PFNGLFRAMEBUFFERTEXTURE3DEXTPROC    _glFramebufferTexture3DEXT;
extern PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC _glFramebufferRenderbufferEXT;
extern PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVEXTPROC _glGetFramebufferAttachmentParameterivEXT;
extern PFNGLGENERATEMIPMAPEXTPROC          _glGenerateMipmapEXT;

#if !(defined(HK_PLATFORM_MACPPC) || defined(HK_PLATFORM_MAC386))
extern PFNGLDRAWBUFFERSATIPROC             _glDrawBuffersARB;
#endif

extern PFNGLPOINTPARAMETERFEXTPROC         _glPointParameterfEXT;  // make point sprites fast on pbuffers

// GL_NVX_instanced_arrays
typedef void (APIENTRY * PFNGLVERTEXATTRIBDIVISORNVXPROC) (GLuint attrib, GLuint divisor);
typedef void (APIENTRY * PFNGLDRAWARRAYSINSTANCEDNVXPROC) (GLenum mode, GLint start, GLsizei count, GLsizei primCount);
typedef void (APIENTRY * PFNGLDRAWELEMENTSINSTANCEDNVXPROC) (GLenum mode, GLsizei count, GLenum type, const GLvoid *indices, GLsizei primCount);

extern PFNGLVERTEXATTRIBDIVISORNVXPROC _glVertexAttribDivisorNVX;
extern PFNGLDRAWARRAYSINSTANCEDNVXPROC _glDrawArraysInstancedNVX;
extern PFNGLDRAWELEMENTSINSTANCEDNVXPROC _glDrawElementsInstancedNVX;

// 3D textures
typedef void (APIENTRY * PFNGLTEXIMAGE3DPROC) (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
extern PFNGLTEXIMAGE3DPROC _glTexImage3D;

/* ------------------------- GL_NV_occlusion_query ------------------------- */

#define GL_PIXEL_COUNTER_BITS_NV 0x8864
#define GL_CURRENT_OCCLUSION_QUERY_ID_NV 0x8865
#define GL_PIXEL_COUNT_NV 0x8866
#define GL_PIXEL_COUNT_AVAILABLE_NV 0x8867

typedef void (APIENTRY * PFNGLBEGINOCCLUSIONQUERYNVPROC) (GLuint id);
typedef void (APIENTRY * PFNGLDELETEOCCLUSIONQUERIESNVPROC) (GLsizei n, const GLuint* ids);
typedef void (APIENTRY * PFNGLENDOCCLUSIONQUERYNVPROC) (void);
typedef void (APIENTRY * PFNGLGENOCCLUSIONQUERIESNVPROC) (GLsizei n, GLuint* ids);
typedef void (APIENTRY * PFNGLGETOCCLUSIONQUERYIVNVPROC) (GLuint id, GLenum pname, GLint* params);
typedef void (APIENTRY * PFNGLGETOCCLUSIONQUERYUIVNVPROC) (GLuint id, GLenum pname, GLuint* params);
typedef GLboolean (APIENTRY * PFNGLISOCCLUSIONQUERYNVPROC) (GLuint id);

extern PFNGLBEGINOCCLUSIONQUERYNVPROC _glBeginOcclusionQueryNV;
extern PFNGLDELETEOCCLUSIONQUERIESNVPROC _glDeleteOcclusionQueriesNV;
extern PFNGLENDOCCLUSIONQUERYNVPROC _glEndOcclusionQueryNV;
extern PFNGLGENOCCLUSIONQUERIESNVPROC _glGenOcclusionQueriesNV;
extern PFNGLGETOCCLUSIONQUERYIVNVPROC _glGetOcclusionQueryivNV;
extern PFNGLGETOCCLUSIONQUERYUIVNVPROC _glGetOcclusionQueryuivNV;
extern PFNGLISOCCLUSIONQUERYNVPROC _glIsOcclusionQueryNV;

/* -------------------- GL_EXT_texture_compression_s3tc -------------------- */

#define GL_COMPRESSED_RGB_S3TC_DXT1_EXT 0x83F0
#define GL_COMPRESSED_RGBA_S3TC_DXT1_EXT 0x83F1
#define GL_COMPRESSED_RGBA_S3TC_DXT3_EXT 0x83F2
#define GL_COMPRESSED_RGBA_S3TC_DXT5_EXT 0x83F3

typedef void (APIENTRY * PFNGLCOMPRESSEDTEXIMAGE2DPROC) (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid * data);
typedef void (APIENTRY * PFNGLCOMPRESSEDTEXIMAGE3DPROC) (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const GLvoid * data);
extern PFNGLCOMPRESSEDTEXIMAGE2DPROC _glCompressedTexImage2D;
extern PFNGLCOMPRESSEDTEXIMAGE3DPROC _glCompressedTexImage3D;

#include <Graphics/Ogl/Shared/Window/hkgWindowCgFunctionsOGL.h>

class hkgWindowOgl : public hkgWindow
{
	public:
		hkgWindowOgl();
		virtual ~hkgWindowOgl();

		virtual void* getProcAddress(const char* proc) { return HK_NULL; }

		virtual int getMaxTextureBlendStages();
		virtual bool supportsShaderCompilation() const { return _cgCreateProgramFromFile != HK_NULL; }

			// Set function pointers of supported functions.
			// This will fail on most platforms if a gl window has not been created.
		virtual void setupExtensionFunctions();

			// This will fail on most platforms if a gl window has not been created.
		static hkBool isExtensionSupported(const char* ext);

		virtual hkUint32 vertexShaderMinorVersion() const;
		virtual hkUint32 vertexShaderMajorVersion() const; 
		virtual hkUint32 pixelShaderMinorVersion() const;
		virtual hkUint32 pixelShaderMajorVersion() const;

		virtual bool saveFrontFrameBufferToBMP(const char* filename);

		static hkBool hasCompressedTextureSupport;

	protected:

		unsigned char* m_framebuffer; ///
		int m_framebufferSize;
};




#endif //HK_GRAPHICS_WINDOW_OGL_H


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
