/***************************************************************************
  NoArgumentsMatcher_test.cpp  -  unit tests for NoArgumentsMatcher class
                             -------------------
    begin                : Sat Aug 27 2004
    copyright            : (C) 2002-2008 by Ewald Arnold
    email                : mockpp at ewald-arnold dot de

   $Id: NoArgumentsMatcher_test.cpp 1438 2008-03-08 20:22:43Z ewald-arnold $

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

#include <mockpp/matcher/NoArgumentsMatcher.h>

#include <mockpp/chaining/Invocation.h>

#include <mockpp/compat/Asserter.h>


class NoArgumentsMatcher_test MOCKPP_DERIVE_PUBLIC_UNITFRAMEWORK
{
 public:

#if defined (MOCKPP_USE_CXXTEST)

#elif defined(MOCKPP_USE_BOOSTTEST)

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE(NoArgumentsMatcher_test );

  CPPUNIT_TEST(test_invoke);

CPPUNIT_TEST_SUITE_END();

#endif

 public:

  MOCKPP_TESTMETHOD_DECL void test_invoke();
};


#if defined (MOCKPP_USE_CXXTEST)

MOCKPP_CXXTEST_SUITE_REGISTRATION( NoArgumentsMatcher_test );
MOCKPP_CXXTEST(NoArgumentsMatcher_test, test_invoke);

#elif defined(MOCKPP_USE_BOOSTTEST)

MOCKPP_BOOST_TEST(NoArgumentsMatcher_test::test_invoke);

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE_REGISTRATION (NoArgumentsMatcher_test);

#endif // frameworks



void NoArgumentsMatcher_test::test_invoke()
{
  typedef MOCKPP_NS::Invocation0 SetterInvocation0;
  typedef MOCKPP_NS::Invocation1<int> SetterInvocation1;

  int i2 = 2;
  SetterInvocation0 s0inv(MOCKPP_PCHAR("MyNoArgumentsMatcherDummy::set_0_Prop"));
  SetterInvocation1 s1inv(MOCKPP_PCHAR("MyNoArgumentsMatcherDummy::set_1_Prop"), i2);

  MOCKPP_NS::NoArgumentsMatcher<SetterInvocation0> nam0;
  MOCKPP_NS::NoArgumentsMatcher<SetterInvocation1> nam1;

  MOCKPP_NS::String s = MOCKPP_PCHAR("hallo");
  MOCKPP_ASSERT_TRUE(nam0.describeTo(s) == MOCKPP_PCHAR("hallo(no arguments)"));
  MOCKPP_ASSERT_TRUE(s == MOCKPP_PCHAR("hallo(no arguments)"));

  MOCKPP_ASSERT_TRUE(nam0.matches(s0inv) == true);
  MOCKPP_ASSERT_TRUE(nam1.matches(s1inv) == false);
}



