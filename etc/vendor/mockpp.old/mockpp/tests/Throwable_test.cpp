/***************************************************************************
         Throwable_test.cpp  -  unit tests for Throwable class
                             -------------------
    begin                : Fri Dez 27 2002
    copyright            : (C) 2002-2008 by Ewald Arnold
    email                : mockpp at ewald-arnold dot de

  $Id: Throwable_test.cpp 1438 2008-03-08 20:22:43Z ewald-arnold $

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

#include <mockpp/Throwable.h>
#include <mockpp/compat/Asserter.h>
#include <mockpp/util/AssertMo.h>


class Throwable_test MOCKPP_DERIVE_PUBLIC_UNITFRAMEWORK
{
 public:

#if defined (MOCKPP_USE_CXXTEST)

#elif defined(MOCKPP_USE_BOOSTTEST)

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE( Throwable_test );

  CPPUNIT_TEST(test_getter);
  CPPUNIT_TEST(test_used);
  CPPUNIT_TEST(test_throw);
  CPPUNIT_TEST(test_clear);
  CPPUNIT_TEST(test_copy);

CPPUNIT_TEST_SUITE_END();

#endif

 public:

  MOCKPP_TESTMETHOD_DECL void test_copy();
  MOCKPP_TESTMETHOD_DECL void test_getter();
  MOCKPP_TESTMETHOD_DECL void test_used();
  MOCKPP_TESTMETHOD_DECL void test_throw();
  MOCKPP_TESTMETHOD_DECL void test_clear();
};


#if defined (MOCKPP_USE_CXXTEST)

MOCKPP_CXXTEST_SUITE_REGISTRATION( Throwable_test );

MOCKPP_CXXTEST(Throwable_test, test_getter);
MOCKPP_CXXTEST(Throwable_test, test_used);
MOCKPP_CXXTEST(Throwable_test, test_throw);
MOCKPP_CXXTEST(Throwable_test, test_clear);
MOCKPP_CXXTEST(Throwable_test, test_copy);

#elif defined(MOCKPP_USE_BOOSTTEST)

MOCKPP_BOOST_TEST(Throwable_test::test_getter);
MOCKPP_BOOST_TEST(Throwable_test::test_used);
MOCKPP_BOOST_TEST(Throwable_test::test_throw);
MOCKPP_BOOST_TEST(Throwable_test::test_clear);
MOCKPP_BOOST_TEST(Throwable_test::test_copy);

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE_REGISTRATION (Throwable_test);

#endif // frameworks



void Throwable_test::test_copy()
{
  MOCKPP_NS::ThrowableItem a (MOCKPP_NS::make_throwable (123));
  MOCKPP_NS::ThrowableItem b;

  MOCKPP_ASSERT_TRUE(a.get() != 0);
  MOCKPP_ASSERT_TRUE(b.get() == 0);

  b = a;
  MOCKPP_ASSERT_TRUE(a.get() == 0);
  MOCKPP_ASSERT_TRUE(b.get() != 0);
}


void Throwable_test::test_getter()
{
  MOCKPP_ASSERT_TRUE(0 == MOCKPP_NS::ThrowableItem().get());

  MOCKPP_NS::Throwable* tt1 = MOCKPP_NS::make_throwable (123);
  MOCKPP_NS::Throwable* tt2 = MOCKPP_NS::make_throwable (123);

  MOCKPP_NS::ThrowableItem it (tt1);
  MOCKPP_ASSERT_TRUE(tt1 == it.get());

  it.take(tt2);
  MOCKPP_ASSERT_TRUE(tt2 == it.get());

  it.take(0);
}


void Throwable_test::test_throw()
{
  MOCKPP_NS::ThrowableItem it;
  it.take(MOCKPP_STL::string("my-item"));
  MOCKPP_TRY
  {
    it.get()->throw_me();
    MOCKPP_FAIL(MOCKPP_PCHAR("Did not throw the exception"));
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_STL::string &s)
  {
    MOCKPP_ASSERT_TRUE(s == "my-item");
  }
  catch(...)
  {
    MOCKPP_FAIL(MOCKPP_PCHAR("Caught unexpected exception"));
  }
#endif
}


void Throwable_test::test_used()
{
  MOCKPP_NS::ThrowableItem it;
  it.take(MOCKPP_STL::string("my-item"));

  MOCKPP_ASSERT_TRUE(false == it.get()->hasThrown());

#ifndef MOCKPP_NO_EXCEPTIONS
  MOCKPP_ASSERT_THROWING(it.get()->throw_me(), MOCKPP_PCSTRING("throw"),
                         MOCKPP_STL::string, "my-item");
#endif

  MOCKPP_ASSERT_TRUE(true == it.get()->hasThrown());
}


void Throwable_test::test_clear()
{
  MOCKPP_NS::ThrowableItem it;
  it.take(MOCKPP_STL::string("my-item"));

  MOCKPP_ASSERT_TRUE(it.get() != 0);
  it.reset();
  MOCKPP_ASSERT_TRUE(it.get() == 0);
}



