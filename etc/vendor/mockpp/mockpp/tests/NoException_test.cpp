/***************************************************************************
   NoException_test.cpp  -  unit tests for tests with disabled exceptions
                             -------------------
    begin                : Wed Dec 7 2005
    copyright            : (C) 2002-2008 by Ewald Arnold
    email                : mockpp at ewald-arnold dot de

  $Id: NoException_test.cpp 1438 2008-03-08 20:22:43Z ewald-arnold $

 ***************************************************************************/

/**************************************************************************
 *
 * This program is free software; you can redistribute it Or/or modify
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

// redefine default stuff always

#undef MOCKPP_THROW
#define MOCKPP_THROW(x)  storeMalfunctionData(x)

#undef MOCKPP_RETHROW
#define MOCKPP_RETHROW   storeMalfunctionData("unexpected rethrow")

#define MOCKPP_ENABLE_DEFAULT_FORMATTER

#include <mockpp/ExpectationValue.h>
#include <mockpp/Throwable.h>

#include <mockpp/util/AssertMo.h>

#include <mockpp/compat/Asserter.h>
#include <mockpp/compat/Exception.h>

#include "SelectFramework.h"


MOCKPP_ANON_NS_START


class NoException_test MOCKPP_DERIVE_PUBLIC_UNITFRAMEWORK
{
 public:

  virtual ~NoException_test()
  {}

#if defined (MOCKPP_USE_CXXTEST)

#elif defined(MOCKPP_USE_BOOSTTEST)

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE( NoException_test );

  CPPUNIT_TEST(test_invoke);

CPPUNIT_TEST_SUITE_END();

#endif

 public:

   MOCKPP_TESTMETHOD_DECL void test_invoke();

   virtual void setUp();
   virtual void tearDown();
};


#if defined (MOCKPP_USE_CXXTEST)

MOCKPP_CXXTEST_SUITE_REGISTRATION( NoException_test );
MOCKPP_CXXTEST(NoException_test, test_invoke);

#elif defined(MOCKPP_USE_BOOSTTEST)

MOCKPP_BOOST_TEST(NoException_test::test_invoke);

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE_REGISTRATION (NoException_test);

#endif // frameworks


MOCKPP_NS::String bad_data1;
MOCKPP_NS::String bad_data2;
MOCKPP_NS::String bad_data3;


struct MyStruct
{
  MyStruct(int v)
   : val(v)
  {}

  MyStruct()
   : val(0)
  {}

  bool operator==(const MyStruct &o) const
  {
    return val == o.val;
  }

  int val;
};


struct OtherStruct {};


template <class T>
void storeMalfunctionData(const T & /* dummy */)
{
  bad_data1 = MOCKPP_PCHAR("unknown exception occured");
}


template <>
void storeMalfunctionData(const MyStruct &/*e*/)
{
  bad_data2 = MOCKPP_PCHAR("MyStruct");
}


template <>
void storeMalfunctionData(const MOCKPP_NS::AssertionFailedError &e)
{
  bad_data3 = e.getMessage();
}


void NoException_test::setUp()
{
  MOCKPP_NS::setAssertionFailedForwarder(storeMalfunctionData<MOCKPP_NS::AssertionFailedError>);
}


void NoException_test::tearDown()
{
  MOCKPP_NS::setAssertionFailedForwarder(0);
}


void NoException_test::test_invoke()
{
  MOCKPP_NS::setAssertionFailedForwarder(storeMalfunctionData<MOCKPP_NS::AssertionFailedError>);

  MOCKPP_NS::ThrowableItem it;
  it.take(MyStruct());
  it.get()->throw_me();

  it.take(OtherStruct());
  it.get()->throw_me();

  MOCKPP_NS::ExpectationValue<MyStruct> ec (MOCKPP_PCHAR("verifyValue"), 0);
  ec.setExpected(MyStruct(3));
  ec.setActual(MyStruct(4));

  MOCKPP_NS::setAssertionFailedForwarder(0);

  MOCKPP_ASSERT_EQUALS(bad_data1, MOCKPP_NS::String(MOCKPP_PCHAR("unknown exception occured")));
  MOCKPP_ASSERT_EQUALS(bad_data2, MOCKPP_NS::String(MOCKPP_PCHAR("MyStruct")));
  MOCKPP_ASSERT_TRUE(bad_data3.find(MOCKPP_NS::String(MOCKPP_PCHAR("did not receive the expected value"))) != MOCKPP_NS::String::npos);
}


MOCKPP_NS_END

