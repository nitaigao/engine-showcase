/***************************************************************************
                     VisitableMockMethod_1_test.cpp
                                 -
  unit tests for VisitableMockObject class and 1 parameter methods
                             -------------------
    begin                : Thu Oct 2 2005
    copyright            : (C) 2002-2008 by Ewald Arnold
    email                : mockpp at ewald-arnold dot de

  $Id: VisitableMockMethod_1_test.cpp 1438 2008-03-08 20:22:43Z ewald-arnold $

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

#include <mockpp/visiting/VisitableMockMethod0.h>
#include <mockpp/visiting/VisitableMockMethod1.h>
#include <mockpp/visiting/VisitableMockObject.h>

#include <mockpp/chaining/ChainingMockObjectSupport.h>


MOCKPP_ANON_NS_START


class VisitableMockMethod_1_test MOCKPP_DERIVE_PUBLIC_UNITFRAMEWORK
{
 public:

#if defined (MOCKPP_USE_CXXTEST)

#elif defined(MOCKPP_USE_BOOSTTEST)

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE( VisitableMockMethod_1_test );

  CPPUNIT_TEST( test_parameter_1 );
  CPPUNIT_TEST( test_parameter_1v );

CPPUNIT_TEST_SUITE_END();

#endif

 public:

  MOCKPP_TESTMETHOD_DECL void test_parameter_1();
  MOCKPP_TESTMETHOD_DECL void test_parameter_1v();
};


#if defined (MOCKPP_USE_CXXTEST)

MOCKPP_CXXTEST_SUITE_REGISTRATION( VisitableMockMethod_1_test );
MOCKPP_CXXTEST(VisitableMockMethod_1_test,  test_parameter_1 );
MOCKPP_CXXTEST(VisitableMockMethod_1_test,  test_parameter_1v );

#elif defined(MOCKPP_USE_BOOSTTEST)

MOCKPP_BOOST_TEST(VisitableMockMethod_1_test:: test_parameter_1 );
MOCKPP_BOOST_TEST(VisitableMockMethod_1_test:: test_parameter_1v );

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE_REGISTRATION (VisitableMockMethod_1_test);

#endif // frameworks



class MyVisitableMockObject_1 : public MOCKPP_NS::VisitableMockObject
{
  public:

    MyVisitableMockObject_1(const MOCKPP_NS::String &name)
      : MOCKPP_NS::VisitableMockObject(name, 0)
      , getIntPointer_mocker(MOCKPP_PCHAR("visitablev3"), this)
      , visitable_mocker(MOCKPP_PCHAR("visitablei1"), this)
      , visitablev_mocker(MOCKPP_PCHAR("visitablev2"), this)
    {}

    void visitable(const MOCKPP_NS::ConstraintHolder<unsigned> &p1)
    {
      visitable_mocker.forward(p1);
    }

    int visitable(unsigned i)
    {
      return visitable_mocker.forward(i);
    }

    int* getIntPointer()
    {
      return getIntPointer_mocker.forward();
    }

    void visitablev(const MOCKPP_NS::ConstraintHolder<unsigned> &p1)
    {
      visitablev_mocker.forward(p1);
    }

    void visitablev(unsigned i)
    {
      visitablev_mocker.forward(i);
    }

    MOCKPP_NS::VisitableMockMethod0<int*> getIntPointer_mocker;
    MOCKPP_NS::VisitableMockMethod1<int, unsigned> visitable_mocker;
    MOCKPP_NS::VisitableMockMethod1<void, unsigned> visitablev_mocker;
};


void VisitableMockMethod_1_test::test_parameter_1()
{
  MyVisitableMockObject_1 vmo(MOCKPP_PCHAR("Visitable-object"));
  MOCKPP_NS::VisitableMockMethod1<int, unsigned> &vmb (vmo.visitable_mocker);
  MOCKPP_NS::VisitableMockMethod0<int*> &gp (vmo.getIntPointer_mocker);

  vmb.addThrowable(MOCKPP_NS::make_throwable(333u));
  vmb.setDefaultThrowable(MOCKPP_NS::make_throwable(444u));
  vmb.setDefaultReturnValue(999);
  vmb.addReturnValue(888, 2);
  vmo.reset();

  //////////////////////////////////////////////////////

  vmb.setDefaultReturnValue(123);
  vmb.addReturnValue(321, 1);
  vmb.addThrowable(MOCKPP_NS::make_throwable(123u));
  vmb.addReturnValue(432, 1);
  vmb.addThrowable(MOCKPP_NS::make_throwable(321u));

  gp.addReturnValue((int*)1);
  gp.addReturnValue((int*)2);

  vmo.getIntPointer();
  vmo.getIntPointer();
  vmo.visitable(1);
  vmo.visitable(2);
  vmo.visitable(3);
  vmo.visitable(4);
  vmo.visitable(5);
  vmo.visitable(12);
  vmo.visitable(MOCKPP_NS::eq<unsigned>(10, 2));
  vmo.visitable(13);
  vmo.visitable(14);
  vmo.visitable(15);
  vmo.visitable(52);
  vmb.addResponseValue(222, 12);
  vmb.addResponseValue(333, 13);
  vmb.addResponseValue(666, MOCKPP_NS::eq<unsigned>(10, 1));
  vmb.addResponseThrowable(MOCKPP_NS::make_throwable(444u), 14);
  vmb.addResponseThrowable(MOCKPP_NS::make_throwable(555u), 15);
  vmb.addResponseThrowable(MOCKPP_NS::make_throwable(666u), MOCKPP_NS::eq<unsigned>(50, 2));

  vmo.activate();

  MOCKPP_ASSERT_EQUALS((int*)1, vmo.getIntPointer());
  MOCKPP_ASSERT_EQUALS((int*)2, vmo.getIntPointer());

  MOCKPP_ASSERT_EQUALS(321, vmo.visitable(1));

#ifndef MOCKPP_NO_EXCEPTIONS
  MOCKPP_ASSERT_THROWING(vmo.visitable(2),
                         MOCKPP_PCHAR("vmo.visitable()-1a"),
                         unsigned,
                         unsigned(123));
#endif

  MOCKPP_ASSERT_EQUALS(432, vmo.visitable(3));

#ifndef MOCKPP_NO_EXCEPTIONS
  MOCKPP_ASSERT_THROWING(vmo.visitable(4),
                         MOCKPP_PCHAR("vmo.visitable()-1b"),
                         unsigned,
                         unsigned(321));
#endif

  MOCKPP_ASSERT_EQUALS(123, vmo.visitable(5));

  MOCKPP_ASSERT_EQUALS(222, vmo.visitable(12));
  MOCKPP_ASSERT_EQUALS(666, vmo.visitable(11));
  MOCKPP_ASSERT_EQUALS(333, vmo.visitable(13));

#ifndef MOCKPP_NO_EXCEPTIONS
  MOCKPP_ASSERT_THROWING(vmo.visitable(14),
                         MOCKPP_PCHAR("vmo.visitable()-1a"),
                         unsigned,
                         unsigned(444));

  MOCKPP_ASSERT_THROWING(vmo.visitable(15),
                         MOCKPP_PCHAR("vmo.visitable()-1b"),
                         unsigned,
                         unsigned(555));

  MOCKPP_ASSERT_THROWING(vmo.visitable(52),
                         MOCKPP_PCHAR("vmo.visitable()-1c"),
                         unsigned,
                         unsigned(666));
#endif

  vmo.verify();
  vmb.verify();

  //////////////////////////////////////////////////////

  vmo.reset();
  vmb.unsetThrowablesInline();

  vmb.setDefaultReturnValue(123);

  vmb.addReturnValue(321, 1);
  vmb.addThrowable(MOCKPP_NS::make_throwable(123u));
  vmb.addReturnValue(432, 1);
  vmb.addThrowable(MOCKPP_NS::make_throwable(321u));

  vmo.visitable(9);
  vmo.visitable(8);
  vmo.visitable(1);
  vmo.visitable(2);
  vmo.visitable(3);
  vmo.activate();

#ifndef MOCKPP_NO_EXCEPTIONS
  MOCKPP_ASSERT_THROWING(vmo.visitable(9),
                         MOCKPP_PCHAR("vmo.visitable()-2a"),
                         unsigned,
                         unsigned(123));

  MOCKPP_ASSERT_THROWING(vmo.visitable(9),
                         MOCKPP_PCHAR("vmo.visitable()-2b"),
                         unsigned,
                         unsigned(321));
#endif

  MOCKPP_ASSERT_EQUALS(321, vmo.visitable(1));
  MOCKPP_ASSERT_EQUALS(432, vmo.visitable(2));
  MOCKPP_ASSERT_EQUALS(123, vmo.visitable(3));

  vmo.verify();
  vmb.verify();

  //////////////////////////////////////////////////////

  vmo.reset();
  vmb.setDefaultThrowable(MOCKPP_NS::make_throwable(111u));
  vmb.addThrowable(MOCKPP_NS::make_throwable(123u));

  vmo.visitable(1);
  vmo.visitable(2);
  vmo.visitable(3);
  vmo.activate();

#ifndef MOCKPP_NO_EXCEPTIONS
  MOCKPP_ASSERT_THROWING(vmo.visitable(1),
                         MOCKPP_PCHAR("vmo.visitable()-3a"),
                         unsigned,
                         unsigned(123));

  MOCKPP_ASSERT_THROWING(vmo.visitable(2),
                         MOCKPP_PCHAR("vmo.visitable()-3b"),
                         unsigned,
                         unsigned(111));

  MOCKPP_ASSERT_THROWING(vmo.visitable(3),
                         MOCKPP_PCHAR("vmo.visitable()-3c"),
                         unsigned,
                         unsigned(111));
#endif

  vmo.verify();
  vmb.verify();
}


void VisitableMockMethod_1_test::test_parameter_1v()
{
  MyVisitableMockObject_1 vmo(MOCKPP_PCHAR("Visitable-object"));
  MOCKPP_NS::VisitableMockMethod1<void, unsigned> &vmb (vmo.visitablev_mocker);
  MOCKPP_NS::VisitableMockMethod1<int, unsigned> &vmbi (vmo.visitable_mocker);

  vmb.setDefaultThrowable(MOCKPP_NS::make_throwable(444u));
  vmb.addThrowable(MOCKPP_NS::make_throwable(333u));
  vmo.reset();

  vmbi.setDefaultReturnValue(999);

  //////////////////////////////////////////////////////

  vmb.addThrowable(MOCKPP_NS::make_throwable(123u));
  vmb.addThrowable(MOCKPP_NS::make_throwable(321u));

  vmo.visitablev(1);
  vmo.visitablev(2);
  vmo.visitablev(2);
  vmo.visitablev(MOCKPP_NS::eq<unsigned>(10, 2));
  vmo.visitable(20);
  vmo.visitable(200);
  vmo.visitablev(3);
  vmo.visitablev(14);
  vmo.visitablev(15);
  vmb.addResponseThrowable(MOCKPP_NS::make_throwable(444u), 14);
  vmb.addResponseThrowable(MOCKPP_NS::make_throwable(555u), 15);

  vmo.activate();

#ifndef MOCKPP_NO_EXCEPTIONS
  MOCKPP_ASSERT_THROWING(vmo.visitablev(2),
                         MOCKPP_PCHAR("vmo.visitablev()-1a"),
                         unsigned,
                         unsigned(123));

  MOCKPP_ASSERT_THROWING(vmo.visitablev(4),
                         MOCKPP_PCHAR("vmo.visitablev()-1b"),
                         unsigned,
                         unsigned(321));
#endif

  vmo.visitablev(2);
  vmo.visitablev(11);
  MOCKPP_ASSERT_EQUALS(999, vmo.visitable(20));
  MOCKPP_ASSERT_EQUALS(999, vmo.visitable(200));
  vmo.visitablev(3);

#ifndef MOCKPP_NO_EXCEPTIONS
  MOCKPP_ASSERT_THROWING(vmo.visitablev(14),
                         MOCKPP_PCHAR("vmo.visitablev()-2"),
                         unsigned,
                         unsigned(444));

  MOCKPP_ASSERT_THROWING(vmo.visitablev(15),
                         MOCKPP_PCHAR("vmo.visitablev()-3"),
                         unsigned,
                         unsigned(555));
#endif

  vmo.verify();
  vmb.verify();

  //////////////////////////////////////////////////////

  vmo.reset();
  vmb.unsetThrowablesInline();

  vmb.addThrowable(MOCKPP_NS::make_throwable(123u));
  vmb.addThrowable(MOCKPP_NS::make_throwable(321u));

  vmo.visitablev(9);
  vmo.visitablev(8);
  vmo.visitablev(1);
  vmo.visitablev(2);
  vmo.visitablev(3);
  vmo.activate();

#ifndef MOCKPP_NO_EXCEPTIONS
  MOCKPP_ASSERT_THROWING(vmo.visitablev(9),
                         MOCKPP_PCHAR("vmo.visitablev()-2a"),
                         unsigned,
                         unsigned(123));

  MOCKPP_ASSERT_THROWING(vmo.visitablev(8),
                         MOCKPP_PCHAR("vmo.visitablev()-2b"),
                         unsigned,
                         unsigned(321));
#endif

  vmo.visitablev(1);
  vmo.visitablev(2);
  vmo.visitablev(3);

  vmo.verify();
  vmb.verify();

  //////////////////////////////////////////////////////

  vmo.reset();
  vmb.setDefaultThrowable(MOCKPP_NS::make_throwable(111u));
  vmb.addThrowable(MOCKPP_NS::make_throwable(123u));

  vmo.visitablev(1);
  vmo.visitablev(2);
  vmo.visitablev(3);
  vmo.activate();

#ifndef MOCKPP_NO_EXCEPTIONS
  MOCKPP_ASSERT_THROWING(vmo.visitablev(1),
                         MOCKPP_PCHAR("vmo.visitablev()-3a"),
                         unsigned,
                         unsigned(123));

  MOCKPP_ASSERT_THROWING(vmo.visitablev(2),
                         MOCKPP_PCHAR("vmo.visitablev()-3b"),
                         unsigned,
                         unsigned(111));

  MOCKPP_ASSERT_THROWING(vmo.visitablev(3),
                         MOCKPP_PCHAR("vmo.visitablev()-3c"),
                         unsigned,
                         unsigned(111));
#endif

  vmo.verify();
  vmb.verify();
}


MOCKPP_NS_END



