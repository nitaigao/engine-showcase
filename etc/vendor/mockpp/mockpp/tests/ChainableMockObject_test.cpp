/***************************************************************************
                     ChainableMockObject_test.cpp
                                 -
  unit tests for ChainableMockObject class and 0 parameter methods
                             -------------------
    begin                : Thu Sep 28 2004
    copyright            : (C) 2002-2008 by Ewald Arnold
    email                : mockpp at ewald-arnold dot de

  $Id: ChainableMockObject_test.cpp 1438 2008-03-08 20:22:43Z ewald-arnold $

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

#include <mockpp/chaining/ChainableMockObject.h>

#include <mockpp/chaining/ChainingMockObjectSupport.h>
#include <mockpp/chaining/Invocation.h>


#include "MyBuilderNamespace.h"
#include "MyMatchBuilder.h"


class ChainableMockObject_test MOCKPP_DERIVE_PUBLIC_UNITFRAMEWORK
{
 public:

#if defined (MOCKPP_USE_CXXTEST)

#elif defined(MOCKPP_USE_BOOSTTEST)

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE( ChainableMockObject_test );

  CPPUNIT_TEST(test_register);
  CPPUNIT_TEST(test_after_1);
  CPPUNIT_TEST(test_after_2);
  CPPUNIT_TEST(test_bad_after);
  CPPUNIT_TEST(test_before_1);
  CPPUNIT_TEST(test_before_2);
  CPPUNIT_TEST(test_bad_before);
  CPPUNIT_TEST(test_pending);
  CPPUNIT_TEST(test_expect_return);
  CPPUNIT_TEST(test_expect_throw);
  CPPUNIT_TEST(test_stub_return);
  CPPUNIT_TEST(test_stub_return_const_ref);

CPPUNIT_TEST_SUITE_END();

#endif

 public:

  MOCKPP_TESTMETHOD_DECL void test_after_1();
  MOCKPP_TESTMETHOD_DECL void test_after_2();
  MOCKPP_TESTMETHOD_DECL void test_bad_after();
  MOCKPP_TESTMETHOD_DECL void test_before_1();
  MOCKPP_TESTMETHOD_DECL void test_before_2();
  MOCKPP_TESTMETHOD_DECL void test_bad_before();
  MOCKPP_TESTMETHOD_DECL void test_register();
  MOCKPP_TESTMETHOD_DECL void test_pending();
  MOCKPP_TESTMETHOD_DECL void test_expect_return();
  MOCKPP_TESTMETHOD_DECL void test_expect_throw();
  MOCKPP_TESTMETHOD_DECL void test_stub_return();
  MOCKPP_TESTMETHOD_DECL void test_stub_return_const_ref();
};


#if defined (MOCKPP_USE_CXXTEST)

MOCKPP_CXXTEST_SUITE_REGISTRATION( ChainableMockObject_test );
MOCKPP_CXXTEST(ChainableMockObject_test, test_register);
MOCKPP_CXXTEST(ChainableMockObject_test, test_after_1);
MOCKPP_CXXTEST(ChainableMockObject_test, test_after_2);
MOCKPP_CXXTEST(ChainableMockObject_test, test_bad_after);
MOCKPP_CXXTEST(ChainableMockObject_test, test_before_1);
MOCKPP_CXXTEST(ChainableMockObject_test, test_before_2);
MOCKPP_CXXTEST(ChainableMockObject_test, test_bad_before);
MOCKPP_CXXTEST(ChainableMockObject_test, test_pending);
MOCKPP_CXXTEST(ChainableMockObject_test, test_expect_return);
MOCKPP_CXXTEST(ChainableMockObject_test, test_expect_throw);
MOCKPP_CXXTEST(ChainableMockObject_test, test_stub_return);
MOCKPP_CXXTEST(ChainableMockObject_test, test_stub_return_const_ref);

#elif defined(MOCKPP_USE_BOOSTTEST)

MOCKPP_BOOST_TEST(ChainableMockObject_test::test_register);
MOCKPP_BOOST_TEST(ChainableMockObject_test::test_after_1);
MOCKPP_BOOST_TEST(ChainableMockObject_test::test_after_2);
MOCKPP_BOOST_TEST(ChainableMockObject_test::test_bad_after);
MOCKPP_BOOST_TEST(ChainableMockObject_test::test_before_1);
MOCKPP_BOOST_TEST(ChainableMockObject_test::test_before_2);
MOCKPP_BOOST_TEST(ChainableMockObject_test::test_bad_before);
MOCKPP_BOOST_TEST(ChainableMockObject_test::test_pending);
MOCKPP_BOOST_TEST(ChainableMockObject_test::test_expect_return);
MOCKPP_BOOST_TEST(ChainableMockObject_test::test_expect_throw);
MOCKPP_BOOST_TEST(ChainableMockObject_test::test_stub_return);
MOCKPP_BOOST_TEST(ChainableMockObject_test::test_stub_return_const_ref);

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE_REGISTRATION (ChainableMockObject_test);

#endif // frameworks



class MyChainableMockObject : public MOCKPP_NS::ChainableMockObject
{
  public:

    MyChainableMockObject(const MOCKPP_NS::String &name)
      : MOCKPP_NS::ChainableMockObject(name, 0)
      , MOCKPP_CONSTRUCT_MEMBERS_FOR_CHAINABLE0(chainable_a)
      , MOCKPP_CONSTRUCT_MEMBERS_FOR_CHAINABLE0(chainable_b)
      , MOCKPP_CONSTRUCT_MEMBERS_FOR_CHAINABLE0(chainable_c)
      , MOCKPP_CONSTRUCT_MEMBERS_FOR_CHAINABLE0(chainable_d)
      , MOCKPP_CONSTRUCT_MEMBERS_FOR_CHAINABLE0(chainable_e_Shadow)
    {}

    MOCKPP_CHAINABLE0(MyChainableMockObject, int, chainable_a);
    MOCKPP_CHAINABLE0(MyChainableMockObject, int, chainable_b);
    MOCKPP_CHAINABLE0(MyChainableMockObject, int, chainable_c);
    MOCKPP_CHAINABLE0(MyChainableMockObject, int, chainable_d);
    MOCKPP_CHAINABLE0(MyChainableMockObject, int, chainable_e_Shadow);

    int chainable_e_shadow;

    virtual const int& chainable_e()
    {
      chainable_e_shadow = chainable_e_Shadow();
      return chainable_e_shadow;
    }
};


typedef MOCKPP_NS::Invocation0 MyInvocation;


void ChainableMockObject_test::test_pending()
{
  MyChainableMockObject mcm(MOCKPP_PCHAR("chainable-object"));

  mcm.addPendingRecorder(MOCKPP_PCHAR("id-1"), 0);
  mcm.addPendingRecorder(MOCKPP_PCHAR("id-2"), 0);
  MOCKPP_TRY
  {
    mcm.checkPendingRecorders();
    MOCKPP_ASSERT_TRUE_MESSAGE(MOCKPP_PCHAR("should have thrown"), false);
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_NS::AssertionFailedError &ex)
  {
    MOCKPP_ASSERT_TRUE(ex.getMessage().find(MOCKPP_PCHAR("id-1")) != MOCKPP_NS::String::npos);
    MOCKPP_ASSERT_TRUE(ex.getMessage().find(MOCKPP_PCHAR("id-2")) != MOCKPP_NS::String::npos);
  }
#endif
}


void ChainableMockObject_test::test_expect_return()
{
  MyChainableMockObject mcm(MOCKPP_PCHAR("chainable-object"));

  MOCKPP_CHAINER_FOR(MyChainableMockObject, chainable_a) chain(&mcm);

  chain.setDefaultStub(MOCKPP_NS::returnValue<int>(321));

  chain.expects(MOCKPP_NS::exactly(2))
       .will(MOCKPP_NS::returnValue<int>(456))
       .id(MOCKPP_PCHAR("chainable-2"));

  MOCKPP_ASSERT_TRUE(mcm.chainable_a() == 456);
  MOCKPP_ASSERT_TRUE(mcm.chainable_a() == 456);
  MOCKPP_ASSERT_TRUE(mcm.chainable_a() == 321);
  MOCKPP_ASSERT_TRUE(mcm.chainable_a() == 321);
  MOCKPP_ASSERT_TRUE(mcm.chainable_a() == 321);
  MOCKPP_ASSERT_TRUE(mcm.chainable_a() == 321);

  mcm.verify();
}


void ChainableMockObject_test::test_expect_throw()
{
  MyChainableMockObject mcm(MOCKPP_PCHAR("chainable-object"));

  MOCKPP_CHAINER_FOR(MyChainableMockObject, chainable_a) chain(&mcm);

  MOCKPP_TRY
  {
    mcm.chainable_a();
    MOCKPP_ASSERT_TRUE_MESSAGE(MOCKPP_PCHAR("should have thrown"), false);
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_NS::AssertionFailedError &ex)
  {
    MOCKPP_ASSERT_TRUE(ex.getMessage().find(MOCKPP_PCHAR("unexpected")) != MOCKPP_NS::String::npos);
  }
#endif

  mcm.verify();
}


void ChainableMockObject_test::test_stub_return()
{
  MyChainableMockObject mcm(MOCKPP_PCHAR("chainable-object"));

  MOCKPP_CHAINER_FOR(MyChainableMockObject, chainable_a) chain(&mcm);

  chain.stubs(MOCKPP_NS::exactly(2))
       .will(MOCKPP_NS::returnValue<int>(123));

  chain.stubs(MOCKPP_NS::once())
       .will(MOCKPP_NS::returnValue<int>(456))
       .id(MOCKPP_PCHAR("chainable-2"));

  chain.stubs()
       .will(MOCKPP_NS::returnValue<int>(789))
       .id(MOCKPP_PCHAR("chainable-3"));

  MOCKPP_ASSERT_TRUE(mcm.chainable_a() == 123);
  MOCKPP_ASSERT_TRUE(mcm.chainable_a() == 123);

  MOCKPP_ASSERT_TRUE(mcm.chainable_a() == 456);

  MOCKPP_ASSERT_TRUE(mcm.chainable_a() == 789);
  MOCKPP_ASSERT_TRUE(mcm.chainable_a() == 789);
  MOCKPP_ASSERT_TRUE(mcm.chainable_a() == 789);

  mcm.verify();
}


void ChainableMockObject_test::test_register()
{
  MOCKPP_NS::ChainableMockObject cm(MOCKPP_PCHAR("name-xx"));
  MOCKPP_ASSERT_TRUE(cm.getIdent() == MOCKPP_PCHAR("name-xx"));
  MyMatchBuilder<void, MyInvocation> mbd1;
  MyMatchBuilder<void, MyInvocation> mbd2;
  cm.registerUniqueID(MOCKPP_PCHAR("first-name"), new MOCKPP_NS::MatchBuilderAdapter<void, MyInvocation>(&mbd1));
  cm.registerUniqueID(MOCKPP_PCHAR("second-name"), new MOCKPP_NS::MatchBuilderAdapter<void, MyInvocation>(&mbd2));
  MOCKPP_ASSERT_TRUE(&mbd1 == cm.lookupID(MOCKPP_PCHAR("first-name"))->getBuilder());
  MOCKPP_ASSERT_TRUE(&mbd2 == cm.lookupID(MOCKPP_PCHAR("second-name"))->getBuilder());

  MOCKPP_TRY
  {
     cm.registerUniqueID(MOCKPP_PCHAR("second-name"), 0);
     MOCKPP_ASSERT_TRUE_MESSAGE(MOCKPP_PCHAR("should have thrown"), false);
  }

#ifndef MOCKPP_NO_EXCEPTIONS
  catch ( const MOCKPP_NS::AssertionFailedError &ae )
  {
    MOCKPP_ASSERT_TRUE(ae.getMessage().find(MOCKPP_PCHAR( "duplicate invocation named" )) != MOCKPP_NS::String::npos);
  }
#endif

  cm.reset();
  MOCKPP_ASSERT_TRUE(0 == cm.lookupID(MOCKPP_PCHAR("second-name")));

  cm.verify();
}


void ChainableMockObject_test::test_after_1()
{
  MyChainableMockObject mcm(MOCKPP_PCHAR("chainable-object"));

  MOCKPP_CHAINER_FOR(MyChainableMockObject, chainable_a) chain_a(&mcm);
  MOCKPP_CHAINER_FOR(MyChainableMockObject, chainable_b) chain_b(&mcm);
  MOCKPP_CHAINER_FOR(MyChainableMockObject, chainable_c) chain_c(&mcm);
  MOCKPP_CHAINER_FOR(MyChainableMockObject, chainable_d) chain_d(&mcm);

  chain_a.expects(MOCKPP_NS::once())
         .will(MOCKPP_NS::returnValue<int>(123))
         .id(MOCKPP_PCHAR("chainable-a"));

  chain_b.expects(MOCKPP_NS::once())
         .after(MOCKPP_PCHAR("chainable-a"))
         .will(MOCKPP_NS::returnValue<int>(456))
         .id(MOCKPP_PCHAR("chainable-b"));

  chain_c.expects(MOCKPP_NS::once())
         .after(MOCKPP_PCHAR("chainable-a"))
         .will(MOCKPP_NS::returnValue<int>(789))
         .id(MOCKPP_PCHAR("chainable-c"));

  chain_d.expects(MOCKPP_NS::once())
         .after(MOCKPP_PCHAR("chainable-a"))
         .will(MOCKPP_NS::returnValue<int>(111))
         .id(MOCKPP_PCHAR("chainable-d"));

  MOCKPP_ASSERT_TRUE(mcm.chainable_a() == 123);
  MOCKPP_ASSERT_TRUE(mcm.chainable_b() == 456);
  MOCKPP_ASSERT_TRUE(mcm.chainable_c() == 789);
  MOCKPP_ASSERT_TRUE(mcm.chainable_d() == 111);

  mcm.verify();
}


void ChainableMockObject_test::test_after_2()
{
  MyChainableMockObject mcm(MOCKPP_PCHAR("chainable-object"));

  MOCKPP_CHAINER_FOR(MyChainableMockObject, chainable_a) chain_a(&mcm);
  MOCKPP_CHAINER_FOR(MyChainableMockObject, chainable_b) chain_b(&mcm);
  MOCKPP_CHAINER_FOR(MyChainableMockObject, chainable_c) chain_c(&mcm);
  MOCKPP_CHAINER_FOR(MyChainableMockObject, chainable_d) chain_d(&mcm);

  chain_a.expects(MOCKPP_NS::once())
         .after(MOCKPP_PCHAR("chainable-d"))
         .will(MOCKPP_NS::returnValue<int>(123))
         .id(MOCKPP_PCHAR("chainable-a"));

  chain_b.expects(MOCKPP_NS::once())
         .after(MOCKPP_PCHAR("chainable-d"))
         .will(MOCKPP_NS::returnValue<int>(456))
         .id(MOCKPP_PCHAR("chainable-b"));

  chain_c.expects(MOCKPP_NS::once())
         .after(MOCKPP_PCHAR("chainable-d"))
         .will(MOCKPP_NS::returnValue<int>(789))
         .id(MOCKPP_PCHAR("chainable-c"));

  chain_d.expects(MOCKPP_NS::once())
         .will(MOCKPP_NS::returnValue<int>(111))
         .id(MOCKPP_PCHAR("chainable-d"));

  MOCKPP_ASSERT_TRUE(mcm.chainable_d() == 111);
  MOCKPP_ASSERT_TRUE(mcm.chainable_c() == 789);
  MOCKPP_ASSERT_TRUE(mcm.chainable_b() == 456);
  MOCKPP_ASSERT_TRUE(mcm.chainable_a() == 123);

  mcm.verify();
}


void ChainableMockObject_test::test_bad_after()
{
  MyChainableMockObject mcm(MOCKPP_PCHAR("chainable-object"));

  MOCKPP_CHAINER_FOR(MyChainableMockObject, chainable_a) chain_a(&mcm);
  MOCKPP_CHAINER_FOR(MyChainableMockObject, chainable_b) chain_b(&mcm);

  chain_a.expects(MOCKPP_NS::once())
         .after(MOCKPP_PCHAR("chainable-b"))
         .will(MOCKPP_NS::returnValue<int>(123))
         .id(MOCKPP_PCHAR("chainable-a"));

  chain_b.expects(MOCKPP_NS::once())
         .will(MOCKPP_NS::returnValue<int>(456))
         .id(MOCKPP_PCHAR("chainable-b"));

  MOCKPP_TRY
  {
    mcm.chainable_a();
    MOCKPP_ASSERT_TRUE_MESSAGE(MOCKPP_PCHAR("should have thrown"), false);
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_NS::AssertionFailedError &ex)
  {
    MOCKPP_NS::String s = ex.getMessage();
    MOCKPP_ASSERT_TRUE(s.find(MOCKPP_PCHAR("after")) != MOCKPP_NS::String::npos);
    MOCKPP_ASSERT_TRUE(s.find(MOCKPP_PCHAR("(not invoked)")) != MOCKPP_NS::String::npos);
    MOCKPP_ASSERT_TRUE(s.find(MOCKPP_PCHAR("chainable-b")) != MOCKPP_NS::String::npos);
  }
#endif
}


void ChainableMockObject_test::test_before_1()
{
  MyChainableMockObject mcm(MOCKPP_PCHAR("chainable-object"));

  MOCKPP_CHAINER_FOR(MyChainableMockObject, chainable_a) chain_a(&mcm);
  MOCKPP_CHAINER_FOR(MyChainableMockObject, chainable_b) chain_b(&mcm);
  MOCKPP_CHAINER_FOR(MyChainableMockObject, chainable_c) chain_c(&mcm);
  MOCKPP_CHAINER_FOR(MyChainableMockObject, chainable_d) chain_d(&mcm);

  chain_d.expects(MOCKPP_NS::once())
         .before(MOCKPP_PCHAR("chainable-a"))
         .will(MOCKPP_NS::returnValue<int>(111))
         .id(MOCKPP_PCHAR("chainable-d"));

  chain_c.expects(MOCKPP_NS::once())
         .before(MOCKPP_PCHAR("chainable-a"))
         .will(MOCKPP_NS::returnValue<int>(789))
         .id(MOCKPP_PCHAR("chainable-c"));

  chain_b.expects(MOCKPP_NS::once())
         .before(MOCKPP_PCHAR("chainable-a"))
         .will(MOCKPP_NS::returnValue<int>(123))
         .id(MOCKPP_PCHAR("chainable-b"));

  chain_a.expects(MOCKPP_NS::once())
         .will(MOCKPP_NS::returnValue<int>(456))
         .id(MOCKPP_PCHAR("chainable-a"));

  MOCKPP_ASSERT_TRUE(mcm.chainable_d() == 111);
  MOCKPP_ASSERT_TRUE(mcm.chainable_c() == 789);
  MOCKPP_ASSERT_TRUE(mcm.chainable_b() == 123);
  MOCKPP_ASSERT_TRUE(mcm.chainable_a() == 456);

  mcm.verify();
}


void ChainableMockObject_test::test_before_2()
{
  MyChainableMockObject mcm(MOCKPP_PCHAR("chainable-object"));

  MOCKPP_CHAINER_FOR(MyChainableMockObject, chainable_a) chain_a(&mcm);
  MOCKPP_CHAINER_FOR(MyChainableMockObject, chainable_b) chain_b(&mcm);
  MOCKPP_CHAINER_FOR(MyChainableMockObject, chainable_c) chain_c(&mcm);
  MOCKPP_CHAINER_FOR(MyChainableMockObject, chainable_d) chain_d(&mcm);

  chain_a.expects(MOCKPP_NS::once())
         .will(MOCKPP_NS::returnValue<int>(123))
         .id(MOCKPP_PCHAR("chainable-a"));

  chain_b.expects(MOCKPP_NS::once())
         .before(MOCKPP_PCHAR("chainable-a"))
         .will(MOCKPP_NS::returnValue<int>(456))
         .id(MOCKPP_PCHAR("chainable-b"));

  chain_c.expects(MOCKPP_NS::once())
         .before(MOCKPP_PCHAR("chainable-a"))
         .will(MOCKPP_NS::returnValue<int>(789))
         .id(MOCKPP_PCHAR("chainable-c"));

  chain_d.expects(MOCKPP_NS::once())
         .before(MOCKPP_PCHAR("chainable-a"))
         .will(MOCKPP_NS::returnValue<int>(111))
         .id(MOCKPP_PCHAR("chainable-d"));

  MOCKPP_ASSERT_TRUE(mcm.chainable_d() == 111);
  MOCKPP_ASSERT_TRUE(mcm.chainable_c() == 789);
  MOCKPP_ASSERT_TRUE(mcm.chainable_b() == 456);
  MOCKPP_ASSERT_TRUE(mcm.chainable_a() == 123);

  mcm.verify();
}


void ChainableMockObject_test::test_bad_before()
{
  MyChainableMockObject mcm(MOCKPP_PCHAR("chainable-object"));

  MOCKPP_CHAINER_FOR(MyChainableMockObject, chainable_a) chain_a(&mcm);
  MOCKPP_CHAINER_FOR(MyChainableMockObject, chainable_b) chain_b(&mcm);

  chain_a.expects(MOCKPP_NS::once())
          .before(MOCKPP_PCHAR("chainable-b"))
          .will(MOCKPP_NS::returnValue<int>(123))
          .id(MOCKPP_PCHAR("chainable-a"));

  chain_b.expects(MOCKPP_NS::once())
         .will(MOCKPP_NS::returnValue<int>(456))
         .id(MOCKPP_PCHAR("chainable-b"));

  MOCKPP_ASSERT_TRUE(mcm.chainable_b() == 456);
  MOCKPP_TRY
  {
    mcm.chainable_a();
    MOCKPP_ASSERT_TRUE_MESSAGE(MOCKPP_PCHAR("should have thrown"), false);
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_NS::AssertionFailedError &ex)
  {
    MOCKPP_NS::String s = ex.getMessage();
    MOCKPP_ASSERT_TRUE(s.find(MOCKPP_PCHAR("(invoked)")) != MOCKPP_NS::String::npos);
    MOCKPP_ASSERT_TRUE(s.find(MOCKPP_PCHAR("before")) != MOCKPP_NS::String::npos);
    MOCKPP_ASSERT_TRUE(s.find(MOCKPP_PCHAR("chainable-b")) != MOCKPP_NS::String::npos);
  }
#endif
}


void ChainableMockObject_test::test_stub_return_const_ref()
{
  MyChainableMockObject mcm(MOCKPP_PCHAR("chainable-object"));

  MOCKPP_CHAINER_FOR(MyChainableMockObject, chainable_e_Shadow) chain(&mcm);

  chain.setDefaultStub(MOCKPP_NS::returnValue<int>(321));

  MOCKPP_ASSERT_TRUE(mcm.chainable_e() == 321);

  mcm.verify();
}




