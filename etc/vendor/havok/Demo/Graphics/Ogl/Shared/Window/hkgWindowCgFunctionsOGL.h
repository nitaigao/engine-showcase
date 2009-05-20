/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_GRAPHICS_WINDOW_CG_FUNCTIONS_OGL_H
#define HK_GRAPHICS_WINDOW_CG_FUNCTIONS_OGL_H

// CG headers

typedef CGcontext (*_cgCreateContextFUNC)(void); 
typedef void (*_cgDestroyContextFUNC)(CGcontext ctx); 
typedef CGerror (*_cgGetErrorFUNC)(void);
typedef const char* (*_cgGetErrorStringFUNC)(CGerror error);
typedef void (*_cgSetErrorCallbackFUNC)(CGerrorCallbackFunc func);

typedef void (*_cgCompileProgramFUNC)(CGprogram program);
typedef CGparameter (*_cgGetNamedParameterFUNC)(CGprogram prog, const char *name);
typedef CGbool (*_cgIsParameterReferencedFUNC)(CGparameter param );
typedef const char* (*_cgGetLastListingFUNC)(CGcontext ctx);
typedef CGprogram (*_cgCreateProgramFUNC)(CGcontext ctx, CGenum program_type, const char *program, CGprofile profile, const char *entry, const char **args);
typedef CGprogram (*_cgCreateProgramFromFileFUNC)(CGcontext ctx, CGenum program_type, const char *program_file, CGprofile profile, const char *entry, const char **args);
typedef CGbool (*_cgIsProgramCompiledFUNC)(CGprogram program); 
typedef void (*_cgSetParameter1fFUNC)(CGparameter param, float x);

extern _cgCreateContextFUNC _cgCreateContext;
extern _cgDestroyContextFUNC _cgDestroyContext;
extern _cgGetErrorFUNC _cgGetError;
extern _cgGetErrorStringFUNC _cgGetErrorString;
extern _cgSetErrorCallbackFUNC _cgSetErrorCallback;

extern _cgCompileProgramFUNC _cgCompileProgram;
extern _cgGetNamedParameterFUNC	_cgGetNamedParameter;
extern _cgIsParameterReferencedFUNC _cgIsParameterReferenced;
extern _cgGetLastListingFUNC _cgGetLastListing;
extern _cgCreateProgramFUNC _cgCreateProgram;
extern _cgCreateProgramFromFileFUNC	_cgCreateProgramFromFile;
extern _cgIsProgramCompiledFUNC _cgIsProgramCompiled;
extern _cgSetParameter1fFUNC _cgSetParameter1f;

// CGGL headers
typedef void (*_cgGLSetParameter1fvFUNC)(CGparameter param, const float *v);
typedef void (*_cgGLSetParameter2fvFUNC)(CGparameter param, const float *v);
typedef void (*_cgGLSetParameter3fvFUNC)(CGparameter param, const float *v);
typedef void (*_cgGLSetParameter4fvFUNC)(CGparameter param, const float *v);
typedef void (*_cgGLSetMatrixParameterfrFUNC)(CGparameter param, const float *matrix);
typedef void (*_cgGLSetMatrixParameterArrayfrFUNC)(CGparameter param, long offset, long nelements, const float * v );

typedef void (*_cgGLEnableProfileFUNC)(CGprofile profile);
typedef void (*_cgGLBindProgramFUNC)(CGprogram program);
typedef void (*_cgGLDisableProfileFUNC)(CGprofile profile);
typedef void (*_cgGLLoadProgramFUNC)(CGprogram program);
typedef CGprofile (*_cgGLGetLatestProfileFUNC)(CGGLenum profile_type);
typedef void (*_cgGLSetOptimalOptionsFUNC)(CGprofile profile);

extern _cgGLSetParameter1fvFUNC	_cgGLSetParameter1fv;
extern _cgGLSetParameter2fvFUNC	_cgGLSetParameter2fv;
extern _cgGLSetParameter3fvFUNC	_cgGLSetParameter3fv;
extern _cgGLSetParameter4fvFUNC _cgGLSetParameter4fv;
extern _cgGLSetMatrixParameterfrFUNC _cgGLSetMatrixParameterfr;
extern _cgGLEnableProfileFUNC _cgGLEnableProfile;
extern _cgGLBindProgramFUNC _cgGLBindProgram;
extern _cgGLDisableProfileFUNC _cgGLDisableProfile;
extern _cgGLLoadProgramFUNC _cgGLLoadProgram;
extern _cgGLGetLatestProfileFUNC _cgGLGetLatestProfile;
extern _cgGLSetOptimalOptionsFUNC _cgGLSetOptimalOptions;
extern _cgGLSetMatrixParameterArrayfrFUNC _cgGLSetMatrixParameterArrayfr;

void HK_CALL hkgSetupCgFunctionPointers( );

#endif //HK_GRAPHICS_WINDOW_CG_FUNCTIONS_OGL_H


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
