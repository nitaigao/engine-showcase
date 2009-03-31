/***************************************************************************
             IsCloseTo_test.cpp  -  unit tests for IsCloseTo class
                             -------------------
    begin                : Sat Aug 21 2004
    copyright            : (C) 2002-2008 by Ewald Arnold
    email                : mockpp at ewald-arnold dot de

  $Id: IsCloseTo_test.cpp 1438 2008-03-08 20:22:43Z ewald-arnold $

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

#include <mockpp/constraint/IsCloseTo.h>

#include <mockpp/compat/Asserter.h>


class IsCloseTo_test MOCKPP_DERIVE_PUBLIC_UNITFRAMEWORK
{
 public:

#if defined (MOCKPP_USE_CXXTEST)

#elif defined(MOCKPP_USE_BOOSTTEST)

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE( IsCloseTo_test );

  CPPUNIT_TEST(test_describe);
  CPPUNIT_TEST(test_invoke);

CPPUNIT_TEST_SUITE_END();

#endif

 public:

  MOCKPP_TESTMETHOD_DECL void test_describe();
  MOCKPP_TESTMETHOD_DECL void test_invoke();
};


#if defined (MOCKPP_USE_CXXTEST)

MOCKPP_CXXTEST_SUITE_REGISTRATION( IsCloseTo_test );
MOCKPP_CXXTEST(IsCloseTo_test, test_describe);
MOCKPP_CXXTEST(IsCloseTo_test, test_invoke);

#elif defined(MOCKPP_USE_BOOSTTEST)

MOCKPP_BOOST_TEST(IsCloseTo_test::test_describe);
MOCKPP_BOOST_TEST(IsCloseTo_test::test_invoke);

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE_REGISTRATION (IsCloseTo_test);

#endif // frameworks



void IsCloseTo_test::test_describe()
{
   MOCKPP_NS::IsCloseTo<int> myclose (12, 34);
   MOCKPP_NS::String s = MOCKPP_PCHAR("close?");
   MOCKPP_ASSERT_TRUE(myclose.describeTo(s) == MOCKPP_PCHAR("close?a numeric value within +-34 of 12"));
   MOCKPP_ASSERT_TRUE(s == MOCKPP_PCHAR("close?a numeric value within +-34 of 12"));
}


void IsCloseTo_test::test_invoke()
{
   MOCKPP_NS::IsCloseTo<float> myclose1 (2.0f, 0.5f);
   MOCKPP_ASSERT_TRUE(myclose1.eval(2) == true);
   MOCKPP_ASSERT_TRUE(myclose1.eval(1) == false);
   MOCKPP_ASSERT_TRUE(myclose1.eval(3) == false);

   MOCKPP_NS::IsCloseTo<float> myclose1b (2.0f, -0.5f);
   MOCKPP_ASSERT_TRUE(myclose1b.eval(2) == true);
   MOCKPP_ASSERT_TRUE(myclose1b.eval(1) == false);
   MOCKPP_ASSERT_TRUE(myclose1b.eval(3) == false);

   MOCKPP_NS::IsCloseTo<float> myclose2 (-2.0f, 0.5f);
   MOCKPP_ASSERT_TRUE(myclose2.eval(-2) == true);
   MOCKPP_ASSERT_TRUE(myclose2.eval(-1) == false);
   MOCKPP_ASSERT_TRUE(myclose2.eval(-3) == false);

   MOCKPP_NS::IsCloseTo<float> myclose2a (-2.0f, -0.5f);
   MOCKPP_ASSERT_TRUE(myclose2a.eval(-2) == true);
   MOCKPP_ASSERT_TRUE(myclose2a.eval(-1) == false);
   MOCKPP_ASSERT_TRUE(myclose2a.eval(-3) == false);

   MOCKPP_NS::IsCloseTo<unsigned> myclose3a (20, 1);
   MOCKPP_ASSERT_TRUE(myclose3a.eval(20) == true);
   MOCKPP_ASSERT_TRUE(myclose3a.eval(21) == true);
   MOCKPP_ASSERT_TRUE(myclose3a.eval(19) == true);
   MOCKPP_ASSERT_TRUE(myclose3a.eval(22) == false);
   MOCKPP_ASSERT_TRUE(myclose3a.eval(18) == false);
}



