#ifndef __cxxtest_StdHeaders_h__
#define __cxxtest_StdHeaders_h__

//
// This file basically #includes the STL headers.
// It exists to support warning level 4 in Visual C++
//

#include <cxxtest/Flags.h>

#ifdef _MSC_VER
#   pragma warning( push, 1 )
#endif // _MSC_VER

#include <complex>

#include CXXTEST_STRING_H
#include CXXTEST_VECTOR_H
#include CXXTEST_MAP_H
#include CXXTEST_SET_H
#include CXXTEST_LIST_H
// #include CXXTEST_ALGORITHM_H
// #include CXXTEST_FUNCTION_H

#ifndef CXXTEST_USE_MINI_STL
#include CXXTEST_DEQUE_H
#endif

#ifdef _MSC_VER
#   pragma warning( pop )
#endif // _MSC_VER

#endif // __cxxtest_StdHeaders_h__
