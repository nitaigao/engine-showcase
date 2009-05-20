/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_GRAPHICS_WINDOW_OGLS_H
#define HK_GRAPHICS_WINDOW_OGLS_H

#include <Graphics/Common/Window/hkgWindow.h>

#ifdef HK_PLATFORM_UNIX
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

#ifdef HK_PLATFORM_PS3_PPU
#define GL_SIZEI_PTR GLsizeiptr 
#else
#define GL_SIZEI_PTR GLsizeiptrARB 
#endif

typedef void (APIENTRY * PFNGLBINDBUFFERARBPROC) (GLenum target, GLuint buffer);
typedef void (APIENTRY * PFNGLDELETEBUFFERSARBPROC) (GLsizei n, const GLuint *buffers);
typedef void (APIENTRY * PFNGLGENBUFFERSARBPROC) (GLsizei n, GLuint *buffers);
typedef void (APIENTRY* PFNGLBUFFERDATAARBPROC) (GLenum target, GL_SIZEI_PTR size, const GLvoid *data, GLenum usage);
typedef GLvoid* (APIENTRY* PFNGLMAPBUFFERPROC) (GLenum target, GLenum access);
typedef GLboolean (APIENTRY* PFNGLUNMAPBUFFERPROC) (GLenum target);

typedef void (APIENTRY* PFNGLENABLEVERTEXATTRIBARRAYARBPROC) (GLuint index);
typedef void (APIENTRY* PFNGLDISABLEVERTEXATTRIBARRAYARBPROC) (GLuint index);
typedef void (APIENTRY* PFNGLVERTEXATTRIBPOINTERARBPROC) (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer);

typedef void (APIENTRY* PFNGLMULTITEXCOORD4FVARBPROC) (GLenum target, const GLfloat *v);
typedef void (APIENTRY* PFNGLMULTITEXCOORD4FARBPROC) (GLenum target, GLfloat s, GLfloat t, GLfloat r, GLfloat q);
typedef void (APIENTRY* PFNGLACTIVETEXTUREARBPROC) (GLenum texture);
typedef void (APIENTRY* PFNGLCLIENTACTIVETEXTUREARBPROC) (GLenum texture);

extern PFNGLGENBUFFERSARBPROC __glGenBuffersARB;
extern PFNGLBINDBUFFERARBPROC __glBindBufferARB;
extern PFNGLBUFFERDATAARBPROC __glBufferDataARB;
extern PFNGLDELETEBUFFERSARBPROC __glDeleteBuffersARB;
extern PFNGLMAPBUFFERPROC __glMapBufferARB;
extern PFNGLUNMAPBUFFERPROC __glUnmapBufferARB;

extern PFNGLENABLEVERTEXATTRIBARRAYARBPROC __glEnableVertexAttribArrayARB;
extern PFNGLDISABLEVERTEXATTRIBARRAYARBPROC __glDisableVertexAttribArrayARB;
extern PFNGLVERTEXATTRIBPOINTERARBPROC __glVertexAttribPointerARB;

extern PFNGLMULTITEXCOORD4FVARBPROC __glMultiTexCoord4fvARB;
extern PFNGLMULTITEXCOORD4FARBPROC __glMultiTexCoord4fARB;
extern PFNGLACTIVETEXTUREARBPROC __glActiveTextureARB;
extern PFNGLCLIENTACTIVETEXTUREARBPROC __glClientActiveTextureARB;

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

typedef void (APIENTRY * PFNGLDRAWBUFFERSARBPROC)(GLsizei n, const GLenum *bufs);
typedef void (APIENTRY * PFNGLGENERATEMIPMAPEXTPROC) (GLenum target);
typedef void (APIENTRY * PFNGLPOINTPARAMETERFEXTPROC) (GLenum pname, GLfloat param );

extern PFNGLISRENDERBUFFEREXTPROC          __glIsRenderbufferEXT;
extern PFNGLBINDRENDERBUFFEREXTPROC        __glBindRenderbufferEXT;
extern PFNGLDELETERENDERBUFFERSEXTPROC     __glDeleteRenderbuffersEXT;
extern PFNGLGENRENDERBUFFERSEXTPROC        __glGenRenderbuffersEXT;
extern PFNGLRENDERBUFFEERSTORAGEEXTPROC    __glRenderbufferStorageEXT;
extern PFNGLGETRENDERBUFFERPARAMETERIVEXTPROC __glGetRenderbufferParameterivEXT;
extern PFNGLISFRAMEBUFFEREXTPROC           __glIsFramebufferEXT;
extern PFNGLBINDFRAMEBUFFEREXTPROC         __glBindFramebufferEXT;
extern PFNGLDELETEFRAMEBUFFERSEXTPROC      __glDeleteFramebuffersEXT;
extern PFNGLGENFRAMEBUFFERSEXTPROC         __glGenFramebuffersEXT;
extern PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC  __glCheckFramebufferStatusEXT;
extern PFNGLFRAMEBUFFERTEXTURE1DEXTPROC    __glFramebufferTexture1DEXT;
extern PFNGLFRAMEBUFFERTEXTURE2DEXTPROC    __glFramebufferTexture2DEXT;
extern PFNGLFRAMEBUFFERTEXTURE3DEXTPROC    __glFramebufferTexture3DEXT;
extern PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC __glFramebufferRenderbufferEXT;
extern PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVEXTPROC __glGetFramebufferAttachmentParameterivEXT;
extern PFNGLGENERATEMIPMAPEXTPROC          __glGenerateMipmapEXT;

extern PFNGLDRAWBUFFERSARBPROC             __glDrawBuffersARB;
extern PFNGLPOINTPARAMETERFEXTPROC         __glPointParameterfEXT;  // make point sprites fast on pbuffers

// GL_NVX_instanced_arrays
typedef void (APIENTRY * PFNGLVERTEXATTRIBDIVISORNVXPROC) (GLuint attrib, GLuint divisor);
typedef void (APIENTRY * PFNGLDRAWARRAYSINSTANCEDNVXPROC) (GLenum mode, GLint start, GLsizei count, GLsizei primCount);
typedef void (APIENTRY * PFNGLDRAWELEMENTSINSTANCEDNVXPROC) (GLenum mode, GLsizei count, GLenum type, const GLvoid *indices, GLsizei primCount);

extern PFNGLVERTEXATTRIBDIVISORNVXPROC __glVertexAttribDivisorNVX;
extern PFNGLDRAWARRAYSINSTANCEDNVXPROC __glDrawArraysInstancedNVX;
extern PFNGLDRAWELEMENTSINSTANCEDNVXPROC __glDrawElementsInstancedNVX;

// 3D textures
typedef void (APIENTRY * PFNGLTEXIMAGE3DPROC) (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
extern PFNGLTEXIMAGE3DPROC __glTexImage3D;

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

extern PFNGLBEGINOCCLUSIONQUERYNVPROC __glBeginOcclusionQueryNV;
extern PFNGLDELETEOCCLUSIONQUERIESNVPROC __glDeleteOcclusionQueriesNV;
extern PFNGLENDOCCLUSIONQUERYNVPROC __glEndOcclusionQueryNV;
extern PFNGLGENOCCLUSIONQUERIESNVPROC __glGenOcclusionQueriesNV;
extern PFNGLGETOCCLUSIONQUERYIVNVPROC __glGetOcclusionQueryivNV;
extern PFNGLGETOCCLUSIONQUERYUIVNVPROC __glGetOcclusionQueryuivNV;
extern PFNGLISOCCLUSIONQUERYNVPROC __glIsOcclusionQueryNV;

/* -------------------- GL_EXT_texture_compression_s3tc -------------------- */

#define GL_COMPRESSED_RGB_S3TC_DXT1_EXT 0x83F0
#define GL_COMPRESSED_RGBA_S3TC_DXT1_EXT 0x83F1
#define GL_COMPRESSED_RGBA_S3TC_DXT3_EXT 0x83F2
#define GL_COMPRESSED_RGBA_S3TC_DXT5_EXT 0x83F3

typedef void (APIENTRY * PFNGLCOMPRESSEDTEXIMAGE2DPROC) (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid * data);
typedef void (APIENTRY * PFNGLCOMPRESSEDTEXIMAGE3DPROC) (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const GLvoid * data);
extern PFNGLCOMPRESSEDTEXIMAGE2DPROC __glCompressedTexImage2D;
extern PFNGLCOMPRESSEDTEXIMAGE3DPROC __glCompressedTexImage3D;

#ifndef GL_MAX_COLOR_ATTACHMENTS_EXT
#define GL_MAX_COLOR_ATTACHMENTS_EXT GL_MAX_COLOR_ATTACHMENTS_OES
#endif

#ifndef GL_DEPTH_ATTACHMENT_EXT
#define GL_DEPTH_ATTACHMENT_EXT GL_DEPTH_ATTACHMENT_OES
#endif 

#ifndef GL_RENDERBUFFER_EXT
#define GL_RENDERBUFFER_EXT GL_RENDERBUFFER_OES
#define GL_MAX_RENDERBUFFER_SIZE_EXT GL_MAX_RENDERBUFFER_SIZE_OES
#endif

#ifndef GL_FRAMEBUFFER_EXT
#define GL_FRAMEBUFFER_EXT GL_FRAMEBUFFER_OES
#define GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE_EXT GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE_OES
#define GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME_EXT GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME_OES
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE_EXT GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE_OES
//#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_3D_ZOFFSET_EXT GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_3D_ZOFFSET_OES
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL_EXT GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL_OES 
#endif


#include <Graphics/Ogls/Shared/Window/hkgWindowCgFunctionsOGLS.h>


class hkgWindowOGLS : public hkgWindow
{
	public:

		virtual void* getProcAddress(const char* proc) { return HK_NULL; }

		virtual int getMaxTextureBlendStages();

			// Set function pointers of supported functions.
			// This will fail on most platforms if a gl window has not been created.
		virtual void setupExtensionFunctions();

			// This will fail on most platforms if a gl window has not been created.
		static hkBool isExtensionSupported(const char* ext);

		virtual hkUint32 vertexShaderMinorVersion() const;
		virtual hkUint32 vertexShaderMajorVersion() const; 
		virtual hkUint32 pixelShaderMinorVersion() const;
		virtual hkUint32 pixelShaderMajorVersion() const;

		virtual void buildCommonShaderDefines( hkArray<const char*>& defines );
		virtual bool supportsShaderCompilation() const { return __cgCreateProgramFromFile != HK_NULL; }

		virtual HKG_SHADOWMAP_SUPPORT getShadowMapSupport();
		virtual void prepareForShadowMap(); // creates depth and render target
		virtual void beginShadowMapRender(class hkgDisplayWorld* world, class hkgCamera* camera, class hkgLight* light, bool clearMap = true  ); // sets the depth and render target to be the current
		virtual int endShadowMapRender(int textureStageForMap, bool shaderDriven); // resets to the normal depth and render target
		virtual void revertShadowMapState(); // resets to the normal texture modes etc
		virtual void cleanupShadowMap(); // releases held targets
		virtual void setShadowMapSize(int size); 
		virtual int getShadowMapSize() const;
		virtual void enableDebugShadowMapView(bool on) const { m_debugShadowMaps = on; } 

		virtual const float* getComputedShadowMatrix(bool includeCameraViewMatri) const; // get the perspective shadow matrix
		virtual const float* getComputedShadowTextureMatrix(bool includeCameraViewMatri) const; // get the perspective shadow matrix with the texture lookup and bias baked in

		virtual bool saveFrontFrameBufferToBMP(const char* filename);

		void displayShadowMap();

		static hkBool hasCompressedTextureSupport;
		hkBool m_supportsHWShadowMaps;
		hkBool m_supportsDepthRenderTargets;
		hkBool m_supportsVSM;
		hkBool m_supportsL32F;
		hkBool m_supports16F;
		hkBool m_supportsLA32F;
		hkBool m_supports32F;


	protected:

		hkgWindowOGLS();
		virtual ~hkgWindowOGLS();

		void initShaderCollection();

		int						m_shadowMapSize;
		HKG_SHADOWMAP_SUPPORT   m_shadowMapSupport;

		HKG_ENABLED_STATE		m_nonShadowState;

		
		int						m_currentShadowTextureStage;
		mutable float			m_shadowTexMatrix[16];
		mutable bool			m_debugShadowMaps;
		class hkgPixelShaderOGLS* m_debugShadowMapPShader;
		class hkgVertexShaderOGLS* m_debugShadowMapVShader;
		
		hkgPixelShaderOGLS* m_blurShadowMapPShader;
		hkgVertexShaderOGLS* m_blurShadowMapVShader;

		GLuint							m_shadowColorTexture;     // float texture
		GLuint							m_shadowColorBlurTexture; // float texture 
		class hkgRenderTargetOGLS*		m_pShadowZ; // depth texture
		class hkgFrameBufferOGLS*		m_pShadowFrameBuffer; // attach float color and z as the current fbo

		unsigned char* m_framebuffer; ///
		int m_framebufferSize;
};

#endif //HK_GRAPHICS_WINDOW_OGLS_H


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
