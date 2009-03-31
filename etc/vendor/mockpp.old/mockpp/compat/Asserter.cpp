/** @file
    @brief    handle assertions

  $Id: Asserter.cpp 1437 2008-03-08 20:12:53Z ewald-arnold $

 ***************************************************************************/

/**************************************************************************

   begin                : Sat Dec 21 2002
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

#define MOCKPP_NEED_EXPORTS
#include <mockpp/mockpp.h> // always first

#include <cmath>
#include <cstring>

#ifdef HAVE_CWCHAR
#include <cwchar>
#endif

// Taken almost literally from GNU Libc sources:

#if !defined(HAVE_WCSCMP) && defined(MOCKPP_UNICODE)

/* Compare S1 and S2, returning less than, equal to or
   greater than zero if S1 is lexicographically less than,
   equal to or greater than S2.	 */
static int
wcscmp (const wchar_t  *s1, const wchar_t *s2)
{
  wchar_t c1, c2;

  do
    {
      c1 = *s1++;
      c2 = *s2++;
      if (c1 == L'\0')
         return c1 - c2;
    }
  while (c1 == c2);

  return c1 - c2;
}

#endif // HAVE_WCSCMP


#include <mockpp/compat/Asserter.h>


MOCKPP_NS_START


MOCKPP_API_IMPL(void) assertTrue(unsigned srcline, const char* srcfile,
                              const String &message, bool condition)
{
  if (!condition)
    fail(srcline, srcfile, message);
}


MOCKPP_API_IMPL(void) assertTrue(unsigned srcline, const char* srcfile,
                              bool condition)
{
  assertTrue(srcline, srcfile, MOCKPP_PCHAR(""), condition);
}


MOCKPP_API_IMPL(void) assertFalse(unsigned srcline, const char* srcfile,
                               const String &message, bool condition)
{
  assertTrue(srcline, srcfile, message, !condition);
}


MOCKPP_API_IMPL(void) assertFalse(unsigned srcline, const char* srcfile,
                               bool condition)
{
  assertFalse(srcline, srcfile, MOCKPP_PCHAR(""), condition);
}


MOCKPP_API_IMPL(void) fail(unsigned srcline, const char* srcfile,
                        const String &message)
{
  assertionFailed(srcline, srcfile, message);
}


MOCKPP_API_IMPL(void) fail(unsigned srcline, const char* srcfile)
{
  fail(srcline, srcfile, MOCKPP_PCHAR(""));
}


MOCKPP_API_IMPL(void) assertEquals(unsigned srcline, const char* srcfile,
                                const char *expected, const char *actual)
{
  assertEquals(srcline, srcfile, MOCKPP_PCHAR(""), expected, actual);
}


MOCKPP_API_IMPL(void) assertEquals(unsigned srcline, const char* srcfile,
                                const String &message,
                                const char *expected,
                                const char *actual)
{
#if defined(_MSC_VER)
  if (strcmp(expected, actual) != 0)
#else
  if (MOCKPP_STD_NS::strcmp(expected, actual) != 0)
#endif
    fail(srcline, srcfile, message);
}


#ifdef MOCKPP_UNICODE

MOCKPP_API_IMPL(void) assertEquals(unsigned srcline, const char* srcfile,
                                   const wchar_t *expected, const wchar_t *actual)
{
  assertEquals(srcline, srcfile, MOCKPP_PCHAR(""), expected, actual);
}


MOCKPP_API_IMPL(void) assertEquals(unsigned srcline, const char* srcfile,
                                   const String &message,
                                   const wchar_t *expected,
                                   const wchar_t *actual)
{
#if !defined(HAVE_STD_WCSCMP)
  if (wcscmp(expected, actual) != 0)
#else
  if (MOCKPP_STD_NS::wcscmp(expected, actual) != 0)
#endif
    fail(srcline, srcfile, message);
}

#endif

MOCKPP_API_IMPL(void) assertDelta(unsigned srcline, const char* srcfile,
                               const String &message,
                               double expected, double actual, double delta)
{
 // handle infinity specially since subtracting to infinite values gives NaN and the

 // the following test fails

#ifdef HAVE_LIMITS
  if (expected == MOCKPP_STD_NS::numeric_limits<double>::infinity())
  {
    if (!(expected == actual))
      fail(srcline, srcfile, message);
  }
  else
#endif
#if defined(_MSC_VER)
    if (!(fabs(expected-actual) <= fabs(delta))) // Because comparison with NaN always returns false
#else
    if (!(MOCKPP_STD_NS::fabs(expected-actual) <= MOCKPP_STD_NS::fabs(delta))) // Because comparison with NaN always returns false
#endif
      fail(srcline, srcfile, message);
}


MOCKPP_API_IMPL(void) assertDelta(unsigned srcline, const char* srcfile,
                               double expected, double actual, double delta)
{
  assertDelta(srcline, srcfile, MOCKPP_PCHAR(""), expected, actual, delta);
}


MOCKPP_API_IMPL(void) assertDelta(unsigned srcline, const char* srcfile,
                               const String &message, float expected, float actual, float delta)
{
  // handle infinity specially since subtracting to infinite values gives NaN and the
  // the following test fails

#ifdef HAVE_LIMITS
  if (expected == MOCKPP_STD_NS::numeric_limits<float>::infinity())
  {
    if (!(expected == actual))
      fail(srcline, srcfile, message);
  }
  else
#endif

#if defined(_MSC_VER)
    if (!(fabs(expected-actual) <= fabs(delta)))
#else
    if (!(MOCKPP_STD_NS::fabs(expected-actual) <= MOCKPP_STD_NS::fabs(delta)))
#endif
      fail(srcline, srcfile, message);
}


MOCKPP_API_IMPL(void) assertDelta(unsigned srcline, const char* srcfile,
                               float expected, float actual, float delta)
{
  assertDelta(srcline, srcfile, MOCKPP_PCHAR(""), expected, actual, delta);
}


MOCKPP_NS_END
