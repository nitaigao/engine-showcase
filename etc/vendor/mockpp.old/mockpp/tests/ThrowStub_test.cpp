/***************************************************************************
            ThrowStub_test.cpp  -  unit tests for  class
                            -------------------
   begin                : Sun Aug 22 2004
   copyright            : (C) 2002-2008 by Ewald Arnold
   email                : mockpp at ewald-arnold dot de

 $Id: ThrowStub_test.cpp 1438 2008-03-08 20:22:43Z ewald-arnold $

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

#include <mockpp/stub/CustomStub.h>
#include <mockpp/Throwable.h>
#include <mockpp/compat/Asserter.h>
#include <mockpp/stub/ThrowStub.h>




class ThrowStub_test MOCKPP_DERIVE_PUBLIC_UNITFRAMEWORK
{
  public:

#if defined (MOCKPP_USE_CXXTEST)

#elif defined(MOCKPP_USE_BOOSTTEST)

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE( ThrowStub_test );

  CPPUNIT_TEST( test_describe );
  CPPUNIT_TEST( test_invoke1 );
  CPPUNIT_TEST( test_invoke2 );
  CPPUNIT_TEST( test_invoke_void );

CPPUNIT_TEST_SUITE_END();

#endif

  public:

   MOCKPP_TESTMETHOD_DECL void test_describe();
   MOCKPP_TESTMETHOD_DECL void test_invoke1();
   MOCKPP_TESTMETHOD_DECL void test_invoke2();
   MOCKPP_TESTMETHOD_DECL void test_invoke_void();
};


#if defined (MOCKPP_USE_CXXTEST)

MOCKPP_CXXTEST_SUITE_REGISTRATION( ThrowStub_test );
MOCKPP_CXXTEST(ThrowStub_test, test_describe);
MOCKPP_CXXTEST(ThrowStub_test, test_invoke1);
MOCKPP_CXXTEST(ThrowStub_test, test_invoke2);
MOCKPP_CXXTEST(ThrowStub_test, test_invoke_void);

#elif defined(MOCKPP_USE_BOOSTTEST)

MOCKPP_BOOST_TEST(ThrowStub_test::test_describe);
MOCKPP_BOOST_TEST(ThrowStub_test::test_invoke1);
MOCKPP_BOOST_TEST(ThrowStub_test::test_invoke2);
MOCKPP_BOOST_TEST(ThrowStub_test::test_invoke_void);

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE_REGISTRATION ( ThrowStub_test );

#endif // frameworks



void ThrowStub_test::test_describe()
{
  MOCKPP_NS::ThrowableItem it;
  it.take( MOCKPP_STL::string( "my-item" ) );
  MOCKPP_NS::ThrowStub<int> ts( it );

  MOCKPP_NS::String b = MOCKPP_GET_STRING("abcd ");
  MOCKPP_NS::String s = ts.describeTo(b);

  MOCKPP_ASSERT_TRUE(s.find(MOCKPP_PCHAR("abcd throws <")) != MOCKPP_NS::String::npos);
  MOCKPP_ASSERT_TRUE(b.find(MOCKPP_PCHAR("abcd throws <")) != MOCKPP_NS::String::npos);
}


void ThrowStub_test::test_invoke1()
{
  MOCKPP_NS::ThrowableItem it ( MOCKPP_NS::make_throwable( MOCKPP_STL::string( "my-item" ) ) );
  MOCKPP_NS::ThrowStub<int> ts( it );
  MOCKPP_TRY
  {
    ts.typelessInvoke();
    MOCKPP_FAIL( MOCKPP_PCHAR( "Did not throw the exception" ) );
  }

#ifndef MOCKPP_NO_EXCEPTIONS
  catch ( MOCKPP_STL::string & s )
  {
    MOCKPP_ASSERT_TRUE( s == "my-item" );
  }

  catch ( ... )
  {
    MOCKPP_FAIL( MOCKPP_PCHAR( "Caught unexpected exception" ) );
  }
#endif
}


void ThrowStub_test::test_invoke2()
{
  MOCKPP_NS::ThrowableItem it;
  it.take( MOCKPP_STL::string( "my-item" ) );
  MOCKPP_NS::ThrowStub<int> ts( it );
  try
  {
    ts.typelessInvoke();
    MOCKPP_FAIL( MOCKPP_PCHAR( "Did not throw the exception" ) );
  }

#ifndef MOCKPP_NO_EXCEPTIONS
  catch ( MOCKPP_STL::string & s )
  {
    MOCKPP_ASSERT_TRUE( s == "my-item" );
  }

  catch ( ... )
  {
    MOCKPP_FAIL( MOCKPP_PCHAR( "Caught unexpected exception" ) );
  }
#endif
}


void ThrowStub_test::test_invoke_void()
{
#ifndef _MSC_VER // why necessary?
  MOCKPP_NS::ThrowableItem it;
  it.take( MOCKPP_STL::string( "my-item" ) );
  MOCKPP_NS::ThrowStub<void> ts( it );
  MOCKPP_TRY
  {
    ts.typelessInvoke();
    MOCKPP_FAIL( MOCKPP_PCHAR( "Did not throw the exception" ) );
  }

#ifndef MOCKPP_NO_EXCEPTIONS
  catch ( MOCKPP_STL::string & s )
  {
    MOCKPP_ASSERT_TRUE( s == "my-item" );
  }

  catch ( ... )
  {
    MOCKPP_FAIL( MOCKPP_PCHAR( "Caught unexpected exception" ) );
  }
#endif
#endif
}



