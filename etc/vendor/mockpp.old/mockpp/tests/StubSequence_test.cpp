/***************************************************************************
           StubSequence_test.cpp  -  unit tests for StubSequence class
                             -------------------
    begin                : Sat Sep 18 2004
    copyright            : (C) 2002-2008 by Ewald Arnold
    email                : mockpp at ewald-arnold dot de

  $Id: StubSequence_test.cpp 1438 2008-03-08 20:22:43Z ewald-arnold $

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

#include MOCKPP_VECTOR_H

#include <mockpp/stub/StubSequence.h>
#include <mockpp/stub/ReturnStub.h>
#include <mockpp/stub/ThrowStub.h>

#include <mockpp/chaining/ChainingMockObjectSupport.h>


class StubSequence_test MOCKPP_DERIVE_PUBLIC_UNITFRAMEWORK
{
 public:

#if defined (MOCKPP_USE_CXXTEST)

#elif defined(MOCKPP_USE_BOOSTTEST)

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE( StubSequence_test );

  CPPUNIT_TEST(test_invoke);
  CPPUNIT_TEST(test_describe);

CPPUNIT_TEST_SUITE_END();

#endif

 public:

  MOCKPP_TESTMETHOD_DECL void test_describe();
  MOCKPP_TESTMETHOD_DECL void test_invoke();
};


#if defined (MOCKPP_USE_CXXTEST)

MOCKPP_CXXTEST_SUITE_REGISTRATION( StubSequence_test );
MOCKPP_CXXTEST(StubSequence_test, test_describe);
MOCKPP_CXXTEST(StubSequence_test, test_invoke);

#elif defined(MOCKPP_USE_BOOSTTEST)

MOCKPP_BOOST_TEST(StubSequence_test::test_describe);
MOCKPP_BOOST_TEST(StubSequence_test::test_invoke);

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE_REGISTRATION (StubSequence_test);

#endif // frameworks



void StubSequence_test::test_describe()
{
  MOCKPP_STL::vector<MOCKPP_NS::Stub<int, int>* > sv;
  MOCKPP_NS::ReturnStub<int> rs (12);
  sv.push_back(new MOCKPP_NS::TypelessStubAdapter<int, int>(MOCKPP_NS::returnValue<int>(12)));
  sv.push_back(new MOCKPP_NS::TypelessStubAdapter<int, int>(MOCKPP_NS::returnValue<int>(34)));

  MOCKPP_NS::StubSequence<int, int> ss (sv.begin(), sv.end());
  MOCKPP_NS::String b = MOCKPP_GET_STRING("abcd");
  MOCKPP_NS::String s = ss.describeTo(b);
  MOCKPP_ASSERT_TRUE(s.find(MOCKPP_PCHAR("abcd")) != MOCKPP_NS::String::npos);
  MOCKPP_ASSERT_TRUE(s.find(MOCKPP_PCHAR("12")) != MOCKPP_NS::String::npos);
  MOCKPP_ASSERT_TRUE(s.find(MOCKPP_PCHAR("34")) != MOCKPP_NS::String::npos);
  MOCKPP_ASSERT_TRUE(b.find(MOCKPP_PCHAR("abcd")) != MOCKPP_NS::String::npos);
  MOCKPP_ASSERT_TRUE(b.find(MOCKPP_PCHAR("12")) != MOCKPP_NS::String::npos);
  MOCKPP_ASSERT_TRUE(b.find(MOCKPP_PCHAR("34")) != MOCKPP_NS::String::npos);
}


void StubSequence_test::test_invoke()
{
  MOCKPP_STL::vector<MOCKPP_NS::Stub<int, int>* > sv;
  sv.push_back(new MOCKPP_NS::TypelessStubAdapter<int, int>(MOCKPP_NS::returnValue<int>(12)));
  sv.push_back(new MOCKPP_NS::TypelessStubAdapter<int, int>(MOCKPP_NS::returnValue<int>(34)));
  sv.push_back(new MOCKPP_NS::TypelessStubAdapter<int, int>(MOCKPP_NS::returnValue<int>(56)));

  MOCKPP_NS::StubSequence<int, int> ss(sv.begin(), sv.end());

  int val = ss.invoke((int)1);
  MOCKPP_ASSERT_TRUE(val == 12);

  val = ss.invoke((int)1);
  MOCKPP_ASSERT_TRUE(val == 34);

  val = ss.invoke((int) 1);
  MOCKPP_ASSERT_TRUE(val == 56);

  MOCKPP_TRY
  {
    ss.invoke((int)1);
    MOCKPP_FAIL(MOCKPP_PCHAR("Did not throw the exception"));
  }

#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_NS::AssertionFailedError &x)
  {
    MOCKPP_ASSERT_TRUE(x.getMessage().find(MOCKPP_PCHAR("no more stubs"))
                       != MOCKPP_NS::String::npos);
  }

  catch(...)
  {
    MOCKPP_FAIL(MOCKPP_PCHAR("Caught unexpected exception"));
  }
#endif
}



