/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_GRAPHICS_WINDOW_CG_FUNCTIONS_OGLS_H
#define HK_GRAPHICS_WINDOW_CG_FUNCTIONS_OGLS_H

// CG headers

typedef CGcontext (*__cgCreateContextFUNC)(void); 
typedef void (*__cgDestroyContextFUNC)(CGcontext ctx);
typedef CGbool (*__cgIsContextFUNC)(CGcontext ctx);
typedef CGerror (*__cgGetErrorFUNC)(void);
typedef const char* (*__cgGetErrorStringFUNC)(CGerror error);
typedef const char* (*__cgGetLastErrorStringFUNC)(CGerror* error);
typedef void (*__cgSetErrorCallbackFUNC)(CGerrorCallbackFunc func);

typedef void (*__cgCompileProgramFUNC)(CGprogram program);
typedef CGparameter (*__cgGetNamedParameterFUNC)(CGprogram prog, const char *name);
typedef CGbool (*__cgIsParameterReferencedFUNC)(CGparameter param);
	
typedef const char* (*__cgGetLastListingFUNC)(CGcontext ctx);
typedef CGprogram (*__cgCreateProgramFUNC)(CGcontext ctx, CGenum program_type, const char *program, CGprofile profile, const char *entry, const char **args);
typedef CGprogram (*__cgCreateProgramFromFileFUNC)(CGcontext ctx, CGenum program_type, const char *program_file, CGprofile profile, const char *entry, const char **args);
typedef CGbool (*__cgIsProgramCompiledFUNC)(CGprogram program); 
typedef void (*__cgSetParameter1fFUNC)(CGparameter param, float x);

extern __cgCreateContextFUNC __cgCreateContext;
extern __cgDestroyContextFUNC __cgDestroyContext;
extern __cgIsContextFUNC __cgIsContext;
extern __cgGetErrorFUNC __cgGetError;
extern __cgGetErrorStringFUNC __cgGetErrorString;
extern __cgGetLastErrorStringFUNC __cgGetLastErrorString;
extern __cgSetErrorCallbackFUNC __cgSetErrorCallback;

extern __cgCompileProgramFUNC __cgCompileProgram;
extern __cgGetNamedParameterFUNC	__cgGetNamedParameter;
extern __cgIsParameterReferencedFUNC __cgIsParameterReferenced;
extern __cgGetLastListingFUNC __cgGetLastListing;
extern __cgCreateProgramFUNC __cgCreateProgram;
extern __cgCreateProgramFromFileFUNC	__cgCreateProgramFromFile;
extern __cgIsProgramCompiledFUNC __cgIsProgramCompiled;
extern __cgSetParameter1fFUNC __cgSetParameter1f;

// CGGL headers
typedef void (*__cgGLSetParameter1fvFUNC)(CGparameter param, const float *v);
typedef void (*__cgGLSetParameter2fvFUNC)(CGparameter param, const float *v);
typedef void (*__cgGLSetParameter3fvFUNC)(CGparameter param, const float *v);
typedef void (*__cgGLSetParameter4fvFUNC)(CGparameter param, const float *v);
typedef void (*__cgGLSetMatrixParameterfrFUNC)(CGparameter param, const float *matrix);
typedef void (*__cgGLSetMatrixParameterfcFUNC)(CGparameter param, const float *matrix);
typedef void (*__cgGLSetMatrixParameterArrayfrFUNC)(CGparameter param, long offset, long nelements, const float * v );


typedef void (*__cgGLEnableProfileFUNC)(CGprofile profile);
typedef void (*__cgGLBindProgramFUNC)(CGprogram program);
typedef void (*__cgGLDisableProfileFUNC)(CGprofile profile);
typedef void (*__cgGLLoadProgramFUNC)(CGprogram program);
typedef CGprofile (*__cgGLGetLatestProfileFUNC)(CGGLenum profile_type);
typedef void (*__cgGLSetOptimalOptionsFUNC)(CGprofile profile);

extern __cgGLSetParameter1fvFUNC	__cgGLSetParameter1fv;
extern __cgGLSetParameter2fvFUNC	__cgGLSetParameter2fv;
extern __cgGLSetParameter3fvFUNC	__cgGLSetParameter3fv;
extern __cgGLSetParameter4fvFUNC __cgGLSetParameter4fv;
extern __cgGLSetMatrixParameterfrFUNC __cgGLSetMatrixParameterfr;
extern __cgGLSetMatrixParameterfcFUNC __cgGLSetMatrixParameterfc;
extern __cgGLEnableProfileFUNC __cgGLEnableProfile;
extern __cgGLBindProgramFUNC __cgGLBindProgram;
extern __cgGLDisableProfileFUNC __cgGLDisableProfile;
extern __cgGLLoadProgramFUNC __cgGLLoadProgram;
extern __cgGLGetLatestProfileFUNC __cgGLGetLatestProfile;
extern __cgGLSetOptimalOptionsFUNC __cgGLSetOptimalOptions;
extern __cgGLSetMatrixParameterArrayfrFUNC __cgGLSetMatrixParameterArrayfr;

void HK_CALL hkgSetupCgFunctionPointersOgls( );

#endif //HK_GRAPHICS_WINDOW_CG_FUNCTIONS_OGLS_H


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
