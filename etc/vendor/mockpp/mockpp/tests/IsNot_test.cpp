/***************************************************************************
             IsNot_test.cpp  -  unit tests for IsNot class
                             -------------------
    begin                : Sat Aug 21 2004
    copyright            : (C) 2002-2008 by Ewald Arnold
    email                : mockpp at ewald-arnold dot de

  $Id: IsNot_test.cpp 1445 2008-05-16 19:08:55Z ewald-arnold $

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

#include <mockpp/constraint/IsNot.h>
#include <mockpp/constraint/IsAnything.h>

#include <mockpp/compat/Asserter.h>


class IsNot_test MOCKPP_DERIVE_PUBLIC_UNITFRAMEWORK
{
 public:

#if defined (MOCKPP_USE_CXXTEST)

#elif defined(MOCKPP_USE_BOOSTTEST)

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE( IsNot_test );

  CPPUNIT_TEST(test_invoke);
  CPPUNIT_TEST(test_describe);

CPPUNIT_TEST_SUITE_END();

#endif

 public:

  MOCKPP_TESTMETHOD_DECL void test_invoke();
  MOCKPP_TESTMETHOD_DECL void test_describe();
};


#if defined (MOCKPP_USE_CXXTEST)

MOCKPP_CXXTEST_SUITE_REGISTRATION( IsNot_test );
MOCKPP_CXXTEST(IsNot_test, test_describe);
MOCKPP_CXXTEST(IsNot_test, test_invoke);

#elif defined(MOCKPP_USE_BOOSTTEST)

MOCKPP_BOOST_TEST(IsNot_test::test_describe);
MOCKPP_BOOST_TEST(IsNot_test::test_invoke);

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE_REGISTRATION (IsNot_test);

#endif // frameworks



void IsNot_test::test_describe()
{
   MOCKPP_NS::IsAnything myany;
   MOCKPP_NS::IsNot<bool> mynot (new MOCKPP_NS::IsAnything);
   MOCKPP_NS::String s = MOCKPP_PCHAR("mynot?");
   MOCKPP_ASSERT_TRUE(mynot.describeTo(s) == MOCKPP_PCHAR("mynot?not ANYTHING"));
   MOCKPP_ASSERT_TRUE(s == MOCKPP_PCHAR("mynot?not ANYTHING"));
}


void IsNot_test::test_invoke()
{
   MOCKPP_NS::IsAnything myany;
   MOCKPP_ASSERT_TRUE(myany.typelessEval() == true);

   MOCKPP_NS::IsNot<bool> mynot (new MOCKPP_NS::IsAnything);

   MOCKPP_ASSERT_TRUE(mynot.eval(true) == false);
   MOCKPP_ASSERT_TRUE(mynot.eval(false) == false);

   MOCKPP_ASSERT_TRUE(mynot.verify(true) == false);
   MOCKPP_ASSERT_TRUE(mynot.verify(false) == false);
}



