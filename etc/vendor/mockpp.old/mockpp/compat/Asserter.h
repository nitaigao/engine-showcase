/** @file
    @brief    Handle assertions

  $Id: Asserter.h 1437 2008-03-08 20:12:53Z ewald-arnold $

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

#ifndef MOCKPP_ASSERT_H
#define MOCKPP_ASSERT_H

#include <mockpp/mockpp.h> // always first

#include <mockpp/compat/AssertionFailedError.h>


MOCKPP_NS_START


/** Asserts that a condition is true. If it isn't it throws
  * an AssertionFailedError with the given infomation.
  * @ingroup grp_helper
  * @param srcline    the line in the sourcefile
  * @param srcfile    the name of the sourcefile
  * @param message    a human readable description about the cause
  * @param condition  if false, an AssertionFailedError is thrown
  */
  MOCKPP_API_DECL(void) assertTrue(unsigned srcline, const char* srcfile,
                                   const String &message, bool condition);

/** Asserts that a condition is true. If it isn't it throws
  * an AssertionFailedError.
  * @ingroup grp_helper
  * @param srcline    the line in the sourcefile
  * @param srcfile    the name of the sourcefile
  * @param condition  if false, an AssertionFailedError is thrown
  */
  MOCKPP_API_DECL(void) assertTrue(unsigned srcline, const char* srcfile,
                                   bool condition);

/** Asserts that a condition is false. If it isn't it throws
  * an AssertionFailedError with the given message.
  * @ingroup grp_helper
  * @param srcline    the line in the sourcefile
  * @param srcfile    the name of the sourcefile
  * @param message    a human readable description about the cause
  * @param condition  if true, an AssertionFailedError is thrown
  */
  MOCKPP_API_DECL(void) assertFalse(unsigned srcline, const char* srcfile,
                                    const String &message, bool condition);

/** Asserts that a condition is false. If it isn't it throws
  * an AssertionFailedError.
  * @ingroup grp_helper
  * @param srcline    the line in the sourcefile
  * @param srcfile    the name of the sourcefile
  * @param condition  if true, an AssertionFailedError is thrown
  */
  MOCKPP_API_DECL(void) assertFalse(unsigned srcline, const char* srcfile,
                                    bool condition);

/** Fails a test with the given message (AssertionFailedError is thrown).
  * @ingroup grp_helper
  * @param srcline    the line in the sourcefile
  * @param srcfile    the name of the sourcefile
  * @param message    a human readable description about the cause
  */
  MOCKPP_API_DECL(void) fail(unsigned srcline, const char* srcfile,
                             const String &message);

/** Fails a test with no message.
  * @ingroup grp_helper
  * @param srcline    the line in the sourcefile
  * @param srcfile    the name of the sourcefile
  */
  MOCKPP_API_DECL(void) fail(unsigned srcline, const char* srcfile);

/** Asserts that two doubles are equal concerning a delta.  If they are not
  * an AssertionFailedError is thrown with the given message.  If the expected
  * value is infinity then the delta value is ignored.
  * @ingroup grp_helper
  * @param srcline    the line in the sourcefile
  * @param srcfile    the name of the sourcefile
  * @param message    a human readable description about the cause
  * @param expected   the expected value
  * @param actual     the actual value
  * @param delta      the maximal difference which is allowed for actual from expected
  */
  MOCKPP_API_DECL(void) assertDelta(unsigned srcline, const char* srcfile,
                                    const String &message,
                                    double expected, double actual, double delta);

/** Asserts that two doubles are equal concerning a delta. If the expected
  * value is infinity then the delta value is ignored.
  * @ingroup grp_helper
  * @param srcline    the line in the sourcefile
  * @param srcfile    the name of the sourcefile
  * @param expected   the expected value
  * @param actual     the actual value
  * @param delta      the maximal difference which is allowed for actual from expected
  */
  MOCKPP_API_DECL(void) assertDelta(unsigned srcline, const char* srcfile,
                                    double expected, double actual, double delta);

/** Asserts that two floats are equal concerning a delta. If they are not
  * an AssertionFailedError is thrown with the given message.  If the expected
  * value is infinity then the delta value is ignored.
  * @ingroup grp_helper
  * @param srcline    the line in the sourcefile
  * @param srcfile    the name of the sourcefile
  * @param message    a human readable description about the cause
  * @param expected   the expected value
  * @param actual     the actual value
  * @param delta      the maximal difference which is allowed for actual from expected
  */
  MOCKPP_API_DECL(void) assertDelta(unsigned srcline, const char* srcfile,
                                    const String &message,
                                    float expected, float actual, float delta);


/** Asserts that two floats are equal concerning a delta. If the expected
  * value is infinity then the delta value is ignored.
  * @ingroup grp_helper
  * @param srcline    the line in the sourcefile
  * @param srcfile    the name of the sourcefile
  * @param expected   the expected value
  * @param actual     the actual value
  * @param delta      the maximal difference which is allowed for actual from expected
  */
  MOCKPP_API_DECL(void) assertDelta(unsigned srcline, const char* srcfile,
                                    float expected, float actual, float delta);

/** Asserts that two c-string are equal. If they are not
  * an AssertionFailedError is thrown with the given message.
  * @ingroup grp_helper
  * @param srcline    the line in the sourcefile
  * @param srcfile    the name of the sourcefile
  * @param message    a human readable description about the cause
  * @param expected   the expected value
  * @param actual     the actual value
  */
  MOCKPP_API_DECL(void) assertEquals(unsigned srcline, const char* srcfile,
                                     const String &message,
                                     const char *expected, const char *actual);

/** Asserts that two c-string are equal.
  * @ingroup grp_helper
  * @param srcline    the line in the sourcefile
  * @param srcfile    the name of the sourcefile
  * @param expected   the expected value
  * @param actual     the actual value
  */
  MOCKPP_API_DECL(void) assertEquals(unsigned srcline, const char* srcfile,
                                     const char *expected, const char *actual);

#ifdef MOCKPP_UNICODE

/** Asserts that two unicode c-string are equal. If they are not
  * an AssertionFailedError is thrown with the given message.
  * @ingroup grp_helper
  * @param srcline    the line in the sourcefile
  * @param srcfile    the name of the sourcefile
  * @param message    a human readable description about the cause
  * @param expected   the expected value
  * @param actual     the actual value
  */
  MOCKPP_API_DECL(void) 
     assertEquals(unsigned srcline, const char* srcfile,
                  const String &message,
                  const wchar_t *expected, const wchar_t *actual);

/** Asserts that two unicode c-string are equal.
  * @ingroup grp_helper
  * @param srcline    the line in the sourcefile
  * @param srcfile    the name of the sourcefile
  * @param expected   the expected value
  * @param actual     the actual value
  */
  MOCKPP_API_DECL(void)
     assertEquals(unsigned srcline, const char* srcfile,
                  const wchar_t *expected, const wchar_t *actual);

#endif

/** Asserts that two values are equal.
  * @ingroup grp_helper
  * @param srcline    the line in the sourcefile
  * @param srcfile    the name of the sourcefile
  * @param message    a human readable description about the cause
  * @param expected   the expected value
  * @param actual     the actual value
  */
  template <class T>
  void assertEquals(unsigned srcline, const char* srcfile,
                    const String &message, const T &expected, const T &actual)
  {
    if (expected != actual)
      fail(srcline, srcfile, message);
  }


/** Asserts that two values are equal.
  * @ingroup grp_helper
  * @param srcline    the line in the sourcefile
  * @param srcfile    the name of the sourcefile
  * @param expected   the expected value
  * @param actual     the actual value
  */
  template <class T>
  void assertEquals(unsigned srcline, const char* srcfile,
                    const T &expected, const T &actual)
  {
    assertEquals(srcline, srcfile, MOCKPP_PCHAR(""), expected, actual);
  }


/** Asserts that two values are equal concerning a delta.  If they are not
  * an AssertionFailedError is thrown with the given message.
  * @ingroup grp_helper
  * @param srcline    the line in the sourcefile
  * @param srcfile    the name of the sourcefile
  * @param message    a human readable description about the cause
  * @param expected   the expected value
  * @param actual     the actual value
  * @param delta      the maximal difference which is allowed for actual from expected
  */
template <class T>
void assertDelta(unsigned srcline, const char* srcfile,
                 const String &message,
                 const T &expected, const T &actual, const T &delta)
{
  T diff = expected < actual
         ? actual   - expected
         : expected - actual;
  if (diff > delta)
    fail(srcline, srcfile, message);
}


/** Asserts that two values are equal concerning a delta.
  * @ingroup grp_helper
  * @param srcline    the line in the sourcefile
  * @param srcfile    the name of the sourcefile
  * @param expected   the expected value
  * @param actual     the actual value
  * @param delta      the maximal difference which is allowed for actual from expected
  */
  template <class T>
  void assertDelta(unsigned srcline, const char* srcfile,
                   const T &expected, const T &actual, const T &delta)
  {
    assertDelta(srcline, srcfile, MOCKPP_PCHAR(""), expected, actual, delta);
  }


/** Asserts that a values lies between two boundary values.
  * @ingroup grp_helper
  * @param srcline    the line in the sourcefile
  * @param srcfile    the name of the sourcefile
  * @param message   a human readable description about the cause
  * @param lo         lower boundaray
  * @param up         upper boundary
  * @param actual     value
  */
  template <class T>
  void assertBoundary(unsigned srcline, const char* srcfile,
                      const String &message,
                      const T &lo, const T &up, const T &actual)
  {
    if ((actual < lo) || (up < actual))
      fail(srcline, srcfile, message);
  }


/** Asserts that a values lies between two boundary values.
  * @ingroup grp_helper
  * @param srcline    the line in the sourcefile
  * @param srcfile    the name of the sourcefile
  * @param lo         lower boundaray
  * @param up         upper boundary
  * @param actual     value
  */
  template <class T>
  void assertBoundary(unsigned srcline, const char* srcfile,
                      const T &lo, const T &up, const T &actual)
  {
    assertBoundary(srcline, srcfile, MOCKPP_PCHAR(""), lo, up, actual);
  }


MOCKPP_NS_END


/** Asserts that two values are equal.
  * @ingroup grp_helper
  * @param msg        a human readable description about the cause
  * @param a          first value
  * @param b          second value
  */
#define MOCKPP_ASSERT_EQUALS_MESSAGE(msg, a, b)  MOCKPP_NS::assertEquals(__LINE__, __FILE__, msg, a, b)

/** @def MOCKPP_ASSERT_EQUALS
  * @ingroup grp_helper
  * Asserts that two values are equal.
  * The condition is converted and passed as the according message.
  * @param a          first value
  * @param b          second value
  */

#define MOCKPP_ASSERT_EQUALS(a, b)  MOCKPP_NS::assertEquals(__LINE__, __FILE__, MOCKPP_PCHAR("invalid: ") \
                                                          MOCKPP_PCHAR(#a) MOCKPP_PCHAR(" == ") MOCKPP_PCHAR(#b), a, b)

/** Asserts that two values are equal concerning a delta.
  * @ingroup grp_helper
  * @param msg        a human readable description about the cause
  * @param a          first value
  * @param b          second value
  * @param delta      maximal difference a to b
  */
#define MOCKPP_ASSERT_DELTA_MESSAGE(msg, a, b, delta)  MOCKPP_NS::assertDelta(__LINE__, __FILE__, msg, a, b, delta)

/** @def MOCKPP_ASSERT_DELTA
  * @ingroup grp_helper
  * Asserts that two values are equal concerning a delta.
  * The condition is converted and passed as the according message.
  * @param a          first value
  * @param b          second value
  * @param delta      maximal difference a to b
  */

# define MOCKPP_ASSERT_DELTA(a, b, delta)  MOCKPP_NS::assertDelta(__LINE__, __FILE__, MOCKPP_PCHAR("invalid: ") \
                                                               MOCKPP_PCHAR(#a) MOCKPP_PCHAR(" +- ") MOCKPP_PCHAR(#delta) \
                                                               MOCKPP_PCHAR(" == ") MOCKPP_PCHAR(#b), a, b, delta)

/** Asserts that a values lies between two boundary values.
  * @ingroup grp_helper
  * @param msg        a human readable description about the cause
  * @param lo         lower boundaray
  * @param up         upper boundary
  * @param a          value
  */
#define MOCKPP_ASSERT_BOUNDARY_MESSAGE(msg, lo, up, a)  MOCKPP_NS::assertBoundary(__LINE__, __FILE__, msg, lo, up, a)

/** @def MOCKPP_ASSERT_BOUNDARY
  * @ingroup grp_helper
  * Asserts that two values are equal concerning a delta.
  * The condition is converted and passed as the according message.
  * @param lo         lower boundaray
  * @param up         upper boundary
  * @param a          value
  */

# define MOCKPP_ASSERT_BOUNDARY(lo, up, a)  MOCKPP_NS::assertBoundary(__LINE__, __FILE__, MOCKPP_PCHAR("invalid: ") \
                                                                   MOCKPP_PCHAR(#lo) MOCKPP_PCHAR(" < ") MOCKPP_PCHAR(#a) \
                                                                   MOCKPP_PCHAR(" < ") MOCKPP_PCHAR(#up), lo, up, a)

/** Asserts that a condition is true.
  * @ingroup grp_helper
  * @param msg        a human readable description about the cause
  * @param cond       boolean condition
  */
#define MOCKPP_ASSERT_TRUE_MESSAGE(msg, cond) MOCKPP_NS::assertTrue(__LINE__, __FILE__, msg, cond)

/** @def MOCKPP_ASSERT_TRUE
  * @ingroup grp_helper
  * Asserts that a condition is true.
  * The condition is converted and passed as the according message.
  * @param cond       boolean condition
  */

# define MOCKPP_ASSERT_TRUE(cond) MOCKPP_NS::assertTrue(__LINE__, __FILE__, MOCKPP_PCHAR("!true: ") MOCKPP_PCHAR(#cond), cond)

/** Asserts that a condition is false.
  * @ingroup grp_helper
  * @param msg        a human readable description about the cause
  * @param cond       boolean condition
  */
#define MOCKPP_ASSERT_FALSE_MESSAGE(msg, cond) MOCKPP_NS::assertFalse(__LINE__, __FILE__, msg, cond)

/** @def MOCKPP_ASSERT_FALSE
  * @ingroup grp_helper
  * Asserts that a condition is false.
  * The condition is converted and passed as the according message.
  * @param cond       boolean condition
  */

# define MOCKPP_ASSERT_FALSE(cond) MOCKPP_NS::assertFalse(__LINE__, __FILE__, MOCKPP_PCHAR("!false: ") MOCKPP_PCHAR(#cond), cond)

/** Unconditionally throws an AssertionFailedError.
  * @ingroup grp_helper
  * @param msg        a human readable description about the cause
  */
#define MOCKPP_FAIL(msg)  MOCKPP_NS::fail(__LINE__, __FILE__, msg)


#endif // MOCKPP_ASSERT_H

