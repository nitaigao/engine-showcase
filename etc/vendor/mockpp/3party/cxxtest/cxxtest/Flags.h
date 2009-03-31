#ifndef __cxxtest__Flags_h__
#define __cxxtest__Flags_h__

//
// These are the flags that control CxxTest
//

#ifdef _MSC_VER
#  include <mockpp/mockpp_config-msvc_71.h>

#elif defined(__BORLANDC__)
# include <mockpp/mockpp_config-bcb5.h>

#elif defined(__CYGWIN__)
# ifdef HAVE_CONFIG_H
#  include <config.h>
# else
#  include <mockpp/mockpp_config.h>
# endif

#else
# ifdef HAVE_CONFIG_H
#  include <config.h>
# else
#  include <mockpp/mockpp_config.h>
# endif
#endif

//////////////////////////////////////////////////
//

#ifdef CXXTEST_NO_STD_NS
# define CXXTEST_STD_NS           /* nothing */
#else
# define CXXTEST_STD_NS           std
#endif

//////////////////////////////////////////////////
//

#ifdef CXXTEST_NO_NAMESPACE
# define CXXTEST_ANON_NS_START    /* nothing */
# define CXXTEST_NS               /* nothing */
# define CXXTEST_NS_START         /* nothing */
# define CXXTEST_NS_END           /* nothing */
# define USING_NAMESPACE_CXXTEST  /* nothing */
#else
# define CXXTEST_ANON_NS_START    namespace {
# define CXXTEST_NS               CxxTest
# define CXXTEST_NS_START         namespace CxxTest {
# define CXXTEST_NS_END           }
# define USING_NAMESPACE_CXXTEST  using namespace CxxTest;
#endif

//////////////////////////////////////////////////
//

#ifdef CXXTEST_USE_MINI_STL

#define CXXTEST_STRING_H    <ministl/string>
#define CXXTEST_VECTOR_H    <ministl/vector>
#define CXXTEST_MAP_H       <ministl/map>
#define CXXTEST_SET_H       <ministl/set>
#define CXXTEST_LIST_H      <ministl/list>
#define CXXTEST_ALGORITHM_H <ministl/algorithm>
#define CXXTEST_FUNCTION_H  <ministl/function>

// namespace ministl
#define CXXTEST_STL   MINISTL_NS
#define CXXTEST_ALTERNATIVE_STL

#else

#define CXXTEST_DEQUE_H     <deque>
#define CXXTEST_STRING_H    <string>
#define CXXTEST_VECTOR_H    <vector>
#define CXXTEST_MAP_H       <map>
#define CXXTEST_SET_H       <set>
#define CXXTEST_LIST_H      <list>
#define CXXTEST_FUNCTION_H  <function>
#define CXXTEST_ALGORITHM_H <algorithm>

// namespace std
#define CXXTEST_STL std
#undef CXXTEST_ALTERNATIVE_STL

#endif

//////////////////////////////////////////////////
//

#if defined(_WIN32) || defined(_WIN64)
# ifndef __WIN32__
#  define __WIN32__
# endif
#endif 

#ifdef __WIN32__
# ifdef _MSC_VER
// STATIC
#  if defined(CXXTEST_STATIC_LIB)
#   define CXXTEST_API_DECL(type) type
#   define CXXTEST_API_DECL0
#   define CXXTEST_API_IMPL(type) type
#   define CXXTEST_API_IMPL0
#   pragma message("*** CxxTest: Static ***")
#  else //CXXTEST_STATIC_LIB

#  if defined(CXXTEST_NEED_EXPORTS)
#   define CXXTEST_API_DECL(type)  __declspec(dllexport)  type 
#   define CXXTEST_API_DECL0       __declspec(dllexport)
#   define CXXTEST_API_IMPL(type)  __declspec(dllexport)  type
#   define CXXTEST_API_IMPL0       __declspec(dllexport)
#   pragma message("*** CxxTest: DllExport ***")

#  else // CXXTEST_NEED_EXPORTS

#   define CXXTEST_API_DECL(type)  __declspec(dllimport) type
#   define CXXTEST_API_DECL0       __declspec(dllimport) 
#   define CXXTEST_API_IMPL(type)  __declspec(dllimport) type
#   define CXXTEST_API_IMPL0       __declspec(dllimport)
#   pragma message("*** CxxTest: DllImport ***")
#  endif // CXXTEST_NEED_EXPORTS

#  endif //CXXTEST_STATIC_LIB

# else // __BORLANDC__

# if defined(CXXTEST_NEED_EXPORTS)
#  define CXXTEST_API_DECL0      __declspec(dllexport)
#  define CXXTEST_API_IMPL0      __declspec(dllexport)
#  define CXXTEST_API_DECL(type) __declspec(dllexport) type
#  define CXXTEST_API_IMPL(type) __declspec(dllexport) type
# else
#  define CXXTEST_API_DECL0      __declspec(dllimport)
#  define CXXTEST_API_IMPL0
#  define CXXTEST_API_DECL(type) __declspec(dllimport) type
#  define CXXTEST_API_IMPL(type)  type
# endif

# endif

#else

#  define CXXTEST_API_DECL0    
#  define CXXTEST_API_IMPL0
#  define CXXTEST_API_DECL(type)  type
#  define CXXTEST_API_IMPL(type)  type
#endif

//////////////////////////////////////////////////
//

#if !defined(CXXTEST_FLAGS)
#   define CXXTEST_FLAGS
#endif // !CXXTEST_FLAGS

#if defined(CXXTEST_HAVE_EH) && !defined(_CXXTEST_HAVE_EH)
#   define _CXXTEST_HAVE_EH
#endif // CXXTEST_HAVE_EH

#if defined(CXXTEST_HAVE_STD) && !defined(_CXXTEST_HAVE_STD)
#   define _CXXTEST_HAVE_STD
#endif // CXXTEST_HAVE_STD

#if defined(CXXTEST_OLD_TEMPLATE_SYNTAX) && !defined(_CXXTEST_OLD_TEMPLATE_SYNTAX)
#   define _CXXTEST_OLD_TEMPLATE_SYNTAX
#endif // CXXTEST_OLD_TEMPLATE_SYNTAX

#if defined(CXXTEST_OLD_STD) && !defined(_CXXTEST_OLD_STD)
# define _CXXTEST_OLD_STD
# ifdef CXXTEST_USE_MINI_STL
#  error ministl namespace always need to avoid abiguity with default STL
# endif
#endif // CXXTEST_OLD_STD

#if defined(CXXTEST_ABORT_TEST_ON_FAIL) && !defined(_CXXTEST_ABORT_TEST_ON_FAIL)
#   define _CXXTEST_ABORT_TEST_ON_FAIL
#endif // CXXTEST_ABORT_TEST_ON_FAIL

#if defined(CXXTEST_NO_COPY_CONST) && !defined(_CXXTEST_NO_COPY_CONST)
#   define _CXXTEST_NO_COPY_CONST
#endif // CXXTEST_NO_COPY_CONST

#if defined(CXXTEST_FACTOR) && !defined(_CXXTEST_FACTOR)
#   define _CXXTEST_FACTOR
#endif // CXXTEST_FACTOR

#if defined(CXXTEST_PARTIAL_TEMPLATE_SPECIALIZATION) && !defined(_CXXTEST_PARTIAL_TEMPLATE_SPECIALIZATION)
#   define _CXXTEST_PARTIAL_TEMPLATE_SPECIALIZATION
#endif // CXXTEST_PARTIAL_TEMPLATE_SPECIALIZATION

#if defined(CXXTEST_LONGLONG)
#   if defined(_CXXTEST_LONGLONG)
#       undef _CXXTEST_LONGLONG
#   endif
#   define _CXXTEST_LONGLONG CXXTEST_LONGLONG
#endif // CXXTEST_LONGLONG

#ifndef CXXTEST_MAX_DUMP_SIZE
#   define CXXTEST_MAX_DUMP_SIZE 0
#endif // CXXTEST_MAX_DUMP_SIZE

#if defined(_CXXTEST_ABORT_TEST_ON_FAIL) && !defined(CXXTEST_DEFAULT_ABORT)
#   define CXXTEST_DEFAULT_ABORT true
#endif // _CXXTEST_ABORT_TEST_ON_FAIL && !CXXTEST_DEFAULT_ABORT

#if !defined(CXXTEST_DEFAULT_ABORT)
#   define CXXTEST_DEFAULT_ABORT false
#endif // !CXXTEST_DEFAULT_ABORT

#if defined(_CXXTEST_ABORT_TEST_ON_FAIL) && !defined(_CXXTEST_HAVE_EH)
#   warning "CXXTEST_ABORT_TEST_ON_FAIL is meaningless without CXXTEST_HAVE_EH"
#   undef _CXXTEST_ABORT_TEST_ON_FAIL
#endif // _CXXTEST_ABORT_TEST_ON_FAIL && !_CXXTEST_HAVE_EH

//////////////////////////////////////////////////
//

//
// Some minimal per-compiler configuration to allow us to compile
//

#ifdef __BORLANDC__
#   if __BORLANDC__ <= 0x520 // Borland C++ 5.2 or earlier
#       ifndef _CXXTEST_OLD_STD
#           define _CXXTEST_OLD_STD
#       endif
#       ifndef _CXXTEST_OLD_TEMPLATE_SYNTAX
#           define _CXXTEST_OLD_TEMPLATE_SYNTAX
#       endif
#   endif
#   if __BORLANDC__ >= 0x540 // C++ Builder 4.0 or later
#       ifndef _CXXTEST_NO_COPY_CONST
#           define _CXXTEST_NO_COPY_CONST
#       endif
#       ifndef _CXXTEST_LONGLONG
#           define _CXXTEST_LONGLONG __int64
#       endif
#   endif
#endif // __BORLANDC__

//////////////////////////////////////////////////
//

#ifdef _MSC_VER // Visual C++
#   ifndef _CXXTEST_LONGLONG
#       define _CXXTEST_LONGLONG __int64
#   endif
#   if (_MSC_VER >= 0x51E)
#       ifndef _CXXTEST_PARTIAL_TEMPLATE_SPECIALIZATION
#           define _CXXTEST_PARTIAL_TEMPLATE_SPECIALIZATION
#       endif
#   endif
#   pragma warning( disable : 4127 )
#   pragma warning( disable : 4290 )
#   pragma warning( disable : 4511 )
#   pragma warning( disable : 4512 )
#   pragma warning( disable : 4514 )
#endif // _MSC_VER

//////////////////////////////////////////////////
//

#ifdef __GNUC__
#   if (__GNUC__ > 2) || (__GNUC__ == 2 && __GNUC_MINOR__ >= 9)
#       ifndef _CXXTEST_PARTIAL_TEMPLATE_SPECIALIZATION
#           define _CXXTEST_PARTIAL_TEMPLATE_SPECIALIZATION
#       endif
#   endif
#endif // __GNUC__

//////////////////////////////////////////////////
//

#ifdef __DMC__ // Digital Mars
#   ifndef _CXXTEST_OLD_STD
#       define _CXXTEST_OLD_STD
#   endif
#endif

#endif // __cxxtest__Flags_h__
