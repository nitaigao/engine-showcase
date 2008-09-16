/***************************************************************************
           RandomStub_test.cpp  -  unit tests for RandomStub class
                             -------------------
    begin                : Thu Sep 23 2006
    copyright            : (C) 2002-2008 by Ewald Arnold
    email                : mockpp at ewald-arnold dot de

  $Id: RandomStub_test.cpp 1438 2008-03-08 20:22:43Z ewald-arnold $

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

//#include <iostream>

#include "SelectFramework.h"

#include <mockpp/stub/RandomStub.h>


class RandomStub_test MOCKPP_DERIVE_PUBLIC_UNITFRAMEWORK
{
 public:

#if defined (MOCKPP_USE_CXXTEST)

#elif defined(MOCKPP_USE_BOOSTTEST)

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE( RandomStub_test );

  CPPUNIT_TEST(test_invoke);
  CPPUNIT_TEST(test_describe);

CPPUNIT_TEST_SUITE_END();

#endif

 public:

  MOCKPP_TESTMETHOD_DECL void test_describe();
  MOCKPP_TESTMETHOD_DECL void test_invoke();
};


#if defined (MOCKPP_USE_CXXTEST)

MOCKPP_CXXTEST_SUITE_REGISTRATION( RandomStub_test );
MOCKPP_CXXTEST(RandomStub_test, test_describe);
MOCKPP_CXXTEST(RandomStub_test, test_invoke);

#elif defined(MOCKPP_USE_BOOSTTEST)

MOCKPP_BOOST_TEST(RandomStub_test::test_describe);
MOCKPP_BOOST_TEST(RandomStub_test::test_invoke);

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE_REGISTRATION (RandomStub_test);

#endif // frameworks



void RandomStub_test::test_describe()
{
  typedef int Invocation;

  MOCKPP_NS::RandomStub<int> ds (1234, 5678);
  MOCKPP_NS::String b;
  MOCKPP_NS::String s = ds.describeTo(b);
  MOCKPP_ASSERT_TRUE(s.find(MOCKPP_PCHAR("random {1234 .. 5678}")) != MOCKPP_NS::String::npos);
  MOCKPP_ASSERT_TRUE(b.find(MOCKPP_PCHAR("random {1234 .. 5678}")) != MOCKPP_NS::String::npos);
}


void RandomStub_test::test_invoke()
{
  typedef int Invocation;

  MOCKPP_NS::RandomStub<float, float> ds (1234, 5678);
  for (unsigned i = 0; i < 5000; ++i)
  {
    float val = ds.typelessInvoke();
    //std::cout << val << std::endl;
    MOCKPP_ASSERT_TRUE(val >= 1234 && val <= 5678);
  }
}



