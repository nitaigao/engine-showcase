/***************************************************************************
 ThrowableList_test.cpp  -  unit tests for ThrowableList class
                             -------------------
    begin                : Fri Dez 27 2002
    copyright            : (C) 2002-2008 by Ewald Arnold
    email                : mockpp at ewald-arnold dot de

  $Id: ThrowableList_test.cpp 1438 2008-03-08 20:22:43Z ewald-arnold $

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

#include <mockpp/ThrowableList.h>
#include <mockpp/Throwable.h>

#include <mockpp/compat/Asserter.h>



class ThrowableList_test MOCKPP_DERIVE_PUBLIC_UNITFRAMEWORK
{
 public:

#if defined (MOCKPP_USE_CXXTEST)

#elif defined(MOCKPP_USE_BOOSTTEST)

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE( ThrowableList_test );

  CPPUNIT_TEST(test_throw);
  CPPUNIT_TEST(test_at);
  CPPUNIT_TEST(test_reset);
  CPPUNIT_TEST(test_setter);

CPPUNIT_TEST_SUITE_END();

#endif

 public:

  MOCKPP_TESTMETHOD_DECL void test_throw();
  MOCKPP_TESTMETHOD_DECL void test_at();
  MOCKPP_TESTMETHOD_DECL void test_reset();
  MOCKPP_TESTMETHOD_DECL void test_setter();
};


#if defined (MOCKPP_USE_CXXTEST)

MOCKPP_CXXTEST_SUITE_REGISTRATION( ThrowableList_test );

MOCKPP_CXXTEST(ThrowableList_test, test_throw);
MOCKPP_CXXTEST(ThrowableList_test, test_at);
MOCKPP_CXXTEST(ThrowableList_test, test_reset);
MOCKPP_CXXTEST(ThrowableList_test, test_setter);

#elif defined(MOCKPP_USE_BOOSTTEST)

MOCKPP_BOOST_TEST(ThrowableList_test::test_throw);
MOCKPP_BOOST_TEST(ThrowableList_test::test_at);
MOCKPP_BOOST_TEST(ThrowableList_test::test_reset);
MOCKPP_BOOST_TEST(ThrowableList_test::test_setter);

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE_REGISTRATION (ThrowableList_test);

#endif // frameworks



void ThrowableList_test::test_throw()
{
  MOCKPP_NS::ThrowableList tl(MOCKPP_PCHAR("tl"), 0);
  tl.push_back(MOCKPP_NS::make_throwable(MOCKPP_STL::string("my-item 1")));
  tl.addObjectToThrow(MOCKPP_STL::string("my-item 2"));

  MOCKPP_TRY
  {
    tl.nextThrowableObject()->throw_me();
    MOCKPP_FAIL(MOCKPP_PCHAR("Did not throw the exception"));
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_STL::string &s)
  {
    MOCKPP_ASSERT_TRUE(s == "my-item 1");
  }
  catch(...)
  {
    MOCKPP_FAIL(MOCKPP_PCHAR("Caught unexpected exception"));
  }
#endif

  MOCKPP_TRY
  {
    tl.nextThrowableObject()->throw_me();
    MOCKPP_FAIL(MOCKPP_PCHAR("Did not throw the exception"));
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_STL::string &s)
  {
    MOCKPP_ASSERT_TRUE(s == "my-item 2");
  }
  catch(...)
  {
    MOCKPP_FAIL(MOCKPP_PCHAR("Caught unexpected exception"));
  }
#endif

  MOCKPP_ASSERT_TRUE(tl.hasMoreObjects() == false);
  tl.verify();
}


void ThrowableList_test::test_reset()
{
  MOCKPP_NS::ThrowableList tl(MOCKPP_PCHAR("tl"), 0);

  MOCKPP_NS::Throwable *tt1 = MOCKPP_NS::make_throwable(MOCKPP_STL::string("my-item 1"));
  MOCKPP_NS::Throwable *tt2 = MOCKPP_NS::make_throwable(MOCKPP_STL::string("my-item 2"));
  MOCKPP_NS::Throwable *tt3 = MOCKPP_NS::make_throwable(MOCKPP_STL::string("my-item 3"));
  MOCKPP_NS::Throwable *tt4 = MOCKPP_NS::make_throwable(MOCKPP_STL::string("my-item 4"));
  tl.push_back(tt1);
  tl.push_back(tt2);
  tl.addObjectToThrow(tt3);
  tl.addObjectToThrow(tt4);
  MOCKPP_ASSERT_TRUE(tl.hasMoreObjects() == true);
  tl.reset();
  MOCKPP_ASSERT_TRUE(tl.hasMoreObjects() == false);
  tl.verify();
}


void ThrowableList_test::test_at()
{
  MOCKPP_NS::ThrowableList tl(MOCKPP_PCHAR("tl"), 0);

  MOCKPP_NS::Throwable *tt1 = MOCKPP_NS::make_throwable(MOCKPP_STL::string("my-item 1"));
  MOCKPP_NS::Throwable *tt2 = MOCKPP_NS::make_throwable(MOCKPP_STL::string("my-item 2"));
  MOCKPP_NS::Throwable *tt3 = MOCKPP_NS::make_throwable(MOCKPP_STL::string("my-item 3"));
  MOCKPP_NS::Throwable *tt4 = MOCKPP_NS::make_throwable(MOCKPP_STL::string("my-item 4"));

  tl.push_back(tt1);
  tl.push_back(tt2);
  tl.addObjectToThrow(tt3);
  tl.addObjectToThrow(tt4);

  MOCKPP_ASSERT_TRUE(tl.at(0) == tt1);
  MOCKPP_ASSERT_TRUE(tl.at(1) == tt2);
  MOCKPP_ASSERT_TRUE(tl.at(2) == tt3);
  MOCKPP_ASSERT_TRUE(tl.at(3) == tt4);

  tl.nextThrowableObject();         // shift objects internally
  tl.nextThrowableObject();

  MOCKPP_ASSERT_TRUE(tl.at(0) == tt1);  // still get the correct objects
  MOCKPP_ASSERT_TRUE(tl.at(1) == tt2);
  MOCKPP_ASSERT_TRUE(tl.at(2) == tt3);
  MOCKPP_ASSERT_TRUE(tl.at(3) == tt4);

  tl.nextThrowableObject();
  tl.nextThrowableObject();

  MOCKPP_ASSERT_TRUE(tl.at(0) == tt1);  // still get the correct objects
  MOCKPP_ASSERT_TRUE(tl.at(1) == tt2);
  MOCKPP_ASSERT_TRUE(tl.at(2) == tt3);
  MOCKPP_ASSERT_TRUE(tl.at(3) == tt4);

  tl.verify();
}


void ThrowableList_test::test_setter()
{
  MOCKPP_NS::ThrowableList tl(MOCKPP_PCHAR("tl"), 0);

  MOCKPP_NS::Throwable *tt1 = MOCKPP_NS::make_throwable(MOCKPP_STL::string("my-item 1"));
  tl.push_back(tt1);
  MOCKPP_ASSERT_TRUE(tl.hasMoreObjects() == true);
  tl.push_back(MOCKPP_NS::make_throwable(MOCKPP_STL::string("my-item 2")));
  tl.addObjectToThrow(MOCKPP_STL::string("my-item 3"));
  tl.addObjectToThrow(MOCKPP_STL::string("my-item 4"));

  MOCKPP_ASSERT_TRUE(tl.hasMoreObjects() == true);

  MOCKPP_ASSERT_TRUE(tl.nextThrowableObject() == tt1);

  tl.reset();
  MOCKPP_ASSERT_TRUE(tl.hasMoreObjects() == false);
  tl.verify();
}



