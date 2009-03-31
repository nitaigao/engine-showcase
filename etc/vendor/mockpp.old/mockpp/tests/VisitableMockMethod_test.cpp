/***************************************************************************
                     VisitableMockMethod_test.cpp
                                 -
  unit tests for VisitableMockObject class and no parameter methods
                             -------------------
    begin                : Sat Oct 15 2005
    copyright            : (C) 2002-2008 by Ewald Arnold
    email                : mockpp at ewald-arnold dot de

  $Id: VisitableMockMethod_test.cpp 1438 2008-03-08 20:22:43Z ewald-arnold $

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

//#define MOCKPP_COUNTED_WEAKNESS  // only BCB5

#include <mockpp/visiting/VisitableMockMethod.h>
#include <mockpp/visiting/VisitableMockMethod0.h>
#include <mockpp/visiting/VisitableMockMethod1.h>
#include <mockpp/visiting/VisitableMockMethod2.h>
#include <mockpp/visiting/VisitableMockMethod3.h>
#include <mockpp/visiting/VisitableMockMethod4.h>
#include <mockpp/visiting/VisitableMockMethod5.h>
#include <mockpp/visiting/VisitableMockMethod6.h>
#include <mockpp/visiting/CountedVisitableMethod.h>
#include <mockpp/visiting/VisitableMockObject.h>

#include <mockpp/constraint/OutBound.h>

#include <mockpp/compat/Asserter.h>

#include <mockpp/Throwable.h>



MOCKPP_ANON_NS_START


class VisitableMockMethod_test MOCKPP_DERIVE_PUBLIC_UNITFRAMEWORK
{
 public:

#if defined (MOCKPP_USE_CXXTEST)

#elif defined(MOCKPP_USE_BOOSTTEST)

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE( VisitableMockMethod_test );

  CPPUNIT_TEST( test_parameter_0 );
  CPPUNIT_TEST( test_parameter_0v );
  CPPUNIT_TEST( test_base );
  CPPUNIT_TEST( test_inst );
  CPPUNIT_TEST( test_base_returning );

CPPUNIT_TEST_SUITE_END();

#endif

 public:

  MOCKPP_TESTMETHOD_DECL void test_base();
  MOCKPP_TESTMETHOD_DECL void test_inst();
  MOCKPP_TESTMETHOD_DECL void test_base_returning();
  MOCKPP_TESTMETHOD_DECL void test_parameter_0();
  MOCKPP_TESTMETHOD_DECL void test_parameter_0v();
};


#if defined (MOCKPP_USE_CXXTEST)

MOCKPP_CXXTEST_SUITE_REGISTRATION( VisitableMockMethod_test );

MOCKPP_CXXTEST(VisitableMockMethod_test,  test_parameter_0 );
MOCKPP_CXXTEST(VisitableMockMethod_test,  test_parameter_0v );
MOCKPP_CXXTEST(VisitableMockMethod_test,  test_base );
MOCKPP_CXXTEST(VisitableMockMethod_test,  test_inst );
MOCKPP_CXXTEST(VisitableMockMethod_test,  test_base_returning );

#elif defined(MOCKPP_USE_BOOSTTEST)

MOCKPP_BOOST_TEST(VisitableMockMethod_test:: test_parameter_0 );
MOCKPP_BOOST_TEST(VisitableMockMethod_test:: test_parameter_0v );
MOCKPP_BOOST_TEST(VisitableMockMethod_test:: test_base );
MOCKPP_BOOST_TEST(VisitableMockMethod_test:: test_inst );
MOCKPP_BOOST_TEST(VisitableMockMethod_test:: test_base_returning );

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE_REGISTRATION (VisitableMockMethod_test);

#endif // frameworks



class MyVisitableMockObject_0 : public MOCKPP_NS::VisitableMockObject
{
  public:

    MyVisitableMockObject_0(const MOCKPP_NS::String &name)
      : MOCKPP_NS::VisitableMockObject(name, 0)
      , visitable_mocker(MOCKPP_PCHAR("visitable"), this)
      , visitablev_mocker(MOCKPP_PCHAR("visitablev"), this)
    {}

    int visitable()
    {
      return visitable_mocker.forward();
    }

    void visitablev()
    {
      visitablev_mocker.forward();
    }

    MOCKPP_NS::VisitableMockMethod0<int>  visitable_mocker;
    MOCKPP_NS::VisitableMockMethod0<void> visitablev_mocker;
};


void VisitableMockMethod_test::test_inst()
{
  MOCKPP_NS::VisitableMockMethod0<void> mmv0(MOCKPP_PCHAR("mm0"), 0);
  MOCKPP_NS::VisitableMockMethod1<void, int> mmv1(MOCKPP_PCHAR("mm1"), 0);
  MOCKPP_NS::VisitableMockMethod2<void, int, int> mmv2(MOCKPP_PCHAR("mm2"), 0);
  MOCKPP_NS::VisitableMockMethod3<void, int, int, int> mmv3(MOCKPP_PCHAR("mm3"), 0);
  MOCKPP_NS::VisitableMockMethod4<void, int, int, int, int> mmv4(MOCKPP_PCHAR("mm4"), 0);
  MOCKPP_NS::VisitableMockMethod5<void, int, int, int, int, int> mmv5(MOCKPP_PCHAR("mm5"), 0);
  MOCKPP_NS::VisitableMockMethod6<void, int, int, int, int, int, int> mmv6(MOCKPP_PCHAR("mm6"), 0);

  MOCKPP_NS::VisitableMockMethod0<int> mmi0(MOCKPP_PCHAR("mm0"), 0);
  MOCKPP_NS::VisitableMockMethod1<int, int> mmi1(MOCKPP_PCHAR("mm1"), 0);
  MOCKPP_NS::VisitableMockMethod2<int, int, int> mmi2(MOCKPP_PCHAR("mm2"), 0);
  MOCKPP_NS::VisitableMockMethod3<int, int, int, int> mmi3(MOCKPP_PCHAR("mm3"), 0);
  MOCKPP_NS::VisitableMockMethod4<int, int, int, int, int> mmi4(MOCKPP_PCHAR("mm4"), 0);
  MOCKPP_NS::VisitableMockMethod5<int, int, int, int, int, int> mmi5(MOCKPP_PCHAR("mm5"), 0);
  MOCKPP_NS::VisitableMockMethod6<int, int, int, int, int, int, int> mmi6(MOCKPP_PCHAR("mm6"), 0);

  MOCKPP_NS::VisitableMockMethod<void> mmvd0(MOCKPP_PCHAR("mm0"), 0);
  MOCKPP_NS::VisitableMockMethod<void, int> mmvd1(MOCKPP_PCHAR("mm1"), 0);
  MOCKPP_NS::VisitableMockMethod<void, int, int> mmvd2(MOCKPP_PCHAR("mm2"), 0);
  MOCKPP_NS::VisitableMockMethod<void, int, int, int> mmvd3(MOCKPP_PCHAR("mm3"), 0);
  MOCKPP_NS::VisitableMockMethod<void, int, int, int, int> mmvd4(MOCKPP_PCHAR("mm4"), 0);
  MOCKPP_NS::VisitableMockMethod<void, int, int, int, int, int> mmvd5(MOCKPP_PCHAR("mm5"), 0);
  MOCKPP_NS::VisitableMockMethod<void, int, int, int, int, int, int> mmvd6(MOCKPP_PCHAR("mm6"), 0);

  MOCKPP_NS::VisitableMockMethod<int> mmid0(MOCKPP_PCHAR("mm0"), 0);
  MOCKPP_NS::VisitableMockMethod<int, int> mmid1(MOCKPP_PCHAR("mm1"), 0);
  MOCKPP_NS::VisitableMockMethod<int, int, int> mmid2(MOCKPP_PCHAR("mm2"), 0);
  MOCKPP_NS::VisitableMockMethod<int, int, int, int> mmid3(MOCKPP_PCHAR("mm3"), 0);
  MOCKPP_NS::VisitableMockMethod<int, int, int, int, int> mmid4(MOCKPP_PCHAR("mm4"), 0);
  MOCKPP_NS::VisitableMockMethod<int, int, int, int, int, int> mmid5(MOCKPP_PCHAR("mm5"), 0);
  MOCKPP_NS::VisitableMockMethod<int, int, int, int, int, int, int> mmid6(MOCKPP_PCHAR("mm6"), 0);

//  MOCKPP_NS::VisitableMockMethod<int(int, int, int, int, int, int)> mmidp6(MOCKPP_PCHAR("mm6"), 0);
}


class MyVisitableMockMethodBase : public MOCKPP_NS::VisitableMockMethodBase
{
  public:

    MyVisitableMockMethodBase(const MOCKPP_NS::String &name, MOCKPP_NS::VisitableMockObject *parent)
      : MOCKPP_NS::VisitableMockMethodBase(name, parent)
    {}

#if defined(__BORLANDC__) // ==> BCB5.5.1 ?? F1004 Internal compiler error at 0x12548c1 with base 0x1200000
    void throwAvailableException() const
    {
      MOCKPP_NS::VisitableMockMethodBase::throwAvailableException();
    }
#else
    using MOCKPP_NS::VisitableMockMethodBase::throwAvailableException;
#endif
};


void VisitableMockMethod_test::test_base()
{
  MOCKPP_NS::VisitableMockObject vmo (MOCKPP_PCHAR("mockobj"), 0);
  MyVisitableMockMethodBase vmb (MOCKPP_PCHAR("methodbase"), &vmo);

  MOCKPP_ASSERT_TRUE(vmb.getMethodName() == MOCKPP_PCHAR("methodbase"));
  MOCKPP_ASSERT_TRUE(vmb.getMethodIdentifier() == "methodbase");
  MOCKPP_ASSERT_TRUE(vmb.getVisitableMockObject() == &vmo);

  vmb.addThrowable(MOCKPP_NS::make_throwable(123u));
  vmb.addThrowable(123u);
  vmb.setDefaultThrowable(MOCKPP_NS::make_throwable(321u));

  vmo.activate();
  vmo.reset();

  vmb.addThrowable(MOCKPP_NS::make_throwable(123u));
  vmb.addThrowable(333u, 2);
  vmb.setDefaultThrowable(MOCKPP_NS::make_throwable(321u));
  vmo.activate();

#ifndef MOCKPP_NO_EXCEPTIONS
  MOCKPP_ASSERT_THROWING(vmb.throwAvailableException(),
                         MOCKPP_PCHAR("vmb.throwAvailableException()"),
                         unsigned,
                         unsigned(123));

  MOCKPP_ASSERT_THROWING(vmb.throwAvailableException(),
                         MOCKPP_PCHAR("vmb.throwAvailableException()"),
                         unsigned,
                         unsigned(333));

  MOCKPP_ASSERT_THROWING(vmb.throwAvailableException(),
                         MOCKPP_PCHAR("vmb.throwAvailableException()"),
                         unsigned,
                         unsigned(333));

  MOCKPP_ASSERT_THROWING(vmb.throwAvailableException(),
                         MOCKPP_PCHAR("vmb.throwAvailableException()"),
                         unsigned,
                         unsigned(321));

  MOCKPP_ASSERT_THROWING(vmb.throwAvailableException(),
                         MOCKPP_PCHAR("vmb.throwAvailableException()"),
                         unsigned,
                         unsigned(321));
#endif

  vmo.verify();
  vmb.verify();

  vmo.reset();
  vmo.verify();
  vmb.verify();

  vmb.setDefaultThrowable(MOCKPP_NS::make_throwable(321u));
  vmo.activate();
#ifndef MOCKPP_NO_EXCEPTIONS
  MOCKPP_ASSERT_THROWING_COND(vmo.verify(),
                              MOCKPP_PCHAR("vmo.verify()"),
                              MOCKPP_NS::AssertionFailedError,
                              exception_object.getMessage().find(MOCKPP_PCHAR("/defaultThrowable is unused")) != MOCKPP_NS::String::npos);
#endif
}


template <class T>
class MyVisitableMockReturningMethodBase : public MOCKPP_NS::VisitableMockReturningMethodBase<T>
{
  public:

    MyVisitableMockReturningMethodBase(const MOCKPP_NS::String &name, MOCKPP_NS::VisitableMockObject *parent)
      : MOCKPP_NS::VisitableMockReturningMethodBase<T>(name, parent)
    {}

#if defined(__BORLANDC__) // ==> BCB5.5.1 ?? F1004 Internal compiler error at 0x12548c1 with base 0x1200000
    void throwAvailableException() const
    {
      MOCKPP_NS::VisitableMockReturningMethodBase<T>::throwAvailableException();
    }

    T determineReturnValue() const
    {
      return MOCKPP_NS::VisitableMockReturningMethodBase<T>::determineReturnValue();
    }
#else
    using MOCKPP_NS::VisitableMockReturningMethodBase<T>::throwAvailableException;
    using MOCKPP_NS::VisitableMockReturningMethodBase<T>::determineReturnValue;
#endif
};


void VisitableMockMethod_test::test_base_returning()
{
  MOCKPP_NS::VisitableMockObject vmo (MOCKPP_PCHAR("mockobj"), 0);
  MyVisitableMockReturningMethodBase<int> vmb (MOCKPP_PCHAR("methodbase-2"), &vmo);

  vmb.setDefaultReturnValue(123);
  vmb.addReturnValue(321,2);
  vmo.reset();

  //////////////////////////////////////////////////////

  vmb.setDefaultReturnValue(123);
  vmb.addReturnValue(321, 2);
  vmb.addThrowable(MOCKPP_NS::make_throwable(123u));
  vmo.activate();

  MOCKPP_ASSERT_EQUALS(321, vmb.determineReturnValue());
  MOCKPP_ASSERT_EQUALS(321, vmb.determineReturnValue());
  MOCKPP_ASSERT_EQUALS(123, vmb.determineReturnValue());

  vmb.throwAvailableException();
  vmb.throwAvailableException();
#ifndef MOCKPP_NO_EXCEPTIONS
  MOCKPP_ASSERT_THROWING(vmb.throwAvailableException(),
                         MOCKPP_PCHAR("vmb.throwAvailableException()-1"),
                         unsigned,
                         unsigned(123));
#endif
  vmo.verify();
  vmb.verify();

  //////////////////////////////////////////////////////

  vmo.reset();
  vmb.unsetThrowablesInline();

  vmb.setDefaultReturnValue(123);
  vmb.addReturnValue(321, 2);
  vmb.addThrowable(MOCKPP_NS::make_throwable(123u));
  vmo.activate();

#ifndef MOCKPP_NO_EXCEPTIONS
  MOCKPP_ASSERT_THROWING(vmb.throwAvailableException(),
                         MOCKPP_PCHAR("vmb.throwAvailableException()-2"),
                         unsigned,
                         unsigned(123));
#endif

  MOCKPP_ASSERT_EQUALS(321, vmb.determineReturnValue());
  MOCKPP_ASSERT_EQUALS(321, vmb.determineReturnValue());
  MOCKPP_ASSERT_EQUALS(123, vmb.determineReturnValue());

  vmo.verify();
  vmb.verify();

  //////////////////////////////////////////////////////

  vmo.reset();
  vmb.setDefaultReturnValue(123);
#ifndef MOCKPP_NO_EXCEPTIONS
  MOCKPP_ASSERT_THROWING_COND(vmo.verify(),
                              MOCKPP_PCHAR("vmo.verify()"),
                              MOCKPP_NS::AssertionFailedError,
                              exception_object.getMessage().find(MOCKPP_PCHAR("/defaultReturnValueUsed is unused")) != MOCKPP_NS::String::npos);
#endif
}


void VisitableMockMethod_test::test_parameter_0()
{
  MyVisitableMockObject_0 vmo(MOCKPP_PCHAR("Visitable-object"));
  MOCKPP_NS::VisitableMockMethod0<int> &vmb (vmo.visitable_mocker);

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

  vmo.visitable();
  vmo.visitable();
  vmo.visitable();
  vmo.visitable();
  vmo.visitable();

  vmo.activate();

  MOCKPP_ASSERT_EQUALS(321, vmo.visitable());

#ifndef MOCKPP_NO_EXCEPTIONS
  MOCKPP_ASSERT_THROWING(vmo.visitable(),
                         MOCKPP_PCHAR("vmo.visitable()-1a"),
                         unsigned,
                         unsigned(123));
#endif

  MOCKPP_ASSERT_EQUALS(432, vmo.visitable());

#ifndef MOCKPP_NO_EXCEPTIONS
  MOCKPP_ASSERT_THROWING(vmo.visitable(),
                         MOCKPP_PCHAR("vmo.visitable()-1b"),
                         unsigned,
                         unsigned(321));
#endif

  MOCKPP_ASSERT_EQUALS(123, vmo.visitable());

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

  vmo.visitable();
  vmo.visitable();
  vmo.visitable();
  vmo.visitable();
  vmo.visitable();

  vmo.activate();

#ifndef MOCKPP_NO_EXCEPTIONS
  MOCKPP_ASSERT_THROWING(vmo.visitable(),
                         MOCKPP_PCHAR("vmo.visitable()-2a"),
                         unsigned,
                         unsigned(123));

  MOCKPP_ASSERT_THROWING(vmo.visitable(),
                         MOCKPP_PCHAR("vmo.visitable()-2b"),
                         unsigned,
                         unsigned(321));
#endif

  MOCKPP_ASSERT_EQUALS(321, vmo.visitable());
  MOCKPP_ASSERT_EQUALS(432, vmo.visitable());
  MOCKPP_ASSERT_EQUALS(123, vmo.visitable());

  vmo.verify();
  vmb.verify();

  //////////////////////////////////////////////////////

  vmo.reset();
  vmb.setDefaultThrowable(MOCKPP_NS::make_throwable(111u));
  vmb.addThrowable(MOCKPP_NS::make_throwable(123u));

  vmo.visitable();
  vmo.visitable();
  vmo.visitable();

  vmo.activate();

#ifndef MOCKPP_NO_EXCEPTIONS
  MOCKPP_ASSERT_THROWING(vmo.visitable(),
                         MOCKPP_PCHAR("vmo.visitable()-3a"),
                         unsigned,
                         unsigned(123));

  MOCKPP_ASSERT_THROWING(vmo.visitable(),
                         MOCKPP_PCHAR("vmo.visitable()-3b"),
                         unsigned,
                         unsigned(111));

  MOCKPP_ASSERT_THROWING(vmo.visitable(),
                         MOCKPP_PCHAR("vmo.visitable()-3c"),
                         unsigned,
                         unsigned(111));
#endif

  vmo.verify();
  vmb.verify();
}


void VisitableMockMethod_test::test_parameter_0v()
{
  MyVisitableMockObject_0 vmo(MOCKPP_PCHAR("Visitable-object"));
  MOCKPP_NS::VisitableMockMethod0<void> &vmb (vmo.visitablev_mocker);
  MOCKPP_NS::VisitableMockMethod0<int> &vmbi (vmo.visitable_mocker);

  vmb.setDefaultThrowable(MOCKPP_NS::make_throwable(444u));
  vmb.addThrowable(MOCKPP_NS::make_throwable(333u));
  vmo.reset();

  vmbi.setDefaultReturnValue(999);

  //////////////////////////////////////////////////////

  vmb.addThrowable(MOCKPP_NS::make_throwable(123u));
  vmb.addThrowable(MOCKPP_NS::make_throwable(321u));

  vmo.visitablev();
  vmo.visitablev();
  vmo.visitablev();
  vmo.visitable();
  vmo.visitable();
  vmo.visitablev();

  vmo.activate();

#ifndef MOCKPP_NO_EXCEPTIONS
  MOCKPP_ASSERT_THROWING(vmo.visitablev(),
                         MOCKPP_PCHAR("vmo.visitablev()-1a"),
                         unsigned,
                         unsigned(123));

  MOCKPP_ASSERT_THROWING(vmo.visitablev(),
                         MOCKPP_PCHAR("vmo.visitablev()-1b"),
                         unsigned,
                         unsigned(321));
#endif

  vmo.visitablev();
  vmo.visitable();
  vmo.visitable();
  vmo.visitablev();

  vmo.verify();
  vmb.verify();

  //////////////////////////////////////////////////////

  vmo.reset();
  vmb.unsetThrowablesInline();

  vmb.addThrowable(MOCKPP_NS::make_throwable(123u));
  vmb.addThrowable(MOCKPP_NS::make_throwable(321u));

  vmo.visitablev();
  vmo.visitablev();
  vmo.visitablev();
  vmo.visitablev();
  vmo.visitablev();

  vmo.activate();

#ifndef MOCKPP_NO_EXCEPTIONS
  MOCKPP_ASSERT_THROWING(vmo.visitablev(),
                         MOCKPP_PCHAR("vmo.visitablev()-2a"),
                         unsigned,
                         unsigned(123));

  MOCKPP_ASSERT_THROWING(vmo.visitablev(),
                         MOCKPP_PCHAR("vmo.visitablev()-2b"),
                         unsigned,
                         unsigned(321));
#endif

  vmo.visitablev();
  vmo.visitablev();
  vmo.visitablev();

  vmo.verify();
  vmb.verify();

  //////////////////////////////////////////////////////

  vmo.reset();
  vmb.setDefaultThrowable(MOCKPP_NS::make_throwable(111u));
  vmb.addThrowable(MOCKPP_NS::make_throwable(123u));

  vmo.visitablev();
  vmo.visitablev();
  vmo.visitablev();
  vmo.activate();

#ifndef MOCKPP_NO_EXCEPTIONS
  MOCKPP_ASSERT_THROWING(vmo.visitablev(),
                         MOCKPP_PCHAR("vmo.visitablev()-3a"),
                         unsigned,
                         unsigned(123));

  MOCKPP_ASSERT_THROWING(vmo.visitablev(),
                         MOCKPP_PCHAR("vmo.visitablev()-3b"),
                         unsigned,
                         unsigned(111));

  MOCKPP_ASSERT_THROWING(vmo.visitablev(),
                         MOCKPP_PCHAR("vmo.visitablev()-3c"),
                         unsigned,
                         unsigned(111));
#endif

  vmo.verify();
  vmb.verify();
}


MOCKPP_NS_END




