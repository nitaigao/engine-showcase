#!/bin/sh
echo $2
echo $3

cat >> $1 <<__EOF
/* Define to turn debugging globally on. */
#undef DEBUG

/* Version number of package */
#define MOCKPP_VERSION "$2"

/* Name of package */
#define MOCKPP_PACKAGE "$3"

/* Define to use Boost.Test as framework. */
#undef MOCKPP_USE_BOOSTTEST

/* Define to use CppUnit as framework. */
#undef MOCKPP_USE_CPPUNIT

/* Define to 1 if you have the <cppunit/Message.h> header file. */
#undef HAVE_CPPUNIT_MESSAGE_H

/* Define if you have CxxTest installed. */
#define MOCKPP_USE_CXXTEST

/* Define to 1 if you have the <limits> header file. */
#define HAVE_LIMITS

/* Define to compile for unicode environment. */
#undef MOCKPP_UNICODE

/* Define if you have the cwchar header. */
#define HAVE_CWCHAR

/* Define if you have the std::wstring type. */
#define HAVE_WSTRING

/* Define if you have the wcscmp function. */
#define HAVE_WCSCMP

/* Define if you have the std::wcscmp function. */
#define HAVE_STD_WCSCMP

/* Define to supress compiler warnings about unused variables. */
#define MOCKPP_UNUSED(x) x;

/* Define to enable built-in mini-stl. */
/* #undef MOCKPP_USE_MINI_STL  */
/* #undef CXXTEST_USE_MINI_STL  */

/* Define to disable runtime type information. */
/* #undef MOCKPP_NO_RTTI */

/* Define to disable exceptions. */
/* #undef MOCKPP_NO_EXCEPTIONS */

/* Define if the compiler does not support the template hint. */
/* #undef MOCKPP_NO_TEMPLATE_HINT */


__EOF

#echo "/* Define to enable built-in mini-stl. */" >>$1
#grep MOCKPP_USE_MINI_STL   ../config.h >>$1
#grep CXXTEST_USE_MINI_STL  ../config.h >>$1
#echo >>$1

#echo "/* Define to disable runtime type information. */" >>$1
#grep MOCKPP_NO_RTTI        ../config.h >>$1
#echo >>$1

#echo "/* Define to disable exceptions. */" >>$1
#grep MOCKPP_NO_EXCEPTIONS  ../config.h >>$1
