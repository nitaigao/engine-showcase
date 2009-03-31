/** @file
    @brief Common stuff for mockpp project

  $Id: mockpp.h 1437 2008-03-08 20:12:53Z ewald-arnold $

 ***************************************************************************/


/**************************************************************************

   begin                : Thu Dec 12 2002
   copyright            : (C) 2002-2008 by Ewald Arnold
   email                : mockpp at ewald-arnold dot de

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU Lesser General Public License as
   published by the Free Software Foundation; either version 2 of the License,
   or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU Lesser General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

 **/

/** @defgroup grp_config Configuration elements.
  */

/** @defgroup grp_helper General helper classes
  */

/** @mainpage

   \section abstract Abstract

   mockpp is a platform independent generic unit testing framework for C++. It's goal is to facilitate
   developing unit tests and integraton tests in the spirit of
   <a href="http://www.mockobjects.com/">Mock Objects for Java</a>,
   <a href="http://www.easymock.org/">EasyMock</a> and <a href="http://www.jmock.org/">jMock</a>.

   Mock objects allow you to set up predictible
   behaviour to help you test your production code by emulating some functionality your code depends on.
   This might for example be a huge database which is too difficult and time consuming to exist just for
   testing purposes.

   \section docs_struct  Getting Started With The Documentation

   mockpp consists of a lot of classes in the meanwhile. Many of them are useless for daily coding.
   Some of them are internal helper classes, some are mere interface classes which are
   a remainder of the port from java. Some are only needed when you want to extend the library in
   some way.

   To help finding the really important classes the library documentation is divided into the
   groups below. Please note that there is also a
   <a href="../tutorial/index.html">tutorial available</a> which introduces into the library
   on the base of a simple example.

   - Mock-objects in general
     - \ref grp_poor_mo
     - \ref grp_basic_mo
     - \ref grp_advanced_mo
   - More about visitable mock objects
     - \ref grp_controller
   - More about chainable mock objects
     - \ref grp_chainer
     - Sub-expectations
       - Stubs
         - \ref grp_stub
         - \ref grp_stub_abbrev
       - Matcher
         - \ref grp_matcher
         - \ref grp_matcher_abbrev
       - Constraints
         - \ref grp_constraint
         - \ref grp_constraint_abbrev
   - \ref grp_helper
   - \ref grp_framework
   - \ref grp_production
   - \ref grp_config

*/

#ifndef MOCKPP_H
#define MOCKPP_H

# ifdef _MSC_VER
// Compilation Warning remover
#  pragma warning (disable:4800)  // conversion int - bool
#  pragma warning (disable:4786)  // identifier was truncated to 'number' characters in the debug information
#  pragma warning (disable:4251)  // class 'type' needs to have dll-interface to be used by clients of class 'type2'
#  pragma warning (disable:4180)  // qualifier applied to function type has no meaning
#  pragma warning (disable:4355)  // 'this' : used in base member initializer list
#  pragma warning (disable:4512)  // assignment operator could not be generated
#  pragma warning (disable:4511)  // copy constructor could not be generated
#  pragma warning (disable:4018)  // signed/unsigned mismatch
#  pragma warning (disable:4100)  // unreferenced formal parameter
#  pragma warning (disable:4663)  // C++ language change: to explicitly specialize class template 'identifier' use the following syntax
#  pragma warning (disable:4146)  // unary minus operator applied to unsigned type, result still unsigned
#  pragma warning (disable:4244)  // conversion from 'type1' to 'type2', possible loss of data
#  pragma warning (disable:4138)  // '*/' found outside of comment
#  pragma warning (disable:4516)  // access-declarations are deprecated; member using-declarations provide a better alternative
#  pragma warning (disable:4097)  // typedef-name 'identifier1' used as synonym for class-name 'identifier2'
#  pragma warning (disable:4702)  // unreachable code
# endif // _MSC_VER


// msvc71   is 1310
// msvc2005 is 1400
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

/*
#if defined(MOCKPP_UNICODE)
# ifndef _UNICODE
#  define _UNICODE
# endif
# ifndef UNICODE
#  define UNICODE
# endif
#endif
*/

#if defined(MOCKPP_UNICODE) && defined(__CYGWIN__) // probably ugly hack
# define _GLIBCPP_USE_WSTRING 1
//# define _GLIBCPP_USE_WCHAR_T 1
//# define _GLIBCPP_HAVE_WCHAR_H
#endif


//////////////////////////////////////////////////
// Catch Macros from MACOS

#ifdef verify
#error "<verify> must not be #defined within mockpp sources"
#endif

#ifdef check
#error "<check> must not be #defined within mockpp sources"
#endif

#ifdef require
#error "<require> must not be #defined within mockpp sources"
#endif

//////////////////////////////////////////////////
//

#ifdef MOCKPP_USE_MINI_STL

#define MOCKPP_STRING_H    <ministl/string>
#define MOCKPP_VECTOR_H    <ministl/vector>
#define MOCKPP_MAP_H       <ministl/map>
#define MOCKPP_SET_H       <ministl/set>
#define MOCKPP_ALGORITHM_H <ministl/algorithm>
#define MOCKPP_FUNCTION_H  <ministl/function>

// namespace ministl
#define MOCKPP_STL       MINISTL_NS
#define MOCKPP_ALTERNATIVE_STL

#else

#define MOCKPP_STRING_H    <string>
#define MOCKPP_VECTOR_H    <vector>
#define MOCKPP_MAP_H       <map>
#define MOCKPP_SET_H       <set>
#define MOCKPP_FUNCTION_H  <function>
#define MOCKPP_ALGORITHM_H <algorithm>

// namespace std
#define MOCKPP_STL  MOCKPP_STD_NS
#undef MOCKPP_ALTERNATIVE_STL

#endif

//////////////////////////////////////////////////
//

#ifndef MOCKPP_NO_RTTI
# include <typeinfo>
#else
# undef CPPUNIT_USE_TYPEINFO_NAME
# define CPPUNIT_USE_TYPEINFO_NAME 0
#endif

//////////////////////////////////////////////////
//

#ifdef MOCKPP_NO_NAMESPACE
# define MOCKPP_ANON_NS_START    /* nothing */
# define MOCKPP_NS               /* nothing */
# define MOCKPP_NS_START         /* nothing */
# define MOCKPP_NS_END           /* nothing */
# define USING_NAMESPACE_MOCKPP  /* nothing */
#else
# define MOCKPP_ANON_NS_START    namespace {
# define MOCKPP_NS               mockpp
# define MOCKPP_NS_START         namespace mockpp {
# define MOCKPP_NS_END           }
# define USING_NAMESPACE_MOCKPP  using namespace mockpp;
#endif

//////////////////////////////////////////////////
//

#ifdef MOCKPP_NO_STD_NS
# define MOCKPP_STD_NS           /* nothing */
#else
# define MOCKPP_STD_NS           std
#endif

//////////////////////////////////////////////////
//

#ifndef MOCKPP_NO_EXCEPTIONS

#define MOCKPP_THROW(x)   throw (x)
#define MOCKPP_RETHROW    throw
#define MOCKPP_TRY        try
#define MOCKPP_CATCH(x)   (x)
#define MOCKPP_CATCH_ALL  catch(...)

#else

#define CPPUNIT_DISABLE_EXCEPTIONS // ea-hack not available in official cppunit

# ifndef MOCKPP_THROW
#  define MOCKPP_THROW(x) /* nothing */
# endif

# ifndef MOCKPP_RETHROW
#  define MOCKPP_RETHROW /* nothing */
# endif

# ifndef MOCKPP_TRY
#  define MOCKPP_TRY  /* nothing */
# endif

# ifndef MOCKPP_CATCH
#  define MOCKPP_CATCH(x)  if(false) /* usage limited */
# endif

# ifndef MOCKPP_CATCH_ALL
#  define MOCKPP_CATCH_ALL  /* nothing */
# endif

#endif

//////////////////////////////////////////////////
//

#ifdef HAVE_LIMITS
# include <limits>     // current
#else
# ifdef HAVE_VALUES_H
#  include <values.h>  // older
# else
#  include <machine/limits.h> // very old??
# endif
#endif

/** @def MOCKPP_UNLIMITED
  * Value representing an @p unlimited number.
  */

#if defined(HAVE_LIMITS) && !defined (MOCKPP_NO_STD_NUMERICLIMITS)
# define MOCKPP_UNLIMITED  (std::numeric_limits<unsigned>::max())
#else
# define MOCKPP_UNLIMITED  (UINT_MAX)
#endif

//////////////////////////////////////////////////
//

#ifdef __BORLANDC__
# define MOCKPP_FUNC_MACRO  __FUNC__
#elif defined(_MSC_VER)
# if _MSC_VER <= 1200
#  define MOCKPP_FUNC_MACRO  (MOCKPP_STL::string(__FILE__)  + "_" + MOCKPP_NS::getLatin1(MOCKPP_NS::number((long unsigned)__LINE__))) // needs #include <mockpp/compat/Formatter.h>
# else
#  define MOCKPP_FUNC_MACRO  __FUNCSIG__
# endif
#else
# define MOCKPP_FUNC_MACRO  __func__
#endif

/** @def MOCKPP_FUNC_MACRO
  * Returns the name of the current function or something similar unique
  * for a whole project.
  */

//////////////////////////////////////////////////
//

/** @def MOCKPP_UNUSED
  * Simple macro to suppress warnings about unused variables if possible.
  * Note: This is just an empty fallback. The actual content (if any) is in the
  * according config-*.h.
  * Can be used without semicolon at the end since it is already contained
  * in the macro.
  */
#ifndef MOCKPP_UNUSED
#define MOCKPP_UNUSED(x)
#endif

//////////////////////////////////////////////////
//

/** Output char values more readable (e.g. non-alphanumeric as number)
  * @ingroup grp_config
  */
#define MOCKPP_MAKE_CHAR_READABLE 1

/** MOCKPP_NS::Exception is derived from std::exception
  * @ingroup grp_config
  */
#define MOCKPP_USE_STD_EXCEPTION 1

#undef HAVE_MULTITHREAD  // obsolete and error prone

//! ExpectationBoundary::setExpectedDelta() is activated.
//! This method is only ativated upon reqest as it might cause problems
//! with user's data type if there are no operator+() and  operator-().
#define MOCKPP_BOUNDARY_DELTA 0

//////////////////////////////////////////////////
//

#if defined(_WIN32) || defined(_WIN64)
# ifndef __WIN32__
#  define __WIN32__
# endif
#endif

#if defined(__BORLANDC__) || defined (_MSC_VER)

# ifndef __WIN32__
#  define __WIN32__
# endif //__WIN32__

# ifdef __BORLANDC__
#  include <io.h>

//# ifndef __MT__
//# error MultiThreaded compilation is HIGHLY recommended, change at own risk
//# endif

//# ifndef _RTLDLL
//# error Linking with dynamic RTL is HIGHLY recommended, change at own risk
//# endif

# endif

//////////////////////////////////////////////////
//

# ifdef _MSC_VER
// STATIC
#  if defined(MOCKPP_STATIC_LIB)
#   define MOCKPP_API_DECL(type) type
#   define MOCKPP_API_DECL0
#   define MOCKPP_API_IMPL(type) type
#   define MOCKPP_API_IMPL0
#   pragma message("*** mockpp: Static ***")
#  else //MOCKPP_STATIC_LIB

#  if defined(MOCKPP_NEED_EXPORTS)
#   define MOCKPP_API_DECL(type)  __declspec(dllexport)  type
#   define MOCKPP_API_DECL0       __declspec(dllexport)
#   define MOCKPP_API_IMPL(type)  __declspec(dllexport)  type
#   define MOCKPP_API_IMPL0       __declspec(dllexport)
#   pragma message("*** mockpp: DllExport ***")

#  else // MOCKPP_NEED_EXPORTS

#   define MOCKPP_API_DECL(type)  __declspec(dllimport) type
#   define MOCKPP_API_DECL0       __declspec(dllimport)
#   define MOCKPP_API_IMPL(type)  __declspec(dllimport) type
#   define MOCKPP_API_IMPL0       __declspec(dllimport)
#   pragma message("*** mockpp: DllImport ***")
#  endif // MOCKPP_NEED_EXPORTS

#  endif //MOCKPP_STATIC_LIB

# else // __BORLANDC__

# if defined(MOCKPP_NEED_EXPORTS)
#  define MOCKPP_API_DECL0      __declspec(dllexport)
#  define MOCKPP_API_IMPL0      __declspec(dllexport)
#  define MOCKPP_API_DECL(type) __declspec(dllexport) type
#  define MOCKPP_API_IMPL(type) __declspec(dllexport) type
# else
#  define MOCKPP_API_DECL0      __declspec(dllimport)
#  define MOCKPP_API_IMPL0
#  define MOCKPP_API_DECL(type) __declspec(dllimport) type
#  define MOCKPP_API_IMPL(type)  type
# endif

# endif

#else // __BORLANDC__ || _MSC_VER

# include <unistd.h>
# define MOCKPP_API_DECL0
# define MOCKPP_API_IMPL0
# define MOCKPP_API_DECL(type) type
# define MOCKPP_API_IMPL(type) type

#endif

//////////////////////////////////////////////////
//

#include MOCKPP_STRING_H

//! Namespace for project  "Mock Objects for C++"
MOCKPP_NS_START


#ifdef MOCKPP_UNICODE

  typedef MOCKPP_STL::basic_string<wchar_t> String;
  typedef wchar_t                           Char;
//! make the charactor constant of type wchar_t
# define MOCKPP_CHAR(x)  L##x
//! make the string literal a wchar_t literal
# define MOCKPP_PCHAR(x) L##x

#else

  typedef MOCKPP_STL::basic_string<char>  String;
  typedef char                            Char;
//! make the charactor constant of type char
# define MOCKPP_CHAR(x)   x
//! make the string literal a char literal
# define MOCKPP_PCHAR(x)  x

#endif


//! transform a string literal into a MOCKPP_NS::String
#define MOCKPP_PCSTRING(x) MOCKPP_NS::String(MOCKPP_PCHAR(x))

//! transform a potential mockpp string into a MOCKPP_NS::String
#define MOCKPP_STRING(x) MOCKPP_NS::String(x)

//! Dummy, tags a string for translation without doing anything
#ifndef MOCKPP_I18N_NOOP
#define MOCKPP_I18N_NOOP(x) x
#endif

//! Dummy, prepare automatic translation within the code
#ifndef mockpp_i18n
#define mockpp_i18n(x) x
#endif

//! add template specialisation to prevent float/double ExpectationValue's
#undef MOCKP_NO_PREVENT_FLOAT_EXPECTATION


/** Gets the various parts of the version number.
  * @param  major  major part
  * @param  minor  minor part
  * @param  patch  patch counter
  * @param  debug  true: compiled with DEBUG
  * @param  info   some human readable information
  */
  MOCKPP_API_DECL(void) getVersion (int &major, int &minor, int &patch,
                               bool &debug, MOCKPP_STL::string &info);

#ifdef MOCKPP_UNICODE

/** Converts a string from latin1 to unicode.
  * Since latin1 is the first unicode page the charcters are just
  * "expanded" from "char" to "wchar_t".
  * @param  latin1  string in ISO8859-1 encoding
  * @return the string in unicode
  */
  MOCKPP_API_DECL(MOCKPP_STL::basic_string<wchar_t>)
     getUnicode(const MOCKPP_STL::string &latin1);


/** Converts a string from unicode to unicode.
  * Dummy function to avoid error messages.
  * @param  uni string in unicode
  * @return the same string
  */
  inline MOCKPP_STL::basic_string<wchar_t> getUnicode(const MOCKPP_STL::basic_string<wchar_t> &uni)
  {
    return uni;
  }

/** Converts a string from unicode to latin1.
  * Since latin1 is the first unicode page the charcters are just
  * "reduced" from "wchar_t" to "char". Characters beyond this range are undefined.
  * @param  uni string in unicode
  * @return the string in IOS8859-1 encoding
  */
  MOCKPP_API_DECL(MOCKPP_STL::string)
    getLatin1(const MOCKPP_STL::basic_string<wchar_t> &uni);

#endif

/** Converts a string from latin1 to latin1.
  * Dummy function to avoid error messages.
  * @param  latin1  string in ISO8859-1 encoding
  * @return the same string
  */
  inline MOCKPP_STL::string getLatin1(const MOCKPP_STL::string &latin1)
  {
    return latin1;
  }

  class AssertionFailedError;

/** Forwarder for failures.
  */
  typedef void (*AssertionFailedForwarder_t)(const AssertionFailedError &err);

/** Sets the function pointer which handles failed assertions.
  * Actually it is invoked instead of throwing exceptions.
  * @param  fwd  pointer to function.
  * @return previous pointer
  * @ingroup grp_config
  */
  MOCKPP_API_DECL(AssertionFailedForwarder_t)
  setAssertionFailedForwarder(AssertionFailedForwarder_t fwd);

MOCKPP_NS_END

#ifdef __SUNPRO_CC
#define MOCKPP_NO_TEMPLATE_HINT
#endif

#ifdef MOCKPP_UNICODE
//! transform the string into unicode if needed
# define MOCKPP_GET_STRING(x) MOCKPP_NS::getUnicode(x)
#else
//! transform the string into ascii if needed
# define MOCKPP_GET_STRING(x) MOCKPP_NS::getLatin1(x)
#endif

//! tag for mockpp2xml: include this constructor
#define mockpp_constructor

//! tag for mockpp2xml: include following methods until next [public | protected | private]
#define mockpp_methods

#if defined(__BORLANDC__) || (__GNUC__ < 3)
# define MOCKPP_MEMBER_RESTRICTOR_PRIVATE    public
# define MOCKPP_MEMBER_RESTRICTOR_PROTECTED  public
#else
# define MOCKPP_MEMBER_RESTRICTOR_PRIVATE    private
# define MOCKPP_MEMBER_RESTRICTOR_PROTECTED  protected
#endif

#ifdef DOXYGEN_SHOULD_INCLUDE_THIS  // make doxygen happily include docs
#define MOCKPP_UNICODE
#define CXXTEST_USE_MINI_STL
#define MOCKPP_USE_MINI_STL
#define MOCKPP_NO_NAMESPACE
#define MOCKPP_NO_TEMPLATE_HINT
#define MOCKPP_NO_TYPENAME_FOR_STL_NS
#define MOCKPP_ALTERNATIVE_STL
#define MOCKPP_NEED_EXPORTS
#define MOCKPP_API_DECL0
#define MOCKPP_API_DECL
#define MOCKPP_API_IMPL0
#define MOCKPP_API_IMPL
#endif

/** @def MOCKPP_NEED_EXPORTS
  * Enable the creation of module exports. Otherwise imports are created.
  */

/** @def MOCKPP_API_DECL0
  * Returns a platform dependent symbol to export names to the linker.
  */

/** @def MOCKPP_API_DECL
  * Returns a platform dependent symbol to export names to the linker.
  */

/** @def MOCKPP_API_IMPL0
  * Returns a platform dependent symbol to export names to the linker.
  */

/** @def MOCKPP_API_IMPL
  * Returns a platform dependent symbol to export names to the linker.
  */

/** @def MOCKPP_NO_NAMESPACE
  * @ingroup grp_config
  * Defined if namespace support is not working properly.
  * In this case \c mockpp resides in the global namespace.
  * If ministl and CxxTest are used they should also be configured
  * with \c MINISTL_NO_NAMESPACE and  \c CXXTEST_NO_NAMESPACE .
  */

/** @def MOCKPP_UNICODE
  * @ingroup grp_config
  * Defined if \c Unicode strings are used.
  */

/** @def CXXTEST_USE_MINI_STL
  * @ingroup grp_config
  * Defined if CxxTest uses the alternative STL implementation below
  * 3party/ministl instead of the regular STL.
  */

/** @def MOCKPP_NO_TEMPLATE_HINT
  * @ingroup grp_config
  * Defined if the compiler does not support the template hint
  * like this:  value_type::template long_name&lt;T1,T2>::end_type
  */

/** @def MOCKPP_MEMBER_RESTRICTOR_PROTECTED
  * Makes member public for Borland BCB5 otherwise protected.
  */

/** @def MOCKPP_MEMBER_RESTRICTOR_PRIVATE
  * Makes member public for Borland BCB5 otherwise private.
  */

/** @def MOCKPP_MEMBER_RESTRICTOR_PROTECTED
  * Makes member public for Borland BCB5 otherwise protected.
  */

/** @def MOCKPP_NO_TYPENAME_FOR_STL_NS
  * @ingroup grp_config
  * Using typename yields an error in certain cases when stl is in
  * the global namespace.
  */

/** @def MOCKPP_ANON_NS_START
  * Conditional start of anonymous namespace
  */

/** @def MOCKPP_NS
  * Conditional mockpp namespace
  */

/** @def MOCKPP_NS_START
  * Conditional start of mockpp namespace
  */

/** @def MOCKPP_NS_END
  * Conditional end of mockpp namespace
  */

/** @def USING_NAMESPACE_MOCKPP
  * Conditional using mockpp namespace
  */

/** @def MOCKPP_USE_INVOCATION_EQUALS
  * Include \c equals methods in \c Invocation classes.
  * @ingroup grp_config
  */

/** @def MOCKPP_USE_MINI_STL
  * @ingroup grp_config
  * When defined mockpp uses built-in mini stl.
  */

/** @def MOCKPP_STL
  * Actual namespace for \c STL classes
  */

/** @def MOCKPP_STD_NS
  * Actual name for \c std namespace
  */

/** @def MOCKPP_ALTERNATIVE_STL
  * When defined alternative stl in use
  */

/** @def MOCKPP_STRING_H
  * actual &lt;string> header file
  */

/** @def MOCKPP_VECTOR_H
  * actual &lt;vector> header file
  */

/** @def MOCKPP_MAP_H
  * actual &lt;map> header file
  */

/** @def MOCKPP_SET_H
  * actual &lt;set> header file
  */

/** @def MOCKPP_ALGORITHM_H
  * actual &lt;algorithm> header file
  */

/** @def MOCKPP_FUNCTION_H
  * actual &lt;function> header file
  */

/** @def MOCKPP_THROW
  * actual implementation of \c throw statement when throwing a type
  */

/** @def MOCKPP_RETHROW
  * actual implementation of \c throw statement when re-throwing an undefined value
  */

/** @def MOCKPP_TRY
  * actual implementation of \c try statement
  */

/** @def MOCKPP_CATCH
  * actual implementation of \c catch statement for a certain object
  */

/** @def MOCKPP_CATCH_ALL
  * actual implementation of \c catch statement for a \c catch(...)
  */

/** @var mockpp::Char
  * Actual type of a single character (char/wchar_t)
  */

/** @var mockpp::String
  * Actual type of a string (basic_string&lt;char/wchar_t>)
  */

#endif // MOCKPP_H
