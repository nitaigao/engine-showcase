/***************************************************************************
       ConstraintSet_test.cpp  -  unit tests for ConstraintSet class
                             -------------------
    begin                : Sat Aug 27 2004
    copyright            : (C) 2002-2008 by Ewald Arnold
    email                : mockpp at ewald-arnold dot de

  $Id: ConstraintSet_test.cpp 1438 2008-03-08 20:22:43Z ewald-arnold $

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

#include <mockpp/chaining/Invocation.h>
#include <mockpp/constraint/ConstraintSet.h>
#include <mockpp/constraint/IsEqual.h>

#include <mockpp/compat/Asserter.h>


class ConstraintSet_test MOCKPP_DERIVE_PUBLIC_UNITFRAMEWORK
{
 public:

#if defined (MOCKPP_USE_CXXTEST)

#elif defined(MOCKPP_USE_BOOSTTEST)

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE( ConstraintSet_test );

  CPPUNIT_TEST(test_matches0);
  CPPUNIT_TEST(test_matches1);
  CPPUNIT_TEST(test_matches2);
  CPPUNIT_TEST(test_matches3);
  CPPUNIT_TEST(test_matches4);
  CPPUNIT_TEST(test_matches5);
  CPPUNIT_TEST(test_describe0);
  CPPUNIT_TEST(test_describe1);
  CPPUNIT_TEST(test_describe2);
  CPPUNIT_TEST(test_describe3);
  CPPUNIT_TEST(test_describe4);
  CPPUNIT_TEST(test_describe5);

CPPUNIT_TEST_SUITE_END();

#endif

 public:

  MOCKPP_TESTMETHOD_DECL void test_matches0();
  MOCKPP_TESTMETHOD_DECL void test_matches1();
  MOCKPP_TESTMETHOD_DECL void test_matches2();
  MOCKPP_TESTMETHOD_DECL void test_matches3();
  MOCKPP_TESTMETHOD_DECL void test_matches4();
  MOCKPP_TESTMETHOD_DECL void test_matches5();
  MOCKPP_TESTMETHOD_DECL void test_describe0();
  MOCKPP_TESTMETHOD_DECL void test_describe1();
  MOCKPP_TESTMETHOD_DECL void test_describe2();
  MOCKPP_TESTMETHOD_DECL void test_describe3();
  MOCKPP_TESTMETHOD_DECL void test_describe4();
  MOCKPP_TESTMETHOD_DECL void test_describe5();
};


#if defined (MOCKPP_USE_CXXTEST)

MOCKPP_CXXTEST_SUITE_REGISTRATION( ConstraintSet_test );
MOCKPP_CXXTEST(ConstraintSet_test, test_matches0);
MOCKPP_CXXTEST(ConstraintSet_test, test_matches1);
MOCKPP_CXXTEST(ConstraintSet_test, test_matches2);
MOCKPP_CXXTEST(ConstraintSet_test, test_matches3);
MOCKPP_CXXTEST(ConstraintSet_test, test_matches4);
MOCKPP_CXXTEST(ConstraintSet_test, test_matches5);
MOCKPP_CXXTEST(ConstraintSet_test, test_describe0);
MOCKPP_CXXTEST(ConstraintSet_test, test_describe1);
MOCKPP_CXXTEST(ConstraintSet_test, test_describe2);
MOCKPP_CXXTEST(ConstraintSet_test, test_describe3);
MOCKPP_CXXTEST(ConstraintSet_test, test_describe4);
MOCKPP_CXXTEST(ConstraintSet_test, test_describe5);

#elif defined(MOCKPP_USE_BOOSTTEST)

MOCKPP_BOOST_TEST(ConstraintSet_test::test_matches0);
MOCKPP_BOOST_TEST(ConstraintSet_test::test_matches1);
MOCKPP_BOOST_TEST(ConstraintSet_test::test_matches2);
MOCKPP_BOOST_TEST(ConstraintSet_test::test_matches3);
MOCKPP_BOOST_TEST(ConstraintSet_test::test_matches4);
MOCKPP_BOOST_TEST(ConstraintSet_test::test_matches5);
MOCKPP_BOOST_TEST(ConstraintSet_test::test_describe0);
MOCKPP_BOOST_TEST(ConstraintSet_test::test_describe1);
MOCKPP_BOOST_TEST(ConstraintSet_test::test_describe2);
MOCKPP_BOOST_TEST(ConstraintSet_test::test_describe3);
MOCKPP_BOOST_TEST(ConstraintSet_test::test_describe4);
MOCKPP_BOOST_TEST(ConstraintSet_test::test_describe5);

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE_REGISTRATION (ConstraintSet_test);

#endif // frameworks



class MyConstraintSetDummy
{
  public:

   MOCKPP_TESTMETHOD_DECL void set_0_Prop() {}
   MOCKPP_TESTMETHOD_DECL void set_1_Prop(int) {}
   MOCKPP_TESTMETHOD_DECL void set_2_Prop(int, int) {}
   MOCKPP_TESTMETHOD_DECL void set_3_Prop(int, int, int) {}
   MOCKPP_TESTMETHOD_DECL void set_4_Prop(int, int, int, int) {}
   MOCKPP_TESTMETHOD_DECL void set_5_Prop(int, int, int, int, int) {}

    typedef void (MyConstraintSetDummy::*Setter_0_PMF) ();
    typedef void (MyConstraintSetDummy::*Setter_1_PMF) (int);
    typedef void (MyConstraintSetDummy::*Setter_2_PMF) (int, int);
    typedef void (MyConstraintSetDummy::*Setter_3_PMF) (int, int, int);
    typedef void (MyConstraintSetDummy::*Setter_4_PMF) (int, int, int, int);
    typedef void (MyConstraintSetDummy::*Setter_5_PMF) (int, int, int, int, int);
};


void ConstraintSet_test::test_matches0()
{
  MOCKPP_NS::ConstraintSet0 cs;

  typedef MOCKPP_NS::Invocation0 SetterInvocation;
  SetterInvocation sinv (MOCKPP_PCHAR("MyConstraintSetDummy::set_0_Prop"));
  SetterInvocation sinva(MOCKPP_PCHAR("MyConstraintSetDummy::set_0_Prop"));

  MOCKPP_ASSERT_TRUE(cs.matches(sinv) == true);
  MOCKPP_ASSERT_TRUE(cs.matches(sinva) == true);
}


void ConstraintSet_test::test_matches1()
{
  MOCKPP_NS::ConstraintSet1<int> cs (new MOCKPP_NS::IsEqual<int>(1));

  typedef MOCKPP_NS::Invocation1<int> SetterInvocation;
  int i0 = 0;
  int i1 = 1;
  SetterInvocation sinv (MOCKPP_PCHAR("MyConstraintSetDummy::set_1_Prop"), i1);
  SetterInvocation sinva(MOCKPP_PCHAR("MyConstraintSetDummy::set_1_Prop"), i0);

  MOCKPP_ASSERT_TRUE(cs.matches(sinv) == true);
  MOCKPP_ASSERT_TRUE(cs.matches(sinva) == false);
}


void ConstraintSet_test::test_matches2()
{
  MOCKPP_NS::ConstraintSet2<int, int> cs (new MOCKPP_NS::IsEqual<int>(1),
                                       new MOCKPP_NS::IsEqual<int>(2));

  typedef MOCKPP_NS::Invocation2<int, int> SetterInvocation;
  int i0 = 0;
  int i1 = 1;
  int i2 = 2;
  SetterInvocation sinv (MOCKPP_PCHAR("MyConstraintSetDummy::set_2_Prop"), i1, i2);
  SetterInvocation sinva(MOCKPP_PCHAR("MyConstraintSetDummy::set_2_Prop"), i0, i2);
  SetterInvocation sinvb(MOCKPP_PCHAR("MyConstraintSetDummy::set_2_Prop"), i1, i0);

  MOCKPP_ASSERT_TRUE(cs.matches(sinv) == true);
  MOCKPP_ASSERT_TRUE(cs.matches(sinva) == false);
  MOCKPP_ASSERT_TRUE(cs.matches(sinvb) == false);
}


void ConstraintSet_test::test_matches3()
{
  MOCKPP_NS::ConstraintSet3<int, int, int>
     cs (new MOCKPP_NS::IsEqual<int>(1),
         new MOCKPP_NS::IsEqual<int>(2),
         new MOCKPP_NS::IsEqual<int>(3));

  typedef MOCKPP_NS::Invocation3<int, int, int> SetterInvocation;
  int i0 = 0;
  int i1 = 1;
  int i2 = 2;
  int i3 = 3;
  SetterInvocation sinv (MOCKPP_PCHAR("MyConstraintSetDummy::set_3_Prop"), i1, i2, i3);
  SetterInvocation sinva(MOCKPP_PCHAR("MyConstraintSetDummy::set_3_Prop"), i0, i2, i3);
  SetterInvocation sinvb(MOCKPP_PCHAR("MyConstraintSetDummy::set_3_Prop"), i1, i0, i3);
  SetterInvocation sinvc(MOCKPP_PCHAR("MyConstraintSetDummy::set_3_Prop"), i1, i2, i0);

  MOCKPP_ASSERT_TRUE(cs.matches(sinv) == true);
  MOCKPP_ASSERT_TRUE(cs.matches(sinva) == false);
  MOCKPP_ASSERT_TRUE(cs.matches(sinvb) == false);
  MOCKPP_ASSERT_TRUE(cs.matches(sinvc) == false);
}


void ConstraintSet_test::test_matches4()
{
  MOCKPP_NS::ConstraintSet4<int, int, int, int>
      cs (new MOCKPP_NS::IsEqual<int>(1),
          new MOCKPP_NS::IsEqual<int>(2),
          new MOCKPP_NS::IsEqual<int>(3),
          new MOCKPP_NS::IsEqual<int>(4));

  typedef MOCKPP_NS::Invocation4<int, int, int, int> SetterInvocation;
  int i0 = 0;
  int i1 = 1;
  int i2 = 2;
  int i3 = 3;
  int i4 = 4;
  SetterInvocation sinv (MOCKPP_PCHAR("MyConstraintSetDummy::set_4_Prop"), i1, i2, i3, i4);
  SetterInvocation sinva(MOCKPP_PCHAR("MyConstraintSetDummy::set_4_Prop"), i0, i2, i3, i4);
  SetterInvocation sinvb(MOCKPP_PCHAR("MyConstraintSetDummy::set_4_Prop"), i1, i0, i3, i4);
  SetterInvocation sinvc(MOCKPP_PCHAR("MyConstraintSetDummy::set_4_Prop"), i1, i2, i0, i4);
  SetterInvocation sinvd(MOCKPP_PCHAR("MyConstraintSetDummy::set_4_Prop"), i1, i2, i3, i0);

  MOCKPP_ASSERT_TRUE(cs.matches(sinv) == true);
  MOCKPP_ASSERT_TRUE(cs.matches(sinva) == false);
  MOCKPP_ASSERT_TRUE(cs.matches(sinvb) == false);
  MOCKPP_ASSERT_TRUE(cs.matches(sinvc) == false);
  MOCKPP_ASSERT_TRUE(cs.matches(sinvd) == false);
}


void ConstraintSet_test::test_matches5()
{
  MOCKPP_NS::ConstraintSet5<int, int, int, int, int>
      cs (new MOCKPP_NS::IsEqual<int>(1),
          new MOCKPP_NS::IsEqual<int>(2),
          new MOCKPP_NS::IsEqual<int>(3),
          new MOCKPP_NS::IsEqual<int>(4),
          new MOCKPP_NS::IsEqual<int>(5));

  typedef MOCKPP_NS::Invocation5<int, int, int, int, int> SetterInvocation;
  int i0 = 0;
  int i1 = 1;
  int i2 = 2;
  int i3 = 3;
  int i4 = 4;
  int i5 = 5;
  SetterInvocation sinv (MOCKPP_PCHAR("MyConstraintSetDummy::set_5_Prop"), i1, i2, i3, i4, i5);
  SetterInvocation sinva(MOCKPP_PCHAR("MyConstraintSetDummy::set_5_Prop"), i0, i2, i3, i4, i5);
  SetterInvocation sinvb(MOCKPP_PCHAR("MyConstraintSetDummy::set_5_Prop"), i1, i0, i3, i4, i5);
  SetterInvocation sinvc(MOCKPP_PCHAR("MyConstraintSetDummy::set_5_Prop"), i1, i2, i0, i4, i5);
  SetterInvocation sinvd(MOCKPP_PCHAR("MyConstraintSetDummy::set_5_Prop"), i1, i2, i3, i0, i5);
  SetterInvocation sinve(MOCKPP_PCHAR("MyConstraintSetDummy::set_5_Prop"), i1, i2, i3, i4, i0);

  MOCKPP_ASSERT_TRUE(cs.matches(sinv) == true);
  MOCKPP_ASSERT_TRUE(cs.matches(sinva) == false);
  MOCKPP_ASSERT_TRUE(cs.matches(sinvb) == false);
  MOCKPP_ASSERT_TRUE(cs.matches(sinvc) == false);
  MOCKPP_ASSERT_TRUE(cs.matches(sinvd) == false);
  MOCKPP_ASSERT_TRUE(cs.matches(sinve) == false);
}


void ConstraintSet_test::test_describe0()
{
  MOCKPP_NS::ConstraintSet0 cs;

  MOCKPP_NS::String s = MOCKPP_PCHAR("cs?");
  MOCKPP_ASSERT_TRUE(cs.describeTo(s) == MOCKPP_PCHAR("cs?"));
  MOCKPP_ASSERT_TRUE(s == MOCKPP_PCHAR("cs?"));
}


void ConstraintSet_test::test_describe1()
{
  MOCKPP_NS::ConstraintSet1<int>
      cs (new MOCKPP_NS::IsEqual<int>(1));

  MOCKPP_NS::String s = MOCKPP_PCHAR("cs?");
  MOCKPP_ASSERT_TRUE(cs.describeTo(s) == MOCKPP_PCHAR("cs?equalTo 1"));
  MOCKPP_ASSERT_TRUE(s == MOCKPP_PCHAR("cs?equalTo 1"));
}


void ConstraintSet_test::test_describe2()
{
  MOCKPP_NS::ConstraintSet2<int, int>
      cs (new MOCKPP_NS::IsEqual<int>(1),
          new MOCKPP_NS::IsEqual<int>(2));

  MOCKPP_NS::String s = MOCKPP_PCHAR("cs?");
  MOCKPP_ASSERT_TRUE(cs.describeTo(s) == MOCKPP_PCHAR("cs?equalTo 1, equalTo 2"));
  MOCKPP_ASSERT_TRUE(s == MOCKPP_PCHAR("cs?equalTo 1, equalTo 2"));
}


void ConstraintSet_test::test_describe3()
{
  MOCKPP_NS::ConstraintSet3<int, int, int>
      cs (new MOCKPP_NS::IsEqual<int>(1),
          new MOCKPP_NS::IsEqual<int>(2),
          new MOCKPP_NS::IsEqual<int>(3));

  MOCKPP_NS::String s = MOCKPP_PCHAR("cs?");
  MOCKPP_ASSERT_TRUE(cs.describeTo(s) == MOCKPP_PCHAR("cs?equalTo 1, equalTo 2, equalTo 3"));
  MOCKPP_ASSERT_TRUE(s == MOCKPP_PCHAR("cs?equalTo 1, equalTo 2, equalTo 3"));
}


void ConstraintSet_test::test_describe4()
{
  MOCKPP_NS::ConstraintSet4<int, int, int, int>
      cs (new MOCKPP_NS::IsEqual<int>(1),
          new MOCKPP_NS::IsEqual<int>(2),
          new MOCKPP_NS::IsEqual<int>(3),
          new MOCKPP_NS::IsEqual<int>(4));

  MOCKPP_NS::String s = MOCKPP_PCHAR("cs?");
  MOCKPP_ASSERT_TRUE(cs.describeTo(s) == MOCKPP_PCHAR("cs?equalTo 1, equalTo 2, equalTo 3, equalTo 4"));
  MOCKPP_ASSERT_TRUE(s == MOCKPP_PCHAR("cs?equalTo 1, equalTo 2, equalTo 3, equalTo 4"));
}


void ConstraintSet_test::test_describe5()
{
  MOCKPP_NS::ConstraintSet5<int, int, int, int, int>
      cs (new MOCKPP_NS::IsEqual<int>(1),
          new MOCKPP_NS::IsEqual<int>(2),
          new MOCKPP_NS::IsEqual<int>(3),
          new MOCKPP_NS::IsEqual<int>(4),
          new MOCKPP_NS::IsEqual<int>(5));

  MOCKPP_NS::String s = MOCKPP_PCHAR("cs?");
  MOCKPP_ASSERT_TRUE(cs.describeTo(s) == MOCKPP_PCHAR("cs?equalTo 1, equalTo 2, equalTo 3, equalTo 4, equalTo 5"));
  MOCKPP_ASSERT_TRUE(s == MOCKPP_PCHAR("cs?equalTo 1, equalTo 2, equalTo 3, equalTo 4, equalTo 5"));
}



