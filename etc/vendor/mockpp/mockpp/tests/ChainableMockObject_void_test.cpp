 /***************************************************************************
                  ChainableMockObject_void_test.cpp
                                 -
  unit tests for ChainableMockObject class and 0 parameter void methods

                             -------------------

    begin                : Thu Sep 28 2004
    copyright            : (C) 2002-2008 by Ewald Arnold
    email                : mockpp at ewald-arnold dot de

  $Id: ChainableMockObject_void_test.cpp 1438 2008-03-08 20:22:43Z ewald-arnold $

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

#include <mockpp/matcher/TypelessMatcher.h>


#include "MyBuilderNamespace.h"
#include "MyMatchBuilder.h"


class ChainableMockObject_void_test MOCKPP_DERIVE_PUBLIC_UNITFRAMEWORK
{
 public:

#if defined (MOCKPP_USE_CXXTEST)

#elif defined(MOCKPP_USE_BOOSTTEST)

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE( ChainableMockObject_void_test );

  CPPUNIT_TEST(test_register);
  CPPUNIT_TEST(test_expect_throw);

CPPUNIT_TEST_SUITE_END();

#endif

 public:

  MOCKPP_TESTMETHOD_DECL void test_register();
  MOCKPP_TESTMETHOD_DECL void test_expect_throw();
};


#if defined (MOCKPP_USE_CXXTEST)

MOCKPP_CXXTEST_SUITE_REGISTRATION( ChainableMockObject_void_test );
MOCKPP_CXXTEST(ChainableMockObject_void_test, test_register);
MOCKPP_CXXTEST(ChainableMockObject_void_test, test_expect_throw);

#elif defined(MOCKPP_USE_BOOSTTEST)

MOCKPP_BOOST_TEST(ChainableMockObject_void_test::test_register);
MOCKPP_BOOST_TEST(ChainableMockObject_void_test::test_expect_throw);

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE_REGISTRATION (ChainableMockObject_void_test);

#endif // frameworks



typedef MOCKPP_NS::Invocation0 MyInvocation;

class MyChainableMockObject_void : public MOCKPP_NS::ChainableMockObject
{
  public:

    MyChainableMockObject_void(const MOCKPP_NS::String &name)
      : MOCKPP_NS::ChainableMockObject(name, 0)
      , MOCKPP_CONSTRUCT_MEMBERS_FOR_VOID_CHAINABLE0(chainable)
    {}

    MOCKPP_VOID_CHAINABLE0(MyChainableMockObject_void, chainable);
};


class MyChainableMockObject_void_ex : public MOCKPP_NS::ChainableMockObject
{
  public:

    MyChainableMockObject_void_ex(const MOCKPP_NS::String &name)
      : MOCKPP_NS::ChainableMockObject(name, 0)
      , MOCKPP_CONSTRUCT_MEMBERS_FOR_VOID_CHAINABLE_EXT0(chainable0, Ext)
    {}

    MOCKPP_VOID_CHAINABLE_EXT0(MyChainableMockObject_void_ex, chainable0,
                                                              Ext);
};


void ChainableMockObject_void_test::test_expect_throw()
{
  MyChainableMockObject_void mcm(MOCKPP_PCHAR("chainable-object"));

  MOCKPP_CHAINER_FOR(MyChainableMockObject_void, chainable) chain(&mcm);

  MOCKPP_TRY
  {
    mcm.chainable();
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


void ChainableMockObject_void_test::test_register()
{
  MOCKPP_NS::ChainableMockObject cm(MOCKPP_PCHAR("name"));
  MyMatchBuilder<void, MyInvocation> mbd;
  cm.registerUniqueID(MOCKPP_PCHAR("first-name"), new MOCKPP_NS::MatchBuilderAdapter<void, MyInvocation>(&mbd));
  cm.registerUniqueID(MOCKPP_PCHAR("second-name"), new MOCKPP_NS::MatchBuilderAdapter<void, MyInvocation>(&mbd));
  MOCKPP_ASSERT_TRUE(&mbd == cm.lookupID(MOCKPP_PCHAR("first-name"))->getBuilder());
  MOCKPP_ASSERT_TRUE(&mbd == cm.lookupID(MOCKPP_PCHAR("second-name"))->getBuilder());

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
}



