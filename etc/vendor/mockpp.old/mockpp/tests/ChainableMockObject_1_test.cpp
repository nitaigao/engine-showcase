/***************************************************************************
                     ChainableMockObject_1_test.cpp
                                 -
  unit tests for ChainableMockObject class and 1 parameter methods
                             -------------------
    begin                : Thu Sep 28 2004
    copyright            : (C) 2002-2008 by Ewald Arnold
    email                : mockpp at ewald-arnold dot de

  $Id: ChainableMockObject_1_test.cpp 1438 2008-03-08 20:22:43Z ewald-arnold $

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

#define MOCKPP_ENABLE_DEFAULT_FORMATTER
#include <mockpp/mockpp.h> // always first

#include "SelectFramework.h"

#include <mockpp/chaining/ChainableMockObject.h>
#include <mockpp/chaining/ChainingMockObjectSupport.h>
#include <mockpp/chaining/Invocation.h>

#include <mockpp/constraint/OutBound.h>


MOCKPP_ANON_NS_START


class ChainableMockObject_1_test MOCKPP_DERIVE_PUBLIC_UNITFRAMEWORK
{
 public:

#if defined (MOCKPP_USE_CXXTEST)

#elif defined(MOCKPP_USE_BOOSTTEST)

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE( ChainableMockObject_1_test );

  CPPUNIT_TEST(test_outbound);
  CPPUNIT_TEST(test_parameter_1);
  CPPUNIT_TEST(test_resolver);

CPPUNIT_TEST_SUITE_END();

#endif

 public:

  MOCKPP_TESTMETHOD_DECL void test_outbound();
  MOCKPP_TESTMETHOD_DECL void test_parameter_1();
  MOCKPP_TESTMETHOD_DECL void test_resolver();
};


#if defined (MOCKPP_USE_CXXTEST)

MOCKPP_CXXTEST_SUITE_REGISTRATION( ChainableMockObject_1_test );
MOCKPP_CXXTEST(ChainableMockObject_1_test, test_outbound);
MOCKPP_CXXTEST(ChainableMockObject_1_test,  test_parameter_1 );
MOCKPP_CXXTEST(ChainableMockObject_1_test,  test_resolver);

#elif defined(MOCKPP_USE_BOOSTTEST)

MOCKPP_BOOST_TEST(ChainableMockObject_1_test::test_outbound);
MOCKPP_BOOST_TEST(ChainableMockObject_1_test::test_parameter_1 );
MOCKPP_BOOST_TEST(ChainableMockObject_1_test::test_resolver);

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE_REGISTRATION (ChainableMockObject_1_test);

#endif // frameworks


class MyChainableMockObject_1 : public MOCKPP_NS::ChainableMockObject
{
  public:

    MyChainableMockObject_1(const MOCKPP_NS::String &name)
      : MOCKPP_NS::ChainableMockObject(name, 0)
      , MOCKPP_CONSTRUCT_MEMBERS_FOR_CHAINABLE1(chainable)
    {}

    MOCKPP_CHAINABLE1(MyChainableMockObject_1, int, chainable, unsigned);

};


class MyChainableMockObject_1ex : public MOCKPP_NS::ChainableMockObject
{
  public:

    MyChainableMockObject_1ex(const MOCKPP_NS::String &name)
      : MOCKPP_NS::ChainableMockObject(name, 0)
      , MOCKPP_CONSTRUCT_MEMBERS_FOR_CHAINABLE_EXT1(chainable1, Ext)
      , MOCKPP_CONSTRUCT_MEMBERS_FOR_CHAINABLE_EXT1(outbound, Ext)
    {}

    MOCKPP_CHAINABLE_EXT1(MyChainableMockObject_1ex, MOCKPP_STL::string, chainable1, const MOCKPP_STL::string &,
                                                     MOCKPP_STL::string, Ext,        MOCKPP_STL::string);

    MOCKPP_CHAINABLE_EXT1(MyChainableMockObject_1ex, int, outbound, unsigned &,
                                                     int, Ext,      unsigned );

};


void ChainableMockObject_1_test::test_parameter_1()
{
  MyChainableMockObject_1 mcm(MOCKPP_PCHAR("chainable-object"));

  MOCKPP_CHAINER_FOR(MyChainableMockObject_1, chainable) chain(&mcm);

  chain.stubs(MOCKPP_NS::once())
       .with(MOCKPP_NS::eq(12u))
       .will(MOCKPP_NS::returnValue<int>(21));

  chain.stubs(MOCKPP_NS::once())
       .with(MOCKPP_NS::eq(34u)) // @todoc pitfall
       .will(MOCKPP_NS::returnValue<int>(43));

  MOCKPP_ASSERT_TRUE(mcm.chainable(12) == 21);
  MOCKPP_ASSERT_TRUE(mcm.chainable(34) == 43);

  MOCKPP_TRY
  {
    mcm.chainable(56);
    MOCKPP_ASSERT_TRUE_MESSAGE(MOCKPP_PCHAR("should have thrown"), false);
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_NS::AssertionFailedError &ex)
  {
    MOCKPP_ASSERT_TRUE(ex.getMessage().find(MOCKPP_PCHAR("unexpected invocation")) != MOCKPP_NS::String::npos);
  }
#endif

  mcm.verify();
}


void ChainableMockObject_1_test::test_outbound()
{
  MyChainableMockObject_1ex mcm(MOCKPP_PCHAR("chainable-object"));

  MOCKPP_CHAINER_FOR_EXT(MyChainableMockObject_1ex, outbound, Ext) chain(&mcm);

  chain.stubs(MOCKPP_NS::once())
       .with(MOCKPP_NS::outBound( (unsigned int) 123));

  chain.stubs(MOCKPP_NS::once())
       .with(MOCKPP_NS::outBound( (unsigned int) 456));

  unsigned val = 0;
  mcm.outbound(val);
  MOCKPP_ASSERT_EQUALS((unsigned)123, val);
  mcm.outbound(val);
  MOCKPP_ASSERT_EQUALS((unsigned)456, val);
}


class MockUnitResolver : public MOCKPP_NS::ChainableMockObject
{

public:

    MockUnitResolver()
        : MOCKPP_NS::ChainableMockObject( MOCKPP_PCHAR("MockUnitResolver"), 0 )
        , MOCKPP_CONSTRUCT_MEMBERS_FOR_CHAINABLE_EXT1( Find, _ )
    {}

    MOCKPP_CONST_CHAINABLE_EXT1( MockUnitResolver, int*, Find, const MOCKPP_NS::String&,
                                                   int*, _   , MOCKPP_NS::String );
};


void ChainableMockObject_1_test::test_resolver()
{
    MockUnitResolver resolver;
    MOCKPP_CHAINER_FOR_EXT( MockUnitResolver, Find, _) resolverChainer( &resolver );
    resolverChainer.stubs( MOCKPP_NS::once() ).will( new MOCKPP_NS::ReturnStub< int* >( 0 ) );
    resolverChainer.reset();
    resolver.reset();
}


MOCKPP_NS_END


