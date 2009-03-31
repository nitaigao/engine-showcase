/***************************************************************************
                     ChainableMockMethod_test.cpp
                                 -
  unit tests for ChainableMockObject class and no parameter methods
                             -------------------
    begin                : Sat Oct 15 2005
    copyright            : (C) 2002-2008 by Ewald Arnold
    email                : mockpp at ewald-arnold dot de

  $Id: ChainableMockMethod_test.cpp 1438 2008-03-08 20:22:43Z ewald-arnold $

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
#include <mockpp/chaining/ChainableMockMethod6.h>
#include <mockpp/chaining/CountedChainableMethod.h>

#include <mockpp/chaining/ChainingMockObjectSupport.h>
#include <mockpp/chaining/Invocation.h>

#include <mockpp/constraint/OutBound.h>

#include <mockpp/CountParameters.h>

#include "IncCounter.h"



MOCKPP_ANON_NS_START


class ChainableMockMethod_test MOCKPP_DERIVE_PUBLIC_UNITFRAMEWORK
{
 public:

#if defined (MOCKPP_USE_CXXTEST)

#elif defined(MOCKPP_USE_BOOSTTEST)

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE( ChainableMockMethod_test );

  CPPUNIT_TEST( test_parameter_0 );
  CPPUNIT_TEST( test_parameter_0v );
  CPPUNIT_TEST( test_inst );

CPPUNIT_TEST_SUITE_END();

#endif

 public:

  MOCKPP_TESTMETHOD_DECL void test_parameter_0();
  MOCKPP_TESTMETHOD_DECL void test_parameter_0v();
  MOCKPP_TESTMETHOD_DECL void test_inst();
};


#if defined (MOCKPP_USE_CXXTEST)

MOCKPP_CXXTEST_SUITE_REGISTRATION( ChainableMockMethod_test );
MOCKPP_CXXTEST(ChainableMockMethod_test,  test_parameter_0 );
MOCKPP_CXXTEST(ChainableMockMethod_test,  test_parameter_0v );
MOCKPP_CXXTEST(ChainableMockMethod_test,  test_inst );

#elif defined(MOCKPP_USE_BOOSTTEST)

MOCKPP_BOOST_TEST(ChainableMockMethod_test:: test_parameter_0 );
MOCKPP_BOOST_TEST(ChainableMockMethod_test:: test_parameter_0v );
MOCKPP_BOOST_TEST(ChainableMockMethod_test:: test_inst );

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE_REGISTRATION (ChainableMockMethod_test);

#endif // frameworks



class MyChainableMockObject_0 : public MOCKPP_NS::ChainableMockObject
{
  public:

    MyChainableMockObject_0(const MOCKPP_NS::String &name)
      : MOCKPP_NS::ChainableMockObject(name, 0)
      , chainable_mocker(MOCKPP_PCHAR("chainable"), this)
      , chainablev_mocker(MOCKPP_PCHAR("chainablev"), this)
    {}

    int chainable()
    {
      return chainable_mocker.forward();
    }

    void chainablev()
    {
      chainablev_mocker.forward();
    }

    MOCKPP_NS::ChainableMockMethod0<int>  chainable_mocker;
    MOCKPP_NS::ChainableMockMethod0<void> chainablev_mocker;
};



void ChainableMockMethod_test::test_inst()
{
  MOCKPP_NS::ChainableMockMethod0<void> mmv0(MOCKPP_PCHAR("mm0"), 0);
  MOCKPP_NS::ChainableMockMethod1<void, int> mmv1(MOCKPP_PCHAR("mm1"), 0);
  MOCKPP_NS::ChainableMockMethod2<void, int, int> mmv2(MOCKPP_PCHAR("mm2"), 0);
  MOCKPP_NS::ChainableMockMethod3<void, int, int, int> mmv3(MOCKPP_PCHAR("mm3"), 0);
  MOCKPP_NS::ChainableMockMethod4<void, int, int, int, int> mmv4(MOCKPP_PCHAR("mm4"), 0);
  MOCKPP_NS::ChainableMockMethod5<void, int, int, int, int, int> mmv5(MOCKPP_PCHAR("mm5"), 0);
  MOCKPP_NS::ChainableMockMethod6<void, int, int, int, int, int, int> mmv6(MOCKPP_PCHAR("mm6"), 0);

  MOCKPP_NS::ChainableMockMethod0<int> mmi0(MOCKPP_PCHAR("mm0"), 0);
  MOCKPP_NS::ChainableMockMethod1<int, int> mmi1(MOCKPP_PCHAR("mm1"), 0);
  MOCKPP_NS::ChainableMockMethod2<int, int, int> mmi2(MOCKPP_PCHAR("mm2"), 0);
  MOCKPP_NS::ChainableMockMethod3<int, int, int, int> mmi3(MOCKPP_PCHAR("mm3"), 0);
  MOCKPP_NS::ChainableMockMethod4<int, int, int, int, int> mmi4(MOCKPP_PCHAR("mm4"), 0);
  MOCKPP_NS::ChainableMockMethod5<int, int, int, int, int, int> mmi5(MOCKPP_PCHAR("mm5"), 0);
  MOCKPP_NS::ChainableMockMethod6<int, int, int, int, int, int, int> mmi6(MOCKPP_PCHAR("mm6"), 0);

  MOCKPP_NS::ChainableMockMethod<void> mmvd0(MOCKPP_PCHAR("mm0"), 0);
  MOCKPP_NS::ChainableMockMethod<void, int> mmvd1(MOCKPP_PCHAR("mm1"), 0);
  MOCKPP_NS::ChainableMockMethod<void, int, int> mmvd2(MOCKPP_PCHAR("mm2"), 0);
  MOCKPP_NS::ChainableMockMethod<void, int, int, int> mmvd3(MOCKPP_PCHAR("mm3"), 0);
  MOCKPP_NS::ChainableMockMethod<void, int, int, int, int> mmvd4(MOCKPP_PCHAR("mm4"), 0);
  MOCKPP_NS::ChainableMockMethod<void, int, int, int, int, int> mmvd5(MOCKPP_PCHAR("mm5"), 0);
  MOCKPP_NS::ChainableMockMethod<void, int, int, int, int, int, int> mmvd6(MOCKPP_PCHAR("mm6"), 0);

  MOCKPP_NS::ChainableMockMethod<int> mmid0(MOCKPP_PCHAR("mm0"), 0);
  MOCKPP_NS::ChainableMockMethod<int, int> mmid1(MOCKPP_PCHAR("mm1"), 0);
  MOCKPP_NS::ChainableMockMethod<int, int, int> mmid2(MOCKPP_PCHAR("mm2"), 0);
  MOCKPP_NS::ChainableMockMethod<int, int, int, int> mmid3(MOCKPP_PCHAR("mm3"), 0);
  MOCKPP_NS::ChainableMockMethod<int, int, int, int, int> mmid4(MOCKPP_PCHAR("mm4"), 0);
  MOCKPP_NS::ChainableMockMethod<int, int, int, int, int, int> mmid5(MOCKPP_PCHAR("mm5"), 0);
  MOCKPP_NS::ChainableMockMethod<int, int, int, int, int, int, int> mmid6(MOCKPP_PCHAR("mm6"), 0);
}


void ChainableMockMethod_test::test_parameter_0()
{
  MyChainableMockObject_0 mcm(MOCKPP_PCHAR("chainable-object"));

  MOCKPP_NS::ChainableMockMethod0<int>& chain(mcm.chainable_mocker);

  chain.stubs(MOCKPP_NS::once())
       .will(MOCKPP_NS::returnValue<int>(21));

  chain.stubs(MOCKPP_NS::once())
       .will(MOCKPP_NS::returnValue<int>(43));

  MOCKPP_ASSERT_TRUE(mcm.chainable() == 21);
  MOCKPP_ASSERT_TRUE(mcm.chainable() == 43);

  MOCKPP_TRY
  {
    mcm.chainable();
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


void ChainableMockMethod_test::test_parameter_0v()
{
  MyChainableMockObject_0 mcm(MOCKPP_PCHAR("chainable-object"));

  MOCKPP_NS::ChainableMockMethod0<void> &chain(mcm.chainablev_mocker);
  unsigned counter = 0;

  chain.stubs(MOCKPP_NS::once())
       .will(new IncCounter<unsigned>(counter));

  chain.stubs(MOCKPP_NS::once())
       .will(new IncCounter<unsigned>(counter));

  mcm.chainablev();
  MOCKPP_ASSERT_EQUALS(1u, counter);

  mcm.chainablev();
  MOCKPP_ASSERT_EQUALS(2u, counter);

  MOCKPP_TRY
  {
    mcm.chainablev();
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


MOCKPP_NS_END




