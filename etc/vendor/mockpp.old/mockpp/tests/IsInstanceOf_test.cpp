/***************************************************************************
             IsInstanceOf_test.cpp  -  unit tests for IsInstanceOf class
                             -------------------
    begin                : Sat Aug 21 2004
    copyright            : (C) 2002-2008 by Ewald Arnold
    email                : mockpp at ewald-arnold dot de

  $Id: IsInstanceOf_test.cpp 1438 2008-03-08 20:22:43Z ewald-arnold $

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

#include <mockpp/constraint/IsInstanceOf.h>

#include <mockpp/compat/Asserter.h>

#ifndef MOCKPP_NO_RTTI

class IsInstanceOf_test MOCKPP_DERIVE_PUBLIC_UNITFRAMEWORK
{
 public:

#if defined (MOCKPP_USE_CXXTEST)

#elif defined(MOCKPP_USE_BOOSTTEST)

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE( IsInstanceOf_test );

  CPPUNIT_TEST(test_invoke);
  CPPUNIT_TEST(test_describe);

CPPUNIT_TEST_SUITE_END();

#endif

 public:

  MOCKPP_TESTMETHOD_DECL void test_describe();
  MOCKPP_TESTMETHOD_DECL void test_invoke();
};


#if defined (MOCKPP_USE_CXXTEST)

MOCKPP_CXXTEST_SUITE_REGISTRATION( IsInstanceOf_test );
MOCKPP_CXXTEST(IsInstanceOf_test, test_describe);
MOCKPP_CXXTEST(IsInstanceOf_test, test_invoke);

#elif defined(MOCKPP_USE_BOOSTTEST)

MOCKPP_BOOST_TEST(IsInstanceOf_test::test_describe);
MOCKPP_BOOST_TEST(IsInstanceOf_test::test_invoke);

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE_REGISTRATION (IsInstanceOf_test);

#endif // frameworks



class A
{
  public:

    virtual ~A() { }

    int a;
};


class B : public A
{
  public:

    virtual ~B() {}

    double b;
};


class C : public B
{
  public:

    virtual ~C() {}

    MOCKPP_STL::string b;
};


class D
{
  public:

    virtual ~D() {}

    const char *pc;
};


void IsInstanceOf_test::test_describe()
{
   MOCKPP_NS::IsInstanceOf<A, B> instance;
   MOCKPP_NS::String s = MOCKPP_PCHAR("instance?");
   MOCKPP_ASSERT_TRUE(instance.describeTo(s).find(MOCKPP_PCHAR("instance?derived from ")) == 0);
   MOCKPP_ASSERT_TRUE(s.find(MOCKPP_PCHAR("instance?derived from ")) == 0);
}


void IsInstanceOf_test::test_invoke()
{
   A *pa = new A;
   A *pb = new B;
   A *pc = new C;
   A *pd = (A*)new D;

   MOCKPP_NS::IsInstanceOf<A, B> instance;

   MOCKPP_ASSERT_TRUE(instance.eval((B*)pa) == false);
   MOCKPP_ASSERT_TRUE(instance.eval(pb) == true);
   MOCKPP_ASSERT_TRUE(instance.eval(pc) == true);
   MOCKPP_ASSERT_TRUE(instance.eval((B*)pd) == false);

   B &rb = *((B*) 0);
   MOCKPP_ASSERT_TRUE(instance.eval(&rb) == false);

   delete pa;
   delete pb;
   delete pc;
   delete pd;
}

#endif // MOCKPP_NO_RTTI


