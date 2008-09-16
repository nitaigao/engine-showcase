/***************************************************************************
                     VisitableMockObject_2_void_test.cpp
                                 -
  unit tests for VisitableMockObject class and 2 parameter void methods
                             -------------------
    begin                : Die Feb 18 2003
    copyright            : (C) 2002-2008 by Ewald Arnold
    email                : mockpp at ewald-arnold dot de

  $Id: VisitableMockObject_2_void_test.cpp 1438 2008-03-08 20:22:43Z ewald-arnold $

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

#include <mockpp/constraint/IsEqual.h>
#include <mockpp/constraint/IsNot.h>
#include <mockpp/constraint/OutBound.h>

#include <mockpp/chaining/ChainingMockObjectSupport.h>

#include <mockpp/ExpectationValue.h>
#include <mockpp/visiting/VisitableMockObject.h>


class VisitableMockObject_2_void_test MOCKPP_DERIVE_PUBLIC_UNITFRAMEWORK
{
 public:

#if defined (MOCKPP_USE_CXXTEST)

#elif defined(MOCKPP_USE_BOOSTTEST)

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE( VisitableMockObject_2_void_test );

  CPPUNIT_TEST(test_outbound);
  CPPUNIT_TEST(test_controller);
  CPPUNIT_TEST(test_parameter);
  CPPUNIT_TEST(test_parameter_ex);
  CPPUNIT_TEST(test_thrower);
  CPPUNIT_TEST(test_clear);
  CPPUNIT_TEST(test_default_throw);
  CPPUNIT_TEST(test_constraint);
  CPPUNIT_TEST(test_controller_constraint);

  CPPUNIT_TEST(fail_add_throw_after_active);
  CPPUNIT_TEST(fail_add_throw_val_after_active);
  CPPUNIT_TEST(fail_set_throw_after_active);
  CPPUNIT_TEST(fail_add_method_after_active);
  CPPUNIT_TEST(fail_parameter);
  CPPUNIT_TEST(fail_unused_throwable);

CPPUNIT_TEST_SUITE_END();

#endif

 public:

  MOCKPP_TESTMETHOD_DECL void test_outbound();
  MOCKPP_TESTMETHOD_DECL void test_controller();
  MOCKPP_TESTMETHOD_DECL void test_thrower();
  MOCKPP_TESTMETHOD_DECL void test_clear();
  MOCKPP_TESTMETHOD_DECL void test_default_throw();
  MOCKPP_TESTMETHOD_DECL void test_parameter_ex();
  MOCKPP_TESTMETHOD_DECL void test_parameter();
  MOCKPP_TESTMETHOD_DECL void test_constraint();
  MOCKPP_TESTMETHOD_DECL void test_controller_constraint();

  MOCKPP_TESTMETHOD_DECL void fail_unused_throwable();
  MOCKPP_TESTMETHOD_DECL void fail_add_method_after_active();
  MOCKPP_TESTMETHOD_DECL void fail_add_throw_val_after_active();
  MOCKPP_TESTMETHOD_DECL void fail_add_throw_after_active();
  MOCKPP_TESTMETHOD_DECL void fail_set_throw_after_active();
  MOCKPP_TESTMETHOD_DECL void fail_parameter();
};


#if defined (MOCKPP_USE_CXXTEST)

MOCKPP_CXXTEST_SUITE_REGISTRATION( VisitableMockObject_2_void_test );

MOCKPP_CXXTEST(VisitableMockObject_2_void_test, test_outbound);
MOCKPP_CXXTEST(VisitableMockObject_2_void_test, test_controller);
MOCKPP_CXXTEST(VisitableMockObject_2_void_test, test_parameter);
MOCKPP_CXXTEST(VisitableMockObject_2_void_test, test_parameter_ex);
MOCKPP_CXXTEST(VisitableMockObject_2_void_test, test_thrower);
MOCKPP_CXXTEST(VisitableMockObject_2_void_test, test_clear);
MOCKPP_CXXTEST(VisitableMockObject_2_void_test, test_default_throw);
MOCKPP_CXXTEST(VisitableMockObject_2_void_test, test_constraint);
MOCKPP_CXXTEST(VisitableMockObject_2_void_test, test_controller_constraint);

MOCKPP_CXXTEST(VisitableMockObject_2_void_test, fail_add_throw_after_active);
MOCKPP_CXXTEST(VisitableMockObject_2_void_test, fail_add_throw_val_after_active);
MOCKPP_CXXTEST(VisitableMockObject_2_void_test, fail_set_throw_after_active);
MOCKPP_CXXTEST(VisitableMockObject_2_void_test, fail_add_method_after_active);
MOCKPP_CXXTEST(VisitableMockObject_2_void_test, fail_parameter);
MOCKPP_CXXTEST(VisitableMockObject_2_void_test, fail_unused_throwable);

#elif defined(MOCKPP_USE_BOOSTTEST)

MOCKPP_BOOST_TEST(VisitableMockObject_2_void_test::test_outbound);
MOCKPP_BOOST_TEST(VisitableMockObject_2_void_test::test_controller);
MOCKPP_BOOST_TEST(VisitableMockObject_2_void_test::test_parameter);
MOCKPP_BOOST_TEST(VisitableMockObject_2_void_test::test_parameter_ex);
MOCKPP_BOOST_TEST(VisitableMockObject_2_void_test::test_thrower);
MOCKPP_BOOST_TEST(VisitableMockObject_2_void_test::test_clear);
MOCKPP_BOOST_TEST(VisitableMockObject_2_void_test::test_default_throw);
MOCKPP_BOOST_TEST(VisitableMockObject_2_void_test::test_constraint);
MOCKPP_BOOST_TEST(VisitableMockObject_2_void_test::test_controller_constraint);

MOCKPP_BOOST_TEST(VisitableMockObject_2_void_test::fail_add_throw_after_active);
MOCKPP_BOOST_TEST(VisitableMockObject_2_void_test::fail_add_throw_val_after_active);
MOCKPP_BOOST_TEST(VisitableMockObject_2_void_test::fail_set_throw_after_active);
MOCKPP_BOOST_TEST(VisitableMockObject_2_void_test::fail_add_method_after_active);
MOCKPP_BOOST_TEST(VisitableMockObject_2_void_test::fail_parameter);
MOCKPP_BOOST_TEST(VisitableMockObject_2_void_test::fail_unused_throwable);

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE_REGISTRATION (VisitableMockObject_2_void_test);

#endif // frameworks



class MyVisitableMockObject_2_void : public MOCKPP_NS::VisitableMockObject
{
  public:

    MyVisitableMockObject_2_void(const MOCKPP_NS::String &name)
      : MOCKPP_NS::VisitableMockObject(name, 0),
        MOCKPP_CONSTRUCT_MEMBERS_FOR_VOID_VISITABLE2(visitable)
    {}

    MOCKPP_VOID_VISITABLE2(MyVisitableMockObject_2_void, visitable, unsigned, unsigned);

    bool isClear()
    {
      return    visitableDefaultThrowable.get() == 0  // check preprocessed doc-template.h for the actual members
             && visitableThrowables.size() == 0

             && visitableParameter1.size() == 0
             && visitableParameter2.size() == 0;
    }
};


class MyVisitableMockObject_2_void_ex : public MOCKPP_NS::VisitableMockObject
{
  public:

    MyVisitableMockObject_2_void_ex(const MOCKPP_NS::String &name)
      : MOCKPP_NS::VisitableMockObject(name, 0),
        MOCKPP_CONSTRUCT_MEMBERS_FOR_VOID_VISITABLE_EXT2(visitable2, Ext),
        MOCKPP_CONSTRUCT_MEMBERS_FOR_VOID_VISITABLE_EXT1(visitable1, Ext),
        MOCKPP_CONSTRUCT_MEMBERS_FOR_VOID_VISITABLE_EXT0(visitable0, Ext),
        MOCKPP_CONSTRUCT_MEMBERS_FOR_VOID_VISITABLE_EXT2(outbound, Ext)
    {}

    MOCKPP_VOID_VISITABLE_EXT2(MyVisitableMockObject_2_void_ex, visitable2, const MOCKPP_STL::string &, const MOCKPP_STL::string &,
                                                                Ext,        MOCKPP_STL::string,         MOCKPP_STL::string);

    MOCKPP_VOID_VISITABLE_EXT1(MyVisitableMockObject_2_void_ex, visitable1, const MOCKPP_STL::string &,
                                                                Ext,        MOCKPP_STL::string);

    MOCKPP_VOID_VISITABLE_EXT0(MyVisitableMockObject_2_void_ex, visitable0,
                                                                Ext);

    MOCKPP_VOID_VISITABLE_EXT2(MyVisitableMockObject_2_void_ex, outbound, unsigned &, unsigned &,
                                                                Ext,      unsigned,   unsigned);

    bool isClear()
    {
      return    visitable2ExtDefaultThrowable.get() == 0  // check preprocessed doc-template.h for the actual members
             && visitable2ExtThrowables.size() == 0

             && visitable2ExtResponseValues.size() == 0
             && visitable2ExtParameter1.size() == 0
             && visitable2ExtParameter2.size() == 0;
    }
};


void VisitableMockObject_2_void_test::test_controller()
{
  MyVisitableMockObject_2_void mvo(MOCKPP_PCHAR("mvo"));
  MOCKPP_CONTROLLER_FOR(MyVisitableMockObject_2_void, visitable) ctr (&mvo);
  ctr.addThrowable(MOCKPP_NS::make_throwable(MOCKPP_STL::string("exceptional string")));
  ctr.addThrowable(MOCKPP_STL::string("exceptional string"), 3);
  ctr.setDefaultThrowable(MOCKPP_NS::make_throwable(int(123)));
  ctr.addResponseThrowable(MOCKPP_NS::make_throwable(int(1)), 1, 2);
  ctr.addResponseThrowable(MOCKPP_NS::make_throwable(int(0)), 0, 2);
}


void VisitableMockObject_2_void_test::test_thrower()
{
  MyVisitableMockObject_2_void mvo(MOCKPP_PCHAR("mvo"));
  MOCKPP_CONTROLLER_FOR(MyVisitableMockObject_2_void, visitable) ctr (&mvo);
  ctr.addThrowable(MOCKPP_NS::make_throwable(MOCKPP_STL::string("exceptional string 1")));
  ctr.addThrowable(MOCKPP_STL::string("exceptional string 2"), 2);

  mvo.visitable(2, 2);
  mvo.visitable(22, 2);
  mvo.visitable(222, 2);
  mvo.visitable(2222, 2);
  mvo.visitable(22222, 2);
  mvo.visitable(222222, 2);

  mvo.activate();

#ifndef MOCKPP_NO_EXCEPTIONS
  MOCKPP_ASSERT_THROWING(mvo.visitable(2, 2), MOCKPP_PCSTRING("1"),
                         MOCKPP_STL::string, "exceptional string 1");

  MOCKPP_ASSERT_THROWING(mvo.visitable(22, 2), MOCKPP_PCSTRING("2"),
                         MOCKPP_STL::string, "exceptional string 2");

  MOCKPP_ASSERT_THROWING(mvo.visitable(222, 2), MOCKPP_PCSTRING("3"),
                         MOCKPP_STL::string, "exceptional string 2");
#endif
  mvo.visitable(2222, 2);
  mvo.visitable(22222, 2);
  mvo.visitable(222222, 2);
  mvo.verify();
}


void VisitableMockObject_2_void_test::fail_add_method_after_active()
{
  MyVisitableMockObject_2_void mvo(MOCKPP_PCHAR("mvo"));
  MOCKPP_CONTROLLER_FOR(MyVisitableMockObject_2_void, visitable) ctr (&mvo);
  mvo.visitable(1, 2);
  mvo.activate();
  mvo.visitable(1, 2);
  bool thrown = false;
  MOCKPP_TRY
  {
    mvo.visitable(1, 2);
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_NS::AssertionFailedError &)
  {
    thrown = true;
  }
#endif
  MOCKPP_ASSERT_TRUE(thrown == true);
}


void VisitableMockObject_2_void_test::fail_add_throw_after_active()
{
  MyVisitableMockObject_2_void mvo(MOCKPP_PCHAR("mvo"));
  MOCKPP_CONTROLLER_FOR(MyVisitableMockObject_2_void, visitable) ctr (&mvo);
  mvo.activate();
  bool thrown = false;
  MOCKPP_TRY
  {
    ctr.addThrowable(MOCKPP_NS::make_throwable(MOCKPP_STL::string("exceptional string")));
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_NS::AssertionFailedError &)
  {
    thrown = true;
  }
#endif
  MOCKPP_ASSERT_TRUE(thrown == true);
}


void VisitableMockObject_2_void_test::fail_add_throw_val_after_active()
{
  MyVisitableMockObject_2_void mvo(MOCKPP_PCHAR("mvo"));
  MOCKPP_CONTROLLER_FOR(MyVisitableMockObject_2_void, visitable) ctr (&mvo);
  mvo.activate();
  bool thrown = false;
  MOCKPP_TRY
  {
    ctr.addThrowable(MOCKPP_STL::string("exceptional string"), 1);
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_NS::AssertionFailedError &)
  {
    thrown = true;
  }
#endif
  MOCKPP_ASSERT_TRUE(thrown == true);
}


void VisitableMockObject_2_void_test::fail_set_throw_after_active()
{
  MyVisitableMockObject_2_void mvo(MOCKPP_PCHAR("mvo"));
  MOCKPP_CONTROLLER_FOR(MyVisitableMockObject_2_void, visitable) ctr (&mvo);
  mvo.activate();
  bool thrown = false;
  MOCKPP_TRY
  {
    ctr.setDefaultThrowable(MOCKPP_NS::make_throwable(MOCKPP_STL::string("exceptional string")));
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_NS::AssertionFailedError &)
  {
    thrown = true;
  }
#endif
  MOCKPP_ASSERT_TRUE(thrown == true);
}


void VisitableMockObject_2_void_test::fail_parameter()
{
  MyVisitableMockObject_2_void mvo(MOCKPP_PCHAR("mvo"));
  MOCKPP_CONTROLLER_FOR(MyVisitableMockObject_2_void, visitable) ctr (&mvo);
  mvo.visitable(1, 2);
  mvo.visitable(11, 2);
  mvo.visitable(111, 2);
  mvo.activate();
  mvo.visitable(1, 2);
  mvo.visitable(11, 2);
  bool thrown = false;
  MOCKPP_TRY
  {
    mvo.visitable(222, 2);
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_NS::AssertionFailedError &)
  {
    thrown = true;
  }
#endif
  MOCKPP_ASSERT_TRUE(thrown == true);
}


void VisitableMockObject_2_void_test::test_parameter_ex()
{
  MyVisitableMockObject_2_void_ex mvo(MOCKPP_PCHAR("mvo"));
  MOCKPP_ASSERT_TRUE(mvo.isClear() == true);

  MOCKPP_CONTROLLER_FOR_EXT(MyVisitableMockObject_2_void_ex, visitable2, Ext) ctr2 (&mvo);
  MOCKPP_CONTROLLER_FOR_EXT(MyVisitableMockObject_2_void_ex, visitable1, Ext) ctr1 (&mvo);
  MOCKPP_CONTROLLER_FOR_EXT(MyVisitableMockObject_2_void_ex, visitable0, Ext) ctr0 (&mvo);

  mvo.visitable2("3", "4");
  mvo.visitable1("4");
  mvo.visitable0();

  mvo.activate();

  mvo.visitable2("3", "4");
  mvo.visitable1("4");
  mvo.visitable0();

  mvo.verify();
}


void VisitableMockObject_2_void_test::test_parameter()
{
  MyVisitableMockObject_2_void mvo(MOCKPP_PCHAR("mvo"));
  MOCKPP_CONTROLLER_FOR(MyVisitableMockObject_2_void, visitable) ctr (&mvo);
  mvo.visitable(1, 2);
  mvo.visitable(11, 2);
  mvo.visitable(111, 2);
  mvo.activate();
  mvo.visitable(1, 2);
  mvo.visitable(11, 2);
  mvo.visitable(111, 2);
  mvo.verify();
}


void VisitableMockObject_2_void_test::test_default_throw()
{
  MyVisitableMockObject_2_void mvo(MOCKPP_PCHAR("mvo"));
  MOCKPP_CONTROLLER_FOR(MyVisitableMockObject_2_void, visitable) ctr (&mvo);
  ctr.addResponseThrowable(MOCKPP_NS::make_throwable(MOCKPP_STL::string("throw me 2")), 922, 2);
  ctr.addResponseThrowable(MOCKPP_NS::make_throwable(MOCKPP_STL::string("throw me 1")), 911, 2);
  ctr.setDefaultThrowable(MOCKPP_NS::make_throwable(MOCKPP_STL::string("throw default")));

  mvo.visitable(922, 2);
  mvo.visitable(911, 2);
  mvo.visitable(1, 2);
  mvo.visitable(2, 2);

  mvo.activate();

#ifndef MOCKPP_NO_EXCEPTIONS
  MOCKPP_ASSERT_THROWING(mvo.visitable(922, 2), MOCKPP_PCHAR("response 1"),
                         MOCKPP_STL::string, MOCKPP_STL::string("throw me 2"));

  MOCKPP_ASSERT_THROWING(mvo.visitable(911, 2), MOCKPP_PCHAR("response 2"),
                         MOCKPP_STL::string, MOCKPP_STL::string("throw me 1"));

  MOCKPP_ASSERT_THROWING(mvo.visitable(1, 2), MOCKPP_PCHAR("default response"),
                         MOCKPP_STL::string, MOCKPP_STL::string("throw default"));

  MOCKPP_ASSERT_THROWING(mvo.visitable(2, 2), MOCKPP_PCHAR("default response"),
                         MOCKPP_STL::string, MOCKPP_STL::string("throw default"));
#endif
  mvo.verify();
}


void VisitableMockObject_2_void_test::test_clear()
{
  MyVisitableMockObject_2_void mvo(MOCKPP_PCHAR("mvo"));
  MOCKPP_ASSERT_TRUE(mvo.isClear() == true);

  MOCKPP_CONTROLLER_FOR(MyVisitableMockObject_2_void, visitable) ctr (&mvo);

  ctr.addThrowable(MOCKPP_STL::string("exceptional string"), 3);
  ctr.setDefaultThrowable(MOCKPP_NS::make_throwable(int(123)));

  ctr.addResponseThrowable(MOCKPP_NS::make_throwable(int(1)), 0, 2);
  mvo.visitable(3, 2);
  mvo.activate();

  mvo.reset();
  MOCKPP_ASSERT_TRUE(mvo.isClear() == true);
}


void VisitableMockObject_2_void_test::fail_unused_throwable()
{
  MyVisitableMockObject_2_void mvo(MOCKPP_PCHAR("mvo"));
  MOCKPP_CONTROLLER_FOR(MyVisitableMockObject_2_void, visitable) ctr (&mvo);
  ctr.setDefaultThrowable(MOCKPP_NS::make_throwable(MOCKPP_STL::string("throw default")));
  mvo.activate();
  bool thrown = false;
  MOCKPP_TRY
  {
    mvo.verify();
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_NS::AssertionFailedError &)
  {
    thrown = true;
  }
#endif
  MOCKPP_ASSERT_TRUE(thrown == true);
}


void VisitableMockObject_2_void_test::test_constraint()
{
  MyVisitableMockObject_2_void mvo(MOCKPP_PCHAR("mvo"));
  MOCKPP_CONTROLLER_FOR(MyVisitableMockObject_2_void, visitable) ctr (&mvo);

  mvo.visitable(new MOCKPP_NS::IsEqual<unsigned>(12),
                new MOCKPP_NS::IsEqual<unsigned>(90));

  mvo.activate();
  mvo.visitable(12, 90);
  mvo.verify();
}


void VisitableMockObject_2_void_test::test_controller_constraint()
{
  MyVisitableMockObject_2_void mvo(MOCKPP_PCHAR("mvo"));
  MOCKPP_CONTROLLER_FOR(MyVisitableMockObject_2_void, visitable) ctr (&mvo);

  mvo.visitable(MOCKPP_NS::eq<unsigned>(10, 5),
                MOCKPP_NS::eq<unsigned>(50, 5));

  mvo.visitable(MOCKPP_NS::eq<unsigned>(10, 5),
                MOCKPP_NS::eq<unsigned>(50, 5));

  mvo.visitable(1, 1);

  ctr.addResponseThrowable(MOCKPP_NS::make_throwable((int)2),
                           MOCKPP_NS::eq<unsigned>(8),
                           MOCKPP_NS::eq<unsigned>(48));

  ctr.addResponseThrowable(MOCKPP_NS::make_throwable((int)1),
                           MOCKPP_NS::eq<unsigned>(12),
                           MOCKPP_NS::eq<unsigned>(52));

  mvo.activate();

#ifndef MOCKPP_NO_EXCEPTIONS
  MOCKPP_ASSERT_THROWING(mvo.visitable(8, 48),
                         MOCKPP_PCHAR("mvo.visitable(8, 48)"),
                         int,
                         2);

  MOCKPP_ASSERT_THROWING(mvo.visitable(12, 52),
                         MOCKPP_PCHAR("mvo.visitable(12, 52)"),
                         int,
                         1);
#endif
  mvo.visitable(1, 1);
  mvo.verify();
}


void VisitableMockObject_2_void_test::test_outbound()
{
  MyVisitableMockObject_2_void_ex mvo(MOCKPP_PCHAR("mvo"));
  MOCKPP_CONTROLLER_FOR_EXT(MyVisitableMockObject_2_void_ex, outbound, Ext) ctr1 (&mvo);

  mvo.outbound(MOCKPP_NS::outBound<unsigned>(1123),
               MOCKPP_NS::outBound<unsigned>(2123));
  mvo.activate();

  unsigned val1 = 0;
  unsigned val2 = 0;
  mvo.outbound(val1,val2);
  MOCKPP_ASSERT_EQUALS((unsigned)1123, val1);
  MOCKPP_ASSERT_EQUALS((unsigned)2123, val2);
}



