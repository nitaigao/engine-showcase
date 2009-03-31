/***************************************************************************
           ReturnAndTriggerStub_test.cpp  -  unit tests for ReturnAndTriggerStub class
                             -------------------
    begin                : Thu Aug 01 2006
    copyright            : (C) 2002-2008 by Ewald Arnold
    email                : mockpp at ewald-arnold dot de

  $Id: ReturnAndTriggerStub_test.cpp 1438 2008-03-08 20:22:43Z ewald-arnold $

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

#include <mockpp/stub/ReturnAndTriggerStub.h>

#include <mockpp/compat/Asserter.h>


class ReturnAndTriggerStub_test MOCKPP_DERIVE_PUBLIC_UNITFRAMEWORK
{
 public:

#if defined (MOCKPP_USE_CXXTEST)

#elif defined(MOCKPP_USE_BOOSTTEST)

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE( ReturnAndTriggerStub_test );

  CPPUNIT_TEST(test_invoke);
  CPPUNIT_TEST(test_describe);

CPPUNIT_TEST_SUITE_END();

#endif

 public:

  MOCKPP_TESTMETHOD_DECL void test_describe();
  MOCKPP_TESTMETHOD_DECL void test_invoke();
};


#if defined (MOCKPP_USE_CXXTEST)

MOCKPP_CXXTEST_SUITE_REGISTRATION( ReturnAndTriggerStub_test );
MOCKPP_CXXTEST(ReturnAndTriggerStub_test, test_describe);
MOCKPP_CXXTEST(ReturnAndTriggerStub_test, test_invoke);

#elif defined(MOCKPP_USE_BOOSTTEST)

MOCKPP_BOOST_TEST(ReturnAndTriggerStub_test::test_describe);
MOCKPP_BOOST_TEST(ReturnAndTriggerStub_test::test_invoke);

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE_REGISTRATION (ReturnAndTriggerStub_test);

#endif // frameworks


template <class T>
class MyTriggeredConstraint : public  MOCKPP_NS::TriggeredConstraint<T>
{
  public:

    bool triggered;
    MOCKPP_NS::String id;

    MyTriggeredConstraint(const MOCKPP_NS::String &in_id)
      : id(in_id)
    {}

    virtual void trigger()
    {
      triggered = true;
    }

    virtual MOCKPP_NS::String describeTo( MOCKPP_NS::String &buffer ) const
    {
      buffer += id;
      return buffer;
    }

    virtual bool eval( const T &/*arg*/ ) const
    {
      return (T)true;
    }
};


void ReturnAndTriggerStub_test::test_describe()
{
  typedef int Invocation;

  MyTriggeredConstraint<unsigned> tc1(MOCKPP_PCHAR("MyUnsigned"));
  MyTriggeredConstraint<float> tc2(MOCKPP_PCHAR("MyFloat"));
  MOCKPP_NS::ReturnAndTriggerStub2<int, unsigned, float> ds (1234, &tc1, &tc2);
  MOCKPP_NS::String b = MOCKPP_GET_STRING("abcd");
  MOCKPP_NS::String s = ds.describeTo(b);

//  MOCKPP_STD_NS::wcout << s << MOCKPP_STD_NS::endl;

  MOCKPP_ASSERT_TRUE(s.find(MOCKPP_PCHAR("abcd")) != MOCKPP_NS::String::npos);
  MOCKPP_ASSERT_TRUE(s.find(MOCKPP_PCHAR("1234")) != MOCKPP_NS::String::npos);
  MOCKPP_ASSERT_TRUE(s.find(MOCKPP_PCHAR("triggers [MyFloat]")) != MOCKPP_NS::String::npos);
  MOCKPP_ASSERT_TRUE(s.find(MOCKPP_PCHAR("triggers [MyUnsigned]")) != MOCKPP_NS::String::npos);

  MOCKPP_ASSERT_TRUE(b.find(MOCKPP_PCHAR("abcd")) != MOCKPP_NS::String::npos);
  MOCKPP_ASSERT_TRUE(b.find(MOCKPP_PCHAR("1234")) != MOCKPP_NS::String::npos);
  MOCKPP_ASSERT_TRUE(b.find(MOCKPP_PCHAR("triggers [MyFloat]")) != MOCKPP_NS::String::npos);
  MOCKPP_ASSERT_TRUE(b.find(MOCKPP_PCHAR("triggers [MyUnsigned]")) != MOCKPP_NS::String::npos);
}


void ReturnAndTriggerStub_test::test_invoke()
{
  typedef int Invocation;

  MyTriggeredConstraint<unsigned> tc1(MOCKPP_PCHAR("MyUnsigned"));
  MyTriggeredConstraint<float> tc2(MOCKPP_PCHAR("MyFloat"));
  MOCKPP_NS::ReturnAndTriggerStub2<int, unsigned, float> ds (1234, &tc1, &tc2);
  tc1.triggered = false;
  tc2.triggered = false;
  int val = ds.typelessInvoke();
  MOCKPP_ASSERT_TRUE(val == 1234);
  MOCKPP_ASSERT_TRUE(true == tc1.triggered);
  MOCKPP_ASSERT_TRUE(true == tc2.triggered);
}

