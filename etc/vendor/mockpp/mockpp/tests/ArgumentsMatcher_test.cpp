/***************************************************************************
      ArgumentsMatcher_test.cpp  -  unit tests for ArgumentsMatcher class
                             -------------------
    begin                : Sat Aug 27 2004
    copyright            : (C) 2002-2008 by Ewald Arnold
    email                : mockpp at ewald-arnold dot de

  $Id: ArgumentsMatcher_test.cpp 1438 2008-03-08 20:22:43Z ewald-arnold $

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

#include <mockpp/matcher/ArgumentsMatcher.h>

#include <mockpp/compat/Asserter.h>

#include <mockpp/chaining/Invocation.h>

#include <mockpp/constraint/ConstraintSet.h>
#include <mockpp/constraint/IsEqual.h>


class ArgumentsMatcher_test MOCKPP_DERIVE_PUBLIC_UNITFRAMEWORK
{
 public:

#if defined (MOCKPP_USE_CXXTEST)

#elif defined(MOCKPP_USE_BOOSTTEST)

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE( ArgumentsMatcher_test );

  CPPUNIT_TEST(test_matches1);
  CPPUNIT_TEST(test_matches2);
  CPPUNIT_TEST(test_describe);

CPPUNIT_TEST_SUITE_END();

#endif

 public:

  MOCKPP_TESTMETHOD_DECL void test_describe();
  MOCKPP_TESTMETHOD_DECL void test_matches1();
  MOCKPP_TESTMETHOD_DECL void test_matches2();
};


#if defined (MOCKPP_USE_CXXTEST)

MOCKPP_CXXTEST_SUITE_REGISTRATION( ArgumentsMatcher_test );
MOCKPP_CXXTEST(ArgumentsMatcher_test, test_matches1);
MOCKPP_CXXTEST(ArgumentsMatcher_test, test_matches2);
MOCKPP_CXXTEST(ArgumentsMatcher_test, test_describe);

#elif defined(MOCKPP_USE_BOOSTTEST)

MOCKPP_BOOST_TEST(ArgumentsMatcher_test::test_matches1);
MOCKPP_BOOST_TEST(ArgumentsMatcher_test::test_matches2);
MOCKPP_BOOST_TEST(ArgumentsMatcher_test::test_describe);

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE_REGISTRATION (ArgumentsMatcher_test);

#endif // frameworks



void ArgumentsMatcher_test::test_matches1()
{
  MOCKPP_NS::ConstraintSet1<int> cs1 (new MOCKPP_NS::IsEqual<int>(1));

  typedef MOCKPP_NS::Invocation1<int> SetterInvocation1;
  int i1 = 1;
  int i0 = 0;
  SetterInvocation1 sinv (MOCKPP_PCHAR("MyArgumentsMatcherDummy::set_1_Prop"), i1);
  SetterInvocation1 sinva (MOCKPP_PCHAR("MyArgumentsMatcherDummy::set_1_Prop"), i0);

  MOCKPP_NS::ArgumentsMatcher<SetterInvocation1> am(cs1);
  MOCKPP_ASSERT_TRUE(am.matches(sinv) == true);
  MOCKPP_ASSERT_TRUE(am.matches(sinva) == false);
}


void ArgumentsMatcher_test::test_matches2()
{
  MOCKPP_NS::ConstraintSet2<int, int> cs2 (new MOCKPP_NS::IsEqual<int>(1),
                                        new MOCKPP_NS::IsEqual<int>(2));

  typedef MOCKPP_NS::Invocation2<int, int> SetterInvocation2;
  int i0 = 0;
  int i1 = 1;
  int i2 = 2;
  SetterInvocation2 sinv (MOCKPP_PCHAR("MyArgumentsMatcherDummy::set_2_Prop"), i1, i2);
  SetterInvocation2 sinva (MOCKPP_PCHAR("MyArgumentsMatcherDummy::set_2_Prop"), i0, i2);
  SetterInvocation2 sinvb (MOCKPP_PCHAR("MyArgumentsMatcherDummy::set_2_Prop"), i1, i0);

  MOCKPP_NS::ArgumentsMatcher<SetterInvocation2> am(cs2);
  MOCKPP_ASSERT_TRUE(am.matches(sinv) == true);
  MOCKPP_ASSERT_TRUE(am.matches(sinva) == false);
  MOCKPP_ASSERT_TRUE(am.matches(sinvb) == false);
}


void ArgumentsMatcher_test::test_describe()
{
  MOCKPP_NS::ConstraintSet2<int, int> cs2 (new MOCKPP_NS::IsEqual<int>(1),
                                        new MOCKPP_NS::IsEqual<int>(2));

  typedef MOCKPP_NS::Invocation2<int, int> SetterInvocation2;
  int i0 = 0;
  int i1 = 1;
  int i2 = 2;
  SetterInvocation2 sinv (MOCKPP_PCHAR("MyArgumentsMatcherDummy::set_2_Prop"), i1, i2);
  SetterInvocation2 sinva (MOCKPP_PCHAR("MyArgumentsMatcherDummy::set_2_Prop"), i0, i2);
  SetterInvocation2 sinvb (MOCKPP_PCHAR("MyArgumentsMatcherDummy::set_2_Prop"), i1, i0);

  MOCKPP_NS::ArgumentsMatcher<SetterInvocation2> am(cs2);
  MOCKPP_NS::String s = MOCKPP_PCHAR("cs?");
  MOCKPP_ASSERT_TRUE(am.describeTo(s) == MOCKPP_NS::String(MOCKPP_PCHAR("cs?(equalTo 1, equalTo 2)")));
  MOCKPP_ASSERT_TRUE(s == MOCKPP_PCHAR("cs?(equalTo 1, equalTo 2)"));
}



