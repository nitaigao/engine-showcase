/***************************************************************************
             Or_test.cpp  -  unit tests for Or class
                             -------------------
    begin                : Sat Aug 21 2004
    copyright            : (C) 2002-2008 by Ewald Arnold
    email                : mockpp at ewald-arnold dot de

  $Id: Or_test.cpp 1449 2008-05-22 14:57:05Z ewald-arnold $

 ***************************************************************************/

/**************************************************************************
 *
 * This program is free software; you can redistribute it Or/or modify
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

#include <mockpp/chaining/ChainingMockObjectSupport.h>

#include <mockpp/constraint/IsAnything.h>
#include <mockpp/constraint/IsNothing.h>

#include <mockpp/compat/Asserter.h>


class Or_test MOCKPP_DERIVE_PUBLIC_UNITFRAMEWORK
{
 public:

#if defined (MOCKPP_USE_CXXTEST)

#elif defined(MOCKPP_USE_BOOSTTEST)

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE( Or_test );

  CPPUNIT_TEST(test_invoke);
  CPPUNIT_TEST(test_describe);

CPPUNIT_TEST_SUITE_END();

#endif

 public:

  MOCKPP_TESTMETHOD_DECL void test_invoke();
  MOCKPP_TESTMETHOD_DECL void test_describe();
};


#if defined (MOCKPP_USE_CXXTEST)

MOCKPP_CXXTEST_SUITE_REGISTRATION( Or_test );
MOCKPP_CXXTEST(Or_test, test_describe);
MOCKPP_CXXTEST(Or_test, test_invoke);

#elif defined(MOCKPP_USE_BOOSTTEST)

MOCKPP_BOOST_TEST(Or_test::test_describe);
MOCKPP_BOOST_TEST(Or_test::test_invoke);

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE_REGISTRATION (Or_test);

#endif // frameworks



void Or_test::test_describe()
{
   MOCKPP_NS::Or<bool> or1 (new MOCKPP_NS::IsAnything,
                            new MOCKPP_NS::IsNothing);
   MOCKPP_NS::String s = MOCKPP_PCHAR("or1?");
   MOCKPP_ASSERT_TRUE(or1.describeTo(s) == MOCKPP_PCHAR("or1?(ANYTHING || NOTHING)"));
   MOCKPP_ASSERT_TRUE(s == MOCKPP_PCHAR("or1?(ANYTHING || NOTHING)"));
}


namespace {

template<typename T>
class CountedEqual : public MOCKPP_NS::IsEqual<T>
{
  public:

    CountedEqual( const T &equalArg )
        : MOCKPP_NS::IsEqual<T>( equalArg )
        , eval_ctr(0)
        , verify_ctr(0)
    {}

    virtual bool eval( const T &arg ) const
    {
      eval_ctr++;
      return MOCKPP_NS::IsEqual<T>::eval(arg);
    }

    virtual bool verify( const T &arg ) const
    {
      verify_ctr++;
      return MOCKPP_NS::IsEqual<T>::verify(arg);
    }

    mutable unsigned eval_ctr;
    mutable unsigned verify_ctr;
};

}

void Or_test::test_invoke()
{
  CountedEqual<int> *c1;
  CountedEqual<int> *c2;

  {
    MOCKPP_NS::Or<int> or1 (c1 = new CountedEqual<int>(1),
                            c2 = new CountedEqual<int>(5));

    MOCKPP_ASSERT_TRUE(or1.eval(1) == true);
    MOCKPP_ASSERT_TRUE(c1->eval_ctr == 1);
    MOCKPP_ASSERT_TRUE(c2->eval_ctr == 0);

    MOCKPP_ASSERT_TRUE(or1.eval(2) == false);
    MOCKPP_ASSERT_TRUE(c1->eval_ctr == 2);
    MOCKPP_ASSERT_TRUE(c2->eval_ctr == 1);

    MOCKPP_ASSERT_TRUE(or1.eval(4) == false);
    MOCKPP_ASSERT_TRUE(c1->eval_ctr == 3);
    MOCKPP_ASSERT_TRUE(c2->eval_ctr == 2);

    MOCKPP_ASSERT_TRUE(or1.eval(5) == true);
    MOCKPP_ASSERT_TRUE(c1->eval_ctr == 4);
    MOCKPP_ASSERT_TRUE(c2->eval_ctr == 3);

    // --

    MOCKPP_ASSERT_TRUE(or1.verify(1) == true);
    MOCKPP_ASSERT_TRUE(c1->verify_ctr == 1);
    MOCKPP_ASSERT_TRUE(c2->verify_ctr == 0);

    MOCKPP_ASSERT_TRUE(or1.verify(2) == false);
    MOCKPP_ASSERT_TRUE(c1->verify_ctr == 2);
    MOCKPP_ASSERT_TRUE(c2->verify_ctr == 1);

    MOCKPP_ASSERT_TRUE(or1.verify(4) == false);
    MOCKPP_ASSERT_TRUE(c1->verify_ctr == 3);
    MOCKPP_ASSERT_TRUE(c2->verify_ctr == 2);

    MOCKPP_ASSERT_TRUE(or1.verify(5) == true);
    MOCKPP_ASSERT_TRUE(c1->verify_ctr == 4);
    MOCKPP_ASSERT_TRUE(c2->verify_ctr == 3);
  }

  {
    MOCKPP_NS::Or<int> or1 (c1 = new CountedEqual<int>(1),
                            c2 = new CountedEqual<int>(5),
                            false);

    MOCKPP_ASSERT_TRUE(or1.eval(1) == true);
    MOCKPP_ASSERT_TRUE(c1->eval_ctr == 1);
    MOCKPP_ASSERT_TRUE(c2->eval_ctr == 1);

    MOCKPP_ASSERT_TRUE(or1.eval(2) == false);
    MOCKPP_ASSERT_TRUE(c1->eval_ctr == 2);
    MOCKPP_ASSERT_TRUE(c2->eval_ctr == 2);

    MOCKPP_ASSERT_TRUE(or1.eval(4) == false);
    MOCKPP_ASSERT_TRUE(c1->eval_ctr == 3);
    MOCKPP_ASSERT_TRUE(c2->eval_ctr == 3);

    MOCKPP_ASSERT_TRUE(or1.eval(5) == true);
    MOCKPP_ASSERT_TRUE(c1->eval_ctr == 4);
    MOCKPP_ASSERT_TRUE(c2->eval_ctr == 4);

    // --

    MOCKPP_ASSERT_TRUE(or1.verify(1) == true);
    MOCKPP_ASSERT_TRUE(c1->verify_ctr == 1);
    MOCKPP_ASSERT_TRUE(c2->verify_ctr == 1);

    MOCKPP_ASSERT_TRUE(or1.verify(2) == false);
    MOCKPP_ASSERT_TRUE(c1->verify_ctr == 2);
    MOCKPP_ASSERT_TRUE(c2->verify_ctr == 2);

    MOCKPP_ASSERT_TRUE(or1.verify(4) == false);
    MOCKPP_ASSERT_TRUE(c1->verify_ctr == 3);
    MOCKPP_ASSERT_TRUE(c2->verify_ctr == 3);

    MOCKPP_ASSERT_TRUE(or1.verify(5) == true);
    MOCKPP_ASSERT_TRUE(c1->verify_ctr == 4);
    MOCKPP_ASSERT_TRUE(c2->verify_ctr == 4);
  }
}



