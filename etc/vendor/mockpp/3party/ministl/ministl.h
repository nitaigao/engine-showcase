// -*- c++ -*-
/*
 *  MICO --- a free CORBA implementation
 *  Copyright (C) 1997-98 Kay Roemer & Arno Puder
 *
 *  Copyright (C) 2005-2007 Ewald Arnold MINISTL at ewald-arnold dot de
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public
 *  License along with this library; if not, write to the Free
 *  Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 *  Send comments and/or bug reports to:
 *                 mico@informatik.uni-frankfurt.de
 */

// #define MINISTL_NO_NAMESPACE

#ifndef __ministl_ministl_h__
#define __ministl_ministl_h__

#ifndef assert
#include <cassert>
#endif

#ifdef MINISTL_NDEBUG
#define ministl_assert(exp)
#else
#define ministl_assert(exp) assert(exp)
#endif

#ifdef MINISTL_NO_STD_NS
# define MINISTL_STD_NS           /* nothing */
#else
# define MINISTL_STD_NS           std
#endif

#ifdef MINISTL_NO_NAMESPACE
# define MINISTL_ANON_NS_START    /* nothing */
# define MINISTL_NS               /* nothing */
# define MINISTL_NS_START         /* nothing */
# define MINISTL_NS_END           /* nothing */
# define USING_NAMESPACE_MINISTL  /* nothing */
# define USING_MINISTL_XX(xx)     /* nothing */
#else
# define MINISTL_ANON_NS_START    namespace {
# define MINISTL_NS               ministl
# define MINISTL_NS_START         namespace ministl {
# define MINISTL_NS_END           }
# define USING_NAMESPACE_MINISTL  using namespace ministl;
# define USING_MINISTL_XX(xx)     using ministl::xx;
#endif

MINISTL_NS_START


static inline long _min_ (long x, long y)
{
  return x < y ? x : y;
}

static inline long _max_ (long x, long y)
{
  return x > y ? x : y;
}


MINISTL_NS_END


#endif // __ministl_ministl_h__
