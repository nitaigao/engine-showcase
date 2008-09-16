/***************************************************************************
                  VisitableMockObject_void_test.cpp
                                 -
  unit tests for VisitableMockObject class and 0 parameter void methods

                             -------------------

    begin                : Sam Feb 8 2003
    copyright            : (C) 2002-2008 by Ewald Arnold
    email                : mockpp at ewald-arnold dot de

  $Id: VisitableMockObject_void_test.cpp 1438 2008-03-08 20:22:43Z ewald-arnold $

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

#include <mockpp/ExpectationValue.h>
#include <mockpp/visiting/VisitableMockObject.h>


class VisitableMockObject_void_test MOCKPP_DERIVE_PUBLIC_UNITFRAMEWORK
{
 public:

#if defined (MOCKPP_USE_CXXTEST)

#elif defined(MOCKPP_USE_BOOSTTEST)

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE( VisitableMockObject_void_test );

  CPPUNIT_TEST(test_controller);
  CPPUNIT_TEST(test_thrower);
  CPPUNIT_TEST(test_clear);
  CPPUNIT_TEST(test_parameter_ex);

  CPPUNIT_TEST(fail_add_throw_after_active);
  CPPUNIT_TEST(fail_add_throw_val_after_active);
  CPPUNIT_TEST(fail_set_throw_after_active);
  CPPUNIT_TEST(fail_add_method_after_active);
  CPPUNIT_TEST(fail_unused_throwable);

CPPUNIT_TEST_SUITE_END();

#endif

 public:

  MOCKPP_TESTMETHOD_DECL void test_controller();
  MOCKPP_TESTMETHOD_DECL void test_thrower();
  MOCKPP_TESTMETHOD_DECL void test_clear();
  MOCKPP_TESTMETHOD_DECL void test_parameter_ex();

  MOCKPP_TESTMETHOD_DECL void fail_unused_throwable();
  MOCKPP_TESTMETHOD_DECL void fail_add_method_after_active();
  MOCKPP_TESTMETHOD_DECL void fail_add_throw_val_after_active();
  MOCKPP_TESTMETHOD_DECL void fail_add_throw_after_active();
  MOCKPP_TESTMETHOD_DECL void fail_set_throw_after_active();
};


#if defined (MOCKPP_USE_CXXTEST)

MOCKPP_CXXTEST_SUITE_REGISTRATION( VisitableMockObject_void_test );

MOCKPP_CXXTEST(VisitableMockObject_void_test, test_controller);
MOCKPP_CXXTEST(VisitableMockObject_void_test, test_thrower);
MOCKPP_CXXTEST(VisitableMockObject_void_test, test_clear);
MOCKPP_CXXTEST(VisitableMockObject_void_test, test_parameter_ex);

MOCKPP_CXXTEST(VisitableMockObject_void_test, fail_add_throw_after_active);
MOCKPP_CXXTEST(VisitableMockObject_void_test, fail_add_throw_val_after_active);
MOCKPP_CXXTEST(VisitableMockObject_void_test, fail_set_throw_after_active);
MOCKPP_CXXTEST(VisitableMockObject_void_test, fail_add_method_after_active);
MOCKPP_CXXTEST(VisitableMockObject_void_test, fail_unused_throwable);

#elif defined(MOCKPP_USE_BOOSTTEST)

MOCKPP_BOOST_TEST(VisitableMockObject_void_test::test_controller);
MOCKPP_BOOST_TEST(VisitableMockObject_void_test::test_thrower);
MOCKPP_BOOST_TEST(VisitableMockObject_void_test::test_clear);
MOCKPP_BOOST_TEST(VisitableMockObject_void_test::test_parameter_ex);

MOCKPP_BOOST_TEST(VisitableMockObject_void_test::fail_add_throw_after_active);
MOCKPP_BOOST_TEST(VisitableMockObject_void_test::fail_add_throw_val_after_active);
MOCKPP_BOOST_TEST(VisitableMockObject_void_test::fail_set_throw_after_active);
MOCKPP_BOOST_TEST(VisitableMockObject_void_test::fail_add_method_after_active);
MOCKPP_BOOST_TEST(VisitableMockObject_void_test::fail_unused_throwable);

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE_REGISTRATION (VisitableMockObject_void_test);

#endif // frameworks



class MyVisitableMockObject_void : public MOCKPP_NS::VisitableMockObject
{
  public:

    MyVisitableMockObject_void(const MOCKPP_NS::String &name)
      : MOCKPP_NS::VisitableMockObject(name, 0),
        MOCKPP_CONSTRUCT_MEMBERS_FOR_VOID_VISITABLE0(visitable)
    {}

    MOCKPP_VOID_VISITABLE0(MyVisitableMockObject_void, visitable);

    bool isClear()
    {
      return    visitableDefaultThrowable.get() == 0  // check preprocessed doc-template.h for the actual members
             && visitableThrowables.size() == 0;
    }
};


class MyVisitableMockObject_void_ex : public MOCKPP_NS::VisitableMockObject
{
  public:

    MyVisitableMockObject_void_ex(const MOCKPP_NS::String &name)
      : MOCKPP_NS::VisitableMockObject(name, 0),
        MOCKPP_CONSTRUCT_MEMBERS_FOR_VOID_VISITABLE_EXT0(visitable0, Ext)
    {}

    MOCKPP_VOID_VISITABLE_EXT0(MyVisitableMockObject_void_ex, visitable0,
                                                              Ext);

    bool isClear()
    {
      return    visitable0ExtDefaultThrowable.get() == 0  // check preprocessed doc-template.h for the actual members
             && visitable0ExtThrowables.size() == 0;
    }
};


void VisitableMockObject_void_test::test_parameter_ex()
{
  MyVisitableMockObject_void_ex mvo(MOCKPP_PCHAR("mvo"));
  MOCKPP_ASSERT_TRUE(mvo.isClear() == true);

  MOCKPP_CONTROLLER_FOR_EXT(MyVisitableMockObject_void_ex, visitable0, Ext) ctr0 (&mvo);

  mvo.visitable0();

  mvo.activate();

  mvo.visitable0();

  mvo.verify();
}


void VisitableMockObject_void_test::test_controller()
{
  MyVisitableMockObject_void mvo(MOCKPP_PCHAR("mvo"));
  MOCKPP_CONTROLLER_FOR(MyVisitableMockObject_void, visitable) ctr (&mvo);
  ctr.addThrowable(MOCKPP_NS::make_throwable(MOCKPP_STL::string("exceptional string")));
  ctr.addThrowable(MOCKPP_STL::string("exceptional string"), 3);
  ctr.setDefaultThrowable(MOCKPP_NS::make_throwable(int(123)));
}


void VisitableMockObject_void_test::test_thrower()
{
  MyVisitableMockObject_void mvo(MOCKPP_PCHAR("mvo"));
  MOCKPP_CONTROLLER_FOR(MyVisitableMockObject_void, visitable) ctr (&mvo);
  ctr.addThrowable(MOCKPP_NS::make_throwable(MOCKPP_STL::string("exceptional string 1")));
  ctr.addThrowable(MOCKPP_STL::string("exceptional string 2"), 2);
  ctr.setDefaultThrowable(MOCKPP_NS::make_throwable(MOCKPP_STL::string("exceptional string 3")));

  mvo.visitable();
  mvo.visitable();
  mvo.visitable();
  mvo.visitable();

  mvo.activate();

#ifndef MOCKPP_NO_EXCEPTIONS
  MOCKPP_ASSERT_THROWING(mvo.visitable(), MOCKPP_PCSTRING("1"),
                         MOCKPP_STL::string, "exceptional string 1");

  MOCKPP_ASSERT_THROWING(mvo.visitable(), MOCKPP_PCSTRING("2"),
                         MOCKPP_STL::string, "exceptional string 2");

  MOCKPP_ASSERT_THROWING(mvo.visitable(), MOCKPP_PCSTRING("3"),
                         MOCKPP_STL::string, "exceptional string 2");

  MOCKPP_ASSERT_THROWING(mvo.visitable(), MOCKPP_PCSTRING("4"),
                         MOCKPP_STL::string, "exceptional string 3");
#endif
  mvo.verify();
}


void VisitableMockObject_void_test::fail_add_method_after_active()
{
  MyVisitableMockObject_void mvo(MOCKPP_PCHAR("mvo"));
  MOCKPP_CONTROLLER_FOR(MyVisitableMockObject_void, visitable) ctr (&mvo);

  mvo.visitable();
  mvo.activate();
  mvo.visitable();
  bool thrown = false;
  MOCKPP_TRY
  {
    mvo.visitable();
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_NS::AssertionFailedError &)
  {
    thrown = true;
  }
#endif
  MOCKPP_ASSERT_TRUE(thrown == true);
}


void VisitableMockObject_void_test::fail_add_throw_after_active()
{
  MyVisitableMockObject_void mvo(MOCKPP_PCHAR("mvo"));
  MOCKPP_CONTROLLER_FOR(MyVisitableMockObject_void, visitable) ctr (&mvo);
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


void VisitableMockObject_void_test::fail_add_throw_val_after_active()
{
  MyVisitableMockObject_void mvo(MOCKPP_PCHAR("mvo"));
  MOCKPP_CONTROLLER_FOR(MyVisitableMockObject_void, visitable) ctr (&mvo);
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


void VisitableMockObject_void_test::fail_set_throw_after_active()
{
  MyVisitableMockObject_void mvo(MOCKPP_PCHAR("mvo"));
  MOCKPP_CONTROLLER_FOR(MyVisitableMockObject_void, visitable) ctr (&mvo);
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


void VisitableMockObject_void_test::test_clear()
{
  MyVisitableMockObject_void mvo(MOCKPP_PCHAR("mvo"));
  MOCKPP_ASSERT_TRUE(mvo.isClear() == true);

  MOCKPP_CONTROLLER_FOR(MyVisitableMockObject_void, visitable) ctr (&mvo);

  ctr.addThrowable(MOCKPP_STL::string("exceptional string"), 3);
  ctr.setDefaultThrowable(MOCKPP_NS::make_throwable(int(123)));

  mvo.activate();

  mvo.reset();
  MOCKPP_ASSERT_TRUE(mvo.isClear() == true);
}


void VisitableMockObject_void_test::fail_unused_throwable()
{
  MyVisitableMockObject_void mvo(MOCKPP_PCHAR("mvo"));
  MOCKPP_CONTROLLER_FOR(MyVisitableMockObject_void, visitable) ctr (&mvo);
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



