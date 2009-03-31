/***************************************************************************
           ReturnObjectListStub_test.cpp  -  unit tests for ReturnObjectListStub class
                             -------------------
    begin                : Sat Aug 19 2006
    copyright            : (C) 2002-2008 by Ewald Arnold
    email                : mockpp at ewald-arnold dot de

  $Id: ReturnObjectListStub_test.cpp 1438 2008-03-08 20:22:43Z ewald-arnold $

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

#include <mockpp/stub/ReturnObjectListStub.h>


class ReturnObjectListStub_test MOCKPP_DERIVE_PUBLIC_UNITFRAMEWORK
{
 public:

#if defined (MOCKPP_USE_CXXTEST)

#elif defined(MOCKPP_USE_BOOSTTEST)

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE( ReturnObjectListStub_test );

  CPPUNIT_TEST(test_invoke);
  CPPUNIT_TEST(test_describe);

CPPUNIT_TEST_SUITE_END();

#endif

 public:

  MOCKPP_TESTMETHOD_DECL void test_describe();
  MOCKPP_TESTMETHOD_DECL void test_invoke();
};


#if defined (MOCKPP_USE_CXXTEST)

MOCKPP_CXXTEST_SUITE_REGISTRATION( ReturnObjectListStub_test );
MOCKPP_CXXTEST(ReturnObjectListStub_test, test_describe);
MOCKPP_CXXTEST(ReturnObjectListStub_test, test_invoke);

#elif defined(MOCKPP_USE_BOOSTTEST)

MOCKPP_BOOST_TEST(ReturnObjectListStub_test::test_describe);
MOCKPP_BOOST_TEST(ReturnObjectListStub_test::test_invoke);

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE_REGISTRATION (ReturnObjectListStub_test);

#endif // frameworks



void ReturnObjectListStub_test::test_describe()
{
  typedef int Invocation;

  MOCKPP_NS::ReturnObjectListStub<int> ds (1234);
  MOCKPP_NS::String b = MOCKPP_GET_STRING("abcd");
  MOCKPP_NS::String s = ds.describeTo(b);
  MOCKPP_ASSERT_TRUE(s.find(MOCKPP_PCHAR("abcd")) != MOCKPP_NS::String::npos);
  MOCKPP_ASSERT_TRUE(s.find(MOCKPP_PCHAR("1234")) != MOCKPP_NS::String::npos);
  MOCKPP_ASSERT_TRUE(b.find(MOCKPP_PCHAR("abcd")) != MOCKPP_NS::String::npos);
  MOCKPP_ASSERT_TRUE(b.find(MOCKPP_PCHAR("1234")) != MOCKPP_NS::String::npos);
}


void ReturnObjectListStub_test::test_invoke()
{
  typedef int Invocation;

  {
    MOCKPP_NS::ReturnObjectListStub<int> ds (1234);
    int intvec2[] = { 2, 4 };
    ds.addObjectToReturn(&intvec2[0], &intvec2[2]);
    int val = ds.typelessInvoke();
    MOCKPP_ASSERT_TRUE(val == 1234);
    val = ds.typelessInvoke();
    MOCKPP_ASSERT_TRUE(val == 2);
    val = ds.typelessInvoke();
    MOCKPP_ASSERT_TRUE(val == 4);
  }

  // --

  {
    int intvec2[] = { 2, 4 };
    MOCKPP_NS::ReturnObjectListStub<int> ds (&intvec2[0], &intvec2[2]);
    ds.addObjectToReturn(17);
    int val = ds.typelessInvoke();
    MOCKPP_ASSERT_TRUE(val == 2);
    val = ds.typelessInvoke();
    MOCKPP_ASSERT_TRUE(val == 4);
    val = ds.typelessInvoke();
    MOCKPP_ASSERT_TRUE(val == 17);
  }
}



