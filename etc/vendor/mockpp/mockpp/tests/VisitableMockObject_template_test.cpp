/***************************************************************************
                 VisitableMockObject_template_test.cpp
                                 -
              unit tests for VisitableMockObject templates
                             -------------------
    begin                : Son Feb 16 2003
    copyright            : (C) 2002-2008 by Ewald Arnold
    email                : mockpp at ewald-arnold dot de

  $Id: VisitableMockObject_template_test.cpp 1438 2008-03-08 20:22:43Z ewald-arnold $

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

#include <mockpp/visiting/VisitableMockObject.h>


class VisitableMockObject_template_test MOCKPP_DERIVE_PUBLIC_UNITFRAMEWORK
{
 public:

#if defined (MOCKPP_USE_CXXTEST)

#elif defined(MOCKPP_USE_BOOSTTEST)

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE( VisitableMockObject_template_test );

  CPPUNIT_TEST(test_value_1);
  CPPUNIT_TEST(test_value_2);
  CPPUNIT_TEST(test_value_3);
  CPPUNIT_TEST(test_value_4);
  CPPUNIT_TEST(test_value_5);
  CPPUNIT_TEST(test_throw_1);
  CPPUNIT_TEST(test_throw_2);
  CPPUNIT_TEST(test_throw_3);
  CPPUNIT_TEST(test_throw_4);
  CPPUNIT_TEST(test_throw_5);

CPPUNIT_TEST_SUITE_END();

#endif

 public:
  MOCKPP_TESTMETHOD_DECL void test_value_1();
  MOCKPP_TESTMETHOD_DECL void test_value_2();
  MOCKPP_TESTMETHOD_DECL void test_value_3();
  MOCKPP_TESTMETHOD_DECL void test_value_4();
  MOCKPP_TESTMETHOD_DECL void test_value_5();
  MOCKPP_TESTMETHOD_DECL void test_throw_1();
  MOCKPP_TESTMETHOD_DECL void test_throw_2();
  MOCKPP_TESTMETHOD_DECL void test_throw_3();
  MOCKPP_TESTMETHOD_DECL void test_throw_4();
  MOCKPP_TESTMETHOD_DECL void test_throw_5();
};


#if defined (MOCKPP_USE_CXXTEST)

MOCKPP_CXXTEST_SUITE_REGISTRATION( VisitableMockObject_template_test );

MOCKPP_CXXTEST(VisitableMockObject_template_test, test_value_1);
MOCKPP_CXXTEST(VisitableMockObject_template_test, test_value_2);
MOCKPP_CXXTEST(VisitableMockObject_template_test, test_value_3);
MOCKPP_CXXTEST(VisitableMockObject_template_test, test_value_4);
MOCKPP_CXXTEST(VisitableMockObject_template_test, test_value_5);
MOCKPP_CXXTEST(VisitableMockObject_template_test, test_throw_1);
MOCKPP_CXXTEST(VisitableMockObject_template_test, test_throw_2);
MOCKPP_CXXTEST(VisitableMockObject_template_test, test_throw_3);
MOCKPP_CXXTEST(VisitableMockObject_template_test, test_throw_4);
MOCKPP_CXXTEST(VisitableMockObject_template_test, test_throw_5);

#elif defined(MOCKPP_USE_BOOSTTEST)

MOCKPP_BOOST_TEST(VisitableMockObject_template_test::test_value_1);
MOCKPP_BOOST_TEST(VisitableMockObject_template_test::test_value_2);
MOCKPP_BOOST_TEST(VisitableMockObject_template_test::test_value_3);
MOCKPP_BOOST_TEST(VisitableMockObject_template_test::test_value_4);
MOCKPP_BOOST_TEST(VisitableMockObject_template_test::test_value_5);
MOCKPP_BOOST_TEST(VisitableMockObject_template_test::test_throw_1);
MOCKPP_BOOST_TEST(VisitableMockObject_template_test::test_throw_2);
MOCKPP_BOOST_TEST(VisitableMockObject_template_test::test_throw_3);
MOCKPP_BOOST_TEST(VisitableMockObject_template_test::test_throw_4);
MOCKPP_BOOST_TEST(VisitableMockObject_template_test::test_throw_5);

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE_REGISTRATION (VisitableMockObject_template_test);

#endif // frameworks



void VisitableMockObject_template_test::test_value_1()
{
  MOCKPP_NS::ResponseVector1<int, int> rv(MOCKPP_PCHAR("rv"), 0);
  rv.add(10, 11, 1);
  rv.add(20, 21, 1);
  rv.add(30, 31, 1);

  int i;
  MOCKPP_ASSERT_TRUE(rv.find(i, 11) == true);
  MOCKPP_ASSERT_TRUE(10 == i);
  MOCKPP_ASSERT_TRUE(rv.find(i, 11) == false);

  MOCKPP_ASSERT_TRUE(rv.find(i, 21) == true);
  MOCKPP_ASSERT_TRUE(20 == i);
  MOCKPP_ASSERT_TRUE(rv.find(i, 21) == false);

  MOCKPP_ASSERT_TRUE(rv.find(i, 31) == true);
  MOCKPP_ASSERT_TRUE(30 == i);
  MOCKPP_ASSERT_TRUE(rv.find(i, 31) == false);

  MOCKPP_ASSERT_TRUE(rv.find(i, 0) == false);

  MOCKPP_NS::Throwable *t10 = MOCKPP_NS::make_throwable((int)11);
  MOCKPP_NS::Throwable *t20 = MOCKPP_NS::make_throwable((int)11);
  MOCKPP_NS::Throwable *t30 = MOCKPP_NS::make_throwable((int)11);

  rv.add(t10, 12, 1);
  rv.add(t20, 22, 1);
  rv.add(t30, 32, 1);

  MOCKPP_NS::Throwable *t;

  MOCKPP_ASSERT_TRUE(rv.find(t, 12) == true);
  MOCKPP_ASSERT_TRUE(t10 == t);
  MOCKPP_ASSERT_TRUE(rv.find(t, 12) == false);

  MOCKPP_ASSERT_TRUE(rv.find(t, 22) == true);
  MOCKPP_ASSERT_TRUE(t20 == t);
  MOCKPP_ASSERT_TRUE(rv.find(t, 22) == false);

  MOCKPP_ASSERT_TRUE(rv.find(t, 32) == true);
  MOCKPP_ASSERT_TRUE(t30 == t);
  MOCKPP_ASSERT_TRUE(rv.find(t, 32) == false);

  MOCKPP_ASSERT_TRUE(rv.find(t, 0) == false);

  MOCKPP_NS::Throwable *t40 = MOCKPP_NS::make_throwable((int)11);
  rv.add(130, 131, 1);
  rv.add(t40, 132, 1);
  MOCKPP_ASSERT_TRUE(rv.find(t, 131) == false);  // across !
  MOCKPP_ASSERT_TRUE(rv.find(i, 132) == false);
}


void VisitableMockObject_template_test::test_throw_1()
{
  MOCKPP_NS::ResponseThrowableVector1<int> tv(MOCKPP_PCHAR("rv"), 0);

  MOCKPP_NS::Throwable *t10 = MOCKPP_NS::make_throwable((int)11);
  MOCKPP_NS::Throwable *t20 = MOCKPP_NS::make_throwable((int)11);
  MOCKPP_NS::Throwable *t30 = MOCKPP_NS::make_throwable((int)11);

  tv.add(t10, 11, 1);
  tv.add(t20, 21, 1);
  tv.add(t30, 31, 1);

  MOCKPP_NS::Throwable *t;

  MOCKPP_ASSERT_TRUE(tv.find(t, 11) == true);
  MOCKPP_ASSERT_TRUE(t10 == t);
  MOCKPP_ASSERT_TRUE(tv.find(t, 11) == false);

  MOCKPP_ASSERT_TRUE(tv.find(t, 21) == true);
  MOCKPP_ASSERT_TRUE(t20 == t);
  MOCKPP_ASSERT_TRUE(tv.find(t, 21) == false);

  MOCKPP_ASSERT_TRUE(tv.find(t, 31) == true);
  MOCKPP_ASSERT_TRUE(t30 == t);
  MOCKPP_ASSERT_TRUE(tv.find(t, 31) == false);

  MOCKPP_ASSERT_TRUE(tv.find(t, 0) == false);
}


void VisitableMockObject_template_test::test_value_2()
{
  MOCKPP_NS::ResponseVector2<int, int, int> rv(MOCKPP_PCHAR("rv"), 0);
  rv.add(10, 11, 2, 1);
  rv.add(20, 21, 2, 1);
  rv.add(30, 31, 2, 1);

  int i;
  MOCKPP_ASSERT_TRUE(rv.find(i, 11, 2) == true);
  MOCKPP_ASSERT_TRUE(10 == i);
  MOCKPP_ASSERT_TRUE(rv.find(i, 11, 2) == false);

  MOCKPP_ASSERT_TRUE(rv.find(i, 21, 2) == true);
  MOCKPP_ASSERT_TRUE(20 == i);
  MOCKPP_ASSERT_TRUE(rv.find(i, 21, 2) == false);

  MOCKPP_ASSERT_TRUE(rv.find(i, 31, 2) == true);
  MOCKPP_ASSERT_TRUE(30 == i);
  MOCKPP_ASSERT_TRUE(rv.find(i, 31, 2) == false);

  MOCKPP_ASSERT_TRUE(rv.find(i, 0, 2) == false);

  MOCKPP_NS::Throwable *t10 = MOCKPP_NS::make_throwable((int)11);
  MOCKPP_NS::Throwable *t20 = MOCKPP_NS::make_throwable((int)11);
  MOCKPP_NS::Throwable *t30 = MOCKPP_NS::make_throwable((int)11);

  rv.add(t10, 12, 2, 1);
  rv.add(t20, 22, 2, 1);
  rv.add(t30, 32, 2, 1);

  MOCKPP_NS::Throwable *t;

  MOCKPP_ASSERT_TRUE(rv.find(t, 12, 2) == true);
  MOCKPP_ASSERT_TRUE(t10 == t);
  MOCKPP_ASSERT_TRUE(rv.find(t, 12, 2) == false);

  MOCKPP_ASSERT_TRUE(rv.find(t, 22, 2) == true);
  MOCKPP_ASSERT_TRUE(t20 == t);
  MOCKPP_ASSERT_TRUE(rv.find(t, 22, 2) == false);

  MOCKPP_ASSERT_TRUE(rv.find(t, 32, 2) == true);
  MOCKPP_ASSERT_TRUE(t30 == t);
  MOCKPP_ASSERT_TRUE(rv.find(t, 32, 2) == false);

  MOCKPP_ASSERT_TRUE(rv.find(t, 0, 2) == false);

  MOCKPP_NS::Throwable *t40 = MOCKPP_NS::make_throwable((int)11);
  rv.add(130, 131, 2, 1);
  rv.add(t40, 132, 2, 1);
  MOCKPP_ASSERT_TRUE(rv.find(t, 131, 2) == false);  // across !
  MOCKPP_ASSERT_TRUE(rv.find(i, 132, 2) == false);
}


void VisitableMockObject_template_test::test_throw_2()
{
  MOCKPP_NS::ResponseThrowableVector2<int, int> tv(MOCKPP_PCHAR("rv"), 0);

  MOCKPP_NS::Throwable *t10 = MOCKPP_NS::make_throwable((int)11);
  MOCKPP_NS::Throwable *t20 = MOCKPP_NS::make_throwable((int)11);
  MOCKPP_NS::Throwable *t30 = MOCKPP_NS::make_throwable((int)11);

  tv.add(t10, 11, 2, 1);
  tv.add(t20, 21, 2, 1);
  tv.add(t30, 31, 2, 1);

  MOCKPP_NS::Throwable *t;

  MOCKPP_ASSERT_TRUE(tv.find(t, 11, 2) == true);
  MOCKPP_ASSERT_TRUE(t10 == t);
  MOCKPP_ASSERT_TRUE(tv.find(t, 11, 2) == false);

  MOCKPP_ASSERT_TRUE(tv.find(t, 21, 2) == true);
  MOCKPP_ASSERT_TRUE(t20 == t);
  MOCKPP_ASSERT_TRUE(tv.find(t, 21, 2) == false);

  MOCKPP_ASSERT_TRUE(tv.find(t, 31, 2) == true);
  MOCKPP_ASSERT_TRUE(t30 == t);
  MOCKPP_ASSERT_TRUE(tv.find(t, 31, 2) == false);

  MOCKPP_ASSERT_TRUE(tv.find(t, 0, 2) == false);
}


void VisitableMockObject_template_test::test_value_3()
{
  MOCKPP_NS::ResponseVector3<int, int, int, int> rv(MOCKPP_PCHAR("rv"), 0);
  rv.add(10, 11, 2, 3, 1);
  rv.add(20, 21, 2, 3, 1);
  rv.add(30, 31, 2, 3, 1);

  int i;
  MOCKPP_ASSERT_TRUE(rv.find(i, 11, 2, 3) == true);
  MOCKPP_ASSERT_TRUE(10 == i);
  MOCKPP_ASSERT_TRUE(rv.find(i, 11, 2, 3) == false);

  MOCKPP_ASSERT_TRUE(rv.find(i, 21, 2, 3) == true);
  MOCKPP_ASSERT_TRUE(20 == i);
  MOCKPP_ASSERT_TRUE(rv.find(i, 21, 2, 3) == false);

  MOCKPP_ASSERT_TRUE(rv.find(i, 31, 2, 3) == true);
  MOCKPP_ASSERT_TRUE(30 == i);
  MOCKPP_ASSERT_TRUE(rv.find(i, 31, 2, 3) == false);

  MOCKPP_ASSERT_TRUE(rv.find(i, 0, 2, 3) == false);

  MOCKPP_NS::Throwable *t10 = MOCKPP_NS::make_throwable((int)11);
  MOCKPP_NS::Throwable *t20 = MOCKPP_NS::make_throwable((int)11);
  MOCKPP_NS::Throwable *t30 = MOCKPP_NS::make_throwable((int)11);

  rv.add(t10, 12, 2, 3, 1);
  rv.add(t20, 22, 2, 3, 1);
  rv.add(t30, 32, 2, 3, 1);

  MOCKPP_NS::Throwable *t;

  MOCKPP_ASSERT_TRUE(rv.find(t, 12, 2, 3) == true);
  MOCKPP_ASSERT_TRUE(t10 == t);
  MOCKPP_ASSERT_TRUE(rv.find(t, 12, 2, 3) == false);

  MOCKPP_ASSERT_TRUE(rv.find(t, 22, 2, 3) == true);
  MOCKPP_ASSERT_TRUE(t20 == t);
  MOCKPP_ASSERT_TRUE(rv.find(t, 22, 2, 3) == false);

  MOCKPP_ASSERT_TRUE(rv.find(t, 32, 2, 3) == true);
  MOCKPP_ASSERT_TRUE(t30 == t);
  MOCKPP_ASSERT_TRUE(rv.find(t, 32, 2, 3) == false);

  MOCKPP_ASSERT_TRUE(rv.find(t, 0, 2, 3) == false);

  MOCKPP_NS::Throwable *t40 = MOCKPP_NS::make_throwable((int)11);
  rv.add(130, 131, 2, 3, 1);
  rv.add(t40, 132, 2, 3, 1);
  MOCKPP_ASSERT_TRUE(rv.find(t, 131, 2, 3) == false);  // across !
  MOCKPP_ASSERT_TRUE(rv.find(i, 132, 2, 3) == false);
}


void VisitableMockObject_template_test::test_throw_3()
{
  MOCKPP_NS::ResponseThrowableVector3<int, int, int> tv(MOCKPP_PCHAR("rv"), 0);

  MOCKPP_NS::Throwable *t10 = MOCKPP_NS::make_throwable((int)11);
  MOCKPP_NS::Throwable *t20 = MOCKPP_NS::make_throwable((int)11);
  MOCKPP_NS::Throwable *t30 = MOCKPP_NS::make_throwable((int)11);

  tv.add(t10, 11, 2, 3, 1);
  tv.add(t20, 21, 2, 3, 1);
  tv.add(t30, 31, 2, 3, 1);

  MOCKPP_NS::Throwable *t;

  MOCKPP_ASSERT_TRUE(tv.find(t, 11, 2, 3) == true);
  MOCKPP_ASSERT_TRUE(t10 == t);
  MOCKPP_ASSERT_TRUE(tv.find(t, 11, 2, 3) == false);

  MOCKPP_ASSERT_TRUE(tv.find(t, 21, 2, 3) == true);
  MOCKPP_ASSERT_TRUE(t20 == t);
  MOCKPP_ASSERT_TRUE(tv.find(t, 21, 2, 3) == false);

  MOCKPP_ASSERT_TRUE(tv.find(t, 31, 2, 3) == true);
  MOCKPP_ASSERT_TRUE(t30 == t);
  MOCKPP_ASSERT_TRUE(tv.find(t, 31, 2, 3) == false);

  MOCKPP_ASSERT_TRUE(tv.find(t, 0, 2, 3) == false);
}


void VisitableMockObject_template_test::test_value_4()
{
  MOCKPP_NS::ResponseVector4<int, int, int, int, int> rv(MOCKPP_PCHAR("rv"), 0);
  rv.add(10, 11, 2, 3, 4, 1);
  rv.add(20, 21, 2, 3, 4, 1);
  rv.add(30, 31, 2, 3, 4, 1);

  int i;
  MOCKPP_ASSERT_TRUE(rv.find(i, 11, 2, 3, 4) == true);
  MOCKPP_ASSERT_TRUE(10 == i);
  MOCKPP_ASSERT_TRUE(rv.find(i, 11, 2, 3, 4) == false);

  MOCKPP_ASSERT_TRUE(rv.find(i, 21, 2, 3, 4) == true);
  MOCKPP_ASSERT_TRUE(20 == i);
  MOCKPP_ASSERT_TRUE(rv.find(i, 21, 2, 3, 4) == false);

  MOCKPP_ASSERT_TRUE(rv.find(i, 31, 2, 3, 4) == true);
  MOCKPP_ASSERT_TRUE(30 == i);
  MOCKPP_ASSERT_TRUE(rv.find(i, 31, 2, 3, 4) == false);

  MOCKPP_ASSERT_TRUE(rv.find(i, 0, 2, 3, 4) == false);

  MOCKPP_NS::Throwable *t10 = MOCKPP_NS::make_throwable((int)11);
  MOCKPP_NS::Throwable *t20 = MOCKPP_NS::make_throwable((int)11);
  MOCKPP_NS::Throwable *t30 = MOCKPP_NS::make_throwable((int)11);

  rv.add(t10, 12, 2, 3, 4, 1);
  rv.add(t20, 22, 2, 3, 4, 1);
  rv.add(t30, 32, 2, 3, 4, 1);

  MOCKPP_NS::Throwable *t;

  MOCKPP_ASSERT_TRUE(rv.find(t, 12, 2, 3, 4) == true);
  MOCKPP_ASSERT_TRUE(t10 == t);
  MOCKPP_ASSERT_TRUE(rv.find(t, 12, 2, 3, 4) == false);

  MOCKPP_ASSERT_TRUE(rv.find(t, 22, 2, 3, 4) == true);
  MOCKPP_ASSERT_TRUE(t20 == t);
  MOCKPP_ASSERT_TRUE(rv.find(t, 22, 2, 3, 4) == false);

  MOCKPP_ASSERT_TRUE(rv.find(t, 32, 2, 3, 4) == true);
  MOCKPP_ASSERT_TRUE(t30 == t);
  MOCKPP_ASSERT_TRUE(rv.find(t, 32, 2, 3, 4) == false);

  MOCKPP_ASSERT_TRUE(rv.find(t, 0, 2, 3, 4) == false);

  MOCKPP_NS::Throwable *t40 = MOCKPP_NS::make_throwable((int)11);
  rv.add(130, 131, 2, 3, 4, 1);
  rv.add(t40, 132, 2, 3, 4, 1);
  MOCKPP_ASSERT_TRUE(rv.find(t, 131, 2, 3, 4) == false);  // across !
  MOCKPP_ASSERT_TRUE(rv.find(i, 132, 2, 3, 4) == false);
}


void VisitableMockObject_template_test::test_throw_4()
{
  MOCKPP_NS::ResponseThrowableVector4<int, int, int, int> tv(MOCKPP_PCHAR("rv"), 0);

  MOCKPP_NS::Throwable *t10 = MOCKPP_NS::make_throwable((int)11);
  MOCKPP_NS::Throwable *t20 = MOCKPP_NS::make_throwable((int)11);
  MOCKPP_NS::Throwable *t30 = MOCKPP_NS::make_throwable((int)11);

  tv.add(t10, 11, 2, 3, 4, 1);
  tv.add(t20, 21, 2, 3, 4, 1);
  tv.add(t30, 31, 2, 3, 4, 1);

  MOCKPP_NS::Throwable *t;

  MOCKPP_ASSERT_TRUE(tv.find(t, 11, 2, 3, 4) == true);
  MOCKPP_ASSERT_TRUE(t10 == t);
  MOCKPP_ASSERT_TRUE(tv.find(t, 11, 2, 3, 4) == false);

  MOCKPP_ASSERT_TRUE(tv.find(t, 21, 2, 3, 4) == true);
  MOCKPP_ASSERT_TRUE(t20 == t);
  MOCKPP_ASSERT_TRUE(tv.find(t, 21, 2, 3, 4) == false);

  MOCKPP_ASSERT_TRUE(tv.find(t, 31, 2, 3, 4) == true);
  MOCKPP_ASSERT_TRUE(t30 == t);
  MOCKPP_ASSERT_TRUE(tv.find(t, 31, 2, 3, 4) == false);

  MOCKPP_ASSERT_TRUE(tv.find(t, 0, 2, 3, 4) == false);
}


void VisitableMockObject_template_test::test_value_5()
{
  MOCKPP_NS::ResponseVector5<int, int, int, int, int, int> rv(MOCKPP_PCHAR("rv"), 0);
  rv.add(10, 11, 2, 3, 4, 5, 1);
  rv.add(20, 21, 2, 3, 4, 5, 1);
  rv.add(30, 31, 2, 3, 4, 5, 1);

  int i;
  MOCKPP_ASSERT_TRUE(rv.find(i, 11, 2, 3, 4, 5) == true);
  MOCKPP_ASSERT_TRUE(10 == i);
  MOCKPP_ASSERT_TRUE(rv.find(i, 11, 2, 3, 4, 5) == false);

  MOCKPP_ASSERT_TRUE(rv.find(i, 21, 2, 3, 4, 5) == true);
  MOCKPP_ASSERT_TRUE(20 == i);
  MOCKPP_ASSERT_TRUE(rv.find(i, 21, 2, 3, 4, 5) == false);

  MOCKPP_ASSERT_TRUE(rv.find(i, 31, 2, 3, 4, 5) == true);
  MOCKPP_ASSERT_TRUE(30 == i);
  MOCKPP_ASSERT_TRUE(rv.find(i, 31, 2, 3, 4, 5) == false);

  MOCKPP_ASSERT_TRUE(rv.find(i, 0, 2, 3, 4, 5) == false);

  MOCKPP_NS::Throwable *t10 = MOCKPP_NS::make_throwable((int)11);
  MOCKPP_NS::Throwable *t20 = MOCKPP_NS::make_throwable((int)11);
  MOCKPP_NS::Throwable *t30 = MOCKPP_NS::make_throwable((int)11);

  rv.add(t10, 12, 2, 3, 4, 5, 1);
  rv.add(t20, 22, 2, 3, 4, 5, 1);
  rv.add(t30, 32, 2, 3, 4, 5, 1);

  MOCKPP_NS::Throwable *t;

  MOCKPP_ASSERT_TRUE(rv.find(t, 12, 2, 3, 4, 5) == true);
  MOCKPP_ASSERT_TRUE(t10 == t);
  MOCKPP_ASSERT_TRUE(rv.find(t, 12, 2, 3, 4, 5) == false);

  MOCKPP_ASSERT_TRUE(rv.find(t, 22, 2, 3, 4, 5) == true);
  MOCKPP_ASSERT_TRUE(t20 == t);
  MOCKPP_ASSERT_TRUE(rv.find(t, 22, 2, 3, 4, 5) == false);

  MOCKPP_ASSERT_TRUE(rv.find(t, 32, 2, 3, 4, 5) == true);
  MOCKPP_ASSERT_TRUE(t30 == t);
  MOCKPP_ASSERT_TRUE(rv.find(t, 32, 2, 3, 4, 5) == false);

  MOCKPP_ASSERT_TRUE(rv.find(t, 0, 2, 3, 4, 5) == false);

  MOCKPP_NS::Throwable *t40 = MOCKPP_NS::make_throwable((int)11);
  rv.add(130, 131, 2, 3, 4, 5, 1);
  rv.add(t40, 132, 2, 3, 4, 5, 1);
  MOCKPP_ASSERT_TRUE(rv.find(t, 131, 2, 3, 4, 5) == false);  // across !
  MOCKPP_ASSERT_TRUE(rv.find(i, 132, 2, 3, 4, 5) == false);
}


void VisitableMockObject_template_test::test_throw_5()
{
  MOCKPP_NS::ResponseThrowableVector5<int, int, int, int, int> tv(MOCKPP_PCHAR("rv"), 0);

  MOCKPP_NS::Throwable *t10 = MOCKPP_NS::make_throwable((int)11);
  MOCKPP_NS::Throwable *t20 = MOCKPP_NS::make_throwable((int)11);
  MOCKPP_NS::Throwable *t30 = MOCKPP_NS::make_throwable((int)11);

  tv.add(t10, 11, 2, 3, 4, 5, 1);
  tv.add(t20, 21, 2, 3, 4, 5, 1);
  tv.add(t30, 31, 2, 3, 4, 5, 1);

  MOCKPP_NS::Throwable *t;

  MOCKPP_ASSERT_TRUE(tv.find(t, 11, 2, 3, 4, 5) == true);
  MOCKPP_ASSERT_TRUE(t10 == t);
  MOCKPP_ASSERT_TRUE(tv.find(t, 11, 2, 3, 4, 5) == false);

  MOCKPP_ASSERT_TRUE(tv.find(t, 21, 2, 3, 4, 5) == true);
  MOCKPP_ASSERT_TRUE(t20 == t);
  MOCKPP_ASSERT_TRUE(tv.find(t, 21, 2, 3, 4, 5) == false);

  MOCKPP_ASSERT_TRUE(tv.find(t, 31, 2, 3, 4, 5) == true);
  MOCKPP_ASSERT_TRUE(t30 == t);
  MOCKPP_ASSERT_TRUE(tv.find(t, 31, 2, 3, 4, 5) == false);

  MOCKPP_ASSERT_TRUE(tv.find(t, 0, 2, 3, 4, 5) == false);
}



