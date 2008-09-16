/***************************************************************************
            CoreMock_test.cpp  -  unit tests for CoreMock class
                            -------------------
   begin                : Wed Sep 4 2004
   copyright            : (C) 2002-2008 by Ewald Arnold
   email                : mockpp at ewald-arnold dot de

   $Id: CoreMock_test.cpp 1438 2008-03-08 20:22:43Z ewald-arnold $

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

#include <mockpp/chaining/CoreMock.h>
#include <mockpp/chaining/InvocationMocker.h>
#include <mockpp/chaining/LIFOInvocationDispatcher.h>
#include <mockpp/chaining/FIFOInvocationDispatcher.h>

#include <mockpp/chaining/ChainingMockObjectSupport.h>
#include <mockpp/chaining/mockpp_pti.h>
#include <mockpp/stub/ReturnStub.h>



typedef MOCKPP_NS::Invocation0 GetterInvocation;
typedef MOCKPP_NS::Invocation0 VoidInvocation;


MOCKPP_INVOCATIONMOCKER_PTI_DECL(VoidInvocation)
MOCKPP_ABSTRACTDYNAMICCHAININGMOCK_PTI_IMPL(VoidInvocation)
MOCKPP_COREMOCK_PTI_IMPL(VoidInvocation)


MOCKPP_ANON_NS_START


class CoreMock_test MOCKPP_DERIVE_PUBLIC_UNITFRAMEWORK
{
  public:

#if defined (MOCKPP_USE_CXXTEST)

#elif defined(MOCKPP_USE_BOOSTTEST)

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE( CoreMock_test );

  CPPUNIT_TEST( test_void_invocation );
  CPPUNIT_TEST( test_invocation );

CPPUNIT_TEST_SUITE_END();

#endif

  public:

   MOCKPP_TESTMETHOD_DECL void test_void_invocation();
   MOCKPP_TESTMETHOD_DECL void test_invocation();
};


#if defined (MOCKPP_USE_CXXTEST)

MOCKPP_CXXTEST_SUITE_REGISTRATION( CoreMock_test );
MOCKPP_CXXTEST(CoreMock_test,  test_void_invocation );
MOCKPP_CXXTEST(CoreMock_test,  test_invocation );

#elif defined(MOCKPP_USE_BOOSTTEST)

MOCKPP_BOOST_TEST(CoreMock_test:: test_void_invocation );
MOCKPP_BOOST_TEST(CoreMock_test:: test_invocation );

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE_REGISTRATION ( CoreMock_test );

#endif // frameworks



template <typename I> // Invocation
class MatcherDummy : public MOCKPP_NS::Invokable<int, I>
{
  public:

    bool match_result;
    int invocation_result;
    bool is_invoke;

    virtual bool matches( const I &/*invocation*/ )
    {
      return match_result;
    }

    virtual int invoke( const I &/*invocation*/ )
    {
      is_invoke = true;

      return invocation_result;
    }

    virtual void reset()
    {}

    virtual void verify()
    {}

    virtual MOCKPP_NS::String describeTo ( MOCKPP_NS::String &buffer ) const
    {
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

    virtual bool matches( const I &/*invocation*/ )
    {
      return match_result;
    }

    virtual void invoke( const I &/*invocation*/ )
    {
      is_invoke = true;

    }

    virtual void reset()
    {}

    virtual void verify()
    {}

    virtual MOCKPP_NS::String describeTo ( MOCKPP_NS::String &buffer ) const
    {
      return buffer;
    }

    virtual bool hasDescription()
    {
      return true;
    }
};


void CoreMock_test::test_invocation()
{
  {
    MOCKPP_NS::CoreMock<int, GetterInvocation>
    cm ( new MOCKPP_NS::LIFOInvocationDispatcher<int, GetterInvocation>, MOCKPP_PCHAR( "cm-name" ), 0 );
  }

  MOCKPP_NS::CoreMock<int, GetterInvocation>
  cmd ( MOCKPP_PCHAR( "cm-name1" ), 0 );

  GetterInvocation inv( MOCKPP_PCHAR("InvocationDummy::getProp" ) );

  MatcherDummy<GetterInvocation> *md1 = new MatcherDummy<GetterInvocation>;
  md1->invocation_result = 10;
  cmd.addInvokable( md1 );
  cmd.setDefaultStub( MOCKPP_NS::returnValue<int>( 33 ) );

  md1->match_result = false;
  MOCKPP_ASSERT_TRUE( cmd.invoke( inv ) == 33 );
  MOCKPP_ASSERT_TRUE( cmd.invoke( inv ) == 33 );

  md1->match_result = true;
  md1->is_invoke = false;
  md1->invocation_result = 4;
  MOCKPP_ASSERT_TRUE( cmd.invoke( inv ) == 4 );
  MOCKPP_ASSERT_TRUE( md1->is_invoke == true );
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


void CoreMock_test::test_void_invocation()
{
  MOCKPP_NS::CoreMock<void, VoidInvocation>
  cm ( MOCKPP_PCHAR( "cm-name1" ), 0 );

  MOCKPP_NS::CoreMock<void, VoidInvocation>
  cmd ( new MOCKPP_NS::LIFOInvocationDispatcher<void, VoidInvocation>, MOCKPP_PCHAR( "cm-name" ), 0 );

  VoidInvocation inv( MOCKPP_PCHAR("InvocationDummy::setProp" ) );

  VoidMatcherDummy<VoidInvocation> *md1 = new VoidMatcherDummy<VoidInvocation>;
  cmd.addInvokable( md1 );
  StoreStub<VoidInvocation> *store = new StoreStub<VoidInvocation>;

  store->invoked = false;
  md1->match_result = false;
  cmd.setDefaultStub( store );
  cmd.invoke( inv );
  MOCKPP_ASSERT_TRUE( store->invoked == true );

  store->invoked = false;
  md1->match_result = true;
  md1->is_invoke = false;
  cmd.invoke( inv );
  MOCKPP_ASSERT_TRUE( md1->is_invoke == true );
  MOCKPP_ASSERT_TRUE( store->invoked == false );
}


MOCKPP_NS_END



