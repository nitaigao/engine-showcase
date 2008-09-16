/***************************************************************************
           Invocation_test.cpp  -  unit tests for Invocation class
                             -------------------
    begin                : Sat 27 Aug 2004
    copyright            : (C) 2002-2008 by Ewald Arnold
    email                : mockpp at ewald-arnold dot de

  $Id: Invocation_test.cpp 1438 2008-03-08 20:22:43Z ewald-arnold $

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

#define MOCKPP_USE_INVOCATION_EQUALS

#include <mockpp/mockpp.h> // always first

#include "SelectFramework.h"

#include <mockpp/chaining/Invocation.h>

#include <mockpp/compat/Asserter.h>


class Invocation_test MOCKPP_DERIVE_PUBLIC_UNITFRAMEWORK
{
 public:

#if defined (MOCKPP_USE_CXXTEST)

#elif defined(MOCKPP_USE_BOOSTTEST)

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE( Invocation_test );

  CPPUNIT_TEST(test_equals0);
  CPPUNIT_TEST(test_equals1);
  CPPUNIT_TEST(test_equals2);
  CPPUNIT_TEST(test_equals3);
  CPPUNIT_TEST(test_equals4);
  CPPUNIT_TEST(test_equals5);
  CPPUNIT_TEST(test_get1);
  CPPUNIT_TEST(test_get2);
  CPPUNIT_TEST(test_get3);
  CPPUNIT_TEST(test_get4);
  CPPUNIT_TEST(test_get5);
  CPPUNIT_TEST(test_invoked);

CPPUNIT_TEST_SUITE_END();

#endif

 public:

  MOCKPP_TESTMETHOD_DECL void test_equals0();
  MOCKPP_TESTMETHOD_DECL void test_equals1();
  MOCKPP_TESTMETHOD_DECL void test_equals2();
  MOCKPP_TESTMETHOD_DECL void test_equals3();
  MOCKPP_TESTMETHOD_DECL void test_equals4();
  MOCKPP_TESTMETHOD_DECL void test_equals5();
  MOCKPP_TESTMETHOD_DECL void test_get1();
  MOCKPP_TESTMETHOD_DECL void test_get2();
  MOCKPP_TESTMETHOD_DECL void test_get3();
  MOCKPP_TESTMETHOD_DECL void test_get4();
  MOCKPP_TESTMETHOD_DECL void test_get5();
  MOCKPP_TESTMETHOD_DECL void test_invoked();
};


#if defined (MOCKPP_USE_CXXTEST)

MOCKPP_CXXTEST_SUITE_REGISTRATION( Invocation_test );

MOCKPP_CXXTEST(Invocation_test, test_equals0);
MOCKPP_CXXTEST(Invocation_test, test_equals1);
MOCKPP_CXXTEST(Invocation_test, test_equals2);
MOCKPP_CXXTEST(Invocation_test, test_equals3);
MOCKPP_CXXTEST(Invocation_test, test_equals4);
MOCKPP_CXXTEST(Invocation_test, test_equals5);
MOCKPP_CXXTEST(Invocation_test, test_get1);
MOCKPP_CXXTEST(Invocation_test, test_get2);
MOCKPP_CXXTEST(Invocation_test, test_get3);
MOCKPP_CXXTEST(Invocation_test, test_get4);
MOCKPP_CXXTEST(Invocation_test, test_get5);
MOCKPP_CXXTEST(Invocation_test, test_invoked);

#elif defined(MOCKPP_USE_BOOSTTEST)

MOCKPP_BOOST_TEST(Invocation_test::test_equals0);
MOCKPP_BOOST_TEST(Invocation_test::test_equals1);
MOCKPP_BOOST_TEST(Invocation_test::test_equals2);
MOCKPP_BOOST_TEST(Invocation_test::test_equals3);
MOCKPP_BOOST_TEST(Invocation_test::test_equals4);
MOCKPP_BOOST_TEST(Invocation_test::test_equals5);
MOCKPP_BOOST_TEST(Invocation_test::test_get1);
MOCKPP_BOOST_TEST(Invocation_test::test_get2);
MOCKPP_BOOST_TEST(Invocation_test::test_get3);
MOCKPP_BOOST_TEST(Invocation_test::test_get4);
MOCKPP_BOOST_TEST(Invocation_test::test_get5);
MOCKPP_BOOST_TEST(Invocation_test::test_invoked);

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE_REGISTRATION (Invocation_test);

#endif // frameworks



typedef MOCKPP_NS::Invocation0 Setter0Invocation;
typedef MOCKPP_NS::Invocation1<int> Setter1Invocation;
typedef MOCKPP_NS::Invocation2<int, MOCKPP_STL::string> Setter2Invocation;
typedef MOCKPP_NS::Invocation3<int, MOCKPP_STL::string, MOCKPP_STL::string> Setter3Invocation;
typedef MOCKPP_NS::Invocation4<int, MOCKPP_STL::string, MOCKPP_STL::string, int> Setter4Invocation;
typedef MOCKPP_NS::Invocation5<int, MOCKPP_STL::string, MOCKPP_STL::string, int, MOCKPP_STL::string> Setter5Invocation;


void Invocation_test::test_invoked()
{
  typedef MOCKPP_NS::Invocation0 GetterInvocation;

  GetterInvocation ginv(MOCKPP_PCHAR("MyInvocationDummy::getProp"));
  MOCKPP_ASSERT_TRUE(ginv.getMethodName() == MOCKPP_PCHAR("MyInvocationDummy::getProp"));
}


void Invocation_test::test_equals0()
{
  typedef MOCKPP_NS::Invocation0 GetterInvocation;
  typedef MOCKPP_NS::Invocation0 SetterInvocation;

  GetterInvocation ginv(MOCKPP_PCHAR("MyInvocationDummy::getProp"));
  SetterInvocation sinv(MOCKPP_PCHAR("MyInvocationDummy::set_0_Prop"));

  MOCKPP_ASSERT_TRUE(sinv.toString() == MOCKPP_PCHAR("MyInvocationDummy::set_0_Prop()"));
  MOCKPP_ASSERT_TRUE(ginv.toString() == MOCKPP_PCHAR("MyInvocationDummy::getProp()"));

  SetterInvocation sinva0(MOCKPP_PCHAR("MyInvocationDummy::set_0_Prop"));

  int i1 = 1;
  MOCKPP_STL::string abc = "abc";

  MOCKPP_NS::String s1 = MOCKPP_PCHAR("MyInvocationDummy::set_1_Prop");
  Setter1Invocation sinva1(s1, i1);

  MOCKPP_NS::String s2 = MOCKPP_PCHAR("MyInvocationDummy::set_2_Prop");
  Setter2Invocation sinva2(s2, i1, abc);

  MOCKPP_NS::String s3 = MOCKPP_PCHAR("MyInvocationDummy::set_3_Prop");
  MOCKPP_STL::string xyz = "xyz";
  Setter3Invocation sinva3(s3, i1, abc, xyz);

  MOCKPP_NS::String s4 = MOCKPP_PCHAR("MyInvocationDummy::set_4_Prop");
  int i2 = 2;
  Setter4Invocation sinva4(s4, i1, abc, xyz, i2);

  MOCKPP_NS::String s5 = MOCKPP_PCHAR("MyInvocationDummy::set_5_Prop");
  MOCKPP_STL::string s__ = "==";
  Setter5Invocation sinva5(s5, i1, abc, xyz, i2, s__);

#ifdef MOCKPP_USE_INVOCATION_EQUALS
  MOCKPP_ASSERT_TRUE(sinv.equals(sinva0) == true);
  MOCKPP_ASSERT_TRUE(sinv.equals(sinva1) == false);
  MOCKPP_ASSERT_TRUE(sinv.equals(sinva2) == false);
  MOCKPP_ASSERT_TRUE(sinv.equals(sinva3) == false);
  MOCKPP_ASSERT_TRUE(sinv.equals(sinva4) == false);
  MOCKPP_ASSERT_TRUE(sinv.equals(sinva5) == false);
#endif // MOCKPP_USE_INVOCATION_EQUALS
}


void Invocation_test::test_equals1()
{
  typedef MOCKPP_NS::Invocation1<int> SetterInvocation;

  int i1 = 1;
  SetterInvocation sinv(MOCKPP_PCHAR("MyInvocationDummy::set_1_Prop"), i1);
  MOCKPP_ASSERT_TRUE(sinv.toString() == MOCKPP_PCHAR("MyInvocationDummy::set_1_Prop(1)"));

  SetterInvocation sinv1(MOCKPP_PCHAR("MyInvocationDummy::set_1_Prop"), i1);
#ifdef MOCKPP_USE_INVOCATION_EQUALS
  MOCKPP_ASSERT_TRUE(sinv.equals(sinv1) == true);
#endif // MOCKPP_USE_INVOCATION_EQUALS

  int i0 = 0;
  SetterInvocation sinv2(MOCKPP_PCHAR("(MyInvocationDummy::set_1_Prop"), i0);
#ifdef MOCKPP_USE_INVOCATION_EQUALS
  MOCKPP_ASSERT_TRUE(sinv.equals(sinv2) == false);
#endif // MOCKPP_USE_INVOCATION_EQUALS

  Setter0Invocation sinva0(MOCKPP_PCHAR("MyInvocationDummy::set_0_Prop"));
  SetterInvocation sinva1(MOCKPP_PCHAR("MyInvocationDummy::set_1_Prop"), i1);
  MOCKPP_STL::string abc = "abc";
  MOCKPP_STL::string xyz = "xyz";
  int i2 = 2;
  MOCKPP_STL::string s__ = "==";
  Setter2Invocation sinva2(MOCKPP_PCHAR("MyInvocationDummy::set_2_Prop"), i1, abc);
  Setter3Invocation sinva3(MOCKPP_PCHAR("MyInvocationDummy::set_3_Prop"), i1, abc, xyz);
  Setter4Invocation sinva4(MOCKPP_PCHAR("MyInvocationDummy::set_4_Prop"), i1, abc, xyz, i2);
  Setter5Invocation sinva5(MOCKPP_PCHAR("MyInvocationDummy::set_5_Prop"), i1, abc, xyz, i2, s__);
#ifdef MOCKPP_USE_INVOCATION_EQUALS
  MOCKPP_ASSERT_TRUE(sinv.equals(sinva0) == false);
  MOCKPP_ASSERT_TRUE(sinv.equals(sinva1) == true);
  MOCKPP_ASSERT_TRUE(sinv.equals(sinva2) == false);
  MOCKPP_ASSERT_TRUE(sinv.equals(sinva3) == false);
  MOCKPP_ASSERT_TRUE(sinv.equals(sinva4) == false);
  MOCKPP_ASSERT_TRUE(sinv.equals(sinva5) == false);
#endif // MOCKPP_USE_INVOCATION_EQUALS
}


void Invocation_test::test_equals2()
{
  typedef MOCKPP_NS::Invocation2<int, MOCKPP_STL::string> SetterInvocation;

  int i1 = 1;
  int i2 = 2;
  int i6 = 6;
  MOCKPP_STL::string abc = "abc";
  MOCKPP_STL::string abC = "abC";
  MOCKPP_STL::string xyz = "xyz";
  MOCKPP_STL::string xyZ = "xyZ";
  MOCKPP_STL::string s__ = "==";

  SetterInvocation sinv(MOCKPP_PCHAR("MyInvocationDummy::set_2_Prop"), i1, abc);
  MOCKPP_ASSERT_TRUE(sinv.toString() == MOCKPP_PCHAR("MyInvocationDummy::set_2_Prop(1, abc)"));

  SetterInvocation sinv1(MOCKPP_PCHAR("MyInvocationDummy::set_2_Prop"), i1, abc);
#ifdef MOCKPP_USE_INVOCATION_EQUALS
  MOCKPP_ASSERT_TRUE(sinv.equals(sinv1) == true);
#endif // MOCKPP_USE_INVOCATION_EQUALS

  SetterInvocation sinv2(MOCKPP_PCHAR("MyInvocationDummy::set_2_Prop"), i1, abC);
#ifdef MOCKPP_USE_INVOCATION_EQUALS
  MOCKPP_ASSERT_TRUE(sinv.equals(sinv2) == false);
#endif // MOCKPP_USE_INVOCATION_EQUALS

  SetterInvocation sinv3(MOCKPP_PCHAR("MyInvocationDummy::set_2_Prop"), i6, abc);
#ifdef MOCKPP_USE_INVOCATION_EQUALS
  MOCKPP_ASSERT_TRUE(sinv.equals(sinv3) == false);
#endif // MOCKPP_USE_INVOCATION_EQUALS

  Setter0Invocation sinva0(MOCKPP_PCHAR("MyInvocationDummy::set_0_Prop"));
  Setter1Invocation sinva1(MOCKPP_PCHAR("MyInvocationDummy::set_1_Prop"), i1);
  SetterInvocation sinva2(MOCKPP_PCHAR("MyInvocationDummy::set_2_Prop"), i1, abc);
  Setter3Invocation sinva3(MOCKPP_PCHAR("MyInvocationDummy::set_3_Prop"), i1, abc, xyz);
  Setter4Invocation sinva4(MOCKPP_PCHAR("MyInvocationDummy::set_4_Prop"), i1, abc, xyz, i2);
  Setter5Invocation sinva5(MOCKPP_PCHAR("MyInvocationDummy::set_5_Prop"), i1, abc, xyz, i2, s__);
#ifdef MOCKPP_USE_INVOCATION_EQUALS
  MOCKPP_ASSERT_TRUE(sinv.equals(sinva0) == false);
  MOCKPP_ASSERT_TRUE(sinv.equals(sinva1) == false);
  MOCKPP_ASSERT_TRUE(sinv.equals(sinva2) == true);
  MOCKPP_ASSERT_TRUE(sinv.equals(sinva3) == false);
  MOCKPP_ASSERT_TRUE(sinv.equals(sinva4) == false);
  MOCKPP_ASSERT_TRUE(sinv.equals(sinva5) == false);
#endif // MOCKPP_USE_INVOCATION_EQUALS
}


void Invocation_test::test_equals3()
{
  typedef MOCKPP_NS::Invocation3<int, MOCKPP_STL::string, MOCKPP_STL::string > SetterInvocation;
  int i1 = 1;
  int i2 = 2;
  int i5 = 5;
  MOCKPP_STL::string abc = "abc";
  MOCKPP_STL::string abC = "abC";
  MOCKPP_STL::string xyz = "xyz";
  MOCKPP_STL::string xyZ = "xyZ";
  MOCKPP_STL::string s__ = "==";

  SetterInvocation sinv(MOCKPP_PCHAR("MyInvocationDummy::set_3_Prop"), i1, abc, xyz);
  MOCKPP_ASSERT_TRUE(sinv.toString() == MOCKPP_PCHAR("MyInvocationDummy::set_3_Prop(1, abc, xyz)"));

  SetterInvocation sinv1(MOCKPP_PCHAR("MyInvocationDummy::set_3_Prop"), i1, abc, xyz);
#ifdef MOCKPP_USE_INVOCATION_EQUALS
  MOCKPP_ASSERT_TRUE(sinv.equals(sinv1) == true);
#endif // MOCKPP_USE_INVOCATION_EQUALS

  SetterInvocation sinv2(MOCKPP_PCHAR("MyInvocationDummy::set_3_Prop"), i1, abc, xyZ);
#ifdef MOCKPP_USE_INVOCATION_EQUALS
  MOCKPP_ASSERT_TRUE(sinv.equals(sinv2) == false);
#endif // MOCKPP_USE_INVOCATION_EQUALS

  SetterInvocation sinv3(MOCKPP_PCHAR("MyInvocationDummy::set_3_Prop"), i1, abC, xyz);
#ifdef MOCKPP_USE_INVOCATION_EQUALS
  MOCKPP_ASSERT_TRUE(sinv.equals(sinv3) == false);
#endif // MOCKPP_USE_INVOCATION_EQUALS

  SetterInvocation sinv4(MOCKPP_PCHAR("MyInvocationDummy::set_3_Prop"), i5, abc, xyz);
#ifdef MOCKPP_USE_INVOCATION_EQUALS
  MOCKPP_ASSERT_TRUE(sinv.equals(sinv4) == false);
#endif // MOCKPP_USE_INVOCATION_EQUALS

  Setter0Invocation sinva0(MOCKPP_PCHAR("MyInvocationDummy::set_0_Prop"));
  Setter1Invocation sinva1(MOCKPP_PCHAR("MyInvocationDummy::set_1_Prop"), i1);
  Setter2Invocation sinva2(MOCKPP_PCHAR("MyInvocationDummy::set_2_Prop"), i1, abc);
  SetterInvocation sinva3(MOCKPP_PCHAR("MyInvocationDummy::set_3_Prop"), i1, abc, xyz);
  Setter4Invocation sinva4(MOCKPP_PCHAR("MyInvocationDummy::set_4_Prop"), i1, abc, xyz, i2);
  Setter5Invocation sinva5(MOCKPP_PCHAR("MyInvocationDummy::set_5_Prop"), i1, abc, xyz, i2, s__);
#ifdef MOCKPP_USE_INVOCATION_EQUALS
  MOCKPP_ASSERT_TRUE(sinv.equals(sinva0) == false);
  MOCKPP_ASSERT_TRUE(sinv.equals(sinva1) == false);
  MOCKPP_ASSERT_TRUE(sinv.equals(sinva2) == false);
  MOCKPP_ASSERT_TRUE(sinv.equals(sinva3) == true);
  MOCKPP_ASSERT_TRUE(sinv.equals(sinva4) == false);
  MOCKPP_ASSERT_TRUE(sinv.equals(sinva5) == false);
#endif // MOCKPP_USE_INVOCATION_EQUALS
}


void Invocation_test::test_equals4()
{
  typedef MOCKPP_NS::Invocation4<int, MOCKPP_STL::string, MOCKPP_STL::string, int > SetterInvocation;

  int i1 = 1;
  int i0 = 0;
  int i2 = 2;
  int i5 = 5;
  MOCKPP_STL::string abc = "abc";
  MOCKPP_STL::string abC = "abC";
  MOCKPP_STL::string xyz = "xyz";
  MOCKPP_STL::string xyZ = "xyZ";
  MOCKPP_STL::string s__ = "==";

  SetterInvocation sinv(MOCKPP_PCHAR("MyInvocationDummy::set_4_Prop"), i1, abc, xyz, i2);
  MOCKPP_ASSERT_TRUE(sinv.toString() == MOCKPP_PCHAR("MyInvocationDummy::set_4_Prop(1, abc, xyz, 2)"));

  SetterInvocation sinv1(MOCKPP_PCHAR("MyInvocationDummy::set_4_Prop"), i1, abc, xyz, i2);
#ifdef MOCKPP_USE_INVOCATION_EQUALS
  MOCKPP_ASSERT_TRUE(sinv.equals(sinv1) == true);
#endif // MOCKPP_USE_INVOCATION_EQUALS

  SetterInvocation sinv2(MOCKPP_PCHAR("MyInvocationDummy::set_4_Prop"), i0, abc, xyz, i2);
#ifdef MOCKPP_USE_INVOCATION_EQUALS
  MOCKPP_ASSERT_TRUE(sinv.equals(sinv2) == false);
#endif // MOCKPP_USE_INVOCATION_EQUALS

  SetterInvocation sinv3(MOCKPP_PCHAR("MyInvocationDummy::set_4_Prop"), i1, abc, xyz, i5);
#ifdef MOCKPP_USE_INVOCATION_EQUALS
  MOCKPP_ASSERT_TRUE(sinv.equals(sinv3) == false);
#endif // MOCKPP_USE_INVOCATION_EQUALS

  SetterInvocation sinv4(MOCKPP_PCHAR("MyInvocationDummy::set_4_Prop"), i1, abc, xyZ, i2);
#ifdef MOCKPP_USE_INVOCATION_EQUALS
  MOCKPP_ASSERT_TRUE(sinv.equals(sinv4) == false);
#endif // MOCKPP_USE_INVOCATION_EQUALS

  SetterInvocation sinv5(MOCKPP_PCHAR("MyInvocationDummy::set_4_Prop"), i1, abC, xyz, i2);
#ifdef MOCKPP_USE_INVOCATION_EQUALS
  MOCKPP_ASSERT_TRUE(sinv.equals(sinv5) == false);
#endif // MOCKPP_USE_INVOCATION_EQUALS

  Setter0Invocation sinva0(MOCKPP_PCHAR("MyInvocationDummy::set_0_Prop"));
  Setter1Invocation sinva1(MOCKPP_PCHAR("MyInvocationDummy::set_1_Prop"), i1);
  Setter2Invocation sinva2(MOCKPP_PCHAR("MyInvocationDummy::set_2_Prop"), i1, abc);
  Setter3Invocation sinva3(MOCKPP_PCHAR("MyInvocationDummy::set_3_Prop"), i1, abc, xyz);
  SetterInvocation sinva4(MOCKPP_PCHAR("MyInvocationDummy::set_4_Prop"), i1, abc, xyz, i2);
  Setter5Invocation sinva5(MOCKPP_PCHAR("MyInvocationDummy::set_5_Prop"), i1, abc, xyz, i2, s__);
#ifdef MOCKPP_USE_INVOCATION_EQUALS
  MOCKPP_ASSERT_TRUE(sinv.equals(sinva0) == false);
  MOCKPP_ASSERT_TRUE(sinv.equals(sinva1) == false);
  MOCKPP_ASSERT_TRUE(sinv.equals(sinva2) == false);
  MOCKPP_ASSERT_TRUE(sinv.equals(sinva3) == false);
  MOCKPP_ASSERT_TRUE(sinv.equals(sinva4) == true);
  MOCKPP_ASSERT_TRUE(sinv.equals(sinva5) == false);
#endif // MOCKPP_USE_INVOCATION_EQUALS
}


void Invocation_test::test_equals5()
{
  typedef MOCKPP_NS::Invocation5<int, MOCKPP_STL::string, MOCKPP_STL::string, int, MOCKPP_STL::string > SetterInvocation;

  int i1 = 1;
  int i2 = 2;
  int i0 = 0;
  MOCKPP_STL::string abc = "abc";
  MOCKPP_STL::string abC = "abC";
  MOCKPP_STL::string xyz = "xyz";
  MOCKPP_STL::string xyZ = "xyZ";
  MOCKPP_STL::string s__ = "==";
  MOCKPP_STL::string s_I = "=!";

  SetterInvocation sinv(MOCKPP_PCHAR("MyInvocationDummy::set_5_Prop"), i1, abc, xyz, i2, s__);

  MOCKPP_ASSERT_TRUE(sinv.toString() == MOCKPP_PCHAR("MyInvocationDummy::set_5_Prop(1, abc, xyz, 2, ==)"));

  SetterInvocation sinv2(MOCKPP_PCHAR("MyInvocationDummy::set_5_Prop"), i1, abc, xyz, i2, s__);
#ifdef MOCKPP_USE_INVOCATION_EQUALS
  MOCKPP_ASSERT_TRUE(sinv.equals(sinv2) == true);
#endif // MOCKPP_USE_INVOCATION_EQUALS

  SetterInvocation sinv3(MOCKPP_PCHAR("MyInvocationDummy::set_5_Prop"), i1, abc, xyz, i2, s_I);
#ifdef MOCKPP_USE_INVOCATION_EQUALS
  MOCKPP_ASSERT_TRUE(sinv.equals(sinv3) == false);
#endif // MOCKPP_USE_INVOCATION_EQUALS

  SetterInvocation sinv4(MOCKPP_PCHAR("MyInvocationDummy::set_5_Prop"), i1, abc, xyz, i1, s__);
#ifdef MOCKPP_USE_INVOCATION_EQUALS
  MOCKPP_ASSERT_TRUE(sinv.equals(sinv4) == false);
#endif // MOCKPP_USE_INVOCATION_EQUALS

  SetterInvocation sinv5(MOCKPP_PCHAR("MyInvocationDummy::set_5_Prop"), i1, abc, xyZ, i2, s__);
#ifdef MOCKPP_USE_INVOCATION_EQUALS
  MOCKPP_ASSERT_TRUE(sinv.equals(sinv5) == false);
#endif // MOCKPP_USE_INVOCATION_EQUALS

  SetterInvocation sinv6(MOCKPP_PCHAR("MyInvocationDummy::set_5_Prop"), i1, abC, xyz, i2, s__);
#ifdef MOCKPP_USE_INVOCATION_EQUALS
  MOCKPP_ASSERT_TRUE(sinv.equals(sinv6) == false);
#endif // MOCKPP_USE_INVOCATION_EQUALS

  SetterInvocation sinv7(MOCKPP_PCHAR("MyInvocationDummy::set_5_Prop"), i0, abc, xyz, i2, s__);
#ifdef MOCKPP_USE_INVOCATION_EQUALS
  MOCKPP_ASSERT_TRUE(sinv.equals(sinv7) == false);
#endif // MOCKPP_USE_INVOCATION_EQUALS

  Setter0Invocation sinva0(MOCKPP_PCHAR("MyInvocationDummy::set_0_Prop"));
  Setter1Invocation sinva1(MOCKPP_PCHAR("MyInvocationDummy::set_1_Prop"), i1);
  Setter2Invocation sinva2(MOCKPP_PCHAR("MyInvocationDummy::set_2_Prop"), i1, abc);
  Setter3Invocation sinva3(MOCKPP_PCHAR("MyInvocationDummy::set_3_Prop"), i1, abc, xyz);
  Setter4Invocation sinva4(MOCKPP_PCHAR("MyInvocationDummy::set_4_Prop"), i1, abc, xyz, i2);
  SetterInvocation sinva5(MOCKPP_PCHAR("MyInvocationDummy::set_5_Prop"), i1, abc, xyz, i2, s__);
#ifdef MOCKPP_USE_INVOCATION_EQUALS
  MOCKPP_ASSERT_TRUE(sinv.equals(sinva0) == false);
  MOCKPP_ASSERT_TRUE(sinv.equals(sinva1) == false);
  MOCKPP_ASSERT_TRUE(sinv.equals(sinva2) == false);
  MOCKPP_ASSERT_TRUE(sinv.equals(sinva3) == false);
  MOCKPP_ASSERT_TRUE(sinv.equals(sinva4) == false);
  MOCKPP_ASSERT_TRUE(sinv.equals(sinva5) == true);
#endif // MOCKPP_USE_INVOCATION_EQUALS
}


void Invocation_test::test_get1()
{
  typedef MOCKPP_NS::Invocation1<int> SetterInvocation;

  int i1 = 1;
  SetterInvocation sinv(MOCKPP_PCHAR("MyInvocationDummy::set_1_Prop"), i1);

  int p1 = sinv.getParameter1();
  MOCKPP_ASSERT_TRUE(p1 == 1);

  MOCKPP_ASSERT_TRUE(sinv.describeParameters() == MOCKPP_PCHAR("1") );
}


void Invocation_test::test_get2()
{
  typedef MOCKPP_NS::Invocation2<int, MOCKPP_STL::string> SetterInvocation;

  int i = 1;
  MOCKPP_STL::string s = "abc";
  SetterInvocation sinv(MOCKPP_PCHAR("MyInvocationDummy::set_2_Prop"), i, s);

  int p1 = sinv.getParameter1();
  MOCKPP_ASSERT_TRUE(p1 == 1);

  MOCKPP_STL::string p2 = sinv.getParameter2();
  MOCKPP_ASSERT_TRUE(p2 == "abc");

  MOCKPP_ASSERT_TRUE(sinv.describeParameters() == MOCKPP_PCHAR("1, abc") );
}


void Invocation_test::test_get3()
{
  typedef MOCKPP_NS::Invocation3<int, MOCKPP_STL::string, MOCKPP_STL::string > SetterInvocation;

  int i = 1;
  MOCKPP_STL::string s1 = "abc";
  MOCKPP_STL::string s2 = "xyz";
  SetterInvocation sinv(MOCKPP_PCHAR("MyInvocationDummy::set_3_Prop"), i, s1, s2);

  int p1 = sinv.getParameter1();
  MOCKPP_ASSERT_TRUE(p1 == 1);

  MOCKPP_STL::string p2 = sinv.getParameter2();
  MOCKPP_ASSERT_TRUE(p2 == "abc");

  MOCKPP_STL::string p3 = sinv.getParameter3();
  MOCKPP_ASSERT_TRUE(p3 == "xyz");

  MOCKPP_ASSERT_TRUE(sinv.describeParameters() == MOCKPP_PCHAR("1, abc, xyz") );
}


void Invocation_test::test_get4()
{
  typedef MOCKPP_NS::Invocation4<int, MOCKPP_STL::string, MOCKPP_STL::string, int> SetterInvocation;

  int i1 = 1;
  MOCKPP_STL::string s1 = "abc";
  MOCKPP_STL::string s2 = "xyz";
  int i2 = 2;
  SetterInvocation sinv(MOCKPP_PCHAR("MyInvocationDummy::set_4_Prop"), i1, s1, s2, i2);

  int p1 = sinv.getParameter1();
  MOCKPP_ASSERT_TRUE(p1 == 1);

  MOCKPP_STL::string p2 = sinv.getParameter2();
  MOCKPP_ASSERT_TRUE(p2 == "abc");

  MOCKPP_STL::string p3 = sinv.getParameter3();
  MOCKPP_ASSERT_TRUE(p3 == "xyz");

  int p4 = sinv.getParameter4();
  MOCKPP_ASSERT_TRUE(p4 == 2);

  MOCKPP_ASSERT_TRUE(sinv.describeParameters() == MOCKPP_PCHAR("1, abc, xyz, 2") );
}


void Invocation_test::test_get5()
{
  typedef MOCKPP_NS::Invocation5<int, MOCKPP_STL::string, MOCKPP_STL::string, int, MOCKPP_STL::string > SetterInvocation;
  int i1 = 1;
  MOCKPP_STL::string s1 = "abc";
  MOCKPP_STL::string s2 = "xyz";
  int i2 = 2;
  MOCKPP_STL::string s3 = "==";
  SetterInvocation sinv(MOCKPP_PCHAR("MyInvocationDummy::set_5_Prop"), i1, s1, s2, i2, s3);

  int p1 = sinv.getParameter1();
  MOCKPP_ASSERT_TRUE(p1 == 1);

  MOCKPP_STL::string p2 = sinv.getParameter2();
  MOCKPP_ASSERT_TRUE(p2 == "abc");

  MOCKPP_STL::string p3 = sinv.getParameter3();
  MOCKPP_ASSERT_TRUE(p3 == "xyz");

  int p4 = sinv.getParameter4();
  MOCKPP_ASSERT_TRUE(p4 == 2);

  MOCKPP_STL::string p5 = sinv.getParameter5();
  MOCKPP_ASSERT_TRUE(p5 == "==");

  MOCKPP_ASSERT_TRUE(sinv.describeParameters() == MOCKPP_PCHAR("1, abc, xyz, 2, ==") );
}



