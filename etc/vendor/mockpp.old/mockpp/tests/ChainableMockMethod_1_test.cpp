/***************************************************************************
                     ChainableMockMethod_1_test.cpp
                                 -
  unit tests for ChainableMockObject class and 1 parameter methods
                             -------------------
    begin                : Thu Oct 2 2005
    copyright            : (C) 2002-2008 by Ewald Arnold
    email                : mockpp at ewald-arnold dot de

  $Id: ChainableMockMethod_1_test.cpp 1438 2008-03-08 20:22:43Z ewald-arnold $

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

#include <mockpp/chaining/ChainableMockMethod.h>
#include <mockpp/chaining/ChainableMockMethod0.h>
#include <mockpp/chaining/ChainableMockMethod1.h>
#include <mockpp/chaining/ChainableMockMethod2.h>
#include <mockpp/chaining/ChainableMockMethod3.h>
#include <mockpp/chaining/ChainableMockMethod4.h>
#include <mockpp/chaining/ChainableMockMethod5.h>

#include <mockpp/chaining/ChainingMockObjectSupport.h>
#include <mockpp/chaining/Invocation.h>

#include <mockpp/constraint/OutBound.h>

#include "IncCounter.h"



MOCKPP_ANON_NS_START


class ChainableMockMethod_1_test MOCKPP_DERIVE_PUBLIC_UNITFRAMEWORK
{
 public:

#if defined (MOCKPP_USE_CXXTEST)

#elif defined(MOCKPP_USE_BOOSTTEST)

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE( ChainableMockMethod_1_test );

  CPPUNIT_TEST( test_parameter_1 );
  CPPUNIT_TEST( test_parameter_1v );

CPPUNIT_TEST_SUITE_END();

#endif

 public:

  MOCKPP_TESTMETHOD_DECL void test_parameter_1();
  MOCKPP_TESTMETHOD_DECL void test_parameter_1v();
};


#if defined (MOCKPP_USE_CXXTEST)

MOCKPP_CXXTEST_SUITE_REGISTRATION( ChainableMockMethod_1_test );
MOCKPP_CXXTEST(ChainableMockMethod_1_test,  test_parameter_1 );
MOCKPP_CXXTEST(ChainableMockMethod_1_test,  test_parameter_1v );

#elif defined(MOCKPP_USE_BOOSTTEST)

MOCKPP_BOOST_TEST(ChainableMockMethod_1_test::test_parameter_1 );
MOCKPP_BOOST_TEST(ChainableMockMethod_1_test::test_parameter_1v );

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE_REGISTRATION (ChainableMockMethod_1_test);

#endif // frameworks



class MyChainableMockObject_1 : public MOCKPP_NS::ChainableMockObject
{
  public:

    MyChainableMockObject_1(const MOCKPP_NS::String &name)
      : MOCKPP_NS::ChainableMockObject(name, 0)
      , chainable_mocker(MOCKPP_PCHAR("chainable"), this)
      , chainablev_mocker(MOCKPP_PCHAR("chainablev"), this)
    {}

    int chainable(unsigned i)
    {
      return chainable_mocker.forward(i);
    }

    void chainablev(unsigned i)
    {
      chainablev_mocker.forward(i);
    }

    MOCKPP_NS::ChainableMockMethod1<int, unsigned>  chainable_mocker;
    MOCKPP_NS::ChainableMockMethod1<void, unsigned> chainablev_mocker;
};


void ChainableMockMethod_1_test::test_parameter_1()
{
  MyChainableMockObject_1 mcm(MOCKPP_PCHAR("chainable-object"));

  MOCKPP_NS::ChainableMockMethod1<int, unsigned> &chain(mcm.chainable_mocker);

  chain.stubs(MOCKPP_NS::once())
       .with(MOCKPP_NS::eq(12u))
       .will(MOCKPP_NS::returnValue<int>(21));

  chain.stubs(MOCKPP_NS::once())
       .with(MOCKPP_NS::eq(34u))
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


void ChainableMockMethod_1_test::test_parameter_1v()
{
  MyChainableMockObject_1 mcm(MOCKPP_PCHAR("chainable-object"));

  MOCKPP_NS::ChainableMockMethod1<void, unsigned> &chain(mcm.chainablev_mocker);
  unsigned counter = 0;

  chain.stubs(MOCKPP_NS::once())
       .with(MOCKPP_NS::eq(12u))
       .will(new IncCounter<unsigned>(counter));

  chain.stubs(MOCKPP_NS::once())
       .with(MOCKPP_NS::eq(34u))
       .will(new IncCounter<unsigned>(counter));

  mcm.chainablev(12);
  MOCKPP_ASSERT_EQUALS(1u, counter);

  mcm.chainablev(34);
  MOCKPP_ASSERT_EQUALS(2u, counter);

  MOCKPP_TRY
  {
    mcm.chainablev(56);
    MOCKPP_ASSERT_TRUE_MESSAGE(MOCKPP_PCHAR("should have thrown"), false);
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_NS::AssertionFailedError &ex)
  {
    MOCKPP_ASSERT_TRUE(ex.getMessage().find(MOCKPP_PCHAR("unexpected invocation")) != MOCKPP_NS::String::npos);
  }
#endif

  mcm.verify();
  mcm.reset();
//  chain.reset();
}


MOCKPP_NS_END




