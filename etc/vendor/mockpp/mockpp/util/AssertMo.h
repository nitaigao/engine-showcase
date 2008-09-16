/** @file
    @brief    Handle assertions of mock objects

  $Id: AssertMo.h 1437 2008-03-08 20:12:53Z ewald-arnold $

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

#ifndef MOCKPP_ASSERTMO_H
#define MOCKPP_ASSERTMO_H

#include <mockpp/mockpp.h> // always first

#include <mockpp/compat/Asserter.h>
#include <mockpp/compat/Formatter.h>


MOCKPP_NS_START


class Verifiable;

/** Asserts that a string does not contain a substring. If it does anyway it throws
  * an AssertionFailedError with the given infomation.
  * @ingroup grp_helper
  * @param srcline       the line in the sourcefile
  * @param srcfile       the name of the sourcefile
  * @param description   a human readable description about the cause
  * @param excludeString substring which should not occur
  * @param targetString  string in which is searched
  */
MOCKPP_API_DECL(void) assertExcludes(unsigned srcline, const char* srcfile,
				     const String &description,
				     const MOCKPP_STL::string &excludeString,
				     const MOCKPP_STL::string &targetString);

#ifdef MOCKPP_UNICODE

/** Asserts that a string does not contain a substring. If it does anyway it throws
  * an AssertionFailedError with the given infomation.
  * @ingroup grp_helper
  * @param srcline       the line in the sourcefile
  * @param srcfile       the name of the sourcefile
  * @param description   a human readable description about the cause
  * @param excludeString substring which should not occur
  * @param targetString  string in which is searched
  */
MOCKPP_API_DECL(void) assertExcludes(unsigned srcline, const char* srcfile,
				     const String &description,
				     const MOCKPP_STL::basic_string<wchar_t> &excludeString,
				     const MOCKPP_STL::basic_string<wchar_t> &targetString);

#endif

/** Asserts that a string does contain a substring. If it does not it throws
  * an AssertionFailedError with the given infomation.
  * @ingroup grp_helper
  * @param srcline       the line in the sourcefile
  * @param srcfile       the name of the sourcefile
  * @param description   a human readable description about the cause
  * @param includeString substring which must occur
  * @param targetString  string in which is searched
  */
MOCKPP_API_DECL(void) assertIncludes(unsigned srcline, const char* srcfile,
				     const String &description,
				     const MOCKPP_STL::string &includeString,
				     const MOCKPP_STL::string &targetString);

#ifdef MOCKPP_UNICODE

/** Asserts that a string does contain a substring. If it does not it throws
  * an AssertionFailedError with the given infomation.
  * @ingroup grp_helper
  * @param srcline       the line in the sourcefile
  * @param srcfile       the name of the sourcefile
  * @param description   a human readable description about the cause
  * @param includeString substring which must occur
  * @param targetString  string in which is searched
  */
MOCKPP_API_DECL(void) assertIncludes(unsigned srcline, const char* srcfile,
				     const String &description,
				     const MOCKPP_STL::basic_string<wchar_t> &includeString,
				     const MOCKPP_STL::basic_string<wchar_t> &targetString);

#endif

/** Asserts that a string starts with a substring. If it does not it throws
  * an AssertionFailedError with the given infomation.
  * @ingroup grp_helper
  * @param srcline       the line in the sourcefile
  * @param srcfile       the name of the sourcefile
  * @param description   a human readable description about the cause
  * @param startString   string which must occur at the beginning
  * @param targetString  string in which is searched
  */
MOCKPP_API_DECL(void) assertStartsWith(unsigned srcline, const char* srcfile,
				       const String &description,
				       const MOCKPP_STL::string &startString,
				       const MOCKPP_STL::string &targetString);

#ifdef MOCKPP_UNICODE

/** Asserts that a string starts with a substring. If it does not it throws
  * an AssertionFailedError with the given infomation.
  * @ingroup grp_helper
  * @param srcline       the line in the sourcefile
  * @param srcfile       the name of the sourcefile
  * @param description   a human readable description about the cause
  * @param startString   string which must occur at the beginning
  * @param targetString  string in which is searched
  */
MOCKPP_API_DECL(void) assertStartsWith(unsigned srcline, const char* srcfile,
				       const String &description,
                                       const MOCKPP_STL::basic_string<wchar_t> &startString,
				       const MOCKPP_STL::basic_string<wchar_t> &targetString);

#endif

/** Asserts that a verify() of an object fails. If it does not it throws
  * an AssertionFailedError.
  * @ingroup grp_helper
  * @param srcline       the line in the sourcefile
  * @param srcfile       the name of the sourcefile
  * @param aVerifiable   object to be verified
  */
MOCKPP_API_DECL(void) assertVerifyFails(unsigned srcline, const char* srcfile,
		                        Verifiable *aVerifiable);

/** Throws unconditionally a NotImplementedException with the given name to
  * indicate a not yet implemented method.
  * @ingroup grp_helper
  * @param srcline       the line in the sourcefile
  * @param srcfile       the name of the sourcefile
  * @param mockName      name of the object
  */
MOCKPP_API_DECL(void) notImplemented(unsigned srcline, const char* srcfile,
				     const String &mockName);


MOCKPP_NS_END


/** Asserts that a string does contain a substring. If it does not it throws
  * an AssertionFailedError with the given infomation.
  * @ingroup grp_helper
  * @param msg           a human readable description about the cause
  * @param a             string which must occur at the beginning
  * @param b             string in which is searched
  */
#define MOCKPP_ASSERT_INCLUDES_MESSAGE(msg, a, b)  MOCKPP_NS::assertIncludes(__LINE__, __FILE__, msg, a, b)

/** @def MOCKPP_ASSERT_INCLUDES
  * Asserts that a string does contain a substring. If it does not it throws
  * an AssertionFailedError with the given infomation.
  * The condition is converted and passed as the according message.
  * @ingroup grp_helper
  * @param a             string which must occur at the beginning
  * @param b             string in which is searched
  */

# define MOCKPP_ASSERT_INCLUDES(a, b)  MOCKPP_NS::assertIncludes(__LINE__, __FILE__, MOCKPP_PCHAR(#a) MOCKPP_PCHAR(" <in> ") MOCKPP_PCHAR(#b), a, b)

/** Asserts that a string does not contain a substring. If it does it throws
  * an AssertionFailedError with the given infomation.
  * @ingroup grp_helper
  * @param msg           a human readable description about the cause
  * @param a             string which must occur at the beginning
  * @param b             string in which is searched
  */
#define MOCKPP_ASSERT_EXCLUDES_MESSAGE(msg, a, b)  MOCKPP_NS::assertExcludes(__LINE__, __FILE__, msg, a, b)

/** @def MOCKPP_ASSERT_EXCLUDES
  * Asserts that a string does not contain a substring. If it does it throws
  * an AssertionFailedError with the given infomation.
  * The condition is converted and passed as the according message.
  * @ingroup grp_helper
  * @param a             string which must occur at the beginning
  * @param b             string in which is searched
  */

#define MOCKPP_ASSERT_EXCLUDES(a, b)  MOCKPP_NS::assertExcludes(__LINE__, __FILE__, MOCKPP_PCHAR(#a) MOCKPP_PCHAR(" <!in> ") MOCKPP_PCHAR(#b), a, b)

/** Asserts that a string starts with a substring. If it does not it throws
  * an AssertionFailedError with the given infomation.
  * @ingroup grp_helper
  * @param msg           a human readable description about the cause
  * @param a             string which must occur at the beginning
  * @param b             string in which is searched
  */
#define MOCKPP_ASSERT_STARTSWITH_MESSAGE(msg, a, b)  MOCKPP_NS::assertStartsWith(__LINE__, __FILE__, msg, a, b)

/** @def MOCKPP_ASSERT_STARTSWITH
  * Asserts that a string starts with a substring. If it does not it throws
  * an AssertionFailedError with the given infomation.
  * The condition is converted and passed as the according message.
  * @ingroup grp_helper
  * @param a             string which must occur at the beginning
  * @param b             string in which is searched
  */

#define MOCKPP_ASSERT_STARTSWITH(a, b)  MOCKPP_NS::assertStartsWith(__LINE__, __FILE__, MOCKPP_PCHAR(#a) MOCKPP_PCHAR(" <starts> ") MOCKPP_PCHAR(#b), a, b)

/** Asserts that a verify() of an object fails. If it does not it throws
  * an AssertionFailedError.
  * @ingroup grp_helper
  * @param obj   object to be verified
  */
#define MOCKPP_ASSERT_VERIFYFAILS(obj)  MOCKPP_NS::assertVerifyFails(__LINE__, __FILE__, obj)

/** Throws unconditionally a NotImplementedException with the given name to
  * indicate a not yet implemented method.
  * @ingroup grp_helper
  * @param msg   name of the object
  */
#define MOCKPP_NOT_IMPLEMENTED(msg)  MOCKPP_NS::notImplemented(__LINE__, __FILE__, msg)

// @todo test
/** Asserts that an exception is thrown and that the data is correctly
  * transfered to the catch clause.
  * \code
  *     MOCKPP_ASSERT_THROWING(object.call(),
  *                            "default response",
  *                            MOCKPP_STL::string,
  *                            MOCKPP_STL::string("throw default"));
  * \endcode
  * @ingroup grp_helper
  * @param action        C++-statement to execute and which is expected to throw
  * @param action_name   decription for the action
  * @param except        exception type to be thrown and caught
  * @param except_data   the exact data the exception contains.
  */
#define MOCKPP_ASSERT_THROWING(action, action_name, except, except_data) \
  try \
  { \
    action; \
    MOCKPP_FAIL(MOCKPP_PCHAR("Did not throw the exception: ") + MOCKPP_STRING(action_name)); \
  } \
  catch(except &exception_object) \
  { \
    MOCKPP_ASSERT_TRUE(exception_object == except_data); \
  } \
  catch(MOCKPP_STD_NS::exception &ex)  \
  { \
    MOCKPP_ASSERT_TRUE_MESSAGE(MOCKPP_GET_STRING(ex.what()), false); \
  } \
  catch(...) \
  { \
    MOCKPP_FAIL(MOCKPP_PCHAR("Caught unknown exception: ") + MOCKPP_STRING(action_name)); \
  }

/** Asserts that an exception is thrown and that the data is correctly
  * transfered to the catch clause.
  * \code
  *     MOCKPP_ASSERT_THROWING_COND(object.call(),
  *                                 "default response",
  *                                 MOCKPP_STL::string,
  *                                 exception_object.find("throw default") != MOCKPP_STL::string::npos);
  * \endcode
  * @ingroup grp_helper
  * @param action        C++-statement to execute and throw
  * @param action_name   decription for the action
  * @param except        exception type to be thrown and caught
  * @param condition     condition which must be true. The exception object itself
  *                      can be accessed by reference to \c exception_object
  */
#define MOCKPP_ASSERT_THROWING_COND(action, action_name, except, condition) \
  try \
  { \
    action; \
    MOCKPP_FAIL(MOCKPP_PCHAR("Did not throw the exception: ") + MOCKPP_STRING(action_name)); \
  } \
  catch(except &exception_object) \
  { \
    MOCKPP_ASSERT_TRUE(condition); \
  } \
  catch(MOCKPP_STD_NS::exception &ex)  \
  { \
    MOCKPP_ASSERT_TRUE_MESSAGE(MOCKPP_GET_STRING(ex.what()), false); \
  } \
  catch(...) \
  { \
    MOCKPP_FAIL(MOCKPP_PCHAR("Caught unknown exception: ") + MOCKPP_STRING(action_name)); \
  }


/** Asserts that an exception is thrown and that the data is correctly
  * transfered to the catch clause.
  * \code
  *     MOCKPP_ASSERT_THROWING_EQUALS(object.call(),
  *                                 "default response",
  *                                 std::string,
  *                                 std::string("except-data",
  *                                 exception.object);
  * \endcode
  * @ingroup grp_helper
  * @param action        C++-statement to execute and throw
  * @param action_name   decription for the action
  * @param except        exception type to be thrown and caught
  * @param expected      the expected value from the exception. The exception
  *                      object itself can be accessed by reference to \c exception_object
  * @param actual        the actual value from the exception
  */
#define MOCKPP_ASSERT_THROWING_EQUALS(action, action_name, except, expected, actual) \
  try \
  { \
    action; \
    MOCKPP_FAIL(MOCKPP_PCHAR("Did not throw the exception: ") + MOCKPP_STRING(action_name)); \
  } \
  catch(except &exception_object) \
  { \
    MOCKPP_ASSERT_EQUALS(expected, actual); \
  } \
  catch(std::exception &ex)  \
  { \
    MOCKPP_ASSERT_TRUE_MESSAGE(MOCKPP_GET_STRING(ex.what()), false); \
  } \
  catch(...) \
  { \
    MOCKPP_FAIL(MOCKPP_PCHAR("Caught unknown exception: ") + MOCKPP_STRING(action_name)); \
  }


#endif // MOCKPP_ASSERTMO_H
