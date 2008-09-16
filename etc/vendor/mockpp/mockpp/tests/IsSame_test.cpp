/***************************************************************************
             IsSame_test.cpp  -  unit tests for IsSame class
                             -------------------
    begin                : Sat Aug 21 2004
    copyright            : (C) 2002-2008 by Ewald Arnold
    email                : mockpp at ewald-arnold dot de

  $Id: IsSame_test.cpp 1438 2008-03-08 20:22:43Z ewald-arnold $

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

#include <mockpp/constraint/IsSame.h>

#include <mockpp/compat/Asserter.h>


class IsSame_test MOCKPP_DERIVE_PUBLIC_UNITFRAMEWORK
{
 public:

#if defined (MOCKPP_USE_CXXTEST)

#elif defined(MOCKPP_USE_BOOSTTEST)

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE( IsSame_test );

  CPPUNIT_TEST(test_describe);
  CPPUNIT_TEST(test_invoke);

CPPUNIT_TEST_SUITE_END();

#endif

 public:

  MOCKPP_TESTMETHOD_DECL void test_describe();
  MOCKPP_TESTMETHOD_DECL void test_invoke();
};


#if defined (MOCKPP_USE_CXXTEST)

MOCKPP_CXXTEST_SUITE_REGISTRATION( IsSame_test );
MOCKPP_CXXTEST(IsSame_test, test_describe);
MOCKPP_CXXTEST(IsSame_test, test_invoke);

#elif defined(MOCKPP_USE_BOOSTTEST)

MOCKPP_BOOST_TEST(IsSame_test::test_describe);
MOCKPP_BOOST_TEST(IsSame_test::test_invoke);

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE_REGISTRATION (IsSame_test);

#endif // frameworks



void IsSame_test::test_describe()
{
   MOCKPP_NS::IsSame<int> same (12);
   MOCKPP_NS::String s = MOCKPP_PCHAR("close?");
   MOCKPP_ASSERT_TRUE(same.describeTo(s) == MOCKPP_PCHAR("close?sameAs 12"));
   MOCKPP_ASSERT_TRUE(s == MOCKPP_PCHAR("close?sameAs 12"));
}


void IsSame_test::test_invoke()
{
   MOCKPP_STL::string str = "string";
   MOCKPP_NS::IsSame<MOCKPP_STL::string> same (str);
   MOCKPP_ASSERT_TRUE(same.eval(str) == true);
   MOCKPP_ASSERT_TRUE(same.eval("string") == false);
}



