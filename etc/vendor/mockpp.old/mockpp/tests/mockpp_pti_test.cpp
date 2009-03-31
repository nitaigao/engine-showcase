/***************************************************************************
        mockpp_pti_test.cpp  -  unit tests for pti weakness functions
                             -------------------
    begin                : Sun 04 03 2005
    copyright            : (C) 2002-2008 by Ewald Arnold
    email                : mockpp at ewald-arnold dot de

  $Id: mockpp_pti_test.cpp 1438 2008-03-08 20:22:43Z ewald-arnold $

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

#ifndef MOCKPP_PTI_WEAKNESS // Partial Template Instantiation Weakness
#define MOCKPP_PTI_WEAKNESS
#endif

#include <mockpp/mockpp.h> // always first

#include "SelectFramework.h"

#include <mockpp/chaining/mockpp_pti.h>
#include <mockpp/chaining/Invocation.h>

#include <mockpp/matcher/InvokeOnceMatcher.h>



#ifndef MOCKPP_NO_EXCEPTIONS

typedef MOCKPP_NS::Invocation1<int> Setter1Invocation;

#if 1
MOCKPP_PTI_IMPLEMENT(Setter1Invocation)
#else
MOCKPP_TYPELESSSTUBADAPTER_PTI_IMPL(Setter1Invocation)
MOCKPP_FIFOINVOCATIONDISPATCHER_PTI_IMPL(Setter1Invocation)
MOCKPP_LIFOINVOCATIONDISPATCHER_PTI_IMPL(Setter1Invocation)
MOCKPP_INVOCATIONMOCKER_PTI_IMPL(Setter1Invocation)
MOCKPP_ABSTRACTDYNAMICCHAININGMOCK_PTI_IMPL(Setter1Invocation)
MOCKPP_COREMOCK_PTI_IMPL(Setter1Invocation)
#endif

MOCKPP_ANON_NS_START

class mockpp_pti_test MOCKPP_DERIVE_PUBLIC_UNITFRAMEWORK
{
 public:

#if defined (MOCKPP_USE_CXXTEST)

#elif defined(MOCKPP_USE_BOOSTTEST)

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE( mockpp_pti_test );

  CPPUNIT_TEST(test_coremock);
  CPPUNIT_TEST(test_typeless);
  CPPUNIT_TEST(test_lifo);
  CPPUNIT_TEST(test_fifo);
  CPPUNIT_TEST(test_invocation);
  CPPUNIT_TEST(test_abstract);

CPPUNIT_TEST_SUITE_END();

#endif

 public:

 MOCKPP_TESTMETHOD_DECL void test_coremock();
 MOCKPP_TESTMETHOD_DECL void test_typeless();
 MOCKPP_TESTMETHOD_DECL void test_lifo();
 MOCKPP_TESTMETHOD_DECL void test_fifo();
 MOCKPP_TESTMETHOD_DECL void test_invocation();
 MOCKPP_TESTMETHOD_DECL void test_abstract();
};


#if defined (MOCKPP_USE_CXXTEST)

MOCKPP_CXXTEST_SUITE_REGISTRATION( mockpp_pti_test );
MOCKPP_CXXTEST(mockpp_pti_test, test_coremock);
MOCKPP_CXXTEST(mockpp_pti_test, test_typeless);
MOCKPP_CXXTEST(mockpp_pti_test, test_lifo);
MOCKPP_CXXTEST(mockpp_pti_test, test_fifo);
MOCKPP_CXXTEST(mockpp_pti_test, test_invocation);
MOCKPP_CXXTEST(mockpp_pti_test, test_abstract);

#elif defined(MOCKPP_USE_BOOSTTEST)

MOCKPP_BOOST_TEST(mockpp_pti_test::test_coremock);
MOCKPP_BOOST_TEST(mockpp_pti_test::test_typeless);
MOCKPP_BOOST_TEST(mockpp_pti_test::test_lifo);
MOCKPP_BOOST_TEST(mockpp_pti_test::test_fifo);
MOCKPP_BOOST_TEST(mockpp_pti_test::test_invocation);
MOCKPP_BOOST_TEST(mockpp_pti_test::test_abstract);

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE_REGISTRATION (mockpp_pti_test);

#endif // frameworks



class MyVoidStub : public MOCKPP_NS::TypelessStub<void>
{
  public:

    bool  invoked;

    MyVoidStub()
    {
      invoked = false;
    }

    virtual void typelessInvoke( )
    {
      invoked = true;
    }

    virtual MOCKPP_NS::String describeTo( MOCKPP_NS::String &buffer ) const
    {
      return buffer;
    }
};


void mockpp_pti_test::test_coremock()
{
  int i1 = 1;
  Setter1Invocation si(MOCKPP_PCHAR("Setter1Invocation"), i1);
  {
    MyVoidStub *mvs = new MyVoidStub;
    MOCKPP_NS::CoreMock<void, Setter1Invocation> cmd(MOCKPP_PCHAR("name"), 0);
    cmd.setDefaultStub( mvs );
    cmd.invoke(si);
    MOCKPP_ASSERT_TRUE(true == mvs->invoked);
  }

  {
    MOCKPP_NS::CoreMock<void, Setter1Invocation> cmd(new MOCKPP_NS::LIFOInvocationDispatcher<void, Setter1Invocation>,
                                                  MOCKPP_PCHAR("name"), 0);
    MyVoidStub *mvs = new MyVoidStub;
    cmd.setDefaultStub(mvs);
    cmd.invoke(si);
    MOCKPP_ASSERT_TRUE(true == mvs->invoked);
  }
}


void mockpp_pti_test::test_typeless()
{
  int i1 = 1;
  Setter1Invocation si(MOCKPP_PCHAR("Setter1Invocation"), i1);
  MyVoidStub *mvs = new MyVoidStub;
  MOCKPP_NS::TypelessStubAdapter<void, Setter1Invocation > tsa (mvs);
  tsa.invoke(si);
  MOCKPP_ASSERT_TRUE(true == mvs->invoked);
}


class TestInvokable : public MOCKPP_NS::Invokable<void, Setter1Invocation>
{
  public:

    bool  invoked;

    TestInvokable()
    {
      invoked = false;
    }

    virtual bool matches( const Setter1Invocation &/*invocation*/ )
    {
      return !invoked;
    }

    virtual void invoke( const Setter1Invocation &/*invocation*/ )
    {
      invoked = true;
    }

    virtual bool hasDescription()
    {
      return true;
    }

    virtual MOCKPP_NS::String describeTo( MOCKPP_NS::String &buffer ) const
    {
      return buffer;
    }

    virtual void reset()
    {}

    virtual void verify()
    {}
};


class Lifo : public MOCKPP_NS::LIFOInvocationDispatcher<void, Setter1Invocation>
{
  public:

    void myDispatch( const Setter1Invocation &invocation )
    {
      dispatch(invocation);
    }
};


void mockpp_pti_test::test_lifo()
{
  int i1 = 1;
  Setter1Invocation si(MOCKPP_PCHAR("Setter1Invocation"), i1);
  MyVoidStub *mvs = new MyVoidStub;
  TestInvokable *ti1 = new TestInvokable;
  TestInvokable *ti2 = new TestInvokable;
  Lifo lifo;
  lifo.setDefaultStub( mvs );
  lifo.add(ti1);
  lifo.add(ti2);
  lifo.myDispatch(si);
  MOCKPP_ASSERT_TRUE(true == ti2->invoked);
  MOCKPP_ASSERT_TRUE(false == ti1->invoked);
  lifo.myDispatch(si);
  MOCKPP_ASSERT_TRUE(true == ti1->invoked);
  MOCKPP_ASSERT_TRUE(false == mvs->invoked);
  lifo.myDispatch(si);
  MOCKPP_ASSERT_TRUE(true == mvs->invoked);
}


class Fifo : public MOCKPP_NS::FIFOInvocationDispatcher<void, Setter1Invocation>
{
  public:

    void myDispatch( const Setter1Invocation &invocation )
    {
      dispatch(invocation);
    }
};


void mockpp_pti_test::test_fifo()
{
  int i1 = 1;
  Setter1Invocation si(MOCKPP_PCHAR("Setter1Invocation"), i1);
  MyVoidStub *mvs = new MyVoidStub;
  TestInvokable *ti1 = new TestInvokable;
  TestInvokable *ti2 = new TestInvokable;
  Fifo fifo;
  fifo.setDefaultStub( mvs );
  fifo.add(ti1);
  fifo.add(ti2);
  fifo.myDispatch(si);
  MOCKPP_ASSERT_TRUE(true == ti1->invoked);
  MOCKPP_ASSERT_TRUE(false == ti2->invoked);
  fifo.myDispatch(si);
  MOCKPP_ASSERT_TRUE(true == ti2->invoked);
  MOCKPP_ASSERT_TRUE(false == mvs->invoked);
  fifo.myDispatch(si);
  MOCKPP_ASSERT_TRUE(true == mvs->invoked);
}


class MyInvocationMatcher : public MOCKPP_NS::InvocationMatcher<Setter1Invocation>
{
  public:

    mutable bool invoked;

    MyInvocationMatcher()
    {
      invoked = false;
    }

    virtual bool matches( const Setter1Invocation & /*invocation*/ )
    {
      return !invoked;
    }

    virtual MOCKPP_NS::String describeTo( MOCKPP_NS::String &buffer ) const
    {
      return buffer;
    }

    virtual void verify()
    {
    }

    virtual void incInvoked( const Setter1Invocation &/*invocation*/ )
    {
      invoked = true;
    }

    virtual bool hasDescription()
    {
      return true;
    }
};


void mockpp_pti_test::test_invocation()
{
  int i1 = 1;
  Setter1Invocation si(MOCKPP_PCHAR("Setter1Invocation"), i1);
  MOCKPP_NS::InvocationMocker<void, Setter1Invocation> im;
  MyInvocationMatcher *ivm1 = new MyInvocationMatcher;
  MyInvocationMatcher *ivm2 = new MyInvocationMatcher;
  MyVoidStub *stub = new MyVoidStub;

  im.addMatcher( ivm1 );
  im.addMatcher( ivm2 );
  im.setStub(stub);

  im.invoke(si);
  MOCKPP_ASSERT_TRUE(ivm1->invoked == true);
  MOCKPP_ASSERT_TRUE(ivm2->invoked == true);
  MOCKPP_ASSERT_TRUE(stub->invoked == true);
}


class ChainingMockDummy : public MOCKPP_NS::AbstractDynamicChainingMock<void, Setter1Invocation>
{
  public:

    ChainingMockDummy( MOCKPP_NS::InvocationDispatcher<void, Setter1Invocation>::AP in_invocationDispatcher,
                       const MOCKPP_NS::String &name,
                       MOCKPP_NS::VerifiableList *parent )
      : MOCKPP_NS::AbstractDynamicChainingMock<void, Setter1Invocation>(in_invocationDispatcher, name, parent)
    {}

    void invoke(const Setter1Invocation &invocation)
    {
       mockInvocation(invocation);
    }
};

class MyMatcherDummy : public MOCKPP_NS::Invokable<void, Setter1Invocation>
{
  public:

    bool enable_except;
    bool invoked;

    MyMatcherDummy()
    {
      invoked = false;
      enable_except = false;
    }

    virtual bool matches( const Setter1Invocation &/*invocation*/ )
    {
      return !invoked;
    }

    virtual void reset()
    {
    }

    virtual void invoke( const Setter1Invocation &/*invocation*/ )
    {
      invoked = true;

      if (enable_except)
         MOCKPP_NS::assertionFailed(__LINE__, __FILE__, MOCKPP_PCHAR("invoke-fail"));
    }

    virtual void verify()
    {
    }

    virtual MOCKPP_NS::String describeTo (MOCKPP_NS::String &buffer) const
    {
      return buffer;
    }

    virtual bool hasDescription()
    {
      return true;
    }
};

void mockpp_pti_test::test_abstract()
{
  int i1 = 1;
  Setter1Invocation si(MOCKPP_PCHAR("Setter1Invocation"), i1);
  ChainingMockDummy cmd (new MOCKPP_NS::LIFOInvocationDispatcher<void, Setter1Invocation>, MOCKPP_PCHAR( "adc-name" ), 0 );
  MyVoidStub *stub = new MyVoidStub;
  MyMatcherDummy *mmd = new MyMatcherDummy;

  cmd.setDefaultStub(stub);
  cmd.addInvokable(mmd);

  cmd.invoke(si);
  MOCKPP_ASSERT_TRUE(mmd->invoked == true);
  MOCKPP_ASSERT_TRUE(stub->invoked == false);
  cmd.invoke(si);
  MOCKPP_ASSERT_TRUE(stub->invoked == true);
}

MOCKPP_NS_END

#endif // MOCKPP_NO_EXCEPTIONS



