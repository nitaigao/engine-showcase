/***************************************************************************
AbstractDynamicChainingMock_test.cpp  -  unit tests for AbstractDynamicChainingMock class
                            -------------------
   begin                : Wed Sep 1 2004
   copyright            : (C) 2002-2008 by Ewald Arnold
   email                : mockpp at ewald-arnold dot de

   $Id: AbstractDynamicChainingMock_test.cpp 1438 2008-03-08 20:22:43Z ewald-arnold $

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

#include <mockpp/chaining/AbstractDynamicChainingMock.h>
#include <mockpp/chaining/InvocationMocker.h>
#include <mockpp/chaining/LIFOInvocationDispatcher.h>
#include <mockpp/chaining/FIFOInvocationDispatcher.h>

#include <mockpp/chaining/mockpp_pti.h>
#include <mockpp/stub/ReturnStub.h>
#include <mockpp/stub/TypelessStub.h>

#include <mockpp/chaining/ChainingMockObjectSupport.h>



typedef MOCKPP_NS::Invocation0 GetterInvocation;
typedef MOCKPP_NS::Invocation0 VoidInvocation;
//typedef MOCKPP_NS::ConstraintSet1<int> CSet1;


MOCKPP_INVOCATIONMOCKER_PTI_IMPL(VoidInvocation)


MOCKPP_ANON_NS_START


class AbstractDynamicChainingMock_test MOCKPP_DERIVE_PUBLIC_UNITFRAMEWORK
{
  public:

#if defined (MOCKPP_USE_CXXTEST)

#elif defined(MOCKPP_USE_BOOSTTEST)

#elif defined(MOCKPP_USE_CPPUNIT)

  CPPUNIT_TEST_SUITE( AbstractDynamicChainingMock_test );

  CPPUNIT_TEST( test_void_invocation );
  CPPUNIT_TEST( test_invocation );
  CPPUNIT_TEST( test_describer );
  CPPUNIT_TEST( test_describe );
  CPPUNIT_TEST( test_void_describe );
  CPPUNIT_TEST( test_verify );

CPPUNIT_TEST_SUITE_END();

#endif

  public:

   MOCKPP_TESTMETHOD_DECL void test_void_invocation();
   MOCKPP_TESTMETHOD_DECL void test_invocation();
   MOCKPP_TESTMETHOD_DECL void test_describer();
   MOCKPP_TESTMETHOD_DECL void test_describe();
   MOCKPP_TESTMETHOD_DECL void test_void_describe();
   MOCKPP_TESTMETHOD_DECL void test_verify();
};


#if defined (MOCKPP_USE_CXXTEST)

MOCKPP_CXXTEST_SUITE_REGISTRATION(AbstractDynamicChainingMock_test);
MOCKPP_CXXTEST(AbstractDynamicChainingMock_test, test_void_invocation );
MOCKPP_CXXTEST(AbstractDynamicChainingMock_test, test_invocation );
MOCKPP_CXXTEST(AbstractDynamicChainingMock_test, test_describer );
MOCKPP_CXXTEST(AbstractDynamicChainingMock_test, test_describe );
MOCKPP_CXXTEST(AbstractDynamicChainingMock_test, test_void_describe );
MOCKPP_CXXTEST(AbstractDynamicChainingMock_test, test_verify );

#elif defined(MOCKPP_USE_BOOSTTEST)

MOCKPP_BOOST_TEST(AbstractDynamicChainingMock_test::test_void_invocation );
MOCKPP_BOOST_TEST(AbstractDynamicChainingMock_test::test_invocation );
MOCKPP_BOOST_TEST(AbstractDynamicChainingMock_test::test_describer );
MOCKPP_BOOST_TEST(AbstractDynamicChainingMock_test::test_describe );
MOCKPP_BOOST_TEST(AbstractDynamicChainingMock_test::test_void_describe );
MOCKPP_BOOST_TEST(AbstractDynamicChainingMock_test::test_verify );

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE_REGISTRATION ( AbstractDynamicChainingMock_test );

#endif // frameworks



template <typename R,    // Returntype
          typename I>    // Invocation
class ChainingMockDummy : public MOCKPP_NS::AbstractDynamicChainingMock<R, I>
{
  public:

    ChainingMockDummy( typename MOCKPP_NS::InvocationDispatcher<R, I>::AP in_invocationDispatcher,
                       const MOCKPP_NS::String &name,
                       MOCKPP_NS::VerifiableList *parent )
      : MOCKPP_NS::AbstractDynamicChainingMock<R,I>(in_invocationDispatcher, name, parent)
    {}

    R invoke(const I &invocation)
    {
       return mockInvocation(invocation);
    }

    void void_invoke(const I &invocation)
    {
       mockInvocation(invocation);
    }
};


template <typename I> // Invocation
class MatcherDummy : public MOCKPP_NS::Invokable<int, I>
{
  public:

    bool match_result;
    int invocation_result;
    MOCKPP_NS::String describe_value;
    bool enable_except;
    bool is_invoke;
//
    virtual bool matches( const I &/*invocation*/ )
    {
      return match_result;
    }

    virtual void reset()
    {
    }

    virtual int invoke( const I &/*invocation*/ )
    {
      is_invoke = true;

      if (enable_except)
         MOCKPP_NS::assertionFailed(__LINE__, __FILE__, MOCKPP_PCHAR("invoke-fail"));

      return invocation_result;
    }

    virtual void verify()
    {
      if (enable_except)
         MOCKPP_NS::assertionFailed(__LINE__, __FILE__, MOCKPP_PCHAR("dummy-verify"));
    }

    virtual MOCKPP_NS::String describeTo (MOCKPP_NS::String &buffer) const
    {
      buffer += describe_value;
      return buffer;
    }

    virtual bool hasDescription()
    {
      return true;
    }
};


template <typename I> // Invocation
class VoidMatcherDummy : public MOCKPP_NS::Invokable<void, I>
{
  public:

    bool match_result;
    bool is_invoke;
    MOCKPP_NS::String describe_value;
    bool enable_except;

    virtual bool matches( const I &/*invocation*/ )
    {
      return match_result;
    }

    virtual void reset()
    {
    }

    virtual void invoke( const I &/*invocation*/ )
    {
      is_invoke = true;

      if (enable_except)
         MOCKPP_NS::assertionFailed(__LINE__, __FILE__, MOCKPP_PCHAR("invoke-fail"));

    }

    virtual void verify()
    {
      if (enable_except)
         MOCKPP_NS::assertionFailed(__LINE__, __FILE__, MOCKPP_PCHAR("dummy-verify"));
    }

    virtual MOCKPP_NS::String describeTo (MOCKPP_NS::String &buffer) const
    {
      buffer += describe_value;
      return buffer;
    }

    virtual bool hasDescription()
    {
      return true;
    }
};



void AbstractDynamicChainingMock_test::test_describer()
{
#if !defined(__BORLANDC__) && ((__GNUC__ >= 3) /* || defined(_MSC_VER) */)  // @todo make compiling
  MOCKPP_NS::AbstractDynamicChainingMock<int, GetterInvocation>::Describer desc;
  MOCKPP_ASSERT_TRUE(desc.hasDescription() == false);

  MOCKPP_NS::String s;
  MOCKPP_NS::InvocationMocker<int, GetterInvocation>::List matchers;

  MOCKPP_ASSERT_TRUE(desc.describeTo(s,
                                 matchers,
                                 MOCKPP_NS::returnValue<int>(1).get(),
                                 MOCKPP_NS::String(MOCKPP_PCHAR("name"))  ) == MOCKPP_PCHAR("") );
#endif
}


void AbstractDynamicChainingMock_test::test_describe()
{
  MOCKPP_NS::AbstractDynamicChainingMock<int, GetterInvocation>
  adc_default ( MOCKPP_PCHAR( "adc-name" ), 0 );

  MOCKPP_NS::AbstractDynamicChainingMock<int, GetterInvocation>
  adc ( new MOCKPP_NS::LIFOInvocationDispatcher<int, GetterInvocation>, MOCKPP_PCHAR( "adc-name" ), 0 );

  MOCKPP_ASSERT_TRUE( adc.toString() == MOCKPP_PCHAR( "adc-name" ) );
  MOCKPP_ASSERT_TRUE( adc.getMockName() == MOCKPP_PCHAR( "adc-name" ) );
}


void AbstractDynamicChainingMock_test::test_void_describe()
{
  MOCKPP_NS::AbstractDynamicChainingMock<void, VoidInvocation>
  adc_default ( MOCKPP_PCHAR( "adc-name" ), 0 );

  MOCKPP_NS::AbstractDynamicChainingMock<void, VoidInvocation>
  adc ( new MOCKPP_NS::LIFOInvocationDispatcher<void, VoidInvocation>, MOCKPP_PCHAR( "adc-name" ), 0 );

  MOCKPP_ASSERT_TRUE( adc.toString() == MOCKPP_PCHAR( "adc-name" ) );
  MOCKPP_ASSERT_TRUE( adc.getMockName() == MOCKPP_PCHAR( "adc-name" ) );
}


void AbstractDynamicChainingMock_test::test_verify()
{
  ChainingMockDummy<int, GetterInvocation>
    cmd (new MOCKPP_NS::LIFOInvocationDispatcher<int, GetterInvocation>, MOCKPP_PCHAR( "adc-name" ), 0 );

  MatcherDummy<GetterInvocation> *md1 = new MatcherDummy<GetterInvocation>;
  cmd.addInvokable(md1);

  md1->enable_except = false;
  cmd.verify();

  MOCKPP_TRY
  {
    md1->enable_except = true;
    cmd.verify();
    MOCKPP_ASSERT_TRUE_MESSAGE(MOCKPP_PCHAR("did not throw the exception"), false);
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch (MOCKPP_NS::AssertionFailedError &ex )
  {
    MOCKPP_ASSERT_TRUE(ex.getMessage().find(MOCKPP_PCHAR("dummy-verify")) != MOCKPP_NS::String::npos );
    MOCKPP_ASSERT_TRUE(ex.getMessage().find(MOCKPP_PCHAR("adc-name")) != MOCKPP_NS::String::npos );
  }
#endif
  md1->enable_except = false;
  cmd.verify();
}


void AbstractDynamicChainingMock_test::test_invocation()
{
  ChainingMockDummy<int, GetterInvocation>
     cmd ( new MOCKPP_NS::LIFOInvocationDispatcher<int, GetterInvocation>, MOCKPP_PCHAR( "adc-name" ), 0 );

  GetterInvocation inv(MOCKPP_PCHAR("GetterInvocation"));

  MatcherDummy<GetterInvocation> *md1 = new MatcherDummy<GetterInvocation>;
  md1->invocation_result = 10;
  md1->enable_except = false;
  cmd.addInvokable(md1);
  cmd.setDefaultStub(MOCKPP_NS::returnValue<int>(33));

  md1->match_result = false;
  MOCKPP_ASSERT_TRUE(cmd.invoke(inv) == 33);
  MOCKPP_ASSERT_TRUE(cmd.invoke(inv) == 33);

  md1->match_result = true;
  md1->is_invoke = false;
  md1->invocation_result = 4;
  MOCKPP_ASSERT_TRUE(cmd.invoke(inv) == 4);
  MOCKPP_ASSERT_TRUE(md1->is_invoke == true);

  md1->is_invoke = false;
  md1->invocation_result = 5;
  MOCKPP_ASSERT_TRUE(cmd.invoke(inv) == 5);

  MOCKPP_TRY
  {
    md1->enable_except = true;
    cmd.invoke(inv);
    MOCKPP_ASSERT_TRUE_MESSAGE(MOCKPP_PCHAR("did not throw the exception"), false);
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch (MOCKPP_NS::AssertionFailedError &ex )
  {
    MOCKPP_ASSERT_TRUE(ex.getMessage().find(MOCKPP_PCHAR("invoke-fail")) != MOCKPP_NS::String::npos );
    MOCKPP_ASSERT_TRUE(ex.getMessage().find(MOCKPP_PCHAR("adc-name")) != MOCKPP_NS::String::npos );
  }
#endif

  md1->enable_except = false;
  MOCKPP_TRY
  {
    cmd.invoke(inv);
    MOCKPP_ASSERT_TRUE_MESSAGE(MOCKPP_PCHAR("did not throw the exception"), false);
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch (MOCKPP_NS::AssertionFailedError &ex )
  {
    MOCKPP_ASSERT_TRUE(ex.getMessage().find(MOCKPP_PCHAR("invoke-fail")) != MOCKPP_NS::String::npos );
    MOCKPP_ASSERT_TRUE(ex.getMessage().find(MOCKPP_PCHAR("adc-name")) != MOCKPP_NS::String::npos );
  }
#endif

  cmd.reset();
  cmd.invoke(inv);
}


template <typename I> // Invocation
class StoreStub : public MOCKPP_NS::Stub<void, I>
{
  public:

    bool invoked;

    void invoke( const I &/*invocation*/ )
    {
      invoked = true;
    }

    virtual MOCKPP_NS::String describeTo( MOCKPP_NS::String &buffer ) const
    {
      return buffer;
    }
};


void AbstractDynamicChainingMock_test::test_void_invocation()
{
  ChainingMockDummy<void, VoidInvocation>
     cmd ( new MOCKPP_NS::LIFOInvocationDispatcher<void, VoidInvocation>, MOCKPP_PCHAR( "adc-name" ), 0 );

  VoidInvocation inv(MOCKPP_PCHAR("VoidInvocation"));

  VoidMatcherDummy<VoidInvocation> *md1 = new VoidMatcherDummy<VoidInvocation>;
  md1->enable_except = false;
  cmd.addInvokable(md1);
  StoreStub<VoidInvocation> *store = new StoreStub<VoidInvocation>;

  store->invoked = false;
  md1->match_result = false;
  cmd.setDefaultStub(store);
  cmd.void_invoke(inv);
  MOCKPP_ASSERT_TRUE(store->invoked == true);

  store->invoked = false;
  md1->match_result = true;
  md1->is_invoke = false;
  cmd.void_invoke(inv);
  MOCKPP_ASSERT_TRUE(md1->is_invoke == true);
  MOCKPP_ASSERT_TRUE(store->invoked == false);

  MOCKPP_TRY
  {
    md1->enable_except = true;
    cmd.void_invoke(inv);
    MOCKPP_ASSERT_TRUE_MESSAGE(MOCKPP_PCHAR("did not throw the exception"), false);
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch (MOCKPP_NS::AssertionFailedError &ex )
  {
    MOCKPP_NS::String s = ex.getMessage();
    MOCKPP_ASSERT_TRUE(s.find(MOCKPP_PCHAR("invoke-fail")) != MOCKPP_NS::String::npos );
    MOCKPP_ASSERT_TRUE(s.find(MOCKPP_PCHAR("adc-name")) != MOCKPP_NS::String::npos );
  }
#endif

  md1->enable_except = false;
  MOCKPP_TRY
  {
    cmd.void_invoke(inv);
    MOCKPP_ASSERT_TRUE_MESSAGE(MOCKPP_PCHAR("did not throw the exception"), false);
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch (MOCKPP_NS::AssertionFailedError &ex )
  {
    MOCKPP_ASSERT_TRUE(ex.getMessage().find(MOCKPP_PCHAR("invoke-fail")) != MOCKPP_NS::String::npos );
    MOCKPP_ASSERT_TRUE(ex.getMessage().find(MOCKPP_PCHAR("adc-name")) != MOCKPP_NS::String::npos );
  }
#endif

  cmd.reset();
  cmd.void_invoke(inv);
}


MOCKPP_NS_END



