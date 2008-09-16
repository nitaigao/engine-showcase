/***************************************************************************
             TriggeredTriggeredOutbound_test.cpp  -  unit tests for TriggeredOutbound class
                             -------------------
    begin                : Thu Aug 01 2006
    copyright            : (C) 2002-2008 by Ewald Arnold
    email                : mockpp at ewald-arnold dot de

  $Id: TriggeredOutbound_test.cpp 1447 2008-05-17 18:07:20Z ewald-arnold $

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

#include <mockpp/constraint/TriggeredOutbound.h>


class TriggeredOutbound_test MOCKPP_DERIVE_PUBLIC_UNITFRAMEWORK
{
 public:

#if defined (MOCKPP_USE_CXXTEST)

#elif defined(MOCKPP_USE_BOOSTTEST)

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE( TriggeredOutbound_test );

  CPPUNIT_TEST(test_describe);
  CPPUNIT_TEST(test_invoke);

CPPUNIT_TEST_SUITE_END();

#endif

 public:

  MOCKPP_TESTMETHOD_DECL void test_describe();
  MOCKPP_TESTMETHOD_DECL void test_invoke();
};


#if defined (MOCKPP_USE_CXXTEST)

MOCKPP_CXXTEST_SUITE_REGISTRATION( TriggeredOutbound_test );
MOCKPP_CXXTEST(TriggeredOutbound_test, test_describe);
MOCKPP_CXXTEST(TriggeredOutbound_test, test_invoke);

#elif defined(MOCKPP_USE_BOOSTTEST)

MOCKPP_BOOST_TEST(TriggeredOutbound_test::test_describe);
MOCKPP_BOOST_TEST(TriggeredOutbound_test::test_invoke);

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE_REGISTRATION (TriggeredOutbound_test);

#endif // frameworks



void TriggeredOutbound_test::test_describe()
{
   MOCKPP_NS::TriggeredOutbound<int> myequal (456);
   MOCKPP_NS::String s;
   MOCKPP_ASSERT_TRUE(myequal.describeTo(s) == MOCKPP_PCHAR("stores \n[\n - 456\n]\n"));
   MOCKPP_ASSERT_TRUE(s == MOCKPP_PCHAR("stores \n[\n - 456\n]\n"));
}


void TriggeredOutbound_test::test_invoke()
{
   {
    MOCKPP_NS::TriggeredOutbound<MOCKPP_NS::String> myequal (MOCKPP_PCHAR("string-1"));
    myequal.addOutboundObject(MOCKPP_PCHAR("string-2"));
    MOCKPP_NS::String s = MOCKPP_PCHAR("empty");

    MOCKPP_ASSERT_TRUE(myequal.eval(s) == true);
    MOCKPP_ASSERT_TRUE(s == MOCKPP_PCHAR("empty"));
    myequal.trigger();
    MOCKPP_ASSERT_TRUE(s == MOCKPP_PCHAR("string-1"));

    myequal.eval(s);
    myequal.trigger();
    MOCKPP_ASSERT_TRUE(s == MOCKPP_PCHAR("string-2"));
   }

   // --

   {
    int intvec1[] = { 3, 6 };
    MOCKPP_NS::TriggeredOutbound<int> intoutbound (&intvec1[0], &intvec1[2]);

    int intvec2[] = { 2, 4 };
    intoutbound.addOutboundObject(&intvec2[0], &intvec2[2]);

    int intvar = 0;
    MOCKPP_ASSERT_FALSE(intoutbound.verify(intvar));

    intoutbound.eval(intvar);
    intoutbound.trigger();
    MOCKPP_ASSERT_TRUE(3 == intvar);

    intoutbound.eval(intvar);
    intoutbound.trigger();
    MOCKPP_ASSERT_TRUE(6 == intvar);

    intoutbound.eval(intvar);
    intoutbound.trigger();
    MOCKPP_ASSERT_EQUALS(2, intvar);

    intoutbound.eval(intvar);
    intoutbound.trigger();
    MOCKPP_ASSERT_TRUE(4 == intvar);

    MOCKPP_ASSERT_TRUE(intoutbound.verify(intvar));
  }
}

