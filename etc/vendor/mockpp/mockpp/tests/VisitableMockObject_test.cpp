/***************************************************************************
                     VisitableMockObject_test.cpp
                                 -
  unit tests for VisitableMockObject class and 0 parameter methods
                             -------------------
    begin                : Sam Feb 8 2003
    copyright            : (C) 2002-2008 by Ewald Arnold
    email                : mockpp at ewald-arnold dot de

  $Id: VisitableMockObject_test.cpp 1438 2008-03-08 20:22:43Z ewald-arnold $

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


class VisitableMockObject_test MOCKPP_DERIVE_PUBLIC_UNITFRAMEWORK
{
 public:

#if defined (MOCKPP_USE_CXXTEST)

#elif defined(MOCKPP_USE_BOOSTTEST)

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE( VisitableMockObject_test );

  CPPUNIT_TEST(test_controller);
  CPPUNIT_TEST(test_thrower);
  CPPUNIT_TEST(test_return);
  CPPUNIT_TEST(test_clear);
  CPPUNIT_TEST(test_parameter_ex);
  CPPUNIT_TEST(test_inline_pre_1_2);
  CPPUNIT_TEST(test_inline_post_1_2);
  CPPUNIT_TEST(test_method_unused);

  CPPUNIT_TEST(fail_add_throw_after_active);
  CPPUNIT_TEST(fail_add_throw_val_after_active);
  CPPUNIT_TEST(fail_set_throw_after_active);
  CPPUNIT_TEST(fail_add_method_after_active);
  CPPUNIT_TEST(fail_add_return_after_active);
  CPPUNIT_TEST(fail_set_return_after_active);
  CPPUNIT_TEST(fail_return);
  CPPUNIT_TEST(fail_unused_throwable);
  CPPUNIT_TEST(fail_unused_value);
  CPPUNIT_TEST(test_stub_return_const_ref);

CPPUNIT_TEST_SUITE_END();

#endif

 public:

  MOCKPP_TESTMETHOD_DECL void test_controller();
  MOCKPP_TESTMETHOD_DECL void test_thrower();
  MOCKPP_TESTMETHOD_DECL void test_return();
  MOCKPP_TESTMETHOD_DECL void test_clear();
  MOCKPP_TESTMETHOD_DECL void test_parameter_ex();
  MOCKPP_TESTMETHOD_DECL void test_inline_pre_1_2();
  MOCKPP_TESTMETHOD_DECL void test_inline_post_1_2();
  MOCKPP_TESTMETHOD_DECL void test_method_unused();

  MOCKPP_TESTMETHOD_DECL void fail_unused_value();
  MOCKPP_TESTMETHOD_DECL void fail_unused_throwable();
  MOCKPP_TESTMETHOD_DECL void fail_return();
  MOCKPP_TESTMETHOD_DECL void fail_add_method_after_active();
  MOCKPP_TESTMETHOD_DECL void fail_add_throw_val_after_active();
  MOCKPP_TESTMETHOD_DECL void fail_add_throw_after_active();
  MOCKPP_TESTMETHOD_DECL void fail_set_throw_after_active();
  MOCKPP_TESTMETHOD_DECL void fail_add_return_after_active();
  MOCKPP_TESTMETHOD_DECL void fail_set_return_after_active();
  MOCKPP_TESTMETHOD_DECL void test_stub_return_const_ref();
};


#if defined (MOCKPP_USE_CXXTEST)

MOCKPP_CXXTEST_SUITE_REGISTRATION( VisitableMockObject_test );

MOCKPP_CXXTEST(VisitableMockObject_test, test_controller);
MOCKPP_CXXTEST(VisitableMockObject_test, test_thrower);
MOCKPP_CXXTEST(VisitableMockObject_test, test_return);
MOCKPP_CXXTEST(VisitableMockObject_test, test_clear);
MOCKPP_CXXTEST(VisitableMockObject_test, test_parameter_ex);
MOCKPP_CXXTEST(VisitableMockObject_test, test_inline_pre_1_2);
MOCKPP_CXXTEST(VisitableMockObject_test, test_inline_post_1_2);
MOCKPP_CXXTEST(VisitableMockObject_test, test_method_unused);

MOCKPP_CXXTEST(VisitableMockObject_test, fail_add_throw_after_active);
MOCKPP_CXXTEST(VisitableMockObject_test, fail_add_throw_val_after_active);
MOCKPP_CXXTEST(VisitableMockObject_test, fail_set_throw_after_active);
MOCKPP_CXXTEST(VisitableMockObject_test, fail_add_method_after_active);
MOCKPP_CXXTEST(VisitableMockObject_test, fail_add_return_after_active);
MOCKPP_CXXTEST(VisitableMockObject_test, fail_set_return_after_active);
MOCKPP_CXXTEST(VisitableMockObject_test, fail_return);
MOCKPP_CXXTEST(VisitableMockObject_test, fail_unused_throwable);
MOCKPP_CXXTEST(VisitableMockObject_test, fail_unused_value);
MOCKPP_CXXTEST(VisitableMockObject_test, test_stub_return_const_ref);

#elif defined(MOCKPP_USE_BOOSTTEST)

MOCKPP_BOOST_TEST(VisitableMockObject_test::test_controller);
MOCKPP_BOOST_TEST(VisitableMockObject_test::test_thrower);
MOCKPP_BOOST_TEST(VisitableMockObject_test::test_return);
MOCKPP_BOOST_TEST(VisitableMockObject_test::test_clear);
MOCKPP_BOOST_TEST(VisitableMockObject_test::test_parameter_ex);
MOCKPP_BOOST_TEST(VisitableMockObject_test::test_inline_pre_1_2);
MOCKPP_BOOST_TEST(VisitableMockObject_test::test_inline_post_1_2);
MOCKPP_BOOST_TEST(VisitableMockObject_test::test_method_unused);

MOCKPP_BOOST_TEST(VisitableMockObject_test::fail_add_throw_after_active);
MOCKPP_BOOST_TEST(VisitableMockObject_test::fail_add_throw_val_after_active);
MOCKPP_BOOST_TEST(VisitableMockObject_test::fail_set_throw_after_active);
MOCKPP_BOOST_TEST(VisitableMockObject_test::fail_add_method_after_active);
MOCKPP_BOOST_TEST(VisitableMockObject_test::fail_add_return_after_active);
MOCKPP_BOOST_TEST(VisitableMockObject_test::fail_set_return_after_active);
MOCKPP_BOOST_TEST(VisitableMockObject_test::fail_return);
MOCKPP_BOOST_TEST(VisitableMockObject_test::fail_unused_throwable);
MOCKPP_BOOST_TEST(VisitableMockObject_test::fail_unused_value);
MOCKPP_BOOST_TEST(VisitableMockObject_test::test_stub_return_const_ref);

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE_REGISTRATION (VisitableMockObject_test);

#endif // frameworks



class MyVisitableMockObject : public MOCKPP_NS::VisitableMockObject
{
  public:

    MyVisitableMockObject(const MOCKPP_NS::String &name)
      : MOCKPP_NS::VisitableMockObject(name, 0)
      , MOCKPP_CONSTRUCT_MEMBERS_FOR_VISITABLE0(visitable)
      , MOCKPP_CONSTRUCT_MEMBERS_FOR_VISITABLE0(visitable_e_Shadow)
    {}

    MOCKPP_VISITABLE0(MyVisitableMockObject, int, visitable);
    MOCKPP_VISITABLE0(MyVisitableMockObject, int, visitable_e_Shadow);

    int visitable_e_shadow;

    virtual const int& visitable_e()
    {
      visitable_e_shadow = visitable_e_Shadow();
      return visitable_e_shadow;
    }


    bool isClear()
    {
      return    visitableDefaultThrowable.get() == 0  // check preprocessed doc-template.h for the actual members
             && visitableThrowables.size() == 0

             && visitablehaveDefaultReturnValue == false
             && visitableDefaultReturnValueUsed == false
             && visitableReturnValues.hasMoreObjects() == false;
    }
};


class MyVisitableMockObject_ex : public MOCKPP_NS::VisitableMockObject
{
  public:

    MyVisitableMockObject_ex(const MOCKPP_NS::String &name)
      : MOCKPP_NS::VisitableMockObject(name, 0),
        MOCKPP_CONSTRUCT_MEMBERS_FOR_VISITABLE_EXT0(visitable, Ext)
    {}

    MOCKPP_VISITABLE_EXT0(MyVisitableMockObject_ex, const MOCKPP_STL::string, visitable,
                                                    MOCKPP_STL::string,       Ext);

    bool isClear()
    {
      return    visitableExtDefaultThrowable.get() == 0  // check preprocessed doc-template.h for the actual members
             && visitableExtThrowables.size() == 0

             && visitableExthaveDefaultReturnValue == false
             && visitableExtDefaultReturnValueUsed == false
             && visitableExtReturnValues.hasMoreObjects() == false;
    }
};


void VisitableMockObject_test::test_parameter_ex()
{
  MyVisitableMockObject_ex mvo(MOCKPP_PCHAR("mvo"));
  MOCKPP_ASSERT_TRUE(mvo.isClear() == true);

  MOCKPP_CONTROLLER_FOR_EXT(MyVisitableMockObject_ex, visitable, Ext) ctr (&mvo);
  ctr.setDefaultReturnValue("0");

  mvo.visitable();

  mvo.activate();

  MOCKPP_ASSERT_TRUE(MOCKPP_STL::string("0") == mvo.visitable());

  mvo.verify();
}


void VisitableMockObject_test::test_controller()
{
  MyVisitableMockObject mvo(MOCKPP_PCHAR("mvo"));
  MOCKPP_CONTROLLER_FOR(MyVisitableMockObject, visitable) ctr (&mvo);
  ctr.addThrowable(MOCKPP_NS::make_throwable(MOCKPP_STL::string("exceptional string")));
  ctr.addThrowable(MOCKPP_STL::string("exceptional string"), 3);
  ctr.setDefaultThrowable(MOCKPP_NS::make_throwable(int(123)));
  ctr.addReturnValue(11);
  ctr.setDefaultReturnValue(123);
}


void VisitableMockObject_test::test_thrower()
{
  MyVisitableMockObject mvo(MOCKPP_PCHAR("mvo"));
  MOCKPP_CONTROLLER_FOR(MyVisitableMockObject, visitable) ctr (&mvo);
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


void VisitableMockObject_test::fail_add_method_after_active()
{
  MyVisitableMockObject mvo(MOCKPP_PCHAR("mvo"));
  MOCKPP_CONTROLLER_FOR(MyVisitableMockObject, visitable) ctr (&mvo);
  mvo.visitable();
  mvo.activate();
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


void VisitableMockObject_test::test_method_unused()
{
  MyVisitableMockObject mvo(MOCKPP_PCHAR("mvo"));
  MOCKPP_CONTROLLER_FOR(MyVisitableMockObject, visitable) ctr (&mvo);

  ctr.setDefaultReturnValue(1);
  mvo.visitable();
  mvo.visitable();
  mvo.activate();
  mvo.visitable();

#ifndef MOCKPP_NO_EXCEPTIONS
  MOCKPP_ASSERT_THROWING_COND(mvo.verify(),
                              MOCKPP_PCSTRING("mvo.verify()"),
                              MOCKPP_NS::AssertionFailedError,
                              exception_object.getMessage().find(MOCKPP_PCHAR("did not receive the expected item list")) != MOCKPP_NS::String::npos);
#endif
}


void VisitableMockObject_test::fail_add_throw_after_active()
{
  MyVisitableMockObject mvo(MOCKPP_PCHAR("mvo"));
  MOCKPP_CONTROLLER_FOR(MyVisitableMockObject, visitable) ctr (&mvo);
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


void VisitableMockObject_test::fail_add_throw_val_after_active()
{
  MyVisitableMockObject mvo(MOCKPP_PCHAR("mvo"));
  MOCKPP_CONTROLLER_FOR(MyVisitableMockObject, visitable) ctr (&mvo);
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


void VisitableMockObject_test::fail_set_throw_after_active()
{
  MyVisitableMockObject mvo(MOCKPP_PCHAR("mvo"));
  MOCKPP_CONTROLLER_FOR(MyVisitableMockObject, visitable) ctr (&mvo);
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


void VisitableMockObject_test::fail_add_return_after_active()
{
  MyVisitableMockObject mvo(MOCKPP_PCHAR("mvo"));
  MOCKPP_CONTROLLER_FOR(MyVisitableMockObject, visitable) ctr (&mvo);
  mvo.activate();
  bool thrown = false;
  MOCKPP_TRY
  {
    ctr.addReturnValue(1);
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_NS::AssertionFailedError &)
  {
    thrown = true;
  }
#endif
  MOCKPP_ASSERT_TRUE(thrown == true);
}


void VisitableMockObject_test::fail_set_return_after_active()
{
  MyVisitableMockObject mvo(MOCKPP_PCHAR("mvo"));
  MOCKPP_CONTROLLER_FOR(MyVisitableMockObject, visitable) ctr (&mvo);
  mvo.activate();
  bool thrown = false;
  MOCKPP_TRY
  {
    ctr.setDefaultReturnValue(1);
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_NS::AssertionFailedError &)
  {
    thrown = true;
  }
#endif
  MOCKPP_ASSERT_TRUE(thrown == true);
}


void VisitableMockObject_test::fail_return()
{
  MyVisitableMockObject mvo(MOCKPP_PCHAR("mvo"));
  MOCKPP_CONTROLLER_FOR(MyVisitableMockObject, visitable) ctr (&mvo);

  ctr.addReturnValue(1);
  ctr.addReturnValue(11);

  mvo.visitable();
  mvo.visitable();
  mvo.visitable();

  mvo.activate();

  MOCKPP_ASSERT_TRUE(1 == mvo.visitable());
  MOCKPP_ASSERT_TRUE(11 == mvo.visitable());
  bool thrown = false;
  MOCKPP_TRY
  {
    mvo.visitable(); // no more return objects
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_NS::AssertionFailedError &)
  {
    thrown = true;
  }
#endif
  MOCKPP_ASSERT_TRUE(thrown == true);
}


void VisitableMockObject_test::test_return()
{
  MyVisitableMockObject mvo(MOCKPP_PCHAR("mvo"));
  MOCKPP_CONTROLLER_FOR(MyVisitableMockObject, visitable) ctr (&mvo);

  ctr.setDefaultReturnValue(123);
  ctr.addReturnValue(1);
  ctr.addReturnValue(11);

  mvo.visitable();
  mvo.visitable();
  mvo.visitable();
  mvo.visitable();

  mvo.activate();

  MOCKPP_ASSERT_TRUE(1 == mvo.visitable());
  MOCKPP_ASSERT_TRUE(11 == mvo.visitable());
  MOCKPP_ASSERT_TRUE(123 == mvo.visitable());
  MOCKPP_ASSERT_TRUE(123 == mvo.visitable());
  mvo.verify();
}


void VisitableMockObject_test::test_clear()
{
  MyVisitableMockObject mvo(MOCKPP_PCHAR("mvo"));
  MOCKPP_ASSERT_TRUE(mvo.isClear() == true);

  MOCKPP_CONTROLLER_FOR(MyVisitableMockObject, visitable) ctr (&mvo);
  ctr.addThrowable(MOCKPP_STL::string("exceptional string"), 3);
  ctr.setDefaultThrowable(MOCKPP_NS::make_throwable(int(123)));

  ctr.addReturnValue(1);
  ctr.setDefaultReturnValue(123);

  mvo.activate();

  mvo.reset();
  MOCKPP_ASSERT_TRUE(mvo.isClear() == true);
}


void VisitableMockObject_test::fail_unused_throwable()
{
  MyVisitableMockObject mvo(MOCKPP_PCHAR("mvo"));
  MOCKPP_CONTROLLER_FOR(MyVisitableMockObject, visitable) ctr (&mvo);
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


void VisitableMockObject_test::fail_unused_value()
{
  MyVisitableMockObject mvo(MOCKPP_PCHAR("mvo"));
  MOCKPP_CONTROLLER_FOR(MyVisitableMockObject, visitable) ctr (&mvo);
  ctr.setDefaultReturnValue(111);
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


void VisitableMockObject_test::test_inline_pre_1_2()
{
  MyVisitableMockObject mvo(MOCKPP_PCHAR("mvo"));
  MOCKPP_CONTROLLER_FOR(MyVisitableMockObject, visitable) ctr (&mvo);
  ctr.addReturnValue(111);
  ctr.addThrowable(MOCKPP_NS::make_throwable(MOCKPP_STL::string("throw 1")));
  ctr.addReturnValue(222);
  ctr.addThrowable(MOCKPP_NS::make_throwable(MOCKPP_STL::string("throw 2")));

  mvo.unsetThrowablesInline(); // old pre-1.2 mode
  mvo.activate();

  MOCKPP_TRY
  {
    mvo.visitable();
    MOCKPP_FAIL(MOCKPP_PCHAR("should have thrown here"));
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_STL::string &ex)
  {
    MOCKPP_ASSERT_EQUALS(ex, MOCKPP_STL::string("throw 1"));
  }
#endif

  MOCKPP_TRY
  {
    mvo.visitable();
    MOCKPP_FAIL(MOCKPP_PCHAR("should have thrown here"));
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_STL::string &ex)
  {
    MOCKPP_ASSERT_EQUALS(ex, MOCKPP_STL::string("throw 2"));
  }
#endif

  MOCKPP_ASSERT_TRUE(111 == mvo.visitable());
  MOCKPP_ASSERT_TRUE(222 == mvo.visitable());

  mvo.verify();
}


void VisitableMockObject_test::test_inline_post_1_2()
{
  MyVisitableMockObject mvo(MOCKPP_PCHAR("mvo"));
  MOCKPP_CONTROLLER_FOR(MyVisitableMockObject, visitable) ctr (&mvo);
  ctr.addReturnValue(111);
  ctr.addThrowable(MOCKPP_NS::make_throwable(MOCKPP_STL::string("throw 1")));
  ctr.addReturnValue(222);
  ctr.addThrowable(MOCKPP_NS::make_throwable(MOCKPP_STL::string("throw 2")));
  mvo.activate();

  MOCKPP_ASSERT_TRUE(111 == mvo.visitable());

  MOCKPP_TRY
  {
    mvo.visitable();
    MOCKPP_FAIL(MOCKPP_PCHAR("should have thrown here"));
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_STL::string &ex)
  {
    MOCKPP_ASSERT_EQUALS(ex, MOCKPP_STL::string("throw 1"));
  }

  MOCKPP_ASSERT_TRUE(222 == mvo.visitable());
#endif

  MOCKPP_TRY
  {
    mvo.visitable();
    MOCKPP_FAIL(MOCKPP_PCHAR("should have thrown here"));
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_STL::string &ex)
  {
    MOCKPP_ASSERT_EQUALS(ex, MOCKPP_STL::string("throw 2"));
  }
#endif

  mvo.verify();
}


void VisitableMockObject_test::test_stub_return_const_ref()
{
  MyVisitableMockObject mvo(MOCKPP_PCHAR("mvo"));
  MOCKPP_CONTROLLER_FOR(MyVisitableMockObject, visitable_e_Shadow) ctr (&mvo);

  ctr.addReturnValue(321);
  ctr.addReturnValue(123);
  mvo.activate();

  MOCKPP_ASSERT_EQUALS(321, mvo.visitable_e_Shadow());
  MOCKPP_ASSERT_EQUALS(123, mvo.visitable_e());

  mvo.verify();
}




