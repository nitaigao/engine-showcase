/***************************************************************************
             StringStartsWith_test.cpp  -  unit tests for StringStartsWith class
                             -------------------
    begin                : Wed Jan 05 2005
    copyright            : (C) 2002-2008 by Ewald Arnold
    email                : mockpp at ewald-arnold dot de

  $Id: StringStartsWith_test.cpp 1438 2008-03-08 20:22:43Z ewald-arnold $

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

#include <mockpp/constraint/StringStartsWith.h>

#include <mockpp/compat/Asserter.h>


class StringStartsWith_test MOCKPP_DERIVE_PUBLIC_UNITFRAMEWORK
{
 public:

#if defined (MOCKPP_USE_CXXTEST)

#elif defined(MOCKPP_USE_BOOSTTEST)

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE( StringStartsWith_test );

  CPPUNIT_TEST(test_invoke);
  CPPUNIT_TEST(test_describe);

CPPUNIT_TEST_SUITE_END();

#endif

 public:

  MOCKPP_TESTMETHOD_DECL void test_invoke();
  MOCKPP_TESTMETHOD_DECL void test_describe();
};


#if defined (MOCKPP_USE_CXXTEST)

MOCKPP_CXXTEST_SUITE_REGISTRATION( StringStartsWith_test );
MOCKPP_CXXTEST(StringStartsWith_test, test_describe);
MOCKPP_CXXTEST(StringStartsWith_test, test_invoke);

#elif defined(MOCKPP_USE_BOOSTTEST)

MOCKPP_BOOST_TEST(StringStartsWith_test::test_describe);
MOCKPP_BOOST_TEST(StringStartsWith_test::test_invoke);

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE_REGISTRATION (StringStartsWith_test);

#endif // frameworks



void StringStartsWith_test::test_describe()
{
   MOCKPP_NS::StringStartsWith<MOCKPP_STL::string> mystr ("the-string");
   MOCKPP_NS::String s = MOCKPP_PCHAR("mystr?");
   MOCKPP_ASSERT_TRUE(mystr.describeTo(s) == MOCKPP_GET_STRING("mystr?a string starting with \"the-string")+MOCKPP_CHAR('\"'));
   MOCKPP_ASSERT_TRUE(s == MOCKPP_GET_STRING("mystr?a string starting with \"the-string")+MOCKPP_CHAR('\"'));
}


void StringStartsWith_test::test_invoke()
{
   MOCKPP_NS::StringStartsWith<MOCKPP_STL::string> mystr ("string");
   MOCKPP_ASSERT_TRUE(mystr.eval("stringxyz") == true);
   MOCKPP_ASSERT_TRUE(mystr.eval("-string-xyz") == false);
   MOCKPP_ASSERT_TRUE(mystr.eval("strin") == false);
}



