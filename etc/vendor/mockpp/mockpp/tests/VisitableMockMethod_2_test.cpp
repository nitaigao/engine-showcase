/***************************************************************************
                     VisitableMockMethod_2_test.cpp
                                 -
  unit tests for VisitableMockObject class and 1 parameter methods
                             -------------------
    begin                : Thu Oct 2 2005
    copyright            : (C) 2002-2008 by Ewald Arnold
    email                : mockpp at ewald-arnold dot de

  $Id: VisitableMockMethod_2_test.cpp 1438 2008-03-08 20:22:43Z ewald-arnold $

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

#define MOCKPP_ENABLE_DEFAULT_FORMATTER

#include <mockpp/mockpp.h> // always first

#include "SelectFramework.h"

#include <mockpp/visiting/VisitableMockMethod1.h>
#include <mockpp/visiting/VisitableMockMethod2.h>
#include <mockpp/visiting/VisitableMockObject.h>

#include <mockpp/chaining/ChainingMockObjectSupport.h>


MOCKPP_ANON_NS_START


class VisitableMockMethod_2_test MOCKPP_DERIVE_PUBLIC_UNITFRAMEWORK
{
 public:

#if defined (MOCKPP_USE_CXXTEST)

#elif defined(MOCKPP_USE_BOOSTTEST)

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE( VisitableMockMethod_2_test );

  CPPUNIT_TEST( test_parameter_2 );
  CPPUNIT_TEST( test_parameter_2v );

CPPUNIT_TEST_SUITE_END();

#endif

 public:

  MOCKPP_TESTMETHOD_DECL void test_parameter_2();
  MOCKPP_TESTMETHOD_DECL void test_parameter_2v();
};


#if defined (MOCKPP_USE_CXXTEST)

MOCKPP_CXXTEST_SUITE_REGISTRATION( VisitableMockMethod_2_test );
MOCKPP_CXXTEST(VisitableMockMethod_2_test,  test_parameter_2 );
MOCKPP_CXXTEST(VisitableMockMethod_2_test,  test_parameter_2v );

#elif defined(MOCKPP_USE_BOOSTTEST)

MOCKPP_BOOST_TEST(VisitableMockMethod_2_test:: test_parameter_2 );
MOCKPP_BOOST_TEST(VisitableMockMethod_2_test:: test_parameter_2v );

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE_REGISTRATION (VisitableMockMethod_2_test);

#endif // frameworks



class MyVisitableMockObject_2 : public MOCKPP_NS::VisitableMockObject
{
  public:

    MyVisitableMockObject_2(const MOCKPP_NS::String &name)
      : MOCKPP_NS::VisitableMockObject(name, 0)
      , visitables_mocker(MOCKPP_PCHAR("visitables"), this)
      , visitable_mocker(MOCKPP_PCHAR("visitablei2"), this)
      , visitablev_mocker(MOCKPP_PCHAR("visitablev2"), this)
    {}

    void visitable(const MOCKPP_NS::ConstraintHolder<unsigned> &p1,
                   const MOCKPP_NS::ConstraintHolder<unsigned> &p2)
    {
      visitable_mocker.forward(p1, p2);
    }

    int visitable(unsigned i, unsigned j)
    {
      return visitable_mocker.forward(i, j);
    }

    void visitablev(const MOCKPP_NS::ConstraintHolder<unsigned> &p1,
                    const MOCKPP_NS::ConstraintHolder<unsigned> &p2)
    {
      visitablev_mocker.forward(p1, p2);
    }

    void visitablev(unsigned i, unsigned j)
    {
      visitablev_mocker.forward(i, j);
    }

    void visitables(const MOCKPP_NS::ConstraintHolder<std::string> &p)
    {
      visitables_mocker.forward(p);
    }

    void visitables(const std::string &s)
    {
      visitables_mocker.forward(s);
    }

    MOCKPP_NS::VisitableMockMethod1<int, std::string>  visitables_mocker;
    MOCKPP_NS::VisitableMockMethod2<int, unsigned, unsigned>  visitable_mocker;
    MOCKPP_NS::VisitableMockMethod2<void, unsigned, unsigned> visitablev_mocker;
};


void VisitableMockMethod_2_test::test_parameter_2()
{
  MyVisitableMockObject_2 vmo(MOCKPP_PCHAR("Visitable-object"));
  MOCKPP_NS::VisitableMockMethod2<int, unsigned, unsigned> &vmb (vmo.visitable_mocker);
  vmo.verify();
  vmb.verify();

  vmb.addThrowable(0);

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

  vmo.visitable(1, 2);
  vmo.visitable(2, 3);
  vmo.visitable(3, 4);
  vmo.visitable(4, 5);
  vmo.visitable(5, 6);
  vmo.visitable(2, 2);
  vmo.visitable(MOCKPP_NS::eq<unsigned>(2, 2),
                MOCKPP_NS::eq<unsigned>(20, 2));
  vmo.visitable(3, 3);
  vmo.visitable(4, 4);
  vmo.visitable(5, 5);
  vmo.visitable(52, 58);
  vmb.addResponseValue(222, 2, 2);
  vmb.addResponseValue(333, 3, 3);
  vmb.addResponseValue(666, MOCKPP_NS::eq<unsigned>(2, 2),
                             MOCKPP_NS::eq<unsigned>(20, 2));
  vmb.addResponseThrowable(MOCKPP_NS::make_throwable(444u), 4, 4);
  vmb.addResponseThrowable(MOCKPP_NS::make_throwable(555u), 5, 5);
  vmb.addResponseThrowable(MOCKPP_NS::make_throwable(666u), MOCKPP_NS::eq<unsigned>(50, 2),
                                                           MOCKPP_NS::eq<unsigned>(60, 2));

  vmo.activate();

  MOCKPP_ASSERT_EQUALS(321, vmo.visitable(1, 2));

#ifndef MOCKPP_NO_EXCEPTIONS
  MOCKPP_ASSERT_THROWING(vmo.visitable(2, 3),
                         MOCKPP_PCHAR("vmo.visitable()-1a"),
                         unsigned,
                         unsigned(123));
#endif

  MOCKPP_ASSERT_EQUALS(432, vmo.visitable(3, 4));

#ifndef MOCKPP_NO_EXCEPTIONS
  MOCKPP_ASSERT_THROWING(vmo.visitable(4, 5),
                         MOCKPP_PCHAR("vmo.visitable()-1b"),
                         unsigned,
                         unsigned(321));
#endif

  MOCKPP_ASSERT_EQUALS(123, vmo.visitable(5, 6));

  MOCKPP_ASSERT_EQUALS(222, vmo.visitable(2, 2));
  MOCKPP_ASSERT_EQUALS(666, vmo.visitable(3, 21));
  MOCKPP_ASSERT_EQUALS(333, vmo.visitable(3, 3));

#ifndef MOCKPP_NO_EXCEPTIONS
  MOCKPP_ASSERT_THROWING(vmo.visitable(4, 4),
                         MOCKPP_PCHAR("vmo.visitable()-2"),
                         unsigned,
                         unsigned(444));

  MOCKPP_ASSERT_THROWING(vmo.visitable(5, 5),
                         MOCKPP_PCHAR("vmo.visitable()-3"),
                         unsigned,
                         unsigned(555));

  MOCKPP_ASSERT_THROWING(vmo.visitable(52, 58),
                         MOCKPP_PCHAR("vmo.visitable()-3"),
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

  vmo.visitable(9, 9);
  vmo.visitable(8, 8);
  vmo.visitable(1, 2);
  vmo.visitable(2, 3);
  vmo.visitable(3, 4);
  vmo.activate();

#ifndef MOCKPP_NO_EXCEPTIONS
  MOCKPP_ASSERT_THROWING(vmo.visitable(9, 9),
                         MOCKPP_PCHAR("vmo.visitable()-2a"),
                         unsigned,
                         unsigned(123));

  MOCKPP_ASSERT_THROWING(vmo.visitable(9, 9),
                         MOCKPP_PCHAR("vmo.visitable()-2b"),
                         unsigned,
                         unsigned(321));
#endif

  MOCKPP_ASSERT_EQUALS(321, vmo.visitable(1, 2));
  MOCKPP_ASSERT_EQUALS(432, vmo.visitable(2, 3));
  MOCKPP_ASSERT_EQUALS(123, vmo.visitable(3, 4));

  vmo.verify();
  vmb.verify();

  //////////////////////////////////////////////////////

  vmo.reset();
  vmb.setDefaultThrowable(MOCKPP_NS::make_throwable(111u));
  vmb.addThrowable(MOCKPP_NS::make_throwable(123u));

  vmo.visitable(1, 2);
  vmo.visitable(2, 3);
  vmo.visitable(3, 4);
  vmo.activate();

#ifndef MOCKPP_NO_EXCEPTIONS
  MOCKPP_ASSERT_THROWING(vmo.visitable(1, 2),
                         MOCKPP_PCHAR("vmo.visitable()-3a"),
                         unsigned,
                         unsigned(123));

  MOCKPP_ASSERT_THROWING(vmo.visitable(2, 3),
                         MOCKPP_PCHAR("vmo.visitable()-3b"),
                         unsigned,
                         unsigned(111));

  MOCKPP_ASSERT_THROWING(vmo.visitable(3, 4),
                         MOCKPP_PCHAR("vmo.visitable()-3c"),
                         unsigned,
                         unsigned(111));
#endif

  vmo.verify();
  vmb.verify();
}


void VisitableMockMethod_2_test::test_parameter_2v()
{
  MyVisitableMockObject_2 vmo(MOCKPP_PCHAR("Visitable-object"));
  MOCKPP_NS::VisitableMockMethod2<void, unsigned, unsigned> &vmb (vmo.visitablev_mocker);
  MOCKPP_NS::VisitableMockMethod2<int, unsigned, unsigned> &vmbi (vmo.visitable_mocker);
  vmo.verify();
  vmb.verify();

  vmb.setDefaultThrowable(MOCKPP_NS::make_throwable(444u));
  vmb.addThrowable(MOCKPP_NS::make_throwable(333u));
  vmo.reset();

  vmbi.setDefaultReturnValue(999);

  //////////////////////////////////////////////////////

  vmb.addThrowable(MOCKPP_NS::make_throwable(123u));
  vmb.addThrowable(MOCKPP_NS::make_throwable(321u));

  vmo.visitablev(1, 2);
  vmo.visitablev(2, 3);
  vmo.visitablev(2, 2);
  vmo.visitablev(MOCKPP_NS::eq<unsigned>(2, 1),
                 MOCKPP_NS::eq<unsigned>(20, 3));
  vmo.visitable(20, 30);
  vmo.visitable(200, 300);
  vmo.visitablev(3, 3);
  vmo.visitablev(4, 4);
  vmo.visitablev(5, 5);
  vmb.addResponseThrowable(MOCKPP_NS::make_throwable(444u), 4, 4);
  vmb.addResponseThrowable(MOCKPP_NS::make_throwable(555u), 5, 5);

  vmo.activate();

#ifndef MOCKPP_NO_EXCEPTIONS
  MOCKPP_ASSERT_THROWING(vmo.visitablev(2, 3),
                         MOCKPP_PCHAR("vmo.visitablev()-1a"),
                         unsigned,
                         unsigned(123));

  MOCKPP_ASSERT_THROWING(vmo.visitablev(4, 5),
                         MOCKPP_PCHAR("vmo.visitablev()-1b"),
                         unsigned,
                         unsigned(321));
#endif

  vmo.visitablev(2, 2);
  vmo.visitablev(1, 19);
  MOCKPP_ASSERT_EQUALS(999, vmo.visitable(20, 30));
  MOCKPP_ASSERT_EQUALS(999, vmo.visitable(200, 300));
  vmo.visitablev(3, 3);

#ifndef MOCKPP_NO_EXCEPTIONS
  MOCKPP_ASSERT_THROWING(vmo.visitablev(4, 4),
                         MOCKPP_PCHAR("vmo.visitablev()-2"),
                         unsigned,
                         unsigned(444));

  MOCKPP_ASSERT_THROWING(vmo.visitablev(5, 5),
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

  vmo.visitablev(9, 9);
  vmo.visitablev(8, 8);
  vmo.visitablev(1, 2);
  vmo.visitablev(2, 3);
  vmo.visitablev(3, 4);
  vmo.activate();

#ifndef MOCKPP_NO_EXCEPTIONS
  MOCKPP_ASSERT_THROWING(vmo.visitablev(9, 9),
                         MOCKPP_PCHAR("vmo.visitablev()-2a"),
                         unsigned,
                         unsigned(123));

  MOCKPP_ASSERT_THROWING(vmo.visitablev(8, 8),
                         MOCKPP_PCHAR("vmo.visitablev()-2b"),
                         unsigned,
                         unsigned(321));
#endif

  vmo.visitablev(1, 2);
  vmo.visitablev(2, 3);
  vmo.visitablev(3, 4);

  vmo.verify();
  vmb.verify();

  //////////////////////////////////////////////////////

  vmo.reset();
  vmb.setDefaultThrowable(MOCKPP_NS::make_throwable(111u));
  vmb.addThrowable(MOCKPP_NS::make_throwable(123u));

  vmo.visitablev(1, 2);
  vmo.visitablev(2, 3);
  vmo.visitablev(3, 4);
  vmo.activate();

#ifndef MOCKPP_NO_EXCEPTIONS
  MOCKPP_ASSERT_THROWING(vmo.visitablev(1, 2),
                         MOCKPP_PCHAR("vmo.visitablev()-3a"),
                         unsigned,
                         unsigned(123));

  MOCKPP_ASSERT_THROWING(vmo.visitablev(2, 3),
                         MOCKPP_PCHAR("vmo.visitablev()-3b"),
                         unsigned,
                         unsigned(111));

  MOCKPP_ASSERT_THROWING(vmo.visitablev(3, 4),
                         MOCKPP_PCHAR("vmo.visitablev()-3c"),
                         unsigned,
                         unsigned(111));
#endif

  vmo.verify();
  vmb.verify();
}


MOCKPP_NS_END




