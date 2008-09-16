#ifndef _pocodefs_h_
# define _pocodefs_h_

//
// This header file is used by examples (could also be used by 
// user applications) to export/import components class/symbols 
// on windows.
//

#ifdef WIN32
# ifdef BUILD_DLL
#  define POCO_EXPORT __declspec(dllexport)
# else
#  define POCO_EXPORT __declspec(dllimport)
# endif
#else
# define POCO_EXPORT
#endif

#endif
