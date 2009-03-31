/*
 * Copyright (C) 2005-2007 Ewald Arnold mockpp at ewald-arnold dot de
 *
 * Copyright (c) 1994
 * Hewlett-Packard Company
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  Hewlett-Packard Company makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 *
 */

#include <new>

#ifndef MINISTL_DEFALLOC_H
#define MINISTL_DEFALLOC_H


MINISTL_NS_START


template <class T>
inline void destroy(T* pointer)
{
  pointer->~T();
#ifdef __BORLANDC__
  pointer;
#endif    
}

template<> inline void destroy(char*) {}
template<> inline void destroy(unsigned char*) {}
template<> inline void destroy(short*) {}
template<> inline void destroy(unsigned short*) {}
template<> inline void destroy(int*) {}
template<> inline void destroy(unsigned int*) {}
template<> inline void destroy(long*) {}
template<> inline void destroy(unsigned long*) {}
template<> inline void destroy(float*) {}
template<> inline void destroy(double*) {}
template<> inline void destroy(char**) {}
template<> inline void destroy(unsigned char**) {}
template<> inline void destroy(short**) {}
template<> inline void destroy(unsigned short**) {}
template<> inline void destroy(int**) {}
template<> inline void destroy(unsigned int**) {}
template<> inline void destroy(long**) {}
template<> inline void destroy(unsigned long**) {}
template<> inline void destroy(float**) {}
template<> inline void destroy(double**) {}


template <class T>
inline void destroy(T* beg, T* end)
{
  for ( ; beg != end; ++beg)
    beg->~T();
}

template<> inline void destroy(char*, char*) {}
template<> inline void destroy(unsigned char*, unsigned char*) {}
template<> inline void destroy(short*, short*) {}
template<> inline void destroy(unsigned short*, unsigned short*) {}
template<> inline void destroy(int*, int*) {}
template<> inline void destroy(unsigned int*, unsigned int*) {}
template<> inline void destroy(long*, long*) {}
template<> inline void destroy(unsigned long*, unsigned long*) {}
template<> inline void destroy(float*, float*) {}
template<> inline void destroy(double*, double*) {}
template<> inline void destroy(char**, char**) {}
template<> inline void destroy(unsigned char**, unsigned char**) {}
template<> inline void destroy(short**, short**) {}
template<> inline void destroy(unsigned short**, unsigned short**) {}
template<> inline void destroy(int**, int**) {}
template<> inline void destroy(unsigned int**, unsigned int**) {}
template<> inline void destroy(long**, long**) {}
template<> inline void destroy(unsigned long**, unsigned long**) {}
template<> inline void destroy(float**, float**) {}
template<> inline void destroy(double**, double**) {}

MINISTL_NS_END


#endif // MINISTL_DEFALLOC_H

