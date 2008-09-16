/***************************************************************************
             ReturnObjectList_test.cpp  -  unit tests for ReturnObjectList class
                             -------------------
    begin                : Fri Dez 27 2002
    copyright            : (C) 2002-2008 by Ewald Arnold
    email                : mockpp at ewald-arnold dot de

  $Id: ReturnObjectList_test.cpp 1438 2008-03-08 20:22:43Z ewald-arnold $

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

#include <mockpp/ReturnObjectList.h>


class ReturnObjectList_test MOCKPP_DERIVE_PUBLIC_UNITFRAMEWORK
{
 public:

#if defined (MOCKPP_USE_CXXTEST)

#elif defined(MOCKPP_USE_BOOSTTEST)

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE( ReturnObjectList_test );

  CPPUNIT_TEST(test_get_object);
  CPPUNIT_TEST(test_default);
  CPPUNIT_TEST(test_tostring);

  CPPUNIT_TEST(fail_get_object);
  CPPUNIT_TEST(fail_not_empty);

CPPUNIT_TEST_SUITE_END();

#endif

 public:

  MOCKPP_TESTMETHOD_DECL void test_get_object();
  MOCKPP_TESTMETHOD_DECL void test_default();
  MOCKPP_TESTMETHOD_DECL void test_tostring();

  MOCKPP_TESTMETHOD_DECL void fail_get_object();
  MOCKPP_TESTMETHOD_DECL void fail_not_empty();
};


#if defined (MOCKPP_USE_CXXTEST)

MOCKPP_CXXTEST_SUITE_REGISTRATION( ReturnObjectList_test );

MOCKPP_CXXTEST(ReturnObjectList_test, test_get_object);
MOCKPP_CXXTEST(ReturnObjectList_test, test_default);
MOCKPP_CXXTEST(ReturnObjectList_test, test_tostring);

MOCKPP_CXXTEST(ReturnObjectList_test, fail_get_object);
MOCKPP_CXXTEST(ReturnObjectList_test, fail_not_empty);

#elif defined(MOCKPP_USE_BOOSTTEST)

MOCKPP_BOOST_TEST(ReturnObjectList_test::test_get_object);
MOCKPP_BOOST_TEST(ReturnObjectList_test::test_default);
MOCKPP_BOOST_TEST(ReturnObjectList_test::test_tostring);

MOCKPP_BOOST_TEST(ReturnObjectList_test::fail_get_object);
MOCKPP_BOOST_TEST(ReturnObjectList_test::fail_not_empty);

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE_REGISTRATION (ReturnObjectList_test);

#endif // frameworks



void ReturnObjectList_test::test_get_object()
{
  MOCKPP_NS::ReturnObjectList<char> rol (MOCKPP_PCHAR("data"), 0);

  char buff[] = "abcdefg";

  rol.addObjectToReturn('1');
  rol.addObjectToReturn('2');
  rol.addObjectToReturn(buff, buff+sizeof(buff)-1)  // take care of trailing '\0'!
     .addObjectToReturn('3')
     .addObjectToReturn('4')
     .addObjectToReturn('5', 3);

  MOCKPP_ASSERT_TRUE(rol.hasMoreObjects());

  MOCKPP_ASSERT_TRUE('1' == rol.nextReturnObject());
  MOCKPP_ASSERT_TRUE('2' == rol.nextReturnObject());
  MOCKPP_ASSERT_TRUE('a' == rol.nextReturnObject());
  MOCKPP_ASSERT_TRUE('b' == rol.nextReturnObject());
  MOCKPP_ASSERT_TRUE('c' == rol.nextReturnObject());
  MOCKPP_ASSERT_TRUE('d' == rol.nextReturnObject());
  MOCKPP_ASSERT_TRUE('e' == rol.nextReturnObject());
  MOCKPP_ASSERT_TRUE('f' == rol.nextReturnObject());
  MOCKPP_ASSERT_TRUE('g' == rol.nextReturnObject());
  MOCKPP_ASSERT_TRUE('3' == rol.nextReturnObject());
  MOCKPP_ASSERT_TRUE('4' == rol.nextReturnObject());
  MOCKPP_ASSERT_TRUE('5' == rol.nextReturnObject());
  MOCKPP_ASSERT_TRUE('5' == rol.nextReturnObject());
  MOCKPP_ASSERT_TRUE('5' == rol.nextReturnObject());

  MOCKPP_ASSERT_TRUE(!rol.hasMoreObjects());

  rol.verify();
}


void ReturnObjectList_test::fail_get_object()
{
  MOCKPP_NS::ReturnObjectList<char> rol (MOCKPP_PCHAR("data"), 0);

  char buff[] = "1234";
  rol.addObjectToReturn(buff, buff+sizeof(buff)-1);  // take care of trailing '\0'!
  rol.nextReturnObject();
  rol.nextReturnObject();
  rol.nextReturnObject();
  rol.nextReturnObject();
  bool thrown = false;
  MOCKPP_TRY
  {
    rol.nextReturnObject();  // fail
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_NS::AssertionFailedError &)
  {
    thrown = true;
  }
#endif
  MOCKPP_ASSERT_TRUE(thrown == true);
}


void ReturnObjectList_test::fail_not_empty()
{
  MOCKPP_NS::ReturnObjectList<char> rol (MOCKPP_PCHAR("data"), 0);

  char buff[] = "1234";
  rol.addObjectToReturn(buff, buff+sizeof(buff));
  bool thrown = false;
  MOCKPP_TRY
  {
    rol.verify();
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_NS::AssertionFailedError &)
  {
    thrown = true;
  }
#endif
  MOCKPP_ASSERT_TRUE(thrown == true);
}


void ReturnObjectList_test::test_default()
{
  MOCKPP_NS::ReturnObjectList<unsigned> rol (MOCKPP_PCHAR("data"), 0);
  rol.addObjectToReturn(456);
  rol.setDefaultReturnValue(123);

  MOCKPP_ASSERT_TRUE(456 == rol.nextReturnObject());
  MOCKPP_ASSERT_TRUE(123 == rol.nextReturnObject());
  MOCKPP_ASSERT_TRUE(123 == rol.nextReturnObject());
  rol.verify();

  rol.addObjectToReturn(456);
  rol.reset();
  MOCKPP_ASSERT_TRUE(!rol.hasMoreObjects());
}

void ReturnObjectList_test::test_tostring()
{
  MOCKPP_NS::ReturnObjectList<unsigned> rol (MOCKPP_PCHAR("data"), 0);
  MOCKPP_ASSERT_TRUE(MOCKPP_PCSTRING("[]") == rol.toString());

  rol.addObjectToReturn(456);
  MOCKPP_ASSERT_TRUE(MOCKPP_PCSTRING("\n[\n - 456\n]\n") == rol.toString());

  rol.setDefaultReturnValue(123);
  MOCKPP_ASSERT_TRUE(MOCKPP_PCSTRING("\n[\n - 456\n * 123\n]\n") == rol.toString());
}




