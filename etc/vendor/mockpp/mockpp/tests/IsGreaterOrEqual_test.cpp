/***************************************************************************
             IsGreaterOrEqual_test.cpp  -  unit tests for IsGreaterOrEqual class
                             -------------------
    begin                : Sat Aug 21 2004
    copyright            : (C) 2002-2008 by Ewald Arnold
    email                : mockpp at ewald-arnold dot de

  $Id: IsGreaterOrEqual_test.cpp 1438 2008-03-08 20:22:43Z ewald-arnold $

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

#include <mockpp/constraint/IsGreaterOrEqual.h>

#include <mockpp/compat/Asserter.h>


class IsGreaterOrEqual_test MOCKPP_DERIVE_PUBLIC_UNITFRAMEWORK
{
 public:

#if defined (MOCKPP_USE_CXXTEST)

#elif defined(MOCKPP_USE_BOOSTTEST)

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE( IsGreaterOrEqual_test );

  CPPUNIT_TEST(test_describe);
  CPPUNIT_TEST(test_invoke);

CPPUNIT_TEST_SUITE_END();

#endif

 public:

  MOCKPP_TESTMETHOD_DECL void test_describe();
  MOCKPP_TESTMETHOD_DECL void test_invoke();
};


#if defined (MOCKPP_USE_CXXTEST)

MOCKPP_CXXTEST_SUITE_REGISTRATION( IsGreaterOrEqual_test );
MOCKPP_CXXTEST(IsGreaterOrEqual_test, test_describe);
MOCKPP_CXXTEST(IsGreaterOrEqual_test, test_invoke);

#elif defined(MOCKPP_USE_BOOSTTEST)

MOCKPP_BOOST_TEST(IsGreaterOrEqual_test::test_describe);
MOCKPP_BOOST_TEST(IsGreaterOrEqual_test::test_invoke);

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE_REGISTRATION (IsGreaterOrEqual_test);

#endif // frameworks



void IsGreaterOrEqual_test::test_describe()
{
   MOCKPP_NS::IsGreaterOrEqual<int> greaterEqual (12);
   MOCKPP_NS::String s = MOCKPP_PCHAR("IsGreaterOrEqual?");
   MOCKPP_ASSERT_TRUE(greaterEqual.describeTo(s) == MOCKPP_PCHAR("IsGreaterOrEqual?greaterOrEqual 12"));
   MOCKPP_ASSERT_TRUE(s == MOCKPP_PCHAR("IsGreaterOrEqual?greaterOrEqual 12"));
}


void IsGreaterOrEqual_test::test_invoke()
{
   MOCKPP_NS::IsGreaterOrEqual<int> greaterEqual (10);
   MOCKPP_ASSERT_TRUE(greaterEqual.eval(11) == true);
   MOCKPP_ASSERT_TRUE(greaterEqual.eval(10) == true);
   MOCKPP_ASSERT_TRUE(greaterEqual.eval(9) == false);
}



