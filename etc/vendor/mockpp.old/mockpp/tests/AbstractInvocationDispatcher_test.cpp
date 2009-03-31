/***************************************************************************
AbstractInvocationDispatcher_test.cpp  -  unit tests for AbstractInvocationDispatcher class
                            -------------------
   begin                : Wed Sep 4 2004
   copyright            : (C) 2002-2008 by Ewald Arnold
   email                : mockpp at ewald-arnold dot de

   $Id: AbstractInvocationDispatcher_test.cpp 1438 2008-03-08 20:22:43Z ewald-arnold $

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

// #include <iostream>

#include <mockpp/chaining/AbstractInvocationDispatcher.h>

#include <mockpp/compat/Asserter.h>


#include <mockpp/Throwable.h>


MOCKPP_ANON_NS_START


class AbstractInvocationDispatcher_test MOCKPP_DERIVE_PUBLIC_UNITFRAMEWORK
{
  public:

    AbstractInvocationDispatcher_test()
    {}

#if defined (MOCKPP_USE_CXXTEST)

#elif defined(MOCKPP_USE_BOOSTTEST)

#elif defined(MOCKPP_USE_CPPUNIT)

   CPPUNIT_TEST_SUITE( AbstractInvocationDispatcher_test );
  CPPUNIT_TEST( test_describe );
  CPPUNIT_TEST( test_throw );
  CPPUNIT_TEST( test_verify );
CPPUNIT_TEST_SUITE_END();

#endif

  public:

   MOCKPP_TESTMETHOD_DECL void test_throw();
   MOCKPP_TESTMETHOD_DECL void test_verify();
   MOCKPP_TESTMETHOD_DECL void test_describe();
};


#if defined (MOCKPP_USE_CXXTEST)

MOCKPP_CXXTEST_SUITE_REGISTRATION(AbstractInvocationDispatcher_test);
MOCKPP_CXXTEST(AbstractInvocationDispatcher_test, test_describe );
MOCKPP_CXXTEST(AbstractInvocationDispatcher_test, test_throw );
MOCKPP_CXXTEST(AbstractInvocationDispatcher_test, test_verify );

#elif defined(MOCKPP_USE_BOOSTTEST)

MOCKPP_BOOST_TEST(AbstractInvocationDispatcher_test::test_describe );
MOCKPP_BOOST_TEST(AbstractInvocationDispatcher_test::test_throw );
MOCKPP_BOOST_TEST(AbstractInvocationDispatcher_test::test_verify );

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE_REGISTRATION ( AbstractInvocationDispatcher_test );

#endif // frameworks



typedef MOCKPP_NS::Invocation0 GetterInvocation;


template <typename R, typename I>
class TestInvokable : public MOCKPP_NS::Invokable<R, I>
{
  public:

    bool                   match_result;
    R                      invoke_return;
    bool                   description_result;
    MOCKPP_NS::String         describeTo_result;
    mutable bool           verified;

    virtual bool matches( const I &/*invocation*/ )
    {
      return match_result;
    }

    virtual R invoke( const I &/*invocation*/ )
    {
      return invoke_return;
    }

    virtual bool hasDescription()
    {
      return description_result;
    }

    virtual MOCKPP_NS::String describeTo( MOCKPP_NS::String &buffer ) const
    {
      buffer += describeTo_result;
      return buffer;
    }

    virtual void reset()
    {
    }

    virtual void verify()
    {
      verified = true;
    }
};


template <typename R, typename I>
class AID : public MOCKPP_NS::AbstractInvocationDispatcher<R, I>
{
  public:

    R dispatch_result;

    virtual R dispatch( const I &invocation )
    {
      return this->defaultStub->invoke(invocation);
    }
};


void AbstractInvocationDispatcher_test::test_throw()
{
  GetterInvocation ginv( MOCKPP_PCHAR("TestClass::getProp" ) );

  AID<int, GetterInvocation> aid;
  typedef TestInvokable<int, GetterInvocation> TInvok;

  TInvok *ti = new TInvok();
  ti->match_result = false;
  aid.add(ti);
  MOCKPP_TRY
  {
    aid.dispatch(ginv);
    MOCKPP_ASSERT_TRUE(false);
  }

#ifndef MOCKPP_NO_EXCEPTIONS
  catch ( MOCKPP_NS::AssertionFailedError &ex)
  {
    MOCKPP_ASSERT_TRUE(ex.getMessage().find (MOCKPP_PCHAR("unexpected")) != MOCKPP_NS::String::npos);
  }
#endif
}


void AbstractInvocationDispatcher_test::test_verify()
{
  GetterInvocation ginv( MOCKPP_PCHAR("TestClass::getProp" ) );

  AID<int, GetterInvocation> aid;
  typedef TestInvokable<int, GetterInvocation> TInvok;

  TInvok *ti1 = new TInvok();
  TInvok *ti2 = new TInvok();

  aid.add( ti1 );
  aid.add( ti2 );

  ti1->verified = false;
  ti2->verified = false;
  aid.verify();
  MOCKPP_ASSERT_TRUE(ti1->verified == true);
  MOCKPP_ASSERT_TRUE(ti2->verified == true);
}


void AbstractInvocationDispatcher_test::test_describe()
{
  GetterInvocation ginv( MOCKPP_PCHAR("TestClass::getProp" ) );

  AID<int, GetterInvocation> aid;
  typedef TestInvokable<int, GetterInvocation> TInvok;

  TInvok *ti1 = new TInvok();
  TInvok *ti2 = new TInvok();

  aid.add( ti1 );
  aid.add( ti2 );

  ti1->description_result = false;
  ti2->description_result = false;
  ti1->describeTo_result = MOCKPP_PCHAR("t1");
  ti2->describeTo_result = MOCKPP_PCHAR("t2");

  MOCKPP_NS::String s = MOCKPP_PCHAR("??") ;
  MOCKPP_ASSERT_TRUE( aid.describeTo(s) == MOCKPP_PCHAR( "??no expectations set" ));
  MOCKPP_ASSERT_TRUE( s == MOCKPP_PCHAR( "??no expectations set" ));

  ti1->description_result = true;
  s = MOCKPP_PCHAR("??") ;
  MOCKPP_ASSERT_TRUE( aid.describeTo(s) == MOCKPP_PCHAR( "??t1\n" ));
  MOCKPP_ASSERT_TRUE( s == MOCKPP_PCHAR( "??t1\n" ));

  ti2->description_result = true;
  s = MOCKPP_PCHAR("??") ;
  MOCKPP_ASSERT_TRUE( aid.describeTo(s) == MOCKPP_PCHAR( "??\n - t1\n - t2\n" ));
  MOCKPP_ASSERT_TRUE( s == MOCKPP_PCHAR( "??\n - t1\n - t2\n" ));

  aid.reset();
  s = MOCKPP_PCHAR("");
  MOCKPP_ASSERT_TRUE( aid.describeTo(s) == MOCKPP_PCHAR( "no expectations set" ));
}


MOCKPP_NS_END



