/***************************************************************************
        Exception_test.cpp  -  unit tests for Exception class
                             -------------------
    begin                : Wed Jan 1 2003
    copyright            : (C) 2002-2008 by Ewald Arnold
    email                : mockpp at ewald-arnold dot de

  $Id: Exception_test.cpp 1438 2008-03-08 20:22:43Z ewald-arnold $

 ***************************************************************************/

/**************************************************************************
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 2 of the License,
 * or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 ***************************************************************************/

#include <mockpp/mockpp.h> // always first

#include "SelectFramework.h"

#include <mockpp/compat/Exception.h>
#include <mockpp/compat/Asserter.h>


class Exception_test MOCKPP_DERIVE_PUBLIC_UNITFRAMEWORK
{
 public:

#if defined (MOCKPP_USE_CXXTEST)

#elif defined(MOCKPP_USE_BOOSTTEST)

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE( Exception_test );

  CPPUNIT_TEST(test_file_line);

CPPUNIT_TEST_SUITE_END();

#endif

 public:

  MOCKPP_TESTMETHOD_DECL void test_file_line();
};


#if defined (MOCKPP_USE_CXXTEST)

MOCKPP_CXXTEST_SUITE_REGISTRATION( Exception_test );
MOCKPP_CXXTEST(Exception_test, test_file_line);

#elif defined(MOCKPP_USE_BOOSTTEST)

MOCKPP_BOOST_TEST(Exception_test::test_file_line);

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE_REGISTRATION (Exception_test);

#endif // frameworks



void Exception_test::test_file_line()
{
  bool thrown = false;
  unsigned l = 0;
  const char* f = 0;
  MOCKPP_TRY
  {
    // keep on the same line
    l = __LINE__; f = __FILE__; MOCKPP_FAIL(MOCKPP_PCHAR("test-assert\n\rline 2"));
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_NS::AssertionFailedError &ex)
  {
    MOCKPP_ASSERT_EQUALS_MESSAGE(MOCKPP_PCHAR("error line parameter"), (int)l, (int)ex.getSrcLine());

    MOCKPP_ASSERT_EQUALS_MESSAGE(MOCKPP_PCHAR("error file parameter"),
                                 MOCKPP_STL::string(f), MOCKPP_NS::getLatin1(ex.getSrcFile()));

    MOCKPP_ASSERT_EQUALS_MESSAGE(MOCKPP_PCHAR("error message parameter <MOCKPP_NS::String>"),
                                 MOCKPP_PCSTRING("test-assert\n\rline 2"), ex.getMessage());

    MOCKPP_ASSERT_EQUALS_MESSAGE(MOCKPP_PCHAR("error message parameter <MOCKPP_STL::string>"),
                                 MOCKPP_STL::string("test-assert\n\rline 2"), MOCKPP_STL::string(ex.what()));
    thrown = true;
  }
#endif

  MOCKPP_ASSERT_TRUE_MESSAGE(MOCKPP_PCHAR("Should have thrown"), thrown);
}



